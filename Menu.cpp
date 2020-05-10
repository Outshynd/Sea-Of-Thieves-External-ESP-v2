#include "Menu.h"


cMenu* Menu = new cMenu();

void cMenu::DrawMenu()
{

}

void cDrawing::Draw()
{
	Cheat->readData();


	//String(std::string("Credits: Gummy8unny").c_str(), 10, Process->Size[1] - 20, D3DCOLOR_XRGB(0, 255, 0), false, "small");
	String(std::string("Credits: Gummy8unny - Edited by Shynd").c_str(), 10, Process->Size[1] - (Menu->largeFont == false ? 20 : 40), D3DCOLOR_XRGB(255, 255, 255), false, "RobotoS");

	//String(std::to_string(directX->frames).c_str(), 100, 100, D3DCOLOR_XRGB(0, 255, 0), false, "small");

	float screen_top = Process->Size[1] * 0.01f;

	if (Vars.GUI)
	{
		if (Vars.ESP.Animals.bActive)
			String(std::string("F2: Animals On").c_str(), 100, screen_top, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoM");
		else
			String(std::string("F2: Animals Off").c_str(), 100, screen_top, D3DCOLOR_XRGB(255, 0, 0), false, "RobotoM");

		if (Vars.ESP.Player.bActive)
			String(std::string("F3: Players On").c_str(), (Menu->largeFont == false ? 250 : 500), screen_top, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoM");
		else
			String(std::string("F3: Players Off").c_str(), (Menu->largeFont == false ? 250 : 500), screen_top, D3DCOLOR_XRGB(255, 0, 0), false, "RobotoM");

		if (Vars.ESP.Ships.bActive)
			String(std::string("F4: Ships On").c_str(), (Menu->largeFont == false ? 400 : 900), screen_top, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoM");
		else
			String(std::string("F4: Ships Off").c_str(), (Menu->largeFont == false ? 400 : 900), screen_top, D3DCOLOR_XRGB(255, 0, 0), false, "RobotoM");

		if (Vars.ESP.Treasure.bActive)
			String(std::string("F5: Treasure On").c_str(), (Menu->largeFont == false ? 550 : 1300), screen_top, D3DCOLOR_XRGB(0, 255, 0), false, "RobotoM");
		else
			String(std::string("F5: Treasure Off").c_str(), (Menu->largeFont == false ? 550 : 1300), screen_top, D3DCOLOR_XRGB(255, 0, 0), false, "RobotoM");
	}

	//draw crosshair
	/*const int crosshair_size = 8;

	Line(Process->Size[0] / 2 - crosshair_size, Process->Size[1] / 2, Process->Size[0] / 2 + crosshair_size + 1, Process->Size[1] / 2, D3DCOLOR_XRGB(255, 0, 0));

	Line(Process->Size[0] / 2, Process->Size[1] / 2 - crosshair_size, Process->Size[0] / 2, Process->Size[1] / 2 + crosshair_size + 1, D3DCOLOR_XRGB(255, 0, 0));*/

	//FilledBox()

	const int crosshair_width = 2;
	const int crosshair_length = (5 * 2) + crosshair_width;

	Vector2 crossvert, crosshoriz, screenmiddle;
	int vertical_height, vertical_width, horiz_height, horiz_width;

	screenmiddle.x = Process->Size[0] / 2;
	screenmiddle.y = Process->Size[1] / 2;

	//vertical setup
	crossvert.x = screenmiddle.x - (crosshair_width / 2);
	crossvert.y = screenmiddle.y - (crosshair_length / 2);

	FilledBox(crossvert.x, crossvert.y, crosshair_width, crosshair_length, D3DCOLOR_XRGB(255, 255, 255));

	//horizontal setup
	crosshoriz.x = screenmiddle.x - (crosshair_length / 2);
	crosshoriz.y = screenmiddle.y - (crosshair_width / 2);

	FilledBox(crosshoriz.x, crosshoriz.y, crosshair_length, crosshair_width, D3DCOLOR_XRGB(255, 255, 255));


	//crossvert2.x = screenmiddle.x + (crosshair_width / 2);
	//crossvert1.y = screenmiddle.y + crosshair_length + (crosshair_width / 2);


	//Vector2 compass_pos = { Process->Size[0], Process->Size[1] };

	//float compass = CameraManager.GetCameraRotation().y;
	//compass += 90.0f;


	//draw menu
	if (Menu->menuOpen)
		DrawScoreBoard(0, 100, Cheat->Crews);

	Menu->DrawMenu();
}
