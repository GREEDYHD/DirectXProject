#include "pch.h"
#include "Shape.h"

void Shape::CreateVertex(Vector3 Position)
{
	Vertex V;
	V.Position = Position;
	V.Colour = Color(0.1, 0.1, 1.0);
	V.TextureCoordinates = Vector2::One;
	V.Normal = Vector3::Zero;
	vVertices.push_back(V);
}

Shape::Shape(ID3D11Device* Device, int _Sides, Color _Colour)
{
	m_Rotation = Vector3(0, 0, (XM_PI / _Sides));
	m_Colour = _Colour;
	int sides = _Sides;
	int numVerts = 3 * sides;
	int radius = 100;
	m_numPrimitives = numVerts / 3;
	double degrees = (2 * XM_PI) / m_numPrimitives;

	int vert = 0;

	for (int i = 0; i < numVerts; i++)
	{
		vIndices.push_back(i);
	}

	for (int i = 0; i < m_numPrimitives; i++)
	{
		CreateVertex(Vector3::Zero);
		CreateVertex(Vector3(sin((i + 1) * degrees) * radius, cos((i + 1) * degrees) * radius, 0.0f));
		CreateVertex(Vector3(sin(i * degrees) * radius, cos(i * degrees) * radius, 0.0f));
	}

	BuildIB(Device, vIndices);
	BuildVB(Device, numVerts, vVertices);
}

Shape::~Shape()
{

}
void Shape::Update(SimulationData* SD)
{
	Object::Update(SD);
}