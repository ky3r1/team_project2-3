#pragma once
#include "All.h"
#include "Enemy.h"
#include "Effect.h"

class Spown
{
public:
    //C:category 敵の種類
    //P:position 座標
    
    //唯一のインスタンス取得
    static Spown& Instance()
    {
        static Spown instance;
        return instance;
    }

    void Update(float elapsedTime);

    void render(ID3D11DeviceContext* dc);
private:

    void MakeWave(int category, int count, Enemy* enemy,DirectX::XMFLOAT2 top, DirectX::XMFLOAT2 middle);
    int timer = 0;
    int spown_delay_timer = 0;
    bool flg[10][2] = {};
    std::unique_ptr<Effect> warning = std::unique_ptr<Effect>(new Effect("Data/Effect/Warning.efkefc"));
};