#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "MouseManager.h"

//#include "StageMapChip.h"

static Player* instance = nullptr;

Player& Player::Instance()
{
    return *instance;
}

//TODO:弾のDelayTime
#define DELAYAUTOTIME 70
#define DELAYALLANGLETIME 60
#define DELAYFRONTTIME 60

//TODO:PlayerDamageの当たり判定のDelayTime
#define DELAYPLAYERVSENEMY 60

Player::Player()
{
    instance = this;
    //TODO:プレイヤーのステータス設定
    //model = new Model("Data/Model/UnitychanSD/UnitychanSD.mdl");
    //scale.x = scale.y = scale.z = 0.02f;
    //model = new Model("Data/Model/Dragon/dragon.mdl");
    model = new Model("Data/Model/GP5_UnityChan/unitychan.mdl");
    area = new Model("Data/Model/Player/Area.mdl");
    area_scale = { 0.1f,1.0f,0.1f };
    //scale.x = scale.y = scale.z = 0.1f;
    scale.x = scale.y = scale.z = 1.0f;
    turnSpeed = DirectX::XMConvertToRadians(720);
    weight = 100.0f;
    color = { 1,1,1,1 };

    projectile_auto.time = DELAYAUTOTIME;

    hit_delay.time = DELAYPLAYERVSENEMY;
    moveSpeed = 15.0f;

    position = { 0.0001f,2,0 };
    attack_range = 9.0f;
    sub_attack_range = 1.0f;

    projectile_category = 0;;
    state = State::Idle;

    // エフェクト
    AT_Field = new Effect("Data/Effect/AT_field.efk");

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/Hit.efk");
    lineEffect = std::unique_ptr<Effect>(new Effect("Data/Effect/PlayerLine.efkefc"));
    //hitEffect = std::unique_ptr<Effect>(new Effect("Data/Effect/GP3_sample.efk"));
    category = PLAYERCATEGORY;
    turnSpeed = DirectX::XMConvertToRadians(900);
    ProjectileManager& projectile_manager = ProjectileManager::Instance();
}

Player::~Player()
{
    delete AT_Field;

    delete hitEffect;
    hitEffect = nullptr;
    delete model;
    model = nullptr;
    delete area;
    area = nullptr;
}


void Player::Update(float elapsedTime)
{
    //すべての敵と総当たりで衝突判定
    //int enemyCount = EnemyManager::Instance().GetEnemyCount();
    //for (int i = 0; i < enemyCount; i++)
    //{
    //    Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
    //}
    //色変え
    //ChangeColor(color, category);

    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    }

    if (invincible == true)
    {        
        invincible_timer -= elapsedTime;

        if (invincible_timer <= 0.0f)
        {
            invincible = false;
            invincible_timer = 0.1f;
        }
    }
    Mouse& mouse = Input::Instance().GetMouse();

    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        projectile_type++;
    }
    if (projectile_type == 0)
    {
        projectile_category = PENETRATION;
    }
    if (projectile_type == 1)
    {
        projectile_category = RICOCHET;
    }
    if (projectile_type >= 2)
    {
        projectile_type = 0;
    }
    //GamePad gamePad=

    //速力処理更新
    UpdateVelocity(elapsedTime);

    //弾丸更新処理
    ProjectileManager::Instance().Update(elapsedTime);

    //プレイヤーと敵との衝突処理
    CollisionPlayerVsEnemies();

    //弾丸と敵の衝突処理
    CollisionProjectilesVsEnemies();

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
    //モデルアニメーション更新
    model->UpdateAnimation(elapsedTime);

    AT_Field->Play(position, 10.0f);

    //当たり判定のdelay
    UpdateDelayTime(hit_delay.checker, hit_delay.time, DELAYPLAYERVSENEMY);
    //オートで出てる弾のdelay
    UpdateDelayTime(projectile_auto.checker, projectile_auto.time, DELAYAUTOTIME);
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model, color);

    ////攻撃範囲行列を更新01
    //area_scale = { 0.1f,1.0f,0.1f };
    //AreaTransform();
    //area->UpdateTransform(transform);
    //shader->Draw(dc, area, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));

    ////攻撃範囲行列を更新02
    //area_scale = { 0.5f,0.9f,0.5f };
    //AreaTransform();
    //area->UpdateTransform(transform);
    //shader->Draw(dc, area, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));

    //弾丸描画処理
    ProjectileManager::Instance().Render(dc, shader);
}

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 0, 0, 1));

    //衝突判定用のデバッグ立方体を描画
    //debugRenderer->DrawCube({ -12,-10,29 }, { 12,-5,31 }, { 1,0,0,1 });
    // 攻撃範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, attack_range, 1.0f, DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f));
    debugRenderer->DrawCylinder(position, sub_attack_range, 1.0f, DirectX::XMFLOAT4(0.5f, 0.3f, 0.3f, 1.0f));
    //弾丸デバッグプリミティブ描画
    ProjectileManager::Instance().DrawDebugPrimitive();
}
//デバッグGUI
void Player::DrawDebugGUI()
{

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    // デバッグ文字列表示の変更
    std::string str = "";
    // 現在のステート番号に合わせてデバッグ文字列をstrに格納
    switch (state)
    {
    case State::Idle:
        str="Idle";
        break;
    case State::Move:
        str = "Move";
        break;
    case State::Attack:
        str = "Attack";
        break;
    case State::Damage:
        str = "Damage";
        break;
    case State::Death:
        str = "Death";
        break;
    }
    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        Character::DrawDebugGUI();
        if (ImGui::TreeNode("Transform"))
        {
            ImGui::Text(u8"State:%s", str.c_str());
            ImGui::SliderFloat3("position", &position.x, -5, 5);
            ImGui::SliderFloat3("scale", &scale.x, 0.01f, 4.0f);
            ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);
            ImGui::SliderInt("health", &health, 0.0f, 10.0f);
            ImGui::SliderFloat("movespeed", &moveSpeed, 0.0f, 20.0f);

            ImGui::SliderInt("delay_auto_time", &projectile_auto.time, 0.0f, DELAYAUTOTIME);

            ImGui::TreePop();
        }
    }
    ImGui::End();

    ProjectileManager::Instance().DrawDebugGUI();
}

//プレイヤーの移動方向を取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //入力情報の取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //カメラ方向とスティックの入力によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //移動ベクトルはXY平面に水平なベクトルになるようにする

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //スティックの水平入力値をカメラの右方向に反映し、
    //スティックの垂直入力値をカメラの前方向に反映し、
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}


//プレイヤーと地面の衝突
void Player::UpdateVerticalMove(float elapsedTime)
{
    Character::UpdateVerticalMove(elapsedTime);
    if (position.y < -15)
    {
        health = 0;
    }
    if (Collision::InPoint({ -12,-10,29 }, { 12,-5,31 }, position))
    {
        //StageMapChip::Instance().SetStageNum(StageMapChip::Instance().GetStageNum() + 1);
    }
}
//プレイヤーと敵の衝突
void Player::CollisionPlayerVsEnemies()
{
    ////すべての敵と総当たりで衝突判定
    //int enemyCount = enemyManager.GetEnemyCount();
    //for (int i = 0; i < enemyCount; i++)
    //{
    //    Enemy* enemy = enemyManager.GetEnemy(i);
    Enemy* enemy = EnemyManager::Instance().NearEnemy(position);
        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (enemy == nullptr);
        else if (Collision::IntersectCylinderVsSphere(
            position, radius, height, weight,
            enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(), enemy->GetWeight(),
            outPosition))
        {
            enemy->SetPosition(outPosition);
#ifdef ENEMYHITTINGDAMAGE
        if (hit_delay.checker)
        {
            //吹き飛ばす
            {
                //吹き飛ばす力
                const float power = 10.0f;

                //敵の位置
                DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
                //プレイヤーの位置
                DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&position);
                //弾から敵への方向ベクトルを計算（敵 - 弾）
                auto v = DirectX::XMVectorSubtract(pVec, eVec);
                //方向ベクトルを正規化
                v = DirectX::XMVector3Normalize(v);

                DirectX::XMFLOAT3 vec;
                DirectX::XMStoreFloat3(&vec, v);

                velocity.x += power * vec.x;
                velocity.y += power * 0.5f;
                velocity.z += power * vec.z;
            }
        }
#endif // ENEMYHITTINGDAMAGE
    }
    //}
}

//プレイヤーと弾丸の衝突
void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    //すべての弾丸とすべての敵をそう当たりで衝突判定
    int enemyCount = enemyManager.GetEnemyCount();
    for (int j = 0; j < enemyCount; ++j)
    {
        Enemy* enemy = enemyManager.GetEnemy(j);
        Character::CollisionProjectileVsCharacter(enemy, *hitEffect);
    }
}

//弾入力処理
void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
        //前方弾丸発射
        if (projectile_auto.checker)
        {
            if (Collision::PointInsideCircle(enemy->GetPosition(), position, sub_attack_range))
            {
                penetration_count = 30;
                ricochet_count = 1;
                ProjectileStraightShotting(PLAYERCATEGORY, 0.0f, FRONT);
                projectile_auto.checker = false;
            }
            else if (Collision::PointInsideCircle(enemy->GetPosition(), position, attack_range))
            {
                ProjectileStraightShotting(PLAYERCATEGORY, 0.0f, FRONT);
                projectile_auto.checker = false;
            }
        }
    }
}

void Player::AreaTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(area_scale.x, area_scale.y, area_scale.z);
    //DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.1, 0.1, 0.1);
    //回転行列を作成
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //3つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;
    //計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

//待機ステート
void Player::TransitionIdleState()
{
#ifdef PLAYERANIMATION
    state = State::Idle;
    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
#endif // PLAYERANIMATION
}

void Player::UpdateIdleState(float elapsedTime)
{
    static float Yangle = 0;
    static DirectX::XMFLOAT3 e_pos = {};
    //移動入力処理
    if (InputMove(elapsedTime))
    {
        TransitionMoveState();
    }
    //攻撃処理
    //すべての敵を検索し、敵が攻撃範囲内に入ったら攻撃ステートに遷移
    Enemy* enemy = EnemyManager::Instance().NearEnemy(position);
    if (enemy == nullptr)e_pos=position;
    else
    {
        e_pos=enemy->GetPosition();
        if (Collision::PointInsideCircle(enemy->GetPosition(), position, attack_range))
        {
            if (projectile_auto.checker)
            {
                if (Yangle < angle.y + 0.006f && Yangle > angle.y - 0.006f)
                {
                    TransitionAttackState();
                }
                else
                {
                    Yangle = angle.y;
                }
            }
        }
    }
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR NE = DirectX::XMLoadFloat3(&e_pos);
    DirectX::XMVECTOR Vec = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(NE, Pos));
    DirectX::XMFLOAT3 ND;
    DirectX::XMStoreFloat3(&ND, Vec);
    //旋回処理
    Turn(elapsedTime, ND.x, ND.z, turnSpeed);
}

//移動ステート
bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    if (moveVec.x != 0 /*|| moveVec.y != 0 */ || moveVec.z != 0)return true;
    return false;
}
void Player::TransitionMoveState()
{
    state = State::Move;
    //走りアニメーション再生
    model->PlayAnimation(Anim_Running, true);
}
void Player::UpdateMoveState(float elapsedTime)
{
    Turn(elapsedTime, GetMoveVec().x, GetMoveVec().z, turnSpeed);

    //移動入力処理
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }
}

//攻撃ステート
bool Player::InputAttack()
{
    bool result = false;
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        result = true;
    }
    return result;
}
void Player::TransitionAttackState()
{
    state = State::Attack;
    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}
void Player::UpdateAttackState(float elapsedTime)
{
    //最も近い敵を総当たりで探索
    Enemy* ne = EnemyManager::Instance().NearEnemy(position);
    if (ne == nullptr)TransitionIdleState();
    else
    {
        DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR NE = DirectX::XMLoadFloat3(&ne->GetPosition());
        DirectX::XMVECTOR Vec = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(NE, Pos));
        DirectX::XMFLOAT3 ND;
        DirectX::XMStoreFloat3(&ND, Vec);

        //旋回処理
        Turn(elapsedTime, ND.x, ND.z, turnSpeed);

        //任意のアニメーション区間でのみ衝突処理
        float animationTime = 0.138;
        //attackCollisionFlag = animationTime ? true : false;
        //if (attackCollisionFlag)    CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
#ifdef PLAYERATTACK
        InputProjectile();
#endif // PLAYERATTACK
        //攻撃モーションが終わったら待機モーションに移動
        if (InputMove(elapsedTime))
        {
            TransitionMoveState();
        }
        if (!model->IsPlayAnimation())
        {
            TransitionIdleState();
        }
    }
}

//ダメージステート
void Player::OnDamaged()
{
    TransitionDamageState();
}
void Player::TransitionDamageState()
{
    state = State::Damage;
    model->PlayAnimation(Anim_GetHit, false);
}
void Player::UpdateDamageState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

//死亡ステート
void Player::OnDead()
{
    if (health <= 0)
    {
        TransitionDeathState();
    }
}
void Player::TransitionDeathState()
{
    state = State::Death;
    model->PlayAnimation(Anim_Death, false);
}
void Player::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {

    }
}