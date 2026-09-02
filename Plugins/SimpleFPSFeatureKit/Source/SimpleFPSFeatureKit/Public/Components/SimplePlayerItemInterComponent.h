// Fill out your copyright notice in the Description page of Project Settings.
//玩家的交互

#pragma once

#include "CoreMinimal.h"
#include "SimpleItemInterComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SimplePlayerItemInterComponent.generated.h"


struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),BlueprintType,Blueprintable)
class SIMPLEFPSFEATUREKIT_API USimplePlayerItemInterComponent : public USimpleItemInterComponent
{
	GENERATED_BODY()

protected:
	//射线检测
	UPROPERTY(EditDefaultsOnly, Category=Config)
	TEnumAsByte<ECollisionChannel> CheckCollisionType;

	//Debug图形
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTraceMode;

	//检测半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Config, meta=(ClampMin = 0.f))
	float SelectItemActorRange;

	//检测点相对于玩家的偏移
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config)
	FVector SphereCenterOffset;

public:
	virtual void
	OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem, bool bForceInHand) override;
	virtual void
	OnInteractingItemTriggerEnd_Implementation(ASimpleItemActorBase* InInteractingItem, bool bIsPutPack) override;

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

	virtual void Trigger(const FInputActionValue& Value);
	virtual void Throw(const FInputActionValue& Value);

public:
	void CheckItemAroundPlayer();

public:
	UPROPERTY(BlueprintReadOnly, Category=Config)
	TArray<ASimpleItemActorBase*> NearbyItems;

protected:
	UPROPERTY()
	float TriggerInterval;
};
