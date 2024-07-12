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
    //���S�G�t�F�N�g
    death_effect = std::unique_ptr<Effect>(new Effect("Data/Effect/enemy_death.efkefc"));
    //HPbar
    HPbar_model= std::unique_ptr<Model>(new Model("Data/Model/Cube/Cube.mdl"));
    HPbar_scale = { 1.0f,1.0f,1.0f };
    health = 5;
}

void Enemy::Update(float elapsedTime)
{
    UpdateDelayTime(projectile_auto.checker, projectile_auto.time, DELAYAUTOTIME);
    CollisionProjectileVsPlayer();
}

void Enemy::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    DirectX::XMFLOAT4 HP_color = {};
    switch (health)
    {
    case 1:
        //Red
        HP_color = { 1.0f,0.0f,0.0f,1.0f };
        break;
    case 2:
        //Orange
        HP_color = { 1.0f,0.5f,0.0f,1.0f };
        break;
    case 3:
        //Yellow
        HP_color = { 1.0f,1.0f,0.0f,1.0f };
        break;
    case 4:
        //LightGreen
        HP_color = { 0.5f,1.0f,0.0f,1.0f };
        break;
    case 5:
        //Green
        HP_color = { 0.0f,1.0f,0.0f,1.0f };
        break;
    }
    DirectX::XMFLOAT4X4 transform_HP;
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(HPbar_scale.x, HPbar_scale.y, HPbar_scale.z);
    //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.1, 0.1, 0.1);
    //��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //�ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y + 4.5f, position.z);
    //3�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;
    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform_HP, W);
    HPbar_model.get()->UpdateTransform(transform_HP);
    shader->Draw(dc, HPbar_model.get(), { HP_color.x,HP_color.y,HP_color.z,1});
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
            case static_cast<int>(Enemy::Battle::Idle):
                subStr = "BattleIdle";
                break;
            case static_cast<int>(Enemy::Battle::Hit):
                subStr = "Hit";
                break;
            }
            break;
        }
        ImGui::Text(u8"State:%s", str.c_str());
        ImGui::Text(u8"SubState:%s", subStr.c_str());
        ImGui::Text(u8"ID:%d", id);
        ImGui::SliderFloat3("position", &position.x, -5, 5);
        ImGui::SliderFloat3("scale", &scale.x, 0.01f, 4.0f);
        ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);
        ImGui::SliderFloat("attack_range", &attack_range, 1.0f, 10.0f);
        if (ImGui::Button("delete", ImVec2(100, 30)))Destroy();
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

        death_effect.get()->Play(position, 1.0f);

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
