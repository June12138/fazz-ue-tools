// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_WeaponRecoilComponent.h"
#include "UE5Coro/Public/UE5Coro.h"

// Sets default values for this component's properties
UCPP_WeaponRecoilComponent::UCPP_WeaponRecoilComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCPP_WeaponRecoilComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCPP_WeaponRecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVoidCoroutine UCPP_WeaponRecoilComponent::RecoilCoroutine()
{
    return FVoidCoroutine();
}

void UCPP_WeaponRecoilComponent::StartRecoil()
{

}

void UCPP_WeaponRecoilComponent::EndRecoil()
{

}