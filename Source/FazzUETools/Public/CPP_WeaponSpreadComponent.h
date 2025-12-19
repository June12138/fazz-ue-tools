// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UE5Coro/Public/UE5Coro.h"
#include "CPP_WeaponSpreadComponent.generated.h"

USTRUCT(BlueprintType)
struct FWeaponSpreadData
{
	GENERATED_BODY()
	// 最小散布
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread")
	FVector2D BaseSpread = FVector2D(5.f, 5.f);
	// 开火散布能达到的最大值
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread")
	FVector2D FireSpreadMax = FVector2D(10.f, 10.f);
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
private:
	FVector2D CurrentSpread;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	FVector2D GetSpread() const;
	// 散布的插值速率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread")
	float SpreadInterpolationRate = 10.f;
	// 不同状态下的SpreadData
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread|Base")
	TMap<FName, FWeaponSpreadData> SpreadDataMap;
	FWeaponSpreadData CurrentSpreadData;
	// 武器通过开火能达到的最大增量散布
	UFUNCTION(BlueprintCallable)
	void AddFireSpread();
	// 武器每次开火时增加的开火散布
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spread|Fire")
	FVector2D FireSpreadIncrement = FVector2D(0.5, 0.5);
	UFUNCTION(BlueprintCallable)
	FVector2D GetRandomSpread() const;
	// 用于开火平滑过渡的协程
	FVoidCoroutine SpreadLerpCoroutine(FForceLatentCoroutine = {});
	bool bLerpCoroutineRunning = false;
	// ADS相关
	void StartADS();
	void EndADS();
	float ADSAlpha = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spread|ADS")
	float ADSSpreadMultiplier = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spread|ADS")
	float ADSTime = 0.2f;
	FVoidCoroutine ADSLerpCoroutine(FForceLatentCoroutine = {});
	bool bADSCoroutineRunning = false;
	UFUNCTION(BlueprintCallable)
	void SetSpreadData(FName Key);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateSpreadData();
private:
	bool IsADS = false;
};
