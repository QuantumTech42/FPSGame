// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleInputComponent.generated.h"

class USimpleActionInputConfig;
class UInputAction;
class UInputMappingContext;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class SIMPLEINPUTEXPAND_API USimpleInputComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Config|Input")
	TObjectPtr<USimpleActionInputConfig> InputConfig;

public:
	//真正泛化的位置
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	void RegisterMappingContext();

public:
	// Sets default values for this component's properties
	USimpleInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
