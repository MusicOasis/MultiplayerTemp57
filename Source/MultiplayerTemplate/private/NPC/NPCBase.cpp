#include "NPC/NPCBase.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"

/* =========================
 * Constructor
 * ========================= */

ANPCBase::ANPCBase()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
}

/* =========================
 * Lifecycle
 * ========================= */

void ANPCBase::BeginPlay()
{
    Super::BeginPlay();

    // Only the server validates NPC definitions
    if (!HasAuthority())
    {
        return;
    }

    if (!NPCDefinition)
    {
        UE_LOG(LogTemp, Error,
            TEXT("NPCBase '%s' has no NPCDefinition assigned."),
            *GetName());
        return;
    }

    // Informational validation (non-fatal)
    NPCDefinition->IsValidDefinition();
}

/* =========================
 * Public Interaction API
 * ========================= */

void ANPCBase::Interact(APlayerController* InteractingPC)
{
    if (!InteractingPC)
    {
        return;
    }

    // Always route through server
    Server_Interact(InteractingPC);
}

void ANPCBase::RequestInteraction(
    APlayerController* InteractingPC,
    FGameplayTag InteractionIntent)
{
    if (!InteractingPC || !InteractionIntent.IsValid())
    {
        return;
    }

    Server_HandleInteraction(InteractingPC, InteractionIntent);
}

/* =========================
 * Networking
 * ========================= */

void ANPCBase::Server_Interact_Implementation(APlayerController* InteractingPC)
{
    if (!InteractingPC || !NPCDefinition)
    {
        return;
    }

    // Default "Interact" intent
    static const FGameplayTag InteractIntent =
        FGameplayTag::RequestGameplayTag(TEXT("NPC.Intent.Interact"));

    if (!NPCDefinition->AllowedInteractionIntents.HasTagExact(InteractIntent))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("NPC '%s' does not allow Interact intent."),
            *GetName());
        return;
    }

    RouteInteraction(InteractingPC, InteractIntent);
}

void ANPCBase::Server_HandleInteraction_Implementation(
    APlayerController* InteractingPC,
    FGameplayTag InteractionIntent)
{
    if (!InteractingPC || !NPCDefinition)
    {
        return;
    }

    if (!NPCDefinition->AllowedInteractionIntents.HasTagExact(InteractionIntent))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("NPC '%s' rejected interaction intent '%s'."),
            *GetName(),
            *InteractionIntent.ToString());
        return;
    }

    RouteInteraction(InteractingPC, InteractionIntent);
}

/* =========================
 * Capability Queries
 * ========================= */

bool ANPCBase::HasCapability(FGameplayTag Capability) const
{
    if (!NPCDefinition || !Capability.IsValid())
    {
        return false;
    }

    return NPCDefinition->CapabilityTags.HasTagExact(Capability);
}

bool ANPCBase::HasAllCapabilities(const FGameplayTagContainer& Capabilities) const
{
    if (!NPCDefinition || Capabilities.IsEmpty())
    {
        return false;
    }

    return NPCDefinition->CapabilityTags.HasAll(Capabilities);
}

bool ANPCBase::HasAnyCapability(const FGameplayTagContainer& Capabilities) const
{
    if (!NPCDefinition || Capabilities.IsEmpty())
    {
        return false;
    }

    return NPCDefinition->CapabilityTags.HasAny(Capabilities);
}

/* =========================
 * Intent Routing
 * ========================= */

void ANPCBase::RouteInteraction(
    APlayerController* InteractingPC,
    FGameplayTag InteractionIntent)
{
    if (!InteractionIntent.IsValid())
    {
        return;
    }

    static const FGameplayTag InteractIntent =
        FGameplayTag::RequestGameplayTag(TEXT("NPC.Intent.Interact"));
    static const FGameplayTag TalkIntent =
        FGameplayTag::RequestGameplayTag(TEXT("NPC.Intent.Talk"));
    static const FGameplayTag ObserveIntent =
        FGameplayTag::RequestGameplayTag(TEXT("NPC.Intent.Observe"));
    static const FGameplayTag RequestHelpIntent =
        FGameplayTag::RequestGameplayTag(TEXT("NPC.Intent.RequestHelp"));
    static const FGameplayTag ReceiveItemIntent =
        FGameplayTag::RequestGameplayTag(TEXT("NPC.Intent.ReceiveItem"));

    if (InteractionIntent == InteractIntent)
    {
        HandleInteract(InteractingPC);
    }
    else if (InteractionIntent == TalkIntent)
    {
        HandleTalk(InteractingPC);
    }
    else if (InteractionIntent == ObserveIntent)
    {
        HandleObserve(InteractingPC);
    }
    else if (InteractionIntent == RequestHelpIntent)
    {
        HandleRequestHelp(InteractingPC);
    }
    else if (InteractionIntent == ReceiveItemIntent)
    {
        HandleReceiveItem(InteractingPC);
    }
    else
    {
        UE_LOG(LogTemp, Warning,
            TEXT("NPC '%s' received unhandled intent '%s'."),
            *GetName(),
            *InteractionIntent.ToString());
    }
}

/* =========================
 * Studio-Level Intent Handlers
 * ========================= */

void ANPCBase::HandleInteract(APlayerController* InteractingPC)
{
    UE_LOG(LogTemp, Log,
        TEXT("NPC '%s' handled INTERACT."),
        *GetName());

    OnInteract(InteractingPC);
}

void ANPCBase::HandleTalk(APlayerController* InteractingPC)
{
    UE_LOG(LogTemp, Log,
        TEXT("NPC '%s' handled TALK."),
        *GetName());

    OnTalk(InteractingPC);
}

void ANPCBase::HandleObserve(APlayerController* InteractingPC)
{
    UE_LOG(LogTemp, Log,
        TEXT("NPC '%s' handled OBSERVE."),
        *GetName());

    OnObserve(InteractingPC);
}

void ANPCBase::HandleRequestHelp(APlayerController* InteractingPC)
{
    UE_LOG(LogTemp, Log,
        TEXT("NPC '%s' handled REQUEST HELP."),
        *GetName());

    OnRequestHelp(InteractingPC);
}

void ANPCBase::HandleReceiveItem(APlayerController* InteractingPC)
{
    UE_LOG(LogTemp, Log,
        TEXT("NPC '%s' handled RECEIVE ITEM."),
        *GetName());

    OnReceiveItem(InteractingPC);
}
