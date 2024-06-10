#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
#include "Player.h"
#include "StateMachine.h"

//エネミー
class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}

    //更新処理
    virtual void Update(float elapsedTime) = 0;

    //描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//debugオブジェクト描画
    virtual void DrewDebugPrimitive();
	//デバッグ用GUI
	virtual void DrawDebugGUI();

    //破棄
	void Destroy();


	// 目標地点へ移動
	void MoveToTarget(float elapsedTime, float speedRate);

	// プレイヤー索敵
	bool SearchPlayer();

	//行動State
	enum class State
	{
		Search,
		Battle,
	};
	enum class Search
	{
		Death,
		Idle,
	};
	enum class Battle
	{
		Pursuit,
		Attack,
	};

/////////////////////ゲッター・セッター//////////////////////////

	//モデルのゲッター・セッター
	Model* GetModel() { return model; }

	// ポジション取得
	DirectX::XMFLOAT3 GetPosition() { return position; }

	//エネミーの種類
	int GetEnemyCategory() { return category; }

	// ターゲットのゲッター・セッター
	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

	// ステートタイマーのゲッター・セッター
	void SetStateTimer(float timer) { stateTimer = timer; }
	float GetStateTimer() { return stateTimer; }

	// 攻撃範囲のゲッター・セッター
	float GetAttackRange() { return attackRange; }
	StateMachine* GetStateMachine() { return stateMachine; }

	// IDゲッター・セッター
	virtual int GetId() { return id; }
	virtual void	SetId(int id) { this->id = id; }
protected:

	Model* model = nullptr;
	State				state = State::Search;
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	float				moveSpeed = 3.0f;
	float				turnSpeed = DirectX::XMConvertToRadians(360);
	float				stateTimer = 0.0f;
	float				attackRange = 1.5f;
	StateMachine* stateMachine = nullptr;
	int id = 0;
};