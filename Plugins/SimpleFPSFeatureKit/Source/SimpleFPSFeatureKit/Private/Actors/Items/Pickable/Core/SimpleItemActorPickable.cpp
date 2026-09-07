// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Pickable/Core/SimpleItemActorPickable.h"

#include "SimpleFPSFeatureKitType.h"
#include "Actors/ItemDefinition/SimpleItemPickableDefinition.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SimpleItemInterComponent.h"
#include "GameFramework/Character.h"
#include "Interface/SimpleItemInteractionInterface.h"


// Sets default values
ASimpleItemActorPickable::ASimpleItemActorPickable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemType = ESimpleKitItemType::ITEM_PICKABLE;
	PickableType = ESimpleItemPickableType::ITEM_NORMAL;
}

// Called when the game starts or when spawned
void ASimpleItemActorPickable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASimpleItemActorPickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

USimpleItemPickableDefinition* ASimpleItemActorPickable::GetItemDefinition() const
{
	return ItemDefinition.GetDefaultObject();
}

void ASimpleItemActorPickable::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                             bool bForceInHand)
{
	Super::OnStartTrigger_Implementation(ItemInteractionComponent, bForceInHand);

	USkeletalMeshComponent* CharacterMesh = nullptr;
	//接口
	if (ISimpleItemInteractionInterface* ItemInterface = Cast<ISimpleItemInteractionInterface>(ItemInteractionComponent->GetOwner()))
	{
		//使用Execute才能识别是蓝图还是C++
		CharacterMesh = ItemInterface->Execute_GetCharacterMesh(InteractingComponent->GetOwner());
	}
	else
	{
		ACharacter* CharacterOwner = Cast<ACharacter>(ItemInteractionComponent->GetOwner());
		if (CharacterOwner)
		{
			CharacterMesh = CharacterOwner->GetMesh();
		}
	}

	if (CharacterMesh)
	{
		if (ItemDefinition)
		{
			if (ItemDefinition.GetDefaultObject()->StartTriggerAnimMontage)
			{
				ItemInteractionComponent->PlayMontageNetMulticast(
					ItemDefinition.GetDefaultObject()->StartTriggerAnimMontage);
			}

			//客户端自动同步，不需要广播
			AttachToComponent(
				CharacterMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				ItemDefinition.GetDefaultObject()->ItemSlotName);

			SetActorRelativeTransform(ItemDefinition.GetDefaultObject()->ItemRelativeTransform);
		}
	}
}

void ASimpleItemActorPickable::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                           bool bIsPutPack)
{
	Super::OnEndTrigger_Implementation(ItemInteractionComponent, bIsPutPack);
}

void ASimpleItemActorPickable::PickupItem_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                         UPrimitiveComponent* InItemMesh)
{
	if (InItemMesh)
	{
		InItemMesh->SetSimulatePhysics(false);
		InItemMesh->SetRelativeLocation(FVector::ZeroVector);
		InItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//SetReplicatingMovement(false);
	}
}

void ASimpleItemActorPickable::ThrowItem_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                        UPrimitiveComponent* InItemMesh, bool bIsPutPack)
{
	if (InItemMesh && ItemDefinition)
	{
		//SetReplicatingMovement(true);

		InItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		InItemMesh->SetSimulatePhysics(true);

		if (!bIsPutPack)
		{
			if (ItemInteractionComponent && ItemInteractionComponent->GetOwner())
			{
				InItemMesh->AddImpulse(
					ItemInteractionComponent->GetOwner()->GetActorForwardVector() *
					ItemDefinition.GetDefaultObject()->ThrowItemSpeed,
					NAME_None,
					true);
			}
		}
	}
}
