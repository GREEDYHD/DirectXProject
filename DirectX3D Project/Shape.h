#ifndef _SHAPE_
#define _SHAPE_
#include "VertexBuffer.h"
#include "Vertex.h"

class Shape : public VertexBuffer
{
public:
	Shape(ID3D11Device* Device, int _Sides, Color _Colour);
	virtual ~Shape();
	void Update(SimulationData* SD);
	void CreateVertex(Vector3 Position);

protected:

	std::vector<Vertex> vVertices;
	std::vector<WORD> vIndices;
};

#endif