// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Inventory/MasteringInventory.h"
#include "SavedActorInterface.generated.h"

/**
 * 
 */
USTRUCT()
struct FActorSavedData
{
	GENERATED_USTRUCT_BODY()

	FString MyClass;
	FTransform MyTransform;
	FVector MyVelocity;
	FName MyName;
	TArray<uint8> MyData;

	friend FArchive& operator<<(FArchive& Ar, FActorSavedData& SavedData)
	{
		Ar << SavedData.MyClass;
		Ar << SavedData.MyTransform;
		Ar << SavedData.MyVelocity;
		Ar << SavedData.MyName;
		Ar << SavedData.MyData;
		return Ar;
	}
};

USTRUCT()
struct FInventoryItemData
{
	GENERATED_USTRUCT_BODY()

	FString WeaponClass;
	int WeaponPower;
	int Ammo;
	FString TextureClass;

	friend FArchive& operator<<(FArchive& Ar, FInventoryItemData& InvItemData)
	{
		Ar << InvItemData.WeaponClass;
		Ar << InvItemData.WeaponPower;
		Ar << InvItemData.Ammo;
		Ar << InvItemData.TextureClass;
		return Ar;
	}
};

USTRUCT()
struct FInventorySaveData
{
	GENERATED_USTRUCT_BODY()

	FString CurrentWeapon;
	int CurrentWeaponPower = -1;
	TArray<FInventoryItemData> WeaponsArray;

	friend FArchive& operator<<(FArchive& Ar, FInventorySaveData& InvData)
	{
		Ar << InvData.CurrentWeapon;
		Ar << InvData.CurrentWeaponPower;
		Ar << InvData.WeaponsArray;
		return Ar;
	}
};

USTRUCT()
struct FGameSavedData
{
	GENERATED_USTRUCT_BODY()

	FDateTime Timestamp;
	FName MapName;
	FInventorySaveData InventoryData;
	TArray<FActorSavedData> SavedActors;

	friend FArchive& operator<<(FArchive& Ar, FGameSavedData& GameData)
	{
		Ar << GameData.MapName;
		Ar << GameData.Timestamp;
		Ar << GameData.InventoryData;
		Ar << GameData.SavedActors;
		return Ar;
	}
};

struct FSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
	FSaveGameArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, true)
	{
		ArIsSaveGame = true;
	}
};

UINTERFACE(BlueprintType)
class USavedActorInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ISavedActorInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Load-Save")
	void ActorLoaded();
};
