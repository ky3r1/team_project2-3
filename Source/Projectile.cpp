#include "Projectile.h"
#include "ProjectileManager.h"


Projectile::Projectile(ProjectileManager* manager) : manager(manager)
{
    manager->Register(this);
}

void Projectile::Destroy()
{
    manager->Remove(this);
}

//デバッグプリミティブ描画
void Projectile::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    switch (category)
    {
    case PLAYERCATEGORY:
        debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(1, 0, 0, 1));
        break;
    case ENEMYCATEGORY:
        debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 1, 0, 1));
        break;
    default:
        break;
    }
}

void Projectile::ChangeColor(DirectX::XMFLOAT4& color, int category, int projectile_category)
{
    switch (category)
    {
    case PLAYERCATEGORY:
        //color = { 1,0,0,1 };
        if(projectile_category == PENETRATION)
            color = { 0,1,0,1 };
        else if(projectile_category == RICOCHET)
            color = { 0,0,1,1 };
        break;
    case ENEMYCATEGORY:
        color = { 1,0,0,1 };
        break;
    }
}

//行列更新処理
void Projectile::UpdateTransform()
{
    DirectX::XMVECTOR Front, Up, Right;

    //前ベクトルを算出
    Front = DirectX::XMLoadFloat3(&direction);
    Front = DirectX::XMVector3Normalize(Front);

    //仮の上ベクトルを算出
    Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
    Up = DirectX::XMVector3Normalize(Up);

    //右ベクトルを算出
    Right = DirectX::XMVector3Cross(Up,Front);
    Right = DirectX::XMVector3Normalize(Right);

    //上ベクトルを算出
    Up = DirectX::XMVector3Cross(Front, Right);

    //計算結果を取り出し
    DirectX::XMFLOAT3 right, up, front;
    DirectX::XMStoreFloat3(&right, Right);
    DirectX::XMStoreFloat3(&up, Up);
    DirectX::XMStoreFloat3(&front, Front);

    transform._11 = right.x * scale.x;
    transform._12 = right.y * scale.x;
    transform._13 = right.z * scale.x;
    transform._14 = 0.0f;
    transform._21 = up.x * scale.y;
    transform._22 = up.y * scale.y;
    transform._23 = up.z * scale.y;
    transform._24 = 0.0f;
    transform._31 = front.x * scale.z;
    transform._32 = front.y * scale.z;
    transform._33 = front.z * scale.z;
    transform._34 = 0.0f;
    transform._41 = position.x;
    transform._42 = position.y;
    transform._43 = position.z;
    transform._44 = 1.0f;

    //発射方向
    this->direction = front;
}