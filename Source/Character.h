#pragma once

#include "All.h"
#include "Input/Input.h"
#include "StageMain.h"
#include "Effect.h"

#define FRONT 1
#define BACK -1

//キャラクター
class Character
{
public:
    Character(){}
    virtual ~Character(){}

    //行列更新処理
    void UpdateTransform();

    //ダメージを与える
    bool ApplyDamage(int damage, float invincibleTime);

    //衝撃を与える
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    //デバッグ用GUI
    virtual void DrawDebugGUI();

    //初期化
    virtual void Clear();
protected:

    //垂直速力処理
    void UpdateVerticalVelocity(float elapsedFrame);

    //垂直移動更新処理
    virtual void UpdateVerticalMove(float elapsedTime);

    //水平速力更新処理
    void UpdateHorizontalVelocity(float elapsedFrame);

    //水平移動更新処理
    void UpdateHorizontalMove(float elapsedTime);

    // 3Dを2Dに変換
    void ProjectileDirection(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    //DelayTimeの更新
    void UpdateDelayTime(bool& checker, int& time,int delaytime);

    //弾とキャラクターの衝突判定
    void CollisionProjectileVsCharacter(Character* character, Effect hiteffect);

    //弾丸発射処理
    virtual void ProjectileStraightShotting(int category, float angle,int vector);
    virtual void ProjectileStraightShotting(float angle, int vector);

    virtual void ProjectileRicochetShotting(DirectX::XMFLOAT3 ne,int category, float angle, int vector);

    void Ricochet(DirectX::XMFLOAT3 ne,int vector);

public:
    //旋回処理
    void Turn(float elapsedTime, float vx, float vz, float speed);

protected:
    //移動処理
    //void Move(float elapsedTime, float vx, float vz, float speed);
    void Move(float vx, float vz, float speed);

    //ジャンプ処理
    void Jump(float speed);

    //速力処理更新
    void UpdateVelocity(float elapsedTime);

    //無敵時間更新
    void UpdateInvincibleTime(float elapsedTime);

    //接地したときに呼ばれる
    virtual void OnLanding() {}

    //ダメージを受けた時に呼ばれる
    virtual void OnDamaged() {}

    //死亡したときに呼ばれる
    virtual void OnDead() {}
public:
    /////////////////////ゲッター・セッター//////////////////////////

    //モデルのゲッター・セッター
    Model* GetModel() { return model; }
    void SetModel(Model* m) { model = m; }

    //positionのゲッター・セッター
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //angleのゲッター・セッター
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //scaleのゲッター・セッター
    const DirectX::XMFLOAT3& GetScale() const { return scale; }
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //radiusのゲッター
    float GetRadius() const { return radius; }

    //isGroundのゲッター
    bool IsGround()const { return isGround; }

    //heightのゲッター
    float GetHeight() const { return height; }

    //healthのゲッター
    int GetHealth()const { return health; }

    //maxHealthゲッター
    int GetMaxHealth()const { return maxHealth; }

    //categoryのゲッター
    int GetCategory()const { return category; }

    //weightのゲッター・セッター
    float GetWeight()const { return weight; }
    void SetWeight(float w) { weight = w; }

    // 攻撃範囲のゲッター・セッター
    float GetAttackRange() { return attack_range; }
    void SetAttackRange(float a) { attack_range = a; }

    //ターンスピードのゲッター
    float GetTurnSpeed() { return turnSpeed; }

    //スクリーン座標のゲッター・セッター
    void SetScreenPos(DirectX::XMFLOAT3 sp) { screen_pos = sp; }
    DirectX::XMFLOAT3 GetScreenPos() { return screen_pos; }

    //lineEffectのゲッター/セッター
    void SetLineEffect(Effect* e) { lineEffect.reset(e);}
    Effect* GetLineEffect() { return lineEffect.get(); } 
protected:
    Model* model = nullptr;
    //Effect* hitEffect = nullptr;
    std::unique_ptr<Effect>hitEffect=nullptr;
    std::unique_ptr<Effect>lineEffect = nullptr;

    DirectX::XMFLOAT3   position = { 0,0,0 };
    DirectX::XMFLOAT3   screen_pos = { 0,0,0 };
    DirectX::XMFLOAT3   angle = { 0,0,0 };
    DirectX::XMFLOAT3   scale = { 1,1,1 };
    DirectX::XMFLOAT4   color = { 1,1,1,1 };
    DirectX::XMFLOAT4X4 transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    float radius = 0.5f;
    float weight = 0.0f;
    struct DelayTime
    {
        int time;
        bool checker = false;
    };
    DelayTime hit_delay;

    float       gravity = -1.0f;
    DirectX::XMFLOAT3 velocity = { 0,0,0 };
    bool        isGround = false;
    float       height = 2.0f;
    int         health = 5;
    int         maxHealth = 5;
    float       invincibleTimer = 1.0f;
    float       friction = 0.5f;
    float       acceleration = 1.0f;
    float       maxMoveSpeed = 5.0f;
    float       moveVecX = 0.0f;
    float       moveVecZ = 0.0f;
    float       airControl = 0.3f;
    float       stepOffset = 1.0f;
    float       slopeRate = 1.0f;
    int         category = 0;
    float       turnSpeed = DirectX::XMConvertToRadians(360);
    int         penetration_count = 0;
    int         ricochet_count = 0;
    int         projectile_category = 0;
    int         projectile_type = 0;
    int         pro_type = NORMAL;

    float       invincible_timer = 0.1f;
    bool        invincible = false;
    int         count = 0;
    bool        enemy_rico_check = false;

    //攻撃範囲
    float		attack_range = 1.5f;
    float       sub_attack_range = 5.0f;

};