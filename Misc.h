#pragma once
#include "SotStuff.h"
#include <codecvt>
class cMisc
{
public:
	bool WorldToScreen(Vector3 world, Vector2* screen);
	std::string wstringToString(std::wstring wstring);
};
extern cMisc* Misc;