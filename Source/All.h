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
#define PLAYERATTACK
// 弾の種類
#define PENETRATION 0
#define RICOCHET 1
#define NORMAL 2
//Player->Enemy 弾のダメージ
#define PROJECTILEDAMAGE
//Player->Enemy 踏みつけダメージ
#define JUMPDAMAGE

//Enemy->Player 接触ダメージ
#define ENEMYHITTINGDAMAGE

//Enemy
//エネミー
#define ALLENEMY
//スポーン
#define SPOWNENEMY

////01
//#define ENEMY01 
////02
//#define ENEMY02
////03
//#define ENEMY03 
//BOSS
//#define ENEMYBOSS 

// 攻撃
#define ENEMYATTACK

//ステートマシン
#define ENEMYSTATEMACHINE
//アニメーション
#define ENEMYANIMATION

///////////////////UI////////////////////////

////HPゲージ
//#define HPGAUGE
//EnemyAdd
//#define ENEMYADD

///////////////////CATEGORY////////////////////////
#define PLAYERCATEGORY 0       //Player
#define ENEMYCATEGORY 1     //Enemy

///////////////////Stage////////////////////////

//全ステージ
#define ALLSTAGE
//動く床
//#define STAGEMOVE

//Stageの最大値
#define StageTopMax_X 40
#define StageTopMax_Y 40
#define StageBottomMax_X -40
#define StageBottomMax_Y -40