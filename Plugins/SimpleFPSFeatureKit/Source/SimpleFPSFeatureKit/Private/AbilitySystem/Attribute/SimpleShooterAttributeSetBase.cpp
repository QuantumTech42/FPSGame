// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/SimpleShooterAttributeSetBase.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

//处理可以被客户端预测性修改的数据
#define CHECK_OWNING_ASC_GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName,PropertyName,OldValue,String)\
	if (GetOwningAbilitySystemComponent())\
	{\
		GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName,PropertyName,OldValue);\
	}\
	else\
    {\
		GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,String);\
    }

USimpleShooterAttributeSetBase::USimpleShooterAttributeSetBase()
	: Level(1)
	  , Health(100.f)
	  , PhysicsAttack(10.f)
	  , Damage(0.f)
{
}

bool USimpleShooterAttributeSetBase::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void USimpleShooterAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void USimpleShooterAttributeSetBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USimpleShooterAttributeSetBase, Level);
	DOREPLIFETIME(USimpleShooterAttributeSetBase, Health);
	DOREPLIFETIME(USimpleShooterAttributeSetBase, PhysicsAttack);
	DOREPLIFETIME(USimpleShooterAttributeSetBase, Damage);
}

void USimpleShooterAttributeSetBase::OnRep_Level(const FGameplayAttributeData& OldValue)
{
	CHECK_OWNING_ASC_GAMEPLAYATTRIBUTE_REPNOTIFY(
		USimpleShooterAttributeSetBase, Level, OldValue,
		TEXT("USimpleShooterAttributeSetBase::OnRep_Level Error"));
}

void USimpleShooterAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	CHECK_OWNING_ASC_GAMEPLAYATTRIBUTE_REPNOTIFY(
		USimpleShooterAttributeSetBase, Health, OldValue,
		TEXT("USimpleShooterAttributeSetBase::OnRep_Health Error"));
}

void USimpleShooterAttributeSetBase::OnRep_PhysicsAttack(const FGameplayAttributeData& OldValue)
{
	CHECK_OWNING_ASC_GAMEPLAYATTRIBUTE_REPNOTIFY(
		USimpleShooterAttributeSetBase, PhysicsAttack, OldValue,
		TEXT("USimpleShooterAttributeSetBase::OnRep_PhysicsAttack Error"));
}

void USimpleShooterAttributeSetBase::OnRep_Damage(const FGameplayAttributeData& OldValue)
{
	CHECK_OWNING_ASC_GAMEPLAYATTRIBUTE_REPNOTIFY(
		USimpleShooterAttributeSetBase, Damage, OldValue,
		TEXT("USimpleShooterAttributeSetBase::OnRep_Damage Error"));
}
