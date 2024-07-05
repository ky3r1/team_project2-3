#pragma once
#include "Enemy01.h"
#include "StateBase.h"

//メインステート
#define SEARCHSTATE 
#define BATTLESTATE

//捜索
class SearchState :public HierarchicalState
{
public:
	SearchState(Enemy* enemy) :HierarchicalState(enemy){}
	~SearchState();

	//すべて継承先で実装させる必要があるため純粋仮想関数で実装
	//ステートに入った時のメソッド
	void Enter();
	//ステートで実行するメソッド
	void Execute(float elapsedTime);
	// ステートから出ていくときのメソッド 
	void Exit();

};

//戦闘
class BattleState :public HierarchicalState
{
public:
	BattleState(Enemy* enemy) :HierarchicalState(enemy) {}
	~BattleState();

	//すべて継承先で実装させる必要があるため純粋仮想関数で実装
	//ステートに入った時のメソッド
	void Enter();
	//ステートで実行するメソッド
	void Execute(float elapsedTime);
	void Exit();

};

#ifdef SEARCHSTATE
// 死亡ステートオブジェクト
class DeathState : public State
{
public:
	// コンストラクタ
	DeathState(Enemy* enemy) :State(enemy) {};
	// デストラクタ
	~DeathState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 待機ステートオブジェクト
class IdleState : public State
{
public:
	// コンストラクタ
	IdleState(Enemy* enemy) :State(enemy) {};
	// デストラクタ
	~IdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	int idle_timer = 0;
};
#endif // SEARCHSTATE

#ifdef BATTLESTATE
// 追跡ステートオブジェクト
class PursuitState : public State
{
public:
	// コンストラクタ
	PursuitState(Enemy* enemy) :State(enemy) {};
	// デストラクタ
	~PursuitState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 攻撃ステートオブジェクト
class AttackState : public State
{
public:
	// コンストラクタ
	AttackState(Enemy* enemy) :State(enemy) {};
	// デストラクタ
	~AttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 戦闘待機ステートオブジェクト
class BattleIdleState : public State
{
public:
	// コンストラクタ
	BattleIdleState(Enemy* enemy) :State(enemy) {};
	// デストラクタ
	~BattleIdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	int idle_timer = 0;
};
#endif // BATTLESTATE