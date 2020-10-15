#include "TQGameData.h"

nlohmann::json TQGameData::getJson(std::string fileName) {
    std::string path = "./data/" + fileName;
    std::ifstream fileStream(path);
    nlohmann::json gameData;
    fileStream >> gameData;
    return gameData;
}

void TQGameData::load() {
    data = getJson("TQ.json");
    gameData = nlohmann::json::parse(data.dump());
}

void TQGameData::loadGameState() {
    data = getJson("TQ_save.json");
    gameData = nlohmann::json::parse(data.dump());
}

void TQGameData::saveGameState() {
    std::ofstream fileStream("./data/TQ_save.json");
    nlohmann::json  gameState = gameData;
    fileStream << gameState.dump();
}
