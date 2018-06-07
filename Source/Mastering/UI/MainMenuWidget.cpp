// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "LoadSaveLibrary.h"
#include "SavedActorInterface.h"
#include "SavedActorInterface.h"

#include "MasteringCharacter.h"

void UMainMenuWidget::LoadGame(FString SaveFile)
{
	UWorld *World = GetWorld();

	LoadSaveLibrary::LoadGameFile(SaveFile, World);
}

void UMainMenuWidget::SaveGame()
{
	FString outPath = FPaths::ProjectSavedDir() + FDateTime::Now().ToString() + TEXT(".sav");

	UWorld *World = GetWorld();

	LoadSaveLibrary::SaveGameFile(outPath, World);

	Close();
}

void UMainMenuWidget::Open()
{
	checkSlow(GetVisibility() == ESlateVisibility::Hidden); // only want to open from closed
	SetVisibility(ESlateVisibility::Visible);

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* playerController = World->GetFirstPlayerController();
		if (playerController)
		{
			playerController->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			playerController->SetInputMode(InputMode);
			UGameplayStatics::SetGamePaused(this, true);
		}
	}
}

void UMainMenuWidget::Close()
{
	checkSlow(GetVisibility() == ESlateVisibility::Visible); // only want to closed from open
	SetVisibility(ESlateVisibility::Hidden);

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* playerController = World->GetFirstPlayerController();
		if (playerController)
		{
			playerController->bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			playerController->SetInputMode(InputMode);
			UGameplayStatics::SetGamePaused(this, false);
		}
	}
}

void UMainMenuWidget::PopulateSaveFiles()
{
	FString dir = FPaths::ProjectSavedDir();
	FString fileExt = TEXT("sav");

	IFileManager::Get().FindFiles(SaveFileNames, *dir, *fileExt);
}
