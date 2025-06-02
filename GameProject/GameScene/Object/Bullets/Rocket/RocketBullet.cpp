#include "RocketBullet.h"

#include <Quaternion.h>
#include <QuatFunc.h>

#include "EmitterManager.h"
#include "ModelManager.h"
#include "Collision/Collider.h"
#include "Type/ColliderType.h"
#include <Utility/Adaptor.h>
#include "GameSystem/DeltaTimeManager/DeltaTimeManager.h"
#include <variant>

void RocketBullet::Explosion::Init() {
	pCollider_ = std::make_unique<Collision::Collider>();
	pCollider_
		->SetType(Collision::Type::Sphere)
		->SetTranslate(Adaptor(transform_.translate))
		->SetSize(10.0f)
		->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
		->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
		->SetOwner(this)
		->Enable();

	if (emitter_){
		emitter_->SetEmitterPosition("explosion", transform_.translate);
		emitter_->CreateTemporaryEmitterFrom("explosion", GetUniqueId(), 2.f);
	}

	enable_ = true;
}

void RocketBullet::Explosion::Disable() {
	if (pCollider_){
		pCollider_->Disable();
		pCollider_.reset();
	}
	enable_ = false;
}

bool RocketBullet::Explosion::IsEnabled() const {
	return enable_;
}

void RocketBullet::Explosion::Draw() {
}

void RocketBullet::CB::Init() {
	explosion_ = std::make_unique<Explosion>();
	explosion_->SetPosition(position_);
	explosion_->SetEmitter(emitter_);
	explosion_->Init();
}

void RocketBullet::CB::Update() {
	if (!explosion_)return;

	if (explosion_->IsEnabled()){
 		explosion_->Disable();
		exInterval_ = INTERVAL;
		--remaining_;
		return;
	}

	if (remaining_ <= 0){
		explosion_->Disable();
		explosion_.reset();
		return;
	}

	if (0.f <= exInterval_){
		exInterval_ -= DeltaTimeManager::GetInstance()->GetDeltaTime(0);
		return;
	}

	explosion_->Init();
}

void RocketBullet::CB::SetPosition(const Vector3& pos) {
	position_ = pos;
	if (explosion_){
		explosion_->SetPosition(pos);
	}
}

bool RocketBullet::CB::IsFinish() const {
	return remaining_ <= 0;
}

void RocketBullet::CB::SetEmitter(EmitterManager* _emitter) {
	emitter_ = _emitter;
}

RocketBullet::RocketBullet() :BulletBase(){
    
}

void RocketBullet::Initialize()
{
	BulletBase::Initialize();

	type_ = WeaponType::RocketLauncher;
	speed_ = 80.0f;

	model_ = std::make_unique<Object3d>();
	model_->Initialize();
	model_->SetModel("rocketBullet.gltf");
	model_->SetScale({ 0.4f, 0.4f, 0.4f });

	CalcLifeTime();

	statusInit_
		.setAttack(5)
		.setHp(1)
		.setSpeed(1)
		.setDefence(0)
		.setMaxHp(1);
	statusCurrent_ = statusInit_;

	pCollider_ = std::make_unique<Collision::Collider>();
	pCollider_->SetEvent(Collision::EventType::Trigger, [this](const Collision::Collider* pCol){this->OnCollisionTrigger(pCol); })
		->SetTranslate(Adaptor(transform_.translate))
		->SetSize(0.3f)
		->SetType(Collision::Type::Sphere)
		->AddAttribute(static_cast<uint32_t>(Collider::Type::P_BULLET))
		->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
		->AddIgnore(static_cast<uint32_t>(Collider::Type::P_BULLET))
		->SetOwner(this)
		->Enable();
}

void RocketBullet::Update()
{
	BulletBase::Update();

	if (isChainBullet_)
	{
		UpdateChain();
	}
	else
	{
		UpdateNormal();
    	isDead_ = CheckLifeTime();
	}

	model_->SetRotate(transform_.rotate);
	model_->SetTranslate(transform_.translate);
	model_->Update();

	if (pNext_)
	{
		pNext_->Update();
	}
}

void RocketBullet::Draw()
{
	if (!isDead_)
	{
		// 描画処理
		model_->Draw();
	}

	if (pNext_) pNext_->Draw();
	Object::DrawCollider(pCollider_.get());
}

void RocketBullet::OnCollisionTrigger(const Collision::Collider* _other)
{
	if (isDead_ || pCollider_->IsDisabled()) return;

	isDead_ = true;
	pCollider_->Disable();

	// 強化カードの効果を適用するための通知
	NotifyReinforcementManager(_other);

	if (emitter_){
		emitter_->SetEmitterPosition("explosion", transform_.translate);
		emitter_->CreateTemporaryEmitterFrom("explosion", GetUniqueId(), 2.f);
	}

	Next();
}

void RocketBullet::InitializeNormal()
{
}

void RocketBullet::InitializeChain()
{
	cb_ = std::make_unique<CB>();
	cb_->SetPosition(transform_.translate);
	cb_->SetEmitter(emitter_);
	cb_->Init();

	isDead_ = false;
}

void RocketBullet::UpdateNormal()
{
	transform_.translate += forward_ * speed_ * deltaTime_;

	pCollider_->SetTranslate(Adaptor(transform_.translate));
}

void RocketBullet::UpdateChain()
{
	if (!cb_)return;
	cb_->Update();
	isDead_ = cb_->IsFinish();
}
