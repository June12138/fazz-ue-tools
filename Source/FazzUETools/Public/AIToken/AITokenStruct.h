// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AITokenStruct.generated.h"
USTRUCT(BlueprintType)
struct FAITokenStruct
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Token")
	TArray<AActor*> Holders;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Token")
	int MaxHoldersCount;
};
