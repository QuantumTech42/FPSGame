// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Pickable/Weapons/SimpleItemActorWeapon.h"

#include "SimpleFPSFeatureKitType.h"
#include "Actors/ItemDefinition/SimpleItemPickableDefinition.h"
#include "Actors/Items/Pickable/Weapons/SimpleWeaponInstance.h"
#include "Components/SimpleItemInterComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Interface/SimpleItemInteractionInterface.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASimpleItemActorWeapon::ASimpleItemActorWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickableType = ESimpleItemPickableType::ITEM_WEAPON;

	//对应对象开启网络同步，和AddReplicatedSubObject联用
	bReplicateUsingRegisteredSubObjectList = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionProfileName(FName(TEXT("ItemPickable")));
	//只指定Body的某一个开启物理
	WeaponMesh->BodyInstance.bSimulatePhysics = true;
	WeaponMesh->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ASimpleItemActorWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && WeaponInstance == nullptr)
	{
		if (WeaponInstanceClass)
		{
			WeaponInstance = NewObject<USimpleWeaponInstance>(this, WeaponInstanceClass);

			if (WeaponInstance)
			{
				WeaponInstance->InitInstance();
				AddReplicatedSubObject(WeaponInstance);
			}
		}
	}
}

void ASimpleItemActorWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
	{
		RemoveReplicatedSubObject(WeaponInstance);
	}

	Super::EndPlay(EndPlayReason);
}

void ASimpleItemActorWeapon::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                           bool bForceInHand)
{
	Super::OnStartTrigger_Implementation(ItemInteractionComponent, bForceInHand);

	if (WeaponInstance == nullptr)
	{
		return;
	}

	//武器管理相关
}

void ASimpleItemActorWeapon::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent,
                                                         bool bIsPutPack)
{
	Super::OnEndTrigger_Implementation(ItemInteractionComponent, bIsPutPack);

	if (WeaponInstance)
	{
		
	}
}

void ASimpleItemActorWeapon::EquipToSlot()
{
	if (InteractingComponent.IsValid())
	{
		if (ISimpleItemInteractionInterface* ItemInterface = Cast<ISimpleItemInteractionInterface>(
			InteractingComponent->GetOwner()))
		{
			if (USkeletalMeshComponent* CharacterMesh = ItemInterface->Execute_GetCharacterMesh(
				InteractingComponent->GetOwner()))
			{
				PickupItem(InteractingComponent.Get(), WeaponMesh);
			}
		}
	}
}

void ASimpleItemActorWeapon::EquipToHand()
{
	if (InteractingComponent.IsValid())
	{
		if (ISimpleItemInteractionInterface* ItemInterface = Cast<ISimpleItemInteractionInterface>(
			InteractingComponent->GetOwner()))
		{
			if (USkeletalMeshComponent* CharacterMesh = ItemInterface->Execute_GetCharacterMesh(
				InteractingComponent->GetOwner()))
			{
				if (ItemDefinition)
				{
					PickupItem(InteractingComponent.Get(), CharacterMesh);

					AttachToComponent(
						CharacterMesh,
						FAttachmentTransformRules::SnapToTargetNotIncludingScale,
						ItemDefinition.GetDefaultObject()->ItemSlotName);
					SetActorRelativeTransform(ItemDefinition.GetDefaultObject()->ItemRelativeTransform);
				}
			}
		}
	}
}

// Called every frame
void ASimpleItemActorWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimpleItemActorWeapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, WeaponInstance);
}
