// Fill out your copyright notice in the Description page of Project Settings.
#include "ROSSubsystem.h"
#include "RosSettings.h"

void UROSGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);
  const URosSettings* RosSettings = GetDefault<URosSettings>();
  if (RosSettings->bConnectToROS)
    {
      Connect(RosSettings->ROSBridgeServerHost, RosSettings->ROSBridgeServerPort);
    }
  else
    {
      UE_LOG(LogROS, Warning, TEXT("ConnectToROS is false in RosSettings. Don't connect to rosbridge..."));
    }
}

void UROSGameInstanceSubsystem::Deinitialize()
{
  Disconnect();
  Super::Deinitialize();
}

void UROSGameInstanceSubsystem::Tick(float DeltaTime)
{
  if (ROSHandler.IsValid())
    {
      ROSHandler->Process();
    }
}

bool UROSGameInstanceSubsystem::IsTickable() const
{
  return true;
}

TStatId UROSGameInstanceSubsystem::GetStatId() const
{
  return Super::GetStatID();
}

void UROSWorldSubsystem::Tick(float DeltaTime)
{
  if (ROSHandler.IsValid())
    {
      ROSHandler->Process();
    }
}

TStatId UROSWorldSubsystem::GetStatId() const
{
  return Super::GetStatID();
}

bool UROSWorldSubsystem::IsTickable() const
{
  return true;
}

void UROSWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);

  const URosSettings* RosSettings = GetDefault<URosSettings>();
  if(!GIsEditor || GIsPlayInEditorWorld)
    {
      UE_LOG(LogROS, Warning, TEXT("init ros worldsubsystem"));
      if (RosSettings->bConnectToROS)
        {
          Connect(RosSettings->ROSBridgeServerHost, RosSettings->ROSBridgeServerPort);
        }
      else
        {
          UE_LOG(LogROS, Warning, TEXT("ConnectToROS is false in RosSettings. Don't connect to rosbridge..."));
        }
    }
}

void UROSWorldSubsystem::Deinitialize()
{
  Disconnect();
  Super::Deinitialize();
}
