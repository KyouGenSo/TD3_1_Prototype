#pragma once

#include <Vector2.h>
#include <Vector3.h>
#include <Vector4.h>

#include <Collision/Mathematics.h>

#include <Math/NiVec2.h>
#include <Math/NiVec3.h>
#include <Math/NiVec4.h>

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
    static NiVec2 Adaptor(Vector2 _vec)
    {
        return { _vec.x, _vec.y };
    }

    static Vector2 Adaptor(NiVec2 _vec)
    {
        return { _vec.x, _vec.y };
    }

    static NiVec3 Adaptor(Vector3 _vec)
    {
        return { _vec.x, _vec.y, _vec.z };
    }

    static Vector3 Adaptor(NiVec3 _vec)
    {
        return { _vec.x, _vec.y, _vec.z };
    }

    static NiVec4 Adaptor(Vector4 _vec)
    {
        return { _vec.x, _vec.y, _vec.z, _vec.w };
    }

    static Vector4 Adaptor(NiVec4 _vec)
    {
        return { _vec.x, _vec.y, _vec.z, _vec.w };
    }
}

