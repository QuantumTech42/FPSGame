// Fill out your copyright notice in the Description page of Project Settings.
//玩家的交互

#pragma once

#include "CoreMinimal.h"
#include "SimpleItemInterComponent.h"
#include "SimplePlayerItemInterComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimplePlayerItemInterComponent : public USimpleItemInterComponent
{
	GENERATED_BODY()

	virtual void OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem, bool bForceInHand) override;
	virtual void OnInteractingItemTriggerEnd_Implementation(ASimpleItemActorBase* InInteractingItem, bool bIsPutPack) override;

public:
	// Sets default values for this component's properties
	USimplePlayerItemInterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
