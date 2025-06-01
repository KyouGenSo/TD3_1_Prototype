#pragma once
#include <cctype>
#include <string>

namespace Utility {
    inline bool EqualsIgnoreCase(const std::string& str1, const std::string& str2) {
        if (str1.size() != str2.size()) return false;
        for (size_t i = 0; i < str1.size(); ++i){
            if (tolower(str1[i]) != tolower(str2[i])){
                return false;
            }
        }
        return true;
    }
}
