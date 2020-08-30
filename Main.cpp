#include "Menu.h"
#include "Vars.h"
#include <dwmapi.h>
vars Vars;
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
class KeyToggle {
public:
	KeyToggle(int key) :mKey(key), mActive(false) {}
	operator bool() {
		if (GetAsyncKeyState(mKey)) {
			if (!mActive) {
				mActive = true;
				return true;
			}
		}
		else
			mActive = false;
		return false;
	}
private:
	int mKey;
	bool mActive;
};
KeyToggle toggleF1(VK_F1);
KeyToggle toggleF2(VK_F2);
KeyToggle toggleF3(VK_F3);
KeyToggle toggleF4(VK_F4);
KeyToggle toggleF5(VK_F5);
KeyToggle toggleF6(VK_F6);
KeyToggle toggleF7(VK_F7);
clock_t deltaTime = 0;
unsigned int frames = 0;
int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	if (!Process->setWindow("Sea of Thieves"))
	{
		MessageBoxA(NULL, "Failed To Find The Window", "Failed To Find The Window", MB_OK);
		return 1;
	}
	if (!Process->attachProcess("SoTGame.exe"))
	{
		MessageBoxA(NULL, "Failed To Find The Process", "Failed To Find The Process", MB_OK);
		return 1;
	}
	HWND hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass";
	RegisterClassEx(&wc);
	hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT,
		"WindowClass",
		"RandomTitle",
		WS_POPUP,
		Process->Position[0], Process->Position[1],
		Process->Size[0], Process->Size[1],
		NULL,
		NULL,
		hInstance,
		NULL);
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea(hWnd, &margins);
	ShowWindow(hWnd, nCmdShow);
	directX->initD3D(hWnd);
	Process->myWindow = hWnd;
	MSG msg;
	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;
		if (toggleF1) {
			Vars.ESP.World.bActive = !Vars.ESP.World.bActive;
			Vars.ESP.World.bFog = !Vars.ESP.World.bFog;
			Vars.ESP.World.bFort = !Vars.ESP.World.bFort;
			Vars.ESP.World.bMapPins = !Vars.ESP.World.bMapPins;
			Vars.ESP.World.bMermaid = !Vars.ESP.World.bMermaid;
			Vars.ESP.World.bShipWreck = !Vars.ESP.World.bShipWreck;
			Vars.ESP.World.bStorm = !Vars.ESP.World.bStorm;
		}
		if (toggleF2) {
			Menu->menuOpen = !Menu->menuOpen;
			Vars.ESP.Players.bActive = !Vars.ESP.Players.bActive;
			Vars.ESP.Players.bHealth = !Vars.ESP.Players.bHealth;
			Vars.ESP.Players.bName = !Vars.ESP.Players.bName;
			Vars.ESP.Players.bTeam = !Vars.ESP.Players.bTeam;
			Vars.ESP.Players.bWeapon = !Vars.ESP.Players.bWeapon;
		}
		if (toggleF3) {
			Vars.ESP.Skeletons.bActive = !Vars.ESP.Skeletons.bActive;
			Vars.ESP.Skeletons.bWeapon = !Vars.ESP.Skeletons.bWeapon;
		}
		if (toggleF4) {
			Vars.ESP.Ships.bActive = !Vars.ESP.Ships.bActive;
			Vars.ESP.Ships.bRowboat = !Vars.ESP.Ships.bRowboat;
		}
		if (toggleF5) {
			Vars.ESP.Treasure.bActive = !Vars.ESP.Treasure.bActive;
			Vars.ESP.Treasure.bGloriousSeaDogChest = !Vars.ESP.Treasure.bGloriousSeaDogChest;
			Vars.ESP.Treasure.bMermaidStatues = !Vars.ESP.Treasure.bMermaidStatues;
			Vars.ESP.Treasure.bReapersChest = !Vars.ESP.Treasure.bReapersChest;
		}
		if (toggleF6) {
			Vars.ESP.Animals.bActive = !Vars.ESP.Animals.bActive;
			Vars.ESP.Animals.bChicken = !Vars.ESP.Animals.bChicken;
			Vars.ESP.Animals.bPig = !Vars.ESP.Animals.bPig;
			Vars.ESP.Animals.bSnake = !Vars.ESP.Animals.bSnake;
		}
		if (toggleF7) {
			Vars.ESP.Players.bHeadshot = !Vars.ESP.Players.bHeadshot;
		}
		if (GetAsyncKeyState(VK_END))
			break;
		Process->getSize();
		MoveWindow(hWnd, Process->Position[0], Process->Position[1], Process->Size[0], Process->Size[1], false);
		if (Process->isWindowMaximized())
			directX->Reset();
		else
		{
			clock_t beginFrame = clock();
			if (!directX->resetLock)
				directX->Render(Process->isWindowActive());
			clock_t endFrame = clock();
			deltaTime += endFrame - beginFrame;
			frames++;
		}
	}
	directX->cleanD3D();
	return msg.wParam;
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_SIZE:
		Process->zoomOnce = false;
		directX->Reset();
		break;
	case WM_MOUSEMOVE:
		Menu->curPos.x = GET_X_LPARAM(lParam);
		Menu->curPos.y = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONDOWN:
	{
		break;
	}
	case WM_LBUTTONUP:
	{
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}