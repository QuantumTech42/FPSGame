// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Pickable/Inventory/SimpleItemActorInventory.h"

#include "SimpleFPSFeatureKitType.h"


// Sets default values
ASimpleItemActorInventory::ASimpleItemActorInventory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickableType = ESimpleItemPickableType::ITEM_INVENTORY;
	InventoryType = ESimpleItemInventoryType::ITEM_NORMAL;
}

// Called when the game starts or when spawned
void ASimpleItemActorInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimpleItemActorInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimpleItemActorInventory::SetItemCounts(int32 InItemCounts)
{
	if (HasAuthority())
	{
		ItemCounts = InItemCounts;
	}
}

bool ASimpleItemActorInventory::IsStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
	bool bForceInHand)
{
	return Super::IsStartTrigger_Implementation(ItemInteractionComponent, bForceInHand);
}

void ASimpleItemActorInventory::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
	bool bForceInHand)
{
	Super::OnStartTrigger_Implementation(ItemInteractionComponent, bForceInHand);
}

void ASimpleItemActorInventory::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
	bool bIsPutPack)
{
	Super::OnEndTrigger_Implementation(ItemInteractionComponent, bIsPutPack);
}

void ASimpleItemActorInventory::PickupItemToInventory()
{
}

