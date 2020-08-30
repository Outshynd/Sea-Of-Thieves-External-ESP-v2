#pragma once
#include <windowsx.h>
#include <d3dx9.h>
#include "Cheat.h"
struct sFont
{
	ID3DXFont* Font;
	std::string Name;
};
class cDirectX
{
public:
	int windowWidth = 800;
	int windowHeight = 600;
	int windowX = 300;
	int windowY = 300;
	int frames = 30;
	bool resetLock = false;
	void initD3D(HWND hWnd);
	void cleanD3D();
	void Reset();
	void Render(bool active);
	void initFonts();
	ID3DXFont* findFont(std::string name);
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;
	D3DPRESENT_PARAMETERS d3dpp;
	std::vector<sFont>Fonts;
	ID3DXLine* LinePtr;
	bool addFont(std::string name, std::string font, int size);
};
extern cDirectX* directX;