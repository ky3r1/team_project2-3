#include "ProjectileStraight.h"

ProjectileStraight::ProjectileStraight(ProjectileManager* manager, int category, const DirectX::XMFLOAT3& direction,const DirectX::XMFLOAT3& position, int pcolor)
    : Projectile(manager)
{
    //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    model = new Model("Data/Model/Bullet/Bullet.mdl");

    switch (category)
    {
    case PLAYERCATEGORY:
        this->color = { 1,1,1,1};
        break;
    case ENEMYCATEGORY:
        this->color = { 0,0,1,1 };
        break;
    }
    color.w = 0;
    projectile_category = pcolor;
    this->direction = direction;
    this->position = position;
    //�\���T�C�Y�𒲐�
    scale.x = scale.y = scale.z = 0.25f;
    radius = 0.5f;
    this->category = category;
}

ProjectileStraight::~ProjectileStraight()
{
    delete model;
}

void ProjectileStraight::Update(float elapsedTime)
{
    ChangeColor(color, category, projectile_category);
    //��������
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0)
    {
        //�������폜
        Destroy();
    }
    //�ړ�
    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;  //(�ʒu�{������������)
    position.y += direction.y * speed;
    position.z += direction.z * speed;

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();
    if (lineEffect.get() != nullptr)
    {
        lineEffect.get()->Play(position, 0.5f);
    }
    //���f���s��X�V
    model->UpdateTransform(transform);
}

void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model,color);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction,
                                const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}

void ProjectileStraight::DrawDebugGUI()
{
    if (ImGui::TreeNode("ProjectileStraight"))
    {
        ImGui::SliderFloat3("position", &position.x, -5, 5);
        ImGui::SliderFloat3("scale", &scale.x, 0.001f, 4.0f);
        ImGui::SliderFloat3("direction", &direction.x, -3.14f, 3.14f);
        ImGui::TreePop();
    }
}
