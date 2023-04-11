// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ROSSubsystem.generated.h"

/**
   Abstract Subsystem that lives with the GameInstance and is tickable.
 */
UCLASS(Abstract)
class UROSBRIDGE_API UROSGameInstanceSubsystem : public UGameInstanceSubsystem, public FTickableGameObject, public IRosInterface
{
	GENERATED_BODY()

public:
    // Begin USubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // End USubsystem
protected:

    /* FTickableGameObject interface */
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;
};

/**
   Abstract Subsystem that lives with the UWorld and is tickable.
 */
UCLASS(Abstract)
class UROSBRIDGE_API UROSWorldSubsystem : public UTickableWorldSubsystem, public IRosInterface
{
	GENERATED_BODY()

public:
    // Begin USubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // End USubsystem

protected:

    /* FTickableGameObject interface */
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;
};
