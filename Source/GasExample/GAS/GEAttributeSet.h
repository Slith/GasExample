#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GESettings.h"

#include "GEAttributeSet.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAttributes, Log, All);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/*
 *	@NOTE When more attributes of different purpose arise: create new apropriate child classes of UGEAttributeSet and move Health related attributes to one of them.
 */
UCLASS()
class GASEXAMPLE_API UGEAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGEAttributeSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", meta = (HideFromModifiers))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGEAttributeSet, Health);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", meta = (HideFromModifiers))
	FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS(UGEAttributeSet, HealthMax);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UGEAttributeSet, Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData Heal;
	ATTRIBUTE_ACCESSORS(UGEAttributeSet, Heal);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData ResistanceBasicDamage;
	ATTRIBUTE_ACCESSORS(UGEAttributeSet, ResistanceBasicDamage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData ResistanceBasicDamageMax;
	ATTRIBUTE_ACCESSORS(UGEAttributeSet, ResistanceBasicDamageMax);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData ResistanceFireDamage;
	ATTRIBUTE_ACCESSORS(UGEAttributeSet, ResistanceFireDamage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FGameplayAttributeData ResistanceFireDamageMax;
	ATTRIBUTE_ACCESSORS(UGEAttributeSet, ResistanceFireDamageMax);

	bool TryClampAttributeValue(const FGameplayAttribute& ChangedAttribute, const FGameplayAttribute& AttributeToCompare, float MaxVaue, float& InOutValueToClamp) const;
	bool TryApplyDamageResistance(FGameplayEffectModCallbackData& Data, const EDamageType DamageType, const float ResistanceValue) const;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
