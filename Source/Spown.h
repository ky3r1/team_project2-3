#pragma once
#include "All.h"
#include "Enemy.h"
#include "Effect.h"
#include "Graphics/Sprite.h"

class Spown
{
public:

    Spown();
    ~Spown() {};

    void Update(float elapsedTime);

    void render(ID3D11DeviceContext* dc);

    void TextEasing(ID3D11DeviceContext* dc, std::string text_c, DirectX::XMFLOAT3 effect_pos, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 easing_pos, DirectX::XMFLOAT4 color, bool& spown_flg);

    //デバッグ用GUI
    void DrawDebugGUI();

    bool GetStageClear() { return stage_clear; }
private:

    void MakeWave(int category, int count, Enemy* enemy, DirectX::XMFLOAT2 top, DirectX::XMFLOAT2 middle);
    int timer = 0;
    int spown_delay_timer = 0;
    int delay_count = 0;
    float text_timer = 0;
    bool text_flg = false;
    DirectX::XMFLOAT2 text_pos = { 0,0 };
    bool flg[5][2] = {};
    std::unique_ptr<Effect> warning = nullptr;
    std::unique_ptr<Sprite> text = nullptr;
    bool stage_clear = false;
};