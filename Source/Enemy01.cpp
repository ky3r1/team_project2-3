#include "Enemy01.h"
#include "StateDerived.h"
//#include "StageMapChip.h"
#include "ProjectileManager.h"

//TODO:�e��DelayTime
#define DELAYAUTOTIME 1*60

//�R���X�g���N�^
Enemy01::Enemy01(int category)
{
    //TODO:�G�l�~�[�X���C���̃X�e�[�^�X�ݒ�
    model = new Model("Data/Model/Enemy/enemy1/enemy1.mdl");
    lineEffect = std::unique_ptr<Effect>(new Effect("Data/Effect/EnemyLine.efkefc"));
    static int id_enemy01 = 0;
    id_enemy01++;
    category_id = id_enemy01;
    enemy_categry = Enemy01_Num;
    name = std::string("Enemy01:") + std::to_string(category_id);
    //�\���T�C�Y�𒲐�
    scale.x = scale.y = scale.z = 0.03f;

    radius = 0.5f;//�����蔻��̕��A���a
    height = 1.0f;//�����蔻��̍���
    health = 3;
    attack_range = 5.0f;
    this->category = category;
    //ChangeColor(color, category);

#ifdef ENEMYSTATEMACHINE
    // StateMachine�𐶐����A�K�w�^�X�e�[�g�}�V���ɑΉ�����悤�ɓo�^�X�e�[�g��ύX���Ă����B
    stateMachine = new StateMachine();
    // �X�e�[�g�}�V���ɃX�e�[�g�o�^
    stateMachine->RegisterState(new SearchState(this));
    stateMachine->RegisterState(new BattleState(this));
    // �e�e�X�e�[�g�ɃT�u�X�e�[�g��o�^
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Search), new IdleState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Battle), new PursuitState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Battle), new AttackState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Battle), new BattleIdleState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Battle), new HitDamageState(this));
    // �f�t�H���g�X�e�[�g���Z�b�g
    stateMachine->SetState(static_cast<int>(State::Search));
#endif // ENEMYSTATEMACHINE
}

//�f�X�g���N�^
Enemy01::~Enemy01()
{
    delete model;
}

//�X�V����
void Enemy01::Update(float elapsedTime)
{
    Enemy::Update(elapsedTime);
#ifdef ENEMYSTATEMACHINE
    //�X�e�[�g�X�V
    stateMachine->Update(elapsedTime);
#endif // ENEMYSTATEMACHINE

    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //���G���ԍX�V
    UpdateInvincibleTime(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();
    //���f���s��X�V
    model->UpdateTransform(transform);
    model->UpdateAnimation(elapsedTime);

    UpdateDelayTime(projectile_auto.checker, projectile_auto.time, DELAYAUTOTIME);
}

//�`�揈��
void Enemy01::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    Enemy::Render(dc, shader);
    //���f���`��
    shader->Draw(dc, model, color);
}

void Enemy01::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if (projectile_auto.checker)
    {
        projectile_category = NORMAL;
        ProjectileStraightShotting(ENEMYCATEGORY, 0.0f, FRONT);
        projectile_auto.checker = false;
    }
}

//���S�����Ƃ��ɌĂ΂��
void Enemy01::OnDead()
{
    Destroy();
}

void Enemy01::DrewDebugPrimitive()
{
    // ���N���X�̃f�o�b�O�v���~�e�B�u�`��
    Enemy::DrewDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    // �^�[�Q�b�g�ʒu���f�o�b�O���`��
    debugRenderer->DrawSphere(targetPosition, 1.3f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    // �U���͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, attack_range, 1.0f, DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f));
}
