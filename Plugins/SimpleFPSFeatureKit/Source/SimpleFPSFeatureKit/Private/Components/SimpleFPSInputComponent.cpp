// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SimpleFPSInputComponent.h"
#include  "InputActionValue.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include  "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

void USimpleFPSInputComponent::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &USimpleFPSInputComponent::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &USimpleFPSInputComponent::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &USimpleFPSInputComponent::Jump);
	}
}

void USimpleFPSInputComponent::RegisterMappingContext()
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

void USimpleFPSInputComponent::Move(const FInputActionValue& Value)
{
	if (ACharacter* InCharacter = Cast<ACharacter>(GetOwner()))
	{
		FVector2D MovementVector = Value.Get<FVector2D>();

		InCharacter->AddMovementInput(InCharacter->GetActorRightVector(), MovementVector.X);
		InCharacter->AddMovementInput(InCharacter->GetActorForwardVector(), MovementVector.Y);
	}
}

void USimpleFPSInputComponent::Look(const FInputActionValue& Value)
{
	if (ACharacter* InCharacter = Cast<ACharacter>(GetOwner()))
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (InCharacter->GetController() != nullptr)
		{
			InCharacter->AddControllerYawInput(LookAxisVector.X);
			InCharacter->AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void USimpleFPSInputComponent::Jump(const FInputActionValue& Value)
{
	if (ACharacter* InCharacter = Cast<ACharacter>(GetOwner()))
	{
		//@TODO：玩家角色重载Jump函数
		InCharacter->Jump();
	}
}

// Sets default values for this component's properties
USimpleFPSInputComponent::USimpleFPSInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USimpleFPSInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleFPSInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
