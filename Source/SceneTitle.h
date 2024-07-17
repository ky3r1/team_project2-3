#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "CameraController.h"
#include "Graphics/Model.h"
#include "Graphics/fullscreen_quad.h"
#include "Graphics/framebuffer.h"
#include "Ui.h"
#include <Windows.h>
#include "Audio/Audio.h"

//タイトルシーン
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle() override {}

    //初期化
    void Initialize() override;

    //終了化
    void Finalize() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render() override;


    void RenderDebugImgui();
private:
    struct CBblur
    {
        float gaussian_sigma = 1.0f;
        float bloom_intensity = 1.0f;
        DirectX::XMFLOAT2 smoothHL = { 0.6f,0.8f };
        DirectX::XMFLOAT4 color = { 0.299, 0.587, 0.114 ,0 };
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> CBblur_buffer;
    float gaussian_sigma = 1.0f;
    float bloom_intensity = 1.0f;
    DirectX::XMFLOAT2 smoothHL = { 0.6f,0.8f };
    DirectX::XMFLOAT4 color_bl = { 0.299, 0.587, 0.114 ,0 };

    struct luminance_extract_constants
    {
        float threshold = { 0.5f }; // 高輝度抽出のための閾値
        float intensity = { 1.0f }; // ブルームの強度
        DirectX::XMFLOAT2 dummy;
    };
    luminance_extract_constants luminance_extract_constant;
    Microsoft::WRL::ComPtr<ID3D11Buffer> luminance_extract_constant_buffer;

    DirectX::XMFLOAT3 light = { 0.0f,0.0f,1.0f };

    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[2];

    std::unique_ptr<Sprite> sprite = nullptr;
    std::unique_ptr<Sprite> test = nullptr;
    std::unique_ptr<CameraController> cameraController = nullptr;
    std::unique_ptr<Model> model = nullptr;
    std::unique_ptr<Ui> ui = nullptr;
    std::unique_ptr<fullscreen_quad> bit_block_transfer = nullptr;
    std::unique_ptr<framebuffer> framebuffers[2];
    std::unique_ptr<AudioSource> audio;

    int current_title = 0;
    DirectX::XMFLOAT4 Box_color = { 0,0,0,1 };

    DirectX::XMFLOAT3 angle = { 0,0,0 };
    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT4X4 transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};