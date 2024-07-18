#include "SceneResult.h"

#include "Graphics/Graphics.h"
#include "Input/Input.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Enemy01.h"
#include "Enemy02.h"
#include "Enemy03.h"

#include "Ui.h"

#include <string>
#include <wchar.h>

//#include "StageMapChip.h"

float enemy_score[5];
float total_score;
int result_timer = 0;
extern int enemy01_count;
extern int enemy02_count;
extern int enemy03_count;
bool draw_score = false;

wchar_t score[10][3];

void SceneResult::Initialize()
{
    //スプライト初期化
    Graphics& graphics = Graphics::Instance();
    sprite = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/result/result.png"));
    audio = Audio::Instance().LoadAudioSource("Data/Audio/result.wav");
    font = std::make_unique<Font>(graphics.GetDevice(), ".\\Data\\Font\\MS_Gothic.fnt", 1024);
    result_timer = 0;
    draw_score = false;
    //StageMapChip::Instance().SetStageNum(0);
}

void SceneResult::Finalize()
{
    audio->Stop();
}

void SceneResult::Update(float elapsedTime)
{
    audio->Play(true);

    GamePad& gamePad = Input::Instance().GetGamePad();

    GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y
        ;

    Mouse& mouse = Input::Instance().GetMouse();

    result_timer++;
    if(result_timer > 116)
    {
        result_timer = 116;
        draw_score = true;
    }
    if (draw_score&& gamePad.GetButtonDown() & anyButton || mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
    }

    _itow_s(enemy01_count, score[0], 10);
    _itow_s(enemy02_count, score[1], 10);
    _itow_s(enemy03_count, score[2], 10);
;
}

void SceneResult::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.0f,0.5f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);


    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        /*if (outcome)color = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
        else color = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);*/
        //タイトルスプライト描画
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            result_timer / 4 % 4 * 1920, result_timer/ 4 / 4 * 1080, 1920, 1080,
            0,
            color.x, color.y, color.z, color.w);

        Ui::Instance().result(dc);
    }
    //textRender
    if(draw_score)
    {
        font->Begin(dc);
        font->Draw(1050, 170, score[0],3);
        font->Draw(1050, 330, score[1],3);
        font->Draw(1050, 510, score[2],3);
        font->End(dc);
    }
}
