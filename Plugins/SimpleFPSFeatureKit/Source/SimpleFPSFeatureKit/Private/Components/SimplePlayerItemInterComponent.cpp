// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimplePlayerItemInterComponent.h"

#include "SimpleFPSFeatureKitType.h"
#include "Actors/Items/Core/SimpleItemActorBase.h"
#include "Actors/Items/Pickable/Core/SimpleItemActorPickable.h"
#include "Actors/Items/Pickable/Inventory/SimpleItemActorInventory.h"
#include "Actors/Items/Pickable/Weapons/SimpleItemActorWeapon.h"
#include "Actors/Items/Pickable/Weapons/SimpleWeaponInstance.h"
#include "Components/SimpleWeaponManagerComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"


void USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem,
	bool bForceInHand)
{
	if (InSelectingItem == nullptr)
	{
		return;
	}

	//手里没有任何东西
	if (InteractingItem == nullptr)
	{
		if (InSelectingItem->GetItemType() == ESimpleKitItemType::ITEM_PICKABLE)
		{
			if (ASimpleItemActorPickable* PickableItem = Cast<ASimpleItemActorPickable>(InSelectingItem))
			{
				switch (PickableItem->GetPickableType())
				{
				case ESimpleItemPickableType::ITEM_INVENTORY:
					{
						if (InSelectingItem->StartTrigger(this, bForceInHand))
						{
							if (bForceInHand)
							{
								InteractingItem = InSelectingItem;
							}
						}
						break;
					}
				case ESimpleItemPickableType::ITEM_WEAPON:
					{
						if (ASimpleItemActorWeapon* WeaponItem = Cast<ASimpleItemActorWeapon>(PickableItem))
						{
							PickupNewWeapon(WeaponItem, bForceInHand);
						}
						break;
					}
				default:
					{
						if (InSelectingItem->StartTrigger(this, bForceInHand))
						{
							InteractingItem = InSelectingItem;
						}
						break;
					}
				}
			}
		}
		else if (InSelectingItem->GetItemType() == ESimpleKitItemType::ITEM_SCENEINTERACTIVE)
		{
		}
	}
	else
	{
		switch (InSelectingItem->GetItemType())
		{
		case ESimpleKitItemType::ITEM_SCENEINTERACTIVE:
			{
				if (InteractingItem->GetItemType() == ESimpleKitItemType::ITEM_SCENEINTERACTIVE)
				{
					return;
				}
				else if (InteractingItem->EndTrigger(this, true) &&
					InSelectingItem->StartTrigger(this))
				{
					InteractingItem = InSelectingItem;
				}
				else
				{
					UE_LOG(LogTemp, Error,
					       TEXT(
						       "[USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation]场景交互触发交互失败"
					       ))
				}
				break;
			}
		case ESimpleKitItemType::ITEM_PICKABLE:
			{
				if (ASimpleItemActorPickable* PickableItem = Cast<ASimpleItemActorPickable>(InSelectingItem))
				{
					switch (PickableItem->GetPickableType())
					{
					case ESimpleItemPickableType::ITEM_WEAPON:
						{
							if (ASimpleItemActorWeapon* WeaponItem = Cast<ASimpleItemActorWeapon>(PickableItem))
							{
								PickupNewWeapon(WeaponItem, bForceInHand);
							}
							break;
						}
					case ESimpleItemPickableType::ITEM_INVENTORY:
						{
							if (ASimpleItemActorInventory* InventoryItem =
								Cast<ASimpleItemActorInventory>(PickableItem))
							{
								switch (InventoryItem->GetInventoryType())
								{
								case ESimpleItemInventoryType::ITEM_INVALID:
									{
										UE_LOG(LogTemp, Error,
										       TEXT(
											       "[USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation]库存类触发交互失败"
										       ))
										return;
									}
								default:
									{
										if (bForceInHand == false)
										{
											InventoryItem->StartTrigger(this, false);
										}
										else if (InteractingItem->EndTrigger(this, false) &&
											InventoryItem->StartTrigger(this, true))
										{
											InteractingItem = InSelectingItem;
										}
									}
								}
							}
							break;
						}
					case ESimpleItemPickableType::ITEM_NORMAL:
						{
							if (InteractingItem->EndTrigger(this, true) &&
								InSelectingItem->StartTrigger(this))
							{
								InteractingItem = InSelectingItem;
							}
							else
							{
								UE_LOG(LogTemp, Log,
								       TEXT(
									       "[USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation]普通物品触发交互失败"
								       ));
							}
							break;
						}
					case ESimpleItemPickableType::ITEM_INVALID:
						{
							break;
						}
					}
				}
				break;
			}
		default:
			{
				break;
			}
		}
	}
}

void USimplePlayerItemInterComponent::OnInteractingItemTriggerEnd_Implementation(
	ASimpleItemActorBase* InInteractingItem, bool bIsPutPack)
{
	if (InteractingItem.IsValid() && InteractingItem->EndTrigger(this, bIsPutPack))
	{
		InteractingItem = nullptr;
	}
}

// Sets default values for this component's properties
USimplePlayerItemInterComponent::USimplePlayerItemInterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SelectItemActorRange = 150.f;
	SphereCenterOffset = FVector::ZeroVector;

	TriggerInterval = 0.f;
	CheckCollisionType = ECollisionChannel::ECC_GameTraceChannel1;
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
	//是本地输入的代理玩家
	if (GetOwner() && GetOwner()->HasLocalNetOwner())
	{
		CheckItemAroundPlayer();

		TriggerInterval -= DeltaTime;
		if (TriggerInterval < 0.f)
		{
			TriggerInterval = 0.f;
		}
	}
}

void USimplePlayerItemInterComponent::Trigger(const FInputActionValue& Value)
{
	if (TriggerInterval <= 0.0f && SelectingItem.IsValid())
	{
		TriggerInterval = 0.5f;

		StartTriggerSelectingItem();
	}
}

void USimplePlayerItemInterComponent::Throw(const FInputActionValue& Value)
{
	if (TriggerInterval <= 0.0f && InteractingItem.IsValid())
	{
		TriggerInterval = 0.5f;

		EndTriggerInteractingItem();
	}
}

void USimplePlayerItemInterComponent::CheckItemAroundPlayer()
{
	if (!IsStartInteraction())
	{
		SelectingItem = nullptr;
		return;
	}

	UWorld* WorldPtr = GetWorld();
	APlayerController* PlayerControllerPtr = WorldPtr ? WorldPtr->GetFirstPlayerController() : nullptr;
	APawn* PlayerCharacterPtr = PlayerControllerPtr ? PlayerControllerPtr->GetPawn() : nullptr;

	NearbyItems.Empty();

	SelectingItem = nullptr;

	if (WorldPtr && PlayerControllerPtr && PlayerCharacterPtr)
	{
		FVector PlayerLocation = PlayerCharacterPtr->GetActorLocation();
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{UEngineTypes::ConvertToObjectType(CheckCollisionType)};
		TArray<AActor*> ActorsToIgnore{PlayerCharacterPtr, InteractingItem.Get()};

		TArray<FHitResult> HitResults;

		if (UKismetSystemLibrary::SphereTraceMultiForObjects(
			WorldPtr,
			PlayerLocation + SphereCenterOffset,
			PlayerLocation + SphereCenterOffset,
			SelectItemActorRange,
			ObjectTypes,
			false,
			ActorsToIgnore,
			DrawDebugTraceMode,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			2.f))
		{
			double MinItemAngle = 360.0;

			//找到和视口最近的交互点
			for (auto& HitResult : HitResults)
			{
				if (ASimpleItemActorBase* HitItemActor = Cast<ASimpleItemActorBase>(HitResult.GetActor()))
				{
					if (HitItemActor->IsStartTrigger(this, false))
					{
						NearbyItems.Emplace(HitItemActor);

						FVector OutLocation;
						FRotator OutRotation;
						PlayerControllerPtr->GetPlayerViewPoint(OutLocation, OutRotation);

						double DotValue = FVector::DotProduct(
							(HitResult.GetActor()->GetActorLocation() - OutLocation).GetSafeNormal(),
							OutRotation.Vector().GetSafeNormal());

						double TmpItemAngle = FMath::RadiansToDegrees(FMath::Acos(DotValue));

						if (TmpItemAngle < MinItemAngle)
						{
							MinItemAngle = TmpItemAngle;
							SelectingItem = HitItemActor;
						}
					}
				}
			}
		}
	}
}

void USimplePlayerItemInterComponent::PickupNewWeapon(ASimpleItemActorWeapon* InNewWeapon, bool bForceInHand)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	if (InNewWeapon == nullptr || InNewWeapon->GetWeaponInstance() == nullptr)
	{
		return;
	}

	if (USimpleWeaponManagerComponent* WeaponManager = GetWeaponManager())
	{
		if (ASimpleItemActorWeapon* WeaponInSlot = WeaponManager->GetWeaponInSlot(
			(int32)InNewWeapon->GetWeaponInstance()->GetWeaponSlot()))
		{
			WeaponInSlot->EndTrigger(this, false);
			if (WeaponInSlot == InteractingItem)
			{
				InteractingItem = nullptr;
			}
		}

		if (InteractingItem.IsValid())
		{
			if (bForceInHand)
			{
				//装备到手上
				if (InteractingItem->EndTrigger(this, true) &&
					InNewWeapon->StartTrigger(this, true))
				{
					InteractingItem = InNewWeapon;
				}
			}
			else
			{
				//直接放进背包
				InNewWeapon->StartTrigger(this, false);
			}
		}
		else
		{
			if (InNewWeapon->StartTrigger(this, true))
			{
				InteractingItem = InNewWeapon;
			}
		}
	}
}

USimpleWeaponManagerComponent* USimplePlayerItemInterComponent::GetWeaponManager() const
{
	if (GetOwner())
	{
		return GetOwner()->FindComponentByClass<USimpleWeaponManagerComponent>();
	}

	return nullptr;
}
