#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

#include "GESettings.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Basic,
	Fire,

	LAST	UMETA(Hidden)
};


UCLASS(config=Game, defaultconfig, meta=(DisplayName="Gas Example Settings"))
class UGESettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(config, EditAnywhere, Category=Damage, meta = (Categories = "GasExample.DamageType"))
	TMap<EDamageType, FGameplayTag> DamageTypes;
};
