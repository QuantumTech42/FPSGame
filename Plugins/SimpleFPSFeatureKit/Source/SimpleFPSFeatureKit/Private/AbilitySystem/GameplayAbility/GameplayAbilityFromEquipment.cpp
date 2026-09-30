// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/GameplayAbilityFromEquipment.h"

#include "Actors/Items/Pickable/Weapons/SimpleItemActorWeapon.h"

ASimpleItemActorWeapon* UGameplayAbilityFromEquipment::GetSourceWeapon() const
{
	if (FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec())
	{
		return Cast<ASimpleItemActorWeapon>(Spec->SourceObject);
	}
	
	return nullptr;
}

USimpleWeaponInstance* UGameplayAbilityFromEquipment::GetSourceWeaponInstance() const
{
	ASimpleItemActorWeapon*	Weapon = GetSourceWeapon();
	return Weapon != nullptr ? Weapon->GetWeaponInstance() : nullptr;
}
