#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
#include "Player.h"
#include "StateMachine.h"

enum class EnemySlimeAnimation
{
	IdleNormal,
	IdleBattle,
	Attack01,
	Attack02,
	WalkFWD,
	WalkBWD,
	WalkLeft,
	WalkRight,
	RunFWD,
	SenseSomethingST,
	SenseSomethingRPT,
	Taunt,
	Victory,
	GetHit,
	Dizzy,
	Die,
};

//�G�l�~�[
class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}

    //�X�V����
    virtual void Update(float elapsedTime);

    //�`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//debug�I�u�W�F�N�g�`��
    virtual void DrewDebugPrimitive();
	//�f�o�b�O�pGUI
	virtual void DrawDebugGUI();

	//MoveSystem�̎��s
	virtual void MoveSystem();

	//�ʉߕs�}�X�ɓ����������̈ʒu�Ɉړ�
	virtual void OutMove();

    //�j��
	void Destroy();

	// �ڕW�n�_�ֈړ�
	void MoveToTarget(float elapsedTime, float speedRate);

	//�����ړ��X�V����
	void UpdateVerticalMove(float elapsedTime)override;

	//�U���͈͂ɓ�������true��Ԃ�
	bool InAttackRange();

	//�e�ۓ��͏���
	void InputProjectile();

	virtual void ProjectileStraightFront(int category, float angle);

	//�s��State
	enum class State
	{
		Search,
		Battle,
	};
	//�T��State
	enum class Search
	{
		Death,
		Idle,
	};
	//�퓬State
	enum class Battle
	{
		Pursuit,
		Attack,
	};

/////////////////////�Q�b�^�[�E�Z�b�^�[//////////////////////////

	// �^�[�Q�b�g�̃Q�b�^�[�E�Z�b�^�[
	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

	// �X�e�[�g�^�C�}�[�̃Q�b�^�[�E�Z�b�^�[
	float GetStateTimer() { return stateTimer; }
	void SetStateTimer(float timer) { stateTimer = timer; }

	StateMachine* GetStateMachine() { return stateMachine; }

	// ID�Q�b�^�[�E�Z�b�^�[
	virtual int GetId() { return id; }
	virtual void	SetId(int id) { this->id = id; }
protected:
	State				state = State::Search;
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	float				moveSpeed = 3.0f;
	float				stateTimer = 0.0f;
	StateMachine* stateMachine = nullptr;
	std::vector<DirectX::XMFLOAT2> moving_roots;
	int id = 0;
	int old_mapID=0;
	DelayTime projectile_auto;
	ProjectileManager projectileManager;
};