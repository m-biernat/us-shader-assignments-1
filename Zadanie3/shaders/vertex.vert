#version 330

in vec4 vPosition; // pozycja wierzcholka w lokalnym ukladzie wspolrzednych

uniform float angle;

void main()
{
	vec4 position;

	position.x = vPosition.x * cos(angle) - vPosition.y * sin(angle);
	position.y = vPosition.x * sin(angle) + vPosition.y * cos(angle);
	position.z = vPosition.z;
	position.w = vPosition.w;

	gl_Position = position;
}