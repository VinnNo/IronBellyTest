// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingUIActor.h"
#include "IronBellyTestProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
FBindableEvent_OnDetonationTimeChanged,
int, NewTime);

UCLASS(config=Game)
class AIronBellyTestProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:

	UPROPERTY(BlueprintAssignable, Category = Detonation)
	FBindableEvent_OnDetonationTimeChanged OnDetonationTimeChanged;

	// The Floating UI actor, which also handles pickup logic.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		AFloatingUIActor* UIActor;

	/** Only a relevant refernce if it has hit a character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector HitAttachLocation;

	// This dynamic material pointer is for the flashing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UMaterialInstanceDynamic* ProjectileMaterial;

	// Detonation count-down starting point when attached to a character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float ExplodeTimeAttached = 4.0f;

	// Detonation count-down starting point when NOT attached to a character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float ExplodeTimeUnattached = 8.0f;

	// When the flashing visual feedback starts during the detonation
	// count-down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float FlashStartTime = 3.0f;

	// Brightness value to manipulate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float FlashBrightness = 0.5f;

	// Max Brightness
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float FlashMaxBrightness = 13.5f;

	// Minimum brightness
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float FlashMinBrightness = FlashBrightness;

	// Scale brightness over time?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		bool bIncreaseBrightnessOverTime = true;

	// Brightness over time scale rate? This is multiplied by Delta
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float BrightnessOverTimeRate = 30.0f;

	// Color value to manipulate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		FColor FlashColor = FColor(127, 127, 0, 0);

	// The current time position of the count-down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float ExplodeTimeCurrent;

	// Rounded integer read-out for the count-down sequence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		int CountDownTime;

	// As the timer gets closer to zero, the flash speed increases.
	// BUT by how much? This affects color and brightness
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float CountDownMultiplier = 2.0f;

	// Whether a countdown sequence is underway
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		bool bCountDown = false;

	// > 0 Increasing brightness
	// < 0 Descreasing brightness
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		int FlashDirection = 1;

	// > 0 increasing R channel
	// < Decreasing R channel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		int ColorDirection = 1;

	// Increasing/decreasing brightness multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float FlashSpeedMultiplier = 1.0f;

	// Increase/decrease R channel speed multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		float ColorFlashMultiplier = 100.0f;

	// A simple toggle for blueprint extensions EG spawning a particle,
	// something that only needs to happen once.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detonation)
		bool bDetonated = false;





	AIronBellyTestProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void StickToActor(AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	// The flash and color adjustment for countdowns
	void UpdateFlashing(float Delta);

	void UpdateCountDown(float Delta);

	UFUNCTION(BlueprintCallable)
	void SetDetonationTimer(bool bIsAttachedTime);

	// Maybe blueprinters wanna do something with this?
	UFUNCTION(BlueprintImplementableEvent)
		void OnDetonated();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void SpawnUIActor();

	UFUNCTION(BlueprintCallable)
		void DestroyUIActor();

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns StaticMeshComp subobject **/
	UStaticMeshComponent* GetStaticMesh() const { return StaticMeshComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

