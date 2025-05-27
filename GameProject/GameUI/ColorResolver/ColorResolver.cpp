#include "ColorResolver.h"
#include <fstream>
#include <iostream>

void ColorResolver::LoadColorData(const std::string& _filePath)
{
    LoadJsonFromFile(dir_ + _filePath, jsonData_);

    DesirializeFromJson(jsonData_);
}

void ColorResolver::UnloadColorData()
{
    colorMap_.clear();
}

void ColorResolver::LoadJsonFromFile(const std::string& _filePath, json& _jsonData)
{
    std::ifstream file(_filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << _filePath << std::endl;
        return;
    }
    file >> _jsonData;
    file.close();
}

void ColorResolver::DesirializeFromJson(const json& _jsonData)
{
    uint32_t idx_colorname = static_cast<uint32_t>(ColorName::None);
    ++idx_colorname;

    for (uint32_t i = 0; i < static_cast<uint32_t>(ColorName::Count)-1; ++i)
    {
        RGBA            result          = {};
        ColorName       colorname       = static_cast<ColorName>(idx_colorname++);
        std::string     str_colorname   = ColorTypeToString(colorname);
        auto            finditr         = _jsonData.find(str_colorname);

        if (finditr == _jsonData.end())
        {
            // ファイルに色名が存在しない
            continue;
        }

        std::string     hex             = finditr->get<std::string>();

        if (hex.empty())
        {
            std::cerr << "Invalid color value for " << str_colorname << std::endl;
            continue;
        }

        result = color::create(hex.c_str());

        colorMap_[colorname] = result;
    }

    return;
}
