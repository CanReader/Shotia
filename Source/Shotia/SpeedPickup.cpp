#include "Shotia/SpeedPickup.h"


ASpeedPickup::ASpeedPickup()
{
	PickupEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Pickup Effect"));
	PickupEffectComp->SetupAttachment(RootComponent);

	Type = EPickupType::EPT_Speed;
}

void ASpeedPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	ACharacterController* Character = Cast<ACharacterController>(OtherActor);

	if (Character && Character->BuffComp)
	{
		Character->BuffComp->Accelerate(BuffSpeed,BuffCrouchSpeed,SpeedTime);

	Destroy();
	}
}
