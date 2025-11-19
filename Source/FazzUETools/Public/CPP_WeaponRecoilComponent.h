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
	UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo"))
	FVoidCoroutine RecoilCoroutine();
	UFUNCTION(BlueprintCallable, category = "Recoil")
	void StartRecoil();
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
	float RecoilDuration;
	// 当前后座进度
	float RecoilProgress;
	// 后坐力力度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FVector2D RecoilForce;
};
