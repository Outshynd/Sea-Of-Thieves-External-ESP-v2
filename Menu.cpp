#include "Menu.h"
#include "Vars.h"
cMenu* Menu = new cMenu();
void cDrawing::Draw()
{
	Cheat->readData();
	float screen_top = Process->Size[1] * 0.01f;
	if (Vars.GUI)
	{
		if (Vars.ESP.World.bActive)
			String(std::string("F1: World").c_str(), 10, screen_top, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoS");
		else
			String(std::string("F1: World").c_str(), 10, screen_top, D3DCOLOR_XRGB(255, 0, 0), false, "RobotoS");
		if (Vars.ESP.Players.bActive)
			String(std::string("F2: Players").c_str(), 110, screen_top, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoS");
		else
			String(std::string("F2: Players").c_str(), 110, screen_top, D3DCOLOR_XRGB(255, 0, 0), false, "RobotoS");
		if (Vars.ESP.Skeletons.bActive)
			String(std::string("F3: Skeletons").c_str(), 210, screen_top, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoS");
		else
			String(std::string("F3: Skeletons").c_str(), 210, screen_top, D3DCOLOR_XRGB(255, 0, 0), false, "RobotoS");
		if (Vars.ESP.Ships.bActive)
			String(std::string("F4: Ships").c_str(), 310, screen_top, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoS");
		else
			String(std::string("F4: Ships").c_str(), 310, screen_top, D3DCOLOR_XRGB(255, 0, 0), false, "RobotoS");
		if (Vars.ESP.Treasure.bActive)
			String(std::string("F5: Treasure").c_str(), 410, screen_top, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoS");
		else
			String(std::string("F5: Treasure").c_str(), 410, screen_top, D3DCOLOR_XRGB(255, 0, 0), false, "RobotoS");
		if (Vars.ESP.Animals.bActive)
			String(std::string("F6: Animals").c_str(), 510, screen_top, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoS");
		else
			String(std::string("F6: Animals").c_str(), 510, screen_top, D3DCOLOR_XRGB(255, 0, 0), false, "RobotoS");
	}
	Line(Process->Size[0] / 2 - 5, Process->Size[1] / 2, Process->Size[0] / 2 + 5, Process->Size[1] / 2, D3DCOLOR_XRGB(255, 255, 255));
    Line(Process->Size[0] / 2, Process->Size[1] / 2 - 5, Process->Size[0] / 2, Process->Size[1] / 2 + 5, D3DCOLOR_XRGB(255, 255, 255));
	if (Menu->menuOpen)
		DrawPlayersList(0, 50, Cheat->Crews);
}