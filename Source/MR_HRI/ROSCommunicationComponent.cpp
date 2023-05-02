#include "ROSCommunicationComponent.h"

// Sets default values for this component's properties
UROSCommunicationComponent::UROSCommunicationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UROSCommunicationComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UROSCommunicationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Handler->Disconnect();
	UE_LOG(LogTemp, Log, TEXT("[UROSCommunicationComponent::EndPlay()] Websocket server disconnected."));

	// Empty array of robots
	for (auto robot : RobotModels) {
		robot = nullptr;
	}

	// Empty arrays of subscribers
	for (auto subscriber : JointTrajectorySubscribers) {
		subscriber = nullptr;
	}
	for (auto subscriber : JointConfigSubscribers) {
		subscriber = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}


void UROSCommunicationComponent::ConnectToROS(UPARAM() FString ROSIPAddr) {

	Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(*ROSIPAddr, ROSBridgeServerPort));
	Handler->Connect();

	UE_LOG(LogTemp, Log, TEXT("[UROSCommunicationComponent::BeginPlay()] Websocket server connected."));

	isConnected = true;
}


void UROSCommunicationComponent::RegisterNewRobot(UPARAM() ARModel* robot) {
	// Get current array length to set up new id
	auto n = RobotModels.Num();
	// Store reference to new robot
	RobotModels.Add(robot);

	// Reset internal robot config for debug purposes
	FHitResult hit;
	for (auto joint : robot->GetJoints()) {
		joint->SetJointPosition(0.0, &hit);
	}

	// ---- Expose topics for new robot ----
	// joint configuration subscriber
	auto jointConfigTopicName = "/robot/" + FString::FromInt(n) + "/joint_config";
	auto JointConfigSubscriber = MakeShareable<FROSJointConfigSubscriber>(new FROSJointConfigSubscriber(jointConfigTopicName, robot));
	Handler->AddSubscriber(JointConfigSubscriber);

	// joint trajectory subscriber
	auto JointTrajectoryTopicName = "/robot/" + FString::FromInt(n) + "/joint_trajectory";
	auto JointTrajectorySubscriber = MakeShareable<FROSJointTrajectorySubscriber>(new FROSJointTrajectorySubscriber(JointTrajectoryTopicName, robot));
	Handler->AddSubscriber(JointTrajectorySubscriber);

	// Store references to newly added subscribers
	JointConfigSubscribers.Add(JointConfigSubscriber);
	JointTrajectorySubscribers.Add(JointTrajectorySubscriber);
}


// Called every frame
void UROSCommunicationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Process incoming ROS messages...
	if (isConnected) {
		Handler->Process();

		// Check if any subscriber has a pending trajectory to execute...
		for (auto subscriber : JointTrajectorySubscribers) {
			if (subscriber->hasPendingTrajectory_) {
				// Reset boolean flag
				subscriber->hasPendingTrajectory_ = false;
				// Add latent action to world, so trajectory can be properly executed
				int id = FMath::RandRange(1, 1000);
				GetWorld()->GetLatentActionManager().AddNewAction(this, id, new JointTrajectoryLatentAction(
					subscriber->jointTrajectoryMsg_->GetPoints(),
					subscriber->jointTrajectoryMsg_->GetJointNames(),
					subscriber->robot_,
					GetWorld()->DeltaTimeSeconds,
					ROSTrajectoryUpdateFrequency,
					ROSTrajectoryMaxSteps
				));
			}
		}
	}
}

