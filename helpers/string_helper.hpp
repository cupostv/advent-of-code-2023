#pragma once

#include "includes.hpp"

namespace helper {
    namespace string {
        bool isAsciiDigit(char digit) {
            return std::isdigit(static_cast<unsigned char>(digit));
        }

        void removeSpaces(std::string &str) {
            str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
        }
    }
}
