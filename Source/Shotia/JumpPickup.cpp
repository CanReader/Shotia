#include "Shotia/JumpPickup.h"

AJumpPickup::AJumpPickup()
{
	PickupEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Pickup Effect"));
	PickupEffectComp->SetupAttachment(RootComponent);

	OverlapSphere->SetRelativeLocation(FVector(0,0,30));
	OverlapSphere->SetSphereRadius(60);

	Type = EPickupType::EPT_Jump;
}

void AJumpPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent,OtherActor, OtherComp,OtherBodyIndex,bFromSweep,SweepResult);

	auto Character = Cast<ACharacterController>(OtherActor);

	if (Character && Character->BuffComp)
	{
		Character->BuffComp->JumpBoost(BoostPower,JumpBoostTime);

	Destroy();
	}
}
