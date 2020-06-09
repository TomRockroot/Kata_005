///////////////
// Filename: cameraclass.cpp
///////////////
#include "cameraclass.h"
#include <Windows.h>


CameraClass::CameraClass()
{
	m_position = glm::vec3(0.0f);
	m_rotation = glm::vec3(0.0f);
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;

	return;
}

void CameraClass::Render()
{
	

	// Finally create the view matrix from the three updated vectors
	BuildViewMatrix(m_position, m_rotation);

	return;
}

void CameraClass::BuildViewMatrix(glm::vec3 position, glm::vec3 rotation)
{
	glm::mat4 result = glm::mat4(1.0f); // identity


	result = glm::translate(result, position);

	result = glm::rotate(result, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	result = glm::rotate(result, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	result = glm::rotate(result, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 inverse = glm::inverse(result);

	m_viewMatrix = inverse;

	return;
}


glm::mat4 CameraClass::GetViewMatrix()
{
	return m_viewMatrix;
}
