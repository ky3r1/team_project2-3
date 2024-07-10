#include "ProjectileStraight.h"

ProjectileStraight::ProjectileStraight(ProjectileManager* manager, int category)
    : Projectile(manager)
{
    //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    model = new Model("Data/Model/Bullet/Bullet.mdl");

    color = { 0,1,0,1 };
    switch (category)
    {
    case PLAYERCATEGORY:
        color = { 1,1,1,1};
        break;
    case ENEMYCATEGORY:
        color = { 0,0,1,1 };
        break;
    }

    //表示サイズを調整
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
    ChangeColor(color, category);
    //寿命処理
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0)
    {
        //自分を削除
        Destroy();
    }
    //移動
    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;  //(位置＋＝向き＊速さ)
    position.y += direction.y * speed;
    position.z += direction.z * speed;

    //オブジェクト行列を更新
    UpdateTransform();
    if (lineEffect.get() != nullptr)
    {
        lineEffect.get()->Play(position, 0.5f);
    }
    //モデル行列更新
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
