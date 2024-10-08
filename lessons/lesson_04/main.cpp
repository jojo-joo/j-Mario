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

SDL_Window* m_win = NULL;
SDL_Renderer* m_ren = NULL;
SDL_Texture* m_tex = NULL;
cp_image_t m_png;
cute_tiled_map_t *m_map = NULL;

void tmj_init(void) {
	m_map = cute_tiled_load_map_from_memory(data_mario_tmj + 11, sizeof(data_mario_tmj) - 11, NULL);
	assert(m_map);
	printf("map\r\n");
	printf("- width: %d\r\n", m_map->width);
	printf("- height: %d\r\n", m_map->height);
	printf("- tilewidth: %d\r\n", m_map->tilewidth);
	printf("- tileheight: %d\r\n", m_map->tileheight);
	printf("\r\n");

	cute_tiled_layer_t* layer = m_map->layers;
	assert(layer);
	printf("layers\r\n");
	while (layer) {
		printf("- layer %d: %s\r\n", layer->id, layer->name);
		layer = layer->next;
	}
	printf("\r\n");

	cute_tiled_tileset_t* tilesets = m_map->tilesets;
	assert(tilesets);
	printf("tilesets\r\n");
	printf("- image: %s\r\n", tilesets->image);
	printf("- firstgid: %d\r\n", tilesets->firstgid);
	printf("- imagewidth: %d\r\n", tilesets->imagewidth);
	printf("- imageheight: %d\r\n", tilesets->imageheight);
	printf("- tilewidth: %d\r\n", tilesets->tilewidth);
	printf("- tileheight: %d\r\n", tilesets->tileheight);
	printf("\r\n");

	return;
}

void png_init(void) {
	m_png = cp_load_png("asset/tile_set.png");
}

int main() {
	tmj_init();
	png_init();

	//int width = m_map->width * m_map->tilewidth;
	//int height = m_map->height * m_map->tileheight;
	int width = m_map->tilesets->imagewidth;
	int height = m_map->tilesets->imageheight;

	SDL_Init(SDL_INIT_EVERYTHING);
	m_win = SDL_CreateWindow("j-Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	m_ren = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	m_tex = SDL_CreateTexture(m_ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
	SDL_Event event;
	bool is_running = true;

	while (is_running) {
		SDL_UpdateTexture(m_tex, NULL, m_png.pix, width * sizeof(Uint32));

		SDL_RenderClear(m_ren);

		SDL_RenderCopy(m_ren, m_tex, NULL, NULL);

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
	cute_tiled_free_map(m_map);

	return 0;
}
