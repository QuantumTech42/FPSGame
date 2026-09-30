// Fill out your copyright notice in the Description page of Project Settings.
//伤害和攻击

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "SimpleModularCharacter.h"
#include "Interface/SimpleItemInteractionInterface.h"

#include "FPSCharatcerBase.generated.h"

class USimpleShooterAttributeSetBase;
class USimpleShooterAbilityComponent;
//伤害和接受伤害，攻击
UCLASS(config=Game)
class FPSGAME_API AFPSCharatcerBase : public ASimpleModularCharacter,
                                      public ISimpleItemInteractionInterface,
                                      public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TPAbility, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USimpleShooterAbilityComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TPAbility, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USimpleShooterAttributeSetBase> AttributeSet;

public:
	// Sets default values for this character's properties
	AFPSCharatcerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintPure,Category="Shotter|Attribute")
	virtual USimpleShooterAttributeSetBase* GetAttribute() const;

public:
	virtual USkeletalMeshComponent* GetCharacterMesh_Implementation() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
