#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

//移動床ステージ
class StageMoveFloor :public Stage
{
public:
    StageMoveFloor(int category);
    ~StageMoveFloor();

    //更新処理
    void Update(float elapsedTime)override;
    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;
    //デバッグ用GUI
    void DrawDebugGUI()override;

    //スタート位置
    void SetStartPoint(const DirectX::XMFLOAT3& start) { this->start = start; }
    //ゴール位置
    void SetGoalPoint(const DirectX::XMFLOAT3& end) { this->goal = end; }
    //トルク設定
    void SetTrque(const DirectX::XMFLOAT3& torque) { this->torque = torque; }

private:
    DirectX::XMFLOAT4X4 oldTransform =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    DirectX::XMFLOAT3 torque = { 0,0,0 };
    DirectX::XMFLOAT3 start = { 0,0,0 };
    DirectX::XMFLOAT3 goal = { 0,0,0 };

    float moveSpeed = 2.0f;
    float moveRate = 0.0f;

    DirectX::XMFLOAT3 oldAngle = { 0,0,0 };
};