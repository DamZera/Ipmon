#include <SDL2/SDL.h>

typedef struct // Input event
{
	char key[SDL_NUM_SCANCODES];
	int mousex,mousey;
	int mousexrel,mouseyrel;
	char mousebuttons[8];
	char quit;
} Input;

void UpdateEvents(Input* in);
void InitEvents(Input* in);