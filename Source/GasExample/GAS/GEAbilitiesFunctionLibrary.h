#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GEAbilitiesFunctionLibrary.generated.h"

UCLASS()
class UGEAbilitiesFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	// Return percentage between range of 0.0f - 1.0f
	UFUNCTION(BlueprintPure, Category = "AbilitySystem")
	static float GetPercentage(float Value, float MaxValue);
};
