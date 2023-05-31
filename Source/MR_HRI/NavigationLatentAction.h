// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <LatentActions.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include "Physics/RModel.h"


class NavigationLatentAction : public FPendingLatentAction {

public:

	// Array of waypoints in the path
	TArray<geometry_msgs::PoseStamped> Poses_;
	int PoseIdx_;
	// Transform between origin frame (HMD) and robot's spawn pose (QR)
	FVector BasePosition_;
	FQuat BaseRotation_;
	// Transform between world frame (ROS) and robot's spawn pose (QR)
	FVector World2QRPosition_;
	FQuat World2QRRotation_;
	// Pointer to robot model
	ARModel* Robot_;

	float DeltaTime_;
	float UpdateFrequency_;
	float ElapsedTime_;

	// Constructor setting default values for latent action
	NavigationLatentAction(const TArray<geometry_msgs::PoseStamped>& Poses, FVector BasePosition, FQuat BaseRotation,
		FVector World2QRPosition, FQuat World2QRRotation, ARModel* Robot, float DeltaTime, float UpdateFrequency);

	// Dtor
	~NavigationLatentAction();

	// Method called at every frame once latent action is issued
	virtual void UpdateOperation(FLatentResponse& Response) override;
};
