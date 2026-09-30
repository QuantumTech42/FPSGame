// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "SimpleShooterAttributeSetBase.generated.h"

/**
 * 
 */

//处理数据的宏
#define PROPERTY_FUNCTION_REGISTRATION(ClassName,PropertyName)\
	float Last##PropertyName;\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SIMPLEFPSFEATUREKIT_API USimpleShooterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	USimpleShooterAttributeSetBase();

public:
	//等级
	UPROPERTY(BlueprintReadOnly, Category="Attribute", ReplicatedUsing=OnRep_Level)
	FGameplayAttributeData Level;
	PROPERTY_FUNCTION_REGISTRATION(USimpleShooterAttributeSetBase, Level);

	UPROPERTY(BlueprintReadOnly, Category="Attribute", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	PROPERTY_FUNCTION_REGISTRATION(USimpleShooterAttributeSetBase, Health);

	UPROPERTY(BlueprintReadOnly, Category="Attribute", ReplicatedUsing=OnRep_PhysicsAttack)
	FGameplayAttributeData PhysicsAttack;
	PROPERTY_FUNCTION_REGISTRATION(USimpleShooterAttributeSetBase, PhysicsAttack);

	UPROPERTY(BlueprintReadOnly, Category="Attribute", ReplicatedUsing=OnRep_Damage)
	FGameplayAttributeData Damage;
	PROPERTY_FUNCTION_REGISTRATION(USimpleShooterAttributeSetBase, Damage);

public:
	//一般在服务器执行
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_PhysicsAttack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldValue);
};
