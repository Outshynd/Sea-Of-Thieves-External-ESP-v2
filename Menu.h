#pragma once
#include "Drawing.h"


class cMenu
{
public:
	bool menuOpen = true;
	Vector2 curPos;
	bool largeFont = false;

public:
	void DrawMenu();

private:

};

extern cMenu* Menu;