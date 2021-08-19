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
	FloatingWidget->SetWidgetSpace(EWidgetSpace::Screen);
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
		int Ammo = OtherCharacter->AmmoInClip += 1;
		OtherCharacter->OnAmmoChanged.Broadcast(Ammo);
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
		// For some reason there is a pretty big delay when playing in client mode
		// for destroying this actor. That's why I've gotten a LITTLE desperate here. 
		SetActorHiddenInGame(true);
		Cast<AIronBellyTestProjectile>(ParentActor)->DestroyUIActor();
		ParentActor->Destroy();
	}
}

void AFloatingUIActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(AFloatingUIActor, ParentActor);
}

