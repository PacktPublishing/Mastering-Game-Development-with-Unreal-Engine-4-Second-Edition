// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasteringCharacter.h"
#include "StealthCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MASTERING_API AStealthCharacter : public AMasteringCharacter
{
	GENERATED_BODY()
	
public:
	/** Modifier to our turn and pitch rate when in stealth mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float StealthPitchYawScale = 0.5f;
};
