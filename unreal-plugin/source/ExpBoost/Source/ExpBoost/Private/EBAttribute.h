// (c) Aiden Storey

#pragma once

// UE4 Includes
#include "Components/ActorComponent.h"

// Local Includes
#include "EBAttributeModifier.h"

// Generated Header
#include "EBAttribute.generated.h"

UCLASS( Blueprintable, HideCategories = ( Tags, ComponentReplication, Activation ) )
class UEBAttribute : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	friend class UEBComponent;

	// Member Functions
public:
	// Accessors
	UFUNCTION( BlueprintCallable, Category = "Attribute System" ) void SetLevel( int32 _Level );
	UFUNCTION( BlueprintCallable, Category = "Attribute System" ) void SetLevelMax( int32 _Level );
	UFUNCTION( BlueprintCallable, Category = "Attribute System" ) void SetAttributeCurve( class UCurveFloat* _AttributeCurve );
	UFUNCTION( BlueprintCallable, Category = "Attribute System" ) void SetValue( const float& _Value );
	UFUNCTION( BlueprintCallable, Category = "Attribute System" ) void SetValueIsMax( const bool& _ValueIsMax );
	UFUNCTION( BlueprintCallable, Category = "Attribute System" ) void SetBindToComponent( const bool& _Bind );

	// Other
	virtual void InitializeComponent() override;
	virtual void DestroyComponent( bool bPromoteChildren ) override;

	void BindEBComponent();
	void UnbindEBComponent();


	float GetValueAtLevel( int32 _Level );

	UFUNCTION( BlueprintCallable, Category = "Attribute System" ) void AttachModifier( const FEBAttributeModifier& _Modifier );
	UFUNCTION( BlueprintCallable, Category = "Attribute System" ) void DetachModifier( const FEBAttributeModifier& _Modifier );

	UFUNCTION( BlueprintCallable, Category = "Attribute System" ) void RecalculateTotal();

protected:
	// Other
	void OnBoundLevelUp( int32 _Level );

#if WITH_EDITOR
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& _Event ) override;
	virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& _Event ) override;
#endif

	// Member Vairables
public:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "The current level of the attribute" ) ) int32 Level;
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "The max level of the attribute" ) ) int32 LevelMax;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "The attribute curve to derive the value given the level" ) ) class UCurveFloat* AttributeCurve;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "The current value of the attribute" ) ) float Value;
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "The value of the attribute at the given level from the attribute curve" ) ) float ValueAtLevel;
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "The max value for the attribute" ) ) float ValueMax;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attribute System", Meta = ( ToolTip = "If the value is always going to be at its max" ) ) bool ValueIsMax;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attribute System", Meta = ( ToolTip = "If the value should be set to max on start up" ) ) bool ResetValueOnStartUp;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attribute System", Meta = ( ToolTip = "If the value should be set to max on level up" ) ) bool ResetValueOnLevelUp;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "List of modifiers that are altering the max value" ) ) TArray< FEBAttributeModifier > Modifiers;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Attribute System", Meta = ( ToolTip = "If the attribute should take level from the component" ) ) bool BindToComponent;

protected:
	class UEBComponent* BindComponent;
};