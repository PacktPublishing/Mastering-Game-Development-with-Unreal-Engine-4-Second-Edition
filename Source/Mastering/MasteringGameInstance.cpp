// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringGameInstance.h"
#include "Inventory/MasteringInventory.h"



static UMasteringGameInstance* Instance = nullptr;

UMasteringGameInstance* UMasteringGameInstance::GetInstance()
{
	checkf(Instance != nullptr, TEXT("Someone is trying to use the game instance before it has initialized!"));

	return Instance;
}

void UMasteringGameInstance::Init()
{
	Super::Init();

	Instance = this;

	AddToRoot();
}

void UMasteringGameInstance::BeginDestroy()
{
	ClearData();
	Super::BeginDestroy();
}

void UMasteringGameInstance::FinishDestroy()
{
	Super::FinishDestroy();
}

void UMasteringGameInstance::SetPlayerInventory(class UMasteringInventory* Inv)
{
	if (PlayerInv == nullptr)
	{
		PlayerInv = NewObject<UMasteringInventory>(this, TEXT("PlayerInventory"));
	}

	PlayerInv->CopyFromOther(Inv);
}

void UMasteringGameInstance::ClearData()
{
	bPersistPlayerInventory = false;
	PlayerInv = nullptr;
	PlayerSafeLocation = FVector::ZeroVector;
}
