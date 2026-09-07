// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Items/Pickable/Core/SimpleItemActorPickable.h"
#include "SimpleItemActorInventory.generated.h"

enum class ESimpleItemInventoryType : uint8;

UCLASS()
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorInventory : public ASimpleItemActorPickable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleItemActorInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	ESimpleItemInventoryType InventoryType;

public:
	UFUNCTION(BlueprintPure, Category="Inventory")
	ESimpleItemInventoryType GetInventoryType() const { return InventoryType; }

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetItemCounts(int32 InItemCounts);

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetItemCounts() const { return ItemCounts; }

protected:
	virtual bool IsStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent, bool bForceInHand) override;
	virtual void OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent, bool bForceInHand) override;
	virtual void OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent, bool bIsPutPack) override;
	
protected:
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void PickupItemToInventory();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 ItemCounts;
};
