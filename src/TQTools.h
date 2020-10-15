//
// Created by Marek Piotrowski-EXT on 2020-10-14.
//

#ifndef TECHQUEST_TQTOOLS_H
#define TECHQUEST_TQTOOLS_H

#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>

class TQTools {
public:
    [[nodiscard]] static std::vector<std::string>split(const std::string& inputString);

};


#endif //TECHQUEST_TQTOOLS_H
