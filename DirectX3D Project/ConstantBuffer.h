//constant buffer for the shader
#ifndef _CONSTANTBUFFER_
#define _CONSTANTBUFFER_

#include <windows.h>
#include <d3d11.h>
#include <SimpleMath.h>

using namespace DirectX;
using namespace SimpleMath;

//as passing to GPU needs to be correctly memory aligned
__declspec(align(16))
struct ConstantBuffer
{
	Matrix	mWorld;
	Matrix	mView;
	Matrix	mProjection;
	Matrix	mRotation;
	Color	mLightColour;
	Color	mAmbientColour;
	Vector3 mLightPosition;
};

#endif