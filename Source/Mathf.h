#pragma once
#include <DirectXMath.h>
// •‚“®¬”Zp
class Mathf
{
public:
	//üŒ`•âŠ®
	static float Lerp(float a, float b, float t);

	// w’è”ÍˆÍ‚Ìƒ‰ƒ“ƒ_ƒ€’l‚ğfloatŒ^‚ÅŒvZ‚·‚é
	static float RandomRange(float min, float max);

	// 2‚Â‚ÌXMFLOAT3‚ÌŠÔ‚Ì’·‚³‚ğ•Ô‚·ŠÖ”
	static float Pythagoras(DirectX::XMFLOAT3 A,DirectX::XMFLOAT3 B);
};