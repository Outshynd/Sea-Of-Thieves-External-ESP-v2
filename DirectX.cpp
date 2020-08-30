#include "DirectX.h"
cDirectX* directX = new cDirectX();
void cDirectX::initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
	initFonts();
}
void cDirectX::Reset()
{
	if (d3ddev)
	{
		resetLock = true;
		for (int i = 0; i < (Fonts.size() - 1); ++i)
		{
			auto Font = Fonts[i].Font;
			if (Font)
			{
				Font->Release();
				Font = NULL;
			}
		}
		if (LinePtr)
		{
			LinePtr->Release();
			LinePtr = NULL;
		}
		d3dpp.BackBufferWidth = Process->Size[0];
		d3dpp.BackBufferHeight = Process->Size[1];
		d3ddev->Reset(&d3dpp);
		initFonts();
		resetLock = false;
	}
}
void cDirectX::cleanD3D()
{
	d3ddev->Release();
	d3d->Release();
}
bool cDirectX::addFont(std::string name, std::string font, int size)
{
	sFont temp;
	temp.Font = 0;
	temp.Name = "";
	Fonts.push_back(temp);
	int i = Fonts.size() - 1;
	while (!D3DXCreateFont(d3ddev, size, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, font.c_str(), &Fonts.at(i).Font))
	{
		Fonts.at(i).Name = name;
		return true;
	}
	return false;
}
void cDirectX::initFonts()
{
	Fonts.clear();
	addFont("default", "Arial", 20);
	addFont("RobotoS", "Roboto Light", 14);
}
ID3DXFont* cDirectX::findFont(std::string name)
{
	if (Fonts.size() == 0)
		return 0;
	for (int i = 0; i < Fonts.size(); ++i)
	{
		if (name == Fonts.at(i).Name)
			return Fonts.at(i).Font;
	}
	return 0;
}