#include <iostream>
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "direct3d9.hpp"
#include "gamedata.hpp"
#include "worldtoscreen.hpp"

int x, y, width, height;

/*
	Get the target window position and size
*/
void get_window_rect(HWND target, int& x, int& y, int& width, int& height) {
	RECT rect;
	GetWindowRect(target, &rect);
	x = rect.left;
	y = rect.top;
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
}

/*
	Core function to render player boxes
*/
void render_player_box() {
	get_window_rect(g_game_hwnd, g_game_x, g_game_y, g_game_w, g_game_h);
	// Read in-game memory and refresh player list
	init_game_data();
	// Get the local player's team
	int self_camp =		g_game_player[0].camp;
	for (int i = 1; i < g_real_playerCount;i++) {
		if (g_game_player[i].blood>1) {
			D3DCOLOR color = D3DCOLOR_XRGB(0, 255, 0);
			if (g_game_player[i].camp != self_camp) {
				color = D3DCOLOR_XRGB(255, 0, 0);
			}
			vec3 pos;
			pos.x = g_game_player[i].x;
			pos.y = g_game_player[i].y;
			pos.z = g_game_player[i].z;
			int x, y, w, h;
			bool flag = WorldToScreenZ(g_matrix, pos, g_game_w, g_game_h, x, y, w, h);
			if (flag) {
				render_rect(color, x, y, w, h);
				render_line(color, g_game_w / 2, g_game_h, x + w / 2, y + h);
				char display_name[64];
				snprintf(display_name, sizeof(display_name), "%s", g_game_player[i].name);
				render_text(D3DCOLOR_XRGB(255, 255, 255), x, y - 20, display_name);
				char display_hp[64];
				snprintf(display_hp, sizeof(display_hp), "HP: %.0f", g_game_player[i].blood);
				render_text(D3DCOLOR_XRGB(255, 0, 0), x, y - 5, display_hp);
			}
		}
	}

}

/*
	Called every frame to clear screen and draw overlay
*/
VOID render()
{
	if (NULL == g_pd3dDevice)
		return;

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0, 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		render_player_box();

		// End the scene 
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

MARGINS g_MARGINS;
LRESULT _stdcall window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CREATE:
			DwmExtendFrameIntoClientArea(hWnd, &g_MARGINS);
			break;
		case WM_PAINT:
			render();
			break;
		case WM_CLOSE:
			cleanup();
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProcA(hWnd, uMsg, wParam, lParam);
	}
	return 1;
}

/*
	Creates a fully transparent overlay window
*/
HWND creat_transparent_window(HWND game_hwnd) {

	const char* window_name = "walltest";
	WNDCLASSEX window_class;
	ZeroMemory(&window_class, sizeof(window_class));
	// Window size
	window_class.cbSize = sizeof(window_class);
	// Setting the Cursor
	window_class.hCursor = LoadCursor(0, IDC_ARROW);
	// Set the instance handle
	window_class.hInstance = GetModuleHandle(NULL);
	// Set the message processing function
	window_class.lpfnWndProc = window_proc;
	// Window title
	window_class.lpszClassName = window_name;
	// Window style
	window_class.style = CS_VREDRAW | CS_HREDRAW;
	// Background color
	// window_class.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);

	// Register window
	HRESULT result = RegisterClassExA(&window_class);

	get_window_rect(game_hwnd, x, y, width, height);
	// Always on top + mouse click-through + layered window
	HWND transparent_hwnd = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, 
		window_name, window_name, WS_POPUP, x, y, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);
	// Set transparent background color key
	SetLayeredWindowAttributes(transparent_hwnd, 0, RGB(0, 1, 0), LWA_COLORKEY);
	ShowWindow(transparent_hwnd, SW_SHOW);

	UpdateWindow(transparent_hwnd);
	return transparent_hwnd;
}

/*
	Keeps the overlay window synced with the game's position
*/
void message_handle(HWND game_hwnd, HWND transparent_hwnd) {
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		get_window_rect(game_hwnd, x, y, width, height);
		MoveWindow(transparent_hwnd, x, y, width, height, TRUE);
	}
}
