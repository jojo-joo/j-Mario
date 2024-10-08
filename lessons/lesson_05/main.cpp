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
cp_image_t m_tilesets;
cp_image_t m_framebuffer;
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
	m_tilesets = cp_load_png("asset/tile_set.png");
}

int get_tile_id(cute_tiled_layer_t *layer, int x, int y) {
	x /= m_map->tilewidth;
	y /= m_map->tileheight;
	int index = y * m_map->width + x;
	if (index < layer->data_count) {
		return layer->data[index];
	}
	
	return 0;
}

void copy_tile_to_map_from_png(cp_image_t* p_dst_img, cp_image_t* p_src_img, int x, int y, int tile_id) {
	int tiles_in_row = p_src_img->w >> 4;
	int tile_y = tile_id / tiles_in_row;
	int tile_x = tile_id % tiles_in_row;
	uint32_t* p_dst = (uint32_t*)p_dst_img->pix + y * p_dst_img->w + x;
	uint32_t* p_src = (uint32_t*)p_src_img->pix + (tile_y << 4) * p_src_img->w + (tile_x << 4);
	for (int j = 0; j < 16; j++) {
		memcpy(p_dst, p_src, 16 * sizeof(uint32_t));
		p_dst += p_dst_img->w;
		p_src += p_src_img->w;
	}
}

void disp_layer(int layer_id) {
	cute_tiled_layer_t* layer = m_map->layers;
	while (layer) {
		if (layer_id == layer->id) {
			break;
		}

		layer = layer->next;
	}

	if (NULL == layer) {
		printf("background layer not found!\r\n");
		return;
	}

	for (int y = 0; y < m_framebuffer.h; y += m_map->tileheight) {
		for (int x = 0; x < m_framebuffer.w; x += m_map->tilewidth) {
			int tile_gid = get_tile_id(layer, x, y); // layer 3 = background
			if (tile_gid > 0) copy_tile_to_map_from_png(&m_framebuffer, &m_tilesets, x, y, tile_gid - m_map->tilesets->firstgid);
		}
	}

	return;
}

int main() {
	tmj_init();
	png_init();

	int width = m_map->width * m_map->tilewidth;
	int height = m_map->height * m_map->tileheight;
	m_framebuffer.w = width;
	m_framebuffer.h = height;
	m_framebuffer.pix = (cp_pixel_t*)malloc(width*height*sizeof(uint32_t));
	memset(m_framebuffer.pix, 0, width * height * sizeof(uint32_t));

	//int width = m_map->tilesets->imagewidth;
	//int height = m_map->tilesets->imageheight;

	SDL_Init(SDL_INIT_EVERYTHING);
	m_win = SDL_CreateWindow("j-Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	m_ren = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	m_tex = SDL_CreateTexture(m_ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
	SDL_Event event;
	bool is_running = true;

	while (is_running) {
		disp_layer(3);
		disp_layer(1);
		SDL_UpdateTexture(m_tex, NULL, m_framebuffer.pix, width * sizeof(Uint32));

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
