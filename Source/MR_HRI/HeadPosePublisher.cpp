#include "HeadPosePublisher.h"

// Sets default values for this component's properties
UHeadPosePublisher::UHeadPosePublisher()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UHeadPosePublisher::PublishHeadPose(UPARAM() FVector Position, UPARAM() FRotator Rotation, UPARAM() FString Topic)
{
	// Build pose msg from inputs
	geometry_msgs::Point Point;
	geometry_msgs::Quaternion Quat;
	geometry_msgs::Pose PoseMsg;

	Point.SetVector(Position);
	Quat.SetQuat(Rotation.Quaternion());

	PoseMsg.SetPosition(Point);
	PoseMsg.SetOrientation(Quat);

	// Build Kafka producer record based on pose msg
	FProducerRecord record;
	record.Key = "HeadPose";
	record.Topic = Topic;
	record.Value = PoseMsg.ToString();
	record.Id = FMath::RandRange(1, 1000);

	// Get reference to kafka producer
	TSharedPtr<FEasyKafkaModule> EasyKafka = GEngine->GetEngineSubsystem<UEasyKafkaSubsystem>()->GetEasyKafka();
	EasyKafka->GetProducer()->ProduceRecord(record);
}

