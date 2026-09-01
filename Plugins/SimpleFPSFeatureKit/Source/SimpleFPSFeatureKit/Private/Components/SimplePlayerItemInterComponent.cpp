// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimplePlayerItemInterComponent.h"


void USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem,
	bool bForceInHand)
{
	Super::OnSelectingItemTriggerStart_Implementation(InSelectingItem, bForceInHand);
}

void USimplePlayerItemInterComponent::OnInteractingItemTriggerEnd_Implementation(
	ASimpleItemActorBase* InInteractingItem, bool bIsPutPack)
{
	Super::OnInteractingItemTriggerEnd_Implementation(InInteractingItem, bIsPutPack);
}

// Sets default values for this component's properties
USimplePlayerItemInterComponent::USimplePlayerItemInterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USimplePlayerItemInterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USimplePlayerItemInterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

