#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//enum class EnemySlimeAnimation
//{
//	IdleNormal,
//	IdleBattle,
//	Attack01,
//	Attack02,
//	WalkFWD,
//	WalkBWD,
//	WalkLeft,
//	WalkRight,
//	RunFWD,
//	SenseSomethingST,
//	SenseSomethingRPT,
//	Taunt,
//	Victory,
//	GetHit,
//	Dizzy,
//	Die,
//};

//�X���C��
class EnemyBoss : public Enemy
{
public:
	EnemyBoss(int category);
	~EnemyBoss() override;

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

protected:
	//���S�����Ƃ��ɌĂ΂��
	void OnDead() override;

	//�f�o�b�O�pGUI
	void DrawDebugGUI() override;

	//�f�o�b�O�I�u�W�F�N�g�\��
	void DrewDebugPrimitive()override;

	////�O���e����
	//void ProjectileStraightFront(int category, float angle)override;

private:
	//Model* model = nullptr;
};