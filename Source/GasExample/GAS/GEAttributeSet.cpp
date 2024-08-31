#include "GEAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "VisualLogger/VisualLogger.h"
#include "GEAbilitiesInterface.h"

DEFINE_LOG_CATEGORY(LogAttributes);

UGEAttributeSet::UGEAttributeSet()
{
	HealthMax = 100;
	Health = HealthMax;
}

void UGEAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UE_VLOG_ALWAYS_UELOG(GetOwningActor(), LogAttributes, Verbose, TEXT("[%s/%s] PreAttributeChange [%s] from: [%4.2f] to: [%4.2f]."),
		*GetNameSafe(GetOuter()), *GetName(), *Attribute.GetName(), Attribute.GetNumericValue(this), NewValue);
}

void UGEAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	UE_VLOG_ALWAYS_UELOG(GetOwningActor(), LogAttributes, Verbose, TEXT("[%s/%s] PreAttributeBaseChange [%s] from: [%4.2f] to: [%4.2f].."),
		*GetNameSafe(GetOuter()), *GetName(), *Attribute.GetName(), Attribute.GetNumericValue(this), NewValue);
}

bool UGEAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{

	UE_VLOG_ALWAYS_UELOG(GetOwningActor(), LogAttributes, Verbose, TEXT("[%s/%s] PreGameplayEffectExecute [%s] current: [%4.2f] magnitude: [%4.2f]."),
		*GetNameSafe(GetOuter()), *GetName(), *Data.EvaluatedData.Attribute.GetName(), Data.EvaluatedData.Attribute.GetNumericValue(this), Data.EvaluatedData.Magnitude);

	return true;
}

void UGEAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	AActor* OwningActor = GetOwningActor();

	UE_VLOG_ALWAYS_UELOG(OwningActor, LogAttributes, Verbose, TEXT("[%s/%s] PostGameplayEffectExecute [%s] current: [%4.2f] magnitude: [%4.2f]."),
		*GetNameSafe(GetOuter()), *GetName(), *Data.EvaluatedData.Attribute.GetName(), Data.EvaluatedData.Attribute.GetNumericValue(this), Data.EvaluatedData.Magnitude);

	if (TScriptInterface<IGEAbilitiesInterface> GEAbilitiesInterface = OwningActor)
	{
		GEAbilitiesInterface->Execute_OnPostGameplayEffectExecute(OwningActor, Data.EvaluatedData.Attribute);
	}
}