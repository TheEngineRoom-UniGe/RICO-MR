// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <physics/RModel.h>
#include "GameFramework/Actor.h"
#include "RobotPoseInitializer.generated.h"

UCLASS()
class MR_HRI_API ARobotPoseInitializer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobotPoseInitializer();

	// Apply result of IK to target robot model
	UFUNCTION(BlueprintCallable)
	void SetRobotPose(UPARAM() ARModel* Robot, UPARAM() TArray<FString> JointNames, UPARAM() TArray<float> JointValues);
};
