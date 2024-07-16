#include "Ui.h"
#include "Player.h"
void Ui::title(ID3D11DeviceContext* dc)
{    
    static int check = 0;
    switch (check)
    {
    case 0:
        //スプライト初期化
        check = 1;
        break;
    case 1:

        break;
    }
}

void Ui::game(ID3D11DeviceContext* dc)
{
    static int check = 0;
    switch (check)
    {
    case 0:
        //スプライト初期化
        sprite01 = std::unique_ptr<Sprite>(new Sprite("Data/UI/penetration_UI.png"));
        sprite02 = std::unique_ptr<Sprite>(new Sprite("Data/UI/ricochet_UI.png"));
        sprite03 = std::unique_ptr<Sprite>(new Sprite("Data/Font/font4.png"));
        sprite04 = std::unique_ptr<Sprite>(new Sprite("Data/UI/HP/iconHP.png"));
        sprite05 = std::unique_ptr<Sprite>(new Sprite("Data/UI/HP/HPbar.png"));
        sprite06 = std::unique_ptr<Sprite>(new Sprite("Data/UI/HP/gauge.png"));
        sprite07 = std::unique_ptr<Sprite>(new Sprite("Data/UI/HP/HP_background.png"));
        check = 1;
        HPbar = { 140,30 };
        break;
    case 1:
        if(Player::Instance().GetProjectileType() == PENETRATION)
        {
            sprite01->Render(dc, DirectX::XMFLOAT2(1100, 500), DirectX::XMFLOAT2(200, 230), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(200, 230), 0, DirectX::XMFLOAT4(1, 1, 1, 1));
        }
        if (Player::Instance().GetProjectileType() == RICOCHET)
        {
            sprite02->Render(dc, DirectX::XMFLOAT2(1100, 500), DirectX::XMFLOAT2(200, 230), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(200, 230), 0, DirectX::XMFLOAT4(1, 1, 1, 1));
        }
        sprite07->Render(dc, DirectX::XMFLOAT2(0, 580), DirectX::XMFLOAT2(300, 140), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(270, 120), 0, DirectX::XMFLOAT4(1, 1, 1, 0.5f));
        sprite04->Render(dc, DirectX::XMFLOAT2(10, 600), DirectX::XMFLOAT2(131, 104), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(131, 104), 0, DirectX::XMFLOAT4(1, 1, 1, 1));
        sprite05->Render(dc, DirectX::XMFLOAT2(145, 660), DirectX::XMFLOAT2(150, 40), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(132, 29), 0, DirectX::XMFLOAT4(1, 1, 1, 1));
        sprite06->Render(dc, DirectX::XMFLOAT2(150, 664), HPbar, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(127, 23), 0, HPcolor);
        if (Player::Instance().GetHealth() > 5)
        {
            HPcolor = { 0.164f, 0.886f, 0.611f, 1 };
        }
        
        else if (Player::Instance().GetHealth() > 2)
        {
            HPcolor = { 0.811f,0.65f,0.09f, 1 };
        }
        else if (Player::Instance().GetHealth() > 0)
        {
            HPcolor = { 1,0.482f,0.533f, 1 };
        }
        //sprite03->textout(dc, "abcdefghijklmnopqrstuvwxyz", 100, 200, 50, 50, { 1,1,1,1 });
        break;
    }
}

void Ui::result(ID3D11DeviceContext* dc)
{
    static int check = 0;
    switch (check)
    {
    case 0:
        //スプライト初期化
        check = 1;
        break;
    case 1:

        break;
    }
}
