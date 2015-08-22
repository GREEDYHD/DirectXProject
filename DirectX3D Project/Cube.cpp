#include "pch.h"
#include "Cube.h"
#include <string>
#include "Converter.h"
#include <iostream>

Cube::Cube(ID3D11Device* Device)
{
	pDevice = Device;
	VertexA();
}

Cube::~Cube()
{

}

void Cube::CreateVertex(Vector3 Position)
{
	Vertex V;
	V.Position = Position;
	V.Colour = Color(0.1, 0.1, 1.0);
	V.TextureCoordinates = Vector2::One;
	V.Normal = Vector3::Zero;
	vVertices.push_back(V);
}

void Cube::VertexA()
{
	int mDivisions = 42;
	float mDivisionSize = mDivisions / 2;
	int numVerts = 6 * 6 * (mDivisions * mDivisions);
	m_numPrimitives = numVerts / 3;

	for (float i = -mDivisions / 2; i < mDivisions / 2; i++)
	{
		for (float j = -mDivisions / 2; j < mDivisions / 2; j++)
		{
			//Top
			CreateVertex(Vector3(i, mDivisionSize, j));
			CreateVertex(Vector3(i + 1, mDivisionSize, j));
			CreateVertex(Vector3(i, mDivisionSize, j + 1));

			CreateVertex(Vector3(i + 1, mDivisionSize, j));
			CreateVertex(Vector3(i + 1, mDivisionSize, j + 1));
			CreateVertex(Vector3(i, mDivisionSize, j + 1));

			//Bottom
			CreateVertex(Vector3(j, -mDivisionSize, i));
			CreateVertex(Vector3(j, -mDivisionSize, i + 1));
			CreateVertex(Vector3(j + 1, -mDivisionSize, i));

			CreateVertex(Vector3(j, -mDivisionSize, i + 1));
			CreateVertex(Vector3(j + 1, -mDivisionSize, i + 1));
			CreateVertex(Vector3(j + 1, -mDivisionSize, i));

			//Front
			CreateVertex(Vector3(j, i, -mDivisionSize));
			CreateVertex(Vector3(j + 1, i, -mDivisionSize));
			CreateVertex(Vector3(j, i + 1, -mDivisionSize));

			CreateVertex(Vector3(j, i + 1, -mDivisionSize));
			CreateVertex(Vector3(j + 1, i, -mDivisionSize));
			CreateVertex(Vector3(j + 1, i + 1, -mDivisionSize));

			//Back
			CreateVertex(Vector3(i, j, mDivisionSize));
			CreateVertex(Vector3(i, j + 1, mDivisionSize));
			CreateVertex(Vector3(i + 1, j, mDivisionSize));

			CreateVertex(Vector3(i + 1, j, mDivisionSize));
			CreateVertex(Vector3(i, j + 1, mDivisionSize));
			CreateVertex(Vector3(i + 1, j + 1, mDivisionSize));

			//Left
			CreateVertex(Vector3(-mDivisionSize, j, i));
			CreateVertex(Vector3(-mDivisionSize, j + 1, i));
			CreateVertex(Vector3(-mDivisionSize, j, i + 1));

			CreateVertex(Vector3(-mDivisionSize, j, i + 1));
			CreateVertex(Vector3(-mDivisionSize, j + 1, i));
			CreateVertex(Vector3(-mDivisionSize, j + 1, i + 1));

			//Right
			CreateVertex(Vector3(mDivisionSize, i, j));
			CreateVertex(Vector3(mDivisionSize, i, j + 1));
			CreateVertex(Vector3(mDivisionSize, i + 1, j));

			CreateVertex(Vector3(mDivisionSize, i + 1, j));
			CreateVertex(Vector3(mDivisionSize, i, j + 1));
			CreateVertex(Vector3(mDivisionSize, i + 1, j + 1));
		}
	}

	for (UINT i = 0; i < m_numPrimitives * 3; i++)
	{
		Vector3 vertPos = vVertices[i].Position;
		Vector3 spherePos = vVertices[i].Position;

		spherePos.Normalize();

		Vector3 newPos = vertPos + mDivisions * spherePos;

		vVertices[i].Position = mDivisions * spherePos;
	}

	for (UINT i = 0; i < m_numPrimitives; i++)
	{
		Vector3 A = vVertices[3 * i].Position;
		Vector3 B = vVertices[3 * i + 1].Position;
		Vector3 C = vVertices[3 * i + 2].Position;

		Vector3 U, V, N;
	
		U = B - A;
		V = B - C;

		N = U.Cross(V);
		N.Normalize();

		vVertices[(3 * i)].Normal = N;
		vVertices[(3 * i) + 1].Normal = N;
		vVertices[(3 * i) + 2].Normal = N;
	}
	
	for (int i = 0; i < numVerts; i++)
	{
		vIndices.push_back(i);
	}

	BuildIB(pDevice, vIndices);
	BuildVB(pDevice, numVerts, vVertices);
}

void Cube::Update(SimulationData* SD)
{
	Object::Update(SD);
}

void Cube::InitialiseTexture(ID3D11Device* Device, const wchar_t* _Filename)
{
	HRESULT hr = CreateDDSTextureFromFile(Device, _Filename, nullptr, &m_TextureRV);
}
