#include "Core.h"
#include "Math.h"
#include <SDL2/SDL.h>

using namespace std;

static SDL_Window *window;
static SDL_Surface *surface;
static int width = 800;
static int height = 600;
static int **screen = nullptr;
static bool toQuit = false;

bool EvaluateLineToDraw(Line<Pixel> &line)
{
	bool isValid = true;
	Pixel inters[4];

	

	//inters[0] = LinesIntersection(line, Line<Pixel>(Pixel(0, 0), Pixel(0, height)));

	return isValid;
}

void DrawLine(const Line<Pixel> &line)
{
	float dist;
	Line<Pixel> l(line);
	float step;
	int x, y;
	
	bool onScreen = EvaluateLineToDraw(l);

	if (!onScreen) return;

	dist = l.p1 | l.p2;
	step = 1.0f / dist;

	for (float a = 0.0f; a <= 1.0f; a += step)
	{
		x = Lerp(l.p1.x, l.p2.x, a);
		y = Lerp(l.p1.y, l.p2.y, a);
		screen[x][y] = 0xFF0000;
	}

}

void DrawLine(const Pixel &p1, const Pixel &p2)
{
	DrawLine(Line<Pixel>(p1, p2));
}

void DrawScreen()
{
	SDL_LockSurface(surface);
	
	DrawLine(Pixel(50, 50), Pixel(100, 250));
	DrawLine(Pixel(200, 300), Pixel(200, 350));
	DrawLine(Pixel(300, 400), Pixel(350, 200));
	DrawLine(Pixel(400, 50), Pixel(20, 500));

	SDL_UnlockSurface(surface);
	SDL_UpdateWindowSurface(window);
}

void SetupScreen()
{
	int *pixels = (int*)surface->pixels;
	screen = new int*[height];

	for (int i = 0; i < height; i++)
	{
		screen[i] = &pixels[i * width];
	}
}

void Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Bagual Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_SHOWN);

	surface = SDL_GetWindowSurface(window);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);

	SetupScreen();
}

void Term()
{
	SDL_Quit();
}

void ProcessInput()
{
	SDL_Event ev;

	while (SDL_PollEvent(&ev))
	{
		if (SDL_KEYDOWN || SDL_KEYUP)
		{
			switch (ev.key.keysym.sym)
			{
			case 'q':
				toQuit = true;
			default: break;
			}
		}
	}
}

void GameLoop()
{
	while (!toQuit)
	{
		ProcessInput();
		DrawScreen();
		SDL_Delay(10);
	}
}

int main(int argc, char** argv)
{
	Init();
	GameLoop();
	Term();

	return 0;
}