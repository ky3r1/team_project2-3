#include "Ui.h"

void Ui::title(ID3D11DeviceContext* dc)
{    
    static int check = 0;
    switch (check)
    {
    case 0:
        //�X�v���C�g������
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
        //�X�v���C�g������
        sprite01 = std::unique_ptr<Sprite>(new Sprite("Data/UI/penetration_UI.png"));
        sprite02 = std::unique_ptr<Sprite>(new Sprite("Data/UI/ricochet_UI.png"));
        check = 1;
        break;
    case 1:
        
        sprite01->Render(dc,DirectX::XMFLOAT2(0,500), DirectX::XMFLOAT2(150, 150), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(200, 200), 0, DirectX::XMFLOAT4(1, 1, 1, 1));
        break;
    }
}

void Ui::result(ID3D11DeviceContext* dc)
{
    static int check = 0;
    switch (check)
    {
    case 0:
        //�X�v���C�g������
        check = 1;
        break;
    case 1:

        break;
    }
}
