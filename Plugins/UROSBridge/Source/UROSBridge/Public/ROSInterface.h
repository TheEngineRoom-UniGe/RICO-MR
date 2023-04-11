// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSInterface.generated.h"

UINTERFACE(MinimalAPI)
class URosInterface : public UInterface
{
    GENERATED_BODY()
};

class UROSBRIDGE_API IRosInterface
{
  GENERATED_BODY()

public:

    TSharedPtr<FROSBridgeHandler> ROSHandler;

  virtual void Connect(const FString& InIp, const int32& InPort);
  virtual void Disconnect();

};
