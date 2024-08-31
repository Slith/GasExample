#include "GEAbilitiesFunctionLibrary.h"

UGEAbilitiesFunctionLibrary::UGEAbilitiesFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float UGEAbilitiesFunctionLibrary::GetPercentage(float Value, float MaxValue)
{
	return Value / MaxValue;
}