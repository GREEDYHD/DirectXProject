#pragma once

#ifndef _CAMERA_
#define _CAMERA_
#include "Object.h"

struct DrawData;

class Camera : public Object
{
public:
	Camera(float _FieldOfView, float _AspectRatio, float _NearPlane, float _FarPlane, Vector3 _Target, Vector3 _Up);
	~Camera();

	void Update(SimulationData* SD);

	void Render(DrawData* DD){ DD; };

	Matrix GetProjectionMatrix(){ return m_ProjectionMatrix; }
	Matrix GetViewMatrix(){ return m_ViewMatrix; }

	Vector3 GetTarget(){ return m_Target; }
	Vector3 GetUp(){ return m_Up; }

protected:

	Matrix m_ProjectionMatrix;
	Matrix m_ViewMatrix;

	Vector3 m_Target;
	Vector3 m_Up;

	float m_FieldOfView;
	float m_AspectRatio;
	float m_NearPlane;
	float m_FarPlane;
};
#endif