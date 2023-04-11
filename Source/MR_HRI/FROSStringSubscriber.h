// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeSubscriber.h"
#include "std_msgs/String.h"

class FROSStringSubscriber : public FROSBridgeSubscriber {

public:
	FROSStringSubscriber(const FString& Topic);
	~FROSStringSubscriber() override;
	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;
	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;
};
