
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CharacterController.h"
#include "ShotiaLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOTIA_API AShotiaLobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:

};
