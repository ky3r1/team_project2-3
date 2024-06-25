#pragma once
#include <DirectXMath.h>
// ���������Z�p
class Mathf
{
public:
	//���`�⊮
	static float Lerp(float a, float b, float t);

	// �w��͈͂̃����_���l��float�^�Ōv�Z����
	static float RandomRange(float min, float max);

	// 2��XMFLOAT3�̊Ԃ̒�����Ԃ��֐�
	static float Pythagoras(DirectX::XMFLOAT3 A,DirectX::XMFLOAT3 B);
};