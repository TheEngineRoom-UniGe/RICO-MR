#include "LocationPublisherComponent.h"

// Sets default values for this component's properties
ULocationPublisherComponent::ULocationPublisherComponent()
{
	
}

void ULocationPublisherComponent::PublishLocation(UPARAM() FString Topic, UPARAM() FVector Offset)
{
	// Get owner actor's location
	auto OwnerLoc = GetOwner()->GetActorLocation();

	// Get owner's location wrt robot
	auto RelativeOwnerLoc = OwnerLoc - Offset;

	// Build point msg 
	geometry_msgs::Point Point;
	Point.SetX(RelativeOwnerLoc.X);
	Point.SetY(RelativeOwnerLoc.Y);
	Point.SetZ(RelativeOwnerLoc.Z);
	
	// Build Kafka producer record based on pose msg
	FProducerRecord record;
	record.Key = "BallLoc";
	record.Topic = Topic;
	record.Value = Point.ToString();
	record.Id = FMath::RandRange(1, 1000);

	// Get reference to kafka producer
	TSharedPtr<FEasyKafkaModule> EasyKafka = GEngine->GetEngineSubsystem<UEasyKafkaSubsystem>()->GetEasyKafka();
	EasyKafka->GetProducer()->ProduceRecord(record);
}


