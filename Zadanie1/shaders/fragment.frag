#version 330

uniform vec4 fColor;

out vec4 color; // wyjsciowy kolor fragmentu

void main()
{
	color = fColor;
}