#version 330

out vec4 color; // wyjsciowy kolor fragmentu

void main()
{
	vec2 screenSize = vec2(600, 600);
	vec2 fragCoord;

	fragCoord.x = gl_FragCoord.x / screenSize.x;
	fragCoord.y = gl_FragCoord.y / screenSize.y;
	
	if (fragCoord.x >= 0.0f && fragCoord.x < 0.33f)
	{
		if (fragCoord.y <= 1.0f && fragCoord.y > 0.66f)
			color = vec4(0.0f, 0.0f, 1.0f, 1.0f); // Region 0.0 - niebieski
		else if (fragCoord.y >= 0.33f && fragCoord.y <= 0.66f)
			color = vec4(0.0f, 1.0f, 0.0f, 1.0f); // Region 0.1 - zielony
		else if (fragCoord.y < 0.33f && fragCoord.y >= 0.0f)
			color = vec4(1.0f, 0.0f, 0.0f, 1.0f); // Region 0.2 - czerwony
	}	
	else if (fragCoord.x >= 0.33f && fragCoord.x <= 0.66f)
	{
		if (fragCoord.y <= 1.0f && fragCoord.y > 0.66f)
			color = vec4(0.0f, 0.0f, 0.0f, 1.0f); // Region 1.0 - czarny
		else if (fragCoord.y >= 0.33f && fragCoord.y <= 0.66f)
			color = vec4(0.0f, 1.0f, 1.0f, 1.0f); // Region 1.1 - turkusowy
		else if (fragCoord.y < 0.33f && fragCoord.y >= 0.0f)
			color = vec4(1.0f, 1.0f, 0.0f, 1.0f); // Region 1.2 - zolty
	}
	else if (fragCoord.x > 0.66f && fragCoord.x <= 1.0f)
	{
		if (fragCoord.y <= 1.0f && fragCoord.y > 0.66f)
			color = vec4(1.0f, 0.0f, 1.0f, 1.0f); // Region 2.0 - rozowy
		else if (fragCoord.y >= 0.33f && fragCoord.y <= 0.66f)
			color = vec4(0.5f, 0.5f, 0.5f, 1.0f); // Region 2.1 - szary
		else if (fragCoord.y < 0.33f && fragCoord.y >= 0.0f)
			color = vec4(1.0f, 1.0f, 1.0f, 1.0f); // Region 2.2 - bialy
	}
}