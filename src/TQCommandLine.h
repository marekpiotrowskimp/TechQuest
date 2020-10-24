
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
#include "server.h"

class TQCommandLine {
    const double waitTimeMultiply = 5;
    std::stringstream print;
    TQGameData *tqGameData = new TQGameData();
    std::string getfile(std::string fileName);

    void showBackpack();
    void showPlayer();
    void showItems(std::vector<int64_t> items, bool header);
    void showHelp();
    gameType::GameDataAttribute getAttribute(int index);
    gameType::Command getCommand(std::string line);
    void goToAnotherPlace(std::vector<std::string> commandsParts);
    void useItem(std::vector<std::string> commandsParts);
    void getItem(std::vector<std::string> commandsParts);
    void dropItem(std::vector<std::string> commandsParts);
    void updatePlayerAttributes(std::vector<gameType::Influence> influences);
    void wait(double multiply);
    void discoverItems(int placeIndex);

    [[nodiscard]] std::vector<int64_t>::iterator getBackpackItemIter(int itemIndex) const;
    [[nodiscard]] std::vector<int64_t>::iterator getPlaceItemIter(int itemIndex, const std::vector<gameType::Place, std::allocator<gameType::Place>>::iterator &place) const;
    [[nodiscard]] std::vector<gameType::Item>::iterator getItemIter(int itemIndex) const;
    [[nodiscard]] std::vector<gameType::GameDataAction, std::allocator<gameType::GameDataAction>>::iterator getActionIter(int actionIndex) const;
    [[nodiscard]] std::vector<gameType::Place, std::allocator<gameType::Place>>::iterator getPlaceIter(int placeIndex) const;
    [[nodiscard]] std::vector<gameType::PlayerAttribute, std::allocator<gameType::PlayerAttribute>>::iterator getPlayerAttributesIter(int attributeIndex) const;
    [[nodiscard]] std::vector<gameType::ItemAction, std::allocator<gameType::ItemAction>>::iterator getPlaceActionsIter(int actionIndex, const std::vector<gameType::Place, std::allocator<gameType::Place>>::iterator &place) const;
    [[nodiscard]] std::vector<gameType::ItemAction>::iterator getItemActionIter(int actionIndex, const std::vector<gameType::Item, std::allocator<gameType::Item>>::iterator &item) const;
public:
    std::function<void(std::basic_stringstream<char>&)> syncData;
    bool analyzeCommand(std::string line);
    void invitation();
    void showPlace();
    TQCommandLine(std::function<void(std::basic_stringstream<char>&)> syncData);

};

#endif //TECHQUEST_TQCOMMANDLINE_H