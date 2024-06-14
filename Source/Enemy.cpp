#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Mathf.h"
#include "Player.h"
#include "StageMapChip.h"

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

    // �f�o�b�O������\���̕ύX
    std::string mapstr = "";
    // ���݂̃X�e�[�g�ԍ��ɍ��킹�ăf�o�b�O�������str�Ɋi�[
    switch (StageMapChip::Instance().GetMapChipPositionCategory(position)) {
    case WALL:
        str = "Wall";
        break;
    case FLOOR:
        str = "Floor";
        break;
    case HOLE:
        str = "Hole";
        break;
    case SPIKE:
        str = "Spike";
        break;
    default:
        str = "None";
        break;
    }
    ImGui::Text(u8"State�@%s", str.c_str());
    ImGui::Text(u8"SubState�@%s", subStr.c_str());
    ImGui::Text(u8"MapChip�@%s", mapstr.c_str());
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

void Enemy::UpdateVerticalMove(float elapsedTime)
{
    Character::UpdateVerticalMove(elapsedTime);
    if (position.y < -5)
    {
        OnDead();
    }
}

//�U���͈͓��ɓ�������
bool Enemy::InAttackRange()
{
    // �ړI�n�_�܂ł�XZ���ʂł̋�������
    DirectX::XMFLOAT3 player_position = Player::Instance().GetPosition();
    float vx = player_position.x - position.x;
    float vz = player_position.z - position.z;
    float distSq = vx * vx + vz * vz;
    // �U���͈͓��܂Ńv���C���[�ɋ߂Â���
    if (distSq < attackRange * attackRange)
    {
        return true;
    }
    false;
}