#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "GameplayEffect.h"

#include "GEAbilitiesInterface.generated.h"


UINTERFACE(BlueprintType)
class UGEAbilitiesInterface : public UInterface
{
	GENERATED_BODY()
};

class IGEAbilitiesInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = AbilitySystem)
	void OnPostGameplayEffectExecute(const FGameplayAttribute& GameplayAttribute) const;
};
