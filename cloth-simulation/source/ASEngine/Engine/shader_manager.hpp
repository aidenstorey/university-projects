//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

template< typename T > bool CShaderManager::CreateShader( const int _kiShaderID )
{
	bool bReturn{ false };

	if ( this->GetShader( _kiShaderID ) == nullptr )
	{
		CShader* pShader = new T( _kiShaderID, this->m_strShaderPath );

		if( pShader != nullptr && pShader->IsInitialised() )
		{
			bReturn = true;

			try
			{
				if ( !pShader->IsInitialised() )
				{
					throw;
				}

				this->m_ShaderList.push_back( pShader );
			}
			catch ( ... )
			{
				bReturn = false;
				delete pShader;
				pShader = nullptr;
			}
		}
	}

	return ( bReturn );
}