// (c) Aiden Storey

// Precompiled Header
#include "ExpBoostPCH.h"

// Local Includes
#include "EBComponent.h"

// This Include
#include "EBReceiver.h"

// Implementation
void UEBReceiver::ReceiveExp_Implementation( const int32& ExpAmount )
{
	// Give Exp to component if it exists
	UEBComponent* Component{ this->GetOwner()->FindComponentByClass< UEBComponent >() };
	if ( Component != nullptr )
	{
		Component->ReceiveExp( ExpAmount );
		this->OnReceiveExp.Broadcast( ExpAmount );
	}
}