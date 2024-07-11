#include "Character.h"
#include "StageManager.h"
//#include "StageMapChip.h"
#include "Mathf.h"
#include "ProjectileManager.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "EnemyManager.h"


//行列更新処理
void Character::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
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

//ジャンプ処理
void Character::Jump(float speed)
{
    velocity.y = speed;
}

void Character::Move(float vx, float vz, float speed)
{
    //移動方向ベクトル
    moveVecX = vx;
    moveVecZ = vz;

    //最大速度設定
    maxMoveSpeed = speed;
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //進行ベクトルがゼロベクトルの場合は処理する必要なし
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.0001f) return;

    //進行ベクトルを単位ベクトル化
    vx /= length;
    vz /= length;


    //自身の回転値から前方向を求める
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //回転角を求めるため、2つの単位ベクトルの内積を計算する
    float dot = (vx * frontX) + (vz * frontZ);

    //内積値は-1.0～1.0で表現されており、2つの単位ベクトルの角度が
    // 小さいほど1.0に近づくという性質を利用して回転速度を調整する
    float rot = 1.0 - dot;
    if (rot > speed) { rot = speed; }
    //speed *= rot;
    //左右判定を行うために2つの単位ベクトルの外積を計算する
    float cross = (vz * frontX) - (vx * frontZ);

    //2Dの外積値が正の場合か負の場合によって左右判定が行える
    //左右判定を行うことによって左右回転を選択する

    if (cross < 0.0f)
    {
        angle.y += rot;
    }
    else
    {
        angle.y -= rot;
    }
}

//速力処理更新
void Character::UpdateVelocity(float elapsedTime)
{
    //経過フレーム
    float elapsedFrame = 60.0f * elapsedTime;

    //垂直速力更新処理
    UpdateVerticalVelocity(elapsedFrame);

    //水平速力更新処理
    UpdateHorizontalVelocity(elapsedFrame);

    //垂直移動更新処理
    UpdateVerticalMove(elapsedTime);

    //水平移動更新処理
    UpdateHorizontalMove(elapsedTime);
}

//無敵時間
void Character::UpdateInvincibleTime(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

//ダメージを与える
bool Character::ApplyDamage(int damage, float invincibleTime)
{
    //　ダメージが０の場合は健康状態を変更する必要がない
    if (damage == 0) return false;

    //死亡している場合は健康状態を変更しない
    if (health <= 0) return false;
    if (invincibleTimer <= 0)
    {
        //ダメージ処理
        health -= damage;
        invincibleTimer = invincibleTime;
        //死亡通知
        if (health <= 0)
        {
            OnDead();
        }
        else
        {
            OnDamaged();
        }
        //健康状態が変更した場合はtrueを返す
        return true;
    }
}

//衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    //速力に力を加える
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

void Character::DrawDebugGUI()
{
}

//初期化
void Character::Clear()
{
    health = maxHealth;
}

//垂直速力処理
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    ////重力処理
    velocity.y += gravity * elapsedFrame;
}

//垂直移動更新処理
void Character::UpdateVerticalMove(float elapsedTime)
{
    //垂直方向の移動量
    float my = velocity.y * elapsedTime;

    slopeRate = 0.0f;

    //キャラのY軸方向となる法線ベクトル
    DirectX::XMFLOAT3 normal = { 0,1,0 };

    //落下中
    if (my < 0.0f)
    {
        //レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        //レイの終点位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x,position.y + my ,position.z };

        //レイキャストによる地面判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //地面に接地している

            //法線ベクトル取得
            normal = hit.normal;

            position = hit.position;
            //if (position.y > 0.5)angle = hit.rotation;

            position.y = hit.position.y;


            //傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0 - (hit.normal.y / (normalLengthXZ + hit.normal.y));

            //接地した
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            //空中に浮いている
            position.y += my;
            isGround = false;
        }
    }
    //上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    //地面の向きに沿うようにXZ軸回転
    {
        //Y軸が法線ベクトル方向に向くオイラー角回転を算出
        float X, Z;
        X = atan2f(normal.z, normal.y);
        Z = -atan2f(normal.x, normal.y);

        angle.x = Mathf::Lerp(angle.x, X, 0.1f);
        angle.z = Mathf::Lerp(angle.z, Z, 0.1f);
    }
}

//水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    //　XZ平面の速力を減速する
    DirectX::XMFLOAT2 vel = { velocity.x, velocity.z };

    DirectX::XMVECTOR velVel = DirectX::XMLoadFloat2(&vel);
    float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(velVel));
    if (length > 0.0f)
    {
        //摩擦力
        float friction = this->friction * elapsedFrame;

        //空中にいるときは摩擦力を減らす
        if (isGround == false) friction *= airControl;

        //摩擦による横方向の減速処理
        if (length > friction)
        {
            //速度を単位ベクトル化
            auto v = DirectX::XMVector2Normalize(velVel);

            //単位ベクトル化した速度を　friction 分スケーリング
            v = DirectX::XMVectorScale(v, friction);

            //スケーリングしたベクトルを速度に減算
            DirectX::XMStoreFloat2(&vel, DirectX::XMVectorSubtract(velVel, v));

            velocity.x = vel.x;
            velocity.z = vel.y;
        }
        //横方向の速力が摩擦力以下になったので速力を無効化
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }

        //速力の最大値
        if (velocity.x < -5.0f)
        {
            velocity.x = -5.0f;
        }
        if (velocity.x > 5.0f)
        {
            velocity.x = 5.0f;
        }
        if (velocity.z < -5.0f)
        {
            velocity.z = -5.0f;
        }
        if (velocity.z > 5.0f)
        {
            velocity.z = 5.0f;
        }
    }

    //XZ平面の速力を加速する
    if (length <= maxMoveSpeed)
    {
        //移動ベクトルがゼロベクトルでないなら加速する
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            //加速力
            float acceleration = this->acceleration * elapsedFrame;

            if (isGround == false) acceleration *= airControl;

            //移動ベクトルによる加速処理
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;



            //最大速度制限
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }

            //下り坂でガタガタしないようにする
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }
        }
    }
    //移動ベクトルをリセット
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

//水平移動更新処理
void Character::UpdateHorizontalMove(float elapsedTime)
{
    float velocityLengthXZ = sqrtf(velocity.z * velocity.z + velocity.x * velocity.x);
    if (velocityLengthXZ > 0.0f)
    {
        //水平移動値
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        //レイの開始位置と終点位置
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset, position.z };
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset,position.z + mz };

        //レイキャストによる壁判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Start, End);

            //壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            //入射ベクトルを法線に射影
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

            // 補正位置の計算
            DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(End, DirectX::XMLoadFloat3(&hit.position));

            DirectX::XMVECTOR R = DirectX::XMVectorAdd(PB, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Normal), DirectX::XMVectorGetX(Dot)));

            DirectX::XMVECTOR O = DirectX::XMVectorAdd(End, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Normal), DirectX::XMVectorGetX(Dot)));
            DirectX::XMFLOAT3 o;
            DirectX::XMStoreFloat3(&o, O);

            DirectX::XMFLOAT3 collectPosition = position;
            collectPosition.x = o.x;
            collectPosition.z = o.z;
            HitResult hit2;
            if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
            {
                position.x = o.x;
                position.z = o.z;
            }
        }
        else
        {
            //移動
            position.x += mx;
            position.z += mz;
        }
    }
}

void Character::ProjectileDirection(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //ビューポート
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    //変換行列
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


    //エネミー配置処理
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    /*if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {*/
    //マウスカーソル座標取得
    DirectX::XMFLOAT3 screenPosition;
    screenPosition.x = static_cast<float>(mouse.GetPositionX());
    screenPosition.y = 0;
    screenPosition.z = static_cast<float>(mouse.GetPositionY());

    DirectX::XMVECTOR ScreenCursor = DirectX::XMLoadFloat3(&screenPosition);

    DirectX::XMVECTOR WorldPosition = DirectX::XMVector3Unproject
    (
        ScreenCursor,
        viewport.TopLeftX,
        viewport.TopLeftY,
        viewport.Width,
        viewport.Height,
        viewport.MinDepth,
        viewport.MaxDepth,
        Projection,
        View,
        World
    );
    DirectX::XMFLOAT3 world_position_start;
    DirectX::XMStoreFloat3(&world_position_start, WorldPosition);

    screenPosition.z = 1;
    ScreenCursor = DirectX::XMLoadFloat3(&screenPosition);
    WorldPosition = DirectX::XMVector3Unproject
    (
    	ScreenCursor,
    	viewport.TopLeftX,
    	viewport.TopLeftY,
    	viewport.Width,
    	viewport.Height,
    	viewport.MinDepth,
    	viewport.MaxDepth,
    	Projection,
    	View,
    	World
    );
    DirectX::XMFLOAT3 world_position_end;
    DirectX::XMStoreFloat3(&world_position_end, WorldPosition);

    HitResult hit;
    StageMain stage_main;
    if (stage_main.RayCast(world_position_start, world_position_end, hit))
    {
    /*EnemyManager& enemyManager = EnemyManager::Instance();
    EnemySlime* slime = new EnemySlime(GREEN, 0);
    slime->SetPosition(DirectX::XMFLOAT3(world_position_start.x, world_position_start.y, world_position_start.z));
    enemyManager.Register(slime);*/
    }
    //}
}

//DelayTimeの更新
void Character::UpdateDelayTime(bool& checker, int& time, int delaytime)
{
    //checkerがfalse場合はタイムを減らす
    //checkerがtrue場合はcheckerをtrueにしてタイムをリセット
    if (!checker)
    {
        time--;
    }
    if (time < 0)
    {
        checker = true;
        time = delaytime;
    }
}


//弾とキャラクターの衝突判定
void Character::CollisionProjectileVsCharacter(Character* character, Effect hiteffect)
{
    //すべての弾丸とtargetを総当たりで衝突処理
    int projectileCount = ProjectileManager::Instance().GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        //弾丸取得
        Projectile* projectile = ProjectileManager::Instance().GetProjectile(i);
         EnemyManager& enemyManager = EnemyManager::Instance();
         int enemyCount = enemyManager.GetEnemyCount();

        //衝突判定
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            projectile->GetPosition(),
            projectile->GetRadius(),
            character->GetPosition(),
            character->GetRadius(),
            character->GetHeight(),
            outPosition))
        {
            //衝突した弾と弾を出したキャラクター(引数のcharacterではない)のカテゴリーが同じ場合
            if (category == projectile->GetCategory())
            {
                //弾丸破棄
                /*if(pro_type == NORMAL)
                {
                    if (category == PLAYERCATEGORY)
                    {
                        projectile->Destroy();
                    }
                }*/

                if (projectile_category == PENETRATION)
                {

                    if (category == PLAYERCATEGORY && penetration_count == 0)
                    {
                        projectile->Destroy();
                    }
                }
                if (projectile_category == RICOCHET)
                {
                    //if(invincible==true){}
                    for (int i = 0; i < enemyCount; i++)
                    {
                        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
                        Enemy* ne = EnemyManager::Instance().NearEnemy(enemy->GetPosition());
                        if (enemy->isHit)break;
                        enemy->isHit = true;

                        if (invincible == true)break;
                        invincible = true;
                        ProjectileRicochetShotting(character->GetPosition(), PLAYERCATEGORY, 0.0f, FRONT);
                    }
                    if (invincible == false)
                    {
                        if (ricochet_count < 0)
                        {
                            projectile->Destroy();
                            for (int i = 0; i < enemyCount; i++)
                            {
                                Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
                                if (enemy->isHit)
                                {
                                    enemy->isHit = false;
                                }
                            }
                        }
                    }

                }
                if (character->ApplyDamage(1, 0.5f))
                {
                    //吹き飛ばす
                    {
                        DirectX::XMFLOAT3 impulse;
                        //吹き飛ばす力
                        const float power = 2.0f;

                        //敵の位置
                        DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&character->GetPosition());
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
                        penetration_count--;
                        ricochet_count--;

                        character->AddImpulse(impulse);
                    }
                    //ヒットエフェクト再生
                    {
                        DirectX::XMFLOAT3 e = character->GetPosition();
                        e.y += character->GetHeight() * 0.5f;
                        hiteffect.Play(e, 2.0f);
                    }
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

//弾丸発射処理
void Character::ProjectileStraightShotting(int category, float angle, int vector)
{
    //発射
    ProjectileStraight* projectile{};
    //前方向
    DirectX::XMFLOAT3 dir;
    //DirectX::XMFLOAT3 axis = { 0,1,0 };
    //DirectX::XMVECTOR Axis;
    //DirectX::XMFLOAT3 s={ scale.x / 0.1f,scale.y / 0.1f ,scale.z / 0.1f };
    DirectX::XMFLOAT3 s = { 0.1f / scale.x,0.1f / scale.y ,0.1f / scale.z };

    dir.x = vector * transform._31 * 100.0f * s.x;
    dir.y = 0.0f;
    dir.z = vector * transform._33 * 100.0f * s.z;
    DirectX::XMFLOAT3 right;
    right.x = transform._11 * 100.0f * s.x;
    right.y = 0.0f;
    right.z = transform._13 * 100.0f * s.z;
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
    //projectile = new ProjectileStraight(&ProjectileManager::Instance(), category,lineEffect.get());
    projectile = new ProjectileStraight(&ProjectileManager::Instance(), category);
    projectile->Launch(dir, pos);
}

void Character::ProjectileRicochetShotting(DirectX::XMFLOAT3 ne,int category, float angle, int vector)
{
    Ricochet(ne, vector);
}


void Character::Ricochet(DirectX::XMFLOAT3 ne, int vector)
{
    //発射
    ProjectileStraight* projectile{};
    //前方向
    DirectX::XMFLOAT3 dir;
    //DirectX::XMFLOAT3 axis = { 0,1,0 };
    //DirectX::XMVECTOR Axis;
    //DirectX::XMFLOAT3 s={ scale.x / 0.1f,scale.y / 0.1f ,scale.z / 0.1f };
    DirectX::XMFLOAT3 s = { 0.1f / scale.x,0.1f / scale.y ,0.1f / scale.z };


    Enemy* ne1 = EnemyManager::Instance().NearEnemy(position);

    Enemy* ne2 = EnemyManager::Instance().NearEnemy(ne);

    if (ne2 == nullptr)return;
    DirectX::XMVECTOR NE1 = DirectX::XMLoadFloat3(&ne);
    DirectX::XMVECTOR NE2 = DirectX::XMLoadFloat3(&ne2->GetPosition());
    DirectX::XMVECTOR Vec = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(NE2, NE1));
    DirectX::XMFLOAT3 ND;
    DirectX::XMStoreFloat3(&ND, Vec);

    dir.x = vector * ND.x * 100.0f * s.x;
    dir.y = 0.0f;
    dir.z = vector * ND.z * 100.0f * s.z;
    DirectX::XMFLOAT3 right;
    right.x = ND.x * 100.0f * s.x;
    right.y = 0.0f;
    right.z = ND.z * 100.0f * s.z;
    //発射位置（プレイヤーの腰当たり）
    DirectX::XMFLOAT3 pos;
    pos.x = ne.x;
    pos.y = ne.y + height * 0.5f;
    pos.z = ne.z;

    DirectX::XMVECTOR Right = DirectX::XMLoadFloat3(&right);
    Right = DirectX::XMVectorScale(Right, 0.0f);
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
    projectile = new ProjectileStraight(&ProjectileManager::Instance(), category);
    projectile->Launch(dir, pos);
}


void Character::ProjectileStraightShotting(float angle, int vector)
{
    ProjectileStraightShotting(category, angle, vector);
}
