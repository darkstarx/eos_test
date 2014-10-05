#include <SDL2/SDL.h>
#include <utils/log.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


// Declare window and renderer
SDL_Window * window;


void init()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Unable to init SDL, error: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	
	static const std::string app_name = "eos_test";
	window = SDL_CreateWindow(
		app_name.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);
	
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	
	if (window == NULL)
	{
		std::cout << "Unable to create window, error: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)WINDOW_WIDTH /(float) WINDOW_HEIGHT, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}


void drawCube(float xrf, float yrf, float zrf)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -7.0f);    // Сдвинуть вглубь экрана
	
	glRotatef(xrf, 1.0f, 0.0f, 0.0f);   // Вращение куба по X, Y, Z
	glRotatef(yrf, 0.0f, 1.0f, 0.0f);   // Вращение куба по X, Y, Z
	glRotatef(zrf, 0.0f, 0.0f, 1.0f);   // Вращение куба по X, Y, Z
	
	glBegin(GL_QUADS);                  // Рисуем куб
	
	glColor3f(0.0f, 1.0f, 0.0f);        // Синяя сторона (Верхняя)
	glVertex3f( 1.0f, 1.0f, -1.0f);     // Верхний правый угол квадрата
	glVertex3f(-1.0f, 1.0f, -1.0f);     // Верхний левый
	glVertex3f(-1.0f, 1.0f,  1.0f);     // Нижний левый
	glVertex3f( 1.0f, 1.0f,  1.0f);     // Нижний правый
	
	glColor3f(1.0f, 0.5f, 0.0f);        // Оранжевая сторона (Нижняя)
	glVertex3f( 1.0f, -1.0f,  1.0f);    // Верхний правый угол квадрата
	glVertex3f(-1.0f, -1.0f,  1.0f);    // Верхний левый
	glVertex3f(-1.0f, -1.0f, -1.0f);    // Нижний левый
	glVertex3f( 1.0f, -1.0f, -1.0f);    // Нижний правый
	
	glColor3f(1.0f, 0.0f, 0.0f);        // Красная сторона (Передняя)
	glVertex3f( 1.0f,  1.0f, 1.0f);     // Верхний правый угол квадрата
	glVertex3f(-1.0f,  1.0f, 1.0f);     // Верхний левый
	glVertex3f(-1.0f, -1.0f, 1.0f);     // Нижний левый
	glVertex3f( 1.0f, -1.0f, 1.0f);     // Нижний правый
	
	glColor3f(1.0f,1.0f,0.0f);          // Желтая сторона (Задняя)
	glVertex3f( 1.0f, -1.0f, -1.0f);    // Верхний правый угол квадрата
	glVertex3f(-1.0f, -1.0f, -1.0f);    // Верхний левый
	glVertex3f(-1.0f,  1.0f, -1.0f);    // Нижний левый
	glVertex3f( 1.0f,  1.0f, -1.0f);    // Нижний правый
	
	glColor3f(0.0f,0.0f,1.0f);          // Синяя сторона (Левая)
	glVertex3f(-1.0f,  1.0f,  1.0f);    // Верхний правый угол квадрата
	glVertex3f(-1.0f,  1.0f, -1.0f);    // Верхний левый
	glVertex3f(-1.0f, -1.0f, -1.0f);    // Нижний левый
	glVertex3f(-1.0f, -1.0f,  1.0f);    // Нижний правый
	
	glColor3f(1.0f,0.0f,1.0f);          // Фиолетовая сторона (Правая)
	glVertex3f( 1.0f,  1.0f, -1.0f);    // Верхний правый угол квадрата
	glVertex3f( 1.0f,  1.0f,  1.0f);    // Верхний левый
	glVertex3f( 1.0f, -1.0f,  1.0f);    // Нижний левый
	glVertex3f( 1.0f, -1.0f, -1.0f);    // Нижний правый
	
	glEnd();                            // Закончили квадраты   
}


int main(int argc, char ** argv)
{
	init();
	
	bool running = true;
	float xrf = 0, yrf = 0, zrf = 0;
	while(running)
	{
		SDL_Event event;
		
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
							break;
					}
					break;
			}
		}
		
		xrf -= 0.5;
		yrf -= 0.5;
		zrf -= 0.5;
		
		drawCube(xrf, yrf, zrf); // рисуем сам куб с текущими углами поворота
		
		glFlush();
		SDL_GL_SwapWindow(window);
	}
	
	SDL_Quit();
	
	return EXIT_SUCCESS;
}
