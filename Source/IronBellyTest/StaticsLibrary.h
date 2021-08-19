// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StaticsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class IRONBELLYTEST_API UStaticsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/* A simple linear interpolation method akin to Lerp, but without the need to define
	*  a break for when feeding into an equation. FInterpTo is also an option, and would 
	* suit my needs for this project, but I like having this around because there are
	* times I don't want to factor in DeltaTime.
	*/
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
		static float Approach(float start, float end, float shift);
	
};
