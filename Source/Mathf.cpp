#include <stdlib.h>
#include "Mathf.h"

//線形補完
float Mathf::Lerp(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

// 指定範囲のランダム値をfloat型で計算する
float Mathf::RandomRange(float min, float max)
{
	// 0.0～1.0の間までのランダム値
	float value = static_cast<float>(rand()) / RAND_MAX;

	// min～maxまでのランダム値に変換
	return min + (max - min) * value;
}

float Mathf::Pythagoras(DirectX::XMFLOAT3 A, DirectX::XMFLOAT3 B)
{
    DirectX::XMVECTOR AV = DirectX::XMLoadFloat3(&A);
    DirectX::XMVECTOR BV = DirectX::XMLoadFloat3(&B);
    DirectX::XMVECTOR Dir = {};
    Dir = DirectX::XMVectorSubtract(AV, BV);
    Dir = DirectX::XMVector3Length(Dir);
    return DirectX::XMVectorGetX(Dir);
}
