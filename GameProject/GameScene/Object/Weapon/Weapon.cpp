#include "Weapon.h"

#include "QuatFunc.h"
#include "GameScene/Object/Bullets/BulletFactory.h"
#include <GameSystem/SoundManager/SoundManager.h>
#include <Type/ColliderType.h>
#include <Type/Singleton.h>
#include <Collision/CollisionManager.h>
#include <Utility/Adaptor.h>
#include <Camera.h>
#include <Math/NiQuaternion.h>
#include <cstdint>
#include <memory>
#include <utility>
#include <Collision/Collider.h>
#include <Object3d.h>
#include <Object3dBasic.h>
#include <Quaternion.h>
#include <Vector3.h>
#include <Math/NiVec3.h>
#include <GameScene/Object/Bullets/BulletBase.h>


void WeaponBase::Initialize()
{
    pRayToReticle_ = std::make_unique<Collision::Ray>();
    pRayToReticle_->SetLength(1000.0f);
    pRayToReticle_->AddAttribute(static_cast<uint32_t>(Collider::Type::WEAPON))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY));

    pCollisionManager_ = Singleton<Collision::Manager>::GetInstance();
}

void WeaponBase::Update()
{
    this->UpdateRay();
}

void WeaponBase::Fire()
{
    Fire(transform_.translate);
}

void WeaponBase::Fire(const Vector3& _position)
{
    auto coolTime = pChain_->GetCoolTime(pChain_->GetChain().front());
    if (coolTime > 0) return;
    auto bullet = BulletFactory::CreateBullet(pChain_->GetChain().front());
    AddNewBullet(std::move(bullet), _position);

    // 発射音を再生
    if (isEnableSound_)
    {
        SoundManager::GetInstance()->Play(sound_fire_);
    }
}

void WeaponBase::AddNewBullet(std::unique_ptr<BulletBase> _bullet, const Vector3& _position)
{
    _bullet->SetEmitter(emitter_);
    _bullet->SetIsChainBullet(false);
    _bullet->Initialize();
    _bullet->SetPosition(_position);
    _bullet->SetRotation(transform_.rotate);
    _bullet->SetForward(forward_);
    _bullet->SetChainManager(pChain_);
    _bullet->SetEmitter(emitter_);
    _bullet->Fire();

    bullets_.emplace_back(std::move(_bullet));
}

void WeaponBase::DeleteDeadBullet()
{
    bullets_.remove_if([](const std::unique_ptr<BulletBase>& _bullet) { return _bullet->IsDeadAll(); });
}

void WeaponBase::UpdateRay()
{
    // カメラ情報の取得
    Camera* pCamera = *Object3dBasic::GetInstance()->GetCamera();
    cameraPosition_ = pCamera->GetTranslate();
    NiVec3 cameraRotation = NiUtil::Adaptor(pCamera->GetRotate());

    // カメラの回転からクォータニオンを計算（ヨーとピッチ）
    NiQuaternion yawQuat   = NiQuaternion::RotateAxisAngleQuaternion({ 0.0f, 1.0f, 0.0f }, cameraRotation.y);
    NiQuaternion pitchQuat = NiQuaternion::RotateAxisAngleQuaternion({ 1.0f, 0.0f, 0.0f }, cameraRotation.x);
    NiQuaternion combinedRotation = yawQuat * pitchQuat;

    // カメラの前方向ベクトルを取得（Z軸を回転）
    cameraForward_ = NiUtil::Adaptor(FMath::RotateVector({ 0.0f, 0.0f, 1.0f }, combinedRotation));

    // レイの原点と方向を設定
    pRayToReticle_->SetOrigin(Adaptor(cameraPosition_));
    pRayToReticle_->SetDirection(Adaptor(cameraForward_));

    // レイキャストによる当たり判定
    hitdata_ = pCollisionManager_->RayCast(pRayToReticle_.get());
    auto pOtherCollider = pCollisionManager_->Get(hitdata_.uuid);

    // ヒットしている場合：ヒットポイントの方向をforward_に設定
    if (pOtherCollider)
    {
        Vector3 toHitPoint = Adaptor(hitdata_.hitPoint) - transform_.translate;
        Vector3 direction = toHitPoint.Normalize();

        forward_ = direction;
        isHitRayToReticle_ = true;
    }
    else
    {
        // ヒットしていない場合：自分の回転から前方向を計算
        Quaternion yaw   = Quat::MakeRotateAxisAngle({ 0, 1, 0 }, transform_.rotate.y);
        Quaternion pitch = Quat::MakeRotateAxisAngle({ 1, 0, 0 }, transform_.rotate.x);
        forward_ = Quat::RotateVec3({ 0, 0, 1 }, yaw * pitch);
        isHitRayToReticle_ = false;
    }
}

void WeaponBase::DrawDebug()
{
}