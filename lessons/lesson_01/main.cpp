#define SDL_MAIN_HANDLED
#include "sdl.h"

#define GAME_VIEW_WIDTH (256)
#define GAME_VIEW_HEIGHT (256)

SDL_Window* m_win = NULL;
SDL_Renderer* m_ren = NULL;
SDL_Texture* m_tex = NULL;

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	m_win = SDL_CreateWindow("j-Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_VIEW_WIDTH, GAME_VIEW_HEIGHT, SDL_WINDOW_SHOWN);
	m_ren = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	m_tex = SDL_CreateTexture(m_ren, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, GAME_VIEW_WIDTH, GAME_VIEW_HEIGHT);
	SDL_Event event;
	bool is_running = true;

	while (is_running) {
		SDL_SetRenderDrawColor(m_ren, 255, 0, 0, 255);
		SDL_RenderClear(m_ren);
		SDL_Rect rect = { 0, 0, GAME_VIEW_WIDTH, GAME_VIEW_HEIGHT };
		SDL_RenderFillRect(m_ren, &rect);
		SDL_RenderPresent(m_ren);
		SDL_Delay(1000);
		while (SDL_PollEvent(&event)) {
			uint32_t e = 0;
			switch (event.type)
			{
			case SDL_QUIT:
				is_running = false;
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_KEYUP:
				//if (event.key.keysym.sym == 'c') (*(uvgl_signal_t*)&e).type = ULVG_SIGNAL_TYPE_KEY_CLICKED;
				//if (event.key.keysym.sym == '3') (*(uvgl_signal_t*)&e).type = ULVG_SIGNAL_TYPE_REFRESH_DATETIME;
				break;
			}
		}
	}

	SDL_DestroyTexture(m_tex);
	SDL_DestroyRenderer(m_ren);
	SDL_DestroyWindow(m_win);

	return 0;
}
