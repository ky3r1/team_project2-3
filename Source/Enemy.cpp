#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Mathf.h"
#include "Player.h"
//#include "StageMapChip.h"
#include "EnemyMoveSystem.h"

#include "ProjectileManager.h"
#include "ProjectileStraight.h"

#define DELAYAUTOTIME 40

void Enemy::Update(float elapsedTime)
{
    UpdateDelayTime(projectile_auto.checker, projectile_auto.time, DELAYAUTOTIME);
}

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
    Character::DrawDebugGUI();
    ImGui::Text(u8"MapOldID�@%d", old_mapID);
#endif // ENEMYSTATEMACHINE
}

void Enemy::MoveSystem()
{
    EnemyMoveSystem::Instance().NextMoveArea(position, targetPosition);
}

void Enemy::OutMove()
{
    //if (StageMapChip::Instance().GetOnCost(position) > 20)
    //{
    //    DirectX::XMFLOAT3 old_position = StageMapChip::Instance().GetIDPosition(old_mapID);
    //    targetPosition = old_position;
    //}
    //else
    //{
    //    old_mapID = StageMapChip::Instance().GetOnId(position);
    //}
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
    if (distSq < attack_range * attack_range)
    {
        return true;
    }
    false;
}

void Enemy::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
        ProjectileStraightFront(RED, 0.0f);
    if (projectile_auto.checker)
    {

        projectile_auto.checker = false;
    }
}

void Enemy::ProjectileStraightFront(int category, float angle)
{
    //����
    ProjectileStraight* projectile{};
    //�O����
    DirectX::XMFLOAT3 dir;
    DirectX::XMFLOAT3 dis_pos;
    DirectX::XMVECTOR Dis_pos;
    DirectX::XMMATRIX Right_;
    DirectX::XMFLOAT3 r;
    DirectX::XMFLOAT3 axis = { 0,1,0 };
    DirectX::XMVECTOR Axis;

    dir.x = transform._31 * 100.0f;
    dir.y = 0.0f;
    dir.z = transform._33 * 100.0f;
    DirectX::XMFLOAT3 right;
    right.x = transform._11 * 100.0f;
    right.y = 0.0f;
    right.z = transform._13 * 100.0f;
    //���ˈʒu�i�v���C���[�̍�������j
    DirectX::XMFLOAT3 pos;
    pos.x = position.x;
    pos.y = position.y + height * 0.5f;
    pos.z = position.z;

    DirectX::XMVECTOR Right = DirectX::XMLoadFloat3(&right);
    Right = DirectX::XMVectorScale(Right, angle);
    DirectX::XMVECTOR Dir = DirectX::XMLoadFloat3(&dir);
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&pos);
    DirectX::XMVECTOR Ev = DirectX::XMVectorAdd(Dir, Right);
    DirectX::XMVECTOR Ep = DirectX::XMVectorAdd(Pos, Ev);
    Ep = DirectX::XMVectorSubtract(Ep, Pos);
    DirectX::XMFLOAT3 ep;
    DirectX::XMStoreFloat3(&ep, Ep);
    dir.x = ep.x;
    dir.y = 0.0f;
    dir.z = ep.z;
    projectile = new ProjectileStraight(&projectileManager, category);
    projectile->Launch(dir, pos);
}
