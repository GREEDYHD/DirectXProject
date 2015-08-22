#ifndef _VERTEXBUFFER_
#define _VERTEXBUFFER_
#include "Object.h"
#include "ConstantBuffer.h"
#include "Vertex.h"
//the base Game Object drawn using a Vertex and Index Buffer
//all of the main aspects of drawing it have a default which is pointed to by a static pointer
//this is only used if the version for this object is not set to nullptr
class VertexBuffer :public Object
{
public:
	VertexBuffer();
	virtual ~VertexBuffer();

	virtual void Update(SimulationData* SD);
	virtual void Render(DrawData* DD);

	static void Init(ID3D11Device* SD);
	static void CleanUp();
	static void UpdateConstantBuffer(DrawData* DD);

protected:

	//Direct X information for the model for this GameObject
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	UINT m_numPrimitives;

	D3D_PRIMITIVE_TOPOLOGY m_topology;

	//default vertexshader
	static ID3D11VertexShader*			pVertexShader;
	//default vertex layout
	static ID3D11InputLayout*			pVertexLayout;
	//default pixelshader
	static ID3D11PixelShader*			pPixelShader;
	//default texture (white square)
	static ID3D11ShaderResourceView*	pTextureRV;
	//deafult const buffer
	static ID3D11Buffer*				pConstantBuffer;	//GPU side

	static ConstantBuffer*				pCB;				//CPU side
	//default sampler
	static ID3D11SamplerState*			pSampler;
	//default raster state
	static ID3D11RasterizerState*		pRasterState;

	//my vertexshader
	ID3D11VertexShader*			m_pVertexShader;
	//my vertex layout
	ID3D11InputLayout*			m_pVertexLayout;
	//my pixelshader
	ID3D11PixelShader*			m_pPixelShader;
	//my texture
	ID3D11ShaderResourceView*	m_pTextureRV;
	//my const buffer
	ID3D11Buffer*				m_pConstantBuffer; //GPU side
	void*						m_pCB;				//CPU side
	//my sampler
	ID3D11SamplerState*			m_pSampler;
	//my raster state
	ID3D11RasterizerState*		m_pRasterState;

	//once populated build an Index Buffer
	void BuildIB(ID3D11Device* Device, std::vector<WORD> _Indices);

	//once populated build a Vertex Buffer
	void BuildVB(ID3D11Device* Device, long _Verts, std::vector<Vertex> _Vertices);

	void BuildDynamicVB(ID3D11Device* Device, long _numVerts, std::vector<Vertex> _Vertices);
};

#endif