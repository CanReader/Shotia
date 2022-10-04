// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class SHOTIA_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

private:
	//Functions

public:
	//Properties
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ArmorBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Kills;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Deaths;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MagazineAmount;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* AmmoBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchCountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GrenadeCountText;

	FORCEINLINE void SetHealthBar(float NewValue) { HealthBar->Percent = NewValue; }

	FORCEINLINE void SetHealthText(FString NewValue) { HealthText->SetText(FText::FromString(NewValue));}
};
