#include "Graphics/Graphics.h"
#include "SearchAlgorithm.h"


SearchAlgorithm::SearchAlgorithm()
{
	for (int i = 0; i < MAX_WAY_POINT; ++i)
	{
		findRoot.emplace_back(-1);
	}
}
SearchAlgorithm::~SearchAlgorithm()
{
	searchEdge.clear();
}

bool SearchAlgorithm::DijkstraSearch(Stage* stage)
{
	//�G�b�W�^�f�[�^�̃t�����e�B�A�c���[���쐬
	std::vector<Edge*> frontier;

	// �ߋ��̒T���f�[�^���N���A
	SearchClear(stage);

	//�X�^�[�gID��originNode �S�[��ID��distnationNode�Ɏ��_�~�[�G�b�W���쐬
	Edge* edge = debug_new Edge();

	//�_�~�[�G�b�W�͐ڑ����Ɛڑ���͊J�n�n�_�ɂ��Ă����B
	edge->destinationPoint = stage->NearWayPointIndex(Start::Instance().GetPosition());
	edge->originPoint = edge->destinationPoint;
	// �R�X�g0���w��(�܂�Ԃ��ŃX�^�[�g�n�_�ɖ߂�Ȃ��悤�ɂ��邽��)
	stage->wayPoint[edge->originPoint]->costFromStart = 0.0f;
	//�O�����Ƃ��ă_�~�[�G�b�W�f�[�^���Z�b�g
	Edge* nowEdge = edge;

	//TODO 07_01 DijkstraSseach�A���S���Y��
	// �������猊�����ɂȂ��Ă��鏈�����l�����߂Ă����Ȃ���
	while (true)
	{
		//�T�[�`����Edge�̋L�^(���s���ʂ̕\���p�j
		findRoot[nowEdge->destinationPoint] = nowEdge->originPoint;

		//�����A����nowEdge��distnationNode���S�[����������true��return����B
		if (nowEdge->destinationPoint == stage->NearWayPointIndex((Goal::Instance().GetPosition())))
		{
			delete edge;
			return true;
		}

		//nowEdge�̐�̃m�[�h���擾����B
		WayPoint* wayPoint = stage->wayPoint[nowEdge->destinationPoint];//�i�ݐ�̃m�[�h

		//nowEdge�̐�̃m�[�h�ɓo�^���Ă���W�{�̃G�b�W���T�[�`���郋�[�v
		for (int i = 0; i < EdgeNo; i++) {

			Edge* nextEdge = wayPoint->edge[i];

			if (nextEdge->destinationPoint >= 0) {//���̃G�b�W�͗L���ł���B

				WayPoint* nextPoint = stage->wayPoint[nextEdge->destinationPoint];//�i�ݐ�̃m�[�h

				// TODO 07_02 �i�ݐ�̃m�[�h�܂ł̃R�X�g���v�Z
				float newCost = wayPoint->costFromStart + nextEdge->cost;

				// TODO 07_03 �i�ݐ�̃R�X�g���܂��v�Z����Ă��Ȃ����A�V�����R�X�g�̕����Ⴏ��Γo�^
				if (nextPoint->costFromStart < 0 || newCost < nextPoint->costFromStart)
				{
					nextPoint->costFromStart = newCost;
					findRoot[nextEdge->destinationPoint] = nowEdge->destinationPoint; // findRoot���X�V

					// �t�����e�B�A�ɒǉ�
					frontier.push_back(nextEdge);
				}
			}
		}

		nowEdge = searchMinCostEdge(frontier, stage);

		if (nowEdge == nullptr)
		{
			delete edge;
			return false;
		}
	}

	//����������Ȃ����false;
	return false;
}
bool SearchAlgorithm::AStarSearch(Stage* stage)
{
	//TODO 08_01 DijkstraSseach�A���S���Y�������ǂ��AA*�A���S���Y�����쐬���Ȃ���
	SearchClear(stage);

	return false;
}

Edge* SearchAlgorithm::searchMinCostEdge(std::vector<Edge*>& frontier, Stage* stage)
{
	Edge* answer = nullptr;//�����̓��ꕨ
	int  answerNo = 0;
	float minCost = FLT_MAX;//�ŏ��̃R�X�g��ێ����郏�[�N

	for (int fnrNo = 0; fnrNo < frontier.size(); fnrNo++) {

		Edge* edge = frontier.at(fnrNo);

		// �R�X�g�v�Z
		// totalCost�ɐڑ����́u�X�^�[�g�ʒu����̃R�X�g�v�icostFromStart�j�{�G�b�W���̂����R�X�g�icost�j������
		// element�ɓ����Ă���G�b�W�̐ڑ���m�[�h�́u�X�^�[�g�ʒu����̃R�X�g�v���v�Z����B
		float totalCost = stage->wayPoint[edge->originPoint]->costFromStart + edge->cost;
		
		// �R�X�g���o��
		//�ڑ���́u�X�^�[�g�ʒu����̃R�X�g�v��frontCost�Ɏ��o��(�܂��o�^����Ă��Ȃ��Ȃ�O�ƂȂ�)
		float frontCost = stage->wayPoint[edge->destinationPoint]->costFromStart;

		// �R�X�g�̔�r
		//frontCost��0(�܂��o�^����Ă��Ȃ�)���AfrontCost��菭�Ȃ��R�X�g���[�g(>=totalCost)��
		//�������ꂽ�Ȃ�A�ڑ���́u�X�^�[�g�ʒu����̃R�X�g�v�icostFromStart�j��totalCost�ɏ��������B
		//frontCost��totalCost�ɍX�V�B
		if (frontCost < 0 || totalCost < frontCost)
		{
			stage->wayPoint[edge->destinationPoint]->costFromStart = totalCost;
			frontCost = totalCost;
		}

		// �G�b�W�̋L��
		//frontCost���A�����ׂĂ���G�b�W�inowEdge�j�̐ڑ���̃g�[�^���R�X�g�ȏ�̂��̒���
		//��ԏ������ڑ���́u�X�^�[�g�ʒu����̃R�X�g�v�����G�b�W�𓚂��ianswer�j�Ƃ��ċL���B

		if (frontCost < minCost) {
			minCost = frontCost;
			answer = frontier[fnrNo];
			answerNo = fnrNo;
		}
	}

	if (answer == nullptr)return 0;
	searchEdge.push_back(answer);//�T�[�`����Edge�̋L�^(�D�F���C��)

	frontier.erase(frontier.begin() + answerNo);//�����̃G�b�W�̓_�C�N�X�g���̃T�[�`��₩��O��

	//����FNR�ɓo�^����̂͂��̖߂�l�ɓo�^����Ă���m�[�h�̃G�b�W�Q
	return answer;
}
Edge* SearchAlgorithm::HeuristicSearchMinCostEdge(std::vector<Edge*>& frontier,Stage* stage)
{
	// TODO 08_02 searchMinCostEdge�����ǂ��AA*�p�̃R�X�g�v�Z�ɏ��������Ȃ����B
	// �ʓr�p�ӂ��Ă���heuristicCulc���\�b�h���g�p���Ă��ǂ��B
	Edge* answer = nullptr;//�����̓��ꕨ
	return answer;
}
float SearchAlgorithm::heuristicCulc(WayPoint* w1, WayPoint* w2)
{
	// TODO 08_03 2�_�Ԃ̋��������̂܂܃R�X�g�Ƃ��Čv�Z���郁�\�b�h�ł��B�B
	// cost�̊�����ς��邱�ƂŎ��s���ʂ��قȂ邱�Ƃ��m�F���Ȃ����B
	// �� return cost.x * 0.3�Ȃ�
	DirectX::XMVECTOR start, end;
	start = DirectX::XMLoadFloat3(&w1->position);
	end = DirectX::XMLoadFloat3(&w2->position);
	DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(end, start);
	DirectX::XMVECTOR dist = DirectX::XMVector3Length(DirectX::XMVectorSubtract(end, start));
	DirectX::XMFLOAT3 cost;
	DirectX::XMStoreFloat3(&cost, dist);
	return cost.x;
}
// ID3D11DeviceContext::DrawIndexed: Index buffer has not enough space!���o��̂ō̗p������
void SearchAlgorithm::AddSerchArrow(Stage* stage)
{
	for (const auto edge : searchEdge)
	{
		if (edge->destinationPoint < 0) continue;
		// �T�������G�b�W����ŕ\��
		Arrow* arrow = new Arrow();
		arrow->SetPosition(stage->wayPoint[edge->originPoint]->position);
		arrow->height = edge->cost;
		arrow->color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		if (arrow->height > 2.0f)
		{
			arrow->height /= 5.0f;
		}
		arrow->radius = 0.05f;

		// originPoint����destinationPoint�̊p�x���v�Z
		DirectX::XMVECTOR origin = DirectX::XMLoadFloat3(&stage->wayPoint[edge->originPoint]->position);
		DirectX::XMVECTOR destination = DirectX::XMLoadFloat3(&stage->wayPoint[edge->destinationPoint]->position);
		DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(destination, origin);
		DirectX::XMFLOAT3 v, out;
		DirectX::XMStoreFloat3(&v, vector);
		float f = sqrtf(v.x * v.x + v.z * v.z);
		out.x = 0.0f;
		out.y = atan2f(v.z, -v.x);
		out.z = atan2f(f, v.y);

		arrow->SetAngle(out);
		arrow->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		ArrowManager::Instance().AddArrow(arrow);
	}
}
float SearchAlgorithm::AddAnswerArrow(Stage* stage)
{
	float totalCost = 0.0f;
	// �S�[�����W����C���f�b�N�X�ԍ����擾�B�S�[������`�悷��
	int startIndex = stage->NearWayPointIndex(Start::Instance().GetPosition());
	int startNo = stage->NearWayPointIndex(Goal::Instance().GetPosition());
	int endNo = -1;
	while (endNo != startIndex) {
		endNo = findRoot[startNo];
		if (endNo == -1)break;

		// arrow�ŕ`�悷��ꍇ
		// �T�������G�b�W����ŕ\��
		Arrow* arrow = new Arrow();
		arrow->SetPosition(stage->wayPoint[endNo]->position);

		DirectX::XMVECTOR start, end;
		start = DirectX::XMLoadFloat3(&stage->wayPoint[endNo]->position);
		end = DirectX::XMLoadFloat3(&stage->wayPoint[startNo]->position);
		DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(end, start);
		DirectX::XMVECTOR dist = DirectX::XMVector3Length(DirectX::XMVectorSubtract(end, start));
		DirectX::XMFLOAT3 cost;
		DirectX::XMStoreFloat3(&cost, dist);
		arrow->height = cost.x;
		arrow->radius = 0.05f;
		arrow->color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		totalCost += cost.x;

		// start����end�̊p�x���v�Z
		DirectX::XMFLOAT3 v, out;
		DirectX::XMStoreFloat3(&v, vector);
		float f = sqrtf(v.x * v.x + v.z * v.z);
		out.x = 0.0f;
		out.y = atan2f(v.z, -v.x);
		out.z = atan2f(f, v.y);

		arrow->SetAngle(out);
		arrow->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		ArrowManager::Instance().AddArrow(arrow);

		startNo = endNo;
	}
	return totalCost;
}
void SearchAlgorithm::SearchRender(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, Stage* stage)
{
	LineRenderer* lineRenderer = Graphics::Instance().GetLineRenderer();

	// �T�[�`�����G�b�W��`��
	for (const auto edge : searchEdge)
	{
		// �T�������G�b�W��`��
		lineRenderer->AddVertex(DirectX::XMFLOAT3(stage->wayPoint[edge->originPoint]->position.x,
			stage->wayPoint[edge->originPoint]->position.y + 2.0f,
			stage->wayPoint[edge->originPoint]->position.z),
			DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));
		lineRenderer->AddVertex(DirectX::XMFLOAT3(stage->wayPoint[edge->destinationPoint]->position.x,
			stage->wayPoint[edge->destinationPoint]->position.y + 2.0f,
			stage->wayPoint[edge->destinationPoint]->position.z),
			DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f));

	}

	lineRenderer->Render(dc, view, projection);

}
void SearchAlgorithm::SearchClear(Stage* stage)
{
	ArrowManager::Instance().Clear();
	// �T���G�b�W���N���A
	searchEdge.clear();

	//�T�[�`�ς݂�FG�����Z�b�g
	for (int i = 0; i < MAX_WAY_POINT; i++) {
		stage->wayPoint[i]->searchFg = false;
		stage->wayPoint[i]->costFromStart = -1.0f;
		findRoot[i] = -1;
	}
}

void SearchAlgorithm::GoldenPathSpawn(Stage* stage, DirectX::XMFLOAT3 startPosition)
{
	// �\�����Ă���G������
	EnemyManager::Instance().Clear();

}