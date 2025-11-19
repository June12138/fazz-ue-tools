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
};
