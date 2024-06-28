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

Enemy::Enemy()
{
    //�q�b�g�G�t�F�N�g�ǂݍ���
    hitEffect = std::unique_ptr<Effect>(new Effect("Data/Effect/Hit.efk"));
}

void Enemy::Update(float elapsedTime)
{
    UpdateDelayTime(projectile_auto.checker, projectile_auto.time, DELAYAUTOTIME);
    CollisionProjectileVsPlayer();
}

//�f�o�b�O�v���~�e�B�u�`��
void Enemy::DrewDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    switch (category)
    {
    case PLAYERCATEGORY:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 0, 0, 1));
        break;
    case ENEMYCATEGORY:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 0, 1));
        break;
    default:
        break;
    }
}

void Enemy::DrawDebugGUI()
{
#ifdef ENEMYSTATEMACHINE
    Character::DrawDebugGUI();
    if (ImGui::TreeNode(name.c_str()))
    {
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
        ImGui::Text(u8"State:%s", str.c_str());
        ImGui::Text(u8"SubState:%s", subStr.c_str());
        ImGui::Text(u8"ID:%d", id);
        std::string p = std::string("position") + std::to_string(category_id);
        ImGui::SliderFloat3(p.c_str(), &position.x, -5, 5);
        std::string s = std::string("scale") + std::to_string(category_id);
        ImGui::SliderFloat3(s.c_str(), &scale.x, 0.01f, 4.0f);
        std::string a = std::string("angle") + std::to_string(category_id);
        ImGui::SliderFloat3(a.c_str(), &angle.x, -3.14f, 3.14f);
        std::string b = std::string("attack_range") + std::to_string(category_id);
        ImGui::SliderFloat(b.c_str(), &attack_range, 1.0f, 10.0f);
        ImGui::TreePop();
    }
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

    if (projectile_auto.checker)
    {   
        ProjectileStraightShotting(ENEMYCATEGORY, 0.0f, FRONT);
        projectile_auto.checker = false;
    }
}

void Enemy::CollisionProjectileVsPlayer()
{
    //Player�ƒe���Փ˔���
    Player& player= Player::Instance();
    Character::CollisionProjectileVsCharacter(&player, *hitEffect);
}
