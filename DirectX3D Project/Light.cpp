#include "pch.h"
#include "Light.h"

Light::Light(Vector3 _Position, Color _Colour, Color _AmbientColour)
{
	m_Position = _Position;
	m_Colour = _Colour;
	m_AmbientColour = _AmbientColour;
}

Light::~Light()
{

}