// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ROSBridgeHandler.h"
#include <Physics/RModel.h>
#include "FROSJointConfigSubscriber.h"
#include "FROSJointTrajectorySubscriber.h"
#include "ROSCommunicationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MR_HRI_API UROSCommunicationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UROSCommunicationComponent();

	// Pointer to ROSBridge handler for ROS-UE communication
	TSharedPtr<FROSBridgeHandler> Handler;
	// Array of pointers to joint configuration subscribers
	TArray<TSharedPtr<FROSJointConfigSubscriber>> JointConfigSubscribers;
	// Array of pointers to joint trajectory subscribers
	TArray<TSharedPtr<FROSJointTrajectorySubscriber>> JointTrajectorySubscribers;
	// Array of pointers to robot models
	TArray<ARModel*> RobotModels;


	UPROPERTY(EditAnywhere, Category = "ROS Settings")
		FString ROSBridgeServerIPAddr;

	UPROPERTY(EditAnywhere, Category = "ROS Settings")
		uint32 ROSBridgeServerPort;

	UPROPERTY(EditAnywhere, Category = "Trajectory Settings")
		float ROSTrajectoryUpdateFrequency;

	UPROPERTY(EditAnywhere, Category = "Trajectory Settings")
		int ROSTrajectoryMaxSteps;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called when game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Register new robot on QR detection and expose corresponding topics
	UFUNCTION(BlueprintCallable, Category = "ModelSpawner")
	void RegisterNewRobot(UPARAM() ARModel* robot);


		
};
