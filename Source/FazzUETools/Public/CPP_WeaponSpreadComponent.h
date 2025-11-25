// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_WeaponSpreadComponent.generated.h"

USTRUCT(BlueprintType)
struct FSpreadData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread")
	FVector2D Base = FVector2D(0.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread")
	float InterpolationSpeed = 15.f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FAZZUETOOLS_API UCPP_WeaponSpreadComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPP_WeaponSpreadComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread")
	FVector2D CurrentSpread = FVector2D(5.f, 5.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread|Base")
	FVector2D CurrentBaseSpread = FVector2D(5.f, 5.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread|Base")
	FVector2D DefaultBaseSpread = FVector2D(5.f, 5.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread|Base")
	FVector2D ADSBaseSpread = FVector2D(0.f, 0.f);
	// 武器通过开火能达到的最大增量散布
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread|Fire")
	FVector2D FireSpreadMax = FVector2D(10.f, 10.f);
	UFUNCTION(BlueprintCallable)
	void AddFireSpread();
	// 武器每次开火时增加的开火散布
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread|Fire")
	FVector2D FireSpreadIncrement = FVector2D(0.5, 0.5);
	// 开火散布的减少率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread|Fire")
	float CurrentInterpolationRate = 10;
	UFUNCTION(BlueprintCallable)
	FVector2D GetRandomSpread();
	// 用于开火平滑过渡的协程
	UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo"))
	FVoidCoroutine SpreadLerpCoroutine();
	bool bLerpCoroutineRunning = false;
	void StartADS();
	void EndADS();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetBaseSpread(FName Key);
private:
	bool IsADS = false;
};
