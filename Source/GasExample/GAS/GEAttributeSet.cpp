#include "GEAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameplayTagContainer.h"
#include "GEAbilitiesInterface.h"
#include "VisualLogger/VisualLogger.h"

DEFINE_LOG_CATEGORY(LogAttributes);

UGEAttributeSet::UGEAttributeSet()
{
	HealthMax = 100;
	Health = HealthMax;

	Damage = 0.0f;
	Heal = 0.0f;

	ResistanceBasicDamageMax = 1.0f;
	ResistanceBasicDamage = 0.4f;

	ResistanceFireDamageMax = 1.0f;
	ResistanceFireDamage = 0.2f;
}

bool UGEAttributeSet::TryClampAttributeValue(const FGameplayAttribute& ChangedAttribute, const FGameplayAttribute& AttributeToCompare, float MaxVaue, float& InOutValueToClamp) const
{
	if (ChangedAttribute == AttributeToCompare)
	{
		InOutValueToClamp = FMath::Clamp(InOutValueToClamp, 0, MaxVaue);
		return true;
	}

	return false;
}

bool UGEAttributeSet::TryApplyDamageResistance(FGameplayEffectModCallbackData& Data, const EDamageType DamageType, const float ResistanceValue) const
{
	const UGESettings* GESettings = GetDefault<UGESettings>(); // @NOTE Settings are always present. No need to nullcheck.
	const FGameplayTagContainer* AggregatedTags = Data.EffectSpec.CapturedSourceTags.GetAggregatedTags(); // @NOTE GetAggregatedTags gets pointer to existing structure. No need to nullcheck.

	if (const FGameplayTag* DamageTag = GESettings->DamageTypes.Find(DamageType))
	{
		if (AggregatedTags->HasTag(*DamageTag))
		{
			Data.EvaluatedData.Magnitude = Data.EvaluatedData.Magnitude * (1.0f - ResistanceValue);
			return true;
		}
	}

	return false;
}

void UGEAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	TryClampAttributeValue(Attribute, GetHealthAttribute(), GetHealthMax(), NewValue);

	UE_LOG(LogAttributes, Log, TEXT("---"));
	UE_VLOG_ALWAYS_UELOG(GetOwningActor(), LogAttributes, Log, TEXT("[%s/%s] PreAttributeChange [%s] from: [%4.2f] to: [%4.2f]."),
		*GetNameSafe(GetOuter()), *GetName(), *Attribute.GetName(), Attribute.GetNumericValue(this), NewValue);
}

void UGEAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	TryClampAttributeValue(Attribute, GetHealthAttribute(), GetHealthMax(), NewValue);

	UE_VLOG_ALWAYS_UELOG(GetOwningActor(), LogAttributes, Log, TEXT("[%s/%s] PreAttributeBaseChange [%s] from: [%4.2f] to: [%4.2f].."),
		*GetNameSafe(GetOuter()), *GetName(), *Attribute.GetName(), Attribute.GetNumericValue(this), NewValue);
}

bool UGEAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (TryApplyDamageResistance(Data, EDamageType::Basic, ResistanceBasicDamage.GetCurrentValue()))
		{
			// Nothing to do
		}
		else if (TryApplyDamageResistance(Data, EDamageType::Fire, ResistanceFireDamage.GetCurrentValue()))
		{
			// Nothing to do
		}

		//const UGESettings* GESettings = GetDefault<UGESettings>(); // @NOTE Settings are always present. No need to nullcheck.
		//const FGameplayTagContainer* AggregatedTags = Data.EffectSpec.CapturedSourceTags.GetAggregatedTags(); // @NOTE GetAggregatedTags gets pointer to existing structure. No need to nullcheck.

		//if (const FGameplayTag* BasicDamageTag = GESettings->DamageTypes.Find(EDamageType::Basic))
		//{
		//	if (AggregatedTags->HasTag(*BasicDamageTag))
		//	{
		//		Data.EvaluatedData.Magnitude = Data.EvaluatedData.Magnitude * (1.0f - ResistanceBasicDamage.GetCurrentValue());
		//	}
		//}
		//if (const FGameplayTag* FireDamageTag = GESettings->DamageTypes.Find(EDamageType::Fire))
		//{
		//	if (AggregatedTags->HasTag(*FireDamageTag))
		//	{
		//		Data.EvaluatedData.Magnitude = Data.EvaluatedData.Magnitude * (1.0f - ResistanceFireDamage.GetCurrentValue());
		//	}
		//}
	}

	UE_VLOG_ALWAYS_UELOG(GetOwningActor(), LogAttributes, Log, TEXT("[%s/%s] PreGameplayEffectExecute [%s] current: [%4.2f] magnitude: [%4.2f]."),
		*GetNameSafe(GetOuter()), *GetName(), *Data.EvaluatedData.Attribute.GetName(), Data.EvaluatedData.Attribute.GetNumericValue(this), Data.EvaluatedData.Magnitude);

	return true;
}

void UGEAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	AActor* OwningActor = GetOwningActor();

	UE_VLOG_ALWAYS_UELOG(OwningActor, LogAttributes, Log, TEXT("[%s/%s] PostGameplayEffectExecute [%s] current: [%4.2f] magnitude: [%4.2f]."),
		*GetNameSafe(GetOuter()), *GetName(), *Data.EvaluatedData.Attribute.GetName(), Data.EvaluatedData.Attribute.GetNumericValue(this), Data.EvaluatedData.Magnitude);

	static FProperty* HealthProperty = FindFieldChecked<FProperty>(UGEAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGEAttributeSet, Health));
	static FProperty* FinalDamageProperty = FindFieldChecked<FProperty>(UGEAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGEAttributeSet, Damage));
	static FProperty* HealProperty = FindFieldChecked<FProperty>(UGEAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGEAttributeSet, Heal));

	FProperty* ModifiedProperty = Data.EvaluatedData.Attribute.GetUProperty();

	if (ModifiedProperty == FinalDamageProperty)
	{
		const float NewHealth = Health.GetCurrentValue() - Damage.GetCurrentValue();
		Health.SetCurrentValue(FMath::Clamp(NewHealth, 0, GetHealthMax()));
		Damage = 0.f;
	}
	else if (ModifiedProperty == HealProperty)
	{
		const float NewHealth = Health.GetCurrentValue() + Heal.GetCurrentValue();
		Health.SetCurrentValue(FMath::Clamp(NewHealth, 0, GetHealthMax()));
		Heal = 0.f;
	}

	if (TScriptInterface<IGEAbilitiesInterface> GEAbilitiesInterface = OwningActor)
	{
		GEAbilitiesInterface->Execute_OnPostGameplayEffectExecute(OwningActor, Data.EvaluatedData.Attribute);
	}
}
