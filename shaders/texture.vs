///////////////
// Filename: texture.vs
///////////////
#version 400

///////////////
// INPUT VARIABLES
///////////////
in vec3 inputPosition;
in vec2 inputTexCoord;

///////////////
// OUTPUT VARIABLES
///////////////
out vec2 texCoord;

///////////////
// UNIFORM VARIABLES
///////////////
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

////////////////
// Vertex Shader
////////////////
void main(void)
{
	// Calculate the position of the vertex
	gl_Position = worldMatrix * vec4(inputPosition, 1.0f);
	gl_Position = viewMatrix * glPosition;
	gl_Position = projectionMatrix * glPosition;

	// Store the texture coordinates for the pixel shader
	texCoord = inputTexCoord;
}
