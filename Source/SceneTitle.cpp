#include "SceneTitle.h"
#include <imgui.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Graphics/Graphics.h"
#include "Graphics/DebugRenderer.h"
#include "Graphics/GeometricPrimitive.h"
#include "Graphics/Shader.h"
#include "Graphics/framebuffer.h"
#include "Camera.h"
#include "SceneGame.h"
#include "SceneTutorial.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Input/Input.h"
#include "SceneResult.h"

void SceneTitle::Initialize()
{
    //カメラ初期設定
    Graphics& graphics = Graphics::Instance();
    Camera& camera = Camera::Instance();
    camera.SetLookAt(
        DirectX::XMFLOAT3(0, 3, 0),
        DirectX::XMFLOAT3(0, 0, 0),
        DirectX::XMFLOAT3(0, 1, 0)
    );
    camera.SetPerspectiveFov(
        DirectX::XMConvertToRadians(90),
        graphics.GetScreenWidth() / graphics.GetScreenHeight(),
        0.1f,
        100.0f
    );
    //カメラコントローラー初期化
    cameraController = std::unique_ptr<CameraController>(new CameraController());


    D3D11_BUFFER_DESC buffer_desc{};
    {
        HRESULT hr{ S_OK };
        buffer_desc.ByteWidth = sizeof(CBblur);
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;

        hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, CBblur_buffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        buffer_desc.ByteWidth = sizeof(luminance_extract_constants);
        hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr,
            luminance_extract_constant_buffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    // framebufferInitialize
    framebuffers[0] = std::make_unique<framebuffer>(graphics.GetDevice(), graphics.GetScreenWidth(), graphics.GetScreenHeight());
    framebuffers[1] = std::make_unique<framebuffer>(graphics.GetDevice(), graphics.GetScreenWidth() / 2, graphics.GetScreenHeight() / 2);

    // pixelShadersInitialize
    create_ps_from_cso(graphics.GetDevice(), "Data/Shader/luminance_extraction_ps.cso", pixel_shaders[0].GetAddressOf());
    create_ps_from_cso(graphics.GetDevice(), "Data/Shader/blur_ps.cso", pixel_shaders[1].GetAddressOf());

    bit_block_transfer = std::make_unique<fullscreen_quad>(graphics.GetDevice());

    //スプライト初期化
    sprite = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/Title.png"));
    test=std::unique_ptr<Sprite>(new Sprite("Data/Sprite/check.png"));
    model = std::unique_ptr<Model>(new Model("Data/Model/Cube/Cube_02.mdl"));
    ui = std::unique_ptr<Ui>(new Ui);
    srand((unsigned int)time(NULL));
}

void SceneTitle::Finalize()
{
}

void SceneTitle::Update(float elapsedTime)
{
    //cameraController->SetTarget({ 0,0,0 });
    //cameraController->Update(elapsedTime);

    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    GamePadButton anyButton =
          GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y
        ;
    switch (current_title)
    {
    case 0:
        position.y = (sinf(angle.y));
        angle.y += 0.01f;
        if (gamePad.GetButtonDown() & anyButton)
        {
            current_title++;
        }
        if (angle.y > DirectX::XM_PI)
        {
            angle.y -= DirectX::XM_2PI;
        }
        else if (angle.y < -DirectX::XM_PI)
        {
            angle.y += DirectX::XM_2PI;
        }
        break;
    case 1:

        if (gamePad.GetButtonDown() & anyButton)
        {
            current_title++;
        }

        DirectX::XMFLOAT3 ep = { 0.0f, 0.0f, -14.0f };
        DirectX::XMFLOAT3 er = {0.0f,DirectX::XM_2PI * 2.0f,0.0f};
        /*if (position.z > -12.0f)
        {
            angle.y += 1;
            position.z -= 0.5;
        }*/
        DirectX::XMVECTOR OP = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR EP = DirectX::XMLoadFloat3(&ep);
        DirectX::XMVECTOR OR = DirectX::XMLoadFloat3(&angle);
        DirectX::XMVECTOR ER = DirectX::XMLoadFloat3(&er);
        OP = DirectX::XMVectorLerp(OP,EP,0.01f);
        DirectX::XMStoreFloat3(&position, OP);
        OR = DirectX::XMVectorLerp(OR,ER,0.01f);
        DirectX::XMStoreFloat3(&angle, OR);
        if (position.z <= ep.z + 0.01f)
        {
            position = ep;
            angle = er;
            current_title++;
        }
        break;
    //case 2:
    case 2:
        if (gamePad.GetButtonDown() & anyButton)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
        }
    }

    DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(0.005f,0.005f,0.005f) };
    DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z) };
    DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(position.x, position.y, position.z) };
    DirectX::XMStoreFloat4x4(&transform, S * R * T);

    model->UpdateTransform(transform);

    Box_color.x = 0;
    Box_color.y = 1;
    Box_color.z = 0;
}

void SceneTitle::Render()
{

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.4f, 0.4f, 0.4f, 1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 描画処理
    RenderContext rc;
    rc.lightDirection = { light.x,light.y,light.z, 0.0f };	// ライト方向（下方向）

    //カメラパラメータ設定
    //ビュー・プロジェクション変換行列を計算し、それを定数バッファにセットする
    D3D11_VIEWPORT viewport;
    UINT num_viewports{ 1 };
    dc->RSGetViewports(&num_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    DirectX::XMMATRIX P{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30),aspect_ratio,0.1f,100.0f) };


    DirectX::XMVECTOR eye{ DirectX::XMVectorSet(0.0f,0.0f,-16.0f,0.0f) };
    DirectX::XMVECTOR focus{ DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f) };
    DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f) };
    DirectX::XMMATRIX V{ DirectX::XMMatrixLookAtLH(eye,focus,up) };
    DirectX::XMStoreFloat4x4(&rc.view, V);
    DirectX::XMStoreFloat4x4(&rc.projection, P);
   // Camera& camera = Camera::Instance();
    //rc.view = camera.GetView();
    //rc.projection = camera.GetProjection();

    // 定数バッファ更新
    {
        CBblur blur;
        blur.bloom_intensity = bloom_intensity;
        blur.gaussian_sigma = gaussian_sigma;
        blur.smoothHL = smoothHL;
        blur.color = color_bl;
        graphics.GetDeviceContext()->UpdateSubresource(CBblur_buffer.Get(), 0, 0, &blur, 0, 0);
        graphics.GetDeviceContext()->VSSetConstantBuffers(0, 1, CBblur_buffer.GetAddressOf());
        graphics.GetDeviceContext()->PSSetConstantBuffers(0, 1, CBblur_buffer.GetAddressOf());

        graphics.GetDeviceContext()->UpdateSubresource(luminance_extract_constant_buffer.Get(), 0, 0, &luminance_extract_constant, 0, 0);
        graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, luminance_extract_constant_buffer.GetAddressOf());
        graphics.GetDeviceContext()->PSSetConstantBuffers(1, 1, luminance_extract_constant_buffer.GetAddressOf());
    }

    // 3Dオブジェクト描画
    {
        framebuffers[1]->clear(graphics.GetDeviceContext());
        framebuffers[1]->activate(graphics.GetDeviceContext());
        bit_block_transfer->blit(graphics.GetDeviceContext(),
            framebuffers[0]->shader_resource_view[0].GetAddressOf(), 0, 1, pixel_shaders[0].Get());
        framebuffers[1]->deactivate(graphics.GetDeviceContext());

        ID3D11ShaderResourceView* srv[2] =
        { framebuffers[0]->shader_resource_view[0].Get(),framebuffers[1]->shader_resource_view[0].Get() };
        bit_block_transfer->blit(graphics.GetDeviceContext(),srv,0,2,pixel_shaders[1].Get());

        framebuffers[0]->clear(graphics.GetDeviceContext(),0.4f, 0.4f, 0.4f);
        framebuffers[0]->activate(graphics.GetDeviceContext());
        Shader* shader = graphics.GetShader();
        shader->Begin(dc, rc);

        shader->Draw(dc, model.get(), { Box_color});
        shader->End(dc);


        framebuffers[0]->deactivate(graphics.GetDeviceContext());
    }

    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        //タイトルスプライト描画
        /*sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1); */
        ui.get()->title(dc);

    }
    RenderDebugImgui();
}

void SceneTitle::RenderDebugImgui()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("box", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::TreeNode("Transform"))
        {
            ImGui::SliderFloat3("position", &position.x, -5, 5);
            ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);

            ImGui::TreePop();
        }
        if (ImGui::TreeNode("shader"))
        {
            ImGui::SliderFloat3("light", { &light.x }, -3.0f, 3.0f);
            ImGui::SliderFloat("gaussian_sigma", { &gaussian_sigma }, -3.0f, 3.0f);
            ImGui::SliderFloat("bloom_intensity", { &bloom_intensity }, -3.0f, 3.0f);
            ImGui::SliderFloat("threshold", &luminance_extract_constant.threshold, 0.0f, 1.0f);
            ImGui::SliderFloat("intensity", &luminance_extract_constant.intensity, 0.0f, 10.0f);
            ImGui::SliderFloat2("smoothHL", &smoothHL.x, 0.0f, 1.0f);
            ImGui::ColorPicker4("color_bl", &color_bl.x);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}
