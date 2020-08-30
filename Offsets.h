#pragma once
class offsets {
public:
	struct AActor
	{
		int actorId = 0x18;
		int rootComponent = 0x170;
		int PlayerState = 0x478;
		int WieldedItemComponent = 0x880;
		int HealthComponent = 0x8A8;
	}AActor;
	struct ABootyItemInfo
	{
		int BootyType = 0x598;
		int Rarity = 0x704;
	}ABootyItemInfo;
	struct ACrewService
	{
		int Crews = 0x620;
	}ACrewService;
	struct AFauna
	{
		int Name = 0x888;
	}AFauna;
	struct AItemInfo
	{
		int UItemDesc = 0x4B0;
	}AItemInfo;
	struct AItemProxy
	{
		int AItemInfo = 0x620;
	}AItemProxy;
	struct AMaptTable
	{
		int ServerCenter = 0x528;
		int MapPins = 0x548;
		int TrackedShips = 0x558;
	}AMapTable;
	struct APlayerCameraManager
	{
		int CameraCache = 0x4D0;
	}APlayerCameraManager;
	struct APlayerController
	{
		int Pawn = 0x460;
		int CameraManager = 0x4E8;
		int ControlRotation = 0x2AC;
	}APlayerController;
	struct APlayerState
	{
		int PlayerName = 0x460;
	}APlayerState;
	struct AShip
	{
		int CrewOwnershipComponent = 0x7D0;
	}AShip;
	struct AWieldableItem
	{
		int ItemInfo = 0x608;
	}AWieldableItem;
	struct UCrewOwnershipComponent
	{
		int CrewId = 0xD4;
	}UCrewOwnershipComponent;
	struct UGameInstance
	{
		int LocalPlayers = 0x38;
	}UGameInstance;
	struct ULevel
	{
		int AActor = 0xA0;
	}ULevel;
	struct UPlayer
	{
		int PlayerController = 0x30;
	}ULocalPlayer;
	struct UWieldedItemComponent
	{
		int WieldedItem = 0x290;
	}UWieldedItemComponent;
	struct UWorld
	{
		int PersistentLevel = 0x30;
		int OwningGameInstance = 0x1C0;
	}UWorld;
};
extern offsets Offsets;