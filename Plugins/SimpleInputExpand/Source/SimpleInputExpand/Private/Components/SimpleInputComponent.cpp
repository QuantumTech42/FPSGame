// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleInputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Config/SimpleActionInputConfig.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "Interface/SimpleInpputInterface.h"


void USimpleInputComponent::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputConfig)
		{
			if (ISimpleInpputInterface* OwnerInterface = Cast<ISimpleInpputInterface>(PlayerInputComponent->GetOwner()))
			{
				auto InFunc = [OwnerInterface,this](
					const FInputActionValue& Value,
					ETriggerEvent InEvent,
					FGameplayTag InGameplayTag)
				{
					ISimpleInpputInterface::Execute_K2_OnActionInputTag(GetOwner(), InEvent, Value, InGameplayTag);
				};

				auto InBindFunc = [&](
					ETriggerEvent InEvent,
					UInputAction* InAction,
					FGameplayTag InGameplayTag)
				{
					EnhancedInputComponent->BindActionValueLambda(
						InAction,
						InEvent,
						InFunc,
						InEvent,
						InGameplayTag);
				};

				for (const auto& TmpAction : InputConfig->InputActions)
				{
					InBindFunc(ETriggerEvent::Triggered, TmpAction.InputAction, TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Started, TmpAction.InputAction, TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Ongoing, TmpAction.InputAction, TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Canceled, TmpAction.InputAction, TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Completed, TmpAction.InputAction, TmpAction.InputTag);
				}
			}
		}
	}
}

void USimpleInputComponent::RegisterMappingContext()
{
	if (ACharacter* InCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(InCharacter->GetController()))
		{
			//LocalPlayer 本地角色
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Sets default values for this component's properties
USimpleInputComponent::USimpleInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USimpleInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
