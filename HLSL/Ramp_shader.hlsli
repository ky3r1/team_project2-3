struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 world_position : POSITION;
    float4 world_normal : NORMAL;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

#define MAX_BONES 128
cbuffer CbMesh : register(b0)
{
    row_major float4x4 boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b1)
{
    float4 materialColor;
};

cbuffer SCENE_CONSTANT_BUFFER : register(b2)
{
    row_major float4x4 view_projection;
    float4 options;
    float4 camera_position;
};

cbuffer LIGHT_CONSTANT_BUFFER : register(b3)
{
    float4 ambient_color;
    float4 directional_light_direction;
    float4 directional_light_color;
};

#include "shading_functions.hlsli"