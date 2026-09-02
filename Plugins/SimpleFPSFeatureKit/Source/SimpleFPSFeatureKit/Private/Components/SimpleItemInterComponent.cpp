// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleItemInterComponent.h"

#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"


bool USimpleItemInterComponent::IsStartInteraction_Implementation() const
{
	return true;
}

bool USimpleItemInterComponent::IsEndInteraction_Implementation() const
{
	return true;
}

bool USimpleItemInterComponent::IsStartTriggerInteractingItem_Implementation() const
{
	if (!SelectingItem.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("[USimpleItemInterComponent::IsStartTriggerInteractingItem]没有选择的物品"));
		return false;
	}

	return true;
}

bool USimpleItemInterComponent::IsEndTriggerInteractingItem_Implementation() const
{
	if (InteractingItem == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[USimpleItemInterComponent::IsEndTriggerInteractingItem]没有交互的物品"));
		return false;
	}

	return true;
}

void USimpleItemInterComponent::StartTriggerSelectingItemOnServer_Implementation(ASimpleItemActorBase* InSelectingItem)
{
	OnSelectingItemTriggerStart(InSelectingItem, false);
}

void USimpleItemInterComponent::EndTriggerInteractingItemOnServer_Implementation(bool bIsPutPack)
{
	OnInteractingItemTriggerEnd(InteractingItem.Get(), bIsPutPack);
}

void USimpleItemInterComponent::OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem,
                                                                           bool bForceInHand)
{
}

void USimpleItemInterComponent::OnInteractingItemTriggerEnd_Implementation(ASimpleItemActorBase* InInteractingItem,
                                                                           bool bIsPutPack)
{
}

ASimpleItemActorBase* USimpleItemInterComponent::GetInteractingItem() const
{
	return InteractingItem.Get();
}

void USimpleItemInterComponent::StartTriggerSelectingItem()
{
	if (!IsStartTriggerInteractingItem())
	{
		UE_LOG(LogTemp, Error, TEXT("[USimpleItemInterComponent::StartTriggerSelectingItem]不能在本地开始交互"));
		return;
	}

	StartTriggerSelectingItemOnServer(SelectingItem.Get());
}

void USimpleItemInterComponent::EndTriggerInteractingItem()
{
	if (!IsEndTriggerInteractingItem())
	{
		UE_LOG(LogTemp, Error, TEXT("[USimpleItemInterComponent::EndTriggerSelectingItem]不能在本地结束交互"));
		return;
	}

	EndTriggerInteractingItemOnServer(false);
}

void USimpleItemInterComponent::ServerTriggerItem(ASimpleItemActorBase* NewTriggerItem, bool bForceInHand)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		OnSelectingItemTriggerStart(NewTriggerItem, bForceInHand);
	}
}

void USimpleItemInterComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USimpleItemInterComponent, InteractingItem);
}

void USimpleItemInterComponent::OnRep_InteractingItem()
{
}

// Sets default values for this component's properties
USimpleItemInterComponent::USimpleItemInterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	//支持网络同步
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USimpleItemInterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleItemInterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
