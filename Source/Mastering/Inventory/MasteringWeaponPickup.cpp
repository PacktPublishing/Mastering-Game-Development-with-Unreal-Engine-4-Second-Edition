// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringWeaponPickup.h"
#include "MasteringCharacter.h"
#include "MasteringInventory.h"


// Sets default values
AMasteringWeaponPickup::AMasteringWeaponPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMasteringWeaponPickup::HavePlayerPickup(class AMasteringCharacter* Player)
{
	UMasteringInventory *Inventory = Player->GetInventory();

	FWeaponProperties Props(WeaponClass, InventoryIcon, WeaponPower, Ammunition);

	Inventory->AddWeapon(Props);

	// here we automatically select the best weapon which may have changed after adding the above,
	// NOTE: this should probably be an option the player can turn on and off via UI
	Inventory->SelectBestWeapon();

	// and now that we've done our job, destroy ourselves
	Destroy();
}

// Called when the game starts or when spawned
void AMasteringWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMasteringWeaponPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AMasteringCharacter *player = Cast<AMasteringCharacter>(OtherActor);

	if (player == nullptr)
	{
		return;
	}
	
	HavePlayerPickup(player);
}

// Called every frame
void AMasteringWeaponPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator rotationAmount(0.0f, DeltaTime * RotationSpeed, 0.0f);

	AddActorLocalRotation(rotationAmount);
}

