#pragma once
#include <cstdint>

enum class ColorName : uint32_t
{
    None = 0,
    DeepTeal,
    // Custom colors
    CoffeeBlack,
    Background,
    // Primary colors
    Gray,
    Red,
    Green,
    Blue,
    Yellow,
    Orange,
    Purple,
    DarkRed,
    DarkGreen,
    DarkBlue,
    DarkYellow,
    DarkOrange,
    DarkPurple,
    Count
};

static const char* ColorTypeToString(ColorName _colorName)
{
    switch (_colorName)
    {
    case ColorName::DeepTeal: return "DeepTeal";
    case ColorName::CoffeeBlack: return "CoffeeBlack";
    case ColorName::Background: return "Background";
    case ColorName::Gray: return "Gray";
    case ColorName::Red: return "Red";
    case ColorName::Green: return "Green";
    case ColorName::Blue: return "Blue";
    case ColorName::Yellow: return "Yellow";
    case ColorName::Orange: return "Orange";
    case ColorName::Purple: return "Purple";
    case ColorName::DarkRed: return "DarkRed";
    case ColorName::DarkGreen: return "DarkGreen";
    case ColorName::DarkBlue: return "DarkBlue";
    case ColorName::DarkYellow: return "DarkYellow";
    case ColorName::DarkOrange: return "DarkOrange";
    case ColorName::DarkPurple: return "DarkPurple";
    default: return "None";
    }
}