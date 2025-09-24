#include <windows.h>

#define APPNAME "HELLO_WIN"

char        szAppName[] = APPNAME; // The name of this application
char        szTitle[]   = APPNAME; // The title bar text
const char *pWindowText;

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

    hwnd = CreateWindow(szAppName, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 256, 256, 0, 0, hInstance, 0);
    if (NULL == hwnd)
        return 0;

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
