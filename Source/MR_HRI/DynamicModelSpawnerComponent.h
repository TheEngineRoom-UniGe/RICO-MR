// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Physics/RModel.h"
#include "Factory/RModelBuilder.h"
#include "Factory/RControllerBuilder.h"
#include "Controller/RControllerDataAsset.h"
#include <Editor/EditorEngine.h>
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

	// Called for spawning new model in the world
	UFUNCTION(BlueprintCallable, Category = "ModelSpawner")
	ARModel* SpawnModelWithController(UPARAM() USDFDataAsset* SDFAsset, UPARAM() URControllerDataAsset* ControllerDataAsset, UPARAM() FTransform Transform);

		
};
