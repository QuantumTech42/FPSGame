// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleModularGameMode.h"
#include "SimpleModularGameState.h"
#include "SimpleModularPawn.h"
#include "SimpleModularPlayerController.h"

ASimpleModularGameMode::ASimpleModularGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ASimpleModularGameState::StaticClass();
	PlayerControllerClass = ASimpleModularPlayerController::StaticClass();
	PlayerStateClass = ASimpleModularPlayerController::StaticClass();
	DefaultPawnClass = ASimpleModularPawn::StaticClass();
}
