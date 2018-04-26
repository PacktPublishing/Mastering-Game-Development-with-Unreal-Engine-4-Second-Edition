// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringInventoryDisplay.h"
#include "MasteringInventory.h"


void UMasteringInventoryDisplay::Init(class UMasteringInventory* Inventory)
{
	Inventory->OnSelectedWeaponChanged.AddUObject(this, &UMasteringInventoryDisplay::WeaponSelected);
	Inventory->OnWeaponAdded.AddUObject(this, &UMasteringInventoryDisplay::WeaponAdded);
	Inventory->OnWeaponRemoved.AddUObject(this, &UMasteringInventoryDisplay::WeaponRemoved);
}
