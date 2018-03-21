// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthCharacter.h"



void AStealthCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Bind jump events
	PlayerInputComponent->BindAction("Stealth", IE_Pressed, this, &AStealthCharacter::Stealth);
	PlayerInputComponent->BindAction("Stealth", IE_Released, this, &AStealthCharacter::UnStealth);

	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AStealthCharacter::AddControllerPitchInput(float Val)
{
	const float fScale = bIsStealthed ? StealthPitchYawScale : 1.0f;

	Super::AddControllerPitchInput(Val * fScale);
}

void AStealthCharacter::AddControllerYawInput(float Val)
{
	const float fScale = bIsStealthed ? StealthPitchYawScale : 1.0f;

	Super::AddControllerYawInput(Val * fScale);
}

void AStealthCharacter::Stealth()
{
	bIsStealthed = true;
	Super::Crouch();
}

void AStealthCharacter::UnStealth()
{
	bIsStealthed = false;
	Super::UnCrouch();
}
