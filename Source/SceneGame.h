#pragma once

#include "Scene.h"
#include "Player.h"
#include "Spown.h"
#include "CameraController.h"
#include "Graphics/Sprite.h"
#include "Graphics/Font.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;
private:
	void RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
	void RenderPlayerGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
	void CharacterGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, DirectX::XMFLOAT3 position,int health,DirectX::XMFLOAT4 gaugecolor);
	void CrickEnemyAdd(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
private:
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<Spown> spown = nullptr;
	std::unique_ptr<CameraController> cameraController = nullptr;
	std::unique_ptr<Sprite> gauge = nullptr;
	std::unique_ptr<Font> font;

	int mapcategory = 0;
};