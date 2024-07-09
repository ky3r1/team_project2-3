#include "Spown.h"

#include "Mathf.h"

#include "EnemyManager.h"

#include "Enemy.h"
#include "Enemy01.h"
#include "Enemy02.h"
#include "Enemy03.h"
#include "EnemyBoss.h"

#define SPOWN00 5*60
#define SPOWN01 20*60
#define SPOWNDERAY 3*60

void Spown::Update(float elapsedTime)
{
    if (flg[0][0]&&!flg[0][1])
    {
        //MakeWave(1, 4);
        Enemy* enemy = nullptr;
        for (int i = 0; i < 4; i++)
        {
            enemy = new Enemy01(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(-5.0f, 5.0f), 5.0f, Mathf::RandomRange(5.0f, 10.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        flg[0][1] = true;
    }
    if (flg[1][0]&&!flg[1][1])
    {
        //MakeWave(1, 4);
        Enemy* enemy = nullptr;
        for (int i = 0; i < 4; i++)
        {
            enemy = new Enemy01(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(10.0f, 20.0f), 5.0f, Mathf::RandomRange(-5.0f, 5.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        flg[1][1] = true;
    }

    timer++;
}

void Spown::render(ID3D11DeviceContext* dc)
{
    if (EnemyManager::Instance().GetEnemyCount() == 0)
    {
        if (spown_delay_timer < 0)
        {
            if (timer > SPOWN00 && !flg[0][0])
            {
                flg[0][0] = true;
                warning.get()->Play(DirectX::XMFLOAT3(0.0f, 5.0f, 7.5f), 1.0f);
            }
            if (timer > SPOWN01 && !flg[1][0])
            {
                flg[1][0] = true;
            }
        }
    }
    else
    {
        spown_delay_timer = SPOWNDERAY;
    }
    spown_delay_timer--;
}


void Spown::MakeWave(int category, int count, Enemy* enemy, DirectX::XMFLOAT2 top, DirectX::XMFLOAT2 middle)
{
    for (int i = 0; i < count; i++)
    {
        enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(top.x, top.y), 5.0f, Mathf::RandomRange(middle.x, middle.y)));
        EnemyManager::Instance().Register(enemy);
    }
}

