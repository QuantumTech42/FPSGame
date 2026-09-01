// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SimpleActionInputConfig.generated.h"

/**
 * 
 */

class UInputAction;

USTRUCT(BlueprintType)
struct SIMPLEINPUTEXPAND_API FSimpleInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;
};

UCLASS(BlueprintType, Const)
class SIMPLEINPUTEXPAND_API USimpleActionInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="InputConfig")
	const UInputAction* FindActionInputActionForTag(const FGameplayTag& InputTag) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="InputConfig")
	TArray<FSimpleInputAction> InputActions;
};
