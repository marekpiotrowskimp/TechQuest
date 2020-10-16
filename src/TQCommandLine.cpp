
#include "TQCommandLine.h"


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

TQCommandLine::TQCommandLine(std::function<void(void)> outputSwitch) : outputSwitch(outputSwitch) {
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
            outputSwitch();
            std::cout << "Game saved " << std::endl;
            break;
        case 4:
            tqGameData->loadGameState();
            outputSwitch();
            std::cout << "Game loaded " << std::endl;
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
        case 100:
            tqGameData->saveGameState();
            outputSwitch();
            std::cout << "Game saved " << std::endl;
            outputSwitch();
            std::cout << "Good bye master ;-) " << std::endl;
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

void TQCommandLine::discoverItems(int placeIndex) {
    auto placeIter = getPlaceIter(placeIndex);
    placeIter->discovered = true;
    showItems(placeIter->items);
}

void TQCommandLine::updatePlayerAttributes(std::vector<gameType::Influence> influences) {
    for (auto influence : influences) {
        auto attribute = getPlayerAttributesIter(influence.id);
        attribute->value += influence.value;
    }
}

void TQCommandLine::invitation() {
    std::string image = getfile("TechQuest.txt");
    outputSwitch();
    std::cout << image;
    outputSwitch();
    showPlayer();
    outputSwitch();
    std::cout << std::endl;
    outputSwitch();
    showPlace();
}

void TQCommandLine::showItems(std::vector<int64_t> items) {
    outputSwitch();
    std::cout << "--- Items ---" << std::endl;
    for (auto item : items) {
        auto itemIter = find_if( tqGameData->gameData.items.begin(), tqGameData->gameData.items.end(), [&item](const gameType::Item& obj){ return obj.id == item;});
        outputSwitch();
        std::cout << "(" << std::setw(3) << itemIter->id << ")" << std::setw(10) << itemIter->name << "  -  " << itemIter->description << " actions " <<  /*placeAction->name <<*/ std::endl;
    }
}

void TQCommandLine::showPlace() {
    auto place = getPlaceIter(tqGameData->gameData.state.placeIndex);
    if (place != tqGameData->gameData.places.end()) {
        outputSwitch();
        std::cout << "[ " << place->name << " ]" << std::endl;
        outputSwitch();
        std::cout << place->description << std::endl;
        outputSwitch();
        std::cout << "--- Actions ---" << std::endl;
        for (auto actionParams : place->actions) {
            auto action = getActionIter(actionParams.id);
            auto placeAction = getPlaceIter(actionParams.value);
            if ((action != tqGameData->gameData.actions.end()) && (placeAction != tqGameData->gameData.places.end())) {
                outputSwitch();
                std::cout << "(" << std::setw(3) << action->id << ")" << std::setw(10) << action->name << "  -  " << action->description << " to " <<  placeAction->name << std::endl;
            }
        }
        if (place->discovered) {
            showItems(place->items);
        }
    }
}

void TQCommandLine::showPlayer() {
    outputSwitch();
    std::cout << "Player " << tqGameData->gameData.player.name << std::endl;
    for (auto attribute : tqGameData->gameData.player.attributes) {
        auto attributeDesc = getAttribute(attribute.id);
        outputSwitch();
        std::cout << " " << std::setw(10) << attributeDesc.name << " [" << std::setw(3) << attributeDesc.min << "-"  << std::setw(3) << attributeDesc.max << "]" << "  " << attribute.value  << std::endl;
    }
}

void TQCommandLine::showHelp() {
    outputSwitch();
    std::cout << "           ---- HELP ---- " << std::endl;
    for (auto command : tqGameData->gameData.commands) {
        outputSwitch();
        std::cout << " " << std::setw(10) << command.command << " - " << command.description << std::endl;
    }
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
    outputSwitch();
    std::cout << "[action in progress]";
    while (time > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        time -= 100;
        outputSwitch();
        std::cout << "#" << std::flush;
    }
    outputSwitch();
    std::cout << "[END]" << std::endl;
}








