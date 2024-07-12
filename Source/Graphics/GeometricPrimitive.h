#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

class GeometricPrimitive
{
public:
	GeometricPrimitive(ID3D11Device* device);
	~GeometricPrimitive() {}

public:
	// ï`âÊé¿çs
	void Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	void DrawCube(const DirectX::XMFLOAT3& start_position, const DirectX::XMFLOAT3& end_position, DirectX::XMFLOAT3 rotation, const DirectX::XMFLOAT4& color);

private:

	void CreateCubeMesh(ID3D11Device* device);

private:
	struct CbMesh
	{
		DirectX::XMFLOAT4X4	wvp;
		DirectX::XMFLOAT4	color;
	};


	struct Cube
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 rotation;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			cubeVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cubeIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	std::vector<Cube>		cubes;

	UINT	sphereVertexCount = 0;
	UINT	cylinderVertexCount = 0;
	UINT	cubeVertexCount = 0;
	UINT	ByteWidth = 0;
};
