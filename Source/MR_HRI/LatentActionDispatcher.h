#pragma once

#include "CoreMinimal.h"
#include "Json.h"
#include "Components/ActorComponent.h"
#include <Physics/RModel.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include "FROSJointTrajectoryLatentAction.h"
#include "LatentActionDispatcher.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MR_HRI_API ULatentActionDispatcher : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere)
		float TrajectoryUpdateFrequency = 0.025f;

	UPROPERTY(EditAnywhere)
		int TrajectoryMaxSteps = 50;

	// Sets default values for this component's properties
	ULatentActionDispatcher();

	// Method acting as dispatcher for robot action depending on type of message being received
	UFUNCTION(BlueprintCallable)
	void ProcessIncomingMessage(UPARAM() ARModel* Robot, UPARAM() FString Topic, UPARAM() FString Message);

	// Method for dealing with joint trajectory actions
	void TriggerJointTrajectoryLatentAction(ARModel* Robot, FString Message);

	// Method for dealing with joint configuration actions
	void TriggerRobotJointConfigAction(ARModel* Robot, FString Message);



		
};
