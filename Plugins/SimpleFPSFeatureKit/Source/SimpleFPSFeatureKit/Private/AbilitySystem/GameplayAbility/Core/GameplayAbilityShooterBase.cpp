// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/Core/GameplayAbilityShooterBase.h"

#include "AbilitySystem/Misc/Cost/AbilityCost.h"

UGameplayAbilityShooterBase::UGameplayAbilityShooterBase()
{
	//当前文件无状态，不可以存储临时变量，更高效
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//每个角色都会有一个当前组件，只生成一个组件
	//按键立刻触发，不卡手
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	//服务器和客户端都能触发
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

bool UGameplayAbilityShooterBase::CheckCost(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) || !ActorInfo)
	{
		return false;
	}

	for (TObjectPtr<UAbilityCost> AdditionalCost : AdditionalCosts)
	{
		if (AdditionalCost != nullptr)
		{
			if (!AdditionalCost->CheckCost(this, Handle, ActorInfo, OptionalRelevantTags))
			{
				return false;
			}
		}
	}

		return false;
}

void UGameplayAbilityShooterBase::ApplyCost(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	check(ActorInfo);

	for (TObjectPtr<UAbilityCost> AdditionalCost : AdditionalCosts)
	{
		if (AdditionalCost != nullptr)
		{
			//调用GAS
			AdditionalCost->ApplyCost(this, Handle, ActorInfo, ActivationInfo);
		}
	}
}
