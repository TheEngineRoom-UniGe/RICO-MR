// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasyKafkaSubsystem.h"
#include "Components/ActorComponent.h"
#include <trajectory_msgs/JointTrajectory.h>
#include "RobotIKJointsPublisher.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MR_HRI_API URobotIKJointsPublisher : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URobotIKJointsPublisher();

	// Method used to publish player camera's pose through kafka
	UFUNCTION(BlueprintCallable)
	void PublishJoints(UPARAM() FString Topic, UPARAM() TArray<float> JointValues);

	//UFUNCTION(BlueprintCallable)
	//void SetGripperState(UPARAM() int& NewState);
};
