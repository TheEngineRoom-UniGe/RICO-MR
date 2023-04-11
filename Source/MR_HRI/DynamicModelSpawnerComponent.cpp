#include "DynamicModelSpawnerComponent.h"


// Sets default values for this component's properties
UDynamicModelSpawnerComponent::UDynamicModelSpawnerComponent()
{
    ModelBuilder = NewObject<URModelBuilder>(GetOuter());
    ControllerBuilder = NewObject<URControllerBuilder>(GetOuter());
}

// Dtor
UDynamicModelSpawnerComponent::~UDynamicModelSpawnerComponent() {};

// Called when the game starts
void UDynamicModelSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ModelBuilder->GetName());
}

// Called when the game ends
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

//---------------------------------------------

/* Method to spawn new robot model in the world
*  Params:
*   - SDFAsset: parsed SDF file structure containing model's description
*   - ControllerDataAsset: reference to controller asset for spawning appropriate robot controller
*	- Transform: transformation (in world coordinates) where the model needs to be spawned
*/
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
        //NewRobot->GetLinks()[0]->GetCollision()->SetMobility(EComponentMobility::Stationary);
        
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

        // Add new robot to list
        RobotModels.Add(NewRobot);

    }
    return NewRobot;
}

