#include "GraspableItem.h"

// Sets default values
AGraspableItem::AGraspableItem()
{
	GraspableMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GraspableMeshComp"));
	GraspableMeshComp->SetupAttachment(RootComponent);

	GraspableMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	GraspableMeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	GraspableMeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	GraspableMeshComp->SetSimulatePhysics(true);
	GraspableMeshComp->SetEnableGravity(true);

	LocationPublisherComp = CreateDefaultSubobject<ULocationPublisherComponent>(TEXT("LocationPublisherComp"));

	bPublishLocation = false;
	Topic = "";
}

// Called when the game starts or when spawned
void AGraspableItem::BeginPlay()
{
	Super::BeginPlay();

	// Add notification for overlapping events
	GraspableMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AGraspableItem::OnBeginOverlap);
	GraspableMeshComp->OnComponentEndOverlap.AddDynamic(this, &AGraspableItem::OnEndOverlap);
	
}

void AGraspableItem::OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	UE_LOG(LogTemp, Log, TEXT("Begin Overlap"));

	// Increase overlap counter
	this->OverlapCounter++;
	// If item is grasped...
	if (this->OverlapCounter == 2) {
		// Attach item to robot's end-effector
		this->AttachToComponent(OtherComp, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, true));
	}
}

void AGraspableItem::OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	UE_LOG(LogTemp, Log, TEXT("End Overlap"));

	this->OverlapCounter -= 1;
	if (this->OverlapCounter < 2) {
		// Detach item from robot's end-effector
		this->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, false));
		// Reset item's physics simulation
		GraspableMeshComp->SetSimulatePhysics(true);

		// Start routine to publish object's location is corresponding flag is set
		if (bPublishLocation)
		{
			GetWorld()->GetTimerManager().SetTimer(LocationPublish_TimerHandle, this, &AGraspableItem::OnTimerElapsed, 2.0f, false);
		}
	}
}

void AGraspableItem::OnTimerElapsed()
{
	LocationPublisherComp->PublishLocation(Topic, Offset);
}

void AGraspableItem::SetOffset(UPARAM() FVector NewOffset)
{
	Offset.X = NewOffset.X;
	Offset.Y = NewOffset.Y;
	Offset.Z = NewOffset.Z;
}

