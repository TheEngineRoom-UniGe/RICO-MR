// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann (neumann.michael@uni-bremen.de)

#pragma once

#include "CoreMinimal.h"
#include "Physics/RLink.h"
#include "Physics/RModel.h"
#include "SDF/SDFLink.h"
#include "RLinkFactory.generated.h"


UCLASS()
class UROBOSIM_API URLinkBuilder : public UObject
{
  GENERATED_BODY()

public:

  virtual void  Init(UObject* InOuter, USDFLink* InLinkDescription);
  virtual void Init(UObject* InOuter, USDFLink* InLinkDescription,FVector InLocation);
//  virtual URLink* NewLink(FVector InLocation);
  virtual URLink* NewLink();

  UPROPERTY(BlueprintReadWrite)
    ARModel* Model;

  UPROPERTY(BlueprintReadWrite)
    FVector LoadLocation;

  UPROPERTY(BlueprintReadWrite)
    USDFLink* LinkDescription;

  UPROPERTY(BlueprintReadWrite)
    URLink* Link;

  FTransform LinkPose;

  void SetPose(FTransform InPose);
  void SetPoseComponent();

  virtual void SetVisuals();
  virtual void SetVisual(USDFVisual* InVisual);
  virtual void SetCollisions();
  virtual void SetCollision(USDFCollision* InCollision);

  virtual void SetInertial(USDFLinkInertial* InInertial);
  virtual void SetCollisionProfile(bool InSelfColide);
  virtual void SetSimulateGravity(bool InUseGravity);
};

UCLASS()
class UROBOSIM_API URLinkFactory : public UObject
{
  GENERATED_BODY()
 public:

    URLink* Load(UObject* InOuter, USDFLink* InLinkDescription);
    URLink* Load(UObject* InOuter, USDFLink* InLinkDescription,FVector InLoaction);

    UPROPERTY(BlueprintReadWrite)
    URLinkBuilder* LinkBuilder;

  virtual URLinkBuilder* CreateBuilder(USDFLink* InLinkDescription);

  

};
