#pragma once

#include "CoreMinimal.h"
#include "Shotia/Pickup.h"
#include "ArmorPickup.generated.h"

UCLASS()
class SHOTIA_API AArmorPickup : public APickup
{
	GENERATED_BODY()
public:
	AArmorPickup();

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	float ArmorAmount = 100;

	UPROPERTY(EditAnywhere)
	float LoadTime = 5;
};
