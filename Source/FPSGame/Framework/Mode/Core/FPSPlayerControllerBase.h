// Fill out your copyright notice in the Description page of Project Settings.
//输入输出
//服务器客户端都有

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	AFPSPlayerControllerBase();

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
