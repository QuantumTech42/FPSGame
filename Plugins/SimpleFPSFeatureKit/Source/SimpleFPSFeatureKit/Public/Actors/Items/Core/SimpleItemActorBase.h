// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleItemActorBase.generated.h"

class USimpleItemInterComponent;
enum class ESimpleKitItemType : uint8;

UCLASS(Abstract)
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleItemActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ItemActorBase")
	ESimpleKitItemType ItemType;

public:
	UFUNCTION(BlueprintCallable, Category="ItemActorBase")
	bool StartTrigger(USimpleItemInterComponent* ItemInteractionComponent, bool bForceInHand = false);

	UFUNCTION(BlueprintCallable, Category="ItemActorBase")
	bool EndTrigger(USimpleItemInterComponent* ItemInteractionComponent, bool bIsPutPack);

	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	bool IsStartTrigger(USimpleItemInterComponent* ItemInteractionComponent, bool bForceInHand);

	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	bool IsEndTrigger(USimpleItemInterComponent* ItemInteractionComponent, bool bIsPutPack);

	UFUNCTION(BlueprintPure, Category="ItemActorBase")
	ESimpleKitItemType GetItemType() { return ItemType; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	void OnStartTrigger(USimpleItemInterComponent* ItemInteractionComponent, bool bForceInHand);

	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	void OnEndTrigger(USimpleItemInterComponent* ItemInteractionComponent, bool bIsPutPack);

	UPROPERTY(Replicated,BlueprintReadOnly, Category="ItemActorBase")
	TWeakObjectPtr<USimpleItemInterComponent> InteractingComponent;
};
