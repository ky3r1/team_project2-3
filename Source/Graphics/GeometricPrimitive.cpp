#include "GeometricPrimitive.h"
#include <stdio.h>
#include <memory>
#include "misc.h"

GeometricPrimitive::GeometricPrimitive(ID3D11Device* device)
{
	// 頂点シェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Data\\Shader\\DebugVS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// 頂点シェーダー生成
		HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, vertexShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, inputLayout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// ピクセルシェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Data\\Shader\\DebugPS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// ピクセルシェーダー生成
		HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, pixelShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// 定数バッファ
	{
		// シーン用バッファ
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CbMesh);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// ブレンドステート
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blendState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, depthStencilState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// ラスタライザーステート
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = true;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}


	CreateCubeMesh(device);
}

void GeometricPrimitive::Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// シェーダー設定
	context->VSSetShader(vertexShader.Get(), nullptr, 0);
	context->PSSetShader(pixelShader.Get(), nullptr, 0);
	context->IASetInputLayout(inputLayout.Get());

	// 定数バッファ設定
	context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	//context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	// レンダーステート設定
	const float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetBlendState(blendState.Get(), blendFactor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depthStencilState.Get(), 0);
	context->RSSetState(rasterizerState.Get());

	// ビュープロジェクション行列作成
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX VP = V * P;

	// プリミティブ設定
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	context->IASetVertexBuffers(0, 1, cubeVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(cubeIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (const Cube& cube : cubes)
	{
		// ワールドビュープロジェクション行列作成
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(cube.scale.x, cube.scale.y, cube.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(cube.rotation.x, cube.rotation.y, cube.rotation.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(cube.position.x, cube.position.y, cube.position.z);
		DirectX::XMMATRIX W = S * R * T;
		DirectX::XMMATRIX WVP = W * VP;

		// 定数バッファ更新
		CbMesh cbMesh;
		cbMesh.color = cube.color;
		DirectX::XMStoreFloat4x4(&cbMesh.wvp, WVP);

		context->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbMesh, 0, 0);
		//context->Draw(cubeVertexCount, 0);
		context->DrawIndexed(ByteWidth / sizeof(uint32_t), 0, 0);
	}
	cubes.clear();
}

void GeometricPrimitive::DrawCube(const DirectX::XMFLOAT3& start_position, const DirectX::XMFLOAT3& end_position, DirectX::XMFLOAT3 rotation, const DirectX::XMFLOAT4& color)
{
	Cube cube;
	cube.position = start_position;
	cube.scale = { end_position.x - start_position.x,end_position.y - start_position.y,end_position.z - start_position.z };
	cube.color = color;
	cube.rotation = rotation;
	cubes.emplace_back(cube);
}

void GeometricPrimitive::CreateCubeMesh(ID3D11Device* device)
{
	cubeVertexCount = 24;
	std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(cubeVertexCount);

	DirectX::XMFLOAT3* p = vertices.get();




	//front
	p[0] = { 0.0f,0.0f,0.0f }; //0
	p[1] = { 0.0f,1.0f,0.0f }; //1
	p[2] = { 1.0f,1.0f,0.0f }; //2
	p[3] = { 1.0f,0.0f,0.0f }; //3
	//back
	p[4] = { 1.0f,1.0f,1.0f }; //4
	p[5] = { 0.0f,1.0f,1.0f }; //5
	p[6] = { 0.0f,0.0f,1.0f }; //6
	p[7] = { 1.0f,0.0f,1.0f }; //7
	//top
	p[8] = { 0.0f,1.0f,1.0f }; //8
	p[9] = { 1.0f,1.0f,1.0f }; //9
	p[10] = { 1.0f,1.0f,0.0f }; //10
	p[11] = { 0.0f,1.0f,0.0f }; //11
	//bottom
	p[12] = { 0.0f,0.0f,0.0f }; //12
	p[13] = { 1.0f,0.0f,0.0f }; //13
	p[14] = { 1.0f,0.0f,1.0f }; //14
	p[15] = { 0.0f,0.0f,1.0f }; //15
	//left
	p[16] = { 0.0f,1.0f,1.0f }; //16
	p[17] = { 0.0f,1.0f,0.0f }; //17
	p[18] = { 0.0f,0.0f,0.0f }; //18
	p[19] = { 0.0f,0.0f,1.0f }; //19
	//right
	p[20] = { 1.0f,1.0f,0.0f };  //20
	p[21] = { 1.0f,1.0f,1.0f };  //21
	p[22] = { 1.0f,0.0f,1.0f };  //22
	p[23] = { 1.0f,0.0f,0.0f };  //23


	uint32_t indices[36]{
		//front
		0,1,2,
		0,2,3,
		//back
		7,4,5,
		7,5,6,
		//top
		8,9,10,
		8,10,11,
		//bottom
		12,13,14,
		12,14,15,
		//left
		16,17,18,
		16,18,19,
		//right
		20,21,22,
		20,22,23,
	};


	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * cubeVertexCount);
		desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subresourceData.pSysMem = vertices.get();
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&desc, &subresourceData, cubeVertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * 36);
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subresourceData.pSysMem = indices;
		hr = device->CreateBuffer(&desc, &subresourceData, cubeIndexBuffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
}
