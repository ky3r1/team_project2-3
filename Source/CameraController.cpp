#include <imgui.h>
#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

#define MAXANGLE_X 3.14f * 0.5f-0.01f
#define MINANGLE_X 0.1f

//�X�V����
void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //�J�����̉�]���x
    float speed = rollSpeed * elapsedTime;
#ifdef  MOUSECAMERA
    // IMGUI�̃}�E�X���͒l���g���ăJ�������삷��
    ImGuiIO io = ImGui::GetIO();

    // �}�E�X�J�[�\���̈ړ��ʂ����߂�
    float moveX = io.MouseDelta.x * 0.02f;
    float moveY = io.MouseDelta.y * 0.02f;
    // Y����]
    angle.y += moveX * 0.5f;
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }
    else if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    // X����]
    angle.x += moveY * 0.5f;
    if (angle.x > DirectX::XM_PI)
    {
        angle.x -= DirectX::XM_2PI;
    }
    else if (angle.x < -DirectX::XM_PI)
    {
        angle.x += DirectX::XM_2PI;
    }

    //Y���̉�]�l��-3.14~3.14
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }
    //angle.x�̐���
    if (angle.x >= MAXANGLE_X)
    {
        angle.x = MAXANGLE_X;
    }
    if (angle.x <= MINANGLE_X)
    {
        angle.x = MINANGLE_X;
    }    

#endif

    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    //DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range / 1.0f;
    eye.y = target.y - front.y * range / 1.0f;
    eye.z = target.z - front.z * range / 1.0f;

    if(eye.x<-40.0f) eye.x = -40.0f;
    if(eye.x>40.0f) eye.x = 40.0f;
    if(eye.z<-40.0f) eye.z = -40.0f;
    if(eye.z>40.0f) eye.z = 40.0f;
    //�J�����̎��_�ƒ����_��ݒ�
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(310, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::TreeNode("Camera"))
        {
            ImGui::SliderFloat3("target", &target.x, -5, 5);
            ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f * 0.5f);
            ImGui::SliderFloat3("eye", &eye.x, -10, 10);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}