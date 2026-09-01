// Fill out your copyright notice in the Description page of Project Settings.
//输入输出相关接口

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "SimpleInpputInterface.generated.h"

//struct FGameplayTag;
struct FInputActionValue;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class USimpleInpputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLEINPUTEXPAND_API ISimpleInpputInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Input", meta=(DisplayName="OnActionInputTag"))
	void K2_OnActionInputTag(ETriggerEvent InEvent, const FInputActionValue& Value, FGameplayTag InputTag);
};
