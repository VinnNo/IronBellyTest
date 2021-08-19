// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IronBellyTestHUD.generated.h"

UCLASS()
class AIronBellyTestHUD : public AHUD
{
	GENERATED_BODY()

public:
	AIronBellyTestHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	class UWAmmo* AmmoWidget = nullptr;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

