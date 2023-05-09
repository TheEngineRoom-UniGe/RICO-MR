// Fill out your copyright notice in the Description page of Project Settings.


#include "InverseKinematicsComponent.h"


 //----------------------------------

ULink::ULink() {}


void ULink::SetDHParams(float Theta, float Alpha, float a, float d)
{
	Theta_ = Theta;
	Alpha_ = Alpha; 
	a_ = a;
	d_ = d;
}


Eigen::MatrixXf ULink::TransformationMatrix()
{
	float st = FMath::Sin(Theta_);
	float ct = FMath::Cos(Theta_);
	float sa = FMath::Sin(Alpha_);
	float ca = FMath::Cos(Alpha_);

	Eigen::MatrixXf T(4, 4);
	T << ct, -st * ca, st* sa, a_* ct,
		st, ct* ca, -ct * sa, a_* st,
		0, sa, ca, d_,
		0, 0, 0, 1;
	return T;
}


Eigen::VectorXf ULink::BasicJacobian(const Eigen::MatrixXf& PreviousTransform, const Eigen::Vector3f& EEPosition)
{
	Eigen::Vector3f PreviousPosition;
	PreviousPosition(0) = PreviousTransform(0, 3);
	PreviousPosition(1) = PreviousTransform(1, 3);
	PreviousPosition(2) = PreviousTransform(2, 3);

	Eigen::Vector3f PreviousZAxis;
	PreviousZAxis(0) = PreviousTransform(0, 2);
	PreviousZAxis(1) = PreviousTransform(1, 2);
	PreviousZAxis(2) = PreviousTransform(2, 2);

	Eigen::Vector3f PositionDiff = EEPosition - PreviousPosition;
	Eigen::Vector3f CP = PreviousZAxis.cross(PositionDiff);

	Eigen::VectorXf BasicJacobian(6);
	BasicJacobian(0) = CP(0);
	BasicJacobian(1) = CP(1);
	BasicJacobian(2) = CP(2);
	BasicJacobian(3) = PreviousZAxis(0);
	BasicJacobian(4) = PreviousZAxis(1);
	BasicJacobian(5) = PreviousZAxis(2);

	return BasicJacobian;
}


//---------------------------------------

URobotArm::URobotArm() {}


void URobotArm::Init(int NLinks, const Eigen::MatrixXf& DHParams)
{
	NLinks_ = NLinks;
	for (int i = 0; i < NLinks_; i++) 
	{
		float Theta = DHParams(i, 0);
		float Alpha = DHParams(i, 1);
		float a = DHParams(i, 2);
		float d = DHParams(i, 3);

		ULink* NewLink = NewObject<ULink>(this);
		NewLink->SetDHParams(Theta, Alpha, a, d);
		Links.Add(NewLink);
	}
}


Eigen::MatrixXf URobotArm::TransformationMatrix()
{
	Eigen::MatrixXf T = Eigen::MatrixXf::Identity(4, 4);
	for (int i = 0; i < NLinks_; i++)
	{
		T = T * Links[i]->TransformationMatrix();
	}
	return T;
}


Eigen::VectorXf URobotArm::ForwardKinematics(const Eigen::MatrixXf& T_e0)
{
	Eigen::Vector3f EulerAngles = GetEulerAngles(T_e0);

	Eigen::VectorXf EndEffectorPose(6);
	EndEffectorPose(0) = T_e0(0, 3);
	EndEffectorPose(1) = T_e0(1, 3);
	EndEffectorPose(2) = T_e0(2, 3);
	EndEffectorPose(3) = EulerAngles(0);
	EndEffectorPose(4) = EulerAngles(1);
	EndEffectorPose(5) = EulerAngles(2);

	return EndEffectorPose;
}


Eigen::Vector3f URobotArm::GetEulerAngles(const Eigen::MatrixXf& T_e0)
{
	Eigen::Vector3f EulerAngles;

	float alpha = FMath::Atan2(T_e0(1, 2), T_e0(0, 2));
	if (FMath::Abs(alpha) > PI/2) 
	{
		alpha = FMath::Atan2(T_e0(1, 2), T_e0(0, 2)) + PI;
	}
	if (FMath::Abs(alpha) > PI / 2)
	{
		alpha = FMath::Atan2(T_e0(1, 2), T_e0(0, 2)) - PI;
	}
	float beta = FMath::Atan2(
		T_e0(0, 2) * FMath::Cos(alpha) + T_e0(1, 2) * FMath::Sin(alpha),
		T_e0(2, 2)
	);
	float gamma = FMath::Atan2(
		-T_e0(0, 0) * FMath::Sin(alpha) + T_e0(1, 0) * FMath::Cos(alpha),
		-T_e0(0, 1) * FMath::Sin(alpha) + T_e0(1, 1) * FMath::Cos(alpha)
	);

	EulerAngles(0) = alpha;
	EulerAngles(1) = beta;
	EulerAngles(2) = gamma;

	return EulerAngles;
}


Eigen::MatrixXf URobotArm::ComputeJacobian(const Eigen::Vector3f& EEPosition)
{
	Eigen::MatrixXf Jacobian(NLinks_, 6);
	Eigen::MatrixXf T = Eigen::MatrixXf::Identity(4, 4);

	// Fill jacobian matrix with individual links' jacobians
	int RowIdx = 0;
	for (auto Link : Links)
	{
		Eigen::VectorXf LinkJacobian = Link->BasicJacobian(T, EEPosition);
		for (int ColIdx = 0; ColIdx < LinkJacobian.size(); ColIdx++)
		{
			Jacobian(RowIdx, ColIdx) = LinkJacobian(ColIdx);
		}
		// Update transformation matrix
		T = T * Links[RowIdx]->TransformationMatrix();
		RowIdx++;
	}
	Eigen::MatrixXf TransposedJacobian(6, NLinks_);
	TransposedJacobian = Jacobian.transpose();
	return TransposedJacobian;
}


void URobotArm::SetJointAngles(const Eigen::VectorXf& JointAngles)
{
	int i = 0;
	for (auto Link : Links)
	{
		Link->Theta_ = JointAngles(i);
		i++;
	}
}


void URobotArm::UpdateJointAngles(const Eigen::VectorXf& JointAnglesDiff)
{
	int i = 0;
	for (auto Link : Links)
	{
		Link->Theta_ += JointAnglesDiff(i);
		if (Link->Theta_ > PI) {
			Link->Theta_ -= 2 * PI;
		}
		else if (Link->Theta_ < -PI) {
			Link->Theta_ += 2 * PI;
		}
		i++;
	}
}


Eigen::VectorXf URobotArm::InverseKinematics(const Eigen::VectorXf& DesiredEEPose, int MaxIterations)
{
	/* Numerical computation of IK. Repeat steps until convergence */
	for (int i = 0; i < MaxIterations; i++)
	{
		// Compute forward kinematics starting from current joint configuration
		auto TEE_0 = TransformationMatrix();
		Eigen::VectorXf EEPose = ForwardKinematics(TEE_0);

		// Compute error between desired and current ee's pose
		Eigen::VectorXf EEPoseDiff = DesiredEEPose - EEPose;

		// Compute arm jacobian
		Eigen::Vector3f EEPosition;
		for (int k = 0; i < 3; i++)
		{
			EEPosition(k) = EEPose(k);
		}
		Eigen::MatrixXf J_T = ComputeJacobian(EEPosition);

		// Retrieve euler angles
		Eigen::Vector3f EulerAngles = GetEulerAngles(TEE_0);

		// Compute update matrix
		float sa = FMath::Sin(EulerAngles(0));
		float ca = FMath::Cos(EulerAngles(0));
		float sb = FMath::Sin(EulerAngles(1));
		float cb = FMath::Cos(EulerAngles(1));
		Eigen::Matrix3f K_ZYZ;
		K_ZYZ << 0, -sa,  ca* sb,
			     0,  ca,  sa * sb,
			     1,  0,   cb;
		Eigen::MatrixXf K_Alpha = Eigen::MatrixXf::Identity(6, 6);
		K_Alpha.block<3, 3>(3, 3) = K_ZYZ;

		// Update law based on jacobian pseudoinverse
		Eigen::MatrixXf J_Pinv = J_T.completeOrthogonalDecomposition().pseudoInverse();
		Eigen::VectorXf ThetaDot = J_Pinv * K_Alpha * EEPoseDiff;
		UpdateJointAngles(0.01 * ThetaDot);
	}

	Eigen::VectorXf TargetJointAngles(NLinks_);
	int j = 0;
	for (auto Link : Links) {
		TargetJointAngles(j) = Link->Theta_;
	}
	return TargetJointAngles;

}


// ----------------------

UInverseKinematicsComponent::UInverseKinematicsComponent() {}


void UInverseKinematicsComponent::InitFromDHParams(UPARAM() UDataTable* DHParamsTable)
{
	// Instantiate robot arm ptr
	RobotArm = NewObject<URobotArm>(this);

	// Create eigen matrix of DH params from unreal's data table asset
	TArray<FDHParamsStruct*> DHParamsArray;
	DHParamsTable->GetAllRows<FDHParamsStruct>(TEXT("DHParams"), DHParamsArray);
	
	int i = 0;
	int NLinks = DHParamsArray.Num();
	Eigen::MatrixXf M_DHParams(NLinks, 4);
	for (auto Row : DHParamsArray)
	{
		M_DHParams(i, 0) = Row->Theta;
		M_DHParams(i, 1) = Row->Alpha;
		M_DHParams(i, 2) = Row->a;
		M_DHParams(i, 3) = Row->d;
		i++;
	}

	// Initialize Robot arm's kinematic chain
	RobotArm->Init(NLinks, M_DHParams);

	auto TEE_0 = RobotArm->TransformationMatrix();
	auto EEPose = RobotArm->ForwardKinematics(TEE_0);
	Eigen::Vector3f EEPos;
	EEPos(0) = EEPose(0);
	EEPos(1) = EEPose(1);
	EEPos(2) = EEPose(2);
	Eigen::MatrixXf J_T = RobotArm->ComputeJacobian(EEPos);
	for (int j = 0; j < J_T.rows(); j++) {
		UE_LOG(LogTemp, Log, TEXT("%f, %f, %f, %f, %f, %f"), J_T(j,0), J_T(j, 1), J_T(j, 2), J_T(j, 3), J_T(j, 4), J_T(j, 5));
	}
}


void UInverseKinematicsComponent::ComputeInverseKinematics(UPARAM() TArray<float> DesiredEEPose, TArray<float>& TargetJointAngles)
{

}

