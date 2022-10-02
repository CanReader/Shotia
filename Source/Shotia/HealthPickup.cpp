#include "Shotia/HealthPickup.h"
#include "CharacterController.h"

AHealthPickup::AHealthPickup()
{
	PickupEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Pickup Effect"));
	PickupEffectComp->SetupAttachment(RootComponent);

	OverlapSphere->SetSphereRadius(60.f);
	OverlapSphere->SetRelativeLocation(FVector(0,0,30.f));
}

void AHealthPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	ACharacterController* Character = Cast<ACharacterController>(OtherActor);

	if (Character)
		Character->BuffComp->Heal(HealthAmount,HealthTime);

	Destroy();
}
