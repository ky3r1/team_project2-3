#include "SceneTutorial.h"

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

	state = enemy_facade;
	substate = 0;
	timer = 0;
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
	if (Player::Instance().GetHealth() <= 0)SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame()));
#endif //  ALLPLAYER
	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);
	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
	//tutorial更新
	UpdateTutorial(elapsedTime);
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


	if (nextstate_checker)EasingTexture(elapsedTime);
	else
	{
		if (first_in)
		{
			if (state == enemy_facade)	e = new Enemy01(ENEMYCATEGORY);
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
		}
	}
	timer++;
}

void SceneTutorial::EasingTexture(float elapsedTime)
{
	static bool first_in = true;
	switch (state)
	{
	case move_check:
		if (first_in)
		{
			texture_pos[0] = {};
			texture_pos[0] = {};
		}
		switch (substate)
		{
		case 0:
			nextstate_checker = false;
			break;
		case 1:
			state = enemy_facade;
			substate = 0;
			first_in = false;
			break;
		}
		break;
	case enemy_facade:
		if (first_in)
		{
			texture_pos[0] = {};
			texture_pos[0] = {};
		}
		switch (substate)
		{
		case 0:
			nextstate_checker = false;
			break;
		case 1:
			state = move_check;
			substate = 0;
			first_in = false;
			break;
		}
		break;
	}
}

void SceneTutorial::TextRender(ID3D11DeviceContext* dc)
{
	static bool first_in = true;
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
	
	switch (state)
	{
	case move_check:
		if (first_in)
		{
			if (sprite01 != nullptr) sprite01.reset();
			if (sprite02 != nullptr) sprite02.reset();
			if (sprite03 != nullptr) sprite03.reset();
			sprite01 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/MoveWASD-removebg-preview.png"));
			sprite02 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/MoveCamera-removebg-preview.png"));
			first_in=false;
		}
		//WASDで移動
		scale = 0.55f;
		size={ 497 , 141 };
		CheckBoxRender(dc, DirectX::XMFLOAT2(10, 40), checker[0]);
		sprite01.get()->Render(dc,
            DirectX::XMFLOAT2(36, 33),
            DirectX::XMFLOAT2(size.x* scale, size.y* scale),
            DirectX::XMFLOAT2(0, 0),
            DirectX::XMFLOAT2(size.x, size.y),
            0,
            DirectX::XMFLOAT4(0, 0, 0, 1)
        );
		scale = 0.5f;
		size = { 723, 129 };
		//カメラの移動
        CheckBoxRender(dc, DirectX::XMFLOAT2(10, 110), checker[1]);
		sprite02.get()->Render(dc,
			DirectX::XMFLOAT2(50, 33+70),
			DirectX::XMFLOAT2(size.x * scale, size.y * scale),
			DirectX::XMFLOAT2(0, 0),
			DirectX::XMFLOAT2(size.x, size.y),
			0,
			DirectX::XMFLOAT4(0, 0, 0, 1)
		);
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
		if (first_in)
		{
			if (sprite01 != nullptr) sprite01.reset();
			if (sprite02 != nullptr) sprite02.reset();
			if (sprite03 != nullptr) sprite03.reset();
			sprite01 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/AttackEnemy-removebg-preview.png"));
			sprite02 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/AddDamage-removebg-preview.png"));
			sprite03 = std::unique_ptr<Sprite>(new Sprite("Data/Sprite/turorial_texture/DeathEnemy-removebg-preview.png"));
			
			first_in = false;
		}
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

		//エネミーにダメージを与える
		scale = 0.5f;
		size = { 780,137 };
		CheckBoxRender(dc, DirectX::XMFLOAT2(10, 160), checker[0]);
		sprite02.get()->Render(dc,
			DirectX::XMFLOAT2(40, 145),
			DirectX::XMFLOAT2(size.x * scale, size.y * scale),
			DirectX::XMFLOAT2(0, 0),
			DirectX::XMFLOAT2(size.x, size.y),
			0,
			DirectX::XMFLOAT4(0, 0, 0, 1)
		);
		//エネミーを倒す
		scale = 0.5f;
		size = { 356,148 };
		CheckBoxRender(dc, DirectX::XMFLOAT2(10, 210), checker[1]);
		sprite03.get()->Render(dc,
			DirectX::XMFLOAT2(40, 195),
			DirectX::XMFLOAT2(size.x * scale, size.y * scale),
			DirectX::XMFLOAT2(0, 0),
			DirectX::XMFLOAT2(size.x, size.y),
			0,
			DirectX::XMFLOAT4(0, 0, 0, 1)
		);

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
