// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MASTERING_API LoadSaveLibrary
{
public:
	LoadSaveLibrary();
	~LoadSaveLibrary();

	UFUNCTION(BlueprintCallable)
	static void LoadGameFile(FString SaveFile, UWorld* World);

	UFUNCTION(BlueprintCallable)
	static void SaveGameFile(FString SaveFile, UWorld* World);

	static void OnGameLoadedFixup(UWorld* World);

	static void FixupPlayer(UWorld* World, class AMasteringCharacter* Char);

protected:
	static TArray<uint8> BinaryData;
};
