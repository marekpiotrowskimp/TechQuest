#ifndef TECHQUEST_TQGAMEDATA_H
#define TECHQUEST_TQGAMEDATA_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <vector>
#include "TQDataModels.h"

class TQGameData {
    nlohmann::json data;
    nlohmann::json getJson(std::string fileName);

public:
    gameType::GameData gameData = gameType::GameData();
    void load();

    void saveGameState();
    void loadGameState();
};

#endif