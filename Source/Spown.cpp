#include "Spown.h"
#include <imgui.h>

#include "Easing.h"
#include "Mathf.h"

#include "Graphics/Sprite.h"

#include "EnemyManager.h"
#include "Enemy.h"
#include "Enemy01.h"
#include "Enemy02.h"
#include "Enemy03.h"
#include "EnemyBoss.h"

#define SPOWNDERAY 3*60

#define TEXTTIME 3.0f*60.0f
#define TEXTDELAY 1*60


Spown::Spown()
{
    spown_delay_timer = SPOWNDERAY;
    stage_clear = false;
    warning = std::unique_ptr<Effect>(new Effect("Data/Effect/Warning.efkefc"));
    text = std::unique_ptr<Sprite>(new Sprite("Data/Font/font4.png"));
}

void Spown::Update(float elapsedTime)
{
    //Wave01
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
    //Wave02
    if (flg[1][0]&&!flg[1][1])
    {
        //MakeWave(1, 4);
        Enemy* enemy = nullptr;
        for (int i = 0; i < 2; i++)
        {
            enemy = new Enemy02(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(10.0f, 20.0f), 5.0f, Mathf::RandomRange(-5.0f, 5.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        flg[1][1] = true;
    }
    //Wave03
    if (flg[2][0] && !flg[2][1])
    {
        //MakeWave(1, 4);
        Enemy* enemy = nullptr;
        for (int i = 0; i < 2; i++)
        {
            enemy = new Enemy03(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(-30.0f, -20.0f), 5.0f, Mathf::RandomRange(10.0f, 20.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        flg[2][1] = true;
    }
    //Wave04
    if (flg[3][0] && !flg[3][1])
    {
        //MakeWave(1, 4);
        Enemy* enemy = nullptr;
        for (int i = 0; i < 3; i++)
        {
            enemy = new Enemy01(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(-40.0f, 0.0f), 5.0f, Mathf::RandomRange(-20.0f, 10.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        for (int i = 0; i < 2; i++)
        {
            enemy = new Enemy02(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(-40.0f, 0.0f), 5.0f, Mathf::RandomRange(-20.0f, 10.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        flg[3][1] = true;
    }
    //Wave05
    if (flg[4][0] && !flg[4][1])
    {
        Enemy* enemy = nullptr;
        for (int i = 0; i < 4; i++)
        {
            enemy = new Enemy01(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(-40.0f, 40.0f), 5.0f, Mathf::RandomRange(-40.0f, 40.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        for (int i = 0; i < 2; i++)
        {
            enemy = new Enemy02(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(-40.0f, 40.0f), 5.0f, Mathf::RandomRange(-40.0f, 40.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        for (int i = 0; i < 1; i++)
        {
            enemy = new Enemy03(ENEMYCATEGORY);
            enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(-40.0f, 40.0f), 5.0f, Mathf::RandomRange(-40.0f, 40.0f)));
            EnemyManager::Instance().Register(enemy);
        }
        flg[4][1] = true;
    }
    timer++;
}

void Spown::render(ID3D11DeviceContext* dc)
{
    if (EnemyManager::Instance().GetEnemyCount() == 0)
    {
        if (flg[4][0])stage_clear = true;
        if (spown_delay_timer < 0)
        {
            if (!flg[4][0] && flg[3][0])
            {
                TextEasing(dc, "Wave 5/5", { 0.0f, 5.0f, 0.0f }, { -500.0f,200.0f }, { 200.0f,-500.0f }, { 0,1,0.49603,1 }, flg[4][0]);
            }
            if (!flg[3][0] && flg[2][0])
            {
                TextEasing(dc, "Wave 4/5", { -20.0f, 5.0f, -5.0f }, { -500.0f,200.0f }, { 200.0f,-500.0f }, { 0,1,0.49603,1 }, flg[3][0]);
            }
            if (!flg[2][0] && flg[1][0])
            {
                TextEasing(dc, "Wave 3/5", { -25.0f, 5.0f, 15.0f }, { -500.0f,200.0f }, { 200.0f,-500.0f }, { 0,1,0.49603,1 }, flg[2][0]);
            }
            if (!flg[1][0] && flg[0][0])
            {
                TextEasing(dc, "Wave 2/5", { 15.0f, 5.0f, 0.0f }, { -500.0f,200.0f }, { 200.0f,-500.0f }, { 0,1,0.49603,1 }, flg[1][0]);
            }
            if (!flg[0][0])
            {
                TextEasing(dc, "Wave 1/5", { 0.0f, 5.0f, 7.5f }, { -500.0f,200.0f }, { 200.0f,-500.0f }, { 0,1,0.49603,1 }, flg[0][0]);
            }
        }
    }
    else
    {
        spown_delay_timer = SPOWNDERAY;
    }
    spown_delay_timer--;
}

void Spown::TextEasing(ID3D11DeviceContext* dc,std::string text_c, DirectX::XMFLOAT3 effect_pos, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 easing_pos, DirectX::XMFLOAT4 color, bool& spown_flg)
{
    switch (delay_count)
    {
    case 0:
        warning.get()->Play(effect_pos);
        text_flg = false;
        text_timer = TEXTTIME;
        text_pos = pos;
        delay_count++;
        break;
    case 1:
        text_pos = { (Easing::OutCirc(text_timer, 3.0f * 60.0f, easing_pos.x,easing_pos.y)),pos.y };
        if (text_pos.x >= easing_pos.x || text_flg)
        {
            if (text_pos.x <= easing_pos.y)delay_count++;
            text_flg = true;
        }
        text.get()->textout(dc, text_c, text_pos.x, text_pos.y, 50, 50, color);
        text_timer--;
        break;
    case 2:
        delay_count = 0;
        spown_flg = true;
        break;
    }
}

void Spown::DrawDebugGUI()
{
    //ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    //ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Spown", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::TreeNode("Text"))
        {
            ImGui::SliderFloat2("position", &text_pos.x, -5, 5);
            //ImGui::SliderFloat3("scale", &scale.x, 0.01f, 4.0f);
            //ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);
            //ImGui::SliderInt("health", &health, 0.0f, 10.0f);
            //ImGui::SliderFloat("movespeed", &moveSpeed, 0.0f, 20.0f);

            //ImGui::SliderInt("delay_auto_time", &projectile_auto.time, 0.0f, DELAYAUTOTIME);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}


void Spown::MakeWave(int category, int count, Enemy* enemy, DirectX::XMFLOAT2 top, DirectX::XMFLOAT2 middle)
{
    for (int i = 0; i < count; i++)
    {
        enemy->SetPosition(DirectX::XMFLOAT3(Mathf::RandomRange(top.x, top.y), 5.0f, Mathf::RandomRange(middle.x, middle.y)));
        EnemyManager::Instance().Register(enemy);
    }
}

