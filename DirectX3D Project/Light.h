#ifndef _LIGHT_
#define _LIGHT_
#include "Object.h"

struct DrawData;

class Light : public Object
{
public:
	Light(Vector3 _Position, Color _Colour, Color _AmbientColour);
	~Light();

	virtual void Render(DrawData* DD){ DD; }; //draws nothing

	virtual void Update(SimulationData* SD){ m_Position.x++; };

	Color GetColour(){ return m_Colour; }
	Color GetAmbiantColour(){ return m_AmbientColour; }

	void SetColor(Color _Colour){ m_Colour = _Colour; }
	void SetAmbiantColour(Color _Colour){ m_AmbientColour = _Colour; }

protected:

	Color m_AmbientColour;
};

#endif