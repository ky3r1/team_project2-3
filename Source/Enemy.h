#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
#include "Player.h"
#include "StateMachine.h"

enum class EnemySlimeAnimation
{
	Hit,
	Walk,
	Attack01,
	Death,
};

enum
{
	Enemy01_Num,
	Enemy02_Num,
	Enemy03_Num,
	EnemyBoss_Num,
};

//�G�l�~�[
class Enemy : public Character
{
public:
	Enemy();
    ~Enemy() override {}

    //�X�V����
    virtual void Update(float elapsedTime);

    //�`�揈��
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader);

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
	virtual void InputProjectile();

	//�eVSPlayer
	void CollisionProjectileVsPlayer();

	//�s��State
	enum class State
	{
		Search,
		Battle,
	};
	//�T��State
	enum class Search
	{
		Idle,
	};
	//�퓬State
	enum class Battle
	{
		Pursuit,
		Attack,
		Idle,
		Hit,
	};

public:
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

	//delaytime.flg�̃Q�b�^�[
    bool GetProjectileAttackFlg() { return projectile_auto.checker; }

	//�G�̃J�e�S���[�̃Q�b�^�[�E�Z�b�^�[
	int GetEnemyCategory() { return category_id; }
	void SetEnemyCategory(int category_id) { this->category_id = category_id; }

    // ���O�̃Q�b�^�[�E�Z�b�^�[
    std::string GetName() { return name; }
    void SetName(std::string name) { this->name = name; }
	//int GetEnemyCategory() { return enemy_categry; }
public:
	bool isAlreadyHit = false;
	bool isHit = false;
protected:
	std::unique_ptr<Model> HPbar_model = nullptr;
	DirectX::XMFLOAT3 HPbar_scale = {};
	std::unique_ptr<Effect> death_effect = nullptr;
	State				state = State::Search;
	DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
	float				moveSpeed = 3.0f;
	float				stateTimer = 0.0f;
	StateMachine* stateMachine = nullptr;
	std::vector<DirectX::XMFLOAT2> moving_roots;
	int id = 0;
	int category_id = 0;
	int enemy_categry = -1;
	DelayTime projectile_auto;
	std::string name = "";
};