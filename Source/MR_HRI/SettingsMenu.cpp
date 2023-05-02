// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenu.h"

// Sets default values
ASettingsMenu::ASettingsMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASettingsMenu::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASettingsMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASettingsMenu::BroadCastConnectionEvent(UPARAM() FString ConnectionAddr) {
	OnConnectionButtonPressed.Broadcast(ConnectionAddr);
}

