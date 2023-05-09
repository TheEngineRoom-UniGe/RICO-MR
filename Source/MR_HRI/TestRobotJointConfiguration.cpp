// Fill out your copyright notice in the Description page of Project Settings.


#include "TestRobotJointConfiguration.h"

// Sets default values
ATestRobotJointConfiguration::ATestRobotJointConfiguration()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}


void ATestRobotJointConfiguration::TestJointConfig(UPARAM() ARModel* Robot, UPARAM() TArray<float> JointValues)
{
    int i = 0;
    FHitResult Hit;
    for (auto joint : Robot->GetJoints()) {
        joint->SetJointPosition(JointValues[i], &Hit);
        i++;
    }
}

