#include "FROSJointConfigSubscriber.h"

/* Constructor method invoking parent class for initializing subscriber to proper type
*  Args:
*   - topic: name of the topic to which the class will subscribe
*   - robot: reference to robot actor
*/
FROSJointConfigSubscriber::FROSJointConfigSubscriber(const FString& topic, ARModel* robot) :
    FROSBridgeSubscriber(topic, TEXT("sensor_msgs/JointState")), robot_(robot)
{
    FString messageLog = "Subscriber to topic '" + topic + "' [sensor_msgs/JointState] created!";
    UE_LOG(LogTemp, Log, TEXT("%s"), *messageLog);
}

// Dtor
FROSJointConfigSubscriber::~FROSJointConfigSubscriber() {}

// Parser method
TSharedPtr<FROSBridgeMsg> FROSJointConfigSubscriber::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
    TSharedPtr<sensor_msgs::JointState> JointStateMsg = MakeShareable<sensor_msgs::JointState>(new sensor_msgs::JointState());
    JointStateMsg->FromJson(JsonObject);
    return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMsg);
}

// Callback method being invoked (after message parsing) upong receiving new messages...
void FROSJointConfigSubscriber::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
    // Parse joint state message
    auto jointStateMsg = StaticCastSharedPtr<sensor_msgs::JointState>(Msg);
    
    // Apply joint configuration to robot model
    FHitResult hit;
    int i = 0;
    for (auto jointName : jointStateMsg->GetName()) {
        robot_->GetJoint(jointName)->SetJointPosition(jointStateMsg->GetPosition()[i], &hit);
        i++;
    }

    return;
}