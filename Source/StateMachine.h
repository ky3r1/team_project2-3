#pragma once
#include <vector>
#include "StateBase.h"

class StateMachine
{
public:
	// コンストラクタ
	StateMachine() {}
	// デストラクタ
	~StateMachine();
	// 更新処理
	void Update(float elapsedTime);

	//MainState
	// ステートセット
	void SetState(int setstate);
	// ステート変更
	void ChangeState(int newState);
	// ステート登録
	void RegisterState(HierarchicalState* state);

	//SubState
	//2層目のステート変更
	void ChangeSubState(int newState);
	//2層目ステート登録
	void RegisterSubState(int index, State* substate);


	// 現在のステート番号取得
	int GetStateIndex();
	//ステート取得
	HierarchicalState* GetState() { return currentState; }
protected:
	// 現在のステート
	HierarchicalState* currentState = nullptr;
	// 各ステートを保持する配列
	std::vector<HierarchicalState*> statePool;
};