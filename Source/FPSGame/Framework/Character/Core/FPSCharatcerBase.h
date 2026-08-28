// Fill out your copyright notice in the Description page of Project Settings.
//伤害和攻击

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "FPSCharatcerBase.generated.h"

//伤害和接受伤害，攻击
UCLASS(config=Game)
class FPSGAME_API AFPSCharatcerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharatcerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
