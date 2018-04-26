// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/MasteringInventory.h"
#include "MasteringInventoryDisplay.generated.h"

/**
 * 
 */
UCLASS()
class MASTERING_API UMasteringInventoryDisplay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init(class UMasteringInventory* Inventory);

	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void WeaponSelected(FWeaponProperties Weapon);

	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void WeaponAdded(FWeaponProperties Weapon);

	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void WeaponRemoved(FWeaponProperties Weapon);
};
