// Fill out your copyright notice in the Description page of Project Settings.
//玩家和AI的交互

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleItemInterComponent.generated.h"


class ASimpleItemActorBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimpleItemInterComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category=Config)
	TWeakObjectPtr<ASimpleItemActorBase> SelectingItem;

	UPROPERTY(ReplicatedUsing="OnRep_InteractingItem", BlueprintReadOnly, Category=Config)
	TWeakObjectPtr<ASimpleItemActorBase> InteractingItem;

public:
	UFUNCTION(BlueprintCallable, Category="ItemInteractionComponent")
	ASimpleItemActorBase* GetInteractingItem() const;

	//主要逻辑
	UFUNCTION(BlueprintCallable, Category="ItemInteractionComponent")
	void StartTriggerSelectingItem();

	UFUNCTION(BlueprintCallable, Category="ItemInteractionComponent")
	void EndTriggerSelectingItem();

public:
	//是否检测到对象
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsStartInteraction() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsEndInteraction() const;

	//是否触发交互
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsStartTriggerInteractingItem() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsEndTriggerInteractingItem() const;

protected:
	//将选择物转换为交互物
	UFUNCTION(Server, Unreliable)
	void StartTriggerSelectingItemOnServer(ASimpleItemActorBase* InSelectingItem);

	UFUNCTION(Server, Unreliable)
	void EndTriggerSelectingItemOnServer(bool bIsPutPack);

	//面向对象具体逻辑，泛化内容
	UFUNCTION(BlueprintNativeEvent, Category="ItemInteractionComponent")
	void OnSelectingItemTriggerStart(ASimpleItemActorBase* InSelectingItem, bool bForceInHand);

	UFUNCTION(BlueprintNativeEvent, Category="ItemInteractionComponent")
	void OnInteractingItemTriggerEnd(ASimpleItemActorBase* InInteractingItem, bool bIsPutPack);

public:
	//在服务器做交互
	void ServerTriggerItem(ASimpleItemActorBase* NewTriggerItem, bool bForceInHand = false);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION()
	virtual void OnRep_InteractingItem();

public:
	// Sets default values for this component's properties
	USimpleItemInterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
