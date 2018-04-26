// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MasteringHUD.generated.h"

UCLASS()
class AMasteringHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMasteringHUD();

	virtual void BeginPlay() override;

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void InitializeInventory(class UMasteringInventory* PlayerInventory);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMasteringInventoryDisplay> InventoryClass;

	class UMasteringInventoryDisplay* InventoryHUD;
	class UMasteringInventory* Inventory;

	bool bNeedsInventoryInit = true;
};

