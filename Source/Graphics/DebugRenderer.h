#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

class DebugRenderer
{
public:
	DebugRenderer(ID3D11Device* device);
	~DebugRenderer() {}

public:
	// �`����s
	void Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// ���`��
	void DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color);

	// �~���`��
	void DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);

	void DrawCube(const DirectX::XMFLOAT3& start_position, const DirectX::XMFLOAT3& end_position, const DirectX::XMFLOAT4& color);

private:
	// �����b�V���쐬
	void CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks);

	// �~�����b�V���쐬
	void CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks);

	void CreateCubeMesh(ID3D11Device* device);

private:
	struct CbMesh
	{
		DirectX::XMFLOAT4X4	wvp;
		DirectX::XMFLOAT4	color;
	};

	struct Sphere
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	center;
		float				radius;
	};

	struct Cylinder
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		float				radius;
		float				height;
	};

	struct Cube
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 scale;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			sphereVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cylinderVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cubeVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cubeIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	std::vector<Sphere>		spheres;
	std::vector<Cylinder>	cylinders;
	std::vector<Cube>		cubes;

	UINT	sphereVertexCount = 0;
	UINT	cylinderVertexCount = 0;
	UINT	cubeVertexCount = 0;
	UINT	ByteWidth = 0;
};
