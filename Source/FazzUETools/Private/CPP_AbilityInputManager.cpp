// GASAbilityManagerComponent.cpp

#include "CPP_AbilityInputManager.h"
//#include "EnhancedInputSubsystemInterface.h"
//#include "EnhancedPlayerInput.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"

UCPP_AbilityInputManager::UCPP_AbilityInputManager()
{
    // Constructor if needed
}

void UCPP_AbilityInputManager::BeginPlay()
{
    Super::BeginPlay();
    BindInputActions();
}

void UCPP_AbilityInputManager::BindInputActions()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController());
    if (!PC) 
    {
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

    for (const auto& Pair : Configs)
    {
        UInputAction* IA = Pair.Key;
        const FAbilityInputConfig& Config = Pair.Value;

        if (IA)
        {
            // Bind Pressed
            EnhancedInputComponent->BindAction(IA, ETriggerEvent::Started, this, &UCPP_AbilityInputManager::OnInputPressed, IA);

            // Bind Released (use Completed for release)
            EnhancedInputComponent->BindAction(IA, ETriggerEvent::Completed, this, &UCPP_AbilityInputManager::OnInputReleased, IA);
        }
    }
}

UAbilitySystemComponent* UCPP_AbilityInputManager::GetAbilitySystemComponent() const
{
    AActor* Owner = GetOwner();
    if (!Owner) return nullptr;

    // Assuming the AbilitySystemComponent is on the same Actor or Character
    return Owner->FindComponentByClass<UAbilitySystemComponent>();
}

void UCPP_AbilityInputManager::OnInputPressed(UInputAction* InputAction)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    if (!ASC) return;

    const FAbilityInputConfig* Config = Configs.Find(InputAction);
    if (Config)
    {
        ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(Config->AbilityTag), true);
    }
}

void UCPP_AbilityInputManager::OnInputReleased(UInputAction* InputAction)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    if (!ASC) return;

    const FAbilityInputConfig* Config = Configs.Find(InputAction);
    if (Config)
    {
        ASC->AddLooseGameplayTag(Config->ReleaseTag);
    }
}