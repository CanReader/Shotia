#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CharacterController.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Pickup.generated.h"

#define InitializeParticlePickup() PickupEffectComp =CreateDefaultSubobject<UNiagaraComponent>(TEXT("Pickup Effect"));\
PickupEffectComp->SetupAttachment(RootComponent);\
OverlapSphere->SetRelativeLocation(FVector(0, 0, 30));\
OverlapSphere->SetSphereRadius(60);

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	EPT_None UMETA(DisplayName = "NoType"),
	EPT_Ammo UMETA(DisplayName = "Ammo"),
	EPT_Speed UMETA(DisplayName = "Speed"),
	EPT_Jump UMETA(DisplayName = "Jump"),
	EPT_Armor UMETA(DisplayName = "Armor"),
	EPT_Health UMETA(DisplayName = "Health")
};

UCLASS()
class SHOTIA_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* PickupEffectComp;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* PickupEffect;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* OverlapSphere;

	UPROPERTY(BlueprintReadOnly)
	EPickupType Type;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USoundCue* PickupSound;

	FTimerHandle BindOverlapTimer;
	float BinOverlapTime = 0.25f;
	void BindOverlapFinished();
};
