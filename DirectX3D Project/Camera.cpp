//base camera class
#include "pch.h"
#include "Camera.h"

Camera::Camera(float _FieldOfView, float _AspectRatio, float _NearPlane, float _FarPlane, Vector3 _Target, Vector3 _Up)
{
	m_FieldOfView = _FieldOfView;
	m_AspectRatio = _AspectRatio;
	m_NearPlane = _NearPlane;
	m_FarPlane = _FarPlane;

	m_Target = _Target;
	m_Up = _Up;
}

Camera::~Camera()
{

}

void Camera::Update(SimulationData* SD)
{
	m_ProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(m_FieldOfView, m_AspectRatio, m_NearPlane, m_FarPlane);
	m_ViewMatrix = Matrix::CreateLookAt(m_Position, m_Target, m_Up);
	Object::Update(SD);
}