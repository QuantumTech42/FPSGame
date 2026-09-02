// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayer.h"

#include "EnhancedInputComponent.h"
#include "Components/SimpleFPSInputComponent.h"
#include "Components/SimpleInputComponent.h"
#include "Components/SimplePlayerItemInterComponent.h"

//防止代码优化，方便调试
UE_DISABLE_OPTIMIZATION

void AFPSPlayer::K2_OnActionInputTag_Implementation(ETriggerEvent InEvent, const FInputActionValue& Value,
                                                    FGameplayTag InputTag)
{
	if (InputTag.GetTagName() == TEXT("PickUp"))
	{
		if (InEvent == ETriggerEvent::Triggered)
		{
			if (InteractionComponent.IsValid())
			{
				InteractionComponent->Trigger(Value);
			}
		}
	}
	else if (InputTag.GetTagName() == TEXT("Throw"))
	{
		if (InEvent == ETriggerEvent::Triggered)
		{
			if (InteractionComponent.IsValid())
			{
				InteractionComponent->Throw(Value);
			}
		}
	}
}

void AFPSPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (USimpleFPSInputComponent* InFPSInputComponent = FindComponentByClass<USimpleFPSInputComponent>())
	{
		InFPSInputComponent->RegisterMappingContext();
	}

	if (USimpleInputComponent* InInputComponent = FindComponentByClass<USimpleInputComponent>())
	{
		InInputComponent->RegisterMappingContext();
	}
}

// Called to bind functionality to input
void AFPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (USimpleFPSInputComponent* InFPSInputComponent = FindComponentByClass<USimpleFPSInputComponent>())
	{
		InFPSInputComponent->SetupPlayerInputComponent(PlayerInputComponent);
	}

	if (USimpleInputComponent* InInputComponent = FindComponentByClass<USimpleInputComponent>())
	{
		InInputComponent->SetupPlayerInputComponent(PlayerInputComponent);
	}
}

// Sets default values
AFPSPlayer::AFPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	InteractionComponent = FindComponentByClass<USimplePlayerItemInterComponent>();
}

// Called every frame
void AFPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSPlayer::Jump()
{
	Super::Jump();
}

UE_ENABLE_OPTIMIZATION
