#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Mathf.h"
#include "Player.h"

//�f�o�b�O�v���~�e�B�u�`��
void Enemy::DrewDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    switch (category)
    {
    case RED:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 0, 0, 1));
        break;
    case GREEN:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 0, 1));
        break;
    case BLUE:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 1, 1));
        break;
    case YELLOW:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 1, 0, 1));
        break;
    case PURPLE:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 0, 1, 1));
        break;
    case WHITE:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 1, 1, 1));
        break;
    default:
        break;
    }
}

void Enemy::DrawDebugGUI()
{
#ifdef ENEMYSTATEMACHINE
    // �f�o�b�O������\���̕ύX
    std::string str = "";
    std::string subStr = "";
    // ���݂̃X�e�[�g�ԍ��ɍ��킹�ăf�o�b�O�������str�Ɋi�[
    switch (static_cast<State>(stateMachine->GetStateIndex())) {
    case State::Search:
        str = "Search";
        switch (stateMachine->GetState()->GetSubStateIndex())
        {
        case static_cast<int>(Enemy::Search::Death):
            subStr = "Death";
            break;
        case static_cast<int>(Enemy::Search::Idle):
            subStr = "Idle";
            break;
        }
        break;
    case State::Battle:
        str = "Battle";
        switch (stateMachine->GetState()->GetSubStateIndex())
        {
        case static_cast<int>(Enemy::Battle::Pursuit):
            subStr = "Pursuit";
            break;
        case static_cast<int>(Enemy::Battle::Attack):
            subStr = "Attack";
            break;
        }
        break;
    }
    ImGui::Text(u8"State�@%s", str.c_str());
    ImGui::Text(u8"SubState�@%s", subStr.c_str());
#endif // ENEMYSTATEMACHINE
}

//�j��
void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}


void Enemy::MoveToTarget(float elapsedTime, float speedRate)
{
    // �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    // �ړ�����
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

bool Enemy::SearchPlayer()
{
    // �v���C���[�Ƃ̍��፷���l������3D�ŋ������������
    const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    //float vx = player_position.x - position.x;
    //float vy = player_position.y - position.y;
    //float vz = player_position.z - position.z;
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    if (playerPosition.x<-3)
    {
        vx = playerPosition.x - position.x;
    }
    float vz = playerPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < attackRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        // �P�ʃx�N�g����
        vx /= distXZ;
        vz /= distXZ;

        // �����x�N�g����
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        // 2�̃x�N�g���̓��ϒl�őO�㔻��
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }
    return false;
}
