// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Physics/RModel.h"
#include "Factory/RModelBuilder.h"
#include "Factory/RControllerBuilder.h"
#include "Controller/RControllerDataAsset.h"
#include "SDF/SDFDataAsset.h"
#include "DynamicModelSpawnerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MR_HRI_API UDynamicModelSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Robot model factory spawner object
	UPROPERTY(BlueprintReadWrite)
	URModelBuilder* ModelBuilder;
	// Controller factory spawner object
	URControllerBuilder* ControllerBuilder;

	// List of robot models
	TArray<ARModel*> RobotModels;

	// Ctor
	UDynamicModelSpawnerComponent();
	// Dtor
	~UDynamicModelSpawnerComponent();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;
	// Called at game termination
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	


	/* Method to spawn new robot model in the world
	   Args:
	   - SDFAsset: parsed SDF file structure containing model's description
	   - ControllerDataAsset: reference to controller asset for spawning appropriate robot controller
	   - Transform: transformation (in world coordinates) where the model needs to be spawned
	*/
	UFUNCTION(BlueprintCallable, Category = "ModelSpawner")
	ARModel* SpawnModelWithController(UPARAM() USDFDataAsset* SDFAsset, UPARAM() URControllerDataAsset* ControllerDataAsset, UPARAM() FTransform Transform);

		
};
