#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include "Graphics/sprite.h"


class Player : public Character
{
public:
    Player();
    ~Player() override;
public:
    static Player& Instance();

    //更新処理
    void Update(float olapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //デバッグ用GUI
    void DrawDebugGUI() override;
private:
    //垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime)override;

    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;

    //プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

    void CollisionProjectilesVsEnemies();

    //弾丸入力処理
    void InputProjectile();

    //攻撃範囲用の座標変換
    void AreaTransform();
private:
    //待機ステートへ遷移
    void TransitionIdleState();
    //待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    //移動入力処理
    bool InputMove(float elapsedTime);
    //移動ステートへ遷移
    void TransitionMoveState();
    //移動ステート更新処理
    void UpdateMoveState(float elapsedTime);

    //攻撃入力処理
    bool InputAttack();
    //攻撃ステートへ遷移
    void TransitionAttackState();
    //攻撃ステート更新
    void UpdateAttackState(float elapsedTime);

    //ダメージを受けた時
    void OnDamaged()override;
    //ダメージステート遷移
    void TransitionDamageState();
    //ダメージステート更新
    void UpdateDamageState(float elapsedTime);

public:
    //死亡した時
    void OnDead()override;
private:
    //死亡ステート遷移
    void TransitionDeathState();
    //死亡ステート更新
    void UpdateDeathState(float elapsedTime);

public:

    //サブ攻撃範囲のゲッター
    float GetSubAttackRange()
    {
        return sub_attack_range;
    }
private:
    Model* area = nullptr;
    DirectX::XMFLOAT3 area_scale = {};
    float       moveSpeed = 7.0f;
    float       jumpSpeed = 20.0f;
    int projectile_shot;

    int         jumpCount = 0;
    int         jumpLimit = 2;
    DelayTime projectile_allangle;
    DelayTime projectile_auto;
    DelayTime projectile_front;

    int nearest_enemy_index = -1;
    float current_nearest_distance = FLT_MAX;
    int ricochet_count = 30;

    ProjectileManager projectileManager;

    Effect* hitEffect = nullptr;
    Effect* AT_Field = nullptr;

    GamePad& gamePad = Input::Instance().GetGamePad();

    ////アニメーション:Jammo
    //enum Animation
    //{
    //    Anim_Attack,
    //    Anim_Death,
    //    Anim_Falling,
    //    Anim_GetHit1,
    //    Anim_GetHit2,
    //    Anim_Idle,
    //    Anim_Jump,
    //    Anim_Jump_Flip,
    //    Anim_Landing,
    //    Anim_Revive,
    //    Anim_Running,
    //    Anim_Walking,
    //};
    ////アニメーション:UnityChan
    enum Animation
    {
        Anim_Attack,
        Anim_Death,
        Anim_GetHit,
        Anim_Idle,
        Anim_IdleAnim,
        Anim_Running,
    };

    //ステート
    enum class State
    {
        Idle,
        Move,
        Attack,
        Damage,
        Death,
    };
    State state = State::Idle;
};