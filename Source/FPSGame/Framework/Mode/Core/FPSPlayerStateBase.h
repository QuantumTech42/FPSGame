// Fill out your copyright notice in the Description page of Project Settings.
//玩家数据，包括ID，名字
//服务器客户端都有

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "FPSPlayerStateBase.generated.h"

UCLASS()
class FPSGAME_API AFPSPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSPlayerStateBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
