// Fill out your copyright notice in the Description page of Project Settings.


#include "AIToken/AITokenMaster.h"

// Sets default values for this component's properties
UAITokenMaster::UAITokenMaster()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Tokens.Add(FName("LightAttack"), FAITokenStruct(TArray<AActor*>(),6));
	Tokens.Add(FName("HeavyAttack"), FAITokenStruct(TArray<AActor*>(),3));
	// ...
}


// Called when the game starts
void UAITokenMaster::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAITokenMaster::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAITokenMaster::GrantToken(AActor* Requirer, FName TokenKey)
{
	FAITokenStruct* Token = Tokens.Find(TokenKey);
	if (Token){
		if (Token->Holders.Contains(Requirer))
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor %s already has token %s"), *Requirer->GetName(), *TokenKey.ToString());
			return false;
		}
		if (Token->Holders.Num() < Token->MaxHoldersCount)
		{
			Token->Holders.Add(Requirer);
			return true;
		}
	}
	return true;
}
void UAITokenMaster::RetractToken(AActor* Holder, FName TokenKey)
{
	FAITokenStruct* Token = Tokens.Find(TokenKey);
	if (Token)
	{
		if (Token->Holders.Contains(Holder))
		{
			Token->Holders.Remove(Holder);
			return;
		}
	}
}
bool UAITokenMaster::ActorHasToken(AActor* Actor, FName TokenKey)
{
	FAITokenStruct* Token = Tokens.Find(TokenKey);
	if (Token)
	{
		return Token->Holders.Contains(Actor);
	}
	return false;
}