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

TQCommandLine::TQCommandLine() {
    tqGameData->load();
}

void TQCommandLine::invitation() {
    std::string image = getfile("TechQuest.txt");
    std::cout << image;
    showPlace();
}


void TQCommandLine::showPlace() {
    auto place = getPlaceIter(tqGameData->gameData.state.placeIndex);
    if (place != tqGameData->gameData.places.end()) {
        std::cout << "[ " << place->name << " ]" << std::endl;
        std::cout << place->description << std::endl;

        for (auto actionParams : place->actions) {
            auto action = getActionIter(actionParams.id);
            auto placeAction = getPlaceIter(actionParams.value);
            if ((action != tqGameData->gameData.actions.end()) && (placeAction != tqGameData->gameData.places.end())) {
                std::cout << "ACTIONS" << std::endl;
                std::cout << "(" << std::setw(3) << action->id << ")" << std::setw(10) << action->name << "  -  " << action->description << " to " <<  placeAction->name << std::endl;
            }
        }
    }
}

std::vector<gameType::Place>::iterator TQCommandLine::getPlaceIter(int actionValue) const {
    return find_if(tqGameData->gameData.places.begin(), tqGameData->gameData.places.end(), [&actionValue](const gameType::Place& obj) {return obj.id == actionValue;});
}

std::vector<gameType::Action>::iterator TQCommandLine::getActionIter(int actionIndex) const {
    return find_if(tqGameData->gameData.actions.begin(), tqGameData->gameData.actions.end(), [&actionIndex](const gameType::Action& obj){ return obj.id == actionIndex;});
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
            std::cout << "Game saved " << std::endl;
            break;
        case 4:
            tqGameData->loadGameState();
            std::cout << "Game loaded " << std::endl;
            break;
        case 5:
            showPlace();
            break;
        case 6:
            goToAnotherPlace(commandsParts);
            break;
        case 100:
            tqGameData->saveGameState();
            std::cout << "Game saved " << std::endl;
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
    auto place = getPlaceIter(actionIndex);
    if (place != tqGameData->gameData.places.end()) {
        auto actionValue = find_if(place->actions.begin(), place->actions.end(), [&actionIndex](const gameType::InfluenceElement& obj){ return obj.id == actionIndex;});
        if (actionValue != place->actions.end()) {
            auto place = getPlaceIter(actionValue->value);
            if (place != tqGameData->gameData.places.end()) {
                std::cout << actionIndex << "START" << std::endl;
                tqGameData->gameData.state.placeIndex = actionValue->value;
            }
        }


    }
    showPlace();
}

void TQCommandLine::showPlayer() {
    std::cout << "Player " << tqGameData->gameData.player.name << std::endl;
    for (auto attribute : tqGameData->gameData.player.attributes) {
        auto attributeDesc = getAttribute(attribute.id);
        std::cout << " " << std::setw(10) << attributeDesc.name << " [" << std::setw(3) << attributeDesc.min << "-"  << std::setw(3) << attributeDesc.max << "]" << "  " << attribute.value  << std::endl;
    }
}

void TQCommandLine::showHelp() {
    std::cout << " ---- HELP ---- " << std::endl;
    for (auto command : tqGameData->gameData.commands) {
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






