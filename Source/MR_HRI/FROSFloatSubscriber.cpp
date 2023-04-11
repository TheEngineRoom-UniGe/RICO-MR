#include "FROSFloatSubscriber.h"

FROSFloatSubscriber::FROSFloatSubscriber(const FString& Topic) :
    FROSBridgeSubscriber(Topic, TEXT("std_msgs/Float32"))
{
    UE_LOG(LogTemp, Log, TEXT("Subscriber created"));
}

FROSFloatSubscriber::~FROSFloatSubscriber() {}

TSharedPtr<FROSBridgeMsg> FROSFloatSubscriber::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
    TSharedPtr<std_msgs::Float32> FloatMessage = MakeShareable<std_msgs::Float32>(new std_msgs::Float32());
    FloatMessage->FromJson(JsonObject);
    return StaticCastSharedPtr<FROSBridgeMsg>(FloatMessage);
}

void FROSFloatSubscriber::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
    TSharedPtr<std_msgs::Float32> FloatMessage = StaticCastSharedPtr<std_msgs::Float32>(Msg);
    // downcast to subclass using StaticCastSharedPtr function
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(FloatMessage->GetData()));
    latestValue = FloatMessage->GetData();

    return;
}
