#include "Shotia/ArmorPickup.h"
#include "CharacterController.h"

AArmorPickup::AArmorPickup()
{
	PickupEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Pickup Effect"));
	PickupEffectComp->SetupAttachment(RootComponent);

	OverlapSphere->SetSphereRadius(60.f);

	Type = EPickupType::EPT_Armor;
}

void AArmorPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ACharacterController* Character = Cast<ACharacterController>(OtherActor);

	if (Character)
	{
		Character->BuffComp->Armor(ArmorAmount,LoadTime);

	Destroy();
	}
}
