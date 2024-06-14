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

	// DijkstraSearch(ダイクストラサーチ)
	bool DijkstraSearch(Stage* stage);
	// Dijkstraで使用するコスト計算関数
	Edge* searchMinCostEdge(std::vector<Edge*>& FNR, Stage* stage);

	// A*Search(エースターサーチ)
	bool AStarSearch(Stage* stage);
	// A*で使用するコスト計算関数
	Edge* HeuristicSearchMinCostEdge(std::vector<Edge*>& frontier, Stage* stage);
	// A*で使用する見積コスト計算関数
	float heuristicCulc(WayPoint* N1, WayPoint* N2);

	//探索したエッジをを記憶するワーク
	std::vector<Edge*> searchEdge;

	//答えルートのエッジをを記憶するワーク
	std::vector<int> findRoot;
	// 探査したデータを表示
	void SearchRender(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, Stage* stage);
	void AddSerchArrow(Stage* stage);
	void SearchClear(Stage* stage);
	float AddAnswerArrow(Stage* stage);
	void GoldenPathSpawn(Stage* stage, DirectX::XMFLOAT3 startPosition);


};