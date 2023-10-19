// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MRGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MR_HRI_API AMRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMRGameModeBase();
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;
};
