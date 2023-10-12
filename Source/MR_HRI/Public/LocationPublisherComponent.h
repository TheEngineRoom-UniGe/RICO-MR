// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasyKafkaSubsystem.h"
#include <geometry_msgs/Point.h>
#include "Components/ActorComponent.h"
#include "LocationPublisherComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MR_HRI_API ULocationPublisherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULocationPublisherComponent();

	// Method used to publish player camera's pose through kafka
	UFUNCTION(BlueprintCallable)
	void PublishLocation(UPARAM() FString Topic, UPARAM() FVector Offset);
	
};
