#pragma once
#include "Offsets.h"
#include "Process.h"
#include "Vector.h"
#include <vector>
class text
{
public:
	char word[64];
};
class textx
{
public:
	wchar_t word[64];
};
struct FGuid
{
	int A;
	int B;
	int C;
	int D;
	FGuid() : A(0), B(0), C(0), D(0) {}
	FGuid(int a, int b, int c, int d) : A(a), B(b), C(c), D(d) {}
	bool operator==(const FGuid& other)
	{
		return A == other.A && B == other.B && C == other.C && D == other.D;
	}
	bool operator!=(const FGuid& other)
	{
		return A != other.A || B != other.B || C != other.C || D != other.D;
	}
};
struct Players
{
	Vector3 position, angles;
	float health, maxhealth;
	int distance;
	FGuid crewID, allyID;
	std::string name, heldItem;
};
struct Camera
{
	Vector3 position, angles;
	float fov;
};
struct Ships
{
	FGuid crewID;
	std::string type;
};
class cSOT
{
public:
	Players localPlayer;
	Players Pirates[24];
	Camera localCamera;
	Ships  Ships[6];
	std::vector<Vector2> XMarks;
};
template<class TEnum>
class TEnumAsByte
{
public:
	inline TEnumAsByte()
	{
	}
	inline TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}
	explicit inline TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}
	explicit inline TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}
	inline operator TEnum() const
	{
		return (TEnum)value;
	}
	inline TEnum GetValue() const
	{
		return (TEnum)value;
	}
private:
	uint8_t value;
};
enum EBootyTypes
{
	EBootyTypes__Invalid = 0,
	EBootyTypes__BountySkull = 1,
	EBootyTypes__MerchantCrate = 2,
	EBootyTypes__GunpowderBarrel = 3,
	EBootyTypes__AncientChest = 4,
	EBootyTypes__PirateLordBooty = 5,
	EBootyTypes__BoxOfSecrets = 6,
	EBootyTypes__MermaidGem = 7,
	EBootyTypes__CollectorsChest = 8,
	EBootyTypes__DroppedPouch = 9,
	EBootyTypes__Food = 10,
	EBootyTypes__TaleArtifact = 11,
	EBootyTypes__CampaignBooty = 12,
	EBootyTypes__ReapersBounty = 13,
	EBootyTypes__RitualSkull = 14,
	EBootyTypes__AshenBooty = 15,
	EBootyTypes__EmissaryFlotsam = 16,
	EBootyTypes__GoldMound = 17,
	EBootyTypes__EBootyTypes_MAX = 18
};
template<class T>
class TArray
{
public:
	int Length() const
	{
		return m_nCount;
	}
	bool IsValid() const
	{
		if (m_nCount > m_nMax)
			return false;
		if (!m_Data)
			return false;
		return true;
	}
	bool IsValidIndex(int32_t i) const
	{
		return i < m_nCount;
	}
	template<typename U = T>
	typename std::enable_if<std::is_pointer<U>::value, typename std::remove_pointer<U>::type>::type GetValue(int32_t index) const
	{
		auto offset = Mem->Read<uintptr_t>(m_Data + sizeof(uintptr_t) * index);
		return Mem->Read<typename std::remove_pointer<U>::type>(offset);
	}
	template<typename U = T>
	typename std::enable_if<!std::is_pointer<U>::value, U>::type GetValue(int32_t index) const
	{
		return Mem->Read<U>(m_Data + sizeof(U) * index);
	}
	template<typename U = T, typename std::enable_if<std::is_pointer<U>::value, int32_t>::type = 0>
	uintptr_t GetValuePtr(int32_t index) const
	{
		return Mem->Read<uintptr_t>(m_Data + sizeof(uintptr_t) * index);
	}
	template<typename U = T, typename std::enable_if<!std::is_pointer<U>::value, int32_t>::type = 0>
	uintptr_t GetValuePtr(int32_t index) const
	{
		return m_Data + sizeof(U) * index;
	}
	template<typename U = T>
	void SetValue(int32_t index, U value) const
	{
		return Mem->Write(this->GetValuePtr(index), value);
	}
	template<typename U = T>
	typename std::enable_if<std::is_pointer<U>::value, typename std::remove_pointer<U>::type>::type operator[](int32_t index) const
	{
		return GetValue<U>(index);
	}
	template<typename U = T>
	typename std::enable_if<!std::is_pointer<U>::value, U>::type operator[](int32_t index) const
	{
		return GetValue<U>(index);
	}
private:
	uintptr_t m_Data;
	int32_t m_nCount;
	int32_t m_nMax;
};
struct FQuat {
	float X;
	float Y;
	float Z;
	float W;
};
struct FTransform
{
	struct FQuat Rotation;
	struct Vector3 Translation;
	struct Vector3 Scale3D;
	unsigned char UnknownData01[0x4];
};
struct FAlliance
{
	struct FGuid AllianceId;
	TArray<struct FGuid> Crews;
	unsigned char AllianceIndex;
	unsigned char UnknownData00[0x7];
};
class AAllianceService
{
public:
	TArray<struct FAlliance> GetAlliances();
private:
	char __pad0x4B0[0x4B0];
	TArray<struct FAlliance> Alliances;
};
class Chunk
{
	char __pad0x0[0x1000];
};
class USceneComponent
{
public:
	Vector3 GetPosition();
	Vector3 GetRotation();
private:
	char __pad0x130[0x140];
	FTransform transform;
};
class APlayerState
{
public:
	std::wstring GetName();
private:
	char __pad0x0[0x1000];
};
class UHealthComponent
{
public:
	int GetHealth();
	int GetMaxHealth();
private:
	unsigned char UnknownData00[0xD8];
	float maxHealth;
	float health;
	unsigned char UnknownData01[0xC4];
};
class UItemDesc
{
public:
	std::wstring GetName();
private:
	char __pad0x0[0x28];
	uintptr_t m_pName;
};
class AItemInfo
{
public:
	UItemDesc GetItemDesc();
private:
	char __pad0x0[0x1000];
};
class AWieldableItem
{
public:
	AItemInfo GetItemInfo();
private:
	char __pad0x0[0x1000];
};
class UWieldedItemComponent
{
public:
	AWieldableItem GetWieldedItem();
public:
	char __pad0x0[0x1000];
};
class AActor
{
public:
	int GetID();
	USceneComponent GetRootComponent();
	APlayerState GetPlayerState();
	UWieldedItemComponent GetWieldedItemComponent();
	UHealthComponent GetHealthComponent();
	bool operator==(const AActor& rhs) const
	{
		return *(uintptr_t*)(this->__pad0x0 + Offsets.AActor.rootComponent) == *(uintptr_t*)(rhs.__pad0x0 + Offsets.AActor.rootComponent);
	}
	bool operator!=(const AActor& rhs) const
	{
		return *(uintptr_t*)(this->__pad0x0 + Offsets.AActor.rootComponent) != *(uintptr_t*)(rhs.__pad0x0 + Offsets.AActor.rootComponent);
	}
private:
	char __pad0x0[0x1000];
};
struct FCrew
{
public:
	TArray<class APlayerState*> GetPlayers();
	std::string GetShipType();
	FGuid GetCrewID();
private:
	FGuid CrewID;
	char __pad0x10[0x10];
	TArray<class APlayerState*> Players;
	char __pad0x30[0x30];
	int maxPlayersOnShip;
	char __pad0x64[0x1C];
};
class ACrewService
{
public:
	TArray<struct FCrew> GetCrews();
private:
	char __pad0x0[0x1000];
};
class UCrewOwnershipComponent
{
public:
	FGuid GetCrewId();
private:
	char __pad0x0[0x100];
};
class AShip
{
public:
	UCrewOwnershipComponent GetCrewOwnershipComponent();
	uintptr_t GetOwningActor();
private:
	char __pad0x0[0x1000];
};
class ULevel
{
public:
	TArray<Chunk*> GetActors() const;
private:
	char __pad0xA0[0xA0];
	TArray<Chunk*> m_Actors;
};
class AFauna
{
public:
	std::wstring GetName();
private:
	char __pad0x0[0x1000];
};
class APlayerCameraManager
{
public:
	Vector3 GetCameraPosition();
	Vector3 GetCameraRotation();
	float	GetCameraFOV();
private:
	char __pad0x0[0x4E0];
	Vector3 position;
	Vector3 rotation;
	char __pad0x10[0x10];
	float fov;
};
struct FName
{
	int nameId;
	char __pad0x4[0x4];
};
class APlayerController
{
public:
	AActor GetActor();
	APlayerCameraManager GetCameraManager();
	Vector3 GetPlayerAngles();
private:
	char __pad0x0[0x1000];
};
class ULocalPlayer
{
public:
	APlayerController GetPlayerController();
	void SetPlayerAngles(Vector3 angles);
private:
	char __pad0x0[0x1000];
};
class UGameInstance
{
public:
	ULocalPlayer GetLocalPlayer();
private:
	char __pad0x0[0x100];
};
class cUWorld
{
public:
	ULevel GetLevel()  const;
	UGameInstance GetGameInstance()  const;
public:
	char __pad0x0[0x1000];
};
struct FXMarksTheSpotMapMark
{
	struct Vector2 Position;
	float Rotation;
};
class AXMarksTheSpotMap
{
public:
	TArray<struct FXMarksTheSpotMapMark> GetMarks();
private:
	char __pad0x0[0x08E0];
	TArray<struct FXMarksTheSpotMapMark> Marks;
};
class ABootyItemInfo
{
public:
	UItemDesc GetItemDesc();
	int GetBootyType();
	int GetRareityId();
private:
	char __pad0x0[0x1000];
};
class AItemProxy
{
public:
	ABootyItemInfo  GetBootyItemInfo();
private:
	char __pad0x0[0x1000];
};
class UObject
{
public:
	char __pad0x0[0x18];
	int nameId;
};
class FWorldMapShipLocation
{
public:
	FGuid GetCrewId();
	UObject GetUObject();
private:
	FGuid crewId;
	char __pad0x10[0x18];
	uintptr_t m_pUObject;
};
class AMapTable
{
public:
	Vector2 GetServerCenter();
	TArray<struct Vector2> GetMapPins();
	TArray<class FWorldMapShipLocation> GetTrackedShips();
	TArray<struct Vector3> GetTrackedBootyItemLocations();
private:
	char __pad0x0[0x1000];
};
struct FIsland
{
	int IslandNameId;
	char __pad0x4[0x4];
	byte IslandType;
	unsigned char UnknownData00[0x7];
	char __pad0x10[0x8];
	Vector3 IslandBoundsCentre;
	float IslandBoundsRadius;
	float IslandTriggerRadius;
	float IslandSafeZoneRadius;
	char __pad0x30[0x20];
};
struct UIslandDataAssetEntry
{
public:
	int GetNameID();
	std::wstring GetName();
private:
	char __pad0x0[0x28];
	int IslandNameId;
	char __pad0x2C[0x84];
	uintptr_t IslandName;
};
class UIslandDataAsset
{
public:
	TArray<class UIslandDataAssetEntry> GetIslandDataAssetEntry();
private:
	char __pad0x0[0x48];
	TArray<class UIslandDataAssetEntry> IslandDataEntries;
};
class AIslandService
{
public:
	TArray<struct FIsland> GetIslandArray();
	UIslandDataAsset GetIslandDataAsset();
private:
	char __pad0x0[0x488];
	uintptr_t m_pIslandDataAsset;
	TArray<struct FIsland> IslandArray;
};
extern cSOT* SOT;