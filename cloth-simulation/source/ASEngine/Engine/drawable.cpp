//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// This Include
#include "drawable.h"

// Prototypes
CDrawable::CDrawable() :
m_bIsVisible( true )
{
	D3DXMatrixIdentity( &this->m_matWorld );
}

CDrawable::~CDrawable()
{}

bool CDrawable::IsVisible() const
{
	return ( this->m_bIsVisible );
}

void CDrawable::SetVisible( const bool _kbVisible )
{
	this->m_bIsVisible = _kbVisible;
}

D3DXMATRIX CDrawable::GetWorldMatrix() const
{
	return ( this->m_matWorld );
}

void CDrawable::SetWorldMatrix( const D3DXMATRIX& _krWorld )
{
	this->m_matWorld = _krWorld;
}

bool CDrawable::Process( const float& _kfTimeDelta, CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset )
{
	bool bReturn{ true };

	bReturn &= this->Update( _kfTimeDelta );

	if( this->m_bIsVisible )
	{
		this->Draw( _pRenderCamera, _krOffset );
	}

	return ( bReturn );
}

bool CDrawable::AttachChild( CDrawable* _pDrawable )
{
	bool bReturn{ false };

	if ( _pDrawable != nullptr )
	{
		bReturn = true;

		try
		{
			this->m_ChildList.push_back( _pDrawable );
			_pDrawable->m_pParent = this;
		}
		catch ( ... )
		{
			bReturn = false;
		}
	}

	return ( bReturn );
}

void CDrawable::DetachChild( CDrawable* _pDrawable )
{
	std::vector< CDrawable* >::iterator iterDelete = std::find( this->m_ChildList.begin(), this->m_ChildList.end(), _pDrawable );
	if ( iterDelete != this->m_ChildList.end() )
	{
		_pDrawable->m_pParent = nullptr;
		this->m_ChildList.erase( iterDelete );
	}
}


bool CDrawable::OnUpdate( const float& _kfTimeDelta )
{
	return ( true );
}

bool CDrawable::PreUpdate( const float& _kfTimeDelta )
{
	return ( true );
}

bool CDrawable::PostUpdate( const float& _kfTimeDelta )
{
	return ( true );
}

void CDrawable::OnDraw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset )
{}

void CDrawable::PreDraw( CCamera* _pRednerCamera, const D3DXMATRIX& _krOffset )
{}

void CDrawable::PostDraw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset )
{}

bool CDrawable::Update( const float& _kfTimeDelta )
{
	bool bReturn{ true };
	
	bReturn &= this->PreUpdate( _kfTimeDelta );

	for( auto& aChild : this->m_ChildList )
	{
		bReturn &= aChild->Update( _kfTimeDelta );
	}
	
	bReturn &= this->OnUpdate( _kfTimeDelta );
	bReturn &= this->PostUpdate( _kfTimeDelta );

	return ( bReturn );
}

void CDrawable::Draw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset )
{
	if( this->m_bIsVisible )
	{
		this->PreDraw( _pRenderCamera, _krOffset );
		this->OnDraw( _pRenderCamera, _krOffset );

		D3DXMATRIX matOffset = this->m_matWorld * _krOffset;
		for( auto& aChild : m_ChildList )
		{
			aChild->Draw( _pRenderCamera, matOffset );
		}

		this->PostDraw( _pRenderCamera, _krOffset );
	}
}