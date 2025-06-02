#pragma once
#include "GameScene/Object/Object.h"

#include "Camera.h"
#include <Collision/Collider.h>
#include <GameSystem/Chain/Chain.h>
#include <GameScene/Object/Weapon/Weapon.h>
#include <Interfaces/IObserver.h>
#include <GameSystem/Reinforcement/IReinforcement.h>
#include <Type/with_initial.h>
#include <cstdint>

class Player : public Object
{
public:
    struct MinMax
    {
        float min = 0.0f;
        float max = 0.0f;
    };

private:

    std::unique_ptr<Collision::Collider> collider_;

    Chain* chain_ = nullptr;
    std::unique_ptr<WeaponBase> weapon_ = nullptr;

    std::list<IObserver*> observers_ = {};
    std::list<std::unique_ptr<IReinforcement>> reinforcementList_ = {};

    Vector3 move_ = {};

    bool isGround_ = true;

    const float HEIGHT_HALF = 0.5f;
    bool        isCollideCastle_ = false;

    // 挙動データ
    float       floor_                  = 0.5f;
    float       jumpPower_              = 20.0f;    // ジャンプ力
    float       moveSpeed_              = 35.0f;    // 移動速度
    float       frictionCoefficient_    = 5.0f;     // 摩擦係数 (friction coefficient)
    MinMax      posXMinMax              = { .min= -94.0f, .max= 95.0f}; // X座標の最小値と最大値
    MinMax      posZMinMax              = { .min= -395.0f, .max= 395.0f }; // Z座標の最小値と最大値
    float       distance_start_lerp_    = 10.0f;

    // カーソルの状態
    bool        mouseAim_       = false;
    const POINT ORIGIN          = {990, 540};
    float       sensitivity_    = 1.f; // マウス感度

    // コールバックID
    uint32_t    id_callback_enemydead_      = 0;
    uint32_t    id_callback_playerlevelup_  = 0;
    uint32_t    id_callback_chainconfirm_   = 0;
    uint32_t    id_callback_windowOpen_     = 0;

    // プレイヤー独自のステータス
    PlayerBehavior behaviorData_    = {}; // 行動制限
    float       xp_                 = 0.0f;
    float       xpMax_              = 100.0f;
    float       xpGainRetio_        = 0.1f;
    float       xpGained_           = 0.0f;
    float       level_              = 1.0f;
    int         countCursorVisible_ = 1;

    // 特殊なステータス
    with_initial<uint32_t>    numAbleJump_        = 2;        // ジャンプ可能回数


public:
    // 一般
    void    Initialize() override;
    void    Update() override;
    void    Draw() override;
    void    DrawDebug();
    void    Finalize();
    void    ImGui() override;

    // コールバック
    void    OnCollision(const Collision::Collider* pCollider) override;
    void    OnCollisionTrigger(const Collision::Collider* pCollider) override;
    void    OnChainConfirm();

    // Setters
    void    AddObserver(IObserver* _observer) { observers_.push_back(_observer); }
    void    SetFloor(float _floor) { floor_ = _floor; }
    void    SetChain(Chain* _chain) { chain_ = _chain; weapon_->SetChain(_chain); }
    void    AddReinforcement(const std::string& _cardName);

    // Getters
    float   getXP() const { return xp_; }
    float   getXPMax() const { return xpMax_; }

    void    SetSens(float sens) { sensitivity_ = sens; }

private:
    // 内部関数
    void    InitializeCallbacks();
    void    UpdateInputCommands();
    void    UpdateMovement();
    void    UpdateStatus();
    void    UpdateOpacityByCameraDistance();
    void    ChangeAimMode(bool isMouseAim);        // マウスエイムモードを変更 
    void    ChangeAimModeForce(bool _isMouseAim);  // マウスエイムモードを強制変更
    void    ToggleAimModeForce();                  // マウスエイムモードを強制切り替え
};

