#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ShoqianPlayerState.h"
#include "ShotiaGameState.generated.h"

UCLASS()
class SHOTIA_API AShotiaGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	void UpdateTopPlayers(AShoqianPlayerState* Player);

	UPROPERTY(Replicated)
	TArray<AShoqianPlayerState*> TopPlayers;

private:
	float TopKills;
};
