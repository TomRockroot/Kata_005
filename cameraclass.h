//////////////////
// Filename: cameraclass.h
//////////////////
#ifndef CAMERACLASS_H
#define CAMERACLASS_H

//////////////////
// INCLUDES
//////////////////
#include <math.h>
#include <glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

///////////////////
// Class name: CameraClass
///////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void Render();
	glm::mat4 GetViewMatrix();

private:

	void BuildViewMatrix(glm::vec3 position, glm::vec3 rotation);

private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::mat4 m_viewMatrix;
};

#endif // CAMERACLASS