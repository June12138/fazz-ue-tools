// GASAbilityManagerComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CPP_AbilityInputManager.generated.h"

class UInputAction;
class UAbilitySystemComponent;
USTRUCT(BlueprintType)
struct FAbilityInputConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    FGameplayTag AbilityTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    FGameplayTag ReleaseTag;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FAZZUETOOLS_API UCPP_AbilityInputManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UCPP_AbilityInputManager();

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Management")
    TMap<UInputAction*, FAbilityInputConfig> Configs;

private:
    void BindInputActions();

    UAbilitySystemComponent* GetAbilitySystemComponent() const;

    void OnInputPressed(UInputAction* InputAction);
    void OnInputReleased(UInputAction* InputAction);
};