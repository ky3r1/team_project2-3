#include "sprite.hlsli"

cbuffer LUMINANCE_EXTRACT : register(b1)
{
    float threshold; // ���P�x���o�̂��߂�臒l
    float intensity; // �u���[���̋��x
    float2 dummy;
};

#include "filter_functions.hlsli"