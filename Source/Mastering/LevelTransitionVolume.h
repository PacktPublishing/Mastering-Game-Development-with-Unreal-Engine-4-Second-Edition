// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "LevelTransitionVolume.generated.h"

/**
 * 
 */
UCLASS()
class MASTERING_API ALevelTransitionVolume : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	FORCEINLINE FVector GetPlayerSafeLocation() const
	{
		if (PlayerSafeLocation.IsZero())
		{
			UE_LOG(LogTemp, Error, TEXT("Using uninitialized player safe location on Transition Volume!"));
		}
		return PlayerSafeLocation;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TransitionLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerSafeLocation = FVector::ZeroVector;
};
