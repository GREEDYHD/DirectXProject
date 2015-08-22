#ifndef _DRAWDATA_
#define _DRAWDATA_

#include "Effects.h"
#include "CommonStates.h"

class Camera;
class Light;

struct DrawData
{
	ID3D11DeviceContext* pContext;
	CommonStates* pStates;
	Camera* pCam;
	Light* pLight;
};
#endif