#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShoqianPlayerController.h"
#include "CharacterController.h"
#include "ShotiaGameMode.generated.h"

UCLASS()
class SHOTIA_API AShotiaGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AShotiaGameMode();

	virtual void Tick(float DeltaTime)override;

	void EliminatePlayer(ACharacterController* EliminatedPlayer, AShoqianPlayerController* VictimController, AShoqianPlayerController* Attacker);
	
	virtual void RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController);
	
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	UPROPERTY(EditDefaultsOnly)
	float WarmUpTime = 10.f;

	float LevelStartingTime = 0.f;

protected:
	virtual void BeginPlay() override;

	virtual void OnMatchStateSet() override;

private:
	UPROPERTY(EditDefaultsOnly)
		float CountDownTime = 0.f;
};
