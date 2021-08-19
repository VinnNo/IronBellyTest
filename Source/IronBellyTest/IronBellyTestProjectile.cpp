// Copyright Epic Games, Inc. All Rights Reserved.

#include "IronBellyTestProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "IronBellyTestCharacter.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

#include "StaticsLibrary.h"

#include "Components/SphereComponent.h"



AIronBellyTestProjectile::AIronBellyTestProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AIronBellyTestProjectile::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AIronBellyTestProjectile::OnOverlap);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetCollisionProfileName("NoCollision");
	StaticMeshComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComp->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));



	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bShouldBounce = true;

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AIronBellyTestProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Bailout
	if (OtherActor == nullptr)
	{
		return;
	}
	
	// Should we stick?
	if ((OtherActor != GetOwner()) &&
		(OtherActor->IsA(AIronBellyTestCharacter::StaticClass())))
	{
		StickToActor(OtherActor);
		SetDetonationTimer(true);
		return;
	}

	//Bailout
	if (OtherComp == nullptr)
	{
		return;
	}

	//Might as well keep this
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	// At this point we've hit SOMETHING, so let's head to the ground to be picked up.
	ProjectileMovement->ProjectileGravityScale = 1.0f;
}

void AIronBellyTestProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// Should we stick?
	if ((OtherActor != GetOwner()) &&
		(OtherActor->IsA(AIronBellyTestCharacter::StaticClass())))
	{
		StickToActor(OtherActor);
		SetDetonationTimer(true);
		return;
	}
}

void AIronBellyTestProjectile::StickToActor(AActor* OtherActor)
{
	ProjectileMovement->StopMovementImmediately();
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
}

void AIronBellyTestProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Step through and create a dynamic material instance, for the flash 
	// effect down the road, if needed.

	// This should probably be handled in an array or keylist and a loop
	// for scalability reasons. BUT for this project, we know there is 
	// only ever going to be the one material slot AKA slot 0 (Zero).
	
	UMaterialInterface* Mat = GetStaticMesh()->GetMaterial(0);
	if (Mat != nullptr)
	{
		ProjectileMaterial = GetStaticMesh()->CreateDynamicMaterialInstance(0, Mat);
	}

	SetDetonationTimer(false);
	SpawnUIActor();
	if (UIActor)
	{
		UIActor->SetActorHiddenInGame(true);
	}
}

void AIronBellyTestProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCountDown)
	{
		UpdateCountDown(DeltaTime);
	}
}

void AIronBellyTestProjectile::UpdateCountDown(float Delta)
{
	ExplodeTimeCurrent -= Delta;
	int Time = round(ExplodeTimeCurrent);

	if (CountDownTime != Time)
	{
		CountDownTime = Time;
		OnDetonationTimeChanged.Broadcast(CountDownTime);
		if (CountDownTime == 0 && !bDetonated)
		{
			OnDetonated();
			bDetonated = true;
		}
	}

	if (CountDownTime <= 3)
	{
		UpdateFlashing(Delta);
	}

	if (ExplodeTimeCurrent <= 0.0f)
	{
		if (UIActor)
		{
			UIActor->Destroy();
		}
		Destroy();
	}
}

void AIronBellyTestProjectile::UpdateFlashing(float Delta)
{
	if (ProjectileMaterial != nullptr)
	{

		// 
		float Min = FlashMinBrightness, Max = FlashMaxBrightness, 
		TempColorRed = FlashColor.R, TempColorGreen = FlashColor.G;

		// Material parameter names
		FName ParameterBrightness = "Brightness", ParameterColor = "DiffuseColor";

		float FlashSpeed = ( FlashStartTime * (FlashStartTime - CountDownTime) * CountDownMultiplier );

		// Brightness
		if (FlashDirection > 0)
		{
			(FlashBrightness >= Max) ? FlashDirection = -1 : FlashBrightness = UStaticsLibrary::Approach(FlashBrightness, Max, (FlashSpeed * FlashSpeedMultiplier * Delta));
		}
		else
		{
			(FlashBrightness <= Min) ? FlashDirection = 1 : FlashBrightness = UStaticsLibrary::Approach(FlashBrightness, Min, (FlashSpeed * FlashSpeedMultiplier * Delta));
		}

		// Color (red channel)
		if (ColorDirection > 0)
		{
			(TempColorRed >= 255.0f) ? ColorDirection = -1 : TempColorRed =  UStaticsLibrary::Approach(TempColorRed, 255.0f, (FlashSpeed * ColorFlashMultiplier * Delta));
		}
		else
		{
			(TempColorRed <= 127.0f) ? ColorDirection = 1 : TempColorRed =  UStaticsLibrary::Approach(TempColorRed, 127.0f, (FlashSpeed * ColorFlashMultiplier * Delta));
		}

		// Get brighter over time
		if (bIncreaseBrightnessOverTime)
		{
			FlashMaxBrightness += (BrightnessOverTimeRate * Delta);
		}

		// Ditch the green channel
		if (TempColorGreen > 0.0f)
		{
			TempColorGreen -= (ColorFlashMultiplier * Delta);
		}

		// Finalize color values
		FlashColor.R = round(TempColorRed);
		FlashColor.G = round(TempColorGreen);
		
		// Apply what we've gathered
		ProjectileMaterial->SetScalarParameterValue(ParameterBrightness, FlashBrightness);
		ProjectileMaterial->SetVectorParameterValue(ParameterColor, FlashColor);
	}
}

void AIronBellyTestProjectile::SetDetonationTimer(bool bIsAttachedTime)
{
	(bIsAttachedTime) ? ExplodeTimeCurrent = ExplodeTimeAttached : ExplodeTimeCurrent = ExplodeTimeUnattached;
	bCountDown = true;
}

void AIronBellyTestProjectile::SpawnUIActor_Implementation()
{
	if (!UIActor)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Owner = this;
		ActorSpawnParams.Instigator = GetInstigator();

		UWorld* const World = GetWorld();

		UIActor = World->SpawnActor<AFloatingUIActor>(AFloatingUIActor::StaticClass(), GetActorLocation(), GetActorRotation(), ActorSpawnParams);

		if (UIActor)
		{
			UIActor->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
			UIActor->ParentActor = this;
		}
	}
}

void AIronBellyTestProjectile::DestroyUIActor()
{
	if (UIActor)
	{
		UIActor->Destroy();
		UIActor = nullptr;
	}
}
