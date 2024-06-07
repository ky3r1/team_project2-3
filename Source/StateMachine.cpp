#include "StateMachine.h"
#include "StateDerived.h"

StateMachine::~StateMachine()
{
	for (/*State*/HierarchicalState* state : statePool)
	{
		delete state;
	}
	statePool.clear();
}
// 更新処理
void StateMachine::Update(float elapsedTime)
{
	currentState->Execute(elapsedTime);
}


//MainState
// ステートセット
void StateMachine::SetState(int setState)
{
	currentState = statePool.at(setState);
	currentState->Enter();
}
// ステート変更
void StateMachine::ChangeState(int newState)
{
	// サブステート
	currentState->GetSubState()->Exit();
	//メインステート
	currentState->Exit();

	currentState = statePool.at(newState);

	//メインステート
	currentState->Enter();
}
void StateMachine::RegisterState(HierarchicalState* state)
{
	// 親ステート登録
	statePool.emplace_back(state);
}


//SubState
void StateMachine::ChangeSubState(int newState)
{
	currentState->ChangeSubState(newState);
}
void StateMachine::RegisterSubState(int index, State* substate)
{
	statePool.at(index)->RegisterSubState(substate);
}

// ステート番号取得
int StateMachine::GetStateIndex()
{
	int i = 0;
	for (/*State*/HierarchicalState* state : statePool)
	{
		if (state == currentState)
		{
			// i番号目のステートをリターン
			return i;
		}
		++i;
	}
	// ステートが見つからなかった時
	return -1;
}


