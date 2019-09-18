// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeAIController.h"
#include "MasteringCharacter.h"
#include "StealthCharacter.h"
#include "Components/SphereComponent.h"



AMeleeAIController::AMeleeAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HearingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HearingSphere"));
	HearingSphere->SetCollisionObjectType(ECC_Pawn);
	HearingSphere->SetCollisionProfileName("Trigger");
	HearingSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAIController::OnHearingOverlap);

	StealthHearingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("StealthHearingSphere"));
	StealthHearingSphere->SetCollisionObjectType(ECC_Pawn);
	StealthHearingSphere->SetCollisionProfileName("Trigger");
	StealthHearingSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAIController::OnStealthHearingOverlap);

	SightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SightSphere"));
	SightSphere->SetCollisionObjectType(ECC_Pawn);
	SightSphere->SetCollisionProfileName("Trigger");
	SightSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAIController::OnSightOverlap);

	SetReturningHome();

	bAttachToPawn = true;
}

void AMeleeAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	HomeLocation = GetPawn()->GetNavAgentLocation();

	HearingSphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	StealthHearingSphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SightSphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	OnReturnedHome();
}

class AMasteringCharacter* AMeleeAIController::GetTarget()
{
	return CurrentTarget;
}

void AMeleeAIController::SetReturningHome()
{
	HearingSphere->SetSphereRadius(0.0f);
	StealthHearingSphere->SetSphereRadius(0.0f);
	SightSphere->SetSphereRadius(0.0f);

	CurrentTarget = nullptr;
}

void AMeleeAIController::OnReturnedHome()
{
	HearingSphere->SetSphereRadius(HearingRadius);
	StealthHearingSphere->SetSphereRadius(StealthHearingRadius);
	SightSphere->SetSphereRadius(SightRadius);
}

void AMeleeAIController::OnHearingOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AStealthCharacter* StealthChar = Cast<AStealthCharacter>(Other);
	if (StealthChar != nullptr)
	{
		if (StealthChar->IsStealthed())
		{
			return; // we let the stealthed sphere deal with these
		}
	}

	SetPotentialTarget(Other);
}

void AMeleeAIController::OnStealthHearingOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetPotentialTarget(Other);
}

void AMeleeAIController::OnSightOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* OwnerPawn = GetPawn();

	if (OwnerPawn == nullptr || OwnerPawn == Other)
	{
		return;
	}

	FVector ToTarget = Other->GetActorLocation() - OwnerPawn->GetActorLocation();
	FVector Facing = GetPawn()->GetActorForwardVector();

	if (SightAngle > 90.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Sight Angles of 90+ degrees not supported, please use hearing for this detection!"));
		SightAngle = 90.0f;
	}

	if (FVector::DotProduct(ToTarget, Facing) < 0.0f)
	{
		return;
	}

	float DotToTarget = FVector::DotProduct(ToTarget.GetSafeNormal(), Facing.GetSafeNormal());
	float RadiansToTarget = FMath::Acos(DotToTarget);
	float AngToTarget = RadiansToTarget * 180.0f / PI;

	if (AngToTarget < SightAngle)
	{
		SetPotentialTarget(Other);
	}
}

void AMeleeAIController::SetPotentialTarget(AActor* Other)
{
	AMasteringCharacter* Target = Cast<AMasteringCharacter>(Other);
	if (Target != nullptr && CurrentTarget != Target)
	{
		CurrentTarget = Target;
		OnTargetChange(CurrentTarget);
	}
}

