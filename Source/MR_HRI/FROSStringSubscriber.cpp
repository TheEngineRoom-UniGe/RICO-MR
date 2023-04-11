// Fill out your copyright notice in the Description page of Project Settings.
#include "FROSStringSubscriber.h"

FROSStringSubscriber::FROSStringSubscriber(const FString& Topic) :
	FROSBridgeSubscriber(Topic, TEXT("std_msgs/String"))
{
	UE_LOG(LogTemp, Log, TEXT("Subscriber created"));
}

FROSStringSubscriber::~FROSStringSubscriber() {}

TSharedPtr<FROSBridgeMsg> FROSStringSubscriber::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
    TSharedPtr<std_msgs::String> StringMessage = MakeShareable<std_msgs::String>(new std_msgs::String());
    StringMessage->FromJson(JsonObject);
    return StaticCastSharedPtr<FROSBridgeMsg>(StringMessage);
}

void FROSStringSubscriber::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
    TSharedPtr<std_msgs::String> StringMessage = StaticCastSharedPtr<std_msgs::String>(Msg);
    // downcast to subclass using StaticCastSharedPtr function
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *StringMessage->GetData());

    return;
}
