// (c) Aiden Storey

// Ensure STL Min/Max used
#define NOMINMAX

// Precompiled Header
#include "ExpBoostPCH.h"

// UE4 Includes
#include "Curves/CurveFloat.h"

// Library includes
#include <algorithm>

// Local Includes
#include "EBComponent.h"

// This Include
#include "EBAttribute.h"

// Implementation
UEBAttribute::UEBAttribute( const FObjectInitializer& ObjectInitializer ) :
	Super( ObjectInitializer ),
	Level( 1 ),
	LevelMax( 1 ),
	AttributeCurve( nullptr ),
	Value( 0.0f ),
	ValueAtLevel( 0.0f ),
	ValueMax( 0.0f ),
	ValueIsMax( false ),
	ResetValueOnStartUp( true ),
	ResetValueOnLevelUp( true ),
	BindToComponent( false ),
	BindComponent( nullptr )
{
	// Set value at level if attribute curve present
	if ( this->AttributeCurve != nullptr )
	{
		this->ValueAtLevel = this->GetValueAtLevel( this->Level );
	}
	
	// Recalculate total
	this->RecalculateTotal();
	
	// Ensure that tick doesn't happen
	this->PrimaryComponentTick.bCanEverTick = false;
	this->bWantsInitializeComponent = true;
}

void UEBAttribute::SetLevel( int32 _Level )
{
	// Update level if not bound to a component
	if ( this->BindComponent == nullptr )
	{
		this->Level = std::min( this->LevelMax, _Level );
	}

	// Update max from attribute curve if it exists
	if ( this->AttributeCurve != nullptr )
	{
		this->ValueAtLevel = this->GetValueAtLevel( this->Level );
		this->RecalculateTotal();
	}
}

void UEBAttribute::SetLevelMax( int32 _Level )
{
	// Update max level if not bound to a component
	if ( this->BindComponent == nullptr )
	{
		this->LevelMax = _Level;

		if ( this->Level > this->LevelMax )
		{
			this->Level = this->LevelMax;
		}
	}
	
	// Update max from attribute curve if it exists
	if ( this->AttributeCurve != nullptr )
	{
		this->ValueAtLevel = this->GetValueAtLevel( this->Level );
		this->RecalculateTotal();
	}
}

void UEBAttribute::SetAttributeCurve( UCurveFloat* _AttributeCurve )
{
	// Set the attribute curve
	this->AttributeCurve = _AttributeCurve;

	// Update from attribute curve if it exists
	if ( this->AttributeCurve != nullptr )
	{
		this->ValueAtLevel = this->GetValueAtLevel( this->Level );
		this->RecalculateTotal();
	}
}

void UEBAttribute::SetValue( const float& _Value )
{
	// Ensure value doesn't exceed max
	this->Value = std::min( this->ValueMax, std::max( 0.0f, _Value ) );
}

void UEBAttribute::SetValueIsMax( const bool& _ValueIsMax )
{
	this->ValueIsMax = _ValueIsMax;

	// Update value if it should be max
	if ( this->ValueIsMax )
	{
		this->Value = this->ValueMax;
	}
}

void UEBAttribute::SetBindToComponent( const bool& _Bind )
{
	// Bind or unbind component based on updated value
	this->BindToComponent = _Bind;
	if ( this->BindToComponent && this->BindComponent == nullptr )
	{
		this->BindEBComponent();
	}
	else if ( !this->BindToComponent && this->BindComponent != nullptr )
	{
		this->UnbindEBComponent();
	}
}

void UEBAttribute::InitializeComponent()
{
	this->SetBindToComponent( this->BindToComponent );
	
	// Set the value to max if required
	if ( this->ResetValueOnStartUp )
	{
		this->SetValue( this->ValueMax );
	}
}

void UEBAttribute::DestroyComponent( bool bPromoteChildren )
{
	Super::DestroyComponent( bPromoteChildren );

	// Unbind component
	this->SetBindToComponent( false );
}

void UEBAttribute::BindEBComponent()
{
	// Check if able to bind component
	if ( this->BindToComponent && this->BindComponent == nullptr && this->GetOwner() != nullptr )
	{
		// Check if there is a component to bind to
		UEBComponent* Component{ this->GetOwner()->FindComponentByClass< UEBComponent >() };
		if ( Component != nullptr )
		{
			// Bind to component
			Component->BindAttribute( this );
		}
	}
}

void UEBAttribute::UnbindEBComponent()
{
	// Check if able to unbind component
	if ( !this->BindToComponent && this->BindComponent != nullptr )
	{
		// Unbind from component
		this->BindComponent->UnbindAttribute( this );
	}
}

void UEBAttribute::OnBoundLevelUp( int32 _Level )
{
	// Ensure attribute is bound
	if ( this->BindComponent != nullptr )
	{
		// Ensure level doesn't exceed max
		this->Level = std::min( this->LevelMax, _Level );

		// Update max value from attribute curve if it exists
		if ( this->AttributeCurve != nullptr )
		{
			this->ValueAtLevel = this->GetValueAtLevel( this->Level );
			this->RecalculateTotal();
		}

		// Reset the value if required
		if ( this->ResetValueOnLevelUp )
		{
			this->SetValue( this->ValueMax );
		}
	}
}

float UEBAttribute::GetValueAtLevel( int32 _Level )
{
	return ( this->AttributeCurve->GetFloatValue( static_cast< float >( _Level ) ) );
}

void UEBAttribute::AttachModifier( const FEBAttributeModifier& _Modifier )
{
	this->Modifiers.Add( _Modifier );

	this->RecalculateTotal();
}

void UEBAttribute::DetachModifier( const FEBAttributeModifier& _Modifier )
{
	this->Modifiers.RemoveSingle( _Modifier );

	this->RecalculateTotal();
}

void UEBAttribute::RecalculateTotal()
{
	float Multiplyer{ 0.0f };
	float Percentage{ 0.0f };
	float Static{ 0.0f };

	// Iterate over attached modifiers totaling stat modifiers
	for ( auto& aModifier : this->Modifiers )
	{
		switch ( aModifier.Type )
		{
			case EModifierType::MT_Multiplyer:
			{
				Multiplyer += aModifier.Amount;
			} break;

			case EModifierType::MT_Percentage:
			{
				Percentage += aModifier.Amount;
			} break;

			case EModifierType::MT_Static:
			{
				Static += aModifier.Amount;
			} break;

			default: break;
		}
	}

	// Calculate max value from modifiers
	float BaseAndStatic{ this->ValueAtLevel + Static };
	this->ValueMax = BaseAndStatic + ( BaseAndStatic * ( Multiplyer + Percentage / 100.0f ) );

	// Set value depending on required
	if ( this->ValueIsMax )
	{
		this->Value = this->ValueMax;
	}
	else
	{
		this->Value = std::min( this->ValueMax, this->Value );
	}
}

#if WITH_EDITOR

void UEBAttribute::PostEditChangeProperty( struct FPropertyChangedEvent& _Event )
{
	FName PropertyName{ ( _Event.Property != NULL ) ? _Event.Property->GetFName() : NAME_None };

	// Ensure all editor changed parameters meet specifications for change
	if ( PropertyName == FName{ "Level" } )
	{
		this->SetLevel( this->Level );
	}
	else if ( PropertyName == FName{ "LevelMax" } )
	{
		this->SetLevelMax( this->LevelMax );
	}
	else if ( PropertyName == FName{ "AttributeCurve" } )
	{
		this->SetAttributeCurve( this->AttributeCurve );
	}
	else if ( PropertyName == FName{ "Value" } )
	{
		this->SetValue( this->Value );
	}
	else if ( PropertyName == FName{ "ValueIsMax" } )
	{
		this->SetValueIsMax( this->ValueIsMax );
	}

	this->RecalculateTotal();
}

void UEBAttribute::PostEditChangeChainProperty( struct FPropertyChangedChainEvent& _Event )
{
	FName PropertyName{ ( _Event.Property != NULL ) ? _Event.Property->GetFName() : NAME_None };

	// Ensure all editor changed parameters meet specifications for change
	if ( PropertyName == FName{ "Level" } )
	{
		this->SetLevel( this->Level );
	}
	else if ( PropertyName == FName{ "LevelMax" } )
	{
		this->SetLevelMax( this->LevelMax );
	}
	else if ( PropertyName == FName{ "AttributeCurve" } )
	{
		this->SetAttributeCurve( this->AttributeCurve );
	}
	else if ( PropertyName == FName{ "Value" } )
	{
		this->SetValue( this->Value );
	}
	else if ( PropertyName == FName{ "ValueIsMax" } )
	{
		this->SetValueIsMax( this->ValueIsMax );
	}

	this->RecalculateTotal();
}

#endif // WITH_EDITOR