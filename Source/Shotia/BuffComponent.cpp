#include "Shotia/BuffComponent.h"
#include "CharacterController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuffComponent::SetInitialSpeeds(float BaseSpeed, float BaseCrouchSpeed)
{
	InitialSpeed = BaseSpeed;
	InitialCrouchSpeed = BaseCrouchSpeed;
}

void UBuffComponent::SetInitialJump(float BaseJump)
{
	InitialJumpVelocity = BaseJump;
}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	if(Character == nullptr) return;
	
	bIsHealing = true;
	HealRate = HealAmount / HealingTime;
	AmountToHealth += HealAmount;
}

void UBuffComponent::Accelerate(float BuffSpeed, float BuffCrouchSpeed, float Duration)
{
	if(Character == nullptr) return;

	Character->GetWorld()->GetTimerManager().SetTimer(SpeedBuffTimer,this,&UBuffComponent::ResetSpeeds,Duration);

	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BuffSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = BuffCrouchSpeed;
	}

	MulticastAccelerate(BuffSpeed,BuffCrouchSpeed);
}

void UBuffComponent::JumpBoost(float BoostPower, float Duration)
{
	if (Character == nullptr) return;

	Character->GetWorld()->GetTimerManager().SetTimer(JumpBoostTimer, this, &UBuffComponent::ResetJump, Duration);
	
	Character->GetCharacterMovement()->JumpZVelocity = BoostPower;

	MulticastJump(BoostPower);
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RaiseHealth(DeltaTime);
}

void UBuffComponent::RaiseHealth(float DeltaTime)
{
	if (!bIsHealing || Character == nullptr || Character->GetIsEliminated()) return;

	const float HealAmountThisFrame = HealRate * DeltaTime;

	Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealAmountThisFrame,0,Character->GetMaxHealth()));
	Character->UpdateHUD();

	AmountToHealth -= HealAmountThisFrame;
	
	if (Character->GetHealth() >= Character->GetMaxHealth())
	{
		bIsHealing = false;
		AmountToHealth = 0.f;
	}

}

void UBuffComponent::ResetSpeeds()
{
	if (Character == nullptr || Character->GetCharacterMovement() == nullptr) return;

	Character->GetCharacterMovement()->MaxWalkSpeed = InitialSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = InitialCrouchSpeed;

	MulticastAccelerate(InitialSpeed, InitialCrouchSpeed);
}

void UBuffComponent::ResetJump()
{
	if (Character == nullptr || Character->GetCharacterMovement() == nullptr) return;

	Character->GetCharacterMovement()->JumpZVelocity = InitialJumpVelocity;

	MulticastJump(InitialJumpVelocity);

}

void UBuffComponent::MulticastAccelerate_Implementation(float BuffSpeed, float BuffSpeedCrouched)
{
	if (Character == nullptr) return;

	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BuffSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = BuffSpeedCrouched;
	}
}

void UBuffComponent::MulticastJump_Implementation(float  Boost)
{
	if (Character == nullptr) return;

	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = Boost;
	}
}