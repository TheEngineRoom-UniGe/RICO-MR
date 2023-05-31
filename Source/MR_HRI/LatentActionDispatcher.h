#pragma once

#include "CoreMinimal.h"
#include "Json.h"
#include "Components/ActorComponent.h"
#include <Physics/RModel.h>

#include <geometry_msgs/Pose.h>
#include <nav_msgs/Path.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>

#include "JointTrajectoryLatentAction.h"
#include "NavigationLatentAction.h"

#include "LatentActionDispatcher.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MR_HRI_API ULatentActionDispatcher : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, Category = "Trajectory Settings")
		float JointTrajectoryUpdateFrequency = 0.025f;

	UPROPERTY(EditAnywhere, Category = "Trajectory Settings")
		float NavPathUpdateFrequency = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Trajectory Settings")
		int TrajectoryMaxSteps = 50;

	UPROPERTY()
		FTransform BaseTransform;

	// Sets default values for this component's properties
	ULatentActionDispatcher();

	// Method to Set Base Transform from blueprint
	UFUNCTION(BlueprintCallable)
	void SetBaseTransform(UPARAM() FTransform BaseTransf);

	// Method acting as dispatcher for robot action depending on type of message being received
	UFUNCTION(BlueprintCallable)
	void ProcessIncomingMessage(UPARAM() ARModel* Robot, UPARAM() FString Topic, UPARAM() FString Message);

	// Method for dealing with joint trajectory actions
	void TriggerJointTrajectoryLatentAction(ARModel* Robot, FString Message);

	// Method for dealing with joint configuration actions
	void TriggerRobotJointConfigAction(ARModel* Robot, FString Message);

	// Method for dealing with navigation actions
	void TriggerNavigationLatentAction(ARModel* Robot, FString Message);



		
};
