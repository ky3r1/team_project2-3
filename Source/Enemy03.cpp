#include "Enemy03.h"
#include "StateDerived.h"
//#include "StageMapChip.h"
#include "ProjectileManager.h"

//TODO:�e��DelayTime
#define DELAYAUTOTIME 3*60

//�R���X�g���N�^
Enemy03::Enemy03(int category)
{
    //TODO:�G�l�~�[�X���C���̃X�e�[�^�X�ݒ�
    model = new Model("Data/Model/Jammo/Jammo.mdl");
    lineEffect = std::unique_ptr<Effect>(new Effect("Data/Effect/EnemyLine.efkefc"));

    static int id_enemy03= 0;
    id_enemy03++;
    category_id = id_enemy03;
    enemy_categry = Enemy03_Num;
    name = std::string("Enemy03:") + std::to_string(category_id);
    //�\���T�C�Y�𒲐�
    scale.x = scale.y = scale.z = 0.01f;

    radius = 0.5f;//�����蔻��̕��A���a
    height = 1.0f;//�����蔻��̍���
    health = 10.0f;//�̗�

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
    stateMachine->RegisterSubState(static_cast<int>(Enemy03::State::Search), new IdleState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy03::State::Battle), new PursuitState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy03::State::Battle), new AttackState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Battle), new BattleIdleState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Battle), new HitDamageState(this));
    // �f�t�H���g�X�e�[�g���Z�b�g
    stateMachine->SetState(static_cast<int>(State::Search));
#endif // ENEMYSTATEMACHINE
}

//�f�X�g���N�^
Enemy03::~Enemy03()
{
    delete model;
}

//�X�V����
void Enemy03::Update(float elapsedTime)
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
void Enemy03::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model, color);
}

//���S�����Ƃ��ɌĂ΂��
void Enemy03::OnDead()
{
    Destroy();
}

void Enemy03::InputProjectile()
{
    if (projectile_auto.checker)
    {
        /*ProjectileStraightShotting(ENEMYCATEGORY, -1.0f, FRONT);
        ProjectileStraightShotting(ENEMYCATEGORY, 0.0f, FRONT);
        ProjectileStraightShotting(ENEMYCATEGORY, 1.0f, FRONT);

        ProjectileStraightShotting(ENEMYCATEGORY, -1.0f, BACK);
        ProjectileStraightShotting(ENEMYCATEGORY, 0.0f, BACK);
        ProjectileStraightShotting(ENEMYCATEGORY, 1.0f, BACK);*/

        projectile_auto.checker = false;
    }
}

//void Enemy03::DrawDebugGUI()
//{
//    //name = std::string("Enemy03:") + std::to_string(category_id);
//
//    //if (ImGui::TreeNode(name.c_str()))
//    //{
//    //    Enemy::DrawDebugGUI();
//    //    std::string p = std::string("position") + std::to_string(category_id);
//    //    ImGui::SliderFloat3(p.c_str(), &position.x, -5, 5);
//    //    std::string s = std::string("scale") + std::to_string(category_id);
//    //    ImGui::SliderFloat3(s.c_str(), &scale.x, 0.01f, 4.0f);
//    //    std::string a = std::string("angle") + std::to_string(category_id);
//    //    ImGui::SliderFloat3(a.c_str(), &angle.x, -3.14f, 3.14f);
//    //    ImGui::TreePop();
//    //}
//}

void Enemy03::DrewDebugPrimitive()
{
    // ���N���X�̃f�o�b�O�v���~�e�B�u�`��
    Enemy::DrewDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    // �^�[�Q�b�g�ʒu���f�o�b�O���`��
    debugRenderer->DrawSphere(targetPosition, 1.3f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    // �U���͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, attack_range, 1.0f, DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f));
}
