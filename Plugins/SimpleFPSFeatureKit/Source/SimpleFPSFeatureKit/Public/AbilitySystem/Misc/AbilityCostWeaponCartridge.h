// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cost/AbilityCost.h"
#include "AbilityCostWeaponCartridge.generated.h"

/**
 * 
 */
UCLASS(meta=(DisplayName="WeaponCartridge"))
class SIMPLEFPSFEATUREKIT_API UAbilityCostWeaponCartridge : public UAbilityCost
{
	GENERATED_BODY()

public:
	UAbilityCostWeaponCartridge()
		: CartridgeCostPerShot(1)
	{
	}

protected:
	virtual bool CheckCost(
	const UGameplayAbilityShooterBase* Ability,
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(
	const UGameplayAbilityShooterBase* Ability,
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Cost")
	int32 CartridgeCostPerShot;
};
