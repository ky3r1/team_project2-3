#include <memory>
#include <sstream>

#include "EffectManager.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Framework.h"
#include "SceneTitle.h"
#include "SceneManager.h"

//static SceneGame sceneGame;

// 垂直同期間隔設定
static const int syncInterval = 1;

// コンストラクタ
Framework::Framework(HWND hWnd)
	: hWnd(hWnd)
	, input(hWnd)
	, graphics(hWnd)
{
	//エフェクトマネージャー初期化
	EffectManager::Instance().Initialize(); graphics.GetDeviceContext();
	EffectManager::Instance().Initialize(); graphics.GetDeviceContext();
	EffectManager::Instance().Initialize(); graphics.GetDeviceContext();

	//シーン初期化
	//sceneGame.Initialize();
	SceneManager::Instance().ChangeScene(new SceneTitle);

	//graphics.GetSwapChain()->QueryInterface(_uuidof(IDXGISwapChain1), (void**)&swap_chain);
	//graphics.SetFullscreenMode(fullscreen_mode);
}

// デストラクタ
Framework::~Framework()
{
	//sceneGame.Finalize();
	SceneManager::Instance().Clear();

	EffectManager::Instance().Finalize();
}

// 更新処理
void Framework::Update(float elapsedTime/*Elapsed seconds from last frame*/)
{
	//if (fullscreen_mode) stylize_window(fullscreen_mode);

	// 入力更新処理
	input.Update();

	// シーン更新処理
	//sceneGame.Update(elapsedTime);
	SceneManager::Instance().Update(elapsedTime);
}

// 描画処理
void Framework::Render(float elapsedTime/*Elapsed seconds from last frame*/)
{
	//別スレッド中にデバイスコンテキストが使われていた場合に
	//同時アクセスしないように排他制御する
	std::lock_guard<std::mutex> lock(graphics.GetMutex());

	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	// IMGUIフレーム開始処理
	graphics.GetImGuiRenderer()->NewFrame();

	// シーン描画処理
	//sceneGame.Render();
	SceneManager::Instance().Render();

	// IMGUIデモウインドウ描画（IMGUI機能テスト用）
	//ImGui::ShowDemoWindow();

	// IMGUI描画
	graphics.GetImGuiRenderer()->Render(dc);

	// バックバッファに描画した画を画面に表示する。
	graphics.GetSwapChain()->Present(syncInterval, 0);
}

// フレームレート計算
void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the
	// average time it takes to render one frame.  These stats
	// are appended to the window caption bar.
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hWnd, outs.str().c_str());

		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
}

void Framework::stylize_window(BOOL fullscreen)
{
	fullscreen_mode = fullscreen;
	if (fullscreen)
	{
		GetWindowRect(hWnd, &windowed_rect);
		SetWindowLongPtrA(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

		RECT fullscreen_window_rect;

		HRESULT hr{ E_FAIL };
		if (swap_chain)
		{
			Microsoft::WRL::ComPtr<IDXGIOutput> dxgi_output;
			hr = swap_chain->GetContainingOutput(&dxgi_output);
			if (hr == S_OK)
			{
				DXGI_OUTPUT_DESC output_desc;
				hr = dxgi_output->GetDesc(&output_desc);
				if (hr == S_OK)
				{
					fullscreen_window_rect = output_desc.DesktopCoordinates;
				}
			}
		}
		if (hr != S_OK)
		{
			DEVMODE devmode = {};
			devmode.dmSize = sizeof(DEVMODE);
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);

			fullscreen_window_rect = {
				devmode.dmPosition.x,
				devmode.dmPosition.y,
				devmode.dmPosition.x + static_cast<LONG>(devmode.dmPelsWidth),
				devmode.dmPosition.y + static_cast<LONG>(devmode.dmPelsHeight)
			};
		}
		SetWindowPos(
			hWnd,
			NULL,
			fullscreen_window_rect.left,
			fullscreen_window_rect.top,
			fullscreen_window_rect.right,
			fullscreen_window_rect.bottom,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(hWnd, SW_MAXIMIZE);
	}
	else
	{
		SetWindowLongPtrA(hWnd, GWL_STYLE, windowed_style);
		SetWindowPos(
			hWnd,
			HWND_NOTOPMOST,
			windowed_rect.left,
			windowed_rect.top,
			windowed_rect.right - windowed_rect.left,
			windowed_rect.bottom - windowed_rect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(hWnd, SW_NORMAL);
	}
}

void Framework::on_size_changed(UINT64 width, UINT height)
{
	HRESULT hr{ S_OK };
	if (width != framebuffer_dimensions.cx || height != framebuffer_dimensions.cy)
	{
		framebuffer_dimensions.cx = static_cast<LONG>(width);
		framebuffer_dimensions.cy = height;

		// Release all objects that hold shader resource views here.
#ifdef ENABLE_DIRECT2D
		d2d1_device_context.Reset();
#endif

		Microsoft::WRL::ComPtr<IDXGIFactory6> dxgi_factory6;
		hr = swap_chain->GetParent(IID_PPV_ARGS(dxgi_factory6.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		create_swap_chain(dxgi_factory6.Get());

		// Recreate all objects that hold shader resource views here.
#ifdef ENABLE_DIRECT2D
		create_direct2d_objects();
#endif
	}
}

void Framework::create_swap_chain(IDXGIFactory6* dxgi_factory6)
{
	HRESULT hr{ S_OK };

	if (swap_chain)
	{
		ID3D11RenderTargetView* null_render_target_view{};
		graphics.GetDeviceContext()->OMSetRenderTargets(1, &null_render_target_view, NULL);
		render_target_view.Reset();
#if 0
		immediate_context->Flush();
		immediate_context->ClearState();
#endif
		DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
		swap_chain->GetDesc(&swap_chain_desc);
		hr =swap_chain->ResizeBuffers(swap_chain_desc.BufferCount, framebuffer_dimensions.cx, framebuffer_dimensions.cy, swap_chain_desc.BufferDesc.Format, swap_chain_desc.Flags);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target_buffer;
		hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(render_target_buffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		D3D11_TEXTURE2D_DESC texture2d_desc;
		render_target_buffer->GetDesc(&texture2d_desc);


		hr = graphics.GetDevice()->CreateRenderTargetView(render_target_buffer.Get(), NULL, render_target_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	else
	{
		BOOL allow_tearing = FALSE;
		if (SUCCEEDED(hr))
		{
			hr = dxgi_factory6->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(allow_tearing));
		}
		tearing_supported = SUCCEEDED(hr) && allow_tearing;

		DXGI_SWAP_CHAIN_DESC1 swap_chain_desc1{};
		swap_chain_desc1.Width = framebuffer_dimensions.cx;
		swap_chain_desc1.Height = framebuffer_dimensions.cy;
		swap_chain_desc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swap_chain_desc1.Stereo = FALSE;
		swap_chain_desc1.SampleDesc.Count = 1;
		swap_chain_desc1.SampleDesc.Quality = 0;
		swap_chain_desc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc1.BufferCount = 2;
		swap_chain_desc1.Scaling = DXGI_SCALING_STRETCH;
		swap_chain_desc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swap_chain_desc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swap_chain_desc1.Flags = tearing_supported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
		hr = dxgi_factory6->CreateSwapChainForHwnd(graphics.GetDevice(), hWnd, &swap_chain_desc1, NULL, NULL, swap_chain.ReleaseAndGetAddressOf());
#if 0
		swap_chain_desc1.Flags |= DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
#endif
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = dxgi_factory6->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target_buffer;
		hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(render_target_buffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		hr = graphics.GetDevice()->CreateRenderTargetView(render_target_buffer.Get(), NULL, render_target_view.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer{};
	D3D11_TEXTURE2D_DESC texture2d_desc{};
	texture2d_desc.Width = framebuffer_dimensions.cx;
	texture2d_desc.Height = framebuffer_dimensions.cy;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;
	hr = graphics.GetDevice()->CreateTexture2D(&texture2d_desc, NULL, depth_stencil_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	hr = graphics.GetDevice()->CreateDepthStencilView(depth_stencil_buffer.Get(), &depth_stencil_view_desc, depth_stencil_view.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(framebuffer_dimensions.cx);
	viewport.Height = static_cast<float>(framebuffer_dimensions.cy);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	graphics.GetDeviceContext()->RSSetViewports(1, &viewport);
}

// アプリケーションループ
int Framework::Run()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();
			CalculateFrameStats();

			float elapsedTime = syncInterval == 1
				? timer.TimeInterval()
				: syncInterval / 60.0f
				;
			Update(elapsedTime);
			Render(elapsedTime);
		}
	}
	return static_cast<int>(msg.wParam);
}

// メッセージハンドラ
LRESULT CALLBACK Framework::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Graphics::Instance().GetImGuiRenderer()->HandleMessage(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.Start();
		break;
	/*case WM_SIZE:
	{
#if 1
		RECT client_rect{};
		GetClientRect(hWnd, &client_rect);
		on_size_changed(static_cast<UINT64>(client_rect.right - client_rect.left), client_rect.bottom - client_rect.top);
#endif
		break;
	}*/
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
