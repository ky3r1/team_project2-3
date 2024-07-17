#pragma once

#include "All.h"

class CameraController
{
public:
    CameraController() {}
    ~CameraController() {}

    //�X�V����
    void Update(float elapsedTime);

    //�^�[�Q�b�g�ʒu�ݒ�
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    void DrawDebugGUI();

    DirectX::XMFLOAT3 GetAngle() { return angle; }
private:
    DirectX::XMFLOAT3       target = { 0,0,0 };
    DirectX::XMFLOAT3       angle = { /*1.59f*/0.981f,0,0 };
    DirectX::XMFLOAT3       eye = { 0,0,0 };
    float                   rollSpeed = DirectX::XMConvertToRadians(90);
    float                   range = 10.0f;
};