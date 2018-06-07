// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MasteringGameInstance.generated.h"

/**
*
*/
UCLASS()
class MASTERING_API UMasteringGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Mastering Game Instance")
	static UMasteringGameInstance* GetInstance();

	virtual void Init() override;
	virtual void BeginDestroy() override;
	virtual void FinishDestroy() override;

	bool ShouldPersistInventory() const
	{
		return bPersistPlayerInventory;
	}

	void SetPersistInventory(const bool bPersist = true)
	{
		bPersistPlayerInventory = bPersist;
	}

protected:

	bool bPersistPlayerInventory;
};
