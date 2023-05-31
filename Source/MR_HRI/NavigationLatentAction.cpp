#include "NavigationLatentAction.h"

NavigationLatentAction::NavigationLatentAction(const TArray<geometry_msgs::PoseStamped>& Poses, 
	FVector BasePosition, FQuat BaseRotation, FVector World2QRPosition, FQuat World2QRRotation,
	ARModel* Robot, float DeltaTime, float UpdateFrequency) :
		Poses_(Poses),
		PoseIdx_(0),
		World2QRPosition_(World2QRPosition),
		World2QRRotation_(World2QRRotation),
		BasePosition_(BasePosition),
		BaseRotation_(BaseRotation),
		Robot_(Robot),
		DeltaTime_(DeltaTime),
		UpdateFrequency_(UpdateFrequency),
		ElapsedTime_(0.0)

{
	FString logMessage = "Navigation latent action issued to execute path on " + Robot_->GetName();
	UE_LOG(LogTemp, Log, TEXT("%s"), *logMessage);
}


NavigationLatentAction::~NavigationLatentAction() {}


void NavigationLatentAction::UpdateOperation(FLatentResponse& Response)
{
	// Check if elapsed time is greater than update frequency
	ElapsedTime_ += DeltaTime_;
	// If yes, update robot's pose
	if (ElapsedTime_ >= UpdateFrequency_) {

		float pX = Poses_[PoseIdx_].GetPose().GetPosition().GetX();
		float pY = Poses_[PoseIdx_].GetPose().GetPosition().GetY();
		float pZ = Poses_[PoseIdx_].GetPose().GetPosition().GetZ();
		FVector NewPos = FVector(pX, pY, pZ);

		float rX = Poses_[PoseIdx_].GetPose().GetOrientation().GetX();
		float rY = Poses_[PoseIdx_].GetPose().GetOrientation().GetY();
		float rZ = Poses_[PoseIdx_].GetPose().GetOrientation().GetZ();
		float rW = Poses_[PoseIdx_].GetPose().GetOrientation().GetW();
		FQuat NewRotAsQuat = FQuat(rX, rY, rZ, rW);
		
		// TO DO: Set rotation properly
		Robot_->TeleportTo(BasePosition_ + (NewPos - World2QRPosition_) * 100.0f, NewRotAsQuat.Rotator());

		PoseIdx_++;
		ElapsedTime_ = 0.0f;
	}
	Response.DoneIf(PoseIdx_ >= Poses_.Num());
}
