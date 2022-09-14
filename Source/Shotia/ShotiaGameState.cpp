#include "Shotia/ShotiaGameState.h"
#include "Net/UnrealNetwork.h"

void AShotiaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShotiaGameState, TopPlayers);
}

void AShotiaGameState::UpdateTopPlayers(AShoqianPlayerState* Player)
{
	if (TopPlayers.Num() == 0)
	{
		TopPlayers.Add(Player);
		TopKills = Player->GetScore();
	}
	else if (Player->GetScore() == TopKills)
		TopPlayers.AddUnique(Player);
	else if (Player->GetScore() > TopKills)
	{
		TopPlayers.Empty();
		TopPlayers.AddUnique(Player);
		TopKills = Player->GetScore();
	}
}
