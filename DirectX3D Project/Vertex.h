//Vertex Structure
#ifndef _VERTEX_
#define _VERTEX_
#include <d3d11_1.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Color Colour;
	Vector2 TextureCoordinates;
};

const D3D11_INPUT_ELEMENT_DESC VertexLayout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

#endif