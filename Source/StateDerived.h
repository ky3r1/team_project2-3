#pragma once
#include "EnemySlime.h"
#include "StateBase.h"

//���C���X�e�[�g
#define SEARCHSTATE 
#define BATTLESTATE

//�{��
class SearchState :public HierarchicalState
{
public:
	SearchState(EnemySlime* enemy) :HierarchicalState(enemy){}
	~SearchState();

	//���ׂČp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	//�X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	//�X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h 
	void Exit();

};

//�퓬
class BattleState :public HierarchicalState
{
public:
	BattleState(EnemySlime* enemy) :HierarchicalState(enemy) {}
	~BattleState();

	//���ׂČp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	//�X�e�[�g�ɓ��������̃��\�b�h
	void Enter();
	//�X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime);
	void Exit();

};

#ifdef SEARCHSTATE
// �p�j�X�e�[�g�I�u�W�F�N�g
class WanderState : public State
{
public:
	// �R���X�g���N�^
	WanderState(EnemySlime* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~WanderState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ҋ@�X�e�[�g�I�u�W�F�N�g
class IdleState : public State
{
public:
	// �R���X�g���N�^
	IdleState(EnemySlime* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~IdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	int idle_timer = 0;
};
#endif // SEARCHSTATE

#ifdef BATTLESTATE
// �ǐՃX�e�[�g�I�u�W�F�N�g
class PursuitState : public State
{
public:
	// �R���X�g���N�^
	PursuitState(EnemySlime* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~PursuitState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �U���X�e�[�g�I�u�W�F�N�g
class AttackState : public State
{
public:
	// �R���X�g���N�^
	AttackState(EnemySlime* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~AttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
#endif // BATTLESTATE