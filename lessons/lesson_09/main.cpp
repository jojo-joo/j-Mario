#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <list>
#include <memory>
#include <string>

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
cp_image_t m_img_tiled_set;
cp_image_t m_img_map;
cp_image_t m_img_cam;
cute_tiled_map_t* m_tiled_map = NULL;
bool g_leftKeyPressed = false;
bool g_rightKeyPressed = false;
bool g_upKeyPressed = false;
bool g_downKeyPressed = false;


void detectKey(void) {
	g_leftKeyPressed = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT] != 0;
	g_rightKeyPressed = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT] != 0;
	g_upKeyPressed = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP] != 0;
	g_downKeyPressed = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN] != 0;

	return;
}

void blend_tile_to_map_from_png(cp_image_t* p_dst_img, cp_image_t* p_src_img, int x, int y, int tile_id) {
	int tiles_in_row = p_src_img->w >> 4;
	int tile_y = tile_id / tiles_in_row;
	int tile_x = tile_id % tiles_in_row;
	uint32_t* p_dst = (uint32_t*)p_dst_img->pix + y * p_dst_img->w + x;
	uint32_t* p_src = (uint32_t*)p_src_img->pix + (tile_y << 4) * p_src_img->w + (tile_x << 4);
	for (int j = 0; j < 16; j++) {
		//memcpy(p_dst, p_src, 16 * sizeof(uint32_t)); 
		for (int i = 0; i < 16; i++) {
			uint8_t* p_src_rgba = (uint8_t*)(p_src + i);
			uint8_t* p_dst_rgba = (uint8_t*)(p_dst + i);
			float fr = p_src_rgba[0];
			float fg = p_src_rgba[1];
			float fb = p_src_rgba[2];
			float a = p_src_rgba[3] / 255.0f;
			float br = p_dst_rgba[0];
			float bg = p_dst_rgba[1];
			float bb = p_dst_rgba[2];

			br = (1 - a) * br + a * fr;
			bg = (1 - a) * bg + a * fg;
			bb = (1 - a) * bb + a * fb;

			p_dst_rgba[0] = br;
			p_dst_rgba[1] = bg;
			p_dst_rgba[2] = bb;
		}
		p_dst += p_dst_img->w;
		p_src += p_src_img->w;
	}
}

int get_tile_id(cute_tiled_layer_t* layer, int x, int y) {
	x /= m_tiled_map->tilewidth;
	y /= m_tiled_map->tileheight;
	int index = y * m_tiled_map->width + x;
	if (index < layer->data_count) {
		return layer->data[index];
	}
	else {
		throw "tile id error!";
	}

	return 0;
}

void cp_image_fill(cp_image_t* p_img, uint32_t color) {
	uint32_t* p = (uint32_t*)p_img->pix;
	for (int y = 0; y < p_img->h; y++) {
		for (int x = 0; x < p_img->w; x++) {
			*p++ = color;
		}
	}
}

class CRect {
public:
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};

void cp_image_copy(cp_image_t* p_dst, cp_image_t* p_src, CRect &r) {
	uint32_t* s = (uint32_t*)p_src->pix + r.y * p_src->w + r.x;
	uint32_t* d = (uint32_t*)p_dst->pix;

	for (int j = 0; j < r.h; j++) {
		memcpy(d, s, r.w * sizeof(uint32_t));
		s += p_src->w;
		d += p_dst->w;
	}
}

class CCollider {
public:
	static CCollider* factory(const std::string& type);
	virtual void Update() = 0;
	virtual void Render(CRect& rectCamera) = 0;

	CRect m_rect;
};

class CBrick : public CCollider {
public:
	virtual void Update() {}
	virtual void Render(CRect& rectCamera) {}
};
class CWall : public CCollider {
public:
	virtual void Update() {}
	virtual void Render(CRect& rectCamera) {}
};
class CQuestion : public CCollider {
public:
	virtual void Update() {}
	virtual void Render(CRect& rectCamera) {}
};
class CPipe : public CCollider {
public:
	virtual void Update() {}
	virtual void Render(CRect& rectCamera) {}
};

CCollider* CCollider::factory(const std::string& type) {
	if (type == "CBrick") {
		return new CBrick();
	}
	else if (type == "CWall") {
		return new CWall();
	}
	else if (type == "CQuestion") {
		return new CQuestion();
	}
	else if (type == "CPipe") {
		return new CPipe();
	}
	else {
		return nullptr;
	}
}

class CHero : public CCollider {
public:
	CHero() {
		m_img_hero = cp_load_png("asset/mario_bros.png");
	}

	virtual void Update() {
		if (g_leftKeyPressed) {
			if (m_rect.x > 0) {
				m_rect.x -= 2;
			}
		}

		if (g_rightKeyPressed) {
			if (m_rect.x < m_tiled_map->width * m_tiled_map->tilewidth) {
				m_rect.x += 2;
			}
		}

		if (g_upKeyPressed) {
			if (m_rect.y > 0) {
				m_rect.y -= 2;
			}
		}

		if (g_downKeyPressed) {
			if (m_rect.y < m_tiled_map->height * m_tiled_map->tileheight) {
				m_rect.y += 2;
			}
		}
	}


	virtual void Render(CRect& rectCamera) {
		int x = m_rect.x - rectCamera.x;
		int y = m_rect.y - rectCamera.y;
		blend_tile_to_map_from_png(&m_img_cam, &m_img_hero, x, y, 53);
	}

private:
	cp_image_t m_img_hero;
};

class CWorld {
public:
	CWorld(int w, int h) {
		m_camera_rect.w = w;
		m_camera_rect.h = h;
		m_camera_rect.x = 0;
		m_camera_rect.y = 0;

		InitBackgound();
		InitCollider();
	}

	void AddHero(CHero* p) {
		m_pHero = p;
	}

	void Update() {
		detectKey();
		m_pHero->Update();

		for (CCollider* p : m_listColliders) {
			p->Update();
			checkCollision(m_pHero, p);
		}

		int x = m_pHero->m_rect.x;
		int y = m_pHero->m_rect.y;

		if (x < (m_camera_rect.w >> 1)) {
			m_camera_rect.x = 0;
		}
		else if (x > m_tiled_map->width * m_tiled_map->tilewidth - (m_camera_rect.w >> 1)) {
			m_camera_rect.x = m_tiled_map->width * m_tiled_map->tilewidth - (m_camera_rect.w >> 1);
		}
		else {
			m_camera_rect.x = x - (m_camera_rect.w >> 1);
		}

		if (g_leftKeyPressed || g_rightKeyPressed || g_upKeyPressed || g_downKeyPressed) {
			printf("x=%d, y=%d, camerax=%d\r\n", x, y, m_camera_rect.x);
		}
	}

	void Render() {
		cp_image_copy(&m_img_cam, &m_img_map, m_camera_rect);

		for (CCollider* p : m_listColliders) {
			p->Render(m_camera_rect);
		}

		m_pHero->Render(m_camera_rect);

		SDL_UpdateTexture(m_tex, NULL, m_img_cam.pix, m_img_cam.w * sizeof(Uint32));
		SDL_RenderClear(m_ren);
		SDL_RenderCopy(m_ren, m_tex, NULL, NULL);
		SDL_RenderPresent(m_ren);
	}

private:
	CRect m_camera_rect;
	std::list <CCollider*> m_listColliders;
	CHero* m_pHero = NULL;
	bool checkCollision(CHero* pHero, CCollider* p) {

	}
	void InitBackgound() {
		m_tiled_map = cute_tiled_load_map_from_memory(data_mario_tmj + 11, sizeof(data_mario_tmj) - 11, NULL);
		cute_tiled_layer_t* layer = m_tiled_map->layers;
		while (layer) {
			if (layer->id == 3) break;
			layer = layer->next;
		}

		// alloc tile set image
		m_img_tiled_set = cp_load_png("asset/tile_set.png");

		// alloc map image
		m_img_map.w = m_tiled_map->width * m_tiled_map->tilewidth;
		m_img_map.h = m_tiled_map->height * m_tiled_map->tileheight;
		m_img_map.pix = (cp_pixel_t*)malloc(m_img_map.w * m_img_map.h * sizeof(uint32_t));
		cp_image_fill(&m_img_map, (((255) << 16) | ((140) << 8) | (107) | ((255) << 24))); //SDL_PIXELFORMAT_ABGR8888

		for (int y = 0; y < m_img_map.h; y += m_tiled_map->tileheight) {
			for (int x = 0; x < m_img_map.w; x += m_tiled_map->tilewidth) {
				int tile_gid = get_tile_id(layer, x, y); // layer 3 = background
				if (tile_gid > 0) {
					blend_tile_to_map_from_png(&m_img_map, &m_img_tiled_set, x, y, tile_gid - m_tiled_map->tilesets->firstgid);
				}
			}
		}

		// alloc camera image
		m_img_cam.w = m_camera_rect.w;
		m_img_cam.h = m_camera_rect.h;
		m_img_cam.pix = (cp_pixel_t*)malloc(m_img_cam.w * m_img_cam.h * sizeof(uint32_t));
	}

	void InitCollider() {
		cute_tiled_layer_t* layer = m_tiled_map->layers;
		while (layer) {
			if (layer->id == 4) break;
			layer = layer->next;
		}

		cute_tiled_object_t* object = layer->objects;
		while (object) {
			CCollider* p = CCollider::factory(object->type.ptr);
			m_listColliders.push_back(p);
			object = object->next;
		}
	}
};

int main() {
	int width = 256;
	int height = 240;

	CWorld* pWorld = new CWorld(width, height);
	CHero* pMario = new CHero;
	pWorld->AddHero(pMario);

	SDL_Init(SDL_INIT_EVERYTHING);
	m_win = SDL_CreateWindow("j-Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	m_ren = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	m_tex = SDL_CreateTexture(m_ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
	SDL_Event event;
	bool is_running = true;
	Uint64 m_StartTime = SDL_GetPerformanceCounter();
	Uint64 m_PreFrameTime = m_StartTime;
	while (is_running) {
		Uint64 currTime = SDL_GetPerformanceCounter();
		float m_DeltaTime = (currTime - m_PreFrameTime) / (float)SDL_GetPerformanceFrequency();
		m_PreFrameTime = currTime;

		pWorld->Update();
		pWorld->Render();

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

		int MaxFPS = 30;
		if (m_DeltaTime < 1.0f / MaxFPS * 1000.0f) {
			SDL_Delay(Uint32(std::floor(1.0 / MaxFPS * 1000.0 - m_DeltaTime)));
		}
	}

	SDL_DestroyTexture(m_tex);
	SDL_DestroyRenderer(m_ren);
	SDL_DestroyWindow(m_win);
	cute_tiled_free_map(m_tiled_map);

	return 0;
}
