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
    //ヒットエフェクト読み込み
    hitEffect = std::unique_ptr<Effect>(new Effect("Data/Effect/Hit.efk"));
}

void Enemy::Update(float elapsedTime)
{
    UpdateDelayTime(projectile_auto.checker, projectile_auto.time, DELAYAUTOTIME);
    CollisionProjectileVsPlayer();
}

//デバッグプリミティブ描画
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
    // デバッグ文字列表示の変更
    std::string str = "";
    std::string subStr = "";
    // 現在のステート番号に合わせてデバッグ文字列をstrに格納
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
    ImGui::Text(u8"State　%s", str.c_str());
    ImGui::Text(u8"SubState　%s", subStr.c_str());
    Character::DrawDebugGUI();
    ImGui::Text(u8"MapOldID　%d", old_mapID);
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

//破棄
void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}


void Enemy::MoveToTarget(float elapsedTime, float speedRate)
{
    // ターゲット方向への進行ベクトルを算出
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;
    // 移動処理
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

//攻撃範囲内に入ったか
bool Enemy::InAttackRange()
{
    // 目的地点までのXZ平面での距離判定
    DirectX::XMFLOAT3 player_position = Player::Instance().GetPosition();
    float vx = player_position.x - position.x;
    float vz = player_position.z - position.z;
    float distSq = vx * vx + vz * vz;
    // 攻撃範囲内までプレイヤーに近づいた
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
    Player& player= Player::Instance();
    Character::CollisionProjectileVsCharacter(&player, *hitEffect);
}
