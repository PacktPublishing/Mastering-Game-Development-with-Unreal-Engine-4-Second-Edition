// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MASTERING_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void LoadGame(FString SaveFile);

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void Open();

	UFUNCTION(BlueprintCallable)
	void Close();

	UFUNCTION(BlueprintCallable)
	void PopulateSaveFiles();

	void OnGameLoadedFixup(UWorld* World);
	static TArray<uint8> BinaryData;

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> SaveFileNames;
};
