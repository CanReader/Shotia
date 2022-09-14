#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CharacterController.h"
#include "ShoqianPlayerState.generated.h"

UCLASS()
class SHOTIA_API AShoqianPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

    virtual void OnRep_Score() override;

    UFUNCTION()
    virtual void OnRep_DeathCount();

    void IncreaseKillBy(int Amount);
    void IncreaseDeathBy(int Amount);

private:
    ACharacterController* Character;
    AShoqianPlayerController* Player;

    UPROPERTY(ReplicatedUsing=OnRep_DeathCount)
    int DeathCount;

};
