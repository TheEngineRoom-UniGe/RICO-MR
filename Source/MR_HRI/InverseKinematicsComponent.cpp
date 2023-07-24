// Fill out your copyright notice in the Description page of Project Settings.


#include "InverseKinematicsComponent.h"


 //----------------------------------

ULink::ULink() {}


void ULink::SetDHParams(float Theta, float Alpha, float a, float d)
{
	this->Theta_ = Theta;
	this->Alpha_ = Alpha;
	this->a_ = a;
	this->d_ = d;
}


void ULink::SetTheta(float Theta) 
{
	this->Theta_ = Theta;
}


float ULink::GetTheta()
{
	return this->Theta_;
}


Eigen::MatrixXf ULink::TransformationMatrix()
{
	float st = FMath::Sin(this->Theta_);
	float ct = FMath::Cos(this->Theta_);
	float sa = FMath::Sin(this->Alpha_);
	float ca = FMath::Cos(this->Alpha_);

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
	//const std::lock_guard<std::mutex> lock(Mutex);
	
	int i = 0;
	for (auto Link : Links)
	{
		if (!Link->IsValidLowLevel())
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Failed Setting Link!"));
			return;

		Link->SetTheta(JointAngles(i));
		i++;
	}
}


void URobotArm::UpdateJointAngles(const Eigen::VectorXf& JointAnglesDiff)
{
	//const std::lock_guard<std::mutex> lock(Mutex);

	int i = 0;
	for (auto Link : Links)
	{
		if (!Link->IsValidLowLevel())
			return;

		float NewTheta = Link->GetTheta() + JointAnglesDiff(i);
		if (NewTheta > PI) {
			NewTheta -= 2 * PI;
		}
		else if (NewTheta < -PI) {
			NewTheta += 2 * PI;
		}
		Link->SetTheta(NewTheta);
		i++;
	}
}


Eigen::VectorXf URobotArm::GetJointAngles()
{
	//const std::lock_guard<std::mutex> lock(Mutex);

	Eigen::VectorXf JointAngles(NLinks_);
	int i = 0;
	for (auto Link : Links) {

		if (!Link->IsValidLowLevel()) {
			Eigen::VectorXf dummy(1);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Failed Getting Link!"));
			return dummy;
		}
			
		JointAngles(i) = Link->GetTheta();
		i++;
	}
	return JointAngles;
}


/*Eigen::MatrixXf URobotArm::PseudoInverse(const Eigen::MatrixXf& Matrix)
{
	float Tolerance = 1e-4;

	auto svd = Matrix.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV);
	const auto& singularValues = svd.singularValues();
	Eigen::MatrixXf singularValuesInv(Matrix.cols(), Matrix.rows());
	singularValuesInv.setZero();
	for (unsigned int i = 0; i < singularValues.size(); ++i) {
		if (singularValues(i) > Tolerance)
		{
			singularValuesInv(i, i) = 1.0f / singularValues(i);
		}
		else
		{
			singularValuesInv(i, i) = 0.0f;
		}
	}
	return svd.matrixV() * singularValuesInv * svd.matrixU().adjoint();
}*/


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
		for (int k = 0; k < 3; k++)
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
		Eigen::MatrixXf Tmp = J_Pinv * K_Alpha;
		Eigen::VectorXf ThetaDot = Tmp * EEPoseDiff;
		UpdateJointAngles(0.01 * ThetaDot);
	}

	Eigen::VectorXf TargetJointAngles(NLinks_);
	int j = 0;
	for (auto Link : Links) {
		TargetJointAngles(j) = Link->GetTheta();
		j++;
	}
	return TargetJointAngles;

}


// ----------------------

UInverseKinematicsComponent::UInverseKinematicsComponent() {}


void UInverseKinematicsComponent::InitIKUtilsParams(UPARAM() UDataTable* IKUtilsTable, int& OutDirection, FVector& OutEEOffset)
{
	TArray<FIKUtilsStruct*> IKUtilsArray;
	IKUtilsTable->GetAllRows<FIKUtilsStruct>(TEXT("IKUtils"), IKUtilsArray);
	for (auto Row : IKUtilsArray)
	{
		OutDirection = Row->Direction;
		OutEEOffset.X = Row->EndEffectorOffset.X;
		OutEEOffset.Y = Row->EndEffectorOffset.Y;
		OutEEOffset.Z = Row->EndEffectorOffset.Z;
		for (auto JN : Row->JointNames) {
			IKJoints.Add(JN);
		}
		for (auto JM : Row->JointMultiplierValues) {
			IKJointMultiplierArray.Add(JM);
		}
		for (auto JV : Row->JointBaseValues) {
			IKJointBaseValuesArray.Add(JV);
		}
		InvertAxes = Row->InvertAxes;
	}
}


void UInverseKinematicsComponent::InitFromDHParams(UPARAM() UDataTable* DHParamsTable, int& OutNLinks)
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
	UE_LOG(LogTemp, Log, TEXT("End Effector' Initial Pose:"));
	for (int j = 0; j < EEPose.size(); j++) {
		UE_LOG(LogTemp, Log, TEXT("%f"), EEPose(j));
	}
	Eigen::MatrixXf J_T = RobotArm->ComputeJacobian(EEPos);

	// Return number of links to calling blueprint
	OutNLinks = NLinks;
}


void UInverseKinematicsComponent::ComputeInverseKinematics(UPARAM() const FVector& DesiredEndEffectorEPosition, UPARAM() const FVector& DesiredZAxis,
	UPARAM() const FVector& DesiredYAxis, UPARAM() int DesiredDirection, bool& Success, TArray<float>& TargetJointAngles)
{
	//Eigen::VectorXf ZeroVector = Eigen::VectorXf::Zero(6);
	//RobotArm->SetJointAngles(ZeroVector);

	// rewrite vectors in eigen format, including conversion to right-hand rule
	Eigen::Vector3f ZAxis;
	ZAxis(0) = -DesiredZAxis.X;
	ZAxis(1) = DesiredZAxis.Y;
	ZAxis(2) = DesiredZAxis.Z;

	Eigen::Vector3f YAxis;
	YAxis(0) = DesiredYAxis.X;
	YAxis(1) = DesiredYAxis.Y;
	YAxis(2) = DesiredYAxis.Z;

	// Define reference unit vectors
	Eigen::Vector3f XWorld;
	XWorld(0) = 1;
	XWorld(1) = 0;
	XWorld(2) = 0;

	Eigen::Vector3f YWorld;
	YWorld(0) = 0;
	YWorld(1) = 1;
	YWorld(2) = 0;

	Eigen::Vector3f ZWorld;
	ZWorld(0) = 0;
	ZWorld(1) = 0;
	ZWorld(2) = 1;

	// Compute rotation axis needed to align ZWorld with ZAxis
	Eigen::Vector3f RotAxis = ZWorld.cross(ZAxis);
	RotAxis /= RotAxis.norm();

	// Alpha angle is the one between rotation axis and world Y vector
	float Alpha = FMath::Acos(RotAxis.dot(YWorld));
	if (Alpha > PI / 2) {
		Alpha -= PI;
	}
	// Find direction of rotation based on cross product and plane normal
	Eigen::Vector3f C = YWorld.cross(RotAxis);
	float D = ZWorld.dot(C);
	if (D < 0) {
		Alpha = -Alpha;
	}

	// Beta angle is the one between Z vectors
	float Beta = FMath::Acos(ZAxis.dot(ZWorld));
	// Find direction of rotation based on cross product and plane normal
	Eigen::Vector3f E = XWorld.cross(RotAxis);
	float F = ZWorld.dot(E);
	if (F < 0) {
		Beta = -Beta;
	}

	// Gamma angle is the one between target Y axis and rotation axis
	float Gamma = FMath::Acos(YAxis.dot(RotAxis));
	// Find direction of rotation based on cross product and plane normal
	Eigen::Vector3f H = YAxis.cross(RotAxis);
	float M = ZWorld.dot(H);
	if (M < 0) {
		Gamma = -Gamma;
	}

	Eigen::VectorXf DesiredEEPose(6);
	DesiredEEPose(0) = DesiredDirection * DesiredEndEffectorEPosition.X / 100.0f;
	DesiredEEPose(1) = -DesiredDirection * DesiredEndEffectorEPosition.Y / 100.0f;
	if (InvertAxes) {
		DesiredEEPose(0) = DesiredDirection * DesiredEndEffectorEPosition.Y / 100.0f;
		DesiredEEPose(1) = DesiredDirection * DesiredEndEffectorEPosition.X / 100.0f;
	}
	DesiredEEPose(2) = DesiredEndEffectorEPosition.Z / 100.0f;
	DesiredEEPose(3) = Alpha;
	DesiredEEPose(4) = Beta;
	DesiredEEPose(5) = Gamma;

	UE_LOG(LogTemp, Log, TEXT("Desired End Effector Pose:"));
	for (int j = 0; j < DesiredEEPose.size(); j++) {
		UE_LOG(LogTemp, Log, TEXT("%f"), DesiredEEPose(j));
	}
	UE_LOG(LogTemp, Log, TEXT("--------"));

	/*Eigen::VectorXf LastJointAngles = RobotArm->GetJointAngles();
	if (LastJointAngles.size() != RobotArm->NLinks_) {
		Success = false;
		return;
	}*/
	Eigen::VectorXf TargetJointAnglesVector = RobotArm->InverseKinematics(DesiredEEPose, this->MaxIterations);

	auto TEE_0 = RobotArm->TransformationMatrix();
	Eigen::VectorXf EEPose = RobotArm->ForwardKinematics(TEE_0);
	UE_LOG(LogTemp, Log, TEXT("End Effector Pose After IK:"));
	for (int j = 0; j < EEPose.size(); j++) {
		UE_LOG(LogTemp, Log, TEXT("%f"), EEPose(j));
	}
	UE_LOG(LogTemp, Log, TEXT("--------"));

	/*for (int j = 0; j < TargetJointAnglesVector.size(); j++) {
		if (isnan(TargetJointAnglesVector(j))) {
			Success = false;
			RobotArm->SetJointAngles(LastJointAngles);
			return;
		}
		else {
			TargetJointAngles.Add(TargetJointAnglesVector(j));
		}
	}*/
	for (int j = 0; j < TargetJointAnglesVector.size(); j++) {
		TargetJointAngles.Add(TargetJointAnglesVector(j));
	}
	
	Success = true;
	return;
}


void UInverseKinematicsComponent::SetRobotJointState(UPARAM() ARModel* Robot, 
	UPARAM() TArray<float> JointValues)
{
	int i = 0;
	FHitResult Hit;

	if (!IsValid(Robot))
		return;

	UE_LOG(LogTemp, Log, TEXT("------"));
	for (auto IKJoint : IKJoints) {
		float vi = IKJointMultiplierArray[i] * (IKJointBaseValuesArray[i] + JointValues[i]);
		UE_LOG(LogTemp, Log, TEXT("%f"), vi);
		Robot->GetJoint(IKJoint)->SetJointPosition(vi, &Hit);
		i++;
	}
}

