// Copyright Epic Games, Inc. All Rights Reserved.

#include "GEDummyCharacter.h"
#include "GAS/GEAttributeSet.h"
#include "AbilitySystemComponent.h"


DEFINE_LOG_CATEGORY(LogGECharacter);

//////////////////////////////////////////////////////////////////////////
// AGEDummyCharacter

AGEDummyCharacter::AGEDummyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UGEAttributeSet>(TEXT("AttributeSet"));
}

