// (c) Aiden Storey

// Precompiled Header
#include "ExpBoostPCH.h"

// UE4 Includes
#include "GameFramework/Actor.h"

// Local Includes
#include "EBComponent.h"
#include "EBReceiver.h"

// This Include
#include "EBDispatcher.h"

// Implementation
UEBDispatcher::UEBDispatcher() :
	DispatchType( EDispatchType::DT_Individual ),
	DispatchAmount( 0 )
{}

void UEBDispatcher::DispatchExp( class UEBComponent* Component )
{
	// Call require dispatch type
	switch ( this->DispatchType )
	{
		case EDispatchType::DT_Individual:
		{
			this->DispatchExpIndividual( Component );
			this->OnDispatchIndividualExp.Broadcast( this->DispatchAmount );
		} break;

		case EDispatchType::DT_World:
		{
			this->DispatchExpWorld();
			this->OnDispatchWorldExp.Broadcast( this->DispatchAmount );
		} break;

		case EDispatchType::DT_Other:
		{
			this->DispatchExpOther();
			this->OnDispatchOtherExp.Broadcast( this->DispatchAmount );
		} break;

		default: break;
	}

	this->OnDispatchExp.Broadcast( this->DispatchAmount );
}

void UEBDispatcher::DispatchExpOther_Implementation()
{
}

void UEBDispatcher::DispatchExpIndividual( class UEBComponent* Component )
{
	// Ensure component isn't null
	if ( Component != nullptr )
	{
		// Ensure last hit isn't null
		AActor* LastDamager{ Component->LastDamager };
		if ( LastDamager != nullptr )
		{
			// Get actors receiver if available
			UEBReceiver* Receiver{ LastDamager->FindComponentByClass< UEBReceiver >() };
			if ( Receiver != nullptr )
			{
				// Dispatch Exp to receiver
				Receiver->ReceiveExp( this->DispatchAmount );
			}
		}
	}
}

void UEBDispatcher::DispatchExpWorld_Implementation()
{
	// Dispatch Exp to all valid receivers
	for ( TObjectIterator< UEBReceiver > Receiver; Receiver; ++Receiver )
	{
		if ( Receiver->GetOwner() != nullptr && Receiver->GetOwner()->IsInLevel( this->GetOwner()->GetLevel() ) )
		{
			Receiver->ReceiveExp( this->DispatchAmount );
		}
	}
}