﻿// Michal Biernat INZ III PGK 1 - Wprowadzenie do Shaderow - Zestaw 1 Zad. 3

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "shaders.h"


const float ANGLE = 45.0f;


constexpr int WIDTH = 600; // szerokosc okna
constexpr int HEIGHT = 600; // wysokosc okna
constexpr int VAOS = 1; // liczba VAO
constexpr int VBOS = 1; // liczba VBO

//******************************************************************************************
GLuint shaderProgram; // identyfikator programu cieniowania

GLuint vertexLoc; // lokalizacja atrybutu wierzcholka - wspolrzedne


GLuint angleLoc;


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

	window = glfwCreateWindow(WIDTH, HEIGHT, "Zadanie3", nullptr, nullptr); // utworzenie okna i zwiazanego z nim kontekstu
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
	if (!setupShaders("shaders/vertex.vert", "shaders/fragment.frag", shaderProgram))
		exit(3);

	vertexLoc = glGetAttribLocation(shaderProgram, "vPosition");
	
	glUseProgram(shaderProgram); // uruchamiam tutaj, zeby dzialalo z uniformem
	angleLoc = glGetUniformLocation(shaderProgram, "angle");
}

/*------------------------------------------------------------------------------------------
** funkcja inicjujaca VAO oraz zawarte w nim VBO z danymi o modelu
**------------------------------------------------------------------------------------------*/
void setupBuffers()
{
	glGenVertexArrays(VAOS, vao); // generowanie identyfikatora VAO
	glGenBuffers(VBOS, buffers); // generowanie identyfikatorow VBO

	// wspolrzedne wierzcholkow
	float vertices[] =
	{
		-0.6f, 0.6f, 0.0f, 1.0f,
		0.6f, 0.6f, 0.0f, 1.0f,
		-0.6f, -0.6f, 0.0f, 1.0f,
		0.6f, -0.6f, 0.0f, 1.0f
	};

	glBindVertexArray(vao[0]); // dowiazanie pierwszego VAO    	

	// VBO dla wspolrzednych wierzcholkow
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc); // wlaczenie tablicy atrybutu wierzcholka - wspolrzedne
	glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, GL_FALSE, 0, 0); // zdefiniowanie danych tablicy atrybutu wierzchoka - wspolrzedne


	glUniform1f(angleLoc, ANGLE * (3.14f / 180));


	glBindVertexArray(0);
}

/*------------------------------------------------------------------------------------------
** funkcja rysujaca scene
**------------------------------------------------------------------------------------------*/
void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT); // czyszczenie bufora koloru

	// wyrysowanie pierwszego VAO
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}