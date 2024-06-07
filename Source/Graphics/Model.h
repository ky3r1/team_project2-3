#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// ���f��
class Model
{
public:
	Model(const char* filename);
	Model(std::shared_ptr<ModelResource> res);
	~Model() {}

	struct Node
	{
		const char* name = "";
		Node* parent = nullptr;
		DirectX::XMFLOAT3	scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT4	rotate = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3	translate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};

	// �s��v�Z
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);


	// �m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }
	// ���\�[�X�擾
	const ModelResource* GetResource() const { return resource.get(); }
	// ���\�[�X�擾
	std::shared_ptr<ModelResource> GetResourceShare() const { return resource; }
	//�m�[�h����
	Node* FindNode(const char* name);


	// �A�j���[�V�����X�V����
	void UpdateAnimation(float elapsedTime);
	// �A�j���[�V�����Đ�
	void PlayAnimation(int playerNo, bool loop, float blendSeconds = 0.2f);
	// �Đ��A�j���[�V�����ԍ��擾
	int GetPlayAnimationNo() { return currentAnimationIndex; }
	int GetPrevPlayAnimationNo() { return prevAnimationIndex; }
	//���݂̃A�j���[�V�����Đ����Ԏ擾
	float GetCurrentAnimationSeconds() const { return currentAnimationSeconds; }
	// �A�j���[�V�����Đ�����
	bool IsPlayAnimation()const;

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;

	int currentAnimationIndex = -1;
	int prevAnimationIndex = -1;
	float currentAnimationSeconds = 0.0f;
	bool animationLoopFlag = false;
	bool animationEndFlag = false;
	float animationBlendTime = 0.0f;
	float animationBlendSeconds = 0.0f;
};