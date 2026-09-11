// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Pickable/Weapons/SimpleWeaponInstance.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, CurrentCartridge);
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
	CurrentHeat = (MinHeatRange + MaxHeatRange) * 0.5f;

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
	const float HeatPerShot = HeatToHeatPerShotCurve.GetRichCurveConst()->Eval(CurrentHeat);
	CurrentHeat = ClampHeat(CurrentHeat + HeatPerShot);

	CurrentSpreadAngle = HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);
}

void USimpleWeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World);
	LastFireTime = World->GetTimeSeconds();
}

bool USimpleWeaponInstance::UpdateSpread(float DeltaSeconds)
{
	const float TimeSinceFired = GetWorld() ? GetWorld()->TimeSince(LastFireTime) : 0.0f;

	if (TimeSinceFired > SpreadRecoveryCoolDownDelay)
	{
		const float CoolDownRate = HeatToCoolDownPerSecondCurve.GetRichCurveConst()->Eval(CurrentHeat);
		CurrentHeat = ClampHeat(CurrentHeat - (CoolDownRate * DeltaSeconds));

		CurrentSpreadAngle = HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);
	}

	float MinSpread, MaxSpread;
	ComputeSpreadRange(MinSpread, MaxSpread);

	return FMath::IsNearlyEqual(CurrentSpreadAngle, MinSpread,KINDA_SMALL_NUMBER);
}

bool USimpleWeaponInstance::UpdateMultipliers(float DeltaSeconds)
{
	const float MultiplierNearlyEqualThreshold = 0.05f;

	ACharacter* PlayerCharacter = GetCharacter();
	UCharacterMovementComponent* CharMoveComp = PlayerCharacter ? PlayerCharacter->GetCharacterMovement() : nullptr;
	const float CharacterSpeed = PlayerCharacter ? PlayerCharacter->GetVelocity().Length() : 0.0f;

	//速度越快散射越高
	const float MovementTargetValue = FMath::GetMappedRangeValueClamped(
		FVector2D(StandingStillSpeedThreshold, StandingStillSpeedThreshold + StandingStillToMovingSpeedRange),
		FVector2D(SpreadAngleMultiplier_StandingStill, 1.f),
		CharacterSpeed);

	//FInterpTo值可以无限接近于目标
	StandingStillMultiplier = FMath::FInterpTo(
		StandingStillMultiplier,
		MovementTargetValue,
		DeltaSeconds,
		TransactionRate_StandingStill);
	const bool bStandingStillMultiplierAtMin = FMath::IsNearlyEqual(
		StandingStillMultiplier,
		SpreadAngleMultiplier_StandingStill,
		MultiplierNearlyEqualThreshold);

	const bool bIsCrouching = (CharMoveComp != nullptr) && CharMoveComp->IsCrouching();
	const float CrouchingTargetValue = bIsCrouching ? SpreadAngleMultiplier_Crouching : 1.f;
	CrouchingMultiplier = FMath::FInterpTo(
		CrouchingMultiplier,
		CrouchingTargetValue,
		DeltaSeconds,
		TransactionRate_Crouching);
	const bool bCrouchingMultiplierAtMin = FMath::IsNearlyEqual(
		CrouchingMultiplier,
		SpreadAngleMultiplier_Crouching,
		MultiplierNearlyEqualThreshold);

	float AimingAlpha = 0.f;
	//根据开镜比例映射开镜散射倍率
	const float AimingMultiplier = FMath::GetMappedRangeValueClamped(
		FVector2D(0.f, 1.f),
		FVector2D(1.f, SpreadAngleMultiplier_Aiming),
		AimingAlpha);
	const bool bAimingMultiplierAtMin = FMath::IsNearlyEqual(
		AimingMultiplier,
		SpreadAngleMultiplier_Aiming,
		MultiplierNearlyEqualThreshold);

	CurrentSpreadAngleMultiplier = StandingStillMultiplier * CrouchingMultiplier * AimingMultiplier;

	return bStandingStillMultiplierAtMin && bCrouchingMultiplierAtMin && bAimingMultiplierAtMin;
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
	float Min1, Max1;
	HeatToHeatPerShotCurve.GetRichCurveConst()->GetTimeRange(Min1, Max1);

	float Min2, Max2;
	HeatToCoolDownPerSecondCurve.GetRichCurveConst()->GetTimeRange(Min2, Max2);

	float Min3, Max3;
	HeatToSpreadAngleCurve.GetRichCurveConst()->GetTimeRange(Min3, Max3);

	//保证覆盖到全部范围
	MinHeat = FMath::Min(Min1, FMath::Min(Min2, Min3));
	MaxHeat = FMath::Max(Max1, FMath::Max(Max2, Max3));
}

void USimpleWeaponInstance::SetAnimationParamsOnEquipped_Implementation(UObject* InInstigator)
{
}

void USimpleWeaponInstance::SetAnimationParamsOnUnequipped_Implementation(UObject* InInstigator)
{
}
