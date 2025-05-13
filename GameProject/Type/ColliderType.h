#pragma once

namespace Collider{
    enum class Type{
        NONE = 0,
        ALLY = 0b1,
        P_BULLET = 0b1 << 1,
        ENEMY = 0b1 << 2,
        STAGE = 0b1 << 3,
        CAMERA = 0b1 << 4,
    };
}