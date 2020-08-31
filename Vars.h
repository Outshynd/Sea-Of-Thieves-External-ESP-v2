#pragma once
class vars {
public:
	bool GUI = true;
	struct ESP
	{
		bool bHeadshot;
		struct World
		{
			bool bActive, bFog, bFort, bMapPins, bMermaid, bShipWreck, bStorm;
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
			bool bActive, bChicken, bPig, bSnake;
		}Animals;
	}ESP;
};
extern vars Vars;