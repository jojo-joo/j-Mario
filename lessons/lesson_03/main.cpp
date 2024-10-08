#include <assert.h>
#include <stdio.h>
#include <string.h>

struct fsdata_file
{
	const struct fsdata_file* next;
	const unsigned char* name;
	const unsigned char* data;
	const int len;
};
#include "fsdata.h"

#define CUTE_TILED_IMPLEMENTATION
#include "cute_tiled.h"

#define CUTE_PNG_IMPLEMENTATION
#include "cute_png.h"

#define SDL_MAIN_HANDLED
#include "sdl.h"

#define GAME_VIEW_WIDTH (256)
#define GAME_VIEW_HEIGHT (256)

SDL_Window* m_win = NULL;
SDL_Renderer* m_ren = NULL;
SDL_Texture* m_tex = NULL;

void tmj_init(void) {
	cute_tiled_map_t* map = cute_tiled_load_map_from_memory(data_mario_tmj + 11, sizeof(data_mario_tmj) - 11, NULL);
	assert(map);
	printf("map\r\n");
	printf("- width: %d\r\n", map->width);
	printf("- height: %d\r\n", map->height);
	printf("- tilewidth: %d\r\n", map->tilewidth);
	printf("- tileheight: %d\r\n", map->tileheight);
	printf("\r\n");

	cute_tiled_layer_t* layer = map->layers;
	assert(layer);
	printf("layers\r\n");
	while (layer) {
		printf("- layer %d: %s\r\n", layer->id, layer->name);
		layer = layer->next;
	}
	printf("\r\n");

	cute_tiled_tileset_t* tilesets = map->tilesets;
	assert(tilesets);
	printf("tilesets\r\n");
	printf("- image: %s\r\n", tilesets->image);
	printf("- firstgid: %d\r\n", tilesets->firstgid);
	printf("- imagewidth: %d\r\n", tilesets->imagewidth);
	printf("- imageheight: %d\r\n", tilesets->imageheight);
	printf("- tilewidth: %d\r\n", tilesets->tilewidth);
	printf("- tileheight: %d\r\n", tilesets->tileheight);
	printf("\r\n");

	cute_tiled_free_map(map);
}

void png_init(void) {
	cp_image_t pngs = cp_load_png("asset/tile_set.png");
}

int main() {
	tmj_init();
	png_init();

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
