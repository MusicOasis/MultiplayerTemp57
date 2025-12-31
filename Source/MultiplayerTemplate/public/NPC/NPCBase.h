#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "NPC/NPCDefinition.h"
#include "NPCBase.generated.h"

class APlayerController;

/**
 * ANPCBase
 *
 * Studio-level NPC base actor.
 * - Owns identity & intent data via UNPCDefinition
 * - Accepts interaction requests
 * - Routes interaction intents
 * - Enforces capability + intent rules
 *
 * No game-specific logic lives here.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class MULTIPLAYERTEMPLATE_API ANPCBase : public AActor
{
    GENERATED_BODY()

public:
    ANPCBase();

    /* =========================
     * Core NPC Data
     * ========================= */

     /** Data-driven definition of this NPC */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
    TObjectPtr<UNPCDefinition> NPCDefinition;

    /* =========================
     * Lifecycle
     * ========================= */

    virtual void BeginPlay() override;

    /* =========================
     * Public Interaction API
     * ========================= */

     /**
      * Generic interaction entry point (e.g. player presses Interact).
      * Server-authoritative.
      */
    UFUNCTION(BlueprintCallable, Category = "NPC|Interaction")
    void Interact(APlayerController* InteractingPC);

    /**
     * Explicit interaction request with intent.
     * Used by higher-level systems (UI, quests, AI, etc).
     */
    UFUNCTION(BlueprintCallable, Category = "NPC|Interaction")
    void RequestInteraction(
        APlayerController* InteractingPC,
        FGameplayTag InteractionIntent
    );

protected:

    /* =========================
     * Networking
     * ========================= */

     /** Server-side interaction entry */
    UFUNCTION(Server, Reliable)
    void Server_Interact(APlayerController* InteractingPC);

    /** Server-side intent handler */
    UFUNCTION(Server, Reliable)
    void Server_HandleInteraction(
        APlayerController* InteractingPC,
        FGameplayTag InteractionIntent
    );

    /* =========================
     * Capability Queries
     * ========================= */

    bool HasCapability(FGameplayTag Capability) const;
    bool HasAllCapabilities(const FGameplayTagContainer& Capabilities) const;
    bool HasAnyCapability(const FGameplayTagContainer& Capabilities) const;

    /* =========================
     * Intent Routing
     * ========================= */

     /** Routes validated intents to concrete handlers */
    void RouteInteraction(
        APlayerController* InteractingPC,
        FGameplayTag InteractionIntent
    );

    /* =========================
     * Studio-Level Intent Handlers
     * ========================= */

    void HandleInteract(APlayerController* InteractingPC);
    void HandleTalk(APlayerController* InteractingPC);
    void HandleObserve(APlayerController* InteractingPC);
    void HandleRequestHelp(APlayerController* InteractingPC);
    void HandleReceiveItem(APlayerController* InteractingPC);

    /* =========================
     * Blueprint Extension Points
     * ========================= */

     /** Fired when an Interact intent is processed */
    UFUNCTION(BlueprintImplementableEvent, Category = "NPC|Intent")
    void OnInteract(APlayerController* InteractingPC);

    /** Fired when a Talk intent is processed */
    UFUNCTION(BlueprintImplementableEvent, Category = "NPC|Intent")
    void OnTalk(APlayerController* InteractingPC);

    /** Fired when an Observe intent is processed */
    UFUNCTION(BlueprintImplementableEvent, Category = "NPC|Intent")
    void OnObserve(APlayerController* InteractingPC);

    /** Fired when a RequestHelp intent is processed */
    UFUNCTION(BlueprintImplementableEvent, Category = "NPC|Intent")
    void OnRequestHelp(APlayerController* InteractingPC);

    /** Fired when a ReceiveItem intent is processed */
    UFUNCTION(BlueprintImplementableEvent, Category = "NPC|Intent")
    void OnReceiveItem(APlayerController* InteractingPC);
};
