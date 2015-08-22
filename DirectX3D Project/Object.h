#ifndef _OBJECT_
#define _OBJECT_
#include "SimpleMath.h"
#include "CommonStates.h"
#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace SimpleMath;

class Camera;
struct SimulationData;
struct DrawData;

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void Update(SimulationData* SD);
	virtual void Render(DrawData* DD) = 0;


	void AddDebugInformation(SimulationData* SD);
	Vector3 GetPosition(){ return m_Position; }
	//Vector2 GetScale(){ return m_Scale; }
	Vector3 GetRotation(){ return m_Rotation; }

	void SetPosition(Vector3 _pos){ m_Position = _pos; }
	void SetRotation(float _X, float _Y, float _Z){ m_Rotation.x = _X; m_Rotation.y = _Y; m_Rotation.z = _Z; }
	void SetScale(Vector3 _scale){ m_Scale = _scale; }
	void SetScale(float _scale){ m_Scale = _scale * Vector3::One; }

protected:
	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;
	Color m_Colour;

	Matrix m_WorldMatrix;
	Matrix m_RotationMatrix;

};

#endif