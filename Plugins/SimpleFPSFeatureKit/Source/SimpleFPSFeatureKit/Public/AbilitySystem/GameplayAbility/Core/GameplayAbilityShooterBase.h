// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityShooterBase.generated.h"

class UAbilityCost;
/**
 * 
 */
UCLASS()
class SIMPLEFPSFEATUREKIT_API UGameplayAbilityShooterBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbilityShooterBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category="Costs")
	TArray<TObjectPtr<UAbilityCost>> AdditionalCosts;

protected:
	virtual bool CheckCost(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ApplyCost(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const override;
};
