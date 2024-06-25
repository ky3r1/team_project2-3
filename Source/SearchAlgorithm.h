#pragma once
#include "All.h"
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <memory>
#include "Edge.h"
#include "WayPoint.h"
#include "Stage.h"
//#include "Object.h"


#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)

class SearchAlgorithm
{
private:
	SearchAlgorithm();
	~SearchAlgorithm();
public:
	static SearchAlgorithm& Instance()
	{
		static SearchAlgorithm instance;
		return instance;
	}

	// DijkstraSearch(�_�C�N�X�g���T�[�`)
	bool DijkstraSearch(Stage* stage);
	// Dijkstra�Ŏg�p����R�X�g�v�Z�֐�
	Edge* searchMinCostEdge(std::vector<Edge*>& FNR, Stage* stage);

	// A*Search(�G�[�X�^�[�T�[�`)
	bool AStarSearch(Stage* stage);
	// A*�Ŏg�p����R�X�g�v�Z�֐�
	Edge* HeuristicSearchMinCostEdge(std::vector<Edge*>& frontier, Stage* stage);
	// A*�Ŏg�p���錩�σR�X�g�v�Z�֐�
	float heuristicCulc(WayPoint* N1, WayPoint* N2);

	//�T�������G�b�W�����L�����郏�[�N
	std::vector<Edge*> searchEdge;

	//�������[�g�̃G�b�W�����L�����郏�[�N
	std::vector<int> findRoot;
	// �T�������f�[�^��\��
	void SearchRender(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, Stage* stage);
	void AddSerchArrow(Stage* stage);
	void SearchClear(Stage* stage);
	float AddAnswerArrow(Stage* stage);
	void GoldenPathSpawn(Stage* stage, DirectX::XMFLOAT3 startPosition);


};