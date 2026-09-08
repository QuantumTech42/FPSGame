// Fill out your copyright notice in the Description page of Project Settings.
//描述武器的数据状态，表现层和数据层分离

#pragma once

#include "CoreMinimal.h"
#include "SimpleFPSFeatureKitType.h"
#include "Abilities/GameplayAbility.h"
#include "Curves/CurveFloat.h"
#include "UObject/Object.h"
#include "SimpleWeaponInstance.generated.h"

/**
 * 
 */

class ACharacter;
enum class ESimpleWeaponSlot : uint8;
struct FGameplayTag;

USTRUCT(BlueprintType)
struct SIMPLEFPSFEATUREKIT_API FWeaponBindingAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Ability")
	TSubclassOf<UGameplayAbility> WeaponAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Ability")
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct SIMPLEFPSFEATUREKIT_API FSimpleWeaponEquippedMontage
{
	GENERATED_BODY()

	FSimpleWeaponEquippedMontage()
	{
		PlayRate = 1.f;
		bStopAllMontage = true;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	FName TransactionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	bool bStopAllMontage;
};

UCLASS(BlueprintType, Blueprintable)
class SIMPLEFPSFEATUREKIT_API USimpleWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	USimpleWeaponInstance();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	ESimpleWeaponSlot WeaponSlot = ESimpleWeaponSlot::WS_INVALID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	FName InventorySlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	FTransform InventoryRelativeTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	TArray<FWeaponBindingAbility> BindingAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	FSimpleWeaponEquippedMontage EquippedMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	FSimpleWeaponEquippedMontage UnequippedMontage;

	//初始化子弹数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	int32 CartridgeOnSpawn = 0;

	//弹容量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	int32 ClipSize = 10;

	//每发弹丸的子弹数量，主要针对霰弹枪
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	int32 BulletsPerCartridge = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	float MaxDamageRange = 20000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	float BulletRadius = 4.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	FRuntimeFloatCurve DistanceDamageFalloffCurve;

	//材质对应的伤害倍率，主要用于头部击中
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Basic")
	TMap<FName, float> MaterialDamageMultiplier;

	//武器散射和热量系统
	//散射角指数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Fire Params")
	float SpreadExponent = 1.f;

	//热量到散射角的曲线映射
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Fire Params")
	FRuntimeFloatCurve HeatToSpreadAngleCurve;

	//模拟武器升温
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Fire Params")
	FRuntimeFloatCurve HeatToHeatPerShotCurve;

	//武器冷却
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Fire Params")
	FRuntimeFloatCurve HeatToCoolDownPerSecondCurve;

	//停止射击的冷却延迟
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Fire Params")
	float SpreadRecoveryCoolDownDelay = 0.25f;

	//开启首发精准
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Fire Params")
	bool bAllowFirstShotAccuracy = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Multiplier")
	float SpreadAngleMultiplier_Aiming = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Multiplier")
	float SpreadAngleMultiplier_StandingStill = 1.f;

	//散射过渡平滑率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Multiplier")
	float TransactionRate_StandingStill = 5.f;

	//用速度判定角色是否站着不动
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Multiplier")
	float StandingStillSpeedThreshold = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Multiplier")
	float StandingStillToMovingSpeedRange = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Multiplier")
	float SpreadAngleMultiplier_Crouching = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Multiplier")
	float TransactionRate_Crouching = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Default|Spread|Multiplier")
	bool bDrawDebug = false;

private:
	//上次开火的时间戳
	double LastFireTime = 0.0;
	float CurrentHeat = 0.f;
	float CurrentSpreadAngle = 0.f;
	bool bHasFirstShotAccuracy = false;
	float CurrentSpreadAngleMultiplier = 1.f;
	float StandingStillMultiplier = 1.f;
	float CrouchingMultiplier = 1.f;

public:
	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual class UWorld* GetWorld() const override final;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	void WeaponTick(float DeltaSeconds);

	void OnEquipped(UObject* InInstigator);
	void OnUnequipped(UObject* InInstigator);

	void AddSpread();
	//更新连续射击的时间和相关判定，例如首发精准
	//用于GAS记录
	void UpdateFiringTime();

public:
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	ESimpleWeaponSlot GetWeaponSlot() const { return WeaponSlot; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FName GetInventorySlotName() const { return InventorySlotName; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FTransform GetInventoryRelativeTransform() const { return InventoryRelativeTransform; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	TArray<FWeaponBindingAbility> GetBindingAbilities() const { return BindingAbilities; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FSimpleWeaponEquippedMontage GetWeaponEquippedMontage() const { return EquippedMontage; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FSimpleWeaponEquippedMontage GetWeaponUnequippedMontage() const { return UnequippedMontage; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	int32 GetCurrentCartridge() const { return CurrentCartridge; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	int32 GetClipSize() const { return ClipSize; }

	int32 GetBulletsPerCartridge() const { return BulletsPerCartridge; }

	float GetCalculatedSpreadAngle() const { return CurrentSpreadAngle; }

	float GetCalculatedSpreadAngleMultiplier() const
	{
		return bHasFirstShotAccuracy ? 0.f : CurrentSpreadAngleMultiplier;
	}

	bool GetHasFirstShotAccuracy() const { return bHasFirstShotAccuracy; }

	float GetSpreadExponent() const { return SpreadExponent; }

	float GetMaxDamageDistance() const { return MaxDamageRange; }

	float GetBulletRadius() const { return BulletRadius; }

	bool GetCanDrawDebug() const { return bDrawDebug; }

	UFUNCTION(BlueprintNativeEvent, Category="Weapon Animation")
	void SetAnimationParamsOnEquipped(UObject* InInstigator);

	UFUNCTION(BlueprintNativeEvent, Category="Weapon Animation")
	void SetAnimationParamsOnUnequipped(UObject* InInstigator);

	inline float ClampHeat(float NewHeat)
	{
		float MinHeat;
		float MaxHeat;
		ComputeHeatRange(MinHeat, MaxHeat);

		return FMath::Clamp(NewHeat, MinHeat, MaxHeat);
	}

	bool UpdateSpread(float DeltaSeconds);
	bool UpdateMultipliers(float DeltaSeconds);

public:
	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }
	void CartridgeCost(int32 CostCounts);
	void ReloadCartridge(int32 CartridgeCounts);

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	UObject* GetInstigator() const { return Instigator; }

	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	ACharacter* GetCharacter() const;

private:
	void ComputeSpreadRange(float& MinSpread, float& MaxSpread);
	void ComputeHeatRange(float& MinHeat, float& MaxHeat);

public:
	void InitInstance()
	{
		CurrentCartridge = CartridgeOnSpawn;
	}

private:
	//武器的继承者
	UPROPERTY(Replicated)
	UObject* Instigator;

private:
	UPROPERTY(Replicated)
	int32 CurrentCartridge = 0;
};
