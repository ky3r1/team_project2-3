#pragma once

#include"Graphics/Model.h"
#include "All.h"

struct HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 }; //���C�ƃ|���S���̌�_
    DirectX::XMFLOAT3 normal = { 0,0,0 };   //�Փ˂����|���S���̖@���x�N�g��
    float             distance = 0.0f;      //���C�̎n�_�����_�܂ł̋���
    int               materialIndex = -1;   //�Փ˂����|���S���̃}�e���A���ԍ�
    DirectX::XMFLOAT3 rotation = { 0,0,0 };   //�p�x
};

//�R���W����
class Collision
{
public:
    //���Ƌ��̌�������
    static bool IntersectCylinderVsSphere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        float weightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        float weightB,
        DirectX::XMFLOAT3& outPositionB
    );

    //���Ɖ~���̌�������
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    //���C�ƃ��f���̌�_����
    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );

    //HPbar�̈ʒu
    static bool HPbarPoint(
        DirectX::XMFLOAT3 player_position,
        DirectX::XMFLOAT3 enemy_position
    );

    //bottom_left_front�`top_right_back����move_pos�������true��Ԃ�
    static bool InPoint(
        DirectX::XMFLOAT3 bottom_left_front,
        DirectX::XMFLOAT3 top_right_back,
        DirectX::XMFLOAT3 move_pos
    );

    //bottom_left_front�`top_right_back����XZ���ʂɂ�move_pos�������true��Ԃ�
    static bool InXYPoint(
        DirectX::XMFLOAT3 bottom_left_front,
        DirectX::XMFLOAT3 top_right_back,
        DirectX::XMFLOAT3 move_pos
    );

    //�_���~���ɓ����Ă��邩
    static bool PointInsideCircle(DirectX::XMFLOAT3 point, DirectX::XMFLOAT3 center, float radius);
};