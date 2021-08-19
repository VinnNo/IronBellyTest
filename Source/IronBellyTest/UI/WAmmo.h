// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"

#include "../IronBellyTestCharacter.h"

#include "WAmmo.generated.h"

/**
 * 
 */
UCLASS()
class IRONBELLYTEST_API UWAmmo : public UUserWidget
{
	GENERATED_BODY()
public:
	// Items
	UPROPERTY(BlueprintReadOnly, Category = "Widget Items")
		UTextBlock* AmmoCountLabel = nullptr;

	// Items
	UPROPERTY(BlueprintReadOnly, Category = "Widget Items")
		UTextBlock* AmmoTextLabel = nullptr;

	UFUNCTION(BlueprintCallable)
		void InitializeWidget();

	UFUNCTION(BlueprintCallable)
		void OnAmmoChanged(int InAmmo);

	UWAmmo(const FObjectInitializer& ObjectInitializer);

	virtual TSharedRef<SWidget> RebuildWidget() override;
};
