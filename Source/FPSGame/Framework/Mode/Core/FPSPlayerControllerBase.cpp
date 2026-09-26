// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerControllerBase.h"

#include "Components/SimpleShooterAbilityComponent.h"
#include "FPSGame/Framework/Character/Core/FPSCharatcerBase.h"

AFPSPlayerControllerBase::AFPSPlayerControllerBase()
{
}

void AFPSPlayerControllerBase::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);

	if (AbilityComponent.IsValid())
	{
		AbilityComponent->ProcessAbilityInput(DeltaTime);
	}
}

void AFPSPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (AFPSCharatcerBase* InPlayer = Cast<AFPSCharatcerBase>(GetPawn()))
	{
		if (USimpleShooterAbilityComponent* InSystemComponent = Cast<USimpleShooterAbilityComponent>(
			InPlayer->GetAbilitySystemComponent()))
		{
			AbilityComponent = InSystemComponent;
		}
	
	}
}

void AFPSPlayerControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
