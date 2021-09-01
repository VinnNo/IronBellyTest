// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingUIActor.h"

#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

#include "Net/UnrealNetwork.h"

#include "IronBellyTestProjectile.h"

#include "IronBellyTestCharacter.h"


// Sets default values
AFloatingUIActor::AFloatingUIActor()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = RootScene;

	FloatingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingWidget"));
	FloatingWidget->SetupAttachment(RootComponent);
	FloatingWidget->SetWidgetClass(UWFloatingPickupUI::StaticClass());
	FloatingWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingWidget->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
	FloatingWidget->bOnlyOwnerSee = true;

	SightRadius = CreateDefaultSubobject<USphereComponent>(TEXT("SightRadius"));
	SightRadius->SetupAttachment(RootComponent);
	SightRadius->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel13);
	SightRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SightRadius->InitSphereRadius(300.0f);
	SightRadius->OnComponentBeginOverlap.AddDynamic(this, &AFloatingUIActor::OnOverlap);
	SightRadius->OnComponentEndOverlap.AddDynamic(this, &AFloatingUIActor::OnOverlapEnd);

	bReplicates = true;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloatingUIActor::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handler;
	GetWorldTimerManager().SetTimer(Handler, [this]()
	{
		FloatingWidget->SetWidgetSpace(EWidgetSpace::Screen);
		TArray<AActor*> Actors;
		SightRadius->GetOverlappingActors(Actors, AIronBellyTestCharacter::StaticClass());
		if (Actors.Num() > 0 && IsValid(Actors[0]))
		{
			AIronBellyTestCharacter* Character = Cast<AIronBellyTestCharacter>(Actors[0]);
			if (Character != nullptr)
			{
				Character->PickupsInRange.Add(this);
				CharactersInRange.Add(Character);
				SetHidden(false);
			}
		}
	}, 0.01f, false);
}

// Called every frame
void AFloatingUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloatingUIActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AIronBellyTestCharacter* Character = Cast<AIronBellyTestCharacter>(OtherActor);
	if (Character != nullptr)
	{
		Character->PickupsInRange.Add(this);
		CharactersInRange.Add(Character);
		SetHidden(false);
	}
}

void AFloatingUIActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AIronBellyTestCharacter* Character = Cast<AIronBellyTestCharacter>(OtherActor);
	if (Character != nullptr)
	{

		Character->PickupsInRange.Remove(this);
		CharactersInRange.Remove(Character);
		if (CharactersInRange.Num() == 0)
		{
			SetHidden(true);
		}
	}
}


void AFloatingUIActor::OnPickedUp_Implementation(AIronBellyTestCharacter* OtherCharacter)
{
	if (OtherCharacter != nullptr)
	{
		OtherCharacter->PickupsInRange.Remove(this);
		if (CharactersInRange.Num() > 0)
		{
			for (int Itr = 0; Itr < CharactersInRange.Num(); ++Itr)
			{
				CharactersInRange[Itr]->PickupsInRange.Remove(this);
			}
		}
	}

	if (ParentActor != nullptr)
	{
		Cast<AIronBellyTestProjectile>(ParentActor)->DestroyUIActor();
		ParentActor->Destroy();
		Destroy();
	}
}
