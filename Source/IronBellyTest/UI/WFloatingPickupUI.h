// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"

#include "WFloatingPickupUI.generated.h"

/**
 * 
 */
UCLASS()
class IRONBELLYTEST_API UWFloatingPickupUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// Items
	UPROPERTY(BlueprintReadOnly, Category = "Widget Items")
		UTextBlock* PickupTextLabel = nullptr;

	// Items
	UPROPERTY(BlueprintReadOnly, Category = "Widget Items")
		UCanvasPanel* CanvasPanel = nullptr;

	UWFloatingPickupUI(const FObjectInitializer& ObjectInitializer);

	virtual TSharedRef<SWidget> RebuildWidget() override;
};
