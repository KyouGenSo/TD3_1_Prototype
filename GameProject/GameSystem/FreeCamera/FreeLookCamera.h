#pragma once

#include <Camera.h>
#include <Input.h>
#include <Math/NiQuaternion.h>
#include <memory>

class FreeLookCamera
{
public:
    FreeLookCamera();
    ~FreeLookCamera();

    void Initialize();
    void Update();


public: /// getter
    Camera* GetCamera() const
    {
        return pCamera_.get();
    }


private:
    std::unique_ptr<Camera> pCamera_ = nullptr;
    float moveSpeed_ = 0.1f;
    NiQuaternion rotate_ = NiQuaternion::Identity();
    POINT currCursorPos_ = {};
    POINT prevCursorPos_ = {};


private:
    void CatchMoveCommands();
    void CatchRotateCommands();


private:
    Input* pInput_ = nullptr;
};