// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MasteringIconCamera.generated.h"

/**
 * 
 */
UCLASS()
class MASTERING_API AMasteringIconCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:

	virtual void TakeShot();
	virtual void SpawnAndPlaceNextActor();
	virtual FVector ComputeActorLocation();
	virtual void OnFinishedLoadingAssets();

	UPROPERTY(Transient)
	TArray<FSoftObjectPath> WeaponBlueprintSoftRefs;
	UPROPERTY(Transient)
	TArray<class UBlueprint*> WeaponBlueprints;

	UPROPERTY(Transient)
	class UBlueprint* CurrentWeaponBlueprint = nullptr;
	UPROPERTY(Transient)
	class AMasteringWeaponPickup* CurrentWeaponPickup = nullptr;
	UPROPERTY(Transient)
	class UCameraComponent* CameraComp;
	UPROPERTY(Transient)
	bool bInitialied = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponsPath = "FirstPersonCPP/Blueprints/Weapons";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShotDelay = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScreenshotResolutionX = 256;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScreenshotResolutionY = 256;

	int CurrentWeaponIndex = 0;
};
