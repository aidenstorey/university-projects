// (c) Aiden Storey

#pragma once

// UE4 Includes
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"

// Generated Header
#include "EBComponent.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FDeathDelegate, class AActor*, LastHit );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FLevelUpDelegate, int32, Level );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FGainExpDelegate, int32, ExpAmount );

// Prototypes
UCLASS( Blueprintable, HideCategories = ( Tags, ComponentReplication, Activation ) )
class UEBComponent : public UActorComponent
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	UEBComponent();

	// Accessors
	UFUNCTION( BlueprintCallable, Category = "Attribute System" ) UEBAttribute* GetAttribute( const FString& Name );
	
	// Other
	virtual void InitializeComponent() override;
	virtual void DestroyComponent( bool bPromoteChildren ) override;

	void BindAttribute( class UEBAttribute* _Attribute );
	void UnbindAttribute( class UEBAttribute* _Attribute );

	UFUNCTION( BlueprintCallable, Category = "Level System" )void ReceiveExp( const int32& ExpAmount );
	UFUNCTION( BlueprintCallable, Category = "Level System" )int32 CalculateExpRequiredAtLevel( const int32& Level );
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Level System" ) int32 CalculateModifiedExp( const int32& ExpAmount );

	// Events
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Damage" ) void TakeDamage( float DamageAmount, AActor* DamageCauser );

protected:
	// Other
	UFUNCTION() void NotifyAttributes( int32 _Level );

	// Member Variables
public:	
	// Attribute System
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attribute System", Meta = ( ToolTip = "Name of the attached component for health" ) )	FString HealthComponentName;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attribute System", Meta = ( ToolTip = "If the component is alive" ) )	bool IsAlive;

	UPROPERTY( BlueprintAssignable, Category = "Attribute System", Meta = ( ToolTip = "Delegate called when component has died" ) )	FDeathDelegate OnDeath;
	
	// Level System
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Level System", Meta = ( ToolTip = "The current level of the component" ) )		int32 Level;
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Level System", Meta = ( ToolTip = "The max level that the component can reach" ) )		int32 LevelMax;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Level System", Meta = ( ToolTip = "If the component can level multiple times from a single Exp amount" ) )		bool AllowMultiLevel;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Level System", Meta = ( ToolTip = "The current Exp for the component" ) )	int32 Exp;
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Level System", Meta = ( ToolTip = "The Exp required for the component to level up" ) )		int32 ExpMax;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Level System", Meta = ( ToolTip = "Curve that the required Exp is taken from" ) )	UCurveFloat* ExpCurve;

	UPROPERTY( BlueprintAssignable, Category = "Level System", Meta = ( ToolTip = "Delegate called when components level increases" ) )		FLevelUpDelegate OnLevelUp;
	UPROPERTY( BlueprintAssignable, Category = "Level System", Meta = ( ToolTip = "Delegate called when Exp is gained" ) )					FGainExpDelegate OnGainExp;

	AActor*				LastDamager;
	TArray< class UEBAttribute* > Attributes;
};