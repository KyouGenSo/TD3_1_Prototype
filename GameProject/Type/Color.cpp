#include "Color.h"

RGBA RGB::toRGBA(uint8_t _a) const
{
    return RGBA{ r_, g_, b_, _a };
}

RGBA color::create(const std::string& _hex)
{
    std::string hex_main = {};
    if (_hex.front() == '#')
    {
        hex_main = _hex.substr(1);
    }
    else
    {
        hex_main = _hex;
    }

    if (hex_main.size() == 6)
    {
        return RGBA{ static_cast<uint8_t>(std::stoul(hex_main.substr(0, 2), nullptr, 16)),
                     static_cast<uint8_t>(std::stoul(hex_main.substr(2, 2), nullptr, 16)),
                     static_cast<uint8_t>(std::stoul(hex_main.substr(4, 2), nullptr, 16)),
                     255 };
    }
    else if (hex_main.size() == 8)
    {
        return RGBA{ static_cast<uint8_t>(std::stoul(hex_main.substr(0, 2), nullptr, 16)),
                     static_cast<uint8_t>(std::stoul(hex_main.substr(2, 2), nullptr, 16)),
                     static_cast<uint8_t>(std::stoul(hex_main.substr(4, 2), nullptr, 16)),
                     static_cast<uint8_t>(std::stoul(hex_main.substr(6, 2), nullptr, 16)) };
    }

    return {};
}
