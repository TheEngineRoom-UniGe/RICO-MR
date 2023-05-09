// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Physics/RModel.h>
#include "TestRobotJointConfiguration.generated.h"

UCLASS()
class MR_HRI_API ATestRobotJointConfiguration : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestRobotJointConfiguration();
	
	UFUNCTION(BlueprintCallable)
	void TestJointConfig(UPARAM() ARModel* Robot, UPARAM() TArray<float> JointValues);
};
