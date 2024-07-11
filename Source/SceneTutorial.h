#pragma once


#include "Graphics/sprite_bach.h"
#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"

// �Q�[���V�[��
class SceneTutorial : public Scene
{
public:
	SceneTutorial() {}
	~SceneTutorial() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
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