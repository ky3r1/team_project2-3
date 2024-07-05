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

void Spown::Update(float elapsedTime)
{
    static int timer = 0;
    if (timer > SPOWN00 && !flg[0])
    {
        //MakeWave(1, 4);
        Enemy* enemy = nullptr;
        for (int i = 0; i < 4; i++)
        {
            //enemy = SpownEnemyCategory(1);
            enemy = new Enemy02(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(-5.0f, 5.0f), 5.0f, Mathf::RandomRange(-5.0f, 5.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        flg[0] = true;
    }
    if (timer > SPOWN01 && !flg[1])
    {
        //MakeWave(1, 4);
        Enemy* enemy = nullptr;
        for (int i = 0; i < 4; i++)
        {
            //enemy = SpownEnemyCategory(2);
            enemy = new Enemy01(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(10.0f, 20.0f), 5.0f, Mathf::RandomRange(-5.0f, 5.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        flg[1] = true;
    }
    timer++;
}

//ìGèoÇ∑(category==-1Ç»ÇÁÉâÉìÉ_ÉÄ)
Enemy* Spown::SpownEnemyCategory(int category)
{   
    Enemy* enemy = nullptr;
    Enemy01* robot = new Enemy01(ENEMYCATEGORY);
    Enemy02* slime = new Enemy02(ENEMYCATEGORY);
    Enemy03* jammo = new Enemy03(ENEMYCATEGORY);
    EnemyBoss* boss=new EnemyBoss(ENEMYCATEGORY);
    int i = category;
    if (i == -1)
    {
        i = rand() % 3;
    }
    switch (i)
    {
    case Enemy01_Num:
        enemy = robot;
        if (slime != nullptr)
        {
            delete slime;
            slime = nullptr;
        }
        if (jammo != nullptr)
        {
            delete jammo;
            jammo = nullptr;
        }
        if (boss != nullptr)
        {
            delete boss;
            boss = nullptr;
        }
        break;
    case Enemy02_Num:
        enemy = slime;
        if (robot != nullptr)
        {
            delete robot;
            robot = nullptr;
        }
        if (jammo != nullptr)
        {
            delete jammo;
            jammo = nullptr;
        }
        if (boss != nullptr)
        {
            delete boss;
            boss = nullptr;
        }
        break;
    case Enemy03_Num:
        enemy = jammo;
        if (robot != nullptr)
        {
            delete robot;
            robot = nullptr;
        }
        if (slime != nullptr)
        {
            delete slime;
            slime = nullptr;
        }
        if (boss != nullptr)
        {
            delete boss;
            boss = nullptr;
        }
        break;
    case EnemyBoss_Num:
        enemy = boss;
        if (robot != nullptr)
        {
            delete robot;
            robot = nullptr;
        }
        if (slime != nullptr)
        {
            delete slime;
            slime = nullptr;
        }
        if (jammo != nullptr)
        {
            delete jammo;
            jammo = nullptr;
        }
        break;
    }
    return enemy;
}

void Spown::MakeWave(int category, int count)
{
    Enemy* enemy = nullptr;
    for (int i = 0; i < count; i++)
    {
        enemy = SpownEnemyCategory(category);
        enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(-5.0f, 5.0f), 5.0f, Mathf::RandomRange(-5.0f, 5.0f)));
        EnemyManager::Instance().Register(enemy);
    }
}

