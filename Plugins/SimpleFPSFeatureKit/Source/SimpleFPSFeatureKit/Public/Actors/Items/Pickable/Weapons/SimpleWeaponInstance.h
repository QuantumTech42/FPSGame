// Fill out your copyright notice in the Description page of Project Settings.
//描述武器的数据状态，表现层和数据层分离

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimpleWeaponInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLEFPSFEATUREKIT_API USimpleWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	USimpleWeaponInstance();

public:
	void InitInstance()
	{
	}
};
