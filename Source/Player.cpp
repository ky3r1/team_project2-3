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

#include "StageMapChip.h"

static Player* instance = nullptr;

Player& Player::Instance()
{
    return *instance;
}

//TODO:弾のDelayTime
#define DELAYAUTOTIME 40
#define DELAYALLANGLETIME 60
#define DELAYFRONTTIME 60

//TODO:PlayerDamageの当たり判定のDelayTime
#define DELAYPLAYERVSENEMY 60

Player::Player()
{
    instance = this;
    //TODO:プレイヤーのステータス設定
    model = new Model("Data/Model/Jammo/Jammo.mdl");

    scale.x = scale.y = scale.z = 0.013f;

    color = { 1,0,0,1 };

    projectile_auto.time = DELAYAUTOTIME;
    projectile_allangle.time = DELAYALLANGLETIME;
    projectile_front.time = DELAYFRONTTIME;

    hit_delay.time = DELAYPLAYERVSENEMY;
    moveSpeed = 5.0f;

    position = { 0.0001f,2,0 };

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/Hit.efk");
    category = WHITE;
    projectile_shot = 0;
    turnSpeed = DirectX::XMConvertToRadians(720);
}

Player::~Player()
{
    delete hitEffect;
    hitEffect = nullptr;
    delete model;
    model = nullptr;
}


void Player::Update(float elapsedTime)
{
    //すべての敵と総当たりで衝突判定
    int enemyCount = EnemyManager::Instance().GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
    }
    //色変え
    ChangeColor(color, category);

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

    //速力処理更新
    UpdateVelocity(elapsedTime);

    //弾丸更新処理
    projectileManager.Update(elapsedTime);

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

    //当たり判定のdelay
    if (!hit_delay.checker)
    {
        hit_delay.time--;
    }
    if (hit_delay.time < 0)
    {
        hit_delay.checker = true;
        hit_delay.time = DELAYAUTOTIME;
    }

    //オートで出てる弾のdelay
    if (!projectile_auto.checker)
    {
        projectile_auto.time--;
    }
    if (projectile_auto.time < 0)
    {
        projectile_auto.checker = true;
        projectile_auto.time = DELAYAUTOTIME;
    }

    //前方の弾のdelay
    if (!projectile_front.checker)
    {
        projectile_front.time--;
    }
    if (projectile_front.time < 0)
    {
        projectile_front.checker = true;
        projectile_front.time = DELAYFRONTTIME;
    }

    //周囲に出す弾のdelay
    if (!projectile_allangle.checker)
    {
        projectile_allangle.time--;
    }
    if (projectile_allangle.time < 0)
    {
        projectile_allangle.checker = true;
        projectile_allangle.time = DELAYALLANGLETIME;
    }

    if (color_count == 0)
    {
        category = WHITE;
    }
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model, color);

    //弾丸描画処理
    projectileManager.Render(dc, shader);
}

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    //衝突判定用のデバッグ円柱を描画
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
    //衝突判定用のデバッグ立方体を描画
    debugRenderer->DrawCube({ -12,-10,29 }, { 12,-5,31 }, { 1,0,0,1 });
    //弾丸デバッグプリミティブ描画
    projectileManager.DrawDebugPrimitive();

}
//デバッグGUI
void Player::DrawDebugGUI()
{

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        Character::DrawDebugGUI();
        if (ImGui::TreeNode("Transform"))
        {
            ImGui::SliderFloat3("position", &position.x, -5, 5);
            ImGui::SliderFloat3("scale", &scale.x, 0.01f, 4.0f);
            ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);
            ImGui::SliderInt("health", &health, 0.0f, 10.0f);
            ImGui::SliderFloat("movespeed", &moveSpeed, 0.0f, 20.0f);

            ImGui::SliderInt("delay_auto_time", &projectile_auto.time, 0.0f, DELAYAUTOTIME);
            ImGui::SliderInt("delay_front_time", &projectile_front.time, 0.0f, DELAYFRONTTIME);
            ImGui::SliderInt("delay_allangle_time", &projectile_allangle.time, 0.0f, DELAYALLANGLETIME);

            ImGui::TreePop();
        }
    }
    ImGui::End();

    projectileManager.DrawDebugGUI();
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
        StageMapChip::Instance().SetStageNum(StageMapChip::Instance().GetStageNum() + 1);
    }
}
//プレイヤーと敵の衝突
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての敵と総当たりで衝突判定
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsSphere(
            position, radius,height,
            enemy->GetPosition(), enemy->GetRadius(),enemy->GetHeight(),
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

                if (category == WHITE)
                {
                    category = enemy->GetCategory();
                    color_count = 5;
                }

                if (category == enemy->GetCategory())
                {
                }
                else
                {
                    health--;
                    hitEffect->Play(position,2.0f);
                }
                hit_delay.checker = false;
            }
#endif // ENEMYHITTINGDAMAGE
            if (position.y >= (enemy->GetPosition().y + enemy->GetHeight())-0.1f)
            {
                Jump(jumpSpeed);
#ifdef JUMPDAMAGE
                enemy->ApplyDamage(1, 0.5f);
#endif//JUMPDAMAGE
            }
        }
    }
}
//プレイヤーと弾丸の衝突
void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての弾丸とすべての敵をそう当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                if (projectile->GetProectileCategory() == enemy->GetCategory())
                {
                    color_count--;
                    //弾丸破棄
                    projectile->Destroy();
#ifdef PROJECTILEDAMAGE
                    //ダメージを与える
                    if (enemy->ApplyDamage(1, 0.5f))
                    {
                        //吹き飛ばす
                        {
                            DirectX::XMFLOAT3 impulse;
                            //吹き飛ばす力
                            const float power = 10.0f;

                            //敵の位置
                            DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
                            //弾の位置
                            DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&projectile->GetPosition());
                            //弾から敵への方向ベクトルを計算（敵 - 弾）
                            auto v = DirectX::XMVectorSubtract(eVec, pVec);
                            //方向ベクトルを正規化
                            v = DirectX::XMVector3Normalize(v);

                            DirectX::XMFLOAT3 vec;
                            DirectX::XMStoreFloat3(&vec, v);

                            impulse.x = power * vec.x;
                            impulse.y = power * 0.5f;
                            impulse.z = power * vec.z;

                            enemy->AddImpulse(impulse);
                        }

                        //ヒットエフェクト再生
                        {
                            DirectX::XMFLOAT3 e = enemy->GetPosition();
                            e.y += enemy->GetHeight() * 0.5f;
                            hitEffect->Play(e,2.0f);
                        }
                    }
#endif // PROJECTILEDAMAGE
                }
                else if (projectile->GetProectileCategory() == WHITE)
                {
                    category = enemy->GetCategory();
                    color_count = 5;
                    projectile->Destroy();
                }
                else
                {
                    //弾丸破棄
                    projectile->Destroy();
                }
            }
        }
    }
}


void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    //前方弾丸発射
    if (mouse.GetButton() & Mouse::BTN_RIGHT)
    {
        if (projectile_auto.checker)
        {
            ProjectileStraightFront(WHITE, 0.0f);
            projectile_auto.checker = false;
        }
    }
    if(category!=WHITE)
    {
        if (mouse.GetButton() & Mouse::BTN_LEFT)
        {
            if (projectile_shot == 0)
            {
                if (projectile_front.checker)
                {
                    for (int index = 0; index < 3; index++)
                    {
                        switch (index)
                        {
                        case 0:
                            ProjectileStraightFront(category, 0.0f);
                            break;
                        case 1:
                            ProjectileStraightFront(category, 0.3f);
                            break;
                        case 2:
                            ProjectileStraightFront(category, -0.3f);
                            break;
                        default:
                            break;
                        }
                    }
                    projectile_front.checker = false;
                }
            }
        }
            //直進弾丸発射
        if (mouse.GetButton() & Mouse::BTN_LEFT && projectile_allangle.checker)
        {
            if (projectile_shot == 1)
            {
                for (int index = 0; index < 10; index++)
                {
                    switch (index)
                    {
                    case 0:
                        ProjectileStraightFront(category, 0.0f);
                        break;
                    case 1:
                        ProjectileStraightFront(category, 0.9);
                        break;
                    case 2:
                        ProjectileStraightFront(category, 3.0);
                        break;
                    case 3:
                        ProjectileStraightFront(category, -0.9);
                        break;
                    case 4:
                        ProjectileStraightFront(category, -3.0);
                        break;
                    case 5:
                        ProjectileStraightBack(category, 0.0f);
                        break;
                    case 6:
                        ProjectileStraightBack(category, 0.9f);
                        break;
                    case 7:
                        ProjectileStraightBack(category, 3.0f);
                        break;
                    case 8:
                        ProjectileStraightBack(category, -0.9f);
                        break;
                    case 9:
                        ProjectileStraightBack(category, -3.0f);
                        break;
                    default:
                        break;
                    }
                }
                projectile_allangle.checker = false;
            }
        }
    }
}


void Player::ProjectileStraightFront(int category, float angle)//category:弾のタイプ、angle:弾の角度
{
    //発射
    ProjectileStraight* projectile{};
    //前方向
    DirectX::XMFLOAT3 dir;
    DirectX::XMFLOAT3 dis_pos;
    DirectX::XMVECTOR Dis_pos;
    DirectX::XMMATRIX Right_;
    DirectX::XMFLOAT3 r;
    DirectX::XMFLOAT3 axis = { 0,1,0 };
    DirectX::XMVECTOR Axis;

    float dist = FLT_MAX;
    DirectX::XMFLOAT3 enemy_position = {};
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int index = 0; index < enemyCount; index++)
    {
        Enemy* enemy = EnemyManager::Instance().GetEnemy(index);
        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
        DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
        float d;
        DirectX::XMStoreFloat(&d, D);
        if (d < dist)
        {
            enemy_position = enemy->GetPosition();
        }
    }

    dir.x = transform._31 * 100.0f;
    dir.y = 0.0f;
    dir.z = transform._33 * 100.0f; 
    DirectX::XMFLOAT3 right;
    right.x = transform._11 * 100.0f; 
    right.y = 0.0f;
    right.z = transform._13 * 100.0f; 
    //発射位置（プレイヤーの腰当たり）
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
void Player::ProjectileStraightBack(int category, float angle)
{
    //発射
    ProjectileStraight* projectile{};
    //前方向
    DirectX::XMFLOAT3 dir;
    dir.x = -transform._31 * 100.0f;
    dir.y = 0.0f;
    dir.z = -transform._33 * 100.0f;
    DirectX::XMFLOAT3 right;
    right.x = transform._11 * 100.0f;
    right.y = 0.0f;
    right.z = transform._13 * 100.0f;
    //発射位置（プレイヤーの腰当たり）
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
    //移動入力処理
    if (InputMove(elapsedTime))
    {
        TransitionMoveState();
    }

    //攻撃処理
    if (InputAttack())
    {
        TransitionAttackState();
    }

    //弾丸入力処理
    InputProjectile();
}

//移動ステート
bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
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
    //移動入力処理
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }

    //攻撃処理
    if (InputAttack())
    {
        TransitionAttackState();
    }

    //弾丸入力処理
    InputProjectile();
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
    //走りアニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}
void Player::UpdateAttackState(float elapsedTime)
{
    //任意のアニメーション区間でのみ衝突処理
    float animationTime = 0.138;
    //attackCollisionFlag = animationTime ? true : false;
    //if (attackCollisionFlag)    CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    //攻撃モーションが終わったら待機モーションに移動
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
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
    model->PlayAnimation(Anim_GetHit1, false);
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