// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Items/Core/SimpleItemActorBase.h"
#include "SimpleItemActorPickable.generated.h"

enum class ESimpleItemPickableType : uint8;
class USimpleItemPickableDefinition;

UCLASS()
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorPickable : public ASimpleItemActorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleItemActorPickable();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ItemActorPickable")
	ESimpleItemPickableType PickableType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ItemActorPickable")
	TSubclassOf<USimpleItemPickableDefinition> ItemDefinition;

public:
	UFUNCTION(BlueprintPure, Category="TPItemActor_Pickable")
	ESimpleItemPickableType GetPickableType() const { return PickableType; }

	UFUNCTION(BlueprintPure, Category="TPItemActor_Pickable")
	USimpleItemPickableDefinition* GetItemDefinition() const;

protected:
	virtual void
	OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent, bool bForceInHand) override;
	virtual void
	OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent, bool bIsPutPack) override;

protected:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Pickable")
	void PickupItem(USimpleItemInterComponent* ItemInteractionComponent,UPrimitiveComponent* InItemMesh);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Pickable")
	void ThrowItem(USimpleItemInterComponent* ItemInteractionComponent,UPrimitiveComponent* InItemMesh,bool bIsPutPack);
};
