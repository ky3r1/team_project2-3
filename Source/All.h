#pragma once

//�C���N���[�h
#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "Graphics/Graphics.h"
#include <vector>
#include <set>
#include <imgui.h>

///////////////////�f�o�b�O�pdefine////////////////////////

//ImGui
#define DEBUGIMGUI

///////////////////Stage////////////////////////

//�S�X�e�[�W
#define ALLSTAGE
//������
//#define STAGEMOVE

///////////////////Camera////////////////////////
#define MOUSECAMERA

///////////////////Character////////////////////////

//Player
//�v���C���[
#define ALLPLAYER
//�A�j���[�V����
#define PLAYERANIMATION
//Jump
//#define JUMPFRAG
// �U��
#define PLAYERATTACK
// �e�̎��
#define PENETRATION 0
#define RICOCHET 1
//Player->Enemy �e�̃_���[�W
#define PROJECTILEDAMAGE
//Player->Enemy ���݂��_���[�W
#define JUMPDAMAGE

//Enemy->Player �ڐG�_���[�W
#define ENEMYHITTINGDAMAGE

//Enemy
//�G�l�~�[
#define ALLENEMY
////�X�|�[��
//#define SPOWNENEMY

////01
#define ENEMY01 
//02
#define ENEMY02
//03
#define ENEMY03 
//BOSS
//#define ENEMYBOSS 

// �U��
#define ENEMYATTACK

//�X�e�[�g�}�V��
#define ENEMYSTATEMACHINE
//�A�j���[�V����
#define ENEMYANIMATION

///////////////////UI////////////////////////

//HP�Q�[�W
#define HPGAUGE
//EnemyAdd
//#define ENEMYADD

///////////////////CATEGORY////////////////////////
#define PLAYERCATEGORY 0       //Player
#define ENEMYCATEGORY 1     //Enemy