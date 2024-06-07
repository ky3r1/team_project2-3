#pragma once
#include <vector>
#include "StateBase.h"

class StateMachine
{
public:
	// �R���X�g���N�^
	StateMachine() {}
	// �f�X�g���N�^
	~StateMachine();
	// �X�V����
	void Update(float elapsedTime);

	//MainState
	// �X�e�[�g�Z�b�g
	void SetState(int setstate);
	// �X�e�[�g�ύX
	void ChangeState(int newState);
	// �X�e�[�g�o�^
	void RegisterState(HierarchicalState* state);

	//SubState
	//2�w�ڂ̃X�e�[�g�ύX
	void ChangeSubState(int newState);
	//2�w�ڃX�e�[�g�o�^
	void RegisterSubState(int index, State* substate);


	// ���݂̃X�e�[�g�ԍ��擾
	int GetStateIndex();
	//�X�e�[�g�擾
	HierarchicalState* GetState() { return currentState; }
protected:
	// ���݂̃X�e�[�g
	HierarchicalState* currentState = nullptr;
	// �e�X�e�[�g��ێ�����z��
	std::vector<HierarchicalState*> statePool;
};