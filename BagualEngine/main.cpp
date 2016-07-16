#include <SDL2/SDL.h>

#include "Core.h"
#include "Math.h"

using namespace std;

static SDL_Window *window;
static SDL_Surface *surface;
static int width = 800;
static int height = 600;
static int **screen = nullptr;
static bool toQuit = false;

static Line<Pixel> viewportEdges[4];

bool IsLineOnScreen(Line<Pixel> &line)
{
	Box<Pixel> viewport(Pixel(0, 0), Pixel(width-1, height-1));

	bool p1In = viewport.IsIn(line.p1);
	bool p2In = viewport.IsIn(line.p2);

	if (p1In && p2In) return true;

	bool isValid = true;
	bool isIn;
	Pixel inters[4];
	int p1Dist = INT_MAX;
	int p2Dist = INT_MAX;
	int dist;
	Pixel *newP1 = nullptr;
	Pixel *newP2 = nullptr;

	for (int i = 0; i < 4; i++)
	{
		if (LinesIntersection(line, viewportEdges[i], inters[i]))
		{
			isIn = viewport.IsIn(inters[i]);

			if (!p1In && isIn)
			{
				dist = static_cast<int>(line.p1 | inters[i]);

				if (dist < p1Dist)
				{
					p1Dist = dist;
					newP1 = &inters[i];
				}
			}
			
			if (!p2In && isIn)
			{
				dist = static_cast<int>(line.p2 | inters[i]);

				if (dist < p2Dist)
				{
					p2Dist = dist;
					newP2 = &inters[i];
				}
			}
		}
	}

	if (!p1In && newP1) line.p1 = *newP1;
	else isValid = false;

	if (!p2In && newP2) line.p2 = *newP2;
	else isValid = false;

	return isValid;
}

void DrawLine(const Line<Pixel> &line)
{
	float dist;
	Line<Pixel> l(line);
	float step;
	int x, y;
	
	if (IsLineOnScreen(l) == false) return;

	dist = l.p1 | l.p2;
	step = 1.0f / dist;

	for (float a = 0.0f; a <= 1.0f; a += step)
	{
		x = Lerp(l.p1.x, l.p2.x, a);
		y = Lerp(l.p1.y, l.p2.y, a);
		screen[y][x] = 0xFF0000;
	}

}

void DrawLine(const Pixel &p1, const Pixel &p2)
{
	DrawLine(Line<Pixel>(p1, p2));
}

void DrawScreen()
{
	SDL_LockSurface(surface);
	SDL_FillRect(surface, NULL, 0x00000000);
	
	int x1 = rand() % (width + 500) - 250;
	int y1 = rand() % (height + 500) - 250;
	int x2 = rand() % (width + 500) - 250;
	int y2 = rand() % (height + 500) - 250;

	DrawLine(Pixel(x1, y1), Pixel(x2, y2));

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

	viewportEdges[0] = Line<Pixel>(Pixel(0, 0), Pixel(0, height - 1));
	viewportEdges[1] = Line<Pixel>(Pixel(0, height - 1), Pixel(width - 1, height - 1));
	viewportEdges[2] = Line<Pixel>(Pixel(width - 1, height - 1), Pixel(width - 1, 0));
	viewportEdges[3] = Line<Pixel>(Pixel(width - 1, 0), Pixel(0, 0));
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
		SDL_Delay(100);
	}
}

int main(int argc, char** argv)
{
	Init();
	GameLoop();
	Term();

	return 0;
}