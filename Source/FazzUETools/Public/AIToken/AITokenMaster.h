// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AITokenStruct.h"
#include "AITokenMaster.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FAZZUETOOLS_API UAITokenMaster : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAITokenMaster();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tokens", meta = (AllowPrivateAccess = "true"))
	TMap<FName, FAITokenStruct> Tokens;
	UFUNCTION(BlueprintCallable, Category = "AIToken")
	bool GrantToken(AActor* Requirer, FName TokenKey);
	UFUNCTION(BlueprintCallable, Category = "AIToken")
	void RetractToken(AActor* Holder, FName TokenKey);
	UFUNCTION(BlueprintCallable, Category = "AIToken")
	bool ActorHasToken(AActor* Actor, FName TokenKey);
};
