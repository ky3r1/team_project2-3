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
// �X�V����
void StateMachine::Update(float elapsedTime)
{
	currentState->Execute(elapsedTime);
}


//MainState
// �X�e�[�g�Z�b�g
void StateMachine::SetState(int setState)
{
	currentState = statePool.at(setState);
	currentState->Enter();
}
// �X�e�[�g�ύX
void StateMachine::ChangeState(int newState)
{
	// �T�u�X�e�[�g
	currentState->GetSubState()->Exit();
	//���C���X�e�[�g
	currentState->Exit();

	currentState = statePool.at(newState);

	//���C���X�e�[�g
	currentState->Enter();
}
void StateMachine::RegisterState(HierarchicalState* state)
{
	// �e�X�e�[�g�o�^
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

// �X�e�[�g�ԍ��擾
int StateMachine::GetStateIndex()
{
	int i = 0;
	for (/*State*/HierarchicalState* state : statePool)
	{
		if (state == currentState)
		{
			// i�ԍ��ڂ̃X�e�[�g�����^�[��
			return i;
		}
		++i;
	}
	// �X�e�[�g��������Ȃ�������
	return -1;
}


