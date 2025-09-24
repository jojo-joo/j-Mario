#include <assert.h>
#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include <windows.h>

#define APPNAME "HELLO_WIN"

char        szAppName[] = APPNAME; // The name of this application
char        szTitle[]   = APPNAME; // The title bar text
const char *pWindowText;

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

#define GAME_VIEW_WIDTH (256)
#define GAME_VIEW_HEIGHT (256)

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

void CenterWindow(HWND hwnd_self) {
    HWND hwnd_parent;
    RECT rw_self, rc_parent, rw_parent;
    int  xpos, ypos;

    hwnd_parent = GetParent(hwnd_self);
    if (NULL == hwnd_parent)
        hwnd_parent = GetDesktopWindow();

    GetWindowRect(hwnd_parent, &rw_parent);
    GetClientRect(hwnd_parent, &rc_parent);
    GetWindowRect(hwnd_self, &rw_self);

    xpos = rw_parent.left + (rc_parent.right + rw_self.left - rw_self.right) / 2;
    ypos = rw_parent.top + (rc_parent.bottom + rw_self.top - rw_self.bottom) / 2;

    SetWindowPos(hwnd_self, NULL, xpos, ypos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {

    case WM_CREATE:
        CenterWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_RBUTTONUP:
        DestroyWindow(hwnd);
        break;

    case WM_KEYDOWN:
        if (VK_ESCAPE == wParam)
            DestroyWindow(hwnd);
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC         hdc;
        RECT        rc;
        hdc = BeginPaint(hwnd, &ps);

        GetClientRect(hwnd, &rc);
        SetTextColor(hdc, 0x00FFFFFF);
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, pWindowText, -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        EndPaint(hwnd, &ps);
        break;
    }

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stderr);

	tmj_init();
	png_init();

    MSG      msg;
    WNDCLASS wc;
    HWND     hwnd;

    pWindowText = lpCmdLine[0] ? lpCmdLine : "Hello j-mario!";

    ZeroMemory(&wc, sizeof wc);
    wc.hInstance     = hInstance;
    wc.lpszClassName = szAppName;
    wc.lpfnWndProc   = (WNDPROC)WndProc;
    wc.style         = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    if (FALSE == RegisterClass(&wc))
        return 0;

    hwnd = CreateWindow(szAppName, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, GAME_VIEW_WIDTH, GAME_VIEW_HEIGHT, 0, 0, hInstance, 0);
    if (NULL == hwnd)
        return 0;

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	FreeConsole();
    return msg.wParam;
}
