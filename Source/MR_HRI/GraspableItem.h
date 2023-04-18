#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraspableItem.generated.h"

UCLASS()
class MR_HRI_API AGraspableItem : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GraspableMeshComp;

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

};
