#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
#include "Player.h"
#include "StateMachine.h"

//�G�l�~�[
class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}

    //�X�V����
    virtual void Update(float elapsedTime) = 0;

    //�`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//debug�I�u�W�F�N�g�`��
    virtual void DrewDebugPrimitive();
	//�f�o�b�O�pGUI
	virtual void DrawDebugGUI();

    //�j��
	void Destroy();


	// �ڕW�n�_�ֈړ�
	void MoveToTarget(float elapsedTime, float speedRate);

	// �v���C���[���G
	bool SearchPlayer();

	//�s��State
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

/////////////////////�Q�b�^�[�E�Z�b�^�[//////////////////////////

	//���f���̃Q�b�^�[�E�Z�b�^�[
	Model* GetModel() { return model; }

	// �|�W�V�����擾
	DirectX::XMFLOAT3 GetPosition() { return position; }

	//�G�l�~�[�̎��
	int GetEnemyCategory() { return category; }

	// �^�[�Q�b�g�̃Q�b�^�[�E�Z�b�^�[
	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }
	void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

	// �X�e�[�g�^�C�}�[�̃Q�b�^�[�E�Z�b�^�[
	void SetStateTimer(float timer) { stateTimer = timer; }
	float GetStateTimer() { return stateTimer; }

	// �U���͈͂̃Q�b�^�[�E�Z�b�^�[
	float GetAttackRange() { return attackRange; }
	StateMachine* GetStateMachine() { return stateMachine; }

	// ID�Q�b�^�[�E�Z�b�^�[
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