#include "pch.h"
#include <Windows.h>
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"
#include "VertexBuffer.h"
#include "DrawData.h"
#include "Vertex.h"
#include "Camera.h"
#include "Light.h"
#include "d3dx11effect.h"


//the base Game Object drawn using a Vertex and Index Buffer
//all of the main aspects of drawing it have a default which is pointed to by a static pointer
//this is only used if the version for this object is not set to nullptr

using namespace DirectX;

//default vertexshader
ID3D11VertexShader*			VertexBuffer::pVertexShader = nullptr;
//default vertex layout
ID3D11InputLayout*			VertexBuffer::pVertexLayout = nullptr;
//default pixelshader
ID3D11PixelShader*			VertexBuffer::pPixelShader = nullptr;
//default texture (white square)
ID3D11ShaderResourceView*	VertexBuffer::pTextureRV = nullptr;
//deafult const buffer
//GPU side
ID3D11Buffer*				VertexBuffer::pConstantBuffer = nullptr;
//CPU side	
ConstantBuffer*				VertexBuffer::pCB = nullptr;
//default sampler
ID3D11SamplerState*			VertexBuffer::pSampler = nullptr;
//default raster state
ID3D11RasterizerState*		VertexBuffer::pRasterState = nullptr;

VertexBuffer::VertexBuffer()
{
	//set up Buffers
	m_VertexBuffer = NULL;
	m_IndexBuffer = NULL;
	m_numPrimitives = 0;

	m_pVertexShader = nullptr;
	m_pVertexLayout = nullptr;
	m_pPixelShader = nullptr;
	m_pTextureRV = nullptr;
	m_pConstantBuffer = nullptr;
	m_pCB = nullptr;

	m_pSampler = nullptr;
	m_pRasterState = nullptr;

	m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

VertexBuffer::~VertexBuffer()
{
	//BUT WE DO TIDY THEM AWAY
	if (m_VertexBuffer) m_VertexBuffer->Release();
	if (m_IndexBuffer) m_IndexBuffer->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	if (m_pTextureRV) m_pTextureRV->Release();
	if (m_pConstantBuffer) m_pConstantBuffer->Release();
	//if (m_pCB) delete m_pCB; delete this where created as there will know its type
	if (m_pSampler) m_pSampler->Release();
	if (m_pRasterState) m_pRasterState->Release();
}

void VertexBuffer::Update(SimulationData* SD)
{
	Object::Update(SD);
}

void VertexBuffer::Render(DrawData* DD)
{
	//set raster state
	ID3D11RasterizerState* useRasterS = m_pRasterState ? m_pRasterState : pRasterState;
	DD->pContext->RSSetState(useRasterS);

	//set standard Constant Buffer to match this object
	pCB->mWorld = m_WorldMatrix.Transpose();
	pCB->mRotation = m_RotationMatrix.Transpose();

	//Set vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	DD->pContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// Set index buffer
	DD->pContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//update the Constant Buffer in classes that inherit from this then push to the GPU here
	ID3D11Buffer* useCB = m_pConstantBuffer ? m_pConstantBuffer : pConstantBuffer;
	void* useCBData = m_pCB ? m_pCB : pCB;

	DD->pContext->UpdateSubresource(useCB, 0, NULL, useCBData, 0, 0);
	DD->pContext->VSSetConstantBuffers(0, 1, &useCB);
	DD->pContext->PSSetConstantBuffers(0, 1, &useCB);

	//unless it has it own set them to the static defaults

	//set primitive type used
	DD->pContext->IASetPrimitiveTopology(m_topology);

	//set  vertex layout
	//note that if you do use this you'll need to change the stride and offset above
	ID3D11InputLayout* useLayout = m_pVertexLayout ? m_pVertexLayout : pVertexLayout;
	DD->pContext->IASetInputLayout(useLayout);

	//set Vertex Shader
	ID3D11VertexShader* useVS = m_pVertexShader ? m_pVertexShader : pVertexShader;
	DD->pContext->VSSetShader(useVS, NULL, 0);

	//set Pixel Shader
	ID3D11PixelShader* usePS = m_pPixelShader ? m_pPixelShader : pPixelShader;
	DD->pContext->PSSetShader(usePS, NULL, 0);

	//set Texture
	ID3D11ShaderResourceView* useTex = m_pTextureRV ? m_pTextureRV : pTextureRV;
	DD->pContext->PSSetShaderResources(0, 1, &useTex);

	//set sampler
	ID3D11SamplerState* useSample = m_pSampler ? m_pSampler : pSampler;
	DD->pContext->PSSetSamplers(0, 1, &useSample);

	//and draw
	DD->pContext->DrawIndexed(3 * m_numPrimitives, 0, 0);//number here will need to change depending on the primative topology!
}

//--------------------------------------------------------------------------------------
// Compile the  shader
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags = D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}

	return S_OK;
}

void VertexBuffer::Init(ID3D11Device* Device)
{
	//set up all static stuff
	ID3DBlob* pVertexShaderBuffer = nullptr;
	HRESULT hr = CompileShaderFromFile(L"shader.fx", "VS", "vs_5_0", &pVertexShaderBuffer);
	Device->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, &pVertexShader);

	//default pixelshader
	ID3DBlob* pPixelShaderBuffer = NULL;
	hr = CompileShaderFromFile(L"shader.fx", "PS", "ps_5_0", &pPixelShaderBuffer);
	Device->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, &pPixelShader);

	//default vertex layout
	Device->CreateInputLayout(VertexLayout, ARRAYSIZE(VertexLayout), pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), &pVertexLayout);

	//default texture (white square)
	hr = CreateDDSTextureFromFile(Device, L"Default.dds", nullptr, &pTextureRV);

	//deafult const buffer
	//GPU side
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = Device->CreateBuffer(&bd, NULL, &pConstantBuffer);
	//CPU side
	pCB = new ConstantBuffer();
	ZeroMemory(pCB, sizeof(ConstantBuffer));

	//default sampler
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.MipLODBias = 0.0f;
	SamDesc.MaxAnisotropy = 1;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamDesc.BorderColor[0] = SamDesc.BorderColor[1] = SamDesc.BorderColor[2] = SamDesc.BorderColor[3] = 0;
	SamDesc.MinLOD = 0;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = Device->CreateSamplerState(&SamDesc, &pSampler);

	//Setup Raster State
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = Device->CreateRasterizerState(&rasterDesc, &pRasterState);
}

void VertexBuffer::UpdateConstantBuffer(DrawData* DD)
{
	//you'll need your own version of this if you use a different Constant Buffer
	pCB->mView = DD->pCam->GetViewMatrix().Transpose();
	pCB->mProjection = DD->pCam->GetProjectionMatrix().Transpose();
	pCB->mLightColour = DD->pLight->GetColour();
	pCB->mLightPosition = DD->pLight->GetPosition();
	pCB->mAmbientColour = DD->pLight->GetAmbiantColour();
}

void VertexBuffer::CleanUp()
{
	//clear away all the static objects
	if (pVertexShader)		pVertexShader->Release();
	if (pVertexLayout)		pVertexLayout->Release();
	if (pPixelShader)		pPixelShader->Release();
	if (pTextureRV)			pTextureRV->Release();
	if (pConstantBuffer)	pConstantBuffer->Release();
	if (pCB)				delete pCB;
	if (pSampler)			pSampler->Release();
	if (pRasterState)		pRasterState->Release();
}

void VertexBuffer::BuildIB(ID3D11Device* Device, std::vector<WORD> _Indices)
{
	//structures from creating buffers
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = S_OK;

	//build index buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 3 * m_numPrimitives;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = &(_Indices[0]);
	hr = Device->CreateBuffer(&bd, &InitData, &m_IndexBuffer);
}

void VertexBuffer::BuildVB(ID3D11Device* Device, long _numVerts, std::vector<Vertex> _Vertices)
{
	//structures from creating buffers
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = S_OK;

	//build vertex buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * _numVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &(_Vertices[0]);
	hr = Device->CreateBuffer(&bd, &InitData, &m_VertexBuffer);
}

void VertexBuffer::BuildDynamicVB(ID3D11Device* Device, long _numVerts, std::vector<Vertex> _Vertices)
{
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = S_OK;

	//build vertex buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * _numVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &(_Vertices[0]);
	hr = Device->CreateBuffer(&bd, &InitData, &m_VertexBuffer);
}