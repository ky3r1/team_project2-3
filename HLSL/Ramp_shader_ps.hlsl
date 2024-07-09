#include "Ramp_shader.hlsli"

Texture2D color_map : register(t0);
SamplerState color_sampler_state : register(s0);
Texture2D ramp_map : register(t2);
SamplerState ramp_sampler_state : register(s2);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 diffuse_color = color_map.Sample(color_sampler_state, pin.texcoord) * pin.color;

    float3 E = normalize(pin.world_position.xyz - camera_position.xyz);
    float3 L = normalize(directional_light_direction.xyz);
    float3 N = normalize(pin.world_normal.xyz);
    
    float3 ka = float3(1, 1, 1);
    float3 kd = float3(1, 1, 1);
    float3 ks = float3(1, 1, 1);

    float3 ambient = ka * ambient_color.rgb;
    float3 directional_diffuse = CalcRampShading(ramp_map, ramp_sampler_state,
                                                         N, L, directional_light_color.rgb, kd);
    float3 directional_sprcular = CalcPhongSpecular(N, L, E, directional_light_color.rgb, ks.rgb);

    float4 color = float4(ambient, diffuse_color.a);
    color.rgb += diffuse_color.rgb * directional_diffuse;
    color.rgb += directional_sprcular;
    //color.rgb *= color.a;
    return color;

}