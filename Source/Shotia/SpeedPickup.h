#pragma once

#include "CoreMinimal.h"
#include "Shotia/Pickup.h"
#include "SpeedPickup.generated.h"

UCLASS()
class SHOTIA_API ASpeedPickup : public APickup
{
	GENERATED_BODY()

public:
	    ASpeedPickup();

		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
	float BuffSpeed = 1600.f;
	
	UPROPERTY(EditAnywhere)
	float BuffCrouchSpeed = 850.f;

	UPROPERTY(EditAnywhere)
	float SpeedTime = 300.f;
};
