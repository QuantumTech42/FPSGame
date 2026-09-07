// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Items/Pickable/Core/SimpleItemActorPickable.h"
#include "SimpleItemActorWeapon.generated.h"

class USimpleWeaponInstance;

UCLASS()
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorWeapon : public ASimpleItemActorPickable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleItemActorWeapon();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Config)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Config)
	TSubclassOf<USimpleWeaponInstance> WeaponInstanceClass;

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void
	OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent, bool bForceInHand) override;
	virtual void
	OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInteractionComponent, bool bIsPutPack) override;

public:
	UFUNCTION(BlueprintPure, Category="Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	UFUNCTION(BlueprintPure, Category="Weapon")
	USimpleWeaponInstance* GetWeaponInstance() const { return WeaponInstance; }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipToSlot();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipToHand();

private:
	UPROPERTY(Replicated)
	TObjectPtr<USimpleWeaponInstance> WeaponInstance;
};
