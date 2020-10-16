
#ifndef TECHQUEST_TQCOMMANDLINE_H
#define TECHQUEST_TQCOMMANDLINE_H

#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>
#include <nlohmann/json.hpp>
#include "TQGameData.h"
#include "TQTools.h"

class TQCommandLine {
    std::function<void(void)> outputSwitch;
    const double waitTimeMultiply = 5;
    TQGameData *tqGameData = new TQGameData();
    std::string getfile(std::string fileName);

    void showPlayer();
    void showItems(std::vector<int64_t> items);
    void showHelp();
    gameType::GameDataAttribute getAttribute(int index);
    gameType::Command getCommand(std::string line);
    void goToAnotherPlace(std::vector<std::string> commandsParts);
    void updatePlayerAttributes(std::vector<gameType::Influence> influences);
    void wait(double multiply);
    void discoverItems(int placeIndex);


    [[nodiscard]] std::vector<gameType::GameDataAction, std::allocator<gameType::GameDataAction>>::iterator getActionIter(int actionIndex) const;
    [[nodiscard]] std::vector<gameType::Place, std::allocator<gameType::Place>>::iterator getPlaceIter(int placeIndex) const;
    [[nodiscard]] std::vector<gameType::PlayerAttribute, std::allocator<gameType::PlayerAttribute>>::iterator getPlayerAttributesIter(int attributeIndex) const;
    [[nodiscard]] std::vector<gameType::ItemAction, std::allocator<gameType::ItemAction>>::iterator getPlaceActionsIter(int actionIndex, const std::vector<gameType::Place, std::allocator<gameType::Place>>::iterator &place) const;
public:
    bool analyzeCommand(std::string line);
    void invitation();
    void showPlace();
    TQCommandLine(std::function<void(void)> outputSwitch);



};

#endif //TECHQUEST_TQCOMMANDLINE_H