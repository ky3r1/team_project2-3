#pragma once


#include "Graphics/sprite_bach.h"
#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"

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

private:
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<CameraController> cameraController = nullptr;
	std::unique_ptr<Sprite> gauge = nullptr;
};