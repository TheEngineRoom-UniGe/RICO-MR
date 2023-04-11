#pragma once

#include "CoreMinimal.h"
#include <LatentActions.h>
#include "trajectory_msgs/JointTrajectoryPoint.h"
#include "Physics/RModel.h"

class JointTrajectoryLatentAction : public FPendingLatentAction {

public:

	// Array of joint trajectory points and joint names
	TArray<FString> jointNames_;
	TArray<trajectory_msgs::JointTrajectoryPoint> trajectoryPoints_;
	int trajectoryPointIdx_;
	TArray<double> startingJointConfig_;
	// Pointer to robot model
	ARModel* robot_;
	// Reference to deltaTime
	float& deltaTime_;
	// Utility variables for temporal update
	float elapsedTime_;
	float updateFrequency_;
	// Utility variables for smoothing trajectory
	int maxSteps_;
	int stepsCount_;

	// Constructor setting default values for latent action
	JointTrajectoryLatentAction(TArray<trajectory_msgs::JointTrajectoryPoint> trajectoryPoints, 
		TArray<FString> jointNames, ARModel* robot, float& deltaTime, float updateFrequency, int maxSteps);
	~JointTrajectoryLatentAction() {};

	// Method called at every frame once latent action is issued
	virtual void UpdateOperation(FLatentResponse& Response) override;


};
