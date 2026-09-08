// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Pickable/Weapons/SimpleWeaponInstance.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

USimpleWeaponInstance::USimpleWeaponInstance()
{
}

UWorld* USimpleWeaponInstance::GetWorld() const
{
	if (Instigator)
	{
		return Instigator->GetWorld();
	}

	return nullptr;
}

void USimpleWeaponInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass,Instigator);
	DOREPLIFETIME(ThisClass,CurrentCartridge);
}

void USimpleWeaponInstance::WeaponTick(float DeltaSeconds)
{
	const bool bMinSpread = UpdateSpread(DeltaSeconds);
	const bool bMinMultipliers = UpdateMultipliers(DeltaSeconds);

	//点射优于连射
	bHasFirstShotAccuracy = bAllowFirstShotAccuracy && bMinSpread && bMinMultipliers;
}

void USimpleWeaponInstance::OnEquipped(UObject* InInstigator)
{
	float MinHeatRange, MaxHeatRange;
	ComputeHeatRange(MinHeatRange, MaxHeatRange);
	CurrentHeat = (MinHeatRange+MaxHeatRange)*0.5f;

	CurrentSpreadAngle = HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);

	CurrentSpreadAngleMultiplier = 1.f;
	StandingStillMultiplier = 1.f;
	CrouchingMultiplier = 1.f;

	SetAnimationParamsOnEquipped(InInstigator);
}

void USimpleWeaponInstance::OnUnequipped(UObject* InInstigator)
{
	SetAnimationParamsOnUnequipped(InInstigator);
}

void USimpleWeaponInstance::AddSpread()
{
}

void USimpleWeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World);
	LastFireTime = World->GetTimeSeconds();
}

bool USimpleWeaponInstance::UpdateSpread(float DeltaSeconds)
{
	return true;
}

bool USimpleWeaponInstance::UpdateMultipliers(float DeltaSeconds)
{
	return true;
}

void USimpleWeaponInstance::CartridgeCost(int32 CostCounts)
{
	check(CostCounts <= CurrentCartridge);

	CurrentCartridge -= CostCounts;
}

void USimpleWeaponInstance::ReloadCartridge(int32 CartridgeCounts)
{
	CurrentCartridge += CartridgeCounts;

	check(CurrentCartridge <= ClipSize);
}

ACharacter* USimpleWeaponInstance::GetCharacter() const
{
	return Cast<ACharacter>(Instigator);
}

void USimpleWeaponInstance::ComputeSpreadRange(float& MinSpread, float& MaxSpread)
{
	HeatToSpreadAngleCurve.GetRichCurveConst()->GetValueRange(MinSpread, MaxSpread);
}

void USimpleWeaponInstance::ComputeHeatRange(float& MinHeat, float& MaxHeat)
{
}

void USimpleWeaponInstance::SetAnimationParamsOnEquipped_Implementation(UObject* InInstigator)
{
}

void USimpleWeaponInstance::SetAnimationParamsOnUnequipped_Implementation(UObject* InInstigator)
{
}