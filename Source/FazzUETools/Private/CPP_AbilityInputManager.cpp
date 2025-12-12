// GASAbilityManagerComponent.cpp

#include "CPP_AbilityInputManager.h"
//#include "EnhancedInputSubsystemInterface.h"
//#include "EnhancedPlayerInput.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

UCPP_AbilityInputManager::UCPP_AbilityInputManager(){

}

void UCPP_AbilityInputManager::BeginPlay(){
    Super::BeginPlay();
    // beginPlay中绑定按键容易导致复活后出bug，还是要手动绑定
    //BindInputActions();
    SetAbilitySystemComponent();
    GiveAbilities();
}
void UCPP_AbilityInputManager::GiveAbilities(){
    if (!GetOwner()->HasAuthority()) return;
    if (!ASC){
        UE_LOG(LogTemp, Error, TEXT("AbilityInputManager: Owner does not have AbilitySystemComponent"));
    }
    for (const TSubclassOf<UGameplayAbility>& Ability : Abilities){
        ASC->GiveAbility(FGameplayAbilitySpec(Ability, 1, INDEX_NONE, this));
    }
}
void UCPP_AbilityInputManager::BindInputActions(){
    AActor* Owner = GetOwner();
    if (!Owner) return;
    APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController());
    if (!PC) {
        // Alternatively, if attached to Character, get Controller from Character
        ACharacter* Char = Cast<ACharacter>(Owner);
        if (Char)
        {
            PC = Cast<APlayerController>(Char->GetController());
        }
        if (!PC) return;
    }

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
    if (!Subsystem) return;

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent);
    if (!EnhancedInputComponent) return;

    for (const auto& Pair : Configs){
        UInputAction* IA = Pair.Key;
        const FAbilityInputConfig& Config = Pair.Value;
        if (IA)
        {
            EnhancedInputComponent->BindAction(IA, ETriggerEvent::Started, this, &UCPP_AbilityInputManager::OnInputPressed, IA);
            EnhancedInputComponent->BindAction(IA, ETriggerEvent::Completed, this, &UCPP_AbilityInputManager::OnInputReleased, IA);
        }
    }
}

void UCPP_AbilityInputManager::SetAbilitySystemComponent(){
    AActor* Owner = GetOwner();
    if (!Owner) return;
    ASC = Owner->FindComponentByClass<UAbilitySystemComponent>();
}

void UCPP_AbilityInputManager::OnInputPressed(UInputAction* InputAction){
    if (!ASC){
        UE_LOG(LogTemp, Error, TEXT("AbilityInputManager: Owner does not have AbilitySystemComponent"));
        return;
    }
    if (Configs.Contains(InputAction)){
        const FAbilityInputConfig* Config = Configs.Find(InputAction);
        ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(Config->AbilityTag), true);
    }
}

void UCPP_AbilityInputManager::OnInputReleased(UInputAction* InputAction){
    if (!ASC){
        UE_LOG(LogTemp, Error, TEXT("AbilityInputManager: Owner does not have AbilitySystemComponent"));
        return;
    } 
    if (Configs.Contains(InputAction)){
        const FAbilityInputConfig* Config = Configs.Find(InputAction);
        ASC->AddLooseGameplayTag(Config->ReleaseTag);
        ASC->RemoveLooseGameplayTag(Config->ReleaseTag);
    }
}