// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringInventory.h"
#include "MasteringCharacter.h"
#include "MasteringWeaponPickup.h"

#define UNLIMITED_AMMO -1

// Sets default values for this component's properties
UMasteringInventory::UMasteringInventory()
{
	PrimaryComponentTick.bCanEverTick = true;

	MyOwner = Cast<AMasteringCharacter>(GetOwner());
	check(GetOwner() == nullptr || MyOwner != nullptr);
}

void UMasteringInventory::AddDefaultWeapon()
{
	if (DefaultWeaponPickup != nullptr)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// spawn a pickup and immediately add it to our player
		AMasteringWeaponPickup* defaultPickup = GetWorld()->SpawnActor<AMasteringWeaponPickup>(DefaultWeaponPickup, FVector(0.0f), FRotator(0.0f), ActorSpawnParams);
		defaultPickup->HavePlayerPickup(MyOwner);
	}
}

void UMasteringInventory::SelectBestWeapon()
{
	int highestWeaponPower = CurrentWeaponPower;
	FWeaponProperties bestWeapon;

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
			bestWeapon = currentProps;
		}
	}

	if (bestWeapon.WeaponClass != nullptr)
	{
		SelectWeapon(bestWeapon);
	}
}

void UMasteringInventory::SelectWeapon(FWeaponProperties Weapon)
{
	OnSelectedWeaponChanged.Broadcast(Weapon);

	MyOwner->EquipWeapon(Weapon.WeaponClass);
	CurrentWeapon = Weapon.WeaponClass;
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

	if (currentIndex >= WeaponsArray.Num() - 1) // we're at the end
	{
		SelectWeapon(WeaponsArray[0]);
	}
	else if (WeaponsArray.Num() > 0)
	{
		SelectWeapon(WeaponsArray[currentIndex + 1]);
	}
}

void UMasteringInventory::SelectPreviousWeapon()
{
	int currentIndex = FindCurrentWeaponIndex();

	if (currentIndex > 0) // we're not at the start
	{
		SelectWeapon(WeaponsArray[currentIndex - 1]);
	}
	else
	{
		SelectWeapon(WeaponsArray[WeaponsArray.Num() - 1]); // select the last
	}
}

void UMasteringInventory::AddWeapon(const FWeaponProperties &Properties)
{
	int foundIndex = WeaponsArray.Find(Properties);
	if (foundIndex != INDEX_NONE)
	{
		FWeaponProperties &currentProps = WeaponsArray[foundIndex];
		checkSlow(Properties.Ammo >= 0 || Properties.Ammo == UNLIMITED_AMMO);
		currentProps.Ammo += Properties.Ammo;
	}
	else
	{
		WeaponsArray.Add(Properties);
		OnWeaponAdded.Broadcast(Properties);
	}
}

void UMasteringInventory::ChangeAmmo(TSubclassOf<class AMasteringWeapon> Weapon, const int ChangeAmount)
{
	for (int i = 0; i < WeaponsArray.Num(); ++i)
	{
		FWeaponProperties &currentProps = WeaponsArray[i];
		if (currentProps.WeaponClass == Weapon)
		{
			if (currentProps.Ammo == UNLIMITED_AMMO) // unlimited ammo gun, we're done
				return;

			currentProps.Ammo = FMath::Clamp(currentProps.Ammo + ChangeAmount, 0, 999);
			if (currentProps.Ammo == 0) // gun is now empty!
			{
				CurrentWeaponPower = -1;
				OnWeaponRemoved.Broadcast(currentProps);
				WeaponsArray.RemoveAt(i);
				SelectBestWeapon();
			}
			return; // our work is done if we found the gun already in inventory, just update ammo
		}
	}
}
