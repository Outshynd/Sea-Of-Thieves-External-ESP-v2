#include "Drawing.h"
cDrawing* Draw = new cDrawing();
void cDrawing::Line(float x, float y, float x2, float y2, D3DCOLOR color)
{
	if (!directX->LinePtr)
		D3DXCreateLine(directX->d3ddev, &directX->LinePtr);
	else
	{
		D3DXVECTOR2 Vertex[2];
		Vertex[0].x = x;
		Vertex[0].y = y;
		Vertex[1].x = x2;
		Vertex[1].y = y2;
		directX->LinePtr->Begin();
		directX->LinePtr->Draw(Vertex, 2, color);
		directX->LinePtr->End();
	}
}
void cDrawing::Box(int x, int y, int width, int height, D3DCOLOR colour) {
	Line(x, y, x + width, y, colour);
	Line(x + width, y, x + width, y + height, colour);
	Line(x, y, x, y + height, colour);
	Line(x, y + height, x + width, y + height, colour);
}
void cDrawing::FilledBox(float x, float y, float width, float height, D3DCOLOR colour) {
	sD3DVertex pVertex[4] = { { x, y + height, 0.0f, 1.0f, colour },{ x, y, 0.0f, 1.0f, colour },{ x + width, y + height, 0.0f, 1.0f, colour },{ x + width, y, 0.0f, 1.0f, colour } };
	directX->d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	directX->d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(sD3DVertex));
}
void cDrawing::String(const char* szString, float X, float Y, D3DCOLOR COLOR, bool Centered, std::string name)
{
	auto Font = directX->findFont(name);
	if (Font)
	{
		RECT Center = { 0, 0, 0, 0 }, Position = { 0, 0, 0, 0 };
		if (Centered)
			Font->DrawTextA(0, szString, (INT)strlen(szString), &Center, DT_CALCRECT, NULL);
		Position.left = ((LONG)X + 1) - (Center.right / 2);
		Position.top = ((LONG)Y + 1);
		Font->DrawTextA(0, szString, (INT)strlen(szString), &Position, DT_NOCLIP, D3DCOLOR_XRGB(1, 1, 1));
		Position.left = (LONG)X - (Center.right / 2);
		Position.top = (LONG)Y;
		Font->DrawTextA(0, szString, (INT)strlen(szString), &Position, DT_NOCLIP, COLOR);
	}
}
void cDrawing::String(const wchar_t* szString, float X, float Y, D3DCOLOR COLOR, bool Centered, std::string name)
{
	auto Font = directX->findFont(name);
	if (Font)
	{
		RECT Center = { 0, 0, 0, 0 }, Position = { 0, 0, 0, 0 };
		if (Centered)
			Font->DrawTextW(0, szString, (INT)wcslen(szString), &Center, DT_CALCRECT, NULL);
		Position.left = ((LONG)X + 1) - (Center.right / 2);
		Position.top = ((LONG)Y + 1);
		Font->DrawTextW(0, szString, (INT)wcslen(szString), &Position, DT_NOCLIP, D3DCOLOR_XRGB(1, 1, 1));
		Position.left = (LONG)X - (Center.right / 2);
		Position.top = (LONG)Y;
		Font->DrawTextW(0, szString, (INT)wcslen(szString), &Position, DT_NOCLIP, COLOR);
	}
}
void cDrawing::HealthBar(int health, float maxhealth, int x, int y, int width, int height)
{
	if (health > maxhealth)
		health = maxhealth;
	int r = 255 - health * 2.55;
	int g = health * 2.55;
	float wtf = health / maxhealth;
	float healthBar = height * wtf;
	int healthBarDelta = height - healthBar;
	FilledBox(x - width / 2 - 6, y - 1, 5, height + 1, D3DCOLOR_XRGB(10, 10, 10));
	FilledBox(x - width / 2 - 5, y + healthBarDelta, 3, healthBar, D3DCOLOR_XRGB(r, g, 0));
}
void cDrawing::DrawPlayersList(int x, int y, std::vector<Team> Teams)
{
	if (Teams.size() == 0)
		return;
	for (int i = 0; i < Teams.size(); ++i)
	{
		auto color D3DCOLOR_XRGB(255, 255, 255);
		i % 2 == 0 ? color = D3DCOLOR_XRGB(77, 77, 77) : color = D3DCOLOR_XRGB(51, 51, 51);
		String(Teams.at(i).teamName.c_str(), x + 35, y + 2 + 60 * i, D3DCOLOR_XRGB(Teams.at(i).color.r, Teams.at(i).color.g, Teams.at(i).color.b), true, "RobotoS");
		for (int j = 0; j < 4; ++j)
		{
			j % 2 == 0 ? color = D3DCOLOR_XRGB(51, 51, 51) : color = D3DCOLOR_XRGB(77, 77, 77);
			if (j < Teams.at(i).Players.size())
				String(Teams.at(i).Players.at(j).PlayerName.c_str(), x + 75, y + 2 + (i * 4 * 15) + 15 * j, D3DCOLOR_XRGB(255, 255, 255), false, "RobotoS");
		}
	}
}
static bool once = false;
void cDirectX::Render(bool active)
{
	d3ddev->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, NULL);
	d3ddev->BeginScene();
	if (!once)
		Draw->String("Initializing: Please Wait...", 10, 10, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoS");
	if (once)
	{
		if (active)
			Draw->Draw();
	}
	if (!once)
		once = true;
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}
void cCheat::DrawString(const char* szString, float X, float Y, Color COLOR, bool Centered, std::string name)
{
	Draw->String(szString, X, Y, D3DCOLOR_XRGB(COLOR.r, COLOR.g, COLOR.b), Centered, name);
}
void cCheat::DrawString(const wchar_t* szString, float X, float Y, Color COLOR, bool Centered, std::string name)
{
	Draw->String(szString, X, Y, D3DCOLOR_XRGB(COLOR.r, COLOR.g, COLOR.b), Centered, name);
}
void cCheat::DrawHealthBar(int health, float maxhealth, int x, int y, int width, int height)
{
	Draw->HealthBar(health, maxhealth, x, y, width, height);
}
void cCheat::DrawBox(int x, int y, int width, int height, Color color)
{
	Draw->Box(x, y, width, height, D3DCOLOR_XRGB(color.r, color.g, color.b));
}
void cCheat::DrawLine(float X, float Y, float X2, float Y2, Color color)
{
	Draw->Line(X, Y, X2, Y2, D3DCOLOR_XRGB(color.r, color.g, color.b));
}