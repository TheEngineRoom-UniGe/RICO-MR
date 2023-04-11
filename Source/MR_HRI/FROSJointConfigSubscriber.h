#pragma once

#include "CoreMinimal.h"
#include <Physics/RModel.h>
#include "ROSBridgeSubscriber.h"
#include "sensor_msgs/JointState.h"

class FROSJointConfigSubscriber : public FROSBridgeSubscriber
{
public:

	// Reference to robot model
	ARModel* robot_;
	
	// Methods declarations
	FROSJointConfigSubscriber(const FString& topic, ARModel* robot);
	~FROSJointConfigSubscriber();
	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;
	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;
};
