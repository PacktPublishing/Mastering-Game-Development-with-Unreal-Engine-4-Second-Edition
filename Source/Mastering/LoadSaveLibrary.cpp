// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadSaveLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "LoadSaveLibrary.h"
#include "UI/SavedActorInterface.h"

#include "MasteringCharacter.h"

TArray<uint8> LoadSaveLibrary::BinaryData;

LoadSaveLibrary::LoadSaveLibrary()
{
}

LoadSaveLibrary::~LoadSaveLibrary()
{
}

void LoadSaveLibrary::LoadGameFile(FString SaveFile, UWorld* World)
{
	FString outPath = FPaths::ProjectSavedDir() + SaveFile;
	if (!FFileHelper::LoadFileToArray(BinaryData, *outPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(FString("Game Load Failed: ") + outPath));
		return;
	}

	checkSlow(World != nullptr);
	FMemoryReader FromBinary = FMemoryReader(BinaryData, true);
	FromBinary.Seek(0);

	FGameSavedData SaveGameData;
	FromBinary << SaveGameData;

	FromBinary.FlushCache();
	FromBinary.Close();

	FString mapName = SaveGameData.MapName.ToString();

	FString currentMapName = World->GetMapName();

	currentMapName.Split("_", nullptr, &currentMapName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	if (mapName == currentMapName)
	{
		World->ServerTravel("?Restart", true);
	}
	else
	{
		UGameplayStatics::OpenLevel(World, *mapName, true);
	}
}

void LoadSaveLibrary::SaveGameFile(FString SaveFile, UWorld* World)
{
	checkSlow(World != nullptr);
	FGameSavedData SaveGameData;

	SaveGameData.Timestamp = FDateTime::Now();

	FString mapName = World->GetMapName();

	mapName.Split("_", nullptr, &mapName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	SaveGameData.MapName = *mapName;

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(World, USavedActorInterface::StaticClass(), Actors);

	TArray<FActorSavedData> SavedActors;
	for (auto Actor : Actors)
	{
		FActorSavedData ActorRecord;
		ActorRecord.MyName = FName(*Actor->GetName());
		ActorRecord.MyClass = Actor->GetClass()->GetPathName();
		ActorRecord.MyTransform = Actor->GetTransform();
		ActorRecord.MyVelocity = Actor->GetVelocity();

		FMemoryWriter MemoryWriter(ActorRecord.MyData, true);
		FSaveGameArchive Ar(MemoryWriter);
		AMasteringCharacter* Mast = Cast<AMasteringCharacter>(Actor);

		ISavedActorInterface::Execute_ActorSaved(Actor);

		Actor->Serialize(Ar);

		if (Mast != nullptr)
		{
			UMasteringInventory* Inv = Mast->GetInventory();
			SaveGameData.InventoryData.CurrentWeapon = Inv->GetCurrentWeapon()->GetPathName();
			SaveGameData.InventoryData.CurrentWeaponPower = Inv->GetCurrentWeaponPower();
			for (FWeaponProperties weapon : Inv->GetWeaponsArray())
			{
				FInventoryItemData data;
				data.WeaponClass = weapon.WeaponClass->GetPathName();
				data.WeaponPower = weapon.WeaponPower;
				data.Ammo = weapon.Ammo;
				data.TextureClass = weapon.InventoryIcon->GetPathName();

				SaveGameData.InventoryData.WeaponsArray.Add(data);
			}
		}

		SavedActors.Add(ActorRecord);
	}

	FBufferArchive SaveData;

	SaveGameData.SavedActors = SavedActors;

	SaveData << SaveGameData;

	FFileHelper::SaveArrayToFile(SaveData, *SaveFile);

	SaveData.FlushCache();
	SaveData.Empty();
}

void LoadSaveLibrary::OnGameLoadedFixup(UWorld* World)
{
	if (BinaryData.Num() == 0)
	{
		return;
	}

	FMemoryReader FromBinary = FMemoryReader(BinaryData, true);
	FromBinary.Seek(0);

	FGameSavedData SaveGameData;
	FromBinary << SaveGameData;

	FromBinary.FlushCache();
	BinaryData.Empty();
	FromBinary.Close();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(World, USavedActorInterface::StaticClass(), Actors);

	TArray<FActorSavedData> ActorDatas = SaveGameData.SavedActors;

	AMasteringCharacter* Char = nullptr; // if ever more than one, we'll need an array and a map to their inventory

										 // iterate these arrays backwards as we will remove objects as we go, can also use iterators, but RemoveAt is simpler here for now
	for (int i = Actors.Num() - 1; i >= 0; --i)
	{
		AActor* Actor = Actors[i];

		for (int j = ActorDatas.Num() - 1; j >= 0; --j)
		{
			FActorSavedData ActorRecord = ActorDatas[j];

			// These are actors spawned into the world that we also found in our save data (TODO: use TArray intersection?)
			if (ActorRecord.MyName == *Actor->GetName())
			{
				FMemoryReader MemoryReader(ActorRecord.MyData, true);
				FSaveGameArchive Ar(MemoryReader);

				AMasteringCharacter* Mast = Cast<AMasteringCharacter>(Actor);
				if (Mast != nullptr)
					Char = Mast;

				Actor->Serialize(Ar);
				Actor->SetActorTransform(ActorRecord.MyTransform);
				ISavedActorInterface::Execute_ActorLoaded(Actor);

				APawn* pawn = Cast<APawn>(Actor);
				if (pawn != nullptr)
				{
					// controller needs the rotation too as this may set the pawn's rotation once play starts
					AController* controller = pawn->GetController();
					controller->ClientSetRotation(ActorRecord.MyTransform.Rotator());
				}

				ActorDatas.RemoveAt(j);
				Actors.RemoveAt(i);
				break;
			}
		}
	}

	// Assuming we found our player character and saved out some inventory, this is where we do its custom serialization and fix-up
	if (Char != nullptr && SaveGameData.InventoryData.WeaponsArray.Num() > 0)
	{
		UMasteringInventory* NewInv = NewObject<UMasteringInventory>(Char, TEXT("PlayerInventory"), RF_Transient);

		Char->SetInventory(NewInv);

		FWeaponProperties propsEquipped;
		for (FInventoryItemData ItemData : SaveGameData.InventoryData.WeaponsArray)
		{
			FWeaponProperties props;
			props.WeaponClass = FindObject<UClass>(ANY_PACKAGE, *ItemData.WeaponClass);
			props.InventoryIcon = FindObject<UTexture2D>(ANY_PACKAGE, *ItemData.TextureClass);
			props.WeaponPower = ItemData.WeaponPower;
			props.Ammo = ItemData.Ammo;

			if (ItemData.WeaponClass == SaveGameData.InventoryData.CurrentWeapon)
				propsEquipped = props;

			NewInv->AddWeapon(props);
		}

		Char->GetInventory()->SelectWeapon(propsEquipped);
	}

	// These are actors in our save data, but not in the world, spawn them
	for (FActorSavedData ActorRecord : ActorDatas)
	{
		FVector SpawnPos = ActorRecord.MyTransform.GetLocation();
		FRotator SpawnRot = ActorRecord.MyTransform.Rotator();
		FActorSpawnParameters SpawnParams;
		// if we were in a space when saved, we should be able to spawn there again when loaded, but we could also
		// be overlapping an object that loaded, but will be subsequently destroyed below as it was there at level start
		// but not there at save time
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Name = ActorRecord.MyName;
		UClass* SpawnClass = FindObject<UClass>(ANY_PACKAGE, *ActorRecord.MyClass);
		if (SpawnClass)
		{
			AActor* NewActor = GWorld->SpawnActor(SpawnClass, &SpawnPos, &SpawnRot, SpawnParams);
			FMemoryReader MemoryReader(ActorRecord.MyData, true);
			FSaveGameArchive Ar(MemoryReader);
			NewActor->Serialize(Ar);
			NewActor->SetActorTransform(ActorRecord.MyTransform);
			ISavedActorInterface::Execute_ActorLoaded(NewActor);
		}
	}

	// These are actors in the world that are not in our save data, destroy them (for example, a weapon pickup that was, well, picked up)
	for (auto Actor : Actors)
	{
		Actor->Destroy();
	}
}
