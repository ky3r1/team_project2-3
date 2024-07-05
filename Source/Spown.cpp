#include "Spown.h"

#include "Mathf.h"

#include "EnemyManager.h"

#include "Enemy.h"
#include "Enemy01.h"
#include "Enemy02.h"
#include "Enemy03.h"
#include "EnemyBoss.h"

#define SPOWN01 5*60

void Spown::Update(float elapsedTime)
{
    static int timer = 0;
    Enemy* enemy = nullptr;
    if (timer > SPOWN01&&flg[0] == false)
    {
        for(int i=0;i<4;i++)
        {
            enemy = SpownEnemyCategory();
            //enemy = new Enemy01(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(-5.0f, 5.0f), 5.0f, Mathf::RandomRange(-5.0f, 5.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        flg[0] = true;
    }
    timer++;
}

//Ží—Þƒ‰ƒ“ƒ_ƒ€
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

Enemy* Spown::SpownEnemyCategory()
{
    return SpownEnemyCategory(-1);
}

void Spown::MakeWave(Enemy* enemy, int count, int timer, int flg, int spown_timer)
{

}

