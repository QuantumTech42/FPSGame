// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Core/SimpleItemActorBase.h"

#include "ImageCore.h"
#include "SimpleFPSFeatureKitType.h"
#include "Net/UnrealNetwork.h"
#include "Components/SimpleItemInterComponent.h"


// Sets default values
ASimpleItemActorBase::ASimpleItemActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bNetLoadOnClient = false;

	ItemType = ESimpleKitItemType::ITEM_INVALID;
}

// Called when the game starts or when spawned
void ASimpleItemActorBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASimpleItemActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimpleItemActorBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASimpleItemActorBase, InteractingComponent);
}

bool ASimpleItemActorBase::StartTrigger(USimpleItemInterComponent* ItemInteractionComponent, bool bForceInHand)
{
	if (IsStartTrigger(InteractingComponent.Get(), bForceInHand))
	{
		UE_LOG(LogTemp, Error, TEXT("[ASimpleItemActorBase::StartTrigger]正在交互"));
		return false;
	}

	OnStartTrigger(InteractingComponent.Get(), bForceInHand);

	return true;
}

bool ASimpleItemActorBase::EndTrigger(USimpleItemInterComponent* ItemInteractionComponent, bool bIsPutPack)
{
	if (IsEndTrigger(InteractingComponent.Get(), bIsPutPack))
	{
		UE_LOG(LogTemp, Error, TEXT("[ASimpleItemActorBase::EndTrigger]不可结束交互"));
		return false;
	}

	OnEndTrigger(InteractingComponent.Get(), bIsPutPack);

	return true;
}

bool ASimpleItemActorBase::IsStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                         bool bForceInHand)
{
	return InteractingComponent == nullptr && ItemInteractionComponent;
}

bool ASimpleItemActorBase::IsEndTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                       bool bIsPutPack)
{
	return InteractingComponent.IsValid() && InteractingComponent == ItemInteractionComponent;
}

void ASimpleItemActorBase::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                         bool bForceInHand)
{
	InteractingComponent = ItemInteractionComponent;
}

void ASimpleItemActorBase::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                       bool bIsPutPack)
{
	InteractingComponent.Reset();
}
