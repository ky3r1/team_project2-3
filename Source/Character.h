#pragma once

#include "All.h"
#include "Input/Input.h"
#include "StageMain.h"


//�L�����N�^�[
class Character
{
public:
    Character(){}
    virtual ~Character(){}

    //�s��X�V����
    void UpdateTransform();

    //�_���[�W��^����
    bool ApplyDamage(int damage, float invincibleTime);

    //�Ռ���^����
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    //�f�o�b�O�pGUI
    virtual void DrawDebugGUI();

    void SetScreenPos(DirectX::XMFLOAT3 sp) { screen_pos = sp; }

    DirectX::XMFLOAT3 GetScreenPos() { return screen_pos; }
    //������
    virtual void Clear();
protected:

    //�������͏���
    void UpdateVerticalVelocity(float elapsedFrame);

    //�����ړ��X�V����
    virtual void UpdateVerticalMove(float elapsedTime);

    //�������͍X�V����
    void UpdateHorizontalVelocity(float elapsedFrame);

    //�����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);

    // 3D��2D�ɕϊ�
    void ProjectileDirection(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

protected:
    //�ړ�����
    //void Move(float elapsedTime, float vx, float vz, float speed);
    void Move(float vx, float vz, float speed);

    //���񏈗�
    void Turn(float elapsedTime, float vx, float vz, float speed);

    //�W�����v����
    void Jump(float speed);

    //���͏����X�V
    void UpdateVelocity(float elapsedTime);

    //���G���ԍX�V
    void UpdateInvincibleTime(float elapsedTime);

    //�ڒn�����Ƃ��ɌĂ΂��
    virtual void OnLanding() {}

    //�_���[�W���󂯂����ɌĂ΂��
    virtual void OnDamaged() {}

    //���S�����Ƃ��ɌĂ΂��
    virtual void OnDead() {}

    virtual void ChangeColor(DirectX::XMFLOAT4& color, int category);
public:
    /////////////////////�Q�b�^�[�E�Z�b�^�[//////////////////////////

    //���f���̃Q�b�^�[�E�Z�b�^�[
    Model* GetModel() { return model; }
    void SetModel(Model* m) { model = m; }

    //position�̃Q�b�^�[�E�Z�b�^�[
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //angle�̃Q�b�^�[�E�Z�b�^�[
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //scale�̃Q�b�^�[�E�Z�b�^�[
    const DirectX::XMFLOAT3& GetScale() const { return scale; }
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //radius�̃Q�b�^�[
    float GetRadius() const { return radius; }

    //isGround�̃Q�b�^�[
    bool IsGround()const { return isGround; }

    //height�̃Q�b�^�[
    float GetHeight() const { return height; }

    //health�̃Q�b�^�[
    int GetHealth()const { return health; }

    //maxHealth�Q�b�^�[
    int GetMaxHealth()const { return maxHealth; }

    //category�̃Q�b�^�[
    int GetCategory()const { return category; }

    //weight�̃Q�b�^�[
    float GetWeight()const { return weight; }
    void SetWeight(float w) { weight = w; }

    // �U���͈͂̃Q�b�^�[�E�Z�b�^�[
    float GetAttackRange() { return attack_range; }
    void SetAttackRange(float a) { attack_range = a; }
protected:
    Model* model = nullptr;
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
    //�U���͈�
    float				attack_range = 1.5f;
};