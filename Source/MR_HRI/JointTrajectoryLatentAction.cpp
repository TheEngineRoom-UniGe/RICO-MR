#include "JointTrajectoryLatentAction.h"

JointTrajectoryLatentAction::JointTrajectoryLatentAction(TArray<trajectory_msgs::JointTrajectoryPoint> trajectoryPoints,
	TArray<FString> jointNames, ARModel* robot, float& deltaTime, float updateFrequency, int maxSteps) :
		trajectoryPoints_(trajectoryPoints), 
		jointNames_(jointNames), 
		robot_(robot), 
		deltaTime_(deltaTime), 
		trajectoryPointIdx_(1), 
		elapsedTime_(0.0), 
		updateFrequency_(updateFrequency),
		stepsCount_(0),
		maxSteps_(maxSteps)
{
	// Initialize array storing first configuration (initial one)
	for (double jointPosition : trajectoryPoints_[0].GetPositions()) {
		startingJointConfig_.Add(jointPosition);
	}
	FString logMessage = "Joint trajectory latent action issued to execute trajectory on " + robot_->GetName();
	UE_LOG(LogTemp, Log, TEXT("%s"), *logMessage);
}
 
void JointTrajectoryLatentAction::UpdateOperation(FLatentResponse& Response) {
	// Check if elapsed time is greater than update frequency
	elapsedTime_ += deltaTime_;
	// If yes, update robot's joint configuration
	if (elapsedTime_ >= updateFrequency_) {
		int jointIdx = 0;
		FHitResult hit;
		// Update individual joint position step-by-step
		for (double jointPosition : trajectoryPoints_[trajectoryPointIdx_].GetPositions()) {
			double nextJointPositionStep = startingJointConfig_[jointIdx] + 
				1.0 / (double) maxSteps_ * ( jointPosition - startingJointConfig_[jointIdx]) * (double) stepsCount_;
			robot_->GetJoint(jointNames_[jointIdx])->SetJointPosition(nextJointPositionStep, &hit);
			jointIdx++;
		}
		// Increase step count and check if it exceeds max value
		if (stepsCount_++ >= maxSteps_) {
			// Increase trajectory index
			trajectoryPointIdx_++;
			// Reset step count
			stepsCount_ = 0;
		}
		// Reset elapsed time
		elapsedTime_ = 0.0;

	}
	Response.DoneIf(trajectoryPointIdx_ >= trajectoryPoints_.Num());
}
