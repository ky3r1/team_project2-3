#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Mathf.h"
#include "Player.h"
#include "StageMapChip.h"

//デバッグプリミティブ描画
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

    // デバッグ文字列表示の変更
    std::string mapstr = "";
    // 現在のステート番号に合わせてデバッグ文字列をstrに格納
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
    ImGui::Text(u8"State　%s", str.c_str());
    ImGui::Text(u8"SubState　%s", subStr.c_str());
    ImGui::Text(u8"MapChip　%s", mapstr.c_str());
#endif // ENEMYSTATEMACHINE
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
    if (distSq < attackRange * attackRange)
    {
        return true;
    }
    false;
}