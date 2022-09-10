// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CharacterOverlay.h"
#include "WarmupWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOTIA_API UWarmupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta=(BindWidget))
		UTextBlock* AnnouncementText;
	
	UPROPERTY(meta=(BindWidget))
		UTextBlock* WarmupTime;
};
