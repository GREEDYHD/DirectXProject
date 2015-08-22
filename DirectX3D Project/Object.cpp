#pragma once

#include "pch.h"
#include "Object.h"

Object::Object()
{
	m_Position = Vector3::Zero;
	m_Rotation = Vector3::Zero;
	m_Scale = Vector3::One;
	m_Colour = Color(1.0f, 1.0f, 1.0f);

	m_WorldMatrix = Matrix::Identity;

}

Object::~Object()
{

}

void Object::AddDebugInformation(SimulationData* SD)
{
	
}

void Object::Update(SimulationData* SD)
{
	Matrix m_ScaleMatrix = Matrix::CreateScale(m_Scale);
	Matrix m_TranslationMatrix = Matrix::CreateTranslation(m_Position);

	m_RotationMatrix = Matrix::CreateFromYawPitchRoll(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_WorldMatrix = m_ScaleMatrix * m_RotationMatrix * m_TranslationMatrix;
}