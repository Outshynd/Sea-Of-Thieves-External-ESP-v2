#pragma once
#include "DirectX.h"
class cDrawing
{
public:
	enum gr_orientation {
		horizontal,
		vertical
	};
	void Line(float X, float Y, float X2, float Y2, D3DCOLOR color);
	void FilledBox(float x, float y, float width, float height, D3DCOLOR color);
	void String(const char * szString, float X, float Y, D3DCOLOR COLOR, bool Centered, std::string name);
	void String(const wchar_t * szString, float X, float Y, D3DCOLOR COLOR, bool Centered, std::string name);
	void HealthBar(int health, float maxhealth, int x, int y, int width, int height);
	void DrawPlayersList(int x, int y, std::vector<Team> Teams);
	void Box(int x, int y, int width, int height, D3DCOLOR colour);
	void Draw();
private:
	struct sD3DVertex {
		float x, y, z, rhw;
		DWORD color;
	};
};
extern cDrawing* Draw;