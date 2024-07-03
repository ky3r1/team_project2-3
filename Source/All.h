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
#define PLAYERATTACK
//Player->Enemy 弾のダメージ
#define PROJECTILEDAMAGE
//Player->Enemy 踏みつけダメージ
#define JUMPDAMAGE

//Enemy->Player 接触ダメージ
#define ENEMYHITTINGDAMAGE

//Enemy
//エネミー
#define ALLENEMY
//01
#define ENEMY01 
//02
#define ENEMY02
//03
#define ENEMY03 
//BOSS
#define ENEMYBOSS 

// 攻撃
#define ENEMYATTACK

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
#define PLAYERCATEGORY 0       //Player
#define ENEMYCATEGORY 1     //Enemy