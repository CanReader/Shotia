#include "ShotiaLobbyGameMode.h"
#include "GameFramework/GameStateBase.h"


void AShotiaLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 PlayerCount = GameState.Get()->PlayerArray.Num();

	if (PlayerCount >= 2)
	{
		UWorld* World = GetWorld();

		bUseSeamlessTravel = true;
		World->ServerTravel(FString(""));
	}
}