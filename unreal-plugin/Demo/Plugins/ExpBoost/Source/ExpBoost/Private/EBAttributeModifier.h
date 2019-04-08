// (c) Aiden Storey

#pragma once

// UE4 Includes
#include "Object.h"

// Generated Header
#include "EBAttributeModifier.generated.h"

// Types
UENUM( BlueprintType )
enum class EModifierType : uint8
{
	MT_Multiplyer	UMETA( DisplayName = "Multiplyer" ),
	MT_Percentage	UMETA( DisplayName = "Percentage" ),
	MT_Static		UMETA( DisplayName = "Static" ),
};

// Prototypes
USTRUCT( BlueprintType )
struct FEBAttributeModifier
{
	GENERATED_USTRUCT_BODY()

	// Operators
	bool operator== ( const FEBAttributeModifier& _Other ) const
	{
		return ( this->Type == _Other.Type && this->Amount == _Other.Amount );
	}

	bool operator!= ( const FEBAttributeModifier& _Other ) const
	{
		return ( this->Type != _Other.Type || this->Amount != _Other.Amount );
	}

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "Name of attribute to modify" ) ) FString AttributeName;
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "The way the modifier applies to the attribute" ) ) EModifierType Type;
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "Amount the attribute should change" ) ) float Amount;
};