// Fill out your copyright notice in the Description page of Project Settings.
//玩家

#pragma once

#include "CoreMinimal.h"
#include "Core/FPSCharatcerBase.h"
#include "FPSPlayer.generated.h"

UCLASS(config=Game)
class FPSGAME_API AFPSPlayer : public AFPSCharatcerBase
{
	GENERATED_BODY()

protected:
	virtual void NotifyControllerChanged() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Sets default values for this character's properties
	AFPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Jump() override;
};

inline void AFPSPlayer::Jump()
{
	Super::Jump();
}
