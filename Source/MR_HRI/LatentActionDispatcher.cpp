#include "LatentActionDispatcher.h"

// Sets default values for this component's properties
ULatentActionDispatcher::ULatentActionDispatcher()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void ULatentActionDispatcher::ProcessIncomingMessage(UPARAM() ARModel* Robot, UPARAM() FString Topic, UPARAM() FString Message)
{
	if (Topic.Contains("trajectory")) {
		TriggerJointTrajectoryLatentAction(Robot, Message);
	}
	else if (Topic.Contains("configuration")) {
		TriggerRobotJointConfigAction(Robot, Message);
	}
	else if (Topic.Contains("navigation")) {
		// TO DO
	}
	else
		return;
}


void ULatentActionDispatcher::TriggerRobotJointConfigAction(ARModel* Robot, FString Message)
{
	TSharedPtr<FJsonObject> ParsedMessage;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(JsonReader, ParsedMessage))
	{
		auto JsonJointNames = ParsedMessage->GetArrayField("name");
		TArray<FString> JointNames;
		for (auto JsonName : JsonJointNames) 
		{
			FString JointName;
			JsonName->TryGetString(JointName);
			JointNames.Add(JointName);
		}
		auto JsonJointPositions = ParsedMessage->GetArrayField("position");
		TArray<float> JointPositions;
		for (auto JsonPos : JsonJointPositions) 
		{
			float JointPos;
			JsonPos->TryGetNumber(JointPos);
			JointPositions.Add(JointPos);
		}

		int i = 0;
		FHitResult Hit;
		for (auto JointName : JointNames)
		{
			Robot->GetJoint(JointName)->SetJointPosition(JointPositions[i], &Hit);
			i++;
		}
	}
}


void ULatentActionDispatcher::TriggerJointTrajectoryLatentAction(ARModel* Robot, FString Message)
{
	TSharedPtr<FJsonObject> ParsedMessage;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(JsonReader, ParsedMessage))
	{
		auto JsonJointNames = ParsedMessage->GetArrayField("joint_names");
		TArray<FString> JointNames;
		for (auto JsonName : JsonJointNames)
		{
			FString JointName;
			JsonName->TryGetString(JointName);
			JointNames.Add(JointName);
		}

		TArray<trajectory_msgs::JointTrajectoryPoint> JointTrajectoryPoints;
		auto JsonTrajectoryPoints = ParsedMessage->GetArrayField("points");
		for (int u = 0; u < JsonTrajectoryPoints.Num(); u++)
		{
			auto JsonTrajectoryPoint = JsonTrajectoryPoints[u]->AsObject();
			trajectory_msgs::JointTrajectoryPoint JointTrajectoryPoint;
			JointTrajectoryPoint.FromJson(JsonTrajectoryPoint);
			JointTrajectoryPoints.Add(JointTrajectoryPoint);
		}

		int id = FMath::RandRange(1, 1000);
		GetWorld()->GetLatentActionManager().AddNewAction(this, id, new JointTrajectoryLatentAction(
			JointTrajectoryPoints,
			JointNames,
			Robot,
			GetWorld()->DeltaTimeSeconds,
			TrajectoryUpdateFrequency,
			TrajectoryMaxSteps
		));
	}
}



