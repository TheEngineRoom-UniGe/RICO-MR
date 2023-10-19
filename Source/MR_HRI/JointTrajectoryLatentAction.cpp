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
		stepsCount_(0)
{
	// Initialize array storing first configuration (initial one)
	for (double jointPosition : trajectoryPoints_[0].GetPositions()) {
		startingJointConfig_.Add(jointPosition);
		TempJointConfig_.Add(0.0);
	}

	maxSteps_ = trajectoryPoints.Num() < 10 ? maxSteps : 1;

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
			URJoint* JointTemp =  robot_->GetJoint(jointNames_[jointIdx]);
			if (IsValid(JointTemp))
				JointTemp->SetJointPosition(nextJointPositionStep, &hit);

			TempJointConfig_[jointIdx] = nextJointPositionStep;
			jointIdx++;
			
		}
		// Increase step count and check if it exceeds max value
		if (stepsCount_++ >= maxSteps_) {
			// Increase trajectory index
			trajectoryPointIdx_++;
			// Reset step count
			stepsCount_ = 0;
			// Set new starting joint configuration
			for (int r = 0; r < startingJointConfig_.Num(); r++) {
				startingJointConfig_[r] = TempJointConfig_[r];
			}
		}
		// Reset elapsed time
		elapsedTime_ = 0.0;

	}
	Response.DoneIf(trajectoryPointIdx_ >= trajectoryPoints_.Num());
}
