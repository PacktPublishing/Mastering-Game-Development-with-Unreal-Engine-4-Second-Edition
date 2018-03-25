// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringWeapon.h"
#include "MasteringProjectile.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"



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
