// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringWeapon.h"
#include "MasteringProjectile.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MasteringInventory.h"


AMasteringWeapon::AMasteringWeapon()
{
	// Create a gun mesh component
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetOnlyOwnerSee(true); // only the owning player will see this mesh
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;
	WeaponMesh->SetupAttachment(RootComponent);

	// Our muzzle offset object
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WeaponMesh);
}

void AMasteringWeapon::Fire(FRotator ControlRotation, class UAnimInstance* AnimInst, class UMasteringInventory* Inventory)
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + ControlRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.Instigator = Cast<APawn>(GetOwner());
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AMasteringProjectile>(ProjectileClass, SpawnLocation, ControlRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		if (AnimInst != nullptr)
		{
			AnimInst->Montage_Play(FireAnimation, 1.f);
		}
	}

	// reduce ammo by one
	Inventory->ChangeAmmo(GetClass(), -1);
}
