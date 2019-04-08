// (c) Aiden Storey

// Precompiled Header
#include "ExpBoostPCH.h"

// Local Includes
#include "EBAttribute.h"

// This Include
#include "EBItem.h"

// Implementation
AEBItem::AEBItem( const FObjectInitializer& ObjectInitializer ) :
	Super( ObjectInitializer ),
	Actor( nullptr )
{}

bool AEBItem::AttachItem_Implementation( AActor* _Actor )
{
	bool Return{ false };

	// Ensure actor isn't null
	if ( _Actor != nullptr && this->Actor == nullptr )
	{
		// Iterate over actors components
		for ( auto& Component : _Actor->GetComponents() )
		{
			// Try cast to attribtue
			UEBAttribute* Attribute{ Cast< UEBAttribute >( Component ) };
			if ( Attribute != nullptr )
			{
				// Iterate over modifiers
				for ( auto& Modifier : this->Modifiers )
				{
					// If attribute is what the modifier is expecting 
					if ( Attribute->GetName() == Modifier.AttributeName )
					{
						// Attach modifier
						Attribute->AttachModifier( Modifier );
						this->OnAttachModifier.Broadcast( Modifier, _Actor );
					}
				}
			}
		}

		// Set attached actor
		this->Actor = _Actor;
		Return = true;
	}

	return ( Return );
}

bool AEBItem::DetachItem_Implementation()
{
	bool Return{ false };

	// Ensure actor isn't null
	if ( this->Actor != nullptr  )
	{
		// Iterate over actors components
		for ( auto& Component : this->Actor->GetComponents() )
		{
			// Try cast to attribtue
			UEBAttribute* Attribute{ Cast< UEBAttribute >( Component ) };
			if ( Attribute != nullptr )
			{
				// Iterate over modifiers
				for ( auto& Modifier : this->Modifiers )
				{
					// If attribute is what the modifier is expecting 
					if ( Attribute->GetName() == Modifier.AttributeName )
					{
						// Detach modifier
						Attribute->DetachModifier( Modifier );
						this->OnDetachModifier.Broadcast( Modifier, Actor );
					}
				}
			}
		}

		// Set attached actor
		this->Actor = nullptr;
		Return = true;
	}

	return ( Return );
}