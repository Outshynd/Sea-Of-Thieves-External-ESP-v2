#pragma once
class vars {
public:
	bool GUI = true;
	struct ESP
	{
		int colorCommon[4], colorRare[4], colorMythical[4], colorLegendary[4], colorSpecial[4], colorOther[4];
		struct World
		{
			int colorWorld[4];
			bool bActive, bShipWreck, bMermaid, bFort, bReapersChest, bMapPins;
		}World;
		struct Players
		{
			int colorEnemy[4], colorTeam[4];
			bool bActive, bWeapon, bName, bHealth, bTeam;
		}Players;
		struct Skeletons
		{
			int colorSkeleton[4];
			bool bActive, bWeapon;
		}Skeletons;
		struct Ships
		{
			int colorEnemy[4], colorTeam[4];
			bool bActive;
		}Ships;
		struct Treasure
		{
			bool bActive;
		}Treasure;
		struct Animals
		{
			bool bActive, bSnake, bChicken, bPig;
		}Animals;
	}ESP;
};
extern vars Vars;