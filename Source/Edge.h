#pragma once
#include "All.h"

// 0  /*�G�b�W�̕\���@12���̕���*/
// 1  /*�G�b�W�̕\���@2.5���̕���*/
// 2  /*�G�b�W�̕\���@�R���̕���*/
// 3  /*�G�b�W�̕\���@4.5���̕���*/
// 4  /*�G�b�W�̕\���@6���̕���*/
// 5  /*�G�b�W�̕\���@7.5���̕���*/
// 6  /*�G�b�W�̕\���@9���̕���*/
// 7  /*�G�b�W�̕\���@10.5���̕���*/

//       7   0   1
//        �_ | �^
//       6�\    �\2
//        �^ | �_
//       5   4   3

class Edge
{
public:
	//�R���X�g���N�^
	Edge():cost(0),originPoint(-1),destinationPoint(-1){}
	~Edge(){}
	int originPoint=-1;
	int destinationPoint=-1;
	float cost = 0;
};