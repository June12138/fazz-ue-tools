// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_WeaponSpreadComponent.h"

// Sets default values for this component's properties
UCPP_WeaponSpreadComponent::UCPP_WeaponSpreadComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(false);
	SpreadDataMap.Add("Default", FWeaponSpreadData());
}


// Called when the game starts
void UCPP_WeaponSpreadComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentSpreadData = *SpreadDataMap.Find("Default");
    CurrentSpread = CurrentSpreadData.BaseSpread;
}


// Called every frame
void UCPP_WeaponSpreadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}
void UCPP_WeaponSpreadComponent::AddFireSpread()
{
	if (IsADS) return;
	CurrentSpread = FMath::Clamp(CurrentSpread + FireSpreadIncrement, FVector2D(0, 0), CurrentSpreadData.FireSpreadMax + CurrentSpreadData.BaseSpread);
	SpreadLerpCoroutine();
}
FVector2D UCPP_WeaponSpreadComponent::GetRandomSpread() const{
	FVector2D Spread = GetSpread();
	FVector2D RandomSpread = FVector2D(FMath::RandRange(-Spread.X/2, Spread.X/2), FMath::RandRange(-Spread.Y/2, Spread.Y/2));
	return RandomSpread;
}

FVoidCoroutine UCPP_WeaponSpreadComponent::SpreadLerpCoroutine(FForceLatentCoroutine)
{
	if (bLerpCoroutineRunning) co_return;
	bLerpCoroutineRunning = true;
	// 获取世界DeltaTime
	float WorldDeltaTime = GetWorld()->GetDeltaSeconds();
	while (CurrentSpread != CurrentSpreadData.BaseSpread){
		CurrentSpread = FMath::Vector2DInterpConstantTo(CurrentSpread, CurrentSpreadData.BaseSpread, WorldDeltaTime, SpreadInterpolationRate);
		co_await UE5Coro::Latent::NextTick();
		//UE_LOG(LogTemp, Warning, TEXT("CurrentSpread: %s"), *CurrentSpread.ToString());
	}
	bLerpCoroutineRunning = false;
}
FVoidCoroutine UCPP_WeaponSpreadComponent::ADSLerpCoroutine(FForceLatentCoroutine){
	SpreadLerpCoroutine();
	if (bADSCoroutineRunning) co_return;
	bADSCoroutineRunning = true;
	while (true){
		if (IsADS){
			if (ADSAlpha != 1) ADSAlpha = FMath::FInterpConstantTo(ADSAlpha, 1, GetWorld()->GetDeltaSeconds(), 1/ADSTime); else break;
		}else{
			if (ADSAlpha != 0) ADSAlpha = FMath::FInterpConstantTo(ADSAlpha, 0, GetWorld()->GetDeltaSeconds(), 1/ADSTime); else break;
		}
        co_await UE5Coro::Latent::NextTick();
		//UE_LOG(LogTemp, Warning, TEXT("Current ADSAlpha: %f"), ADSAlpha);
	}
	bADSCoroutineRunning = false;
}
void UCPP_WeaponSpreadComponent::StartADS(){
	IsADS = true;
	SpreadLerpCoroutine();
	ADSLerpCoroutine();
}

void UCPP_WeaponSpreadComponent::EndADS(){
	IsADS = false;
	SpreadLerpCoroutine();
	ADSLerpCoroutine();
}

void UCPP_WeaponSpreadComponent::SetSpreadData(FName Key){
	if (SpreadDataMap.Contains(Key)){
		CurrentSpreadData = *SpreadDataMap.Find(Key);
	}
}

void UCPP_WeaponSpreadComponent::UpdateSpreadData_Implementation() {

}

FVector2D UCPP_WeaponSpreadComponent::GetSpread() const{
	return CurrentSpread * (1-ADSAlpha) * (1-ADSSpreadMultiplier);
}