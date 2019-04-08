// (c) Aiden Storey

#pragma once

// UE4 Includes
#include "Components/ActorComponent.h"

// Generated Header
#include "EBDispatcher.generated.h"

// Types
UENUM( BlueprintType )
enum class EDispatchType : uint8
{
	DT_Individual	UMETA( DisplayName = "Individual" ),
	DT_World		UMETA( DisplayName = "World" ),
	DT_Other		UMETA( DisplayName = "Other" ),
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FDispatchExpDelegate, int32, ExpAmount );

// Prototypes
UCLASS( Blueprintable, HideCategories = ( Tags, ComponentReplication, Activation ) )
class UEBDispatcher : public UActorComponent
{
	GENERATED_BODY()

		// Member Functions
public:
	// Constructors
	UEBDispatcher();

	// Other
	UFUNCTION( BlueprintCallable, Category = "Level System" ) void DispatchExp( class UEBComponent* Component );
	UFUNCTION( BlueprintNativeEvent, Category = "Level System" ) void DispatchExpOther();

protected:
	// Other
	void DispatchExpIndividual( class UEBComponent* Component );
	UFUNCTION( NetMulticast, Reliable )void DispatchExpWorld();

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Level System", Meta = ( ToolTip = "The way in which the dispatcher should give out Exp" ) ) EDispatchType DispatchType;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Level System", Meta = ( ToolTip = "The amount of Exp the dispatcher should give out" ) ) int32 DispatchAmount;


	UPROPERTY( BlueprintAssignable, Category = "Level System", Meta = ( ToolTip = "Delegate called when Exp is dispatched" ) ) FDispatchExpDelegate OnDispatchExp;
	UPROPERTY( BlueprintAssignable, Category = "Level System", Meta = ( ToolTip = "Delegate called when Exp is dispatched to an individual" ) ) FDispatchExpDelegate OnDispatchIndividualExp;
	UPROPERTY( BlueprintAssignable, Category = "Level System", Meta = ( ToolTip = "Delegate called when Exp is dispatched to the world" ) ) FDispatchExpDelegate OnDispatchWorldExp;
	UPROPERTY( BlueprintAssignable, Category = "Level System", Meta = ( ToolTip = "Delegate called when Exp is dispatched as user implemented other" ) ) FDispatchExpDelegate OnDispatchOtherExp;
};