#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "NPCDefinition.generated.h"

/**
 * High-level AI relevance classification.
 * This does NOT define behavior, only importance and expectations.
 */
UENUM(BlueprintType)
enum class ENPCAIRelevance : uint8
{
    None            UMETA(DisplayName = "None"),
    Assisted        UMETA(DisplayName = "AI Assisted"),
    Conversational  UMETA(DisplayName = "Conversational"),
    CoreAI          UMETA(DisplayName = "Core AI NPC")
};

/**
 * UNPCDefinition
 *
 * Pure data asset describing NPC identity, intent, and constraints.
 * No behavior, no logic, no presentation.
 */
UCLASS(BlueprintType)
class MULTIPLAYERTEMPLATE_API UNPCDefinition : public UDataAsset
{
    GENERATED_BODY()

public:

    /* =========================
     * Identity
     * ========================= */

     /** Stable internal identifier (not localized, not player-facing) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Identity")
    FName NPCId;

    /** Display name shown to players */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Identity")
    FText DisplayName;

    /** Short editorial description of the NPC */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Identity", meta = (MultiLine = true))
    FText ShortDescription;


    /* =========================
     * Narrative & World Role
     * ========================= */

     /** Primary narrative role (single high-level tag) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Narrative")
    FGameplayTag NarrativeRole;

    /** System-facing world roles (can be multiple) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|World")
    FGameplayTagContainer WorldRoles;

    /* =========================
     * Player Relationship
     * ========================= */

     /** Default disposition toward the player */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Relationship")
    FGameplayTag DefaultPlayerDisposition;

    /* =========================
	* Capabilities
    * ========================= */

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Capabilities")
    FGameplayTagContainer CapabilityTags;

    /* =========================
     * Interaction Intent
     * ========================= */

     /** High-level interaction intents allowed for this NPC */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Interaction")
    FGameplayTagContainer AllowedInteractionIntents;


    /* =========================
     * Constraints
     * ========================= */

     /** Hard constraints that must never be violated */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Constraints")
    FGameplayTagContainer ConstraintTags;

    /* =========================
     * Persona & Voice Anchors
     * ========================= */

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Persona", meta = (MultiLine = true))
    FText PersonaSummary;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Persona")
    FText SpeechStyle;

    /* =========================
     * AI
     * ========================= */

     /** How relevant AI is for this NPC */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|AI")
    ENPCAIRelevance AIRelevance = ENPCAIRelevance::None;

    /* =========================
    * Debug & Tooling
    * ========================= */

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Debug")
    bool bDebugLogInteractions = false;

    /* =========================
     * Notes
     * ========================= */

     /** Optional designer notes (editor-only intent) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Notes", meta = (MultiLine = true))
    FText DesignerNotes;


public:

    /** Basic validity check (non-fatal, informational) */
    UFUNCTION(BlueprintCallable, Category = "NPC")
    bool IsValidDefinition() const;
};
