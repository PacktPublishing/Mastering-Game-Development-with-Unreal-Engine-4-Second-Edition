// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringInventory.h"
#include "MasteringCharacter.h"


// Sets default values for this component's properties
UMasteringInventory::UMasteringInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UMasteringInventory::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWeapon != nullptr)
	{
		AddWeapon(DefaultWeapon);
	}
}

void UMasteringInventory::SelectBestWeapon(class AMasteringCharacter *Player)
{
	for (auto WeaponIt = WeaponsArray.CreateIterator(); WeaponIt; ++WeaponIt)
	{
		//TODO: add criteria for selecting a weapon
		{
			SelectWeapon(Player, *WeaponIt);
			break;
		}
	}
}

void UMasteringInventory::SelectWeapon(class AMasteringCharacter *Player, TSubclassOf<class AMasteringWeapon> Weapon)
{
	Player->EquipWeapon(Weapon);
}

void UMasteringInventory::AddWeapon(TSubclassOf<class AMasteringWeapon> Weapon)
{
	WeaponsArray.AddUnique(Weapon);
}