#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//�X���C��
class Enemy01 : public Enemy
{
public:
	Enemy01(int category);
    ~Enemy01() override;

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    //�e�ۓ��͏���
    void InputProjectile()override;

protected:
    //���S�����Ƃ��ɌĂ΂��
    void OnDead() override;

    //�f�o�b�O�pGUI
    //void DrawDebugGUI() override;

	//�f�o�b�O�I�u�W�F�N�g�\��
	void DrewDebugPrimitive()override;

	////�O���e����
	//void ProjectileStraightFront(int category, float angle)override;

private:
    //Model* model = nullptr;
};