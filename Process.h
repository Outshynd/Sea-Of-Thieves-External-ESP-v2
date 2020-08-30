#pragma once
#include "Memory.h"
#include <string>
class cProcess
{
public:
	int Size[2];
	int Position[2];
	HWND myWindow;
	bool zoomOnce = false;
	bool attachProcess(std::string Process);
	bool setWindow(std::string Window);
	bool isWindowActive();
	bool getSize();
	bool isWindowMaximized();
private:
	RECT WindowRect;
	RECT frame;
	HWND targetWindow;
	HANDLE targetProcess;
};
extern cProcess* Process;