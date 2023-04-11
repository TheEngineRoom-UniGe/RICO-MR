#include "FROSJointTrajectorySubscriber.h"

/* Constructor method invoking parent class for initializing subscriber to proper type
*  Args: 
*   - topic: name of the topic to which the class will subscribe
*   - robot: reference to robot actor to which trajectories will be applied
*/
FROSJointTrajectorySubscriber::FROSJointTrajectorySubscriber(const FString& topic, ARModel* robot) :
    FROSBridgeSubscriber(topic, TEXT("trajectory_msgs/JointTrajectory")), robot_(robot), hasPendingTrajectory_(false), jointTrajectoryMsg_(NULL)
{
    FString messageLog = "Subscriber to topic '" + topic + "' [trajectory_msgs/JointTrajectory] created!";
    UE_LOG(LogTemp, Log, TEXT("%s"), *messageLog);
}

// Destructor method
FROSJointTrajectorySubscriber::~FROSJointTrajectorySubscriber() {}

// Parser method
TSharedPtr<FROSBridgeMsg> FROSJointTrajectorySubscriber::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
    TSharedPtr<trajectory_msgs::JointTrajectory> JointTrajectoryMsg = MakeShareable<trajectory_msgs::JointTrajectory>(new trajectory_msgs::JointTrajectory());
    JointTrajectoryMsg->FromJson(JsonObject);
    return StaticCastSharedPtr<FROSBridgeMsg>(JointTrajectoryMsg);
}

// Callback method being invoked (after message parsing) upong receiving new messages...
void FROSJointTrajectorySubscriber::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
    // Parse joint trajectory message
    jointTrajectoryMsg_ = StaticCastSharedPtr<trajectory_msgs::JointTrajectory>(Msg);
    // Raise boolean flag to signal ROScommunication component that this subscriber has a trajectory to execute
    hasPendingTrajectory_ = true;
    
    return;
}