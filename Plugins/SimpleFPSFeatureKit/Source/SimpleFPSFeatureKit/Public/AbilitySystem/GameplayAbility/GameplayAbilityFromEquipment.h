// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameplayAbilityShooterBase.h"
#include "GameplayAbilityFromEquipment.generated.h"

class USimpleWeaponInstance;
class ASimpleItemActorWeapon;
/**
 * 
 */
UCLASS()
class SIMPLEFPSFEATUREKIT_API UGameplayAbilityFromEquipment : public UGameplayAbilityShooterBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="Equipment Ability")
	ASimpleItemActorWeapon* GetSourceWeapon() const;

	UFUNCTION(BlueprintCallable,Category="Equipment Ability")
	USimpleWeaponInstance* GetSourceWeaponInstance() const;
};
