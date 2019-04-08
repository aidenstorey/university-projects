// (c) Aiden Storey

#pragma once

// UE4 Includes
#include "GameFramework/Actor.h"

// Local Includes
#include "EBAttributeModifier.h"

// Generated Header
#include "EBItem.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FItemModifierDelegate, FEBAttributeModifier, Modifier, class AActor*, _Actor );

// Prototypes
UCLASS( Blueprintable, Abstract )
class AEBItem : public AActor
{
	GENERATED_UCLASS_BODY()

	// Member Functions
public:
	// Events
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Attribute System" ) bool AttachItem( class AActor* _Actor );
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Attribute System" ) bool DetachItem();

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "Attribute modifiers used when attached" ) ) TArray< FEBAttributeModifier > Modifiers;
	UPROPERTY( BlueprintAssignable, Category = "Level System", Meta = ( ToolTip = "Delegate called when Exp is gained" ) ) FItemModifierDelegate OnAttachModifier;
	UPROPERTY( BlueprintAssignable, Category = "Level System", Meta = ( ToolTip = "Delegate called when Exp is gained" ) ) FItemModifierDelegate OnDetachModifier;

private:
	class AActor* Actor;

};