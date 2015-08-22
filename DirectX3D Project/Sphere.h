#ifndef _SPHERE_H
#define _SPHERE_H
#include "VertexBuffer.h"
#include "Vertex.h"
#include <vector>
class Sphere : public VertexBuffer
{
public:
	Sphere(ID3D11Device* Device, int _Divisions);
	virtual ~Sphere();
	void Update(SimulationData* SD);
	float AngleFromXY(float x, float y);
	void Subdivide(std::vector<Vertex> &_VertexVector, std::vector<WORD> &_IndexVector);

protected:

};

#endif