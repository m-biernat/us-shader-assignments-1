#version 330

in vec4 vPosition; // pozycja wierzcholka w lokalnym ukladzie wspolrzednych

out vec4 color; // kolor przekazywany do shadera fragmentow

void main()
{
	if (vPosition.x >= -1.0f && vPosition.x < -0.5f)
	{
		if (vPosition.y <= 1.0f && vPosition.y > 0.5f)
			color = vec4(0.0f, 0.0f, 1.0f, 1.0f); // Region 0.0 - niebieski
		else if (vPosition.y >= -0.5f && vPosition.y <= 0.5f)
			color = vec4(0.0f, 1.0f, 0.0f, 1.0f); // Region 0.1 - zielony
		else if (vPosition.y < -0.5f && vPosition.y >= -1.0f)
			color = vec4(1.0f, 0.0f, 0.0f, 1.0f); // Region 0.2 - czerwony
	}	
	else if (vPosition.x >= -0.5f && vPosition.x <= 0.5f)
	{
		if (vPosition.y <= 1.0f && vPosition.y > 0.5f)
			color = vec4(0.0f, 0.0f, 0.0f, 1.0f); // Region 1.0 - czarny
		else if (vPosition.y >= -0.5f && vPosition.y <= 0.5f)
			color = vec4(0.0f, 1.0f, 1.0f, 1.0f); // Region 1.1 - turkusowy
		else if (vPosition.y < -0.5f && vPosition.y >= -1.0f)
			color = vec4(1.0f, 1.0f, 0.0f, 1.0f); // Region 1.2 - zolty
	}
	else if (vPosition.x > 0.5f && vPosition.x <= 1.0f)
	{
		if (vPosition.y <= 1.0f && vPosition.y > 0.5f)
			color = vec4(1.0f, 0.0f, 1.0f, 1.0f); // Region 2.0 - rozowy
		else if (vPosition.y >= -0.5f && vPosition.y <= 0.5f)
			color = vec4(0.5f, 0.5f, 0.5f, 1.0f); // Region 2.1 - szary
		else if (vPosition.y < -0.5f && vPosition.y >= -1.0f)
			color = vec4(1.0f, 1.0f, 1.0f, 1.0f); // Region 2.2 - bialy
	}

	gl_Position = vPosition;
}