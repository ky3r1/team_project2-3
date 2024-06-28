#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

// �X�V����
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    //�j������
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
    //�G���m�̏Փˏ���
    CollisionEnemyVsEnemies();
}

//�`�揈��
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}

//�G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
    //�G�l�~�[��ID��t�^
    enemy->SetId(static_cast<int>(enemies.size()) + 1);
    enemies.emplace_back(enemy);
}

//�@�G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
    //�j�����X�g�ɒǉ�
    removes.insert(enemy);
}

//�G�l�~�[�S�폜
void EnemyManager::clear()
{
    for (Enemy* enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
}

//�f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrewDebugPrimitive();
    }
}

//�f�o�b�OGUI�\��
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
    //���ׂĂ̓G�Ƒ�������ŏՓ˔���
    int enemyCount = GetEnemyCount();
    for (int i=0;i<enemyCount;i++)
    {
        Enemy* enemy1 = GetEnemy(i);
        for (int j = i+1; j < enemyCount; j++)
        {
            Enemy* enemy2 = GetEnemy(j);

            //�Փˏ���
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

// ID����G�l�~�[���擾����
Enemy* EnemyManager::GetEnemyFromId(int id)
{
    for (Enemy* enemy : enemies)
    {
        if (enemy->GetId() == id)
            return enemy;
    }
    return nullptr;
}