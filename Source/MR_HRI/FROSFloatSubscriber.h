#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeSubscriber.h"
#include "std_msgs/Float32.h"

class FROSFloatSubscriber : public FROSBridgeSubscriber {

public:

	float latestValue = 0.0;

	FROSFloatSubscriber(const FString& Topic);
	~FROSFloatSubscriber();
	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;
	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;
};
