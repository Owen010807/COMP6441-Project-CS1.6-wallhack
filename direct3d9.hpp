#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib,"d3dx9.lib")

LPDIRECT3D9	g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;

//Draw line and text
ID3DXLine* G_ID3DXLine = nullptr;
ID3DXFont* G_ID3DXFont = nullptr;

/*
	Initialize d3d device
*/
HRESULT init_d3d(HWND hWnd) {

	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice))) 
	{
		return E_FAIL;
	}

	D3DXCreateLine(g_pd3dDevice, &G_ID3DXLine);
	D3DXCreateFontA(g_pd3dDevice, 20, 0, FW_DONTCARE, D3DX_DEFAULT, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "ËÎÌו", &G_ID3DXFont);
	return S_OK;


}

/*
	clean d3d device
*/
VOID cleanup()
{
	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();

	if (G_ID3DXLine != NULL)
		G_ID3DXLine->Release();

	if (G_ID3DXFont != NULL)
		G_ID3DXFont->Release();
}

/*
	Draws a straight line between two points
*/
void render_line(D3DCOLOR color, float leftx, float topy, float rightx, float downy, float line_size = 1.0f) {
	D3DXVECTOR2 vextor[2]{ {leftx,topy},{rightx,downy} };
	G_ID3DXLine->SetWidth(1);
	G_ID3DXLine->Draw(vextor, 2, color);
}

/*
	Renders text on screen at (x, y) position
*/
void render_text(D3DCOLOR color, long x, long y, const char* text) {
	RECT rect{ x,y };
	G_ID3DXFont->DrawTextA(nullptr, text, -1, &rect, DT_CALCRECT, color);
	G_ID3DXFont->DrawTextA(nullptr, text, -1, &rect, DT_LEFT, color);
}

/*
	Draws a rectangular box given top-left position and width/height
*/
void render_rect(D3DCOLOR color, float x, float y, float width, float height, float rect_size = 1.0f) {
	D3DXVECTOR2 vextor[5]{ {x,y},{x + width,y},{x + width, y + height}, {x, y + height}, {x,y} };
	G_ID3DXLine->SetWidth(1);
	G_ID3DXLine->Draw(vextor, 5, color);
}


