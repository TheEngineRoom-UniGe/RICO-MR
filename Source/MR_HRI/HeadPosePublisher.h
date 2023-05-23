#pragma once

#include "CoreMinimal.h"
#include "EasyKafkaSubsystem.h"
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Quaternion.h>
#include "Components/ActorComponent.h"
#include "HeadPosePublisher.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MR_HRI_API UHeadPosePublisher : public UActorComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	UHeadPosePublisher();

	// Method used to publish player camera's pose through kafka
	UFUNCTION(BlueprintCallable)
	void PublishHeadPose(UPARAM() FVector Position, UPARAM() FRotator Rotation, UPARAM() FString Topic);

};
