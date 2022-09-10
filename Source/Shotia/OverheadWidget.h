#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Containers/EnumAsByte.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

UCLASS()
class SHOTIA_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoleBlock;

	UFUNCTION(BlueprintCallable, Category = "Networking")
	void SetRoleText(APawn* OwnPawn);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetNameText(FString Name);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	FString GetSteamName(APawn* OwnPawn);

protected:

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

private:
	TEnumAsByte<ENetRole> GetRole(APawn* p);
	
};
