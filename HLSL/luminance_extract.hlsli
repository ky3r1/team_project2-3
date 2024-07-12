#include "sprite.hlsli"

cbuffer LUMINANCE_EXTRACT : register(b1)
{
    float threshold; // 高輝度抽出のための閾値
    float intensity; // ブルームの強度
    float2 dummy;
};

#include "filter_functions.hlsli"