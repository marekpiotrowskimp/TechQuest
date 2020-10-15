//
// Created by Marek Piotrowski-EXT on 2020-10-14.
//

#include "TQTools.h"

std::vector<std::string> TQTools::split(const std::string& inputString) {
    std::vector<std::string> result;
    std::istringstream iss(inputString);
    for(std::string s; iss >> s; ) {
        result.push_back(s);
    }
    return result;
}
