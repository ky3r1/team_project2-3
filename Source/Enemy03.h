#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//enum class EnemySlimeAnimation
//{
//	IdleNormal,
//	IdleBattle,
//	Attack01,
//	Attack02,
//	WalkFWD,
//	WalkBWD,
//	WalkLeft,
//	WalkRight,
//	RunFWD,
//	SenseSomethingST,
//	SenseSomethingRPT,
//	Taunt,
//	Victory,
//	GetHit,
//	Dizzy,
//	Die,
//};

//スライム
class Enemy03 : public Enemy
{
public:
	Enemy03(int category);
	~Enemy03() override;

	//更新処理
	void Update(float elapsedTime);

	//弾丸入力処理
	void InputProjectile()override;

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

protected:
	//死亡したときに呼ばれる
	void OnDead() override;

	//デバッグ用GUI
	//void DrawDebugGUI() override;

	//デバッグオブジェクト表示
	void DrewDebugPrimitive()override;

	////前方弾発射
	//void ProjectileStraightFront(int category, float angle)override;

private:
	//Model* model = nullptr;
};