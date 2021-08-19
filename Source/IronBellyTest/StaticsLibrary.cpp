// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticsLibrary.h"

float UStaticsLibrary::Approach(float start, float end, float shift)
{

	if (start < end)
	{
		return FMath::Min(start + shift, end);
	}
	else
	{
		return FMath::Max(start - shift, end);
	}

}