// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ItemDefinition/SimpleItemDefinition.h"

USimpleItemDefinition::USimpleItemDefinition()
{
	//必须手动初始化防止随机赋值
	ItemRelativeTransform = FTransform::Identity;
}
