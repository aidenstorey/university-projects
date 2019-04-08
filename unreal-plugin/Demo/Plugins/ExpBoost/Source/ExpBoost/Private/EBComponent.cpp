// (c) Aiden Storey

// Ensure STL min/max used
#define NOMINMAX

// Precompiled Header
#include "ExpBoostPCH.h"

// Library includes
#include <algorithm>

// Local Includes
#include "EBAttribute.h"
#include "EBDispatcher.h"

// This Include
#include "EBComponent.h"

// Implementation
UEBComponent::UEBComponent() :
	HealthComponentName( "EBA_Health" ),
	IsAlive( true ),
	Level( 1 ),
	LevelMax( 1 ),
	AllowMultiLevel( true ),
	Exp( 0 ),
	ExpMax( 1 ),
	ExpCurve( nullptr )
{
	// Bind levelup function
	this->OnLevelUp.AddDynamic( this, &UEBComponent::NotifyAttributes );

	// Ensure that tick doesn't happen
	this->PrimaryComponentTick.bCanEverTick = false;
	this->bWantsInitializeComponent = true;
}

UEBAttribute* UEBComponent::GetAttribute( const FString& Name )
{
	UEBAttribute* Return{ nullptr };
	
	// Check owners components if available
	if ( this->GetOwner() != nullptr )
	{
		for ( auto& Component : this->GetOwner()->GetComponents() )
		{
			// Try cast to attribute 
			UEBAttribute* Attribute{ Cast< UEBAttribute >( Component ) };
			if ( Attribute != nullptr )
			{
				// Check if the name as same as desired
				if ( Attribute->GetName() == Name )
				{
					// Set return to be current attribute
					Return = Attribute;
					break;
				}
			}
		}
	}

	return ( Return );
}

void UEBComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Set initial Exp required
	this->ExpMax = this->CalculateExpRequiredAtLevel( this->Level );
}

void UEBComponent::DestroyComponent( bool bPromoteChildren )
{
	Super::DestroyComponent( bPromoteChildren );

	// Unbind all bound attributes
	for ( auto& Attribute : this->Attributes )
	{
		Attribute->SetBindToComponent( false );
	}
}

void UEBComponent::BindAttribute( class UEBAttribute* _Attribute )
{
	// Add attribute to list
	this->Attributes.Add( _Attribute );

	// Set appropriate values
	_Attribute->SetLevelMax( this->LevelMax );
	_Attribute->SetLevel( this->Level );
	_Attribute->BindComponent = this;
}

void UEBComponent::UnbindAttribute( class UEBAttribute* _Attribute )
{
	// Remove attribute from list
	this->Attributes.Remove( _Attribute );
	
	// Set appropriate values
	_Attribute->BindComponent = nullptr;
}

void UEBComponent::NotifyAttributes( int32 _Level )
{
	// Update bound attributes levels
	for ( auto& Attribute : this->Attributes )
	{
		Attribute->OnBoundLevelUp( _Level );
	}
}

void UEBComponent::ReceiveExp( const int32& ExpAmount )
{
	// Ensure level cap isn't hit
	if ( this->Level != this->LevelMax && ExpAmount > 0 )
	{
		// Apply modifiers to Exp
		int32 ExpModified{ this->CalculateModifiedExp( ExpAmount ) };

		// Add Exp amount to level
		this->Exp += ExpModified;
		this->OnGainExp.Broadcast( ExpModified );

		// Level while exp remains 
		while ( this->Exp >= this->ExpMax )
		{
			// Increase level
			this->Level++;
			this->OnLevelUp.Broadcast( this->Level );

			// Remove max from Exp Total
			this->Exp -= this->ExpMax;

			// Update Exp required for level up
			this->ExpMax = this->CalculateExpRequiredAtLevel( this->Level );

			// Check if level cap hit
			if ( this->Level == this->LevelMax )
			{
				this->Exp = 0;
				break;
			}
			// Check if multi level possible 
			else if ( !this->AllowMultiLevel )
			{
				this->Exp = std::min( this->Exp, std::max( this->ExpMax - 1, 0 ) );
				break;
			}
		}
	}
}

int32 UEBComponent::CalculateExpRequiredAtLevel( const int32& Level )
{
	int32 Return{ INT32_MAX };

	// Get required Exp amount if Exp curve present
	if ( this->ExpCurve != nullptr )
	{
		Return = static_cast< int32 >( this->ExpCurve->GetFloatValue( static_cast< float >( Level ) ) );
	}

	return ( Return );
}

int32 UEBComponent::CalculateModifiedExp_Implementation( const int32& ExpAmount )
{
	return ( ExpAmount );
}

void UEBComponent::TakeDamage_Implementation( float DamageAmount, AActor* DamageCauser )
{
	// Check if alive
	if ( this->IsAlive )
	{
		UEBAttribute* Health{ this->GetAttribute( this->HealthComponentName ) };
		if ( Health != nullptr )
		{
			// Set last Damager
			this->LastDamager = DamageCauser;

			// Deduct damage
			Health->Value = std::max( Health->Value - DamageAmount, 0.0f );

			// Check if alive
			if ( Health->Value == 0.0f )
			{
				// If owner has a dispatcher
				if ( this->GetOwner() != nullptr )
				{
					UEBDispatcher* Dispatcher{ this->GetOwner()->FindComponentByClass< UEBDispatcher >() };
					if ( Dispatcher != nullptr )
					{
						Dispatcher->DispatchExp( this );
					}
				}

				// Notify on death 
				this->OnDeath.Broadcast( DamageCauser );
				this->IsAlive = false;
			}
		}
	}
}