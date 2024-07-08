#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
#include "Player.h"
#include "StateMachine.h"

enum class EnemySlimeAnimation
{
	Idle,
	Walk,
	Attack01,
	Death,
	Attack02,
	Attack03,
};

enum
{
	Enemy01_Num,
	Enemy02_Num,
	Enemy03_Num,
	EnemyBoss_Num,
};

//エネミー
class Enemy : public Character
{
public:
	Enemy();
    ~Enemy() override {}

    //更新処理
    virtual void Update(float elapsedTime);

    //描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//debugオブジェクト描画
    virtual void DrewDebugPrimitive();
	//デバッグ用GUI
	virtual void DrawDebugGUI();

	//MoveSystemの実行
	virtual void MoveSystem();

	//通過不可マスに入った時元の位置に移動
	virtual void OutMove();

    //破棄
	void Destroy();

	// 目標地点へ移動
	void MoveToTarget(float elapsedTime, float speedRate);

	//垂直移動更新処理
	void UpdateVerticalMove(float elapsedTime)override;

	//攻撃範囲に入ったらtrueを返す
	bool InAttackRange();

	//弾丸入力処理
	void InputProjectile();

	//弾VSPlayer
	void CollisionProjectileVsPlayer();

	bool IsHitCheck() { return isHit; }

	//行動State
	enum class State
	{
		Search,
		Battle,
	};
	//探索State
	enum class Search
	{
		Death,
		Idle,
	};
	//戦闘State
	enum class Battle
	{
		Pursuit,
		Attack,
	};

public:
	/////////////////////ゲッター・セッター//////////////////////////

	// ターゲットのゲッター・セッター
	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

	// ステートタイマーのゲッター・セッター
	float GetStateTimer() { return stateTimer; }
	void SetStateTimer(float timer) { stateTimer = timer; }

	StateMachine* GetStateMachine() { return stateMachine; }

	// IDゲッター・セッター
	virtual int GetId() { return id; }
	virtual void	SetId(int id) { this->id = id; }

	//delaytime.flgのゲッター
    bool GetProjectileAttackFlg() { return projectile_auto.checker; }

	bool Rico_Checker(int enemy) { return enemy_rico_check; }

	//敵のカテゴリーのゲッター・セッター
	int GetEnemyCategory() { return enemy_categry; }

public:
	bool isAlreadyHit = false;
	bool isHit = false;
protected:
	State				state = State::Search;
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	float				moveSpeed = 3.0f;
	float				stateTimer = 0.0f;
	StateMachine* stateMachine = nullptr;
	std::vector<DirectX::XMFLOAT2> moving_roots;
	int id = 0;
	int category_id = 0;
	int enemy_categry = -1;
	bool enemy_rico_check = false;
	DelayTime projectile_auto;
	std::string name = "";
};