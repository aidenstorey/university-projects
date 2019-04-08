//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

template< typename T > bool CSceneManager::CreateScene( const int _kiSceneID )
{
	return ( this->CreateScene< T >( _kiSceneID, 0, false ) );
}

template< typename T > bool CSceneManager::CreateScene( const int _kiSceneID, const int _kiParentSceneID )
{
	return ( this->CreateScene< T >( _kiSceneID, _kiParentSceneID, true ) );
}

template< typename T > bool CSceneManager::CreateScene( const int _kiSceneID, const int _kiParentSceneID, const bool _kbParented )
{
	bool bReturn{ true };

	CScene* pParent = nullptr;

	if ( _kbParented )
	{
		pParent = this->GetScene( _kiParentSceneID );
	}

	if ( _kbParented && pParent == nullptr )
	{
		bReturn = false;
	}
	else
	{
		CScene* pScene = new T( _kiSceneID );
		if ( pScene == nullptr )
		{
			bReturn = false;
		}
		else
		{
			try
			{
				if ( !pScene->IsInitialised() )
				{
					throw;
				}

				if( pParent != nullptr )
				{
					pParent->AttachChildScene( pScene );
				}

				this->m_SceneList.push_back( pScene );
			}
			catch ( ... )
			{
				bReturn = false;

				delete pScene;
				pScene = nullptr;
			}
		}
	}

	return ( bReturn );
}