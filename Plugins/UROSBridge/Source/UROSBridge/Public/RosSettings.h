// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RosSettings.generated.h"

/**
 *
 */
UCLASS(Config=Ros, defaultconfig, meta = (DisplayName="Ros Settings"))
class UROSBRIDGE_API URosSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
          FString ROSBridgeServerHost = TEXT("127.0.0.1");

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
      int32 ROSBridgeServerPort = 9090;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
      bool bConnectToROS = true;

};
