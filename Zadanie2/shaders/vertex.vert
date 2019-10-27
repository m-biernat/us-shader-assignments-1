#version 330

in vec4 vPosition; // pozycja wierzcholka w lokalnym ukladzie wspolrzednych

out vec4 color; // kolor przekazywany do shadera fragmentow

int x, y;

void main()
{
	if (vPosition.x >= -1.0f && vPosition.x < -0.5f)		x = 0;
	else if (vPosition.x >= -0.5f && vPosition.x <= 0.5f)	x = 1;
	else if (vPosition.x > 0.5f && vPosition.x <= 1.0f)		x = 2;

	if (vPosition.y >= -1.0f && vPosition.y < -0.5f)		y = 0;
	else if (vPosition.y >= -0.5f && vPosition.y <= 0.5f)	y = 1;
	else if (vPosition.y > 0.5f && vPosition.y <= 1.0f)		y = 2;

	gl_Position = vPosition;
}