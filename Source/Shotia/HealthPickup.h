#pragma once

#include "CoreMinimal.h"
#include "Shotia/Pickup.h"
#include "Kismet/GameplayStatics.h"


#include "HealthPickup.generated.h"

UCLASS()
class SHOTIA_API AHealthPickup : public APickup
{
	GENERATED_BODY()
public:
	AHealthPickup();

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:

	UPROPERTY(EditAnywhere)
	float HealthAmount = 100.f;

	UPROPERTY(EditAnywhere)
	float HealthTime = 5.f;

	
};
