// Fill out your copyright notice in the Description page of Project Settings.


#include "WAmmo.h"

#include "Engine.h"


UWAmmo::UWAmmo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWAmmo::InitializeWidget()
{
	APawn* PlayerPawn = GetOwningPlayerPawn();

	AIronBellyTestCharacter* Player = Cast<AIronBellyTestCharacter>(PlayerPawn);

	if (Player != nullptr)
	{
		Player->OnAmmoChanged.AddDynamic(this, &UWAmmo::OnAmmoChanged);
	}
}

void UWAmmo::OnAmmoChanged(int InAmmo)
{
	APawn* PlayerPawn = GetOwningPlayerPawn();

	AIronBellyTestCharacter* Player = Cast<AIronBellyTestCharacter>(PlayerPawn);

	if (AmmoCountLabel)
	{
		AmmoCountLabel->SetText(FText::FromString(FString::FromInt(InAmmo)));
	}
}


TSharedRef<SWidget> UWAmmo::RebuildWidget()
{
	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

	if (!RootWidget)
	{
		if (WidgetTree)
		{
			RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootWidget"));
		}
		UCanvasPanelSlot* RootWidgetSlot = Cast<UCanvasPanelSlot>(RootWidget->Slot);
		if (RootWidgetSlot)
		{
			RootWidgetSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
			RootWidgetSlot->SetOffsets(FMargin(0.0f, 0.0f));
		}
		WidgetTree->RootWidget = RootWidget;
	}

	// This seems to only work the way I need it to when it's here.
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		if (AmmoTextLabel == nullptr)
		{
			AmmoTextLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());//, TEXT("AmmoTextLabel"));
			RootWidget->AddChild(AmmoTextLabel);
			AmmoTextLabel->SetText(FText::FromString(TEXT("Ammo:")));
		}

		if (AmmoCountLabel == nullptr)
		{
			AmmoCountLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());//, TEXT("AmmoCountLabel"));
			RootWidget->AddChild(AmmoCountLabel);
			UCanvasPanelSlot* AmmoCountAsSlot = Cast<UCanvasPanelSlot>(AmmoCountLabel->Slot);
			AmmoCountAsSlot->SetAlignment(FVector2D(-1.25f, 0.0f));
		}
	}

	return Widget;
}

