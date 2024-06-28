#pragma once

#include"Graphics/Model.h"
#include "All.h"

struct HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 }; //レイとポリゴンの交点
    DirectX::XMFLOAT3 normal = { 0,0,0 };   //衝突したポリゴンの法線ベクトル
    float             distance = 0.0f;      //レイの始点から交点までの距離
    int               materialIndex = -1;   //衝突したポリゴンのマテリアル番号
    DirectX::XMFLOAT3 rotation = { 0,0,0 };   //角度
};

//コリジョン
class Collision
{
public:
    //球と球の交差判定
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

    //球と円柱の交差判定
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    //レイとモデルの交点判定
    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );

    //HPbarの位置
    static bool HPbarPoint(
        DirectX::XMFLOAT3 player_position,
        DirectX::XMFLOAT3 enemy_position
    );

    //bottom_left_front～top_right_back内にmove_posが入るとtrueを返す
    static bool InPoint(
        DirectX::XMFLOAT3 bottom_left_front,
        DirectX::XMFLOAT3 top_right_back,
        DirectX::XMFLOAT3 move_pos
    );

    //bottom_left_front～top_right_back内のXZ平面ににmove_posが入るとtrueを返す
    static bool InXYPoint(
        DirectX::XMFLOAT3 bottom_left_front,
        DirectX::XMFLOAT3 top_right_back,
        DirectX::XMFLOAT3 move_pos
    );

    //点が円内に入っているか
    static bool PointInsideCircle(DirectX::XMFLOAT3 point, DirectX::XMFLOAT3 center, float radius);
};