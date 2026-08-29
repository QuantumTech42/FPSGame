// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleModularActor.h"


// Sets default values
ASimpleModularActor::ASimpleModularActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASimpleModularActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimpleModularActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

