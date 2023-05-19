#include "DynamicModelSpawnerComponent.h"


// Sets default values for this component's properties
UDynamicModelSpawnerComponent::UDynamicModelSpawnerComponent() {}

// Dtor
UDynamicModelSpawnerComponent::~UDynamicModelSpawnerComponent() {};

void UDynamicModelSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

    ModelBuilder = NewObject<URModelBuilder>(GetOuter());
    ControllerBuilder = NewObject<URControllerBuilder>(GetOuter());

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ModelBuilder->GetName());
}

void UDynamicModelSpawnerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Set variables to null so that they are collected by GC
    ModelBuilder = nullptr;
    ControllerBuilder = nullptr;

    // Empty array of robot models
    for (auto robot : RobotModels) {
        robot = nullptr;
    }

    Super::EndPlay(EndPlayReason);
}

ARModel* UDynamicModelSpawnerComponent::SpawnModelWithController(UPARAM() USDFDataAsset* SDFAsset, 
    UPARAM() URControllerDataAsset* ControllerDataAsset, UPARAM() FTransform Transform) {

    ARModel* NewRobot = nullptr;
    for (USDFModel* Model : SDFAsset->Models)
    {
        // --- Spawn model ---
        // Spawn empty robot actor
        NewRobot = (ARModel*)GetWorld()->SpawnActor(ARModel::StaticClass(), &Transform);
        // Fill model with joints and links from SDF asset
        ModelBuilder->Load(Model, NewRobot, Transform.GetLocation());
        // Set base link to static to avoid model from falling
        auto RobotLinks = NewRobot->GetLinks();
        
        // -- Add controller --
        // Add controller component to robot model
        URControllerComponent* ControllerComponent = NewObject<URControllerComponent>(NewRobot, TEXT("ControllerComponent"));
        // Iterate over controllers (if robot supports multiple controllers at once)
        for (TPair<FString, URControllerParameter*> ControllerParameters : ControllerDataAsset->ControllerConfiguration.ControllerParameters)
        {
            UE_LOG(LogTemp, Log, TEXT("Create %s of %s"), *ControllerParameters.Key, *NewRobot->GetName());
            // Instantiate new controller, set its parameters and register it to robot model...
            URController* Controller = ControllerBuilder->CreateController(NewRobot, ControllerParameters);
            Controller->SetControllerParameters(ControllerParameters.Value);
            ControllerComponent->AddController(Controller);
        }
        ControllerComponent->RegisterComponent();

        // Reset internal robot config for debug purposes
        FHitResult hit;
        for (auto joint : NewRobot->GetJoints()) {
            joint->SetJointPosition(0.0, &hit);
        }

        // Add new robot to list
        RobotModels.Add(NewRobot);

    }
    return NewRobot;
}

