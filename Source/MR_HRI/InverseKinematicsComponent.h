// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <mutex>

#include <Eigen/Dense>
#include <Eigen/SVD>
#include <Eigen/QR>

#include <Engine/DataTable.h>
#include <Physics/RModel.h>

#include "InverseKinematicsComponent.generated.h"


/** Structure that defines a row of DH params */
USTRUCT(BlueprintType)
struct FDHParamsStruct: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Theta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Alpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float a;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float d;

	FDHParamsStruct() {
		Theta = 0;
		Alpha = 0;
		a = 0;
		d = 0;
	}
};


// Utility class to represent links and their individual transformations
UCLASS()
class MR_HRI_API ULink : public UObject
{
	GENERATED_BODY()

protected:

	// Link parameter variables, according to DH convention
	float Theta_;
	float Alpha_;
	float a_;
	float d_;


public:

	// Default Constructor
	ULink();

	// Set link's DH parameters
	void SetDHParams(float Theta, float Alpha, float a, float d);

	// Utility method to set theta value
	void SetTheta(float Theta);

	// Return link's theta
	float GetTheta();

	// Compute link transformation matrix
	Eigen::MatrixXf TransformationMatrix();

	// Compute basic link jacobian
	Eigen::VectorXf BasicJacobian(const Eigen::MatrixXf& PreviousTransform, const Eigen::Vector3f& EEPosition);
};



// Class representing robot arm, exposing IK functionality
UCLASS()
class MR_HRI_API URobotArm : public UObject
{
	GENERATED_BODY()

public:

	// Variable storing number of links in kinematic chain
	int NLinks_;

	// Array of Links to represent kinematic chain
	UPROPERTY(VisibleAnywhere)
	TArray<ULink*> Links;

	// Mutex needed for safe access to links' data
	std::mutex Mutex;

	// Default Constructor
	URobotArm();

	/* Initialize RobotArm object with params: 
		- Number of links in the kinematic chain
		- Matrix of DH parameters
	*/
	void Init(int NLinks, const Eigen::MatrixXf& DHParams);

	// Arm transformation matrix
	Eigen::MatrixXf TransformationMatrix();

	// Forward kinematics. Returns end-effector's pose given current joint configuration
	Eigen::VectorXf ForwardKinematics(const Eigen::MatrixXf& T_e0);

	// Retrieve Euler angles from current transformation matrix
	Eigen::Vector3f GetEulerAngles(const Eigen::MatrixXf& T_e0);

	// Compute jacobian matrix
	Eigen::MatrixXf ComputeJacobian(const Eigen::Vector3f& EEPosition);

	// Set joint angles
	void SetJointAngles(const Eigen::VectorXf& JointAngles);

	// Update joint angles and clip in [-pi, pi]
	void UpdateJointAngles(const Eigen::VectorXf& JointAnglesDiff);

	// Getter for joint angles
	Eigen::VectorXf GetJointAngles();

	// Utility method to compute pseudoinverse using SVD decomp
	Eigen::MatrixXf PseudoInverse(const Eigen::MatrixXf& Matrix);

	// Retrieve joint angles from desired end-effector's pose
	Eigen::VectorXf InverseKinematics(const Eigen::VectorXf& DesiredEEPose, int MaxIterations);
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MR_HRI_API UInverseKinematicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	int MaxIterations;

	// Reference to RobotArm class
	UPROPERTY(VisibleAnywhere)
	URobotArm* RobotArm;

	// Sets default values for this component's properties
	UInverseKinematicsComponent();

	// Get matrix of DH parameters and initialize RobotArm class
	UFUNCTION(BlueprintCallable)
	void InitFromDHParams(UPARAM() UDataTable* DHParamsTable);

	// Compute inverse kinematics given desired end effector's coordinates
	UFUNCTION(BlueprintCallable)
	void ComputeInverseKinematics(UPARAM() const FVector& DesiredEndEffectorEPosition, UPARAM() const FVector& DesiredZAxis,
		UPARAM() const FVector& DesiredYAxis, UPARAM() int DesiredDirection, bool& Success, TArray<float>& TargetJointAngles);

	// Apply result of IK to target robot model
	UFUNCTION(BlueprintCallable)
	void SetRobotJointState(UPARAM() ARModel* Robot, UPARAM() TArray<float> JointValues);

};
