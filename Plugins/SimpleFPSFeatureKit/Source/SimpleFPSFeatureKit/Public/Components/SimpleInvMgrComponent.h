// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ItemDefinition/SimpleItemPickableDefinition.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "SimpleInvMgrComponent.generated.h"

USTRUCT(BlueprintType)
struct FSimpleItemInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Inventory Entry")
	TSubclassOf<USimpleItemPickableDefinition> ItemDefinition;

	UPROPERTY(BlueprintReadOnly, Category="Inventory Entry")
	int32 ItemCounts;
};

inline bool operator==(const FSimpleItemInventoryEntry& SourceEntry, const FSimpleItemInventoryEntry& OtherEntry)
{
	return SourceEntry.ItemDefinition == OtherEntry.ItemDefinition &&
		SourceEntry.ItemCounts == OtherEntry.ItemCounts;
}

USTRUCT(BlueprintType)
struct FSimpleItemInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	void SetInventorySize(const int32& NewInventorySize);
	//是否可以添加Entry
	//是否可以堆叠物品
	bool IsAddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDef,FSimpleItemInventoryEntry*& ItemEntry);
	bool IsRemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef,const int32& ItemCounts,FSimpleItemInventoryEntry*& ItemEntry);
	//数据增删改查
	int32 AddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDef,const int32& ItemCounts);
	int32 RemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef,const int32& ItemCounts);
	FSimpleItemInventoryEntry* GetEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef);
	
public:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FSimpleItemInventoryEntry, FSimpleItemInventoryList>(
			Entries, DeltaParams, *this);
	}

public:
	//数据的增删改查预处理
	//使用模板匹配，无需virtual
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
	{
	}

	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
	{
	}

	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
	{
	}

private:
	UPROPERTY(BlueprintReadOnly, Category="Inventory List", meta=(AllowPrivateAccess="true"))
	TArray<FSimpleItemInventoryEntry> Entries;

	UPROPERTY(BlueprintReadOnly, Category="Inventory List", meta=(AllowPrivateAccess="true"))
	int32 InventorySize = 1;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class SIMPLEFPSFEATUREKIT_API USimpleInvMgrComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Sets default values for this component's properties
	USimpleInvMgrComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
