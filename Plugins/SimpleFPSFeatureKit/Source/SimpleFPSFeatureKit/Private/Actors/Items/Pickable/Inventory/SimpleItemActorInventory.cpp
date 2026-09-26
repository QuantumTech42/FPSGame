// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Pickable/Inventory/SimpleItemActorInventory.h"

#include "SimpleFPSFeatureKitType.h"
#include "Components/SimpleInvMgrComponent.h"
#include "Components/SimpleItemInterComponent.h"


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
	if (bForceInHand)
	{
		return Super::IsStartTrigger_Implementation(ItemInteractionComponent, bForceInHand);
	}
	else
	{
		if (!Super::IsStartTrigger_Implementation(ItemInteractionComponent, bForceInHand))
		{
			return false;
		}

		if (ItemInteractionComponent->GetOwner() == nullptr)
		{
			return false;
		}

		USimpleInvMgrComponent* InventoryComponent = ItemInteractionComponent->GetOwner()->
																	   FindComponentByClass<USimpleInvMgrComponent>();

		if (InventoryComponent == nullptr)
		{
			return false;
		}

		return InventoryComponent->IsAddItemToInventory(ItemDefinition);
	}
}

void ASimpleItemActorInventory::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                              bool bForceInHand)
{
	if (bForceInHand)
	{
		Super::OnStartTrigger_Implementation(ItemInteractionComponent, bForceInHand);
	}
	else
	{
		ASimpleItemActorBase::OnStartTrigger_Implementation(ItemInteractionComponent, bForceInHand);

		ItemCounts -= PickupItemToInventory();
		if (ItemCounts <= 0)
		{
			Destroy();
		}
	}
}

void ASimpleItemActorInventory::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                            bool bIsPutPack)
{
	if (bIsPutPack)
	{
		check(PickupItemToInventory() == ItemCounts);
		Destroy();
	}
	else
	{
		Super::OnEndTrigger_Implementation(ItemInteractionComponent, bIsPutPack);
	}
}

int32 ASimpleItemActorInventory::PickupItemToInventory()
{
	if (InteractingComponent == nullptr || InteractingComponent->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[ASimpleItemActorInventory::PickupItemToInventory]无效的交互组件或拥有者"));
		return 0;
	}

	USimpleInvMgrComponent* InventoryComponent = InteractingComponent->GetOwner()->
	                                                                   FindComponentByClass<USimpleInvMgrComponent>();
	if (InventoryComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[ASimpleItemActorInventory::PickupItemToInventory]无效的库存管理组件"));
		return 0;
	}

	return InventoryComponent->AddItemToInventory(ItemDefinition,ItemCounts);
}
