﻿#include "SceneTutorial.h"

//CameraInclude
#include "Camera.h"

//CharacterInclude
#include "EnemyManager.h"
#include "Enemy01.h"
#include "Enemy02.h"
#include "Enemy03.h"
#include "EnemyBoss.h"
#include "EffectManager.h"
#include "MouseManager.h"

//SceneIncldue
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneResult.h"
#include "SceneTitle.h"
#include "SceneGame.h"

//StageIncldue
#include "StageManager.h"
#include "StageMain.h"
//#include "StageMapChip.h"
//#include "StageMapChip.h"

#include "Input/Input.h"
#include "Easing.h"
#include "Ui.h"


// 初期化
void SceneTutorial::Initialize()
{
	//ステージ初期化
#ifdef ALLSTAGE
	//Main
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);
#endif // ALLSTAGE


#ifdef ALLPLAYER
	player = std::unique_ptr<Player>(new Player);
#endif // PLAYER


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

	sprite_frame = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/frame.png"));
	sprite_checkmark = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/checkmark.png"));
	sprite_rightclick = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/RightClick-removebg-preview.png"));

	//move_check,//0
	//enemy_facade,
	//enemy_attack,

	state = move_check;
	substate = 0;
	easing_flg_first = true;
	nextstate_checker = true;
}

// 終了化
void SceneTutorial::Finalize()
{
	Player::Instance().Clear();
	//エネミー終了化
	EnemyManager::Instance().clear();
	//ステージ終了化
	StageManager::Instance().Clear();
}

// 更新処理
void SceneTutorial::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = Player::Instance().GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	StageManager::Instance().Update(elapsedTime);

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	MouseManager::GetInstance().MouseTransform(dc, Camera::Instance().GetView(), Camera::Instance().GetProjection());

#ifdef  ALLPLAYER
	Player::Instance().Update(elapsedTime);
	if (Player::Instance().GetHealth() <= 0)SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle()));
#endif //  ALLPLAYER
	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);
	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
	//tutorial更新
	UpdateTutorial(elapsedTime);
	if(state==next_scene)SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle()));
}

// 描画処理
void SceneTutorial::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.4f, 0.4f, 0.4f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		StageManager::Instance().Render(dc, shader);

		//エネミー描画
		EnemyManager::Instance().Render(dc, shader);

		//プレイヤー描画
#ifdef  ALLPLAYER
		Player::Instance().Render(dc, shader);
#endif //  ALLPLAYER
		shader->End(dc);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
#ifdef  DEBUGIMGUI
		//プレイヤーデバッグプリミティブ描画
		Player::Instance().DrawDebugPrimitive();
		//エネミーデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
#endif //DEBUGIMGUI
	}

	// 2Dスプライト描画
	{
		Ui::Instance().game(dc);
		TextRender(dc);
	}

#ifdef DEBUGIMGUI
	// デバッグGUI描画
	Player::Instance().DrawDebugGUI();
	cameraController->DrawDebugGUI();
	EnemyManager::Instance().DrawDebugGUI();
	StageManager::Instance().DrawDebugGUI();
#endif // DebugImGui
}

void SceneTutorial::UpdateTutorial(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	Mouse& mouse = Input::Instance().GetMouse();

	static bool first_in = true;
	static float type = 100;


	if (nextstate_checker)EasingTexture(elapsedTime);
	else
	{
		if (first_in)
		{
				if (state == enemy_facade)	e = new Enemy01(ENEMYCATEGORY);
				if (state == enemy_attack)
				{
					for (int i = 0; i < 3; ++i)
					{
						e2[i] = new Enemy01(ENEMYCATEGORY);
					}
				}
				first_in = false;	
		}
		switch (state)
		{
		case move_check:
			switch (substate)
			{
			case 0:
				checker[0] = false;
				checker[1] = false;
				substate++;
				break;
			case 1:
				//上下左右の移動
				if (gamePad.GetButton() & gamePad.BTN_UP ||
					gamePad.GetButton() & gamePad.BTN_DOWN ||
					gamePad.GetButton() & gamePad.BTN_LEFT ||
					gamePad.GetButton() & gamePad.BTN_RIGHT)
				{
					checker[0] = true;
					player.get()->SetScale(DirectX::XMFLOAT3(10, 10, 10));
				}
				static DirectX::XMINT2 camera_pos = { mouse.GetPositionX(),mouse.GetPositionY() };
				//カメラの移動
				if (mouse.GetPositionX() != camera_pos.x || mouse.GetPositionY() != camera_pos.y)
				{
					checker[1] = true;
					player.get()->SetScale(DirectX::XMFLOAT3(5, 5, 5));
				}
				if (checker[0] && checker[1])
				{
					//右クリック
					if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
					{
						first_in = true;
						nextstate_checker = true;
					}
				}
				break;
			}
			break;
		case enemy_facade:
			switch (substate)
			{
			case 0:
				e->SetPosition(DirectX::XMFLOAT3(0.0f, 0, 20));
				EnemyManager::Instance().Register(e);

				checker[0] = false;
				checker[1] = false;
				substate++;
				break;
			case 1:
				if (e->GetHealth() != e->GetMaxHealth())
				{
					checker[0] = true;
				}
				if (EnemyManager::Instance().GetEnemyCount() == 0)
				{
					checker[1] = true;
					player.get()->SetScale(DirectX::XMFLOAT3(10, 10, 10));
				}
				if (checker[0] && checker[1])
				{
					//右クリック
					if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
					{
						first_in = true;
						nextstate_checker = true;
					}
				}
				break;
			}
			break;
		case enemy_attack:
			switch (substate)
			{
			case 0:
				for(int i=0;i<3;++i)
                {
                    e2[i]->SetPosition(DirectX::XMFLOAT3(-10.0f + 10.0f * i, 0, 20));
                    EnemyManager::Instance().Register(e2[i]);
                }

				type = player.get()->GetProjectileType();
				checker[0] = false;
				checker[1] = false;
				substate++;
				break;
			case 1:
				static bool flg = false;
				if (!flg)
				{
					for (int index = 0; index < EnemyManager::Instance().GetEnemyCount(); index++)
					{
						Enemy* enemy = EnemyManager::Instance().GetEnemy(index);
						if (enemy != nullptr)
						{
							if (player.get()->GetProjectileType() == PENETRATION&&checker[1])
							{
								checker[0] = true;
								player.get()->SetScale(DirectX::XMFLOAT3(5, 5, 5));
							}
							if (player.get()->GetProjectileType() == RICOCHET)
							{
								checker[1] = true;
								player.get()->SetScale(DirectX::XMFLOAT3(0.1, 0.1, 0.1));
							}
						}
					}
				}
				if (EnemyManager::Instance().GetEnemyCount() == 0)
				{
				}
				if (checker[0] && checker[1])
				{
					flg = true;
					//右クリック
					if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
					{
						first_in = true;
						nextstate_checker = true;
					}
				}
				break;
			}
			break;
		}
	}
}

void SceneTutorial::EasingTexture(float elapsedTime)
{
	static bool easing_flg = true;
	switch (state)
	{
	case move_check:
		if (easing_flg_first)
		{
			texture_pos01 = {-1000,33};//DirectX::XMFLOAT2(36, 33)
			texture_pos02 = {-1000,103 };//DirectX::XMFLOAT2(50, 103)
			easing_timer[0] = 0;
			easing_timer[1] = 0;
			easing_timer[2] = 0;
			easing_timer[3] = 0;
			easing_flg_first = false;
		}
		switch (substate)
		{
		case 0:
			if (EasingMove(texture_pos01, DirectX::XMFLOAT2(36, -1000), easing_timer[0],true)
				&&
				EasingMove(texture_pos02, DirectX::XMFLOAT2(50, -1000), easing_timer[1],true)
				)
			{
				nextstate_checker = false;
			}
			break;
		case 1:
			if (EasingMove(texture_pos01, DirectX::XMFLOAT2(-500, 36), easing_timer[2],false)
				&&
				EasingMove(texture_pos02, DirectX::XMFLOAT2(-500, 50), easing_timer[3],false)
				)
			{
				state = enemy_facade;
				substate = 0;
				easing_flg_first = true;
			}
			break;
		}
		break;
	case enemy_facade:
		if (easing_flg_first)
		{
			texture_pos01 = { -1000,145 };//DirectX::XMFLOAT2(40, 145)
			texture_pos02 = { -1000,195 };//DirectX::XMFLOAT2(40, 195)
			easing_timer[0] = 0;
			easing_timer[1] = 0;
			easing_timer[2] = 0;
			easing_timer[3] = 0;
			easing_flg_first = false;
		}
		switch (substate)
		{
		case 0:
			if (EasingMove(texture_pos01, DirectX::XMFLOAT2(40, -1000), easing_timer[0], true)
				&&
				EasingMove(texture_pos02, DirectX::XMFLOAT2(40, -1000), easing_timer[1], true)
				)
			{
				nextstate_checker = false;
			}
			break;
		case 1:
			if (EasingMove(texture_pos01, DirectX::XMFLOAT2(-500, 40), easing_timer[2], false)
				&&
				EasingMove(texture_pos02, DirectX::XMFLOAT2(-500, 40), easing_timer[3], false)
				)
			{
				state = enemy_attack;
				substate = 0;
				easing_flg_first = true;
			}
			break;
		}
		break;
	case enemy_attack:
		if (easing_flg_first)
		{
			texture_pos01 = { -1000,156 };//DirectX::XMFLOAT2(50, 156)
			texture_pos02 = { -1000,206 };//DirectX::XMFLOAT2(50, 206)
			easing_timer[0] = 0;
			easing_timer[1] = 0;
			easing_timer[2] = 0;
			easing_timer[3] = 0;
			easing_flg_first = false;
		}
		switch (substate)
		{
		case 0:
			if (EasingMove(texture_pos01, DirectX::XMFLOAT2(50, -1000), easing_timer[0], true)
				&&
				EasingMove(texture_pos02, DirectX::XMFLOAT2(50, -1000), easing_timer[1], true)
				)
			{
				nextstate_checker = false;
			}
			break;
		case 1:
			if (EasingMove(texture_pos01, DirectX::XMFLOAT2(-500, 50), easing_timer[2], false)
				&&
				EasingMove(texture_pos02, DirectX::XMFLOAT2(-500, 50), easing_timer[3], false)
				)
			{
				state = next_scene;
				substate = 0;
				easing_flg_first = true;
			}
			break;
		}
		break;
	}
}

void SceneTutorial::TextRender(ID3D11DeviceContext* dc)
{
	static DirectX::XMFLOAT2 size = {};
	static float scale = 0.0f;
	sprite_frame.get()->Render(dc,
		DirectX::XMFLOAT2(-40, 10),
		DirectX::XMFLOAT2(500, 500),
		DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2(100, 100),
		0,
		DirectX::XMFLOAT4(0, 0, 0, 1)
	);

	if (first_texture)
	{
		sprite01 = nullptr;
		sprite02 = nullptr;
		sprite03 = nullptr;
		sprite04 = nullptr;
	}

	switch (state)
	{
	case move_check:
		if (first_texture)
		{
			{
				sprite01 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/MoveWASD-removebg-preview.png"));
				sprite02 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/MoveCamera-removebg-preview.png"));
			}
			checker[0] = false;
			checker[1] = false;
			first_texture = false;
		}
		if (sprite01 != nullptr)
		{
			//WASDで移動
			scale = 0.55f;
			size = { 497 , 141 };
			CheckBoxRender(dc, DirectX::XMFLOAT2(10, 40), checker[0]);
			sprite01.get()->Render(dc,
				texture_pos01,
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		if (sprite02 != nullptr)
		{
			scale = 0.5f;
			size = { 723, 129 };
			//カメラの移動
			CheckBoxRender(dc, DirectX::XMFLOAT2(10, 110), checker[1]);
			sprite02.get()->Render(dc,
				texture_pos02,
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		if (checker[0] && checker[1])
		{
			scale = 0.5f;
			size = { 637, 143 };
			sprite_rightclick.get()->Render(dc,
				DirectX::XMFLOAT2(50, 430),
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		break;
	case enemy_facade:
		if (first_texture)
		{
			{	
				sprite01 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/AttackEnemy-removebg-preview.png"));
				sprite02 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/AddDamage-removebg-preview.png"));
				sprite03 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/DeathEnemy-removebg-preview.png"));
				sprite04 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/Rect-removebg-preview.png"));
			}
			checker[0] = false;
			checker[1] = false;
			first_texture = false;
		}
		if (sprite01 != nullptr)
		{
			//赤枠に入ると攻撃
			scale = 0.55f;
			size = { 608, 186 };
			sprite01.get()->Render(dc,
				DirectX::XMFLOAT2(36, 33),
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		if (sprite02 != nullptr)
		{
			//エネミーにダメージを与える
			scale = 0.5f;
			size = { 780,137 };
			CheckBoxRender(dc, DirectX::XMFLOAT2(10, 160), checker[0]);
			sprite02.get()->Render(dc,
				texture_pos01,
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		if (sprite03 != nullptr)
		{
			//エネミーを倒す
			scale = 0.5f;
			size = { 356,148 };
			CheckBoxRender(dc, DirectX::XMFLOAT2(10, 210), checker[1]);
			sprite03.get()->Render(dc,
				texture_pos02,
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		if (sprite04 != nullptr)
		{
			//Rect
			scale = 0.4f;
			size = { 708, 353 };
			sprite04.get()->Render(dc,
				DirectX::XMFLOAT2(40, 270),
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		if (checker[0] && checker[1])
		{
			scale = 0.5f;
			size = { 637, 143 };
			sprite_rightclick.get()->Render(dc,
				DirectX::XMFLOAT2(50, 430),
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		break;

	case enemy_attack:
		if (first_texture)
		{
			{
				sprite01 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/RightProjectile-removebg-preview.png"));
				sprite02 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/k2-removebg-preview.png"));
				sprite03 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/cd-removebg-preview.png"));
			}
			checker[0] = false;
			checker[1] = false;
			first_texture = false;
		}
		if (sprite01 != nullptr)
		{
			//内枠の説明
			scale = 0.55f;
			size = { 846, 194 };
			sprite01.get()->Render(dc,
				DirectX::XMFLOAT2(0, 33),
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		if (sprite02 != nullptr)
		{
			//貫通
			scale = 0.5f;
			size = { 450,96 };
			CheckBoxRender(dc, DirectX::XMFLOAT2(10, 160), checker[0]);
			sprite02.get()->Render(dc,
				texture_pos01,
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		if (sprite03 != nullptr)
		{
			//跳弾
			scale = 0.5f;
			size = { 377,104 };
			CheckBoxRender(dc, DirectX::XMFLOAT2(10, 210), checker[1]);
			sprite03.get()->Render(dc,
				texture_pos02,
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		if (checker[0] && checker[1])
		{
			scale = 0.5f;
			size = { 637, 143 };
			sprite_rightclick.get()->Render(dc,
				DirectX::XMFLOAT2(50, 430),
				DirectX::XMFLOAT2(size.x * scale, size.y * scale),
				DirectX::XMFLOAT2(0, 0),
				DirectX::XMFLOAT2(size.x, size.y),
				0,
				DirectX::XMFLOAT4(0, 0, 0, 1)
			);
		}
		break;
	}
}

void SceneTutorial::CheckBoxRender(ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos,bool flg)
{
	//CheckBox
	sprite_frame.get()->Render(dc,
		DirectX::XMFLOAT2(pos.x, pos.y),
		DirectX::XMFLOAT2(40, 40),
		DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2(100, 100),
		0,
		DirectX::XMFLOAT4(1, 0, 0, 1)
	);
	if (flg)
	{
		sprite_checkmark.get()->Render(dc,
			DirectX::XMFLOAT2(pos.x, pos.y),
			DirectX::XMFLOAT2(40, 40),
			DirectX::XMFLOAT2(0, 0),
			DirectX::XMFLOAT2(100, 100),
			0,
			DirectX::XMFLOAT4(1, 0, 0, 1)
		);
	}
}

bool SceneTutorial::EasingMove(DirectX::XMFLOAT2& pos, DirectX::XMFLOAT2 target,int& timer,bool flg)
{
	first_texture = true;
	timer++;
	pos = { Easing::OutBounce(timer / 60.0f, 100.0f / 60.0f, target.x, target.y),pos.y };
	if (flg)
	{
		if (pos.x >= target.x)
		{
			pos.x = target.x;
			return true;
		}
	}
	else
	{
		if (pos.x <= target.x)
		{
			pos.x = target.x;
			return true;
		}
	}

	return false;
}
