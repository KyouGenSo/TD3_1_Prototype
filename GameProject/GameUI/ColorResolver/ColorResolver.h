#pragma once
#include <string>

#include "../ColorName/ColorName.h"

#include <Type/Color.h>
#include <unordered_map>
#include <json.hpp>

class ColorResolver
{
public:
    ColorResolver(const ColorResolver&) = delete;
    ColorResolver& operator=(const ColorResolver&) = delete; 
    ColorResolver(ColorResolver&&) = delete;
    ColorResolver& operator=(ColorResolver&&) = delete;

    static ColorResolver* GetInstance()
    {
        static ColorResolver instance;
        return &instance;
    }

    ColorResolver* Initialize() { return this; };
    void Finalize();

    void LoadColorData(const std::string& _filePath);

    void UnloadColorData();

    RGBA Resolve(ColorName _colorname) const { return colorMap_.at(_colorname); }

private:
    ColorResolver() = default;
    ~ColorResolver() = default;

    using json = nlohmann::json;
    void LoadJsonFromFile(const std::string& _filePath, json& _jsonData);
    void DesirializeFromJson(const json& _jsonData);

private:
    const std::string dir_ = "resources/json/";
    std::unordered_map<ColorName, RGBA> colorMap_;
    json jsonData_;

};