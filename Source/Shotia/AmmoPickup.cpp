#include "Shotia/AmmoPickup.h"
#include "CharacterController.h"
#include "CombatComponent.h"

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent,OtherActor,OtherComponent,OtherBodyIndex,bFromSweep,SweepResult);

	ACharacterController* Player = Cast<ACharacterController>(OtherActor);

	if (Player && Player->Combat)
		Player->Combat->AddPickupAmmo(WeaponType,AmmoAmount);

	Destroy();
}
