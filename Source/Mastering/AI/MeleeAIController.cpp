// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeAIController.h"
#include "MasteringCharacter.h"
#include "Components/SphereComponent.h"



AMeleeAIController::AMeleeAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HearingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HearingSphere"));
	HearingSphere->InitSphereRadius(HearingRadius);
	HearingSphere->SetCollisionObjectType(ECC_Pawn);
	HearingSphere->SetCollisionProfileName("OverlapOnlyPawn");

	HearingSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAIController::OnHearingOverlap);

	bAttachToPawn = true;
}


class AMasteringCharacter* AMeleeAIController::GetTarget()
{
	return CurrentTarget;
}

void AMeleeAIController::BeginPlay()
{
	Super::BeginPlay();

	HearingSphere->AttachTo(this->GetRootComponent(), NAME_None, EAttachLocation::SnapToTarget);
}

void AMeleeAIController::OnHearingOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMasteringCharacter *Target = Cast<AMasteringCharacter>(Other);
	if (CurrentTarget != nullptr || CurrentTarget != Target)
	{
		CurrentTarget = Target;
		OnTargetChange(CurrentTarget);
	}
}

