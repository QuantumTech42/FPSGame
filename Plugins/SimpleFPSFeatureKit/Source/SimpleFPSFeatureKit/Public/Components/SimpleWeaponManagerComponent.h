// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "SimpleWeaponManagerComponent.generated.h"


class ASimpleItemActorWeapon;

UCLASS(ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimpleWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimpleWeaponManagerComponent();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	void InitWeaponManager(int32 InSlotSize);

	UFUNCTION(BlueprintCallable, Category="Weapon Manager Component")
	TArray<ASimpleItemActorWeapon*> GetWeapons() { return Weapons; }

	UFUNCTION(BlueprintCallable, Category="Weapon Manager Component")
	ASimpleItemActorWeapon* GetEquippingWeapon() const { return EquippingWeapon.Get(); }

	UFUNCTION(BlueprintCallable, Category="Weapon Manager Component")
	ASimpleItemActorWeapon* GetWeaponInSlot(int32 WeaponSlot);

	UFUNCTION(BlueprintCallable, Category="Weapon Manager Component")
	int32 GetEquipSlot() const { return EquipSlot; }

	//增删改查
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	bool AddWeapon(ASimpleItemActorWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	bool RemoveWeapon(int32 InRemoveSlot);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	bool EquipWeapon(int32 InSlot);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	bool UnequipWeapon();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Weapon Manager Component")
	bool ReplaceWeapon(ASimpleItemActorWeapon* NewWeapon);

protected:
	UFUNCTION()
	void OnRep_EquipSlot(const int32& OldEquipSlot);

	UFUNCTION()
	void OnRep_EquippingWeapon(const TWeakObjectPtr<ASimpleItemActorWeapon>& OldWeapon);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="EquipSlotRepNotify"))
	void K2_EquipSlotRepNotify(const int32& OldEquipSlot);

private:
	bool GetWeaponSlot(const ASimpleItemActorWeapon* InWeapon,int32& OutSlot);
	
	void OnWeaponEquipped(ASimpleItemActorWeapon* EquippedWeapon);
	void OnWeaponUnequipped(ASimpleItemActorWeapon* UnequippedWeapon);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing="OnRep_EquipSlot", BlueprintReadOnly, Category="Weapon Manager Component")
	int32 EquipSlot = -1;

	//当前装备的武器
	UPROPERTY(ReplicatedUsing="OnRep_EquippingWeapon", BlueprintReadOnly, Category="Weapon Manager Component")
	TWeakObjectPtr<ASimpleItemActorWeapon> EquippingWeapon;

	//所有武器
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Weapon Manager Component")
	TArray<ASimpleItemActorWeapon*> Weapons;

private:
	TArray<FGameplayAbilitySpecHandle> WeaponGiveAbilities;
};
