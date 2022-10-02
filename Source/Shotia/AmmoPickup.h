#pragma once

#include "CoreMinimal.h"
#include "Shotia/Pickup.h"
#include "Shotia/WeaponTypes.h"
#include "AmmoPickup.generated.h"

UCLASS()
class SHOTIA_API AAmmoPickup : public APickup
{
    GENERATED_BODY()
public:
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComponent,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult) override;

protected:

private:
    UPROPERTY(EditAnywhere)
        int32 AmmoAmount = 30.f;

    UPROPERTY(EditAnywhere)
        EWeaponType WeaponType;



};