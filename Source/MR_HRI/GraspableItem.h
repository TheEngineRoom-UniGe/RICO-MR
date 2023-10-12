#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/LocationPublisherComponent.h"
#include "GraspableItem.generated.h"

UCLASS()
class MR_HRI_API AGraspableItem : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GraspableMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ULocationPublisherComponent* LocationPublisherComp;

	UPROPERTY(VisibleAnywhere, Category = "Loc Publish")
	FVector Offset;

	UPROPERTY(EditAnywhere, Category = "Loc Publish")
	FString Topic;

	UPROPERTY(EditAnywhere, Category = "Loc Publish")
	bool bPublishLocation;

	// Timer handle for controlling object's location publish
	FTimerHandle LocationPublish_TimerHandle;

	// Internal variable used for detecting grasp
	int OverlapCounter = 0;

	// Sets default values for this actor's properties
	AGraspableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Method invoked whenever actor receives overlap event 
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Method invoked when overlapping ends
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Callback method to publish object's location on timer elapsed
	void OnTimerElapsed();

	// Base method to set offset for consistency
	UFUNCTION(BlueprintCallable)
	void SetOffset(UPARAM() FVector NewOffset);

};
