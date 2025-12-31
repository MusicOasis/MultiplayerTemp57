#include "NPC/NPCDefinition.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogNPCDefinition, Log, All);

bool UNPCDefinition::IsValidDefinition() const
{
	bool bIsValid = true;

	/* =========================
	 * Required Identity
	 * ========================= */

	if (NPCId.IsNone())
	{
		UE_LOG(LogNPCDefinition, Warning,
			TEXT("NPCDefinition [%s]: NPCId is not set."),
			*GetName());
		bIsValid = false;
	}

	if (DisplayName.IsEmpty())
	{
		UE_LOG(LogNPCDefinition, Warning,
			TEXT("NPCDefinition [%s]: DisplayName is empty."),
			*GetName());
		bIsValid = false;
	}

	if (ShortDescription.IsEmpty())
	{
		UE_LOG(LogNPCDefinition, Warning,
			TEXT("NPCDefinition [%s]: ShortDescription is empty."),
			*GetName());
		bIsValid = false;
	}

	/* =========================
	 * Narrative & World Role
	 * ========================= */

	if (!NarrativeRole.IsValid() && WorldRoles.IsEmpty())
	{
		UE_LOG(LogNPCDefinition, Warning,
			TEXT("NPCDefinition [%s]: No NarrativeRole or WorldRoles defined."),
			*GetName());
	}

	/* =========================
	 * Capabilities & Interaction
	 * ========================= */

	if (CapabilityTags.IsEmpty())
	{
		UE_LOG(LogNPCDefinition, Warning,
			TEXT("NPCDefinition [%s]: CapabilityTags is empty."),
			*GetName());
	}

	if (AllowedInteractionIntents.IsEmpty())
	{
		UE_LOG(LogNPCDefinition, Warning,
			TEXT("NPCDefinition [%s]: AllowedInteractionIntents is empty."),
			*GetName());
	}

	/* =========================
	 * AI-Specific Validation
	 * ========================= */

	if (AIRelevance != ENPCAIRelevance::None)
	{
		if (PersonaSummary.IsEmpty())
		{
			UE_LOG(LogNPCDefinition, Warning,
				TEXT("NPCDefinition [%s]: AIRelevance is %d but PersonaSummary is empty."),
				*GetName(),
				static_cast<int32>(AIRelevance));
		}

		if (SpeechStyle.IsEmpty())
		{
			UE_LOG(LogNPCDefinition, Warning,
				TEXT("NPCDefinition [%s]: AIRelevance is %d but SpeechStyle is empty."),
				*GetName(),
				static_cast<int32>(AIRelevance));
		}
	}

	/* =========================
	 * Optional Debug Info
	 * ========================= */

	if (bDebugLogInteractions)
	{
		UE_LOG(LogNPCDefinition, Log,
			TEXT("NPCDefinition [%s]: Validation complete (Valid=%s)"),
			*GetName(),
			bIsValid ? TEXT("true") : TEXT("false"));
	}

	return bIsValid;
}
