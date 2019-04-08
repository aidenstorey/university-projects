// (c) Aiden Storey

#pragma once

// UE4 Includes
#include "Components/ActorComponent.h"

// Generated Header
#include "EBReceiver.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FReceiveExpDelegate, int32, ExpAmount );

UCLASS( Blueprintable, HideCategories = ( Tags, ComponentReplication, Activation ) )
class UEBReceiver : public UActorComponent
{
	GENERATED_BODY()

		// Member Functions
public:
	// Other
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Level System" ) void ReceiveExp( const int32& ExpAmount );

	// Member Variables
public:
	UPROPERTY( BlueprintAssignable, Category = "Level System", Meta = ( ToolTip = "Delegate called when Exp is received"  ) ) FReceiveExpDelegate OnReceiveExp;
};