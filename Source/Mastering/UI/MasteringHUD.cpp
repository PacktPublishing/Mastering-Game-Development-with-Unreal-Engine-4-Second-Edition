// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MasteringHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "LoadSaveLibrary.h"
#include "MasteringInventoryDisplay.h"
#include "MainMenuWidget.h"

AMasteringHUD::AMasteringHUD()
{

}

void AMasteringHUD::BeginPlay()
{
	if (InventoryClass != nullptr)
	{
		InventoryHUD = CreateWidget<UMasteringInventoryDisplay>(GetOwningPlayerController(), InventoryClass);
		checkSlow(InventoryHUD != nullptr);

		InventoryHUD->AddToViewport();
	}

	if (MainMenuClass != nullptr)
	{
		MainMenu = CreateWidget<UMainMenuWidget>(GetOwningPlayerController(), MainMenuClass);
		checkSlow(MainMenu != nullptr);

		MainMenu->AddToViewport();

		LoadSaveLibrary::OnGameLoadedFixup(GetWorld());
		MainMenu->Close();
	}
}

void AMasteringHUD::DrawHUD()
{
	if (bNeedsInventoryInit && Inventory != nullptr)
	{
		if (InventoryHUD != nullptr)
		{
			InventoryHUD->Init(Inventory);
		}

		// Equip our best weapon on startup
		Inventory->AddDefaultWeapon();
		Inventory->SelectBestWeapon();

		bNeedsInventoryInit = false;
	}

	Super::DrawHUD();

	// Draw very simple crosshair
	if (CrosshairTex != nullptr)
	{
		// find center of the Canvas
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
		const FVector2D CrosshairDrawPosition((Center.X),
			(Center.Y + 20.0f));

		// draw the crosshair
		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

void AMasteringHUD::InitializeInventory(class UMasteringInventory* PlayerInventory)
{
	Inventory = PlayerInventory;
	if (InventoryHUD != nullptr)
	{
		InventoryHUD->Init(Inventory);
	}
}

void AMasteringHUD::ToggleMainMenu()
{
	if (MainMenu != nullptr)
	{
		if (MainMenu->GetVisibility() == ESlateVisibility::Visible)
			MainMenu->Close();
		else
			MainMenu->Open();
	}
}
