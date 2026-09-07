// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimplePlayerItemInterComponent.h"

#include "Actors/Items/Core/SimpleItemActorBase.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"


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
					if (HitItemActor->IsStartTrigger(this,false))
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
