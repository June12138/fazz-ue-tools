// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_WeaponRecoilComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FAZZUETOOLS_API UCPP_WeaponRecoilComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPP_WeaponRecoilComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 后座协程
	UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo"))
	FVoidCoroutine RecoilCoroutine();
	// 开始后座
	UFUNCTION(BlueprintCallable, category = "Recoil")
	void StartRecoil();
	// 停止后座
	UFUNCTION(BlueprintCallable, category = "Recoil")
	void EndRecoil();
	APawn* OwnerPawn;
	APlayerController* OwnerController;
	// 用于标记动画协程是否在运行
	bool bRecoilCoroutineActive;
	// 是否在后座中
	bool bRecoiling;
	// 后坐力曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	UCurveFloat *RecoilCurve;
	// 每次后座时长
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilDuration = 2;
	// 当前后座进度
	float RecoilProgress = 0;
	// 后坐力力度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FVector2D RecoilForce = FVector2D(1.f, -5.f);
	// 后坐力积攒值，用于后座回中
	FVector2D RecoilAccumulation = FVector2D::ZeroVector;
	// 后座回中协程
	UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo"))
	FVoidCoroutine RecoverCoroutine();
	bool bIsRecovering = false;
	UFUNCTION(BlueprintCallable, category = "Input")
	void LookInput(FVector2D InputValue);
	// 玩家压枪时的输入总和
	FVector2D ControlInputAccumulation = FVector2D::ZeroVector;
	// 射击结束后的回中输入
	FVector2D RecoverResult = FVector2D::ZeroVector;
	void SetRecoverResult();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recover")
	float RecoverSpeed = 5.f;
	void ResetAccumlations();
};
