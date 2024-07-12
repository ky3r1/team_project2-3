#pragma once


#include "Graphics/sprite_bach.h"
#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Enemy01.h"

// ゲームシーン
class SceneTutorial : public Scene
{
public:
	SceneTutorial() {}
	~SceneTutorial() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;
private:
	void UpdateTutorial(float elapsedTime);
	void EasingTexture(float elapsedTime);
	void TextRender(ID3D11DeviceContext* dc);
	void CheckBoxRender(ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos,bool flg);

	float timer=0.0f;
	enum
	{
		move_check,//0
		enemy_facade
	};
	int state = 0;
	int substate = 0;
private:
	bool checker[10] = {true, true, true, true, true, true, true, true, true, true};
	bool nextstate_checker = false;
	DirectX::XMFLOAT2 texture_pos[3] = { };
private:
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<CameraController> cameraController = nullptr;
	std::unique_ptr<Sprite> gauge = nullptr;

	Enemy01* e = nullptr;

	std::unique_ptr<Sprite> sprite_frame = nullptr;
	std::unique_ptr<Sprite> sprite_checkmark = nullptr;
	std::unique_ptr<Sprite> sprite_rightclick = nullptr;
	std::unique_ptr<Sprite> sprite01 = nullptr;
	std::unique_ptr<Sprite> sprite02 = nullptr;
	std::unique_ptr<Sprite> sprite03 = nullptr;
};