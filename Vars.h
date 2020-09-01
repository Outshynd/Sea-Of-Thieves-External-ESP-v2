#pragma once
class vars {
public:
	bool GUI = true;
	struct ESP
	{
		struct World
		{
			bool bActive, bAmmoCrate, bBarrel, bFog, bFort, bMapPins, bMermaid, bShipWreck, bStorageCrate, bStorm;
		}World;
		struct Players
		{
			bool bActive, bHealth, bName, bTeam, bWeapon;
		}Players;
		struct Skeletons
		{
			bool bActive, bWeapon;
		}Skeletons;
		struct Ships
		{
			bool bActive, bRowboat;
		}Ships;
		struct Treasure
		{
			bool bActive, bGloriousSeaDogChest, bMermaidStatues, bReapersChest;
		}Treasure;
		struct Animals
		{
			bool bActive, bChicken, bMegalodon, bPig, bShark, bSnake;
		}Animals;
	}ESP;
};
extern vars Vars;