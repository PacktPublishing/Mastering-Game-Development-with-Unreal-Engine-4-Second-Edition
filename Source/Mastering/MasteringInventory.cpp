// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringInventory.h"
#include "MasteringCharacter.h"

#define UNLIMITED_AMMO -1

// Sets default values for this component's properties
UMasteringInventory::UMasteringInventory()
{
	PrimaryComponentTick.bCanEverTick = true;

	MyOwner = Cast<AMasteringCharacter>(GetOwner());
	check(GetOwner() == nullptr || MyOwner != nullptr);
}

// Called when the game starts
void UMasteringInventory::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWeapon != nullptr)
	{
		// NOTE: since we don't use a pick-up for the default weapon, we always give it a power of 0
		AddWeapon(DefaultWeapon, UNLIMITED_AMMO, 0);
	}
}

void UMasteringInventory::SelectBestWeapon()
{
	int highestWeaponPower = CurrentWeaponPower;
	TSubclassOf<class AMasteringWeapon> bestWeapon = nullptr;

	for (auto WeaponIt = WeaponsArray.CreateConstIterator(); WeaponIt; ++WeaponIt)
	{
		const FWeaponProperties &currentProps = *WeaponIt;

		// skip any weapons that don't have ammo, ammo of -1 is a special case meaning it has unlimited ammo
		if (currentProps.Ammo == 0)
			continue;

		if (currentProps.WeaponPower > highestWeaponPower)
		{
			highestWeaponPower = currentProps.WeaponPower;
			CurrentWeaponPower = highestWeaponPower;
			bestWeapon = currentProps.WeaponClass;
		}
	}

	if (bestWeapon != nullptr)
	{
		SelectWeapon(bestWeapon);
	}
}

void UMasteringInventory::SelectWeapon(TSubclassOf<class AMasteringWeapon> Weapon)
{
	MyOwner->EquipWeapon(Weapon);
	CurrentWeapon = Weapon;
}

int UMasteringInventory::FindCurrentWeaponIndex() const
{
	int currentIndex = 0;
	for (auto WeaponIt = WeaponsArray.CreateConstIterator(); WeaponIt; ++WeaponIt, ++currentIndex)
	{
		const FWeaponProperties &currentProps = *WeaponIt;
		if (currentProps.WeaponClass == CurrentWeapon)
			break;
	}

	checkSlow(currentIndex < WeaponsArray.Num());

	return currentIndex;
}

void UMasteringInventory::SelectNextWeapon()
{
	int currentIndex = FindCurrentWeaponIndex();

	if (currentIndex == WeaponsArray.Num() - 1) // we're at the end
	{
		SelectWeapon(WeaponsArray[0].WeaponClass);
	}
	else
	{
		SelectWeapon(WeaponsArray[currentIndex + 1].WeaponClass);
	}
}

void UMasteringInventory::SelectPreviousWeapon()
{
	int currentIndex = FindCurrentWeaponIndex();

	if (currentIndex > 0) // we're not at the start
	{
		SelectWeapon(WeaponsArray[currentIndex - 1].WeaponClass);
	}
	else
	{
		SelectWeapon(WeaponsArray[WeaponsArray.Num() - 1].WeaponClass); // select the last
	}
}

void UMasteringInventory::AddWeapon(TSubclassOf<class AMasteringWeapon> Weapon, int AmmoCount, uint8 WeaponPower)
{
	for (auto WeaponIt = WeaponsArray.CreateIterator(); WeaponIt; ++WeaponIt)
	{
		FWeaponProperties &currentProps = *WeaponIt;
		if (currentProps.WeaponClass == Weapon)
		{
			checkSlow(AmmoCount >= 0);
			currentProps.Ammo += AmmoCount;
			return; // our work is done if we found the gun already in inventory, just update ammo
		}
	}

	FWeaponProperties weaponProps;
	weaponProps.WeaponClass = Weapon;
	weaponProps.WeaponPower = WeaponPower;
	weaponProps.Ammo = AmmoCount;

	WeaponsArray.Add(weaponProps);
}

void UMasteringInventory::ChangeAmmo(TSubclassOf<class AMasteringWeapon> Weapon, const int ChangeAmount)
{
	for (auto WeaponIt = WeaponsArray.CreateIterator(); WeaponIt; ++WeaponIt)
	{
		FWeaponProperties &currentProps = *WeaponIt;
		if (currentProps.WeaponClass == Weapon)
		{
			if (currentProps.Ammo == UNLIMITED_AMMO) // unlimited ammo gun, we're done
				return;

			currentProps.Ammo = FMath::Clamp(currentProps.Ammo + ChangeAmount, 0, 999);
			if (currentProps.Ammo == 0) // gun is now empty!
			{
				CurrentWeaponPower = -1; // force us to select any better weapon that does have ammo
				SelectBestWeapon();
			}
			return; // our work is done if we found the gun already in inventory, just update ammo
		}
	}
}
