// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI/WFloatingPickupUI.h"
#include "FloatingUIActor.generated.h"

UCLASS()
class IRONBELLYTEST_API AFloatingUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingUIActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		class USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		class USphereComponent* SightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		class UWidgetComponent* FloatingWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
		UWFloatingPickupUI* Widget;

	UPROPERTY( replicated )
		AActor* ParentActor;

	// Used to determine if the Widget component should be hidden or not.
	// Unfortunately, I'm going to leave it to display to all characters,
	// even if only a sing character is in range. :(
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<AIronBellyTestCharacter*> CharactersInRange;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex);

	UFUNCTION( Server, Reliable, BlueprintCallable)
		void OnPickedUp(AIronBellyTestCharacter* OtherCharacter);

};
