#pragma once

#include <Vector3.h>
#include <Collision/Mathematics.h>
#include <Math/NiVec3.h>

static Collision::Vec3 Adaptor(Vector3 _vec)
{
    return {_vec.x, _vec.y, _vec.z};
}

static Vector3 Adaptor(Collision::Vec3 _vec)
{
    return {_vec.x, _vec.y, _vec.z};
}

namespace NiUtil
{
    static NiVec3 Adaptor(Vector3 _vec)
    {
        return { _vec.x, _vec.y, _vec.z };
    }

    static Vector3 Adaptor(NiVec3 _vec)
    {
        return { _vec.x, _vec.y, _vec.z };
    }
}

