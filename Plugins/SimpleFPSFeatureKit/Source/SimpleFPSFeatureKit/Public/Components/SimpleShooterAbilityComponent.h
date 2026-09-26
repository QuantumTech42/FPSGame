// Fill out your copyright notice in the Description page of Project Settings.
//武器总管理
//武器的单点，连点和技能激活

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/ActorComponent.h"
#include "SimpleShooterAbilityComponent.generated.h"


UCLASS(ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimpleShooterAbilityComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//按下
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	//松开
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ClearAbilityInput();

	//监控按下松开输入
	void ProcessAbilityInput(float DeltaTime);

public:
	// Sets default values for this component's properties
	USimpleShooterAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	//技能输入
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
