#pragma once
class vars {
public:
	bool GUI = true;
	struct ESP
	{
		struct World
		{
			bool bActive, bFort, bMapPins, bMermaid, bReapersChest, bShipWreck;
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
			bool bActive;
		}Ships;
		struct Treasure
		{
			bool bActive;
		}Treasure;
		struct Animals
		{
			bool bActive, bChicken, bPig, bSnake;
		}Animals;
	}ESP;
};
extern vars Vars;