// Michal Biernat INZ III PGK 1 - Wprowadzenie do Shaderow - Zestaw 1 Zad. 2

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "shaders.h"


// W wariancie A, program cieniujacy ustala kolor wierzcholka ktory lezy w danym regionie
// Informacja o kolorze wierzcholka przekazywana jest do shadera fragmentow, ktory koloruje obiekt
// Kolor jest interpolowany pomiedzy wierzcholkami. [Sa plynne przejscia między kolorami]
// W wariancie B, shader fragmentow sprawdza bezposrednio w jakim regionie znajduje sie piksel
// i na tej podstawie ustala jego kolor. [Tutaj nie ma plynnych przejsc między kolorami]


const char VARIANT = 'A';
//const char VARIANT = 'B';


constexpr int WIDTH = 600; // szerokosc okna
constexpr int HEIGHT = 600; // wysokosc okna
constexpr int VAOS = 4; // liczba VAO
constexpr int VBOS = 4; // liczba VBO

//******************************************************************************************
GLuint shaderProgram; // identyfikator programu cieniowania

GLuint vertexLoc; // lokalizacja atrybutu wierzcholka - wspolrzedne

GLuint vao[VAOS]; // identyfikatory VAO
GLuint buffers[VBOS]; // identyfikatory VBO
//******************************************************************************************

void errorCallback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void onShutdown();
void initGL();
void setupShaders();
void setupBuffers();
void renderScene();

int main(int argc, char* argv[])
{
	atexit(onShutdown);

	GLFWwindow* window;

	glfwSetErrorCallback(errorCallback); // rejestracja funkcji zwrotnej do obslugi bledow

	if (!glfwInit()) // inicjacja biblioteki GLFW
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // inicjacja wersji kontekstu
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // incicjacja profilu rdzennego

	std::string name = "Zadanie2 ";
	name.push_back(VARIANT);

	window = glfwCreateWindow(WIDTH, HEIGHT, name.c_str(), nullptr, nullptr); // utworzenie okna i zwiazanego z nim kontekstu
	if (!window)
	{
		glfwTerminate(); // konczy dzialanie biblioteki GLFW
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, keyCallback); // rejestracja funkcji zwrotnej do oblsugi klawiatury

	glfwMakeContextCurrent(window);

	// inicjacja GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "Blad: " << glewGetErrorString(err) << std::endl;
		exit(1);
	}

	if (!GLEW_VERSION_3_3)
	{
		std::cerr << "Brak obslugi OpenGL 3.3\n";
		exit(2);
	}

	glfwSwapInterval(1); // v-sync on

	initGL();

	// glowna petla programu
	while (!glfwWindowShouldClose(window))
	{
		renderScene();

		glfwSwapBuffers(window); // zamieniamy bufory
		glfwPollEvents(); // przetwarzanie zdarzen
	}

	glfwDestroyWindow(window); // niszczy okno i jego kontekst
	glfwTerminate();
	exit(EXIT_SUCCESS);
	
	return 0;
}

/*------------------------------------------------------------------------------------------
** funkcja zwrotna do obslugi bledow biblioteki GLFW
** error - kod bledu
** description - opis bledu
**------------------------------------------------------------------------------------------*/
void errorCallback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

/*------------------------------------------------------------------------------------------
** funkcja zwrotna do obslugi klawiatury
** window - okno, kt�re otrzymalo zdarzenie
** key - klawisz jaki zostal nacisniety lub zwolniony
** scancode - scancode klawisza specyficzny dla systemu
** action - zachowanie klawisza (GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT)
** mods - pole bitowe zawierajace informacje o nacisnietych modyfikatorach (GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, GLFW_MOD_ALT, GLFW_MOD_SUPER)
**------------------------------------------------------------------------------------------*/
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

/*------------------------------------------------------------------------------------------
** funkcja wykonywana przed zamknieciem programu
**------------------------------------------------------------------------------------------*/
void onShutdown()
{
	glDeleteBuffers(VBOS, buffers);  // usuniecie VBO
	glDeleteVertexArrays(VAOS, vao); // usuiecie VAO
	glDeleteProgram(shaderProgram); // usuniecie programu cieniowania
}

/*------------------------------------------------------------------------------------------
** funkcja inicjujaca ustawienia OpenGL
**------------------------------------------------------------------------------------------*/
void initGL()
{
	std::cout << "GLEW = " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "GL_VENDOR = " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GL_RENDERER = " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GL_VERSION = " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL = " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // kolor uzywany do czyszczenia bufora koloru

	setupShaders();

	setupBuffers();
}

/*------------------------------------------------------------------------------------------
** funkcja tworzaca program cieniowania skladajacy sie z shadera wierzcholkow i fragmentow
**------------------------------------------------------------------------------------------*/
void setupShaders()
{
	if (VARIANT == 'A')
	{
		if (!setupShaders("shaders/vertex_a.vert", "shaders/fragment_a.frag", shaderProgram))
			exit(3);
	}

	else if (VARIANT == 'B')
	{
		if (!setupShaders("shaders/vertex_b.vert", "shaders/fragment_b.frag", shaderProgram))
			exit(3);
	}

	vertexLoc = glGetAttribLocation(shaderProgram, "vPosition");
}

/*------------------------------------------------------------------------------------------
** funkcja inicjujaca VAO oraz zawarte w nim VBO z danymi o modelu
**------------------------------------------------------------------------------------------*/
void setupBuffers()
{
	glGenVertexArrays(VAOS, vao); // generowanie identyfikatora VAO
	glGenBuffers(VBOS, buffers); // generowanie identyfikatorow VBO

	// wspolrzedne wierzcholkow
	float vertices[4][16] =
	{
		{ -0.9f, 0.9f, 0.0f, 1.0f, -0.05f, 0.9f, 0.0f, 1.0f, -0.9f, 0.05f, 0.0f, 1.0f, -0.05f, 0.05f, 0.0f, 1.0f },
		{ 0.05f, 0.9f, 0.0f, 1.0f, 0.9f, 0.9f, 0.0f, 1.0f, 0.05f, 0.05f, 0.0f, 1.0f, 0.9f, 0.05f, 0.0f, 1.0f },
		{ -0.9f, -0.05f, 0.0f, 1.0f, -0.05f, -0.05f, 0.0f, 1.0f, -0.9f, -0.9f, 0.0f, 1.0f, -0.05f, -0.9f, 0.0f, 1.0f },
		{ 0.05f, -0.05f, 0.0f, 1.0f, 0.9f, -0.05f, 0.0f, 1.0f, 0.05f, -0.9f, 0.0f, 1.0f, 0.9f, -0.9f, 0.0f, 1.0f }
	};

	for (int i = 0; i < 4; i++)
	{
		glBindVertexArray(vao[i]); // dowiazanie i-tego VAO    	

		// VBO dla wspolrzednych wierzcholkow
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
		glEnableVertexAttribArray(vertexLoc); // wlaczenie tablicy atrybutu wierzcholka - wspolrzedne
		glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, GL_FALSE, 0, 0); // zdefiniowanie danych tablicy atrybutu wierzchoka - wspolrzedne
	}

	glBindVertexArray(0);
}

/*------------------------------------------------------------------------------------------
** funkcja rysujaca scene
**------------------------------------------------------------------------------------------*/
void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT); // czyszczenie bufora koloru

	glUseProgram(shaderProgram); // wlaczenie programu cieniowania

	for (int i = 0; i < 4; i++)
	{
		glBindVertexArray(vao[i]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glBindVertexArray(0);
}