#include "Cheat.h"
#include "Vars.h"
cCheat* Cheat = new cCheat();
bool __debug = false;
clock_t elapsed = 0;
clock_t lastTime = clock();
Vector2 last_xy = Vector2(0.0f, 0.0f);
std::string cCheat::getNameFromIDmem(int ID) {
	try {
		DWORD_PTR fNamePtr = Mem->Read<uintptr_t>(GNames + int(ID / 0x4000) * 0x8);
		DWORD_PTR fName = Mem->Read<uintptr_t>(fNamePtr + 0x8 * int(ID % 0x4000));
		return Mem->Read<text>(fName + 0x10).word;
	}
	catch (int e)
	{
		return std::string("");
	}
}
std::string cCheat::getNameFromIDmap(int ID)
{
	auto it = Names.find(ID);
	if (it == Names.end())
		return "";
	else
		return it->second;
}
double _clockToMilliseconds(clock_t ticks) {
	return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}
Vector2 RotatePoint(Vector2 pointToRotate, Vector2 centerPoint, float angle, bool angleInRadians = false)
{
	if (!angleInRadians)
		angle = static_cast<float>(angle * (PI / 180.f));
	float cosTheta = static_cast<float>(cos(angle));
	float sinTheta = static_cast<float>(sin(angle));
	Vector2 returnVec = Vector2(cosTheta * (pointToRotate.x - centerPoint.x) - sinTheta * (pointToRotate.y - centerPoint.y), sinTheta * (pointToRotate.x - centerPoint.x) + cosTheta * (pointToRotate.y - centerPoint.y)
	);
	returnVec += centerPoint;
	return returnVec;
}
void cCheat::readData()
{
	if (!baseModule)
	{
		baseModule = Mem->Module("SoTGame.exe");
		baseSize = Mem->ModuleSize("SoTGame.exe");
	}
	uintptr_t address = 0;
	if (!UWorld)
	{
		address = Mem->FindSignature(baseModule, baseSize,
			(BYTE*)("\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\x88\x00\x00\x00\x00\x48\x85\xC9\x74\x06\x48\x8B\x49\x70"),
			(char*)"xxx????xxx????xxxxxxxxx");
		auto uworldoffset = Mem->Read<int32_t>(address + 3);
		UWorld = address + uworldoffset + 7;
	}
	if (!GNames)
	{
		address = Mem->FindSignature(baseModule, baseSize, (BYTE*)"\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\x00\x75\x3A", (char*)"xxx????xx?xx");
		auto gnamesoffset = Mem->Read<int32_t>(address + 3);
		GNames = Mem->Read<uintptr_t>(address + gnamesoffset + 7);
	}
	if (!GObjects)
	{
		address = Mem->FindSignature(baseModule, baseSize, (BYTE*)"\x48\x8B\x15\x00\x00\x00\x00\x3B\x42\x1C", (char*)"xxx????xxx");
		auto gobjectsoffset = Mem->Read<int32_t>(address + 3);
		auto offset = gobjectsoffset + 7;
		GObjects = Mem->Read<uintptr_t>(address + gobjectsoffset + 7);
	}
	if (Names.empty())
	{
		for (int i = 0; i < 206000; i++)
		{
			auto temp = getNameFromIDmem(i);
			if (temp != "FAIL")
			{
				Names.insert(std::pair<int, std::string>(i, temp));
			}
		}
	}
	auto world = Mem->Read<cUWorld>(Mem->Read<uintptr_t>(UWorld));
	auto LocalPlayer = world.GetGameInstance().GetLocalPlayer();
	auto player_controller = LocalPlayer.GetPlayerController();
	auto CameraManager = player_controller.GetCameraManager();
	SOT->localPlayer.name = Misc->wstringToString(player_controller.GetActor().GetPlayerState().GetName());
	SOT->localPlayer.position = player_controller.GetActor().GetRootComponent().GetPosition();
	SOT->localCamera.fov = CameraManager.GetCameraFOV();
	SOT->localCamera.angles = CameraManager.GetCameraRotation();
	SOT->localCamera.position = CameraManager.GetCameraPosition();
	Vector2 compass_pos = { (float)(Process->Size[0] / 2), (float)(Process->Size[1] * 0.015f) };
	float compass = CameraManager.GetCameraRotation().y;
	compass += 90.0f;
	if (compass < 1)
		compass += 360.0f;
	char* dir = "NE";
	if (compass >= 337.5 || compass < 22.5)
		dir = "N";
	else if (compass >= 22.5 && compass < 67.5)
		dir = "NE";
	else if (compass >= 67.5 && compass < 112.5)
		dir = "E";
	else if (compass >= 112.5 && compass < 157.5)
		dir = "SE";
	else if (compass >= 157.5 && compass < 202.5)
		dir = "S";
	else if (compass >= 202.5 && compass < 247.5)
		dir = "SW";
	else if (compass >= 247.5 && compass < 292.5)
		dir = "W";
	else if (compass >= 292.5 && compass < 337.5)
		dir = "NW";
	DrawString(std::to_string((int)compass).c_str(), compass_pos.x, compass_pos.y + 20, Color{ 255,255,255 }, true, "RobotoS");
	DrawString(dir, compass_pos.x, compass_pos.y, Color{ 255, 255, 255 }, true, "RobotoS");
	Vector2 my_xy = Vector2(SOT->localPlayer.position.x, SOT->localPlayer.position.y);
	if (last_xy.x == 0.0f && last_xy.y == 0.0f)
		last_xy = my_xy;
	if (my_xy.DistTo(last_xy) > 0.5f)
	{
		last_xy = my_xy;
	}
	auto level = world.GetLevel();
	auto actors = level.GetActors();
	if (!actors.IsValid())
		return;
	for (int i = 0; i < actors.Length(); ++i)
	{
		auto actor = *reinterpret_cast<AActor*>(&actors[i]);
		if (actor == player_controller.GetActor())
			continue;
		auto id = actor.GetID();
		auto name = getNameFromIDmem(id);
		if (name.find("BP_Skeleton") != std::string::npos && name.find("Pawn") != std::string::npos)
		{
			if (!Vars.ESP.Skeletons.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				Vector3 headPos = Vector3(pos.x, pos.y, pos.z + 75);
				Vector2 ScreenTop;
				Color boxColor = { 125,0,255,0 };
				if (Misc->WorldToScreen(headPos, &ScreenTop))
				{
					int hi = (Screen.y - ScreenTop.y) * 2;
					int wi = hi * 0.50;
					DrawBox(ScreenTop.x - wi / 2, ScreenTop.y, wi, hi, boxColor);
					DrawString("Skeleton", ScreenTop.x, ScreenTop.y - 14, Color{ 255,255,255 }, true, "RobotoS");
					DrawString(std::string(std::to_string((int)distance) + "m").c_str(), ScreenTop.x, ScreenTop.y - 26, Color{ 255,255,255 }, true, "RobotoS");
					
					if (Vars.ESP.Skeletons.bWeapon)
					{
						auto ItemName = actor.GetWieldedItemComponent().GetWieldedItem().GetItemInfo().GetItemDesc().GetName();
						if (ItemName.length() >= 4 && ItemName.length() < 32)
							DrawString(ItemName.c_str(), ScreenTop.x, ScreenTop.y + hi, Color{ 255,255,255 }, true, "RobotoS");
					}
				}
			}
		}
		if (name.find("BP_GiantSkeletonPawn") != std::string::npos)
		{
			if (!Vars.ESP.Skeletons.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				Vector3 headPos = Vector3(pos.x, pos.y, pos.z + 75);
				Vector2 ScreenTop;
				Color boxColor = { 125,0,255,0 };
				if (Misc->WorldToScreen(headPos, &ScreenTop))
				{
					int hi = (Screen.y - ScreenTop.y) * 2;
					int wi = hi * 0.50;
					DrawBox(ScreenTop.x - wi / 2, ScreenTop.y, wi, hi, boxColor);
					DrawString("Skeleton Lord", ScreenTop.x, ScreenTop.y - 14, Color{ 255,255,255 }, true, "RobotoS");
					DrawString(std::string(std::to_string((int)distance) + "m").c_str(), ScreenTop.x, ScreenTop.y - 26, Color{ 255,255,255 }, true, "RobotoS");
					
					if (Vars.ESP.Skeletons.bWeapon)
					{
						auto ItemName = actor.GetWieldedItemComponent().GetWieldedItem().GetItemInfo().GetItemDesc().GetName();
						if (ItemName.length() >= 4 && ItemName.length() < 32)
							DrawString(ItemName.c_str(), ScreenTop.x, ScreenTop.y + hi, Color{ 255,255,255 }, true, "RobotoS");
					}
				}
			}
		}
		else if (name.find("BP_SkellyShip_ShipCloud") != std::string::npos)
		{
			if (!Vars.ESP.World.bFort)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Ship Cloud", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_SkellyFort_SkullCloud_C") != std::string::npos)
		{
			if (!Vars.ESP.World.bFort)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Skull Fort", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_SkellyFort_RitualSkullCloud") != std::string::npos)
		{
			if (!Vars.ESP.World.bFort)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Fort of the Damned", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("Signal_Flameheart") != std::string::npos)
		{
			if (!Vars.ESP.World.bFort)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Flameheart", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("AshenLord_SkullCloud_C") != std::string::npos)
		{
			if (!Vars.ESP.World.bFort)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Ashen Lord", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("MapTable_C") != std::string::npos)
		{
			auto Table = *reinterpret_cast<AMapTable*>(&actors[i]);
			//REAPERS CHEST
			if (Vars.ESP.World.bReapersChest) {
				auto Chests = Table.GetTrackedBootyItemLocations();
				if (Chests.IsValid())
				{
					for (int p = 0; p < Chests.Length(); ++p)
					{
						auto Chest = Chests[p];
						Vector2 Screen;
						Color color = { 255,255,255,0 };
						int dist = (int)(SOT->localPlayer.position.DistTo(Chest) / 100.f);
						if (dist > 150)
						{
							if (Misc->WorldToScreen(Chest, &Screen))
							{
								DrawString("Reapers Chest", Screen.x, Screen.y, color, true, "RobotoS");
								DrawString(std::string(std::to_string(dist) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
							}
						}
					}
				}
			}
			//MAP PIN
			if (Vars.ESP.World.bMapPins) {
				auto pins = Table.GetMapPins();
				if (pins.IsValid()) {
					Color color = { 255,255,255,0 };
					for (int p = 0; p < pins.Length(); ++p)
					{
						auto pin = pins[p];
						Vector3 worldPin = Vector3(pin.x * 100, pin.y * 100, 5000);
						Vector2 Screen;
						if (Misc->WorldToScreen(worldPin, &Screen))
						{
							DrawString("Map Pin", Screen.x, Screen.y, color, true, "RobotoS");
							DrawString(std::string(std::to_string((int)(SOT->localPlayer.position.DistTo(worldPin) / 100.f)) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
						}
					}
				}
			}
		}
		else if (name.find("CrewService") != std::string::npos)
		{
			Crews.clear();
			Team tempTeam;
			TeamMate tempPlayers;
			auto crewService = *reinterpret_cast<ACrewService*>(&actors[i]);;
			auto crews = crewService.GetCrews();
			if (!crews.IsValid())
				continue;
			for (int c = 0; c < crews.Length(); ++c)
			{
				auto players = crews[c].GetPlayers();
				if (!players.IsValid())
					continue;
				tempTeam.teamName = crews[c].GetShipType();
				if (SOT->Ships[c].type.find("azure") != std::string::npos)
					tempTeam.color = Color(0, 255, 255);
				else if (SOT->Ships[c].type.find("regal") != std::string::npos)
					tempTeam.color = Color(255, 0, 255);
				else if (SOT->Ships[c].type.find("lucky") != std::string::npos)
					tempTeam.color = Color(0, 255, 0);
				else if (SOT->Ships[c].type.find("flaming") != std::string::npos)
					tempTeam.color = Color(255, 0, 0);
				else if (SOT->Ships[c].type.find("golden") != std::string::npos)
					tempTeam.color = Color(255, 255, 0);
				else
					tempTeam.color = Color(255, 255, 255);
				SOT->Ships[c].crewID = crews[c].GetCrewID();
				for (int p = 0; p < players.Length(); ++p)
				{
					tempPlayers.PlayerName = Misc->wstringToString(players[p].GetName());
					if (tempPlayers.PlayerName == SOT->localPlayer.name)
						SOT->localPlayer.crewID = crews[c].GetCrewID();
					SOT->Pirates[(c * 4) + p].crewID = crews[c].GetCrewID();
					SOT->Pirates[(c * 4) + p].name = tempPlayers.PlayerName;
					tempTeam.Players.push_back(tempPlayers);
				}
				Crews.push_back(tempTeam);
				tempTeam.Players.clear();
			}
		}
		else if (name.find("BP_PlayerPirate_C") != std::string::npos)
		{
			if (!Vars.ESP.Players.bActive)
				continue;
			bool revivable = false;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			int health = actor.GetHealthComponent().GetHealth();
			if (health <= 0)				
				revivable = true;
			auto ItemName = actor.GetWieldedItemComponent().GetWieldedItem().GetItemInfo().GetItemDesc().GetName();
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				Vector3 headPos = Vector3(pos.x, pos.y, pos.z + 75);
				Vector2 ScreenTop;
				Color boxColor = { 255,0,0,0 };
				bool bTeammate = false;
				if (Misc->WorldToScreen(headPos, &ScreenTop))
				{
					int hi = (Screen.y - ScreenTop.y) * 2;
					int wi = hi * 0.5;
					auto pirateName = Misc->wstringToString(actor.GetPlayerState().GetName());
					for (int pirates = 0; pirates < 24; ++pirates)
					{
						if (SOT->Pirates[pirates].name == "")
							continue;
						if (pirateName == SOT->Pirates[pirates].name)
						{
							SOT->Pirates[pirates].distance = distance;
							if (SOT->Pirates[pirates].crewID == SOT->localPlayer.crewID)
							{
								boxColor = Color{ 0,255,0,0 };
								bTeammate = true;
							}
						}
					}
					if (!Vars.ESP.Players.bTeam && bTeammate)
						continue;
					DrawBox(ScreenTop.x - wi / 2, ScreenTop.y, wi, hi, boxColor);
					if (Vars.ESP.Players.bName)
					{
						DrawString(pirateName.c_str(), ScreenTop.x, ScreenTop.y - 14, Color{ 255,255,255 }, true, "RobotoS");
						DrawString(std::string(std::to_string((int)distance) + "m").c_str(), ScreenTop.x, ScreenTop.y - 26, Color{ 255,255,255 }, true, "RobotoS");
					}
					if (Vars.ESP.Players.bWeapon)
					{
						if (ItemName.length() >= 4 && ItemName.length() < 32)
							if (revivable == false)
								DrawString(ItemName.c_str(), ScreenTop.x, ScreenTop.y + hi, Color{ 255,255,255 }, true, "RobotoS");
							else
								DrawString("None", ScreenTop.x, ScreenTop.y + hi, Color{ 255,255,255 }, true, "RobotoS");
					}
					if (Vars.ESP.Players.bHealth)
					{
						float maxHealth = actor.GetHealthComponent().GetMaxHealth();
						DrawHealthBar(health, maxHealth, ScreenTop.x, ScreenTop.y + 1, wi, hi);
					}
				}
			}
		}
		else if (name.find("BP_SmallShipTemplate_C") != std::string::npos || name.find("BP_SmallShipNetProxy") != std::string::npos)
		{
			if (!Vars.ESP.Ships.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);
			FGuid crewid = Ship.GetCrewOwnershipComponent().GetCrewId();
			if (name.find("NetProxy") != std::string::npos)
			{
				if (Ship.GetOwningActor())
					continue;
				crewid = FGuid();
			}
			Color color = { 255,0,0,0 };
			if (SOT->localPlayer.crewID == crewid)
			{
				color = { 0,255,0,0 };
			}
			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 1500), &Screen))
			{
				DrawString("Sloop", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_MediumShipTemplate_C") != std::string::npos || name.find("BP_MediumShipNetProxy") != std::string::npos)
		{
			if (!Vars.ESP.Ships.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);
			if (name.find("NetProxy") != std::string::npos)
				if (Ship.GetOwningActor())
					continue;
			Color color = { 255,0,0,0 };
			if (SOT->localPlayer.crewID == Ship.GetCrewOwnershipComponent().GetCrewId())
			{
				color = { 0,255,0,0 };
			}
			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 1750), &Screen))
			{
				DrawString("Brigantine", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_LargeShipTemplate_C") != std::string::npos || name.find("BP_LargeShipNetProxy") != std::string::npos)
		{
			if (!Vars.ESP.Ships.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);
			if (name.find("NetProxy") != std::string::npos)
				if (Ship.GetOwningActor())
					continue;
			Color color = { 255,0,0,0 };
			if (SOT->localPlayer.crewID == Ship.GetCrewOwnershipComponent().GetCrewId())
				color = { 0,255,0,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 2000), &Screen))
			{
				DrawString("Galleon", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_AISmallShipTemplate") != std::string::npos || name.find("BP_AISmallShipNetProxy") != std::string::npos)
		{
			if (!Vars.ESP.Ships.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);
			if (name.find("NetProxy") != std::string::npos)
			{
				if (Ship.GetOwningActor())
					continue;
			}
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 1500), &Screen))
			{
				DrawString("Skeleton Sloop", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_AILargeShipTemplate") != std::string::npos || name.find("BP_AILargeShipNetProxy") != std::string::npos)
		{
			if (!Vars.ESP.Ships.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);
			if (name.find("NetProxy") != std::string::npos)
			{
				if (Ship.GetOwningActor())
					continue;
			}
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 2000), &Screen))
			{
				DrawString("Skeleton Galleon", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_Shipwreck_") != std::string::npos)
		{
			if (!Vars.ESP.World.bShipWreck)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, 1000), &Screen))
			{
				DrawString("Shipwreck", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_SunkenCurseArtefact_") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z), &Screen))
			{
				if (name.find("Ruby") != std::string::npos)
				{
					DrawString("Ruby Statue", Screen.x, Screen.y, Color{ 255,0,0 }, true, "RobotoS");
					DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, Color{ 255,0,0 }, true, "RobotoS");
				}
				else if (name.find("Emerald") != std::string::npos)
				{
					DrawString("Emerald Statue", Screen.x, Screen.y, Color{ 255,0,0 }, true, "RobotoS");
					DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, Color{ 255,0,0 }, true, "RobotoS");
				}
				else if (name.find("Sapphire") != std::string::npos)
				{
					DrawString("Sapphire Statue", Screen.x, Screen.y, Color{ 255,0,0 }, true, "RobotoS");
					DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, Color{ 255,0,0 }, true, "RobotoS");
				}
			}
		}
		else if ((name.find("BP_Pig_") != std::string::npos && Vars.ESP.Animals.bPig) || (name.find("BP_Snake_") != std::string::npos && Vars.ESP.Animals.bSnake) || (name.find("BP_Chicken_") != std::string::npos && Vars.ESP.Animals.bChicken))
		{
			if (!Vars.ESP.Animals.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			auto Fauna = *reinterpret_cast<AFauna*>(&actors[i]);
			Color color = Color{ 255,255,255,0 };
			if (name.find("Common") != std::wstring::npos)
				color = Color{ 255,255,255,0 };
			else if (name.find("Rare") != std::wstring::npos)
				color = Color{ 182,97,70,0 };
			else if (name.find("Mythical") != std::wstring::npos)
				color = Color{ 136,119,151,0 };
			else if (name.find("Legendary") != std::wstring::npos)
				color = Color{ 100,185,185,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString(Fauna.GetName().c_str(), Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("Proxy") != std::string::npos)
		{
			if (!Vars.ESP.Treasure.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			pos.z += 20;
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			auto treasure = *reinterpret_cast<AItemProxy*>(&actors[i]);
			int booty = treasure.GetBootyItemInfo().GetBootyType();
			bool skip = false;
			if (booty > EBootyTypes::EBootyTypes__EBootyTypes_MAX || booty < 1)
				skip = true;
			if (name.find("Pineapple") != std::string::npos || name.find("AnyItemCrate") != std::string::npos || name.find("StrongholdKey") != std::string::npos)
				skip = false;
			if (name.find("SkeletonOrders") != std::string::npos)
				skip = false;
			if (skip)
				continue;
			auto rarity = getNameFromIDmem(treasure.GetBootyItemInfo().GetRareityId());
			Color color = Color{ 255,255,255,0 };
			if (name.find("Common") != std::wstring::npos)
				color = Color{ 255,255,255,0 };
			else if (name.find("Rare") != std::wstring::npos)
				color = Color{ 182,97,70,0 };
			else if (name.find("Mythical") != std::wstring::npos)
				color = Color{ 136,119,151,0 };
			else if (name.find("Legendary") != std::wstring::npos)
				color = Color{ 100,185,185,0 };
			else if (rarity.find("Fort") != std::string::npos || rarity.find("Stronghold") != std::string::npos || rarity.find("PirateLegend") != std::string::npos || rarity.find("Drunken") != std::string::npos || rarity.find("Sorrow") != std::string::npos || rarity.find("AIShip") != std::string::npos)
				color = Color{ 255,215,0,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString(treasure.GetBootyItemInfo().GetItemDesc().GetName().c_str(), Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_Storm_C") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			pos.z = 35000.0f;
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			if (distance > 6000 || distance < 150)
				continue;
			Color color = { 255,255,255 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Storm", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_Mermaid_C") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			pos.z += 20.0f;
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Mermaid", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("HoFFort") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Ashen Guardian's Notes", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("SeaboundSoul_AshenChestRiddle") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Ashen Guardian's Notes", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_RowboatRowingSeat_C") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			pos.z += 20.0f;
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			if (distance < 40)
				continue;
			Color color = { 255,255,255 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Rowboat", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_FogBank_C") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			pos.z = 5000.0f;
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			if (distance > 6000)
				continue;
			Color color = { 255,255,255 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Fog", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_RomeBeacon") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			pos.z += 150;
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			if (distance <= 150)
				continue;
			Color color = Color{ 255,215,0,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Glorious Sea Dog Chest", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_GhostShips_Signal_Flameheart_NetProxy_C") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = Color{ 255,215,0,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Flameheart", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_AggressiveGhostShip_C") != std::string::npos)
		{
			if (!Vars.ESP.Ships.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			auto Ship = *reinterpret_cast<AShip*>(&actors[i]);
			if (name.find("NetProxy") != std::string::npos)
				if (Ship.GetOwningActor())
					continue;
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(Vector3(pos.x, pos.y, pos.z + 2000), &Screen))
			{
				DrawString("Ghost Ship", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_AggressiveGhostShip_Mine_C") != std::string::npos)
		{
			if (!Vars.ESP.Ships.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Ghost Ship Mine", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (name.find("BP_GhostShipRewardMarker_C") != std::string::npos)
		{
			if (!Vars.ESP.Treasure.bActive)
				continue;
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString("Ghost Ship Rewards", Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
		else if (__debug == true && name.find("BP") != std::string::npos)
		{
			auto pos = actor.GetRootComponent().GetPosition();
			auto distance = SOT->localCamera.position.DistTo(pos) / 100.00f;
			Color color = { 255,255,255,0 };
			Vector2 Screen;
			if (Misc->WorldToScreen(pos, &Screen))
			{
				DrawString(name.c_str(), Screen.x, Screen.y, color, true, "RobotoS");
				DrawString(std::string(std::to_string((int)distance) + "m").c_str(), Screen.x, Screen.y + 18, color, true, "RobotoS");
			}
		}
	}
}