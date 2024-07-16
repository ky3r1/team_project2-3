#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include "Graphics/sprite.h"
#include "Audio/Audio.h"


class Player : public Character
{
public:
    Player();
    ~Player() override;
public:
    static Player& Instance();

    //�X�V����
    void Update(float olapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�f�o�b�O�pGUI
    void DrawDebugGUI() override; 
    
    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec() const;
private:
    //�����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime)override;



    //�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

    void CollisionProjectilesVsEnemies();

    //�e�ۓ��͏���
    void InputProjectile();

    //�U���͈͗p�̍��W�ϊ�
    void AreaTransform();
private:
    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();
    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    //�ړ����͏���
    bool InputMove(float elapsedTime);
    //�ړ��X�e�[�g�֑J��
    void TransitionMoveState();
    //�ړ��X�e�[�g�X�V����
    void UpdateMoveState(float elapsedTime);

    //�U�����͏���
    bool InputAttack();
    //�U���X�e�[�g�֑J��
    void TransitionAttackState();
    //�U���X�e�[�g�X�V
    void UpdateAttackState(float elapsedTime);

    //�_���[�W���󂯂���
    void OnDamaged()override;
    //�_���[�W�X�e�[�g�J��
    void TransitionDamageState();
    //�_���[�W�X�e�[�g�X�V
    void UpdateDamageState(float elapsedTime);

public:
    //���S������
    void OnDead()override;
private:
    //���S�X�e�[�g�J��
    void TransitionDeathState();
    //���S�X�e�[�g�X�V
    void UpdateDeathState(float elapsedTime);

public:

    //�T�u�U���͈͂̃Q�b�^�[
    float GetSubAttackRange()
    {
        return sub_attack_range;
    }

    int GetProjectileType()
    {
        return projectile_type;
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

    std::unique_ptr<AudioSource> se;


    ////�A�j���[�V����:Jammo
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
    ////�A�j���[�V����:UnityChan
    enum Animation
    {
        Anim_Waiting,
        Anim_GetHit,
        Anim_Running,
        Anim_Attack,
        Anim_Death,
    };

    //�X�e�[�g
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