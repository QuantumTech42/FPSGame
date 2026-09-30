// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "UObject/Object.h"
#include "AbilityCost.generated.h"

class UGameplayAbilityShooterBase;
struct FGameplayAbilityActivationInfo;
struct FGameplayTagContainer;
/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class SIMPLEFPSFEATUREKIT_API UAbilityCost : public UObject
{
	GENERATED_BODY()

public:
	UAbilityCost()
	{
	}

	virtual bool CheckCost(
		const UGameplayAbilityShooterBase* Ability,
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}
	
	virtual void ApplyCost(
		const UGameplayAbilityShooterBase* Ability,
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo)
	{
		
	}
};
