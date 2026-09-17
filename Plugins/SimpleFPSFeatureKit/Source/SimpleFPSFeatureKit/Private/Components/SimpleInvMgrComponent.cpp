// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleInvMgrComponent.h"


void FSimpleItemInventoryList::SetInventorySize(const int32& NewInventorySize)
{
	if (NewInventorySize < Entries.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("[FSimpleItemInventoryList::SetInventorySize]新容量无效，无法修改数据"));
		return;
	}

	InventorySize = NewInventorySize;
}

bool FSimpleItemInventoryList::IsAddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDef,
                                          FSimpleItemInventoryEntry*& ItemEntry)
{
	for (auto TmpEntry : Entries)
	{
		if (TmpEntry.ItemDefinition == NewItemDef)
		{
			if (TmpEntry.ItemCounts < NewItemDef.GetDefaultObject()->ItemMaxCounts)
			{
				ItemEntry = &TmpEntry;
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[FSimpleItemInventoryList::IsAddEntry]物品数量大于最大堆叠数量"));
				return false;
			}
		}
	}

	return Entries.Num() < InventorySize;
}

bool FSimpleItemInventoryList::IsRemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef,
                                             const int32& ItemCounts, FSimpleItemInventoryEntry*& ItemEntry)
{
	for (auto TmpEntry : Entries)
	{
		if (TmpEntry.ItemDefinition == InItemDef)
		{
			if (TmpEntry.ItemCounts < ItemCounts)
			{
				UE_LOG(LogTemp, Error, TEXT("[FSimpleItemInventoryList::IsRemoveEntry]物品数量小于可移除物品数量，不可移除物品"));
				return false;
			}
			else
			{
				ItemEntry = &TmpEntry;
				return true;
			}
		}
	}

	return false;
}

int32 FSimpleItemInventoryList::AddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDef,
                                         const int32& ItemCounts)
{
	int32 RealAddCounts = 0;
	FSimpleItemInventoryEntry* TargetItemEntry = nullptr;

	if (IsAddEntry(NewItemDef, TargetItemEntry))
	{
		int32 LastCounts = 0;

		if (TargetItemEntry)
		{
			LastCounts = TargetItemEntry->ItemCounts;
			TargetItemEntry->ItemCounts += ItemCounts;
		}
		else
		{
			TargetItemEntry = &Entries.AddDefaulted_GetRef();
			TargetItemEntry->ItemDefinition = NewItemDef;
			TargetItemEntry->ItemCounts = ItemCounts;
		}

		if (TargetItemEntry->ItemCounts > NewItemDef.GetDefaultObject()->ItemMaxCounts)
		{
			TargetItemEntry->ItemCounts = NewItemDef.GetDefaultObject()->ItemMaxCounts;
		}

		RealAddCounts = TargetItemEntry->ItemCounts - LastCounts;

		//标记为脏，只同步已经修改的部分
		MarkItemDirty(*TargetItemEntry);
	}

	return RealAddCounts;
}

int32 FSimpleItemInventoryList::RemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef,
                                            const int32& ItemCounts)
{
	int32 RealRemoveCounts = 0;
	FSimpleItemInventoryEntry* TargetInventoryEntry = nullptr;

	if (IsRemoveEntry(InItemDef, ItemCounts, TargetInventoryEntry))
	{
		TargetInventoryEntry->ItemCounts -= ItemCounts;

		if (TargetInventoryEntry->ItemCounts == 0)
		{
			Entries.RemoveSingle(*TargetInventoryEntry);

			MarkArrayDirty();
		}
		else
		{
			MarkItemDirty(*TargetInventoryEntry);
		}

		RealRemoveCounts = ItemCounts;
	}

	return RealRemoveCounts;
}

FSimpleItemInventoryEntry* FSimpleItemInventoryList::GetEntry(
	const TSubclassOf<USimpleItemPickableDefinition>& InItemDef)
{
	return Entries.FindByPredicate(
		[InItemDef](const FSimpleItemInventoryEntry& TmpEntry)
		{
			return TmpEntry.ItemDefinition == InItemDef;
		});
}

void USimpleInvMgrComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Sets default values for this component's properties
USimpleInvMgrComponent::USimpleInvMgrComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USimpleInvMgrComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleInvMgrComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
