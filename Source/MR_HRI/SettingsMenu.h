// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SettingsMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionButtonPressed, FString, ConnectionAddr);

UCLASS()
class MR_HRI_API ASettingsMenu : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintAssignable)
	FOnConnectionButtonPressed OnConnectionButtonPressed;

	// Sets default values for this actor's properties
	ASettingsMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Invoked when button is pressed to trigger the corresponding action in game pawn
	UFUNCTION(BlueprintCallable)
	void BroadCastConnectionEvent(UPARAM() FString ConnectionAdrr);

};
