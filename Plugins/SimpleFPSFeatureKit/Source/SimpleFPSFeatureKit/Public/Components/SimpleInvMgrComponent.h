// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ItemDefinition/SimpleItemPickableDefinition.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "SimpleInvMgrComponent.generated.h"

class USimplePlayerItemInterComponent;

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
	bool IsAddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDef,
	                FSimpleItemInventoryEntry*& ItemEntry);
	bool IsRemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef, const int32& ItemCounts,
	                   FSimpleItemInventoryEntry*& ItemEntry);
	//数据增删改查
	int32 AddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDef, const int32& ItemCounts);
	int32 RemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef, const int32& ItemCounts);
	FSimpleItemInventoryEntry* GetEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef);

public:
	//2.增量序列化
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

//1.告诉UE走增量同步
template <>
struct TStructOpsTypeTraits<FSimpleItemInventoryList> : public TStructOpsTypeTraitsBase2<FSimpleItemInventoryList>
{
	enum { WithNetDeltaSerializer = true };
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

USimplePlayerItemInterComponent* GetItemInteractionComponent();
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Inventory Manager Component")
	FSimpleItemInventoryList GetInventoryList() { return InventoryList; }

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Inventory Manager Component")
	bool GetItemEntry(TSubclassOf<USimpleItemPickableDefinition> ItemDef,FSimpleItemInventoryEntry& TargetEntry);

	UFUNCTION(BlueprintCallable, Category="Inventory Manager Component")
	bool IsAddItemToInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef);

	UFUNCTION(BlueprintCallable, Category="Inventory Manager Component")
	bool IsRemoveItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef,const int32& ItemCounts);

	UFUNCTION(BlueprintCallable, Category="Inventory Manager Component")
	int32 AddItemToInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef,const int32& ItemCounts);

	UFUNCTION(BlueprintCallable, Category="Inventory Manager Component")
	int32 RemoveItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef,const int32& ItemCounts);

	//丢弃
	UFUNCTION(BlueprintCallable, Category="Inventory Manager Component")
	void DiscardItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef,const int32& ItemCounts);

	//取出，比如取出武器
	UFUNCTION(BlueprintCallable, Category="Inventory Manager Component")
	void TakeOutItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef);

public:
	//网络同步只能接受值传递
	UFUNCTION(BlueprintCallable, Server,Unreliable,Category="Inventory Manager Component")
	void DiscardItemFromInventoryOnServer(TSubclassOf<USimpleItemPickableDefinition> ItemDef,const int32& ItemCounts);

	UFUNCTION(BlueprintCallable,Server,Unreliable, Category="Inventory Manager Component")
	void TakeOutItemFromInventoryOnServer(TSubclassOf<USimpleItemPickableDefinition> ItemDef);
	
private:
	//直接通过变量增量同步，无需函数广播
	UPROPERTY(Replicated)
	FSimpleItemInventoryList InventoryList;
};
