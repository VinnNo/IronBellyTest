// Fill out your copyright notice in the Description page of Project Settings.


#include "WFloatingPickupUI.h"

#include "Kismet/GameplayStatics.h"

#include "Engine.h"

UWFloatingPickupUI::UWFloatingPickupUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWFloatingPickupUI::NativeConstruct()
{
	Super::NativeConstruct();
	FTimerHandle Handler;

	GetWorld()->GetTimerManager().SetTimer(Handler, [this]()
	{
		RebuildWidget();
	}, 0.1f, false);
}

TSharedRef<SWidget> UWFloatingPickupUI::RebuildWidget()
{
	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

	if (!RootWidget)
	{
		if (WidgetTree)
		{
			RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootWidget"));
		}
		UCanvasPanelSlot* RootWidgetAsSlot = Cast<UCanvasPanelSlot>(RootWidget->Slot);
		if (RootWidgetAsSlot)
		{
			RootWidgetAsSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
			RootWidgetAsSlot->SetOffsets(FMargin(0.0f, 0.0f));
		}
		WidgetTree->RootWidget = RootWidget;
	}

	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		if (PickupTextLabel == nullptr)
		{
			PickupTextLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			RootWidget->AddChild(PickupTextLabel);
		}
		PickupTextLabel->SetText(FText::FromString(TEXT("Press E to Pickup")));
		UCanvasPanelSlot* TextAsSlot = Cast<UCanvasPanelSlot>(PickupTextLabel->Slot);
		TextAsSlot->SetAlignment(FVector2D(-1.5F, -5.0f));
	}

	return Widget;
}
