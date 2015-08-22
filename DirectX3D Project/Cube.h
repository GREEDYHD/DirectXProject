#ifndef _CUBE_
#define _CUBE_
#include "VertexBuffer.h"
#include "Vertex.h"
#include <vector>

class Cube : public VertexBuffer
{
public:
	Cube(ID3D11Device* Device);
	virtual ~Cube();
	void Update(SimulationData* SD);
	void InitialiseTexture(ID3D11Device* Device, const wchar_t* _Filename);
	void VertexA();
	void CreateVertex(Vector3 Position);


protected:
	std::vector<Vertex> vVertices;
	std::vector<WORD> vIndices;
	ID3D11ShaderResourceView* m_TextureRV;

	ID3D11Device* pDevice;
};

#endif