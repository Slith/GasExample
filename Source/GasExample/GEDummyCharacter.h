// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "GAS/GEAbilitiesInterface.h"

#include "GEDummyCharacter.generated.h"

class UAbilitySystemComponent;
class UGEAttributeSet;

DECLARE_LOG_CATEGORY_EXTERN(LogGECharacter, Log, All);

UCLASS(config=Game)
class AGEDummyCharacter : public ACharacter, public IAbilitySystemInterface, public IGEAbilitiesInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	UGEAttributeSet* AttributeSet;
	
public:
	AGEDummyCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
};

