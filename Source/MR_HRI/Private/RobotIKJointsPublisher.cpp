#include "RobotIKJointsPublisher.h"

// Sets default values for this component's properties
URobotIKJointsPublisher::URobotIKJointsPublisher()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URobotIKJointsPublisher::PublishJoints(UPARAM() FString Topic, UPARAM() TArray<float> JointValues)
{
	// Build JointTrajectoryPoint msg from input array
	trajectory_msgs::JointTrajectoryPoint Point;
	TArray<double> JointValuesAsDouble;

	for (auto v : JointValues) {
		JointValuesAsDouble.Add((double)v);
	}
	Point.SetPositions(JointValuesAsDouble);

	// Build Kafka producer record based on array of joint values
	FProducerRecord record;
	record.Key = "RobotIK";
	record.Topic = Topic;
	record.Value = Point.ToString();
	record.Id = FMath::RandRange(1, 1000);

	// Get reference to kafka producer
	TSharedPtr<FEasyKafkaModule> EasyKafka = GEngine->GetEngineSubsystem<UEasyKafkaSubsystem>()->GetEasyKafka();
	EasyKafka->GetProducer()->ProduceRecord(record);
}



