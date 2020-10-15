
#ifndef TECHQUEST_TQCOMMANDLINE_H
#define TECHQUEST_TQCOMMANDLINE_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>
#include <nlohmann/json.hpp>
#include "TQGameData.h"
#include "TQTools.h"

class TQCommandLine {
    TQGameData *tqGameData = new TQGameData();
    std::string getfile(std::string fileName);

    void showPlayer();
    void showHelp();
    gameType::GameDataAttribute getAttribute(int index);
    gameType::Command getCommand(std::string line);
    void goToAnotherPlace(std::vector<std::string> commandsParts);

    [[nodiscard]] std::vector<gameType::Action, std::allocator<gameType::Action>>::iterator getActionIter(int actionIndex) const;

    [[nodiscard]] std::vector<gameType::Place, std::allocator<gameType::Place>>::iterator getPlaceIter(int actionValue) const;
public:
    bool analyzeCommand(std::string line);
    void invitation();
    void showPlace();
    TQCommandLine();
};

#endif //TECHQUEST_TQCOMMANDLINE_H