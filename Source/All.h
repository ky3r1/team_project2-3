#pragma once

//インクルード
#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "Graphics/Graphics.h"
#include <vector>
#include <set>
#include <imgui.h>

///////////////////デバッグ用define////////////////////////

//ImGui
#define DEBUGIMGUI

///////////////////Stage////////////////////////

//全ステージ
#define ALLSTAGE
//動く床
//#define STAGEMOVE

///////////////////Camera////////////////////////
#define MOUSECAMERA

///////////////////Character////////////////////////

//Player
//プレイヤー
#define ALLPLAYER
//アニメーション
#define PLAYERANIMATION
//Jump
//#define JUMPFRAG
// 攻撃
//#define PLAYERATTACK
//Player->Enemy 弾のダメージ
#define PROJECTILEDAMAGE
//Player->Enemy 踏みつけダメージ
#define JUMPDAMAGE

//Enemy->Player 接触ダメージ
#define ENEMYHITTINGDAMAGE

//Enemy
//エネミー
#define ALLENEMY
//スライム
#define ENEMYSLIME

//ステートマシン
#define ENEMYSTATEMACHINE
//アニメーション
#define ENEMYANIMATION

///////////////////UI////////////////////////

//HPゲージ
#define HPGAUGE
//EnemyAdd
//#define ENEMYADD

///////////////////CATEGORY////////////////////////
#define RED 0       //enemy01
#define GREEN 1     //enemy02
#define BLUE 2      //enemy03
#define YELLOW 3    //enemy04
#define PURPLE 4    //enemy05
#define WHITE 5    //enemy06