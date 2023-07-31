#include "RobotPoseInitializer.h"

// Sets default values
ARobotPoseInitializer::ARobotPoseInitializer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ARobotPoseInitializer::SetRobotPose(UPARAM() ARModel* Robot, UPARAM() TArray<FString> JointNames,
	UPARAM() TArray<float> JointValues)
{
	if (!IsValid(Robot))
		return;

	if (JointNames.Num() != JointValues.Num()) {
		return;
	}

	int i = 0;
	FHitResult Hit;
	for (auto Joint : JointNames) {
		Robot->GetJoint(Joint)->SetJointPosition(JointValues[i], &Hit);
		i++;
	}
}



