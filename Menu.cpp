#include "Menu.h"


cMenu* Menu = new cMenu();

void cMenu::DrawMenu()
{

}

void cDrawing::Draw()
{
	Cheat->readData();


	//String(std::string("Credits: Gummy8unny").c_str(), 10, Process->Size[1] - 20, D3DCOLOR_XRGB(0, 255, 0), false, "small");
	String(std::string("Edited by Shynd").c_str(), 10, Process->Size[1] - 20, D3DCOLOR_XRGB(255, 255, 255), false, "small");
	
	//String(std::to_string(directX->frames).c_str(), 100, 100, D3DCOLOR_XRGB(0, 255, 0), false, "small");

	if (Vars.GUI)
	{
		if (Vars.ESP.Animals.bActive)
			String(std::string("F2: Animals On").c_str(), 10, 10, D3DCOLOR_XRGB(0, 255, 0), false, "default");
		else
			String(std::string("F2: Animals Off").c_str(), 10, 10, D3DCOLOR_XRGB(255, 0, 0), false, "default");

		if (Vars.ESP.Player.bActive)
			String(std::string("F3: Players On").c_str(), 210, 10, D3DCOLOR_XRGB(0, 255, 0), false, "default");
		else
			String(std::string("F3: Players Off").c_str(), 210, 10, D3DCOLOR_XRGB(255, 0, 0), false, "default");

		if (Vars.ESP.Ships.bActive)
			String(std::string("F4: Ships On").c_str(), 410, 10, D3DCOLOR_XRGB(0, 255, 0), false, "default");
		else
			String(std::string("F4: Ships Off").c_str(), 410, 10, D3DCOLOR_XRGB(255, 0, 0), false, "default");

		if (Vars.ESP.Treasure.bActive)
			String(std::string("F5: Treasure On").c_str(), 610, 10, D3DCOLOR_XRGB(0, 255, 0), false, "default");
		else
			String(std::string("F5: Treasure Off").c_str(), 610, 10, D3DCOLOR_XRGB(255, 0, 0), false, "default");
	}

	//draw crosshair
	const int crosshair_size = 8;

	Line(Process->Size[0] / 2 - crosshair_size, Process->Size[1] / 2, Process->Size[0] / 2 + crosshair_size + 1, Process->Size[1] / 2, D3DCOLOR_XRGB(255, 0, 0));

	Line(Process->Size[0] / 2, Process->Size[1] / 2 - crosshair_size, Process->Size[0] / 2, Process->Size[1] / 2 + crosshair_size + 1, D3DCOLOR_XRGB(255, 0, 0));

	//draw menu
	if (Menu->menuOpen)
		DrawScoreBoard(0, 100, Cheat->Crews);

	Menu->DrawMenu();
}
