#include "fullscreen_quad.h"
#include "Shader.h"
#include "misc.h"

fullscreen_quad::fullscreen_quad(ID3D11Device* device)
{
    create_vs_from_cso(device, "Data/Shader/fullscreen_quad_vs.cso", embedded_vertex_shader.ReleaseAndGetAddressOf(),
        nullptr, nullptr, 0);
    create_ps_from_cso(device, "Data/Shader/fullscreen_quad_ps.cso", embedded_pixel_shader.ReleaseAndGetAddressOf());

    // サンプラーステートの作成
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    device->CreateSamplerState(&samplerDesc, &samplerState);
}

void fullscreen_quad::blit(ID3D11DeviceContext* immediate_context, ID3D11ShaderResourceView** shader_resource_view, uint32_t start_slot, uint32_t num_views, ID3D11PixelShader* replace_pixel_shader)
{
    immediate_context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    immediate_context->IASetInputLayout(nullptr);

    immediate_context->VSSetShader(embedded_vertex_shader.Get(), 0, 0);
    replace_pixel_shader ? immediate_context->PSSetShader(replace_pixel_shader, 0, 0) :
        immediate_context->PSSetShader(embedded_pixel_shader.Get(), 0, 0);
    immediate_context->PSSetSamplers(0, 1, &samplerState);
    immediate_context->PSSetSamplers(1, 1, &samplerState);

    immediate_context->PSSetShaderResources(start_slot, num_views, shader_resource_view);

    immediate_context->Draw(4, 0);
}

