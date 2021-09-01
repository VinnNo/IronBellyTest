// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FloatingUIActor.h"
#include "IronBellyTestCharacter.generated.h"

 // Update ammo count to those listening
 // **
 // The Ammo widget (UWAmmo) binds to this
 // when it is created by the HUD
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
FBindableEvent_AmmoChanged,
int, NewAmmoCount);

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AIronBellyTestCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* L_MotionController;


public:
	AIronBellyTestCharacter();

protected:
	virtual void BeginPlay();

public:

	UPROPERTY(BlueprintAssignable, Category = "Gameplay")
		FBindableEvent_AmmoChanged OnAmmoChanged;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	// Actually, I high-jacked this. All it really does is set an offset from 
	// screenspace, using Z
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset = FVector(0.0f, 0.0f, 40.0f);



	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AIronBellyTestProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace)
		TArray<AFloatingUIActor*> PickupsInRange;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

	/** Starting ammo for sticky bombs */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int AmmoClipSize = 3.0f;
		
	/** Current ammo count. This value is passed passed into the delegate
	 * to update anything bound (UWAmmo) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int AmmoInClip = AmmoClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Trace)
		float AimTraceDistance = 5000.0f;
	
	// Lines won't be drawn by default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Trace)
		bool bDrawAimTrace = false;

	/* So, this is a funny one. It turns out that the crossairs drawn 
	* by the HUD AREN'T actually centered, DESPITE the code comment
	* saying EXACTLY the following:
	* 
	* "offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	* 
	* The half-offset would be 8x8 (16x16 texture), so I was messed up for a bit on this one.
	* When performing a trace for the shot to be fired, I relied on finding the center of the
	* which is done by halving the screen size, so the numbers have to be multiplied by 2
	* X = 8 * 2 = 16
	* Y = 8 + 20 (The HUD's offset amount) * 2 = 56
	* 
	* Anyway, I didn't fix the crosshairs not being centered on purpose, because this could 
	* have been a project that WANTED the crosshairs in this way or something.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Trace)
		FVector2D CrosshairOffset = FVector2D(16.0f, 56.0f);

protected:
	/** Fires a projectile. */
	void OnFire();

	UFUNCTION(Server, Reliable)
	void SpawnShot(TSubclassOf<class AIronBellyTestProjectile> InProjectile, FVector InLocation, FRotator InRotation);

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// A trace to figure out what we're aiming at
	void AimTrace(FVector& OutStart, FVector& OutEnd);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	UFUNCTION()
	void Interact();

	UFUNCTION(Server, Reliable)
	void ServerInteract();

public:

	UFUNCTION(BlueprintCallable)
	void UpdateAmmo(int InAmmo);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

