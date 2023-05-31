#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeSubscriber.h"
#include "trajectory_msgs/JointTrajectory.h"
#include "trajectory_msgs/JointTrajectoryPoint.h"
#include "Physics/RModel.h"

class FROSJointTrajectorySubscriber : public FROSBridgeSubscriber {

public:

	// Reference to robot model
	ARModel* robot_;
	// Utility bool flag
	bool hasPendingTrajectory_;
	// Variable to store current trajectory message
	TSharedPtr<trajectory_msgs::JointTrajectory> jointTrajectoryMsg_;
	
	// Methods declarations
	FROSJointTrajectorySubscriber(const FString& topic, ARModel* robot);
	~FROSJointTrajectorySubscriber();
	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;
	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;
};
