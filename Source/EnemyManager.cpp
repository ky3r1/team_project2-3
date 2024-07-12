#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

// 更新処理
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    //破棄処理
    for (Enemy* enemy : removes)
    {
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        delete enemy;
    }
    removes.clear();
    //敵同士の衝突処理
    CollisionEnemyVsEnemies();
}

//描画処理
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    //エネミーにIDを付与
    enemy->SetId(static_cast<int>(enemies.size()) + 1);
    enemies.emplace_back(enemy);
}

//　エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
    //破棄リストに追加
    removes.insert(enemy);
}

//エネミー全削除
void EnemyManager::clear()
{
    for (Enemy* enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
}

//デバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrewDebugPrimitive();
    }
}

//デバッグGUI表示
void EnemyManager::DrawDebugGUI()
{
    ImGui::Begin("Enemy");
    ImGui::SetNextTreeNodeOpen(true);
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugGUI();
    }
    ImGui::End();
}

DirectX::XMFLOAT3 EnemyManager::GetPosition()
{
    DirectX::XMFLOAT3 reposition = { 0.0f,0.0f,0.0f };
    for (Enemy* enemy : enemies)
    {
        reposition=enemy->GetPosition();
    }
    return reposition;
}

void EnemyManager::CollisionEnemyVsEnemies()
{
    //すべての敵と総当たりで衝突判定
    int enemyCount = GetEnemyCount();
    for (int i=0;i<enemyCount;i++)
    {
        Enemy* enemy1 = GetEnemy(i);
        for (int j = i+1; j < enemyCount; j++)
        {
            Enemy* enemy2 = GetEnemy(j);

            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectCylinderVsSphere(
                enemy1->GetPosition(), enemy1->GetRadius(),enemy1->GetHeight(),enemy1->GetWeight(),
                enemy2->GetPosition(), enemy2->GetRadius(),enemy2->GetHeight(),enemy2->GetWeight(),
                outPosition))
            {
                //if(enemy1->GetWeight()<enemy2->GetWeight())enemy1->SetPosition(outPosition);
                //else enemy2->SetPosition(outPosition);
                enemy2->SetPosition(outPosition);
            }
        }
    }
}

// IDからエネミーを取得する
Enemy* EnemyManager::GetEnemyFromId(int id)
{
    for (Enemy* enemy : enemies)
    {
        if (enemy->GetId() == id)
            return enemy;
    }
    return nullptr;
}

Enemy* EnemyManager::NearEnemy(DirectX::XMFLOAT3 position)
{
    //最も近い敵を総当たりで探索
    EnemyManager& enemyManager = EnemyManager::Instance();
    float current_nearest_distance = FLT_MAX;
    Enemy* c = nullptr;
    int enemyCount = enemyManager.GetEnemyCount();
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&position);
    for (int index = 0; index < enemyCount; index++)
    {
        Enemy* enemy = EnemyManager::Instance().GetEnemy(index);
        if (position.x == enemy->GetPosition().x && position.y == enemy->GetPosition().y && position.z == enemy->GetPosition().z)continue;
        DirectX::XMVECTOR Epos = DirectX::XMLoadFloat3(&enemy->GetPosition());
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Epos, Pos);
        DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(Vec);
        float d;
        DirectX::XMStoreFloat(&d, D);
        if (d < current_nearest_distance)
        {
            current_nearest_distance = d;
            c = enemy;
        }
    }
    return c;
}

bool EnemyManager::IsAllEnemyAlrealyHit()
{
    for (Enemy* enemy : enemies)
    {
        if (!enemy->isHit)return false;
    }
    return true;
}
