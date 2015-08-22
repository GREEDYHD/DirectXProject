#include "pch.h"
//#include "Sphere.h"
//
//using namespace std;
//
//Sphere::Sphere(ID3D11Device* Device, int _Divisions)
//{
//	vector<Vertex> VertexVector;
//	vector<WORD> IndexVector;
//	int m_radius = 200;
//	int numSubdivisions = _Divisions;
//	m_numPrimitives = 20 * pow(4,numSubdivisions);
//
//	const float X = 0.525731f;
//	const float Z = 0.850651f;
//
//	Vector3 vPositions[12] =
//	{
//		Vector3(-X, 0.0f, Z), Vector3(X, 0.0f, Z),
//		Vector3(-X, 0.0f, -Z), Vector3(X, 0.0f, -Z),
//		Vector3(0.0f, Z, X), Vector3(0.0f, Z, -X),
//		Vector3(0.0f, -Z, X), Vector3(0.0f, -Z, -X),
//		Vector3(Z, X, 0.0f), Vector3(-Z, X, 0.0f),
//		Vector3(Z, -X, 0.0f), Vector3(-Z, -X, 0.0f)
//	};
//
//	DWORD vIndices[60] =
//	{
//		1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4,
//		1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2,
//		3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0,
//		10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
//	};
//
//
//	VertexVector.resize(12);
//	IndexVector.resize(60);
//
//	for (UINT i = 0; i < 12; ++i)
//	{
//		VertexVector[i].Position = vPositions[i];
//	}
//
//	for (UINT i = 0; i < 60; ++i)
//	{
//		IndexVector[i] = vIndices[i];
//	}
//
//	for (UINT i = 0; i < numSubdivisions; ++i)
//	{
//		Subdivide(VertexVector, IndexVector);
//	}
//
//	for (UINT i = 0; i < VertexVector.size(); ++i)
//	{
//		Vector3 n = VertexVector[i].Position;
//		n.Normalize();
//
//		Vector3 p = m_radius * n;
//
//		VertexVector[i].Position = p;
//		VertexVector[i].Normal = n;
//
//		float theta = AngleFromXY(VertexVector[i].Position.x, VertexVector[i].Position.z);
//
//		float phi = acosf(VertexVector[i].Position.y / m_radius);
//
//		VertexVector[i].TextureCoordinates.x = theta / XM_2PI;
//		VertexVector[i].TextureCoordinates.y = phi / XM_PI;
//	}
//
//	Vertex* m_Vertices = new Vertex[VertexVector.size()];
//	WORD* m_Indices = new WORD[IndexVector.size()];
//	
//	int i = 0;
//	for (vector<Vertex>::iterator it = VertexVector.begin(); it != VertexVector.end(); ++it)
//	{
//		m_Vertices[i] = *it;
//		m_Vertices[i++].Colour = Color(1, 1, 1);
//	}
//	i = 0;
//	for (vector<WORD>::iterator it = IndexVector.begin(); it != IndexVector.end(); ++it)
//	{
//		m_Indices[i++] = *it;
//	}
//
//	BuildIB(Device, m_Indices);
//	BuildVB(Device, VertexVector.size(), m_Vertices);
//
//	delete[] m_Vertices;
//}
//
//Sphere::~Sphere()
//{
//
//}
//void Sphere::Update(SimulationData* SD)
//{
//	Object::Update(SD);
//}
////
//float Sphere::AngleFromXY(float x, float y)
//{
//	float theta = 0.0f;
//
//	// Quadrant I or IV
//	if (x >= 0.0f)
//	{
//		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
//		//                atanf(y/x) = -pi/2 if y < 0
//		theta = atanf(y / x); // in [-pi/2, +pi/2]
//
//		if (theta < 0.0f)
//			theta += 2.0f*XM_PI; // in [0, 2*pi).
//	}
//
//	// Quadrant II or III
//	else
//		theta = atanf(y / x) + XM_PI; // in [0, 2*pi).
//
//	return theta;
//}
//
//void Sphere::Subdivide(vector<Vertex> &_VertexVector, vector<WORD> &_IndexVector)
//{
//	// Save a copy of the input geometry.
//	
//	vector<Vertex> VertexVectorCopy = _VertexVector;
//	vector<WORD> IndexVectorCopy = _IndexVector;
//
//	_VertexVector.clear();
//	_IndexVector.clear();
//
//	UINT numTris = IndexVectorCopy.size() / 3;
//	for (UINT i = 0; i < numTris; ++i)
//	{
//		Vertex v0 = VertexVectorCopy[IndexVectorCopy[i * 3 + 0]];
//		Vertex v1 = VertexVectorCopy[IndexVectorCopy[i * 3 + 1]];
//		Vertex v2 = VertexVectorCopy[IndexVectorCopy[i * 3 + 2]];
//
//		Vertex m0, m1, m2;
//
//		// For subdivision, we just care about the position component.  We derive the other
//		// vertex components in CreateGeosphere.
//
//		m0.Position = Vector3(
//			0.5f * (v0.Position.x + v1.Position.x),
//			0.5f * (v0.Position.y + v1.Position.y),
//			0.5f * (v0.Position.z + v1.Position.z));
//
//		m1.Position = Vector3(
//			0.5f * (v1.Position.x + v2.Position.x),
//			0.5f * (v1.Position.y + v2.Position.y),
//			0.5f * (v1.Position.z + v2.Position.z));
//
//		m2.Position = Vector3(
//			0.5f * (v0.Position.x + v2.Position.x),
//			0.5f * (v0.Position.y + v2.Position.y),
//			0.5f * (v0.Position.z + v2.Position.z));
//
//		//
//		// Add new geometry.
//		//
//
//		_VertexVector.push_back(v0); // 0
//		_VertexVector.push_back(v1); // 1
//		_VertexVector.push_back(v2); // 2
//		_VertexVector.push_back(m0); // 3
//		_VertexVector.push_back(m1); // 4
//		_VertexVector.push_back(m2); // 5
//
//		_IndexVector.push_back(i * 6 + 0);
//		_IndexVector.push_back(i * 6 + 3);
//		_IndexVector.push_back(i * 6 + 5);
//
//		_IndexVector.push_back(i * 6 + 3);
//		_IndexVector.push_back(i * 6 + 4);
//		_IndexVector.push_back(i * 6 + 5);
//
//		_IndexVector.push_back(i * 6 + 5);
//		_IndexVector.push_back(i * 6 + 4);
//		_IndexVector.push_back(i * 6 + 2);
//
//		_IndexVector.push_back(i * 6 + 3);
//		_IndexVector.push_back(i * 6 + 1);
//		_IndexVector.push_back(i * 6 + 4);
//	}
//}