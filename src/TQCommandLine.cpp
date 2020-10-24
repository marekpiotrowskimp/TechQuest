
#include "TQCommandLine.h"

#include <utility>


std::string TQCommandLine::getfile(std::string fileName) {
    std::string path = "./images/" + fileName;
    std::ifstream t(path);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);
    return buffer;
}

TQCommandLine::TQCommandLine(std::function<void(std::basic_stringstream<char>&)> syncData) : syncData(std::move(syncData)) {
    tqGameData->load();
}

std::vector<gameType::Place>::iterator TQCommandLine::getPlaceIter(int placeIndex) const {
    return find_if(tqGameData->gameData.places.begin(), tqGameData->gameData.places.end(), [&placeIndex](const gameType::Place& obj) {return obj.id == placeIndex;});
}

std::vector<gameType::GameDataAction>::iterator TQCommandLine::getActionIter(int actionIndex) const {
    return find_if(tqGameData->gameData.actions.begin(), tqGameData->gameData.actions.end(), [&actionIndex](const gameType::GameDataAction& obj){ return obj.id == actionIndex;});
}

std::vector<gameType::PlayerAttribute>::iterator TQCommandLine::getPlayerAttributesIter(int attributeIndex) const {
    return find_if(tqGameData->gameData.player.attributes.begin(), tqGameData->gameData.player.attributes.end(), [&attributeIndex](const gameType::PlayerAttribute& obj){ return obj.id == attributeIndex;});
}

std::vector<gameType::ItemAction>::iterator TQCommandLine::getPlaceActionsIter(int actionIndex, const std::vector<gameType::Place, std::allocator<gameType::Place>>::iterator &place) const {
    return find_if(place->actions.begin(), place->actions.end(), [&actionIndex](const gameType::ItemAction& obj){ return obj.id == actionIndex;});
}

std::vector<gameType::Item>::iterator TQCommandLine::getItemIter(int itemIndex) const {
    return find_if(tqGameData->gameData.items.begin(), tqGameData->gameData.items.end(), [&itemIndex](const gameType::Item& obj){ return obj.id == itemIndex;});
}

std::vector<int64_t>::iterator TQCommandLine::getPlaceItemIter(int itemIndex, const std::vector<gameType::Place, std::allocator<gameType::Place>>::iterator &place) const {
    return find_if(place->items.begin(), place->items.end(), [&itemIndex](const int64_t& obj){ return obj == itemIndex;});
}

std::vector<int64_t>::iterator TQCommandLine::getBackpackItemIter(int itemIndex) const {
    return find_if(tqGameData->gameData.player.backpack.items.begin(), tqGameData->gameData.player.backpack.items.end(), [&itemIndex](const int64_t& obj){ return obj == itemIndex;});
}

std::vector<gameType::ItemAction>::iterator TQCommandLine::getItemActionIter(int actionIndex, const std::vector<gameType::Item, std::allocator<gameType::Item>>::iterator &item) const {
    return find_if(item->actions.begin(), item->actions.end(), [&actionIndex](const gameType::ItemAction& obj){ return obj.id == actionIndex;});
}

bool TQCommandLine::analyzeCommand(std::string line) {

    std::vector<std::string> commandsParts = TQTools::split(line);
    if (commandsParts.empty()) {
        return false;
    }
    gameType::Command command = getCommand( *commandsParts.begin() );
    commandsParts.erase(commandsParts.begin());
    switch (command.id) {
        case 1:
            showHelp();
            break;
        case 2:
            showPlayer();
            break;
        case 3:
            tqGameData->saveGameState();
            print << "Game saved " << std::endl;
            syncData(print);
            break;
        case 4:
            tqGameData->loadGameState();
            print << "Game loaded " << std::endl;
            syncData(print);
            invitation();
            break;
        case 5:
            showPlace();
            break;
        case 6:
            goToAnotherPlace(commandsParts);
            break;
        case 7:
            discoverItems(tqGameData->gameData.state.placeIndex);
            break;
        case 8:
            useItem(commandsParts);
            break;
        case 9:
            getItem(commandsParts);
            break;
        case 10:
            dropItem(commandsParts);
            break;
        case 100:
            tqGameData->saveGameState();
            print << "Game saved " << std::endl;
            print << "Good bye master ;-) " << std::endl;
            syncData(print);
            return true;
        case 101:
            invitation();
            break;
    }
    return false;
}

void TQCommandLine::goToAnotherPlace(std::vector<std::string> commandsParts) {
    if (commandsParts.empty()) {
        return;
    }
    int actionIndex = std::stoi(commandsParts.front());
    auto place = getPlaceIter(tqGameData->gameData.state.placeIndex);
    if (place != tqGameData->gameData.places.end()) {
        auto placeAction = getPlaceActionsIter(actionIndex, place);
        if (placeAction != place->actions.end()) {
            auto actionIter = getActionIter(placeAction->id);
            auto newPlace = getPlaceIter((int)placeAction->value);
            if ((newPlace != tqGameData->gameData.places.end()) && (actionIter != tqGameData->gameData.actions.end())) {
                tqGameData->gameData.state.placeIndex = placeAction->value;
                updatePlayerAttributes(actionIter->influence);
            }
        }
    }
    wait(waitTimeMultiply);
    showPlace();
}

void TQCommandLine::getItem(std::vector<std::string> commandsParts) {
    if (commandsParts.empty()) {
        return;
    }
    int itemIndex = std::stoi(commandsParts.front());
    auto place = getPlaceIter(tqGameData->gameData.state.placeIndex);
    auto placeItemIter = getPlaceItemIter(itemIndex, place);
    if (placeItemIter != place->items.end()) {
        auto item = getItemIter(itemIndex);
        if (item != tqGameData->gameData.items.end()) {
            if (tqGameData->gameData.player.backpack.items.size() < tqGameData->gameData.player.backpack.max) {
                tqGameData->gameData.player.backpack.items.push_back(itemIndex);
                place->items.erase(placeItemIter);
                wait(waitTimeMultiply);
                showPlace();
                showBackpack();
            } else {
                print << "Backpack is full" << std::endl;
                syncData(print);
            }
        } else {
            print << "Item not found" << std::endl;
            syncData(print);
        }
    } else {
        print << "Item not found" << std::endl;
        syncData(print);
    }
}

void TQCommandLine::dropItem(std::vector<std::string> commandsParts) {
    if (commandsParts.empty()) {
        return;
    }
    int itemIndex = std::stoi(commandsParts.front());
    auto backpackItemIter = getBackpackItemIter(itemIndex);
    auto place = getPlaceIter(tqGameData->gameData.state.placeIndex);
    if (backpackItemIter != tqGameData->gameData.player.backpack.items.end()) {
        auto item = getItemIter(itemIndex);
        if (item != tqGameData->gameData.items.end()) {

            tqGameData->gameData.player.backpack.items.erase(backpackItemIter);
            place->items.push_back(itemIndex);
            wait(waitTimeMultiply);
            showPlace();
            showBackpack();

        } else {
            print << "Item not found" << std::endl;
            syncData(print);
        }
    } else {
        print << "Item not found" << std::endl;
        syncData(print);
    }
}

void TQCommandLine::useItem(std::vector<std::string> commandsParts) {
    if ((commandsParts.empty()) || (commandsParts.size() < 2)) {
        print << "Error command" << std::endl;
        syncData(print);
        return;
    }

    auto partCommand = commandsParts.front();
    commandsParts.erase(commandsParts.begin());
    int itemIndex = std::stoi(partCommand);
    partCommand = commandsParts.front();
    int actionIndex = std::stoi(partCommand);

    auto backpackItemIter = getBackpackItemIter(itemIndex);
    if (backpackItemIter != tqGameData->gameData.player.backpack.items.end()) {
        auto item = getItemIter(itemIndex);

        auto  actionItemIter = getItemActionIter(actionIndex, item);

        if ((item != tqGameData->gameData.items.end()) && (actionItemIter != item->actions.end())) {
            tqGameData->gameData.player.backpack.items.erase(backpackItemIter);
            auto actionIter = getActionIter(actionItemIter->id);
            wait(waitTimeMultiply);
            if (actionIter != tqGameData->gameData.actions.end()) {
                updatePlayerAttributes(actionIter->influence);
                if ((actionIter->type == "read") && (!item->resources.empty())) {
                    print << getfile(item->resources) << std::endl;
                }
            }
            syncData(print);
            showPlayer();
        } else {
            print << "Item or action not found" << std::endl;
            syncData(print);
        }
    } else {
        print << "Item not found" << std::endl;
        syncData(print);
    }
}

void TQCommandLine::discoverItems(int placeIndex) {
    auto placeIter = getPlaceIter(placeIndex);
    placeIter->discovered = true;
    showItems(placeIter->items, true);
}

void TQCommandLine::updatePlayerAttributes(std::vector<gameType::Influence> influences) {
    for (auto influence : influences) {
        auto attribute = getPlayerAttributesIter(influence.id);
        attribute->value += influence.value;
    }
}

void TQCommandLine::invitation() {
    std::string image = getfile("TechQuest.txt");
    print << image;
    showPlayer();
    print << std::endl;
    showPlace();
    syncData(print);
}

void TQCommandLine::showItems(std::vector<int64_t> items, bool header) {
    if (header) { print << "--- Items ---" << std::endl; }
    for (auto item : items) {
        auto itemIter = getItemIter(item);

        print << "(" << std::setw(3) << itemIter->id << ")" << std::setw(10) << itemIter->name << "  -  " << itemIter->description << " | actions: ";
        for (auto actionItem : itemIter->actions) {
            auto actionIter = getActionIter(actionItem.id);
            print << "(" << std::setw(3) << actionIter->id << ") "  << std::setw(5) << actionIter->name << " - " << actionIter->description << " | ";
        }
        print << std::endl;
    }
    syncData(print);
}

void TQCommandLine::showPlace() {
    auto place = getPlaceIter(tqGameData->gameData.state.placeIndex);
    if (place != tqGameData->gameData.places.end()) {
        print << "[ " << place->name << " ]" << std::endl;
        print << place->description << std::endl;
        print << "--- Actions ---" << std::endl;
        for (auto actionParams : place->actions) {
            auto action = getActionIter(actionParams.id);
            auto placeAction = getPlaceIter(actionParams.value);
            if ((action != tqGameData->gameData.actions.end()) && (placeAction != tqGameData->gameData.places.end())) {
                print << "(" << std::setw(3) << action->id << ")" << std::setw(10) << action->name << "  -  " << action->description << " to " <<  placeAction->name << std::endl;
            }
        }
        if (place->discovered) {
            showItems(place->items, true);
        }
    }
    print << std::endl;
    syncData(print);
}

void TQCommandLine::showBackpack() {
    print << "--- Backpack ---" << std::endl;
    print << "Max items: " << tqGameData->gameData.player.backpack.max << std::endl;
    showItems(tqGameData->gameData.player.backpack.items, false);
}


void TQCommandLine::showPlayer() {
    print << "Player " << tqGameData->gameData.player.name << std::endl;
    for (auto attribute : tqGameData->gameData.player.attributes) {
        auto attributeDesc = getAttribute(attribute.id);
        print << " " << std::setw(15) << attributeDesc.name << " [" << std::setw(3) << attributeDesc.min << "-"  << std::setw(3) << attributeDesc.max << "]" << "  " << attribute.value  << std::endl;
    }
    syncData(print);
    showBackpack();
}

void TQCommandLine::showHelp() {
    print << "           ---- HELP ---- " << std::endl;
    for (auto command : tqGameData->gameData.commands) {
        print << " " << std::setw(10) << command.command << " - " << command.description << std::endl;
    }
    syncData(print);
}

gameType::GameDataAttribute TQCommandLine::getAttribute(int index) {
    for (auto attributeDesc : tqGameData->gameData.attributes){
        if (index == attributeDesc.id) {
            return attributeDesc;
        }
    }
    return gameType::GameDataAttribute();
}

gameType::Command TQCommandLine::getCommand(std::string line) {
    for (auto command : tqGameData->gameData.commands){
        if (line == command.command) {
            return command;
        }
    }
    return gameType::Command();
}

void TQCommandLine::wait(double multiply) {
    double time = (double)(tqGameData->gameData.player.attributes.size() * 100);
    for (auto playerAttributes : tqGameData->gameData.player.attributes) {
        time -= playerAttributes.value * playerAttributes.time;
    }
    time *= multiply;
    print << "[action in progress]";
    syncData(print);
    while (time > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        time -= 100;
        print << "#" << std::flush;
        syncData(print);
    }
    print << "[END]" << std::endl;
    syncData(print);
}









