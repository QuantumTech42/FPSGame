// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleWeaponManagerComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Actors/Items/Pickable/Weapons/SimpleItemActorWeapon.h"
#include "Actors/Items/Pickable/Weapons/SimpleWeaponInstance.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USimpleWeaponManagerComponent::USimpleWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicatedByDefault(true);
}

void USimpleWeaponManagerComponent::InitWeaponManager(int32 InSlotSize)
{
	check(GetOwner() && GetOwner()->HasAuthority() && InSlotSize > 0);

	if (Weapons.Num() > 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[USimpleWeaponManagerComponent::InitWeaponManager]武器管理类已经初始化"));
		return;
	}

	Weapons.SetNum(InSlotSize);
}

ASimpleItemActorWeapon* USimpleWeaponManagerComponent::GetWeaponInSlot(int32 WeaponSlot)
{
	if (Weapons.IsValidIndex(WeaponSlot))
	{
		return Weapons[WeaponSlot];
	}

	return nullptr;
}

bool USimpleWeaponManagerComponent::AddWeapon(ASimpleItemActorWeapon* NewWeapon)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	int32 TargetSlot = INDEX_NONE;

	if (GetWeaponSlot(NewWeapon, TargetSlot))
	{
		if (Weapons[TargetSlot] == nullptr)
		{
			Weapons[TargetSlot] = NewWeapon;

			return true;
		}
	}

	return false;
}

bool USimpleWeaponManagerComponent::RemoveWeapon(int32 InRemoveSlot)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	if (Weapons.IsValidIndex(InRemoveSlot) && Weapons[InRemoveSlot])
	{
		if (EquipSlot == InRemoveSlot)
		{
			UnequipWeapon();
		}

		Weapons[InRemoveSlot] = nullptr;
		EquippingWeapon = nullptr;

		return true;
	}

	return false;
}

bool USimpleWeaponManagerComponent::EquipWeapon(int32 InSlot)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	if (EquipSlot != InSlot && Weapons.IsValidIndex(InSlot) && Weapons[InSlot])
	{
		OnWeaponEquipped(Weapons[InSlot]);
		EquipSlot = InSlot;
		EquippingWeapon = Weapons[InSlot];
		return true;
	}

	return false;
}

bool USimpleWeaponManagerComponent::UnequipWeapon()
{
	check(GetOwner() && GetOwner()->HasAuthority());

	if (Weapons.IsValidIndex(EquipSlot) && Weapons[EquipSlot])
	{
		OnWeaponUnequipped(Weapons[EquipSlot]);
		EquipSlot = -1;
		EquippingWeapon = nullptr;
		return true;
	}

	return false;
}

bool USimpleWeaponManagerComponent::ReplaceWeapon(ASimpleItemActorWeapon* NewWeapon)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	int32 TargetSlot = INDEX_NONE;
	if (GetWeaponSlot(NewWeapon, TargetSlot))
	{
		if (Weapons[TargetSlot])
		{
			Weapons[TargetSlot] = NewWeapon;
			EquippingWeapon = Weapons[TargetSlot];

			return true;
		}
	}

	return false;
}

void USimpleWeaponManagerComponent::OnRep_EquipSlot(const int32& OldEquipSlot)
{
	K2_EquipSlotRepNotify(OldEquipSlot);
}

void USimpleWeaponManagerComponent::OnRep_EquippingWeapon(const TWeakObjectPtr<ASimpleItemActorWeapon>& OldWeapon)
{
	if (OldWeapon.IsValid())
	{
		OnWeaponUnequipped(OldWeapon.Get());
	}

	if (EquippingWeapon.IsValid())
	{
		OnWeaponEquipped(EquippingWeapon.Get());
	}
}

bool USimpleWeaponManagerComponent::GetWeaponSlot(const ASimpleItemActorWeapon* InWeapon, int32& OutSlot)
{
	if (InWeapon && InWeapon->GetWeaponInstance())
	{
		OutSlot = (int32)InWeapon->GetWeaponInstance()->GetWeaponSlot();
		return Weapons.IsValidIndex(OutSlot);
	}

	return false;
}

void USimpleWeaponManagerComponent::OnWeaponEquipped(ASimpleItemActorWeapon* EquippedWeapon)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponInstance())
	{
		if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner()))
		{
			if (PlayerCharacter->HasAuthority())
			{
				if (IAbilitySystemInterface* InAbilityInterface = Cast<IAbilitySystemInterface>(PlayerCharacter))
				{
					if (UAbilitySystemComponent* ASC = InAbilityInterface->GetAbilitySystemComponent())
					{
						//遍历GAS绑定
						for (const FWeaponBindingAbility& TmpAbility : EquippedWeapon->GetWeaponInstance()->
						     GetBindingAbilities())
						{
							//激活GAS
							FGameplayAbilitySpec Spec = FGameplayAbilitySpec(TmpAbility.WeaponAbility, 1);
							Spec.SourceObject = EquippedWeapon;
							Spec.GetDynamicSpecSourceTags().AddTag(TmpAbility.InputTag);

							WeaponGiveAbilities.Add(ASC->GiveAbility(Spec));
						}
					}

					EquippedWeapon->GetWeaponInstance()->SetInstigator(GetOwner());
					FSimpleWeaponEquippedMontage EquippedMontage = EquippedWeapon->GetWeaponInstance()->
						GetWeaponEquippedMontage();
					// PlayerCharacter->MontagePlayOnMulticast(
					// 	EquippedMontage.AnimMontage,
					// 	EquippedMontage.PlayRate,
					// 	0.f,
					// 	EquippedMontage.bStopAllMontage,
					// 	EquippedMontage.TransactionName);
				}
			}
		}
	}
}

void USimpleWeaponManagerComponent::OnWeaponUnequipped(ASimpleItemActorWeapon* UnequippedWeapon)
{
	if (UnequippedWeapon && UnequippedWeapon->GetWeaponInstance())
	{
		if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner()))
		{
			if (PlayerCharacter->HasAuthority())
			{
				if (IAbilitySystemInterface* InAbilityInterface = Cast<IAbilitySystemInterface>(PlayerCharacter))
				{
					if (UAbilitySystemComponent* ASC = InAbilityInterface->GetAbilitySystemComponent())
					{
						for (const auto TmpHandle : WeaponGiveAbilities)
						{
							ASC->ClearAbility(TmpHandle);
						}
						WeaponGiveAbilities.Reset();
					}

					FSimpleWeaponEquippedMontage UnequippedMontage = UnequippedWeapon->GetWeaponInstance()->
						GetWeaponUnequippedMontage();
					// PlayerCharacter->MontagePlayOnMulticast(
					// 	UnequippedMontage.AnimMontage,
					// 	UnequippedMontage.PlayRate,
					// 	0.f,
					// 	UnequippedMontage.bStopAllMontage,
					// 	UnequippedMontage.TransactionName);
				}

				UnequippedWeapon->GetWeaponInstance()->SetInstigator(nullptr);
			}
		}
	}
}


// Called when the game starts
void USimpleWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (GetOwner() && GetOwner()->HasLocalNetOwner())
	{
		if (EquippingWeapon.IsValid() && EquippingWeapon->GetWeaponInstance())
		{
			//每帧处理散射
			EquippingWeapon->GetWeaponInstance()->WeaponTick(DeltaTime);
		}
	}
}

void USimpleWeaponManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//COND_None,REPNOTIFY_Always服务器更改了值会调用当前的通知OnRep
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,EquipSlot,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,EquippingWeapon,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME(ThisClass, Weapons);
}
