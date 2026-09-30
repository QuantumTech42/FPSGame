// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Misc/AbilityCostWeaponCartridge.h"

#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/GameplayAbility/GameplayAbilityFromEquipment.h"
#include "Actors/Items/Pickable/Weapons/SimpleItemActorWeapon.h"
#include "Actors/Items/Pickable/Weapons/SimpleWeaponInstance.h"

bool UAbilityCostWeaponCartridge::CheckCost(const UGameplayAbilityShooterBase* Ability,
                                            const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            FGameplayTagContainer* OptionalRelevantTags) const
{
	const UGameplayAbilityFromEquipment* WeaponAbility = Cast<UGameplayAbilityFromEquipment>(Ability);
	if (WeaponAbility == nullptr)
	{
		return false;
	}

	ASimpleItemActorWeapon* Weapon = WeaponAbility->GetSourceWeapon();
	if (Weapon == nullptr || Weapon->GetWeaponInstance() == nullptr)
	{
		return false;
	}

	return Weapon->GetWeaponInstance()->GetCurrentCartridge() >= CartridgeCostPerShot;
}

void UAbilityCostWeaponCartridge::ApplyCost(const UGameplayAbilityShooterBase* Ability,
                                            const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo)
{
	const UGameplayAbilityFromEquipment* WeaponAbility = Cast<UGameplayAbilityFromEquipment>(Ability);
	if (WeaponAbility == nullptr)
	{
		return;
	}

	ASimpleItemActorWeapon* Weapon = WeaponAbility->GetSourceWeapon();
	if (Weapon == nullptr || Weapon->GetWeaponInstance() == nullptr)
	{
		return;
	}

	USimpleWeaponInstance* WeaponInstance = Weapon->GetWeaponInstance();
	WeaponInstance->CartridgeCost(CartridgeCostPerShot);
}
