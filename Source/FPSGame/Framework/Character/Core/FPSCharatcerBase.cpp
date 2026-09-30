// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharatcerBase.h"

#include "AbilitySystem/Attribute/SimpleShooterAttributeSetBase.h"
#include "Actors/Items/Pickable/Weapons/SimpleWeaponInstance.h"
#include "Components/SimpleShooterAbilityComponent.h"
#include "Components/SimpleWeaponManagerComponent.h"

//防止代码被优化
UE_DISABLE_OPTIMIZATION
// Sets default values
AFPSCharatcerBase::AFPSCharatcerBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<USimpleShooterAbilityComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	//部分同步
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	//只同步动画段，客户端自己推理
	AbilitySystemComponent->SetMontageRepAnimPositionMethod(ERepAnimPositionMethod::CurrentSectionId);
}

// Called when the game starts or when spawned
void AFPSCharatcerBase::BeginPlay()
{
	Super::BeginPlay();

	if (USimpleWeaponManagerComponent* InWeaponManagerComponent = FindComponentByClass<USimpleWeaponManagerComponent>())
	{
		InWeaponManagerComponent->InitWeaponManager((int32)ESimpleWeaponSlot::WS_SNIPER + 1);
	}

	AttributeSet = NewObject<USimpleShooterAttributeSetBase>(this,USimpleShooterAttributeSetBase::StaticClass());
	if (HasAuthority())
	{
		if (AttributeSet)
		{
			AbilitySystemComponent->AddAttributeSetSubobject<USimpleShooterAttributeSetBase>(AttributeSet);
		}
	}
}

// Called every frame
void AFPSCharatcerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFPSCharatcerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

USimpleShooterAttributeSetBase* AFPSCharatcerBase::GetAttribute() const
{
	return AttributeSet;
}

USkeletalMeshComponent* AFPSCharatcerBase::GetCharacterMesh_Implementation()
{
	return GetMesh();
}

UAbilitySystemComponent* AFPSCharatcerBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UE_ENABLE_OPTIMIZATION
