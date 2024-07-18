#pragma once

#include <windows.h>
#include "HighResolutionTimer.h"
#include "Audio/Audio.h"
#include "Graphics/Graphics.h"
#include "Input/input.h"
#include <dxgi1_6.h>


class Framework
{
public:
	Framework(HWND hWnd);
	~Framework();

private:
	void Update(float elapsedTime/*Elapsed seconds from last frame*/);
	void Render(float elapsedTime/*Elapsed seconds from last frame*/);

	void CalculateFrameStats();

	void stylize_window(BOOL fullscreen);

	void on_size_changed(UINT64 width, UINT height);

	void create_swap_chain(IDXGIFactory6* dxgi_factory6);


public:
	int Run();
	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	const HWND				hWnd;
	SIZE framebuffer_dimensions;
	HighResolutionTimer		timer;
	Audio					audio;
	Graphics				graphics;
	Input					input;

	BOOL fullscreen_mode{ TRUE };
	BOOL vsync{ TRUE };
	BOOL tearing_supported{ FALSE };


	RECT windowed_rect;
	DWORD windowed_style;


	Microsoft::WRL::ComPtr<IDXGISwapChain1> swap_chain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view = graphics.GetRenderTargetView();
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view = graphics.GetDepthStencilView();
};
