#ifndef _SIMULATIONDATA_H
#define _SIMULATIONDATA_H
#include <dinput.h>
#include "Object.h"
#include <vector>

struct SimulationData
{
	unsigned char*			pkeyboard;
	unsigned char*			pPreviousKeyboard;
	DIMOUSESTATE*			pMouse;
	std::vector<Object*>	pObjects;
	//TwBar*					pBar;
	ID3D11Device*			pDevice;
	//Vector2* pScreenDimensions;
};

#endif