// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<ACharacterController>(TryGetPawnOwner());

}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(Player == nullptr)
		Player = Cast<ACharacterController>(TryGetPawnOwner());

	if (Player == nullptr)
		return;

	FVector Velocity = Player->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	IsinAir = Player->GetCharacterMovement()->IsFalling();

	IsRunning = Player->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;

	IsEquipped = Player->GetWeaponEquipped();

	IsCrouching = Player->bIsCrouched;

	IsAimimg = Player->GetIsAiming();

	Weapon = Player->GetWeapon();

	TurnDirection = Player->GetTurnDirection();

	FRotator AimRotator = Player->GetBaseAimRotation();
	FRotator MoveRotator = UKismetMathLibrary::MakeRotFromX(Player->GetVelocity()
	);
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MoveRotator,AimRotator);

	DeltaRotation = FMath::RInterpTo(DeltaRotation,DeltaRot,DeltaTime,3);
	YawOffset = DeltaRotation.Yaw;

	LastFrameCharacterRotation = CharacterRotation;
	CharacterRotation = Player->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation,LastFrameCharacterRotation);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean,Target,DeltaTime,6.0f);

	Lean = FMath::Clamp(Interp,-90,90);

	AO_Yaw = Player->GetAOYaw();
	AO_Pitch = Player->GetAOPitch();

	/*
	Store transform of LeftHandSocket
	Transform LeftHandSocket to BoneSpace
	Set new location and rotation of LeftHandSocket
	*/

	if (IsEquipped && Weapon && Weapon->GetWeaponMesh() && Player->GetMesh())
	{
		FVector OutPos;
		FRotator OutRot;

		LeftHandTransform = Weapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"));

		Player->GetMesh()->TransformToBoneSpace(FName("hand_r"),LeftHandTransform.GetLocation(),FRotator::ZeroRotator,OutPos,OutRot);

		LeftHandTransform.SetLocation(OutPos);
		LeftHandTransform.SetRotation(FQuat(OutRot));
	}

	bRotateRootBone = Player->GetProxyRotateRootBone();

	IsEliminated = Player->GetIsEliminated();

	bUseFabrik = Player->GetCombatState() == ECombatState::ECS_Unoccupied;
}