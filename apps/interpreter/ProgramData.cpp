#include "ProgramData.h"
#include "agk.h"
#include "interpreter.h"

#include "AGKPluginCommandList.h"

#ifdef AGK_COMPILER_LIB
	#include "Compiler.h"
#endif

#if defined(AGK_LINUX) || defined(AGK_MACOS)
	#include <dlfcn.h>
#endif

using namespace AGK;

#ifdef _DEBUG
	#define AGK_DEBUG
#endif

#define CHECKARRAYRANGE(n,x) if ( (n)<0 || (n)>=(x) ) \
							 { \
								uString err; \
								if ( (x) == 0 ) err.Format("Array index out of bounds, Index: %d, Array Length: <Empty>",(n)); \
								else err.Format("Array index out of bounds, Index: %d, Array Length: %d",(n),(x)-1); \
								RuntimeError(err.GetStr()); \
								return 1; \
							 }

#ifdef AGK_DEBUG
	#define CHECKRANGE(n,x,y,m) if ( (n)<(x) || (n)>(y) ) { RuntimeError((m)); return 1; }
	#define CHECKARRAYTYPE(n,t) if ( ((n)->GetCurrType()) != (t) ) { RuntimeError("Incorrect array type"); return 1; }
#else
	#define CHECKRANGE(n,x,y,m) 
	#define CHECKARRAYTYPE(n,t)
#endif

int ProgramData::CompareString( const void* a, const void* b )
{
	uString *aS = *((uString**)a);
	uString *bS = *((uString**)b);

	return aS->CompareTo( bS->GetStr() );
}

int ProgramData::CompareTypeString( const void* a, const void* b )
{
	sortTypeString *aT = (sortTypeString*)a;
	sortTypeString *bT = (sortTypeString*)b;

	return aT->pString->CompareTo( bT->pString->GetStr() );
}

int ProgramData::CompareStringCase( const void* a, const void* b )
{
	uString *aS = *((uString**)a);
	uString *bS = *((uString**)b);

	return aS->CompareCaseTo( bS->GetStr() );
}

int ProgramData::CompareTypeStringCase( const void* a, const void* b )
{
	sortTypeString *aT = (sortTypeString*)a;
	sortTypeString *bT = (sortTypeString*)b;

	return aT->pString->CompareCaseTo( bT->pString->GetStr() );
}

ProgramData::ProgramData()
{
	m_iPrevInstructionPointer = 0;
	for ( int i = 0; i < 100; i++ ) m_pPrevInstructions[ i ] = 0;

	m_iBytecodeVersion = 0;
	m_iBytecodeDebug = 0;
	m_iGosubCounter = 0;
	m_iFunctionLevel = 0;
	m_iDebugFrameDepth = 0;
	m_iLastBreakLine = -1;
	m_iLastBreakLevel = -1;

	m_pBreakpoints = 0;
	m_pWatchVaraibles = 0;

	m_iProgramCounter = 0;
	m_iNumInstructions = 0;
	m_pInstructions = 0;

	m_iNumPlugins = 0;
	m_pPlugins = 0;

	m_iNumIncludeFiles = 0;
	m_pIncludeFiles = 0;

	m_iNumStrings = 0;
	m_pStringTable = 0;

	m_iNumTypeStructs = 0;
	m_pTypeStructs = 0;

	m_iNumLabels = 0;
	m_pLabels = 0;

	// global variables
	m_iNumVariablesInt = 0;
	m_iNumVariablesFloat = 0;
	m_iNumVariablesString = 0;
	m_iNumVariablesType = 0;
	m_iNumVariablesArray = 0;

	m_pVariablesInt = 0;
	m_pVariablesFloat = 0;
	m_pVariablesString = 0;
	m_pVariablesType = 0; 
	m_pVariablesArray = 0;

	m_pVariablesIntNames = 0;
	m_pVariablesFloatNames = 0;
	m_pVariablesStringNames = 0; 
	m_pVariablesTypeNames = 0; 
	m_pVariablesArrayNames = 0;
		
	// main stack
	m_iStackPtr = 0;
	m_iFramePtr = -1;

	// string stack
	m_iStrStackPtr = 0;
	m_iStrFramePtr = -1;

	m_iMaxStack = 0;
	m_iInstructionCounter = 200000;

	m_iNumFunctions = 0;
	m_pFunctions = 0;

	m_pSubProgram = 0;
}

ProgramData::~ProgramData()
{
	CleanUp();
}

int ProgramData::LoadBytecodeHeader( const char* filename )
{
    AGK::cFile oBytecode;
	if ( !oBytecode.OpenToRead( filename ) )
	{
		return 0;
	}

	// read bytecode version
	int version = oBytecode.ReadInteger();
	if ( version < AGK_BYTECODE_VERSION )
	{
		oBytecode.Close();
		return 0;
	}
	if ( version > AGK_BYTECODE_VERSION )
	{
		oBytecode.Close();
		return 0;
	}

	int dump = oBytecode.ReadInteger();

	int header_size = oBytecode.ReadInteger();
	if ( header_size >= 4 )
	{
		oBytecode.ReadString2( m_sCompanyName );
	}

	oBytecode.Close();
	return 1;
}

#define AGK_PLUGIN_LOAD_ERROR(funcName) { uString err; \
	err.Format( "Failed to load \"%s\" function from plugin \"%s\"", (funcName), m_pPlugins[i].szName ); \
	GeneralError( err ); \
	oBytecode.Close(); \
	return 0; \
}

int ProgramData::LoadBytecode( const char* filename )
{
	CleanUp();

    AGK::cFile oBytecode;
	if ( !oBytecode.OpenToRead( filename ) )
	{
		GeneralError("Failed to open bytecode file for reading");
		return 0;
	}

	// read bytecode version
	m_iBytecodeVersion = oBytecode.ReadInteger();
	if ( m_iBytecodeVersion < AGK_BYTECODE_VERSION )
	{
		if ( m_iBytecodeVersion < 1000000 && AGK_BYTECODE_VERSION > 1000000 )
			GeneralError("The trial version cannot load bytecode compiled with the full version");
		else
			GeneralError("Cannot load bytecode file because it was compiled with an old compiler");

		oBytecode.Close();
		return 0;
	}
	if ( m_iBytecodeVersion > AGK_BYTECODE_VERSION )
	{
		if ( m_iBytecodeVersion > 1000000 && AGK_BYTECODE_VERSION < 1000000 )
			GeneralError("The full version cannot load bytecode compiled with the trial version");
		else
			GeneralError("Cannot load bytecode file because it was compiled with a newer compiler, you will need to update your AGK player app");

		oBytecode.Close();
		return 0;
	}

	m_iBytecodeDebug = oBytecode.ReadInteger();

	// header has already been read
	int header_size = oBytecode.ReadInteger();
	for( int i = 0; i < header_size; i++ ) oBytecode.ReadByte();

	// read plugins
	m_iNumPlugins = oBytecode.ReadInteger();
	if ( m_iNumPlugins > 0 ) m_pPlugins = new stPlugin[ m_iNumPlugins ];
	else m_pPlugins = 0;
	for( UINT i = 0; i < m_iNumPlugins; i++ )
	{
		int length = oBytecode.ReadInteger();
		m_pPlugins[i].szName = new char[ length + 1 ];
		oBytecode.ReadData( m_pPlugins[i].szName, length );
		m_pPlugins[i].szName[ length ] = 0;
		m_pPlugins[i].m_hModule = 0;

		// load plugin
		uString sPluginPath("/Plugins/");
		sPluginPath.Append( m_pPlugins[i].szName );
#ifdef AGK_WINDOWS
	#ifdef _WIN64
		sPluginPath.Append( "/Windows64.dll" );
		if ( App.m_iStandAlone ) agk::PlatformGetFullPathRead( sPluginPath );
		else agk::PlatformGetFullPathWrite( sPluginPath );
		m_pPlugins[i].m_hModule = (void*) LoadLibraryA( sPluginPath );
	#else
		sPluginPath.Append( "/Windows.dll" );
		if ( App.m_iStandAlone ) agk::PlatformGetFullPathRead( sPluginPath );
		else agk::PlatformGetFullPathWrite( sPluginPath );
		m_pPlugins[i].m_hModule = (void*) LoadLibraryA( sPluginPath );
	#endif
#endif
#ifdef AGK_LINUX
		sPluginPath.Append( "/Linux64.so" );
		if ( App.m_iStandAlone ) agk::PlatformGetFullPathRead( sPluginPath );
		else agk::PlatformGetFullPathWrite( sPluginPath );
		m_pPlugins[i].m_hModule = (void*) dlopen(sPluginPath, RTLD_NOW);
#endif
#ifdef AGK_MACOS
		sPluginPath.Append( "/Mac64.dylib" );
		if ( App.m_iStandAlone ) agk::PlatformGetFullPathRead( sPluginPath );
		else agk::PlatformGetFullPathWrite( sPluginPath );
		m_pPlugins[i].m_hModule = (void*) dlopen(sPluginPath, RTLD_NOW);
#endif
#ifdef AGK_ANDROID
		// Load Android library
#endif
#ifdef AGK_IOS
		// Load iOS library
#endif

		if ( m_pPlugins[i].m_hModule == 0 )
		{
			uString err;
			err.Format( "Failed to load plugin \"%s\" required by this app, it may not be available for this platform", m_pPlugins[i].szName );
			GeneralError( err );
			oBytecode.Close();
			return 0;
		}

		void(*ReceiveAGKPtr)( AGKVoidFunc ptr ) = 0;
#ifdef AGK_WINDOWS
		ReceiveAGKPtr = (void(*)(AGKVoidFunc)) GetProcAddress( (HMODULE)m_pPlugins[i].m_hModule, "ReceiveAGKPtr" );
#endif
#ifdef AGK_LINUX
		ReceiveAGKPtr = (void(*)(AGKVoidFunc)) dlsym( m_pPlugins[i].m_hModule, "ReceiveAGKPtr" );
#endif
#ifdef AGK_MACOS
		ReceiveAGKPtr = (void(*)(AGKVoidFunc)) dlsym( m_pPlugins[i].m_hModule, "ReceiveAGKPtr" );
#endif
#ifdef AGK_ANDROID
		// Load ReceiveAGKPtr
#endif
#ifdef AGK_IOS
		// Load ReceiveAGKPtr
#endif
	
		if ( !ReceiveAGKPtr ) 
		{
			uString err;
			err.Format( "Failed to load ReceiveAGKPtr function from plugin \"%s\", it must be exported undecorated", m_pPlugins[i].szName );
			GeneralError( err );
			oBytecode.Close();
			return 0;
		}

		// let plugin load AGK commands
		ReceiveAGKPtr( (AGKVoidFunc) GetAGKFunction );

		m_pPlugins[i].m_iNumCommands = oBytecode.ReadInteger();
		m_pPlugins[i].m_pCommands = new stPluginCommand[ m_pPlugins[i].m_iNumCommands ];
		for ( int j = 0; j < m_pPlugins[i].m_iNumCommands; j++ )
		{
			stPluginCommand *pCommand = m_pPlugins[i].m_pCommands + j;
			pCommand->pFunc = 0;

			int used = oBytecode.ReadByte();
			if ( !used ) 
			{
				continue;
			}

			pCommand->iReturnType = oBytecode.ReadByte();
			pCommand->iNumParams = oBytecode.ReadByte();
			for ( int k = 0; k < pCommand->iNumParams; k++ )
			{
				pCommand->iParamsTypes[ k ] = oBytecode.ReadByte();
			}

			// Windows name
			length = oBytecode.ReadInteger();
			char *temp = new char[ length + 1 ];
			oBytecode.ReadData( temp, length );
			temp[ length ] = 0;
#if defined(AGK_WINDOWS) && !defined(_WIN64)
			pCommand->pFunc = (AGKVoidFunc) GetProcAddress( (HMODULE)m_pPlugins[i].m_hModule, temp );
			if ( !pCommand->pFunc ) AGK_PLUGIN_LOAD_ERROR( temp )
#endif
			delete [] temp;

			// Linux name
			length = oBytecode.ReadInteger();
			temp = new char[ length + 1 ];
			oBytecode.ReadData( temp, length );
			temp[ length ] = 0;
#ifdef AGK_LINUX
			pCommand->pFunc = (AGKVoidFunc) dlsym( m_pPlugins[i].m_hModule, temp );
			if ( !pCommand->pFunc ) AGK_PLUGIN_LOAD_ERROR( temp )
#endif
			delete [] temp;

			// Mac name
			length = oBytecode.ReadInteger();
			temp = new char[ length + 1 ];
			oBytecode.ReadData( temp, length );
			temp[ length ] = 0;
#ifdef AGK_MACOS
			pCommand->pFunc = (AGKVoidFunc) dlsym( m_pPlugins[i].m_hModule, temp );
			if ( !pCommand->pFunc ) AGK_PLUGIN_LOAD_ERROR( temp )
#endif
			delete [] temp;

			// Android name
			length = oBytecode.ReadInteger();
			temp = new char[ length + 1 ];
			oBytecode.ReadData( temp, length );
			temp[ length ] = 0;
#ifdef AGK_ANDROID
			// load command from library
			if ( !pCommand->pFunc ) AGK_PLUGIN_LOAD_ERROR( temp )
#endif
			delete [] temp;

			// iOS name
			length = oBytecode.ReadInteger();
			temp = new char[ length + 1 ];
			oBytecode.ReadData( temp, length );
			temp[ length ] = 0;
#ifdef AGK_IOS
			// load command from library
			if ( !pCommand->pFunc ) AGK_PLUGIN_LOAD_ERROR( temp )
#endif
			delete [] temp;

			// Windows64 name
			length = oBytecode.ReadInteger();
			temp = new char[ length + 1 ];
			oBytecode.ReadData( temp, length );
			temp[ length ] = 0;
#if defined(AGK_WINDOWS) && defined(_WIN64)
			pCommand->pFunc = (AGKVoidFunc) GetProcAddress( (HMODULE)m_pPlugins[i].m_hModule, temp );
			if ( !pCommand->pFunc ) AGK_PLUGIN_LOAD_ERROR( temp )
#endif
			delete [] temp;
		}
	}

	// read include file names
	m_iNumIncludeFiles = oBytecode.ReadInteger();
	if ( m_iNumIncludeFiles > 0 ) m_pIncludeFiles = new char*[ m_iNumIncludeFiles ];
	else m_pIncludeFiles = 0;

	for ( UINT i = 0; i < m_iNumIncludeFiles; i++ )
	{
		int length = oBytecode.ReadInteger();
		m_pIncludeFiles[ i ] = new char[ length+1 ];
		oBytecode.ReadData( m_pIncludeFiles[i], length );
		m_pIncludeFiles[i][length] = 0;
	}

	// read string table
	int checksum = 0;
	m_iNumStrings = oBytecode.ReadInteger();
	if ( m_iNumStrings > 0 ) m_pStringTable = new char*[ m_iNumStrings ];
	else m_pStringTable = 0;

	for ( UINT i = 0; i < m_iNumStrings; i++ )
	{
		int length = oBytecode.ReadInteger();
		m_pStringTable[ i ] = new char[ length+1 ];
		oBytecode.ReadData( m_pStringTable[ i ], length );
		m_pStringTable[ i ][ length ] = 0;

		int flip = -1;
		for ( int j = 0; j < length; j++ )
		{
			checksum += m_pStringTable[ i ][ j ]*j*flip;
			flip = 0-flip;
		}
	}

	// read string table check sum
	if ( checksum != oBytecode.ReadInteger() )
	{
		oBytecode.Close();
		GeneralError("Bytecode error, String table is corrupt");
		return 0;
	}

	#define BUFFER_LENGTH 1024
	char buffer[ BUFFER_LENGTH ];

	// read type structures
	m_iNumTypeStructs = oBytecode.ReadInteger();
	if ( m_iNumTypeStructs > 0 ) m_pTypeStructs = new stTypeDec[ m_iNumTypeStructs ];
	else m_pTypeStructs = 0;

	for ( UINT i = 0; i < m_iNumTypeStructs; i++ )
	{
		// read variable count for this type
		int numVars = oBytecode.ReadInteger();
		m_pTypeStructs[ i ].m_iNumVars = numVars;
		m_pTypeStructs[ i ].m_pVarTypes = new stTypeDecVar[ numVars ];
		int size = 0;
		for ( int v = 0; v < numVars; v++ )
		{
			int type = oBytecode.ReadInteger();
			switch( type )
			{
				case AGK_VARIABLE_INTEGER: 
				{
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varType = AGK_DATA_TYPE_INT; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varOffset = size;
					size += sizeof(int); 
					break;
				}
				case AGK_VARIABLE_FLOAT: 
				{
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varType = AGK_DATA_TYPE_FLOAT; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varOffset = size;
					size += sizeof(float); 
					break;
				}
				case AGK_VARIABLE_STRING: 
				{
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varType = AGK_DATA_TYPE_STRING; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varOffset = size;
					size += sizeof(uString); 
					break;
				}
				case AGK_VARIABLE_TYPE: 
				{
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varType = AGK_DATA_TYPE_TYPE; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varOffset = size;
					size += sizeof(stType);
					break;
				}
				case AGK_VARIABLE_ARRAY_INT: 
				{
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varType = AGK_DATA_TYPE_ARRAY; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varSubType = AGK_DATA_TYPE_INT; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varOffset = size;
					size += sizeof(stArray);
					break;
				}
				case AGK_VARIABLE_ARRAY_FLOAT: 
				{
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varType = AGK_DATA_TYPE_ARRAY; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varSubType = AGK_DATA_TYPE_FLOAT; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varOffset = size;
					size += sizeof(stArray);
					break;
				}
				case AGK_VARIABLE_ARRAY_STRING: 
				{
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varType = AGK_DATA_TYPE_ARRAY; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varSubType = AGK_DATA_TYPE_STRING; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varOffset = size;
					size += sizeof(stArray);
					break;
				}
				case AGK_VARIABLE_ARRAY_TYPE: 
				{
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varType = AGK_DATA_TYPE_ARRAY; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varSubType = AGK_DATA_TYPE_TYPE; 
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varOffset = size;
					size += sizeof(stArray);
					break;
				}
				default: GeneralError( "Bytecode error, Unrecognised variable type" );
			}
			
			// read each variable data
			m_pTypeStructs[ i ].m_pVarTypes[ v ].varTypeIndex = oBytecode.ReadInteger();
			m_pTypeStructs[ i ].m_pVarTypes[ v ].varOrder = (unsigned short) oBytecode.ReadInteger();
			int numDimensions = oBytecode.ReadInteger();
			m_pTypeStructs[ i ].m_pVarTypes[ v ].numDimensions = numDimensions;
			if ( numDimensions > 0 )
			{
				m_pTypeStructs[ i ].m_pVarTypes[ v ].varLength = new int[ numDimensions ];
				// read array dimensions
				for ( int d = 0; d < numDimensions; d++ )
				{
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varLength[ d ] = oBytecode.ReadInteger();
				}
			}

			//if ( m_iBytecodeDebug > 0 ) // type field names will now always be present for JSON support
			{
				int length = oBytecode.ReadInteger();
				if ( length > 0 )
				{
					if ( length >= BUFFER_LENGTH ) 
					{
						oBytecode.Close();
						GeneralError( "Type field name is too long" );
						return 0;
					}
					oBytecode.ReadData( buffer, length );
					buffer[ length ] = 0;
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varName.SetStrUTF8( buffer );
					m_pTypeStructs[ i ].m_pVarTypes[ v ].varName.XOR( 0xff );
				}
			}
		}

		// record the index of the first variable for use when sorting arrays of this type
		m_pTypeStructs[ i ].m_iFirstIndex = oBytecode.ReadInteger();
		m_pTypeStructs[ i ].m_iDataSize = size;
	}

	// read label offsets
	m_iNumLabels = oBytecode.ReadInteger();
	if ( m_iNumLabels > 0 ) m_pLabels = new UINT[ m_iNumLabels ];
	else m_pLabels = 0;

	for ( UINT i = 0; i < m_iNumLabels; i++ )
	{
		m_pLabels[ i ] = oBytecode.ReadInteger();
	}

	// read integer variables
	m_iNumVariablesInt = oBytecode.ReadInteger();
	m_pVariablesInt = 0;
	m_pVariablesIntNames = 0;
	if ( m_iNumVariablesInt ) 
	{
		m_pVariablesInt = new int[ m_iNumVariablesInt ];
		if ( m_iBytecodeDebug > 0 ) m_pVariablesIntNames = new uString[ m_iNumVariablesInt ];
	}

	for ( UINT i = 0; i < m_iNumVariablesInt; i++ )
	{
		m_pVariablesInt[i] = oBytecode.ReadInteger();

		if ( m_iBytecodeDebug > 0 )
		{
			int length = oBytecode.ReadInteger();
			if ( length > 0 )
			{
				if ( length >= BUFFER_LENGTH ) 
				{
					oBytecode.Close();
					GeneralError( "Integer variable name is too long" );
					return 0;
				}
				oBytecode.ReadData( buffer, length );
				buffer[ length ] = 0;
				m_pVariablesIntNames[i].SetStrUTF8( buffer );
				m_pVariablesIntNames[i].XOR( 0xff );
			}
		}
	}

	// read float variables
	m_iNumVariablesFloat = oBytecode.ReadInteger();
	m_pVariablesFloat = 0;
	m_pVariablesFloatNames = 0;
	if ( m_iNumVariablesFloat > 0 ) 
	{
		m_pVariablesFloat = new float[ m_iNumVariablesFloat ];
		if ( m_iBytecodeDebug > 0 ) m_pVariablesFloatNames = new uString[ m_iNumVariablesFloat ];
	}
	
	for ( UINT i = 0; i < m_iNumVariablesFloat; i++ )
	{
		m_pVariablesFloat[i] = oBytecode.ReadFloat();

		if ( m_iBytecodeDebug > 0 )
		{
			int length = oBytecode.ReadInteger();
			if ( length > 0 )
			{
				if ( length >= BUFFER_LENGTH ) 
				{
					oBytecode.Close();
					GeneralError( "Float variable name is too long" );
					return 0;
				}
				oBytecode.ReadData( buffer, length );
				buffer[ length ] = 0;
				m_pVariablesFloatNames[i].SetStrUTF8( buffer );
				m_pVariablesFloatNames[i].XOR( 0xff );
			}
		}
	}

	// read string variables
	m_iNumVariablesString = oBytecode.ReadInteger();
	m_pVariablesString = 0;
	m_pVariablesStringNames = 0;
	if ( m_iNumVariablesString > 0 ) 
	{
		m_pVariablesString = new uString[ m_iNumVariablesString ];
		if ( m_iBytecodeDebug > 0 ) m_pVariablesStringNames = new uString[ m_iNumVariablesString ];
	}
	
	for ( UINT i = 0; i < m_iNumVariablesString; i++ )
	{
		int index = oBytecode.ReadInteger();
		if ( index >= (int)m_iNumStrings ) 
		{
			oBytecode.Close();
			GeneralError( "Bytecode error, String table index out of bounds when reading string variable default value" );
			return 0;
		}
		// negative means no default value
		if ( index >= 0 ) m_pVariablesString[i].SetStrUTF8( m_pStringTable[ index ] );

		if ( m_iBytecodeDebug > 0 )
		{
			int length = oBytecode.ReadInteger();
			if ( length > 0 )
			{
				if ( length >= BUFFER_LENGTH ) 
				{
					oBytecode.Close();
					GeneralError( "String variable name is too long" );
					return 0;
				}
				oBytecode.ReadData( buffer, length );
				buffer[ length ] = 0;
				m_pVariablesStringNames[i].SetStrUTF8( buffer );
				m_pVariablesStringNames[i].XOR( 0xff );
			}
		}
	}

	// read type variables
	m_iNumVariablesType = oBytecode.ReadInteger();
	m_pVariablesType = 0;
	m_pVariablesTypeNames = 0;
	if ( m_iNumVariablesType > 0 ) 
	{
		m_pVariablesType = new stType[ m_iNumVariablesType ];
		if ( m_iBytecodeDebug > 0 ) m_pVariablesTypeNames = new uString[ m_iNumVariablesType ];
	}
	
	for ( UINT i = 0; i < m_iNumVariablesType; i++ )
	{
		int index = oBytecode.ReadInteger();
		if ( index < 0 || index >= (int)m_iNumTypeStructs ) 
		{
			oBytecode.Close();
			GeneralError( "Bytecode error, Type struct index out of bounds when reading type variable" );
			return 0;
		}
		
		AllocType( &(m_pVariablesType[i]), index );

		if ( m_iBytecodeDebug > 0 )
		{
			int length = oBytecode.ReadInteger();
			if ( length > 0 )
			{
				if ( length >= BUFFER_LENGTH ) 
				{
					oBytecode.Close();
					GeneralError( "Type variable name is too long" );
					return 0;
				}
				oBytecode.ReadData( buffer, length );
				buffer[ length ] = 0;
				m_pVariablesTypeNames[i].SetStrUTF8( buffer );
				m_pVariablesTypeNames[i].XOR( 0xff );
			}
		}
	}

	// read array variables
	int pLengths[ 6 ];
	m_iNumVariablesArray = oBytecode.ReadInteger();
	m_pVariablesArray = 0;
	m_pVariablesArrayNames = 0;
	if ( m_iNumVariablesArray > 0 ) 
	{
		m_pVariablesArray = new stArray[ m_iNumVariablesArray ];
		if ( m_iBytecodeDebug > 0 ) m_pVariablesArrayNames = new uString[ m_iNumVariablesArray ];
	}

	for ( UINT i = 0; i < m_iNumVariablesArray; i++ )
	{
		int type = oBytecode.ReadInteger();
		int typeIndex = oBytecode.ReadInteger();
		
		int numDimensions = oBytecode.ReadInteger();
		if ( numDimensions <= 0 )
		{
			oBytecode.Close();
			GeneralError( "Bytecode error, Array has no dimensions" );
			return 0;
		}

		if ( numDimensions > 6 )
		{
			oBytecode.Close();
			GeneralError( "Bytecode error, Array has too many dimensions" );
			return 0;
		}

		AllocArray( &(m_pVariablesArray[i]), type, typeIndex, numDimensions );

		for ( int d = 0; d < numDimensions; d++ )
		{
			pLengths[ d ] = oBytecode.ReadInteger();
		}

		BlockResizeArray( &(m_pVariablesArray[i]), numDimensions, pLengths, type, typeIndex );

		if ( oBytecode.ReadInteger() )
		{
			// default values present
			if ( type == AGK_VARIABLE_INTEGER || type == AGK_VARIABLE_FLOAT )
			{
				for ( int j = 0; j <= pLengths[0]; j++ )
				{
					m_pVariablesArray[i].m_pI[ j ] = oBytecode.ReadInteger();
				}
			}
			else if ( type == AGK_VARIABLE_STRING )
			{
				for ( int j = 0; j <= pLengths[0]; j++ )
				{
					int index = oBytecode.ReadInteger();
					if ( index < -1 || index > (int)m_iNumStrings )
					{
						oBytecode.Close();
						GeneralError( "Bytecode error, String table index out of bounds when reading array variable default value" );
						return 0;
					}
					if ( index >= 0 ) m_pVariablesArray[i].m_pS[ j ]->SetStrUTF8( m_pStringTable[ index ] );
				}
			}
			else
			{
				oBytecode.Close();
				GeneralError( "Bytecode error, Array of types should not have default values" );
				return 0;
			}
		}

		if ( m_iBytecodeDebug > 0 )
		{
			int length = oBytecode.ReadInteger();
			if ( length > 0 )
			{
				if ( length >= BUFFER_LENGTH ) 
				{
					oBytecode.Close();
					GeneralError( "Array variable name is too long" );
					return 0;
				}
				oBytecode.ReadData( buffer, length );
				buffer[ length ] = 0;
				m_pVariablesArrayNames[i].SetStrUTF8( buffer );
				m_pVariablesArrayNames[i].XOR( 0xff );
			}
		}
	}

	// read num instructions
	m_iNumInstructions = oBytecode.ReadInteger();
	m_pInstructions = new stInstruction[ m_iNumInstructions+1 ];

	// read entry point
	m_iProgramCounter = oBytecode.ReadInteger();

	// read instructions
	for ( UINT i = 0; i < m_iNumInstructions; i++ )
	{
		m_pInstructions[ i ].iInstruction = (AGKInstruction)oBytecode.ReadInteger();
		m_pInstructions[ i ].i = oBytecode.ReadInteger();
		m_pInstructions[ i ].i2 = oBytecode.ReadInteger();
		m_pInstructions[ i ].iLineNum = oBytecode.ReadInteger();
		m_pInstructions[ i ].iIncludeFile = oBytecode.ReadInteger();
		m_pInstructions[ i ].iFlags = 0;

		if ( m_pInstructions[ i ].iInstruction >= AGKI_COMMAND_LAST )
		{
			oBytecode.Close();
			GeneralError( "Bytecode contains unknown instructions, please update the AGK Player" );
			return 0;
		}

		// type loading and storing is based on variable ID, needs converting to byte offset
		// this can only be done in the interpreter, where we know variable byte sizes
		if ( m_pInstructions[ i ].iInstruction == AGKI_TYPE_DEREF_INT
		  || m_pInstructions[ i ].iInstruction == AGKI_TYPE_DEREF_FLOAT
		  || m_pInstructions[ i ].iInstruction == AGKI_TYPE_DEREF_STRING
		  || m_pInstructions[ i ].iInstruction == AGKI_TYPE_DEREF_TYPE
		  || m_pInstructions[ i ].iInstruction == AGKI_TYPE_DEREF_ARRAY

		  || m_pInstructions[ i ].iInstruction == AGKI_STORE_TYPE_INT
		  || m_pInstructions[ i ].iInstruction == AGKI_STORE_TYPE_FLOAT
		  || m_pInstructions[ i ].iInstruction == AGKI_STORE_TYPE_STRING
		  
		  || m_pInstructions[ i ].iInstruction == AGKI_INC_TYPE_INT
		  || m_pInstructions[ i ].iInstruction == AGKI_INC_TYPE_FLOAT
		  || m_pInstructions[ i ].iInstruction == AGKI_DEC_TYPE_INT
		  || m_pInstructions[ i ].iInstruction == AGKI_DEC_TYPE_FLOAT )
		{
			int varIndex = m_pInstructions[ i ].i;
			int typeIndex = m_pInstructions[ i ].i2;
			if ( typeIndex < 0 || typeIndex > (int)m_iNumTypeStructs )
			{
				oBytecode.Close();
				GeneralError( "Bytecode error, invalid type index when calculating variable offsets" );
				return 0;
			}

			if ( varIndex < 0 || varIndex > (int)m_pTypeStructs[ typeIndex ].m_iNumVars )
			{
				oBytecode.Close();
				GeneralError( "Bytecode error, invalid variable index when calculating variable offsets" );
				return 0;
			}

			m_pInstructions[ i ].i = m_pTypeStructs[ typeIndex ].m_pVarTypes[ varIndex ].varOffset;
		}
	}

	m_pInstructions[ m_iNumInstructions ].iInstruction = AGKI_END;
	m_pInstructions[ m_iNumInstructions ].iIncludeFile = 0;
	m_pInstructions[ m_iNumInstructions ].iLineNum = m_pInstructions[ m_iNumInstructions-1 ].iLineNum;

	// read function details for debugging
	if ( m_iBytecodeDebug > 0 ) 
	{
		m_iNumFunctions = oBytecode.ReadInteger();
		m_pFunctions = new AGKFunction[ m_iNumFunctions ];
		for ( UINT i = 0; i < m_iNumFunctions; i++ )
		{
			int length = oBytecode.ReadInteger();
			if ( length > 0 )
			{
				if ( length >= BUFFER_LENGTH ) 
				{
					oBytecode.Close();
					GeneralError( "Function name is too long" );
					return 0;
				}
				oBytecode.ReadData( buffer, length );
				buffer[ length ] = 0;
				m_pFunctions[i].sName.SetStrUTF8( buffer );
				m_pFunctions[i].sName.XOR( 0xff );
			}

			m_pFunctions[i].iInstructionPtr = oBytecode.ReadInteger();
			m_pFunctions[i].iNumVars = oBytecode.ReadInteger();
			m_pFunctions[i].pVars = new AGKFunctionVar[ m_pFunctions[i].iNumVars ];
			for ( int v = 0; v < m_pFunctions[i].iNumVars; v++ )
			{
				int length = oBytecode.ReadInteger();
				if ( length > 0 )
				{
					if ( length >= BUFFER_LENGTH ) 
					{
						oBytecode.Close();
						GeneralError( "Function parameter or variable name is too long" );
						return 0;
					}
					oBytecode.ReadData( buffer, length );
					buffer[ length ] = 0;
					m_pFunctions[i].pVars[v].sName.SetStrUTF8( buffer );
					m_pFunctions[i].pVars[v].sName.XOR( 0xff );
				}

				unsigned int type = oBytecode.ReadInteger();
				m_pFunctions[i].pVars[v].iType = type & 0xff;
				m_pFunctions[i].pVars[v].iNumDimensions = (type >> 8) & 0xff;
				m_pFunctions[i].pVars[v].iTypeIndex = (type >> 16) & 0xffff;
				m_pFunctions[i].pVars[v].iVarID = oBytecode.ReadInteger();
			}
		}
	}

	oBytecode.Close();

	m_iInstructionCounter = 200000;

	return 1;
}

int ProgramData::ShouldEnd()
{
	if ( !m_pSubProgram ) return 1;
	else
	{
		if ( m_pSubProgram->ShouldEnd() )
		{
			delete m_pSubProgram;
			m_pSubProgram = 0;
			agk::MasterReset();
			agk::OverrideDirectories( m_sOldWritePath.GetStr(), 0 );
			if ( m_pInstructions[ m_iProgramCounter ].iInstruction == AGKI_RUN_CODE ) m_iProgramCounter++;
		}
		return 0;
	}
}

void ProgramData::CleanUp()
{
	if ( m_pSubProgram ) delete m_pSubProgram;
	m_pSubProgram = 0;

	while( m_pBreakpoints )
	{
		Breakpoint *pBreakpoint = m_pBreakpoints;
		m_pBreakpoints = m_pBreakpoints->m_pNext;
		delete pBreakpoint;
	}

	while( m_pWatchVaraibles )
	{
		AGKVariableWatch *pVar = m_pWatchVaraibles;
		m_pWatchVaraibles = m_pWatchVaraibles->m_pNext;
		delete pVar;
	}

	if ( m_pInstructions ) delete [] m_pInstructions;
	m_pInstructions = 0;

	if ( m_pPlugins )
	{
		for ( UINT i = 0; i < m_iNumPlugins; i++ )
		{
			if ( m_pPlugins[ i ].m_hModule )
			{
#ifdef AGK_WINDOWS
				FreeLibrary( (HMODULE) m_pPlugins[ i ].m_hModule );
#endif
#ifdef AGK_LINUX
				dlclose( m_pPlugins[ i ].m_hModule );
#endif
#ifdef AGK_MACOS
				dlclose( m_pPlugins[ i ].m_hModule );
#endif
#ifdef AGK_ANDROID
				// free Android library
#endif
#ifdef AGK_IOS
				// free iOS library
#endif
			}
		}

		delete [] m_pPlugins;
		m_pPlugins = 0;
		m_iNumPlugins = 0;
	}

	if ( m_pIncludeFiles ) 
	{
		for ( UINT i = 0; i < m_iNumIncludeFiles; i++ )
		{
			if ( m_pIncludeFiles[ i ] ) delete [] m_pIncludeFiles[ i ];
		}
		delete [] m_pIncludeFiles;
		m_pIncludeFiles = 0;
	}

	if ( m_pStringTable ) 
	{
		for ( UINT i = 0; i < m_iNumStrings; i++ )
		{
			if ( m_pStringTable[ i ] ) delete [] m_pStringTable[ i ];
		}
		delete [] m_pStringTable;
		m_pStringTable = 0;
	}

	if ( m_pLabels ) delete [] m_pLabels;
	m_pLabels = 0;

	if ( m_pVariablesInt ) delete [] m_pVariablesInt;
	m_pVariablesInt = 0;

	if ( m_pVariablesFloat ) delete [] m_pVariablesFloat;
	m_pVariablesFloat = 0;

	if ( m_pVariablesString ) delete [] m_pVariablesString;
	m_pVariablesString = 0;

	if ( m_pVariablesType ) 
	{
		for ( UINT i = 0; i < m_iNumVariablesType; i++ )
		{
			FreeType( m_pVariablesType + i );
		}
		delete [] m_pVariablesType;
		m_pVariablesType = 0;
	}
	if ( m_pVariablesArray ) 
	{
		for ( UINT i = 0; i < m_iNumVariablesArray; i++ )
		{
			FreeArray( m_pVariablesArray + i );
		}
		delete [] m_pVariablesArray;
		m_pVariablesArray = 0;
	}

	if ( m_pTypeStructs ) 
	{
		for ( UINT i = 0; i < m_iNumTypeStructs; i++ )
		{
			if ( m_pTypeStructs[ i ].m_pVarTypes )
			{
				for ( unsigned int v = 0; v < m_pTypeStructs[ i ].m_iNumVars; v++ )
				{
					if ( m_pTypeStructs[ i ].m_pVarTypes[ v ].varLength ) delete [] m_pTypeStructs[ i ].m_pVarTypes[ v ].varLength;
				}
				delete [] m_pTypeStructs[ i ].m_pVarTypes;
			}
		}
		delete [] m_pTypeStructs;
		m_pTypeStructs = 0;
	}

	if ( m_pFunctions ) delete [] m_pFunctions;
	m_pFunctions = 0;

	m_iStackPtr = 0;
	m_iFramePtr = -1;

	m_iStrStackPtr = 0;
	m_iStrFramePtr = -1;

	m_iMaxStack = 0;
	m_iInstructionCounter = 200000;

	m_iGosubCounter = 0;
	m_iFunctionLevel = 0;
	m_iDebugFrameDepth = 0;
	m_iLastBreakLine = -1;
	m_iLastBreakLevel = -1;
}

void ProgramData::RuntimeError( const char *msg )
{
	const char* szInclude = "";
	int lineNum = 0;
	if ( m_iProgramCounter > 0 && m_iProgramCounter < m_iNumInstructions ) 
	{
		szInclude = m_pIncludeFiles[ m_pInstructions[ m_iProgramCounter-1 ].iIncludeFile ];
		lineNum = m_pInstructions[ m_iProgramCounter-1 ].iLineNum;
	}
	uString err;
	err.Format( "%s in %s at line %d", msg, szInclude, lineNum );
	GeneralError( err.GetStr() );
}

void ProgramData::AllocType( stType *pType, UINT typeStruct )
{
	stTypeDec *pStruct = m_pTypeStructs + typeStruct;
	pType->m_iVarSize = pStruct->m_iDataSize;
	pType->m_iTypeDec = typeStruct;
	pType->m_pData = (char*) malloc( pType->m_iVarSize );
	memset( pType->m_pData, 0, pType->m_iVarSize );

	int offset = 0;
	for ( UINT i = 0; i < pStruct->m_iNumVars; i++ )
	{
		switch( pStruct->m_pVarTypes[ i ].varType )
		{
			case 1: offset += sizeof(int); break; // int
			case 2: offset += sizeof(float); break; // float
			case 3: offset += sizeof(uString); break; // string
			case 4: // type
			{
				stType *pSubType = (stType*)(pType->m_pData + offset);
				AllocType( pSubType, pStruct->m_pVarTypes[ i ].varTypeIndex );
				offset += sizeof(stType);
				break;
			}
			case 5: // array
			{
				stArray *pArray = (stArray*)(pType->m_pData + offset);
				int dataType = pStruct->m_pVarTypes[ i ].varSubType;
				AllocArray( pArray, dataType, pStruct->m_pVarTypes[ i ].varTypeIndex, pStruct->m_pVarTypes[ i ].numDimensions );
				BlockResizeArray( pArray, pStruct->m_pVarTypes[ i ].numDimensions, pStruct->m_pVarTypes[ i ].varLength, dataType, pStruct->m_pVarTypes[ i ].varTypeIndex );
				offset += sizeof(stArray);
				break;
			}
		}
	}
}

void ProgramData::ZeroType( stType *pType )
{
	stTypeDec *pStruct = m_pTypeStructs + pType->m_iTypeDec;
	
	int offset = 0;
	for ( UINT i = 0; i < pStruct->m_iNumVars; i++ )
	{
		switch( pStruct->m_pVarTypes[ i ].varType )
		{
			case 1: // int
			{
				int *pInt = (int*)(pType->m_pData + offset);
				*pInt = 0;
				offset += sizeof(int); 
				break; 
			}
			case 2: // float
			{
				float *pFloat = (float*)(pType->m_pData + offset);
				*pFloat = 0;
				offset += sizeof(float); 
				break; 
			}
			case 3: // string
			{
				((uString*)(pType->m_pData + offset))->Reset();
				offset += sizeof(uString); 
				break; 
			}
			case 4: // type
			{
				stType *pSubType = (stType*)(pType->m_pData + offset);
				FreeType( pSubType );
				AllocType( pSubType, pStruct->m_pVarTypes[ i ].varTypeIndex );
				offset += sizeof(stType);
				break;
			}
			case 5: // array
			{
				stArray *pArray = (stArray*)(pType->m_pData + offset);
				FreeArray( pArray );
				int dataType = pStruct->m_pVarTypes[ i ].varSubType;
				AllocArray( pArray, dataType, pStruct->m_pVarTypes[ i ].varTypeIndex, pStruct->m_pVarTypes[ i ].numDimensions );
				BlockResizeArray( pArray, pStruct->m_pVarTypes[ i ].numDimensions, pStruct->m_pVarTypes[ i ].varLength, dataType, pStruct->m_pVarTypes[ i ].varTypeIndex );
				offset += sizeof(stArray);
				break;
			}
		}
	}
}

void ProgramData::FreeType( stType *pType )
{
	if ( !pType->m_pData ) return;

	stTypeDec *pStruct = m_pTypeStructs + pType->m_iTypeDec;
	
	int offset = 0;
	for ( UINT i = 0; i < pStruct->m_iNumVars; i++ )
	{
		switch( pStruct->m_pVarTypes[ i ].varType )
		{
			case AGK_DATA_TYPE_INT: offset += sizeof(int); break; // int
			case AGK_DATA_TYPE_FLOAT: offset += sizeof(float); break; // float
			case AGK_DATA_TYPE_STRING: // string
			{
				((uString*)(pType->m_pData + offset))->Reset();
				offset += sizeof(uString); 
				break; 
			}
			case AGK_DATA_TYPE_TYPE: // type
			{
				stType *pSubType = (stType*)(pType->m_pData + offset);
				FreeType( pSubType );
				offset += sizeof(stType);
				break;
			}
			case AGK_DATA_TYPE_ARRAY: // array
			{
				stArray *pArray = (stArray*)(pType->m_pData + offset);
				FreeArray( pArray );
				offset += sizeof(stArray);
				break;
			}
		}
	}

	free( pType->m_pData );
	pType->m_pData = 0;
}

void ProgramData::CopyType( stType *pFromType, stType *pToType )
{
	if ( !pFromType || !pToType ) return;
	if ( pFromType == pToType ) return;

	if ( pFromType->m_iTypeDec != pToType->m_iTypeDec && pToType->m_pData ) 
	{
		FreeType( pToType );
	}

	pToType->m_iTypeDec = pFromType->m_iTypeDec;

	if ( !pToType->m_pData )
	{
		AllocType( pToType, pToType->m_iTypeDec );
	}

	stTypeDec *pStruct = m_pTypeStructs + pFromType->m_iTypeDec;
	
	int offset = 0;
	for ( UINT i = 0; i < pStruct->m_iNumVars; i++ )
	{
		switch( pStruct->m_pVarTypes[ i ].varType )
		{
			case AGK_DATA_TYPE_INT: // int
			{
				*((int*)(pToType->m_pData + offset)) = *((int*)(pFromType->m_pData + offset));
				offset += sizeof(int);  
				break; 
			}
			case AGK_DATA_TYPE_FLOAT: // float
			{
				*((float*)(pToType->m_pData + offset)) = *((float*)(pFromType->m_pData + offset));
				offset += sizeof(float); 
				break; 
			}
			case AGK_DATA_TYPE_STRING: // string
			{
				((uString*)(pToType->m_pData + offset))->SetStrUTF8( ((uString*)(pFromType->m_pData + offset))->GetStr() );
				offset += sizeof(uString); 
				break; 
			}
			case AGK_DATA_TYPE_TYPE: // type
			{
				stType *pFromSubType = (stType*)(pFromType->m_pData + offset);
				stType *pToSubType = (stType*)(pToType->m_pData + offset);
				CopyType( pFromSubType, pToSubType );
				offset += sizeof(stType);
				break;
			}
			case AGK_DATA_TYPE_ARRAY: // array
			{
				stArray *pFromArray = (stArray*)(pFromType->m_pData + offset);
				stArray *pToArray = (stArray*)(pToType->m_pData + offset);
				CopyArray( pFromArray, pToArray );
				offset += sizeof(stArray);
				break;
			}
		}
	}
}

void ProgramData::AllocArray( stArray *pArray, UINT type, UINT subType, UINT numDimensions )
{
	if ( numDimensions > 1 ) pArray->SetDataType( AGK_DATA_TYPE_ARRAY, subType, numDimensions, type );
	else pArray->SetDataType( type, subType, numDimensions, type );
	pArray->m_iLength = 0;
	pArray->m_iTotalLength = 0;
	pArray->m_pI = 0;
}

void ProgramData::SizeArray( stArray *pArray, UINT length )
{
	if ( length == 0 ) 
	{
		FreeArray( pArray );
		return;
	}

	if ( pArray->m_iTotalLength == length ) return;

	UINT type = pArray->GetCurrType();
	UINT subType = pArray->GetTypeType();
	if ( pArray->m_iTotalLength == 0 )
	{
		// initial size
		switch( type )
		{
			case AGK_DATA_TYPE_INT: // int
			{
				pArray->m_pI = new int[ length ]; 
				for ( UINT i = 0; i < length; i++ ) pArray->m_pI[ i ] = 0;
				break;
			}
			case AGK_DATA_TYPE_FLOAT: // float
			{
				pArray->m_pF = new float[ length ]; 
				for ( UINT i = 0; i < length; i++ ) pArray->m_pF[ i ] = 0;
				break;
			}
			case AGK_DATA_TYPE_STRING: // string
			{
				pArray->m_pS = new uString*[ length ]; 
				for ( UINT i = 0; i < length; i++ ) pArray->m_pS [ i ] = new uString();
				break;
			}
			case AGK_DATA_TYPE_TYPE: // type
			{
				pArray->m_pT = new stType*[ length ]; 
				for ( UINT i = 0; i < length; i++ )
				{
					pArray->m_pT[ i ] = new stType();
					AllocType( pArray->m_pT[ i ], subType ); 
				}
				break;
			}
			case AGK_DATA_TYPE_ARRAY: // array
			{
				int dimensions = pArray->GetDimensions();
				int endType = pArray->GetEndType();
				pArray->m_pA = new stArray*[ length ]; 
				for ( UINT i = 0; i < length; i++ )
				{
					pArray->m_pA[ i ] = new stArray();
					if ( dimensions > 2 ) pArray->m_pA[ i ]->SetDataType( AGK_DATA_TYPE_ARRAY, subType, dimensions-1, endType ); 
					else pArray->m_pA[ i ]->SetDataType( endType, subType, dimensions-1, endType );
				}
				break;
			}
			default:
			{
				RuntimeError( "Failed to resize array as it is an unknown type, must be a bug in the compiler" );
				break;
			}
		}
	}
	else
	{
		// resize
		switch( type )
		{
			case AGK_DATA_TYPE_INT: // int
			{
				int *pNewInts = new int[ length ]; 
				for ( UINT i = 0; i < pArray->m_iLength && i < length; i++ ) pNewInts[ i ] = pArray->m_pI[ i ];
				delete [] pArray->m_pI;
				pArray->m_pI = pNewInts;
				for ( UINT i = pArray->m_iLength; i < length; i++ ) pArray->m_pI[ i ] = 0;
				break;
			}
			case AGK_DATA_TYPE_FLOAT: // float
			{
				float *pNewFloats = new float[ length ]; 
				for ( UINT i = 0; i < pArray->m_iLength && i < length; i++ ) pNewFloats[ i ] = pArray->m_pF[ i ];
				delete [] pArray->m_pF;
				pArray->m_pF = pNewFloats;
				for ( UINT i = pArray->m_iLength; i < length; i++ ) pArray->m_pF[ i ] = 0;
				break;
			}
			case AGK_DATA_TYPE_STRING: // string
			{
				uString **pNewStrings = new uString*[ length ]; 
				if ( length > pArray->m_iTotalLength )
				{
					// increase size
					for ( UINT i = 0; i < pArray->m_iTotalLength; i++ ) pNewStrings[ i ] = pArray->m_pS[ i ];
					delete [] pArray->m_pS;
					pArray->m_pS = pNewStrings;
					for ( UINT i = pArray->m_iTotalLength; i < length; i++ ) pArray->m_pS[ i ] = new uString();
				}
				else
				{
					// decrease size
					for ( UINT i = 0; i < length; i++ ) pNewStrings[ i ] = pArray->m_pS[ i ];
					for ( UINT i = length; i < pArray->m_iTotalLength; i++ ) delete pArray->m_pS[ i ];
					delete [] pArray->m_pS;
					pArray->m_pS = pNewStrings;
				}
				break;
			}
			case AGK_DATA_TYPE_TYPE: // type
			{
				stType **pNewTypes = new stType*[ length ]; 
				if ( length > pArray->m_iTotalLength )
				{
					// increase size
					for ( UINT i = 0; i < pArray->m_iTotalLength; i++ ) pNewTypes[ i ] = pArray->m_pT[ i ];
					delete [] pArray->m_pT;
					pArray->m_pT = pNewTypes;
					for ( UINT i = pArray->m_iTotalLength; i < length; i++ )
					{
						pArray->m_pT[ i ] = new stType();
						AllocType( pArray->m_pT[ i ], subType ); 
					}
				}
				else
				{
					// decrease size
					for ( UINT i = 0; i < length; i++ ) pNewTypes[ i ] = pArray->m_pT[ i ];
					for ( UINT i = length; i < pArray->m_iTotalLength; i++ )
					{
						FreeType( pArray->m_pT[ i ] ); 
						delete pArray->m_pT[ i ];
					}
					delete [] pArray->m_pT;
					pArray->m_pT = pNewTypes;
				}
				break;
			}
			case AGK_DATA_TYPE_ARRAY: // array
			{
				stArray **pNewArrays = new stArray*[ length ];
				if ( length > pArray->m_iTotalLength )
				{
					// increase size
					for ( UINT i = 0; i < pArray->m_iTotalLength; i++ ) pNewArrays[ i ] = pArray->m_pA[ i ];
					delete [] pArray->m_pA;
					pArray->m_pA = pNewArrays;
					for ( UINT i = pArray->m_iTotalLength; i < length; i++ )
					{
						pArray->m_pA[ i ] = new stArray();
						pArray->m_pA[ i ]->m_iDataType = pArray->m_pA[ 0 ]->m_iDataType;
					}
				}
				else
				{
					// decrease size
					for ( UINT i = 0; i < length; i++ ) pNewArrays[ i ] = pArray->m_pA[ i ];
					for ( UINT i = length; i < pArray->m_iTotalLength; i++ )
					{
						FreeArray( pArray->m_pA[ i ] );
						delete pArray->m_pA[ i ];
					}
					delete [] pArray->m_pA;
					pArray->m_pA = pNewArrays;
				}
				break;
			}
			default:
			{
				RuntimeError( "Failed to resize array as it is an unknown type, must be a bug in the compiler" );
				break;
			}
		}
	}

	pArray->m_iTotalLength = length;
}

void ProgramData::ZeroArray( stArray *pArray )
{
	if ( pArray->m_iTotalLength == 0 ) return;

	UINT type = pArray->GetCurrType();
	switch( type )
	{
		case AGK_DATA_TYPE_INT: // int
		{
			for ( UINT i = 0; i < pArray->m_iLength; i++ ) pArray->m_pI[ i ] = 0;
			break;
		}
		case AGK_DATA_TYPE_FLOAT: // float
		{
			for ( UINT i = 0; i < pArray->m_iLength; i++ ) pArray->m_pF[ i ] = 0;
			break;
		}
		case AGK_DATA_TYPE_STRING: // string
		{
			for ( UINT i = 0; i < pArray->m_iTotalLength; i++ ) pArray->m_pS[ i ]->Reset();
			break;
		}
		case AGK_DATA_TYPE_TYPE: // type
		{
			for ( UINT i = 0; i < pArray->m_iTotalLength; i++ ) 
			{
				ZeroType( pArray->m_pT[ i ] );
			}
			break;
		}
		case AGK_DATA_TYPE_ARRAY: // array
		{
			for ( UINT i = 0; i < pArray->m_iTotalLength; i++ ) 
			{
				ZeroArray( pArray->m_pA[ i ] );
			}
			break;
		}
	}
}

void ProgramData::FreeArray( stArray *pArray )
{
	if ( pArray->m_iTotalLength == 0 ) return;

	UINT type = pArray->GetCurrType();
	switch( type )
	{
		case AGK_DATA_TYPE_INT: // int
		{
			delete [] pArray->m_pI;
			pArray->m_pI = 0;
			break;
		}
		case AGK_DATA_TYPE_FLOAT: // float
		{
			delete [] pArray->m_pF;
			pArray->m_pF = 0;
			break;
		}
		case AGK_DATA_TYPE_STRING: // string
		{
			for ( UINT i = 0; i < pArray->m_iTotalLength; i++ ) delete pArray->m_pS[ i ];
			delete [] pArray->m_pS;
			pArray->m_pS = 0;
			break;
		}
		case AGK_DATA_TYPE_TYPE: // type
		{
			for ( UINT i = 0; i < pArray->m_iTotalLength; i++ ) 
			{
				FreeType( pArray->m_pT[ i ] );
				delete pArray->m_pT[ i ];
			}
			delete [] pArray->m_pT;
			pArray->m_pT = 0;
			break;
		}
		case AGK_DATA_TYPE_ARRAY: // array
		{
			for ( UINT i = 0; i < pArray->m_iTotalLength; i++ ) 
			{
				FreeArray( pArray->m_pA[ i ] );
				delete pArray->m_pA[ i ];
			}
			delete [] pArray->m_pA;
			pArray->m_pA = 0;
			break;
		}
	}

	pArray->m_iLength = 0;
	pArray->m_iTotalLength = 0;
}

void ProgramData::CopyArray( stArray *pFromArray, stArray *pToArray )
{
	if ( !pFromArray || !pToArray ) return;
	if ( pFromArray == pToArray ) return;

	// if destination array is wrong type, too small, or ridiculously over sized, empty it and start fresh
	if ( pFromArray->m_iDataType != pToArray->m_iDataType 
	  || pFromArray->m_iLength > pToArray->m_iTotalLength 
	  || pToArray->m_iTotalLength > pFromArray->m_iLength + pFromArray->m_iLength/2 )
	{
		FreeArray( pToArray );
	}

	pToArray->m_iDataType = pFromArray->m_iDataType;
	UINT length = pFromArray->m_iLength;
	UINT type = pFromArray->GetCurrType();
	pToArray->m_iLength = length;

	if ( length == 0 ) return;

	switch( type )
	{
		case AGK_DATA_TYPE_INT: // int
		{
			if ( pToArray->m_iTotalLength == 0 ) pToArray->m_pI = new int[ length ]; 
			for ( UINT i = 0; i < length; i++ ) pToArray->m_pI[ i ] = pFromArray->m_pI[ i ];
			break;
		}
		case AGK_DATA_TYPE_FLOAT: // float
		{
			if ( pToArray->m_iTotalLength == 0 ) pToArray->m_pF = new float[ length ]; 
			for ( UINT i = 0; i < length; i++ ) pToArray->m_pF[ i ] = pFromArray->m_pF[ i ];
			break;
		}
		case AGK_DATA_TYPE_STRING: // string
		{
			if ( pToArray->m_iTotalLength == 0 ) pToArray->m_pS = new uString*[ length ]; 
			for ( UINT i = 0; i < length; i++ ) 
			{
				if ( pToArray->m_iTotalLength == 0 ) pToArray->m_pS[ i ] = new uString();
				pToArray->m_pS[ i ]->SetStrUTF8( pFromArray->m_pS[ i ]->GetStr() );
			}
			break;
		}
		case AGK_DATA_TYPE_TYPE: // type
		{
			if ( pToArray->m_iTotalLength == 0 ) pToArray->m_pT = new stType*[ length ]; 
			for ( UINT i = 0; i < length; i++ )
			{
				if ( pToArray->m_iTotalLength == 0 ) pToArray->m_pT[ i ] = new stType();
				CopyType( pFromArray->m_pT[ i ], pToArray->m_pT[ i ] ); 
			}
			break;
		}
		case AGK_DATA_TYPE_ARRAY: // array
		{
			if ( pToArray->m_iTotalLength == 0 ) pToArray->m_pA = new stArray*[ length ]; 
			for ( UINT i = 0; i < length; i++ )
			{
				if ( pToArray->m_iTotalLength == 0 ) pToArray->m_pA[ i ] = new stArray();
				CopyArray( pFromArray->m_pA[ i ], pToArray->m_pA[ i ] ); 
			}
			break;
		}
	}

	if ( pToArray->m_iTotalLength == 0 ) pToArray->m_iTotalLength = length; 
}

void ProgramData::BlockResizeArray( stArray *pArray, UINT numDimensions, UINT dataType, UINT typeType )
{
	// actual array sizes are on the stack
	if ( numDimensions == 0 ) return;
	int length = m_pStack[ m_iStackPtr-numDimensions ].i;
	length++; // convert between BASIC and C++ indexes
	if ( length < 0 )
	{
		RuntimeError( "Array size cannot be less than -1" );
		return;
	}

	if ( numDimensions == 1 )
	{
		// array of datatype
		if ( pArray->GetCurrType() != dataType
		  || pArray->GetDimensions() != numDimensions 
		  || pArray->GetTypeType() != typeType
		  || pArray->GetEndType() != dataType )
		{
			FreeArray( pArray );
			pArray->SetDataType( dataType, typeType, numDimensions, dataType );
		}
	}
	else
	{
		// array of arrays
		if ( pArray->GetCurrType() != AGK_DATA_TYPE_ARRAY 
		  || pArray->GetDimensions() != numDimensions 
		  || pArray->GetTypeType() != typeType
		  || pArray->GetEndType() != dataType )
		{
			FreeArray( pArray );
			pArray->SetDataType( AGK_DATA_TYPE_ARRAY, typeType, numDimensions, dataType );
		}
	}

	SizeArray( pArray, length );
	pArray->m_iLength = pArray->m_iTotalLength;

	if ( numDimensions > 1 )
	{
		// recursively allocate sub arrays
		for( int i = 0; i < length; i++ )
		{
			BlockResizeArray( pArray->m_pA[ i ], numDimensions-1, dataType, typeType );
		}
	}
}

void ProgramData::BlockResizeArray( stArray *pArray, UINT numDimensions, int *pLengths, UINT dataType, UINT typeType )
{
	// actual array sizes are in an array
	if ( numDimensions == 0 ) return;
	int length = pLengths[ 0 ];
	length++; // convert between BASIC and C++ indexes
	if ( length < 0 )
	{
		RuntimeError( "Array size cannot be less than -1" );
		return;
	}

	if ( numDimensions == 1 )
	{
		// array of datatype
		if ( pArray->GetCurrType() != dataType
		  || pArray->GetDimensions() != numDimensions 
		  || pArray->GetTypeType() != typeType
		  || pArray->GetEndType() != dataType )
		{
			FreeArray( pArray );
			pArray->SetDataType( dataType, typeType, numDimensions, dataType );
		}
	}
	else
	{
		// array of arrays
		if ( pArray->GetCurrType() != AGK_DATA_TYPE_ARRAY 
		  || pArray->GetDimensions() != numDimensions 
		  || pArray->GetTypeType() != typeType
		  || pArray->GetEndType() != dataType )
		{
			FreeArray( pArray );
			pArray->SetDataType( AGK_DATA_TYPE_ARRAY, typeType, numDimensions, dataType );
		}
	}

	SizeArray( pArray, length );
	pArray->m_iLength = pArray->m_iTotalLength;

	if ( numDimensions > 1 )
	{
		// recursively allocate sub arrays
		for( int i = 0; i < length; i++ )
		{
			BlockResizeArray( pArray->m_pA[ i ], numDimensions-1, pLengths+1, dataType, typeType );
		}
	}
}

void ProgramData::ArrayToJSON( stArray *pArray, uString &output, int level )
{
	output.AppendUnicode( '[' );
	uString sNumber;
	unsigned int arrayType = pArray->GetCurrType();
	for( unsigned int i = 0; i < pArray->m_iLength; i++ )
	{
		if ( i > 0 ) output.AppendUTF8( ", " );

		switch( arrayType )
		{
			case AGK_DATA_TYPE_INT:
			{
				sNumber.Format( "%d", pArray->m_pI[i] );
				output.Append( sNumber );
				break;
			}
			case AGK_DATA_TYPE_FLOAT:
			{
				sNumber.Format( "%g", pArray->m_pF[i] );
				output.Append( sNumber );
				break;
			}
			case AGK_DATA_TYPE_STRING:
			{
				output.AppendUnicode( '"' );
				uString sCopy;
				sCopy.SetStr( *(pArray->m_pS[i]) );
				sCopy.ReplaceStr( "\\", "\\\\" );
				sCopy.ReplaceStr( "\n", "\\n" );
				sCopy.ReplaceStr( "\r", "\\r" );
				sCopy.ReplaceStr( "\t", "\\t" );
				sCopy.ReplaceStr( "\"", "\\\"" );
				output.Append( sCopy );
				output.AppendUnicode( '"' );
				break;
			}
			case AGK_DATA_TYPE_TYPE:
			{
				output.AppendUnicode( '\n' );
				for( int j = 0; j < level+1; j++ ) output.AppendUTF8( AGK_JSON_INDENT );
				TypeToJSON( pArray->m_pT[ i ], output, level+1 );
				break;
			}
			case AGK_DATA_TYPE_ARRAY:
			{
				output.AppendUnicode( '\n' );
				for( int j = 0; j < level+1; j++ ) output.AppendUTF8( AGK_JSON_INDENT );
				ArrayToJSON( pArray->m_pA[ i ], output, level+1 );
				break;
			}
		}
	}
	if ( arrayType == AGK_DATA_TYPE_TYPE || arrayType == AGK_DATA_TYPE_ARRAY ) 
	{
		output.AppendUnicode( '\n' );
		for( int j = 0; j < level; j++ ) output.AppendUTF8( AGK_JSON_INDENT );
	}
	output.AppendUnicode( ']' );
}

int ProgramData::CompareOrderVars( const void *a, const void *b )
{
	stTypeDecVar* v1 = *((stTypeDecVar**)a);
	stTypeDecVar* v2 = *((stTypeDecVar**)b);

	if ( v1->varOrder < v2->varOrder ) return -1;
	else if ( v1->varOrder > v2->varOrder ) return 1;
	else return 0;
}

void ProgramData::TypeToJSON( stType *pType, uString &output, int level )
{
	output.AppendUnicode( '{' );
		
	stTypeDec *pTypeDec = m_pTypeStructs + pType->m_iTypeDec;
	stTypeDecVar **pOrderedVars = new stTypeDecVar*[ pTypeDec->m_iNumVars ];
	for( unsigned int i = 0; i < pTypeDec->m_iNumVars; i++ )
	{
		pOrderedVars[ i ] = pTypeDec->m_pVarTypes + i;
	}

	qsort( pOrderedVars, pTypeDec->m_iNumVars, sizeof(stTypeDecVar*), CompareOrderVars );
	
	for( unsigned int i = 0; i < pTypeDec->m_iNumVars; i++ )
	{
		stTypeDecVar *pVar = pOrderedVars[ i ];

		if ( i > 0 ) output.AppendUTF8( ", " );

		output.AppendUnicode( '\n' );
		for( int j = 0; j < level+1; j++ ) output.AppendUTF8( AGK_JSON_INDENT );

		output.AppendUnicode( '"' );
		if ( pVar->varName.CharAt(0) == '_' ) output.AppendUTF8( pVar->varName.GetStr() + 1 );
		else output.Append( pVar->varName );
		output.AppendUTF8( "\": " );

		unsigned int offset = pVar->varOffset;
		switch( pVar->varType )
		{
			case AGK_DATA_TYPE_INT:
			{
				uString sNumber;
				sNumber.Format( "%d", *((int*)(pType->m_pData + offset)) );
				output.Append( sNumber );
				break;
			}
			case AGK_DATA_TYPE_FLOAT:
			{
				uString sNumber;
				sNumber.Format( "%g", *((float*)(pType->m_pData + offset)) );
				output.Append( sNumber );
				break;
			}
			case AGK_DATA_TYPE_STRING:
			{
				uString sCopy;
				sCopy.SetStr( *((uString*)(pType->m_pData + offset)) );
				sCopy.ReplaceStr( "\\", "\\\\" );
				sCopy.ReplaceStr( "\n", "\\n" );
				sCopy.ReplaceStr( "\r", "\\r" );
				sCopy.ReplaceStr( "\t", "\\t" );
				sCopy.ReplaceStr( "\"", "\\\"" );

				output.AppendUnicode( '"' );
				output.Append( sCopy );
				output.AppendUnicode( '"' );
				break;
			}
			case AGK_DATA_TYPE_TYPE:
			{
				TypeToJSON( (stType*)(pType->m_pData + offset), output, level+1 );
				break;
			}
			case AGK_DATA_TYPE_ARRAY:
			{
				ArrayToJSON( (stArray*)(pType->m_pData + offset), output, level+1 );
				break;
			}
		}
	}
	output.AppendUnicode( '\n' );
	for( int j = 0; j < level; j++ ) output.AppendUTF8( AGK_JSON_INDENT );
	output.AppendUnicode( '}' );

	delete [] pOrderedVars;
}

void ProgramData::ArrayFromJSON( stArray *pArray, JSONElement *json )
{
	if ( json->GetType() != AGK_JSON_ARRAY )
	{
		uString err; err.Format( "Cannot convert JSON %s to an AGK array", JSONElement::TypeToString( json->GetType() ) );
		agk::Warning( err );
		return;
	}

	JSONArray *pJSONArray = (JSONArray*)json;
	SizeArray( pArray, pJSONArray->m_iNumElements );
	pArray->m_iLength = pJSONArray->m_iNumElements;
	int type = pArray->GetCurrType();
	switch( type )
	{
		case AGK_DATA_TYPE_INT:
		{
			for( UINT i = 0; i < pJSONArray->m_iNumElements; i++ )
			{
				JSONElement *pItem = pJSONArray->GetElement( i );
				switch( pItem->GetType() )
				{
					case AGK_JSON_NUMBER:
					{
						JSONNumber *pNum = (JSONNumber*)pItem;
						if ( pNum->m_iIsInt ) pArray->m_pI[i] = pNum->m_iValue;
						else pArray->m_pI[i] = (int)pNum->m_fValue;
						break;
					}
					case AGK_JSON_BOOL:
					{
						JSONBool *pBool = (JSONBool*)pItem;
						pArray->m_pI[i] = pBool->m_bValue ? 1 : 0;
						break;
					}
					case AGK_JSON_STRING:
					{
						JSONString *pString = (JSONString*)pItem;
						pArray->m_pI[i] = pString->m_sValue.ToInt();
						break;
					}
					default: pArray->m_pI[i] = 0;
				}
			}
			break;
		}
		case AGK_DATA_TYPE_FLOAT:
		{
			for( UINT i = 0; i < pJSONArray->m_iNumElements; i++ )
			{
				JSONElement *pItem = pJSONArray->GetElement( i );
				switch( pItem->GetType() )
				{
					case AGK_JSON_NUMBER:
					{
						JSONNumber *pNum = (JSONNumber*)pItem;
						pArray->m_pF[i] = pNum->m_fValue;
						break;
					}
					case AGK_JSON_BOOL:
					{
						JSONBool *pBool = (JSONBool*)pItem;
						pArray->m_pF[i] = pBool->m_bValue ? 1.0f : 0.0f;
						break;
					}
					case AGK_JSON_STRING:
					{
						JSONString *pString = (JSONString*)pItem;
						pArray->m_pF[i] = pString->m_sValue.ToFloat();
						break;
					}
					default: pArray->m_pI[i] = 0;
				}
			}
			break;
		}
		case AGK_DATA_TYPE_STRING:
		{
			for( UINT i = 0; i < pJSONArray->m_iNumElements; i++ )
			{
				JSONElement *pItem = pJSONArray->GetElement( i );
				switch( pItem->GetType() )
				{
					case AGK_JSON_NUMBER:
					{
						JSONNumber *pNum = (JSONNumber*)pItem;
						if ( pNum->m_iIsInt ) pArray->m_pS[i]->Format( "%d", pNum->m_iValue );
						else pArray->m_pS[i]->Format( "%g", pNum->m_fValue );
						break;
					}
					case AGK_JSON_BOOL:
					{
						JSONBool *pBool = (JSONBool*)pItem;
						pArray->m_pS[i]->SetStrUTF8( pBool->m_bValue ? "true" : "false" );
						break;
					}
					case AGK_JSON_STRING:
					{
						JSONString *pString = (JSONString*)pItem;
						pArray->m_pS[i]->SetStr( pString->m_sValue );
						break;
					}
					default: pArray->m_pS[i]->SetStr("");
				}
			}
			break;
		}
		case AGK_DATA_TYPE_TYPE:
		{
			for( UINT i = 0; i < pJSONArray->m_iNumElements; i++ )
			{
				JSONElement *pItem = pJSONArray->GetElement( i );
				switch( pItem->GetType() )
				{
					case AGK_JSON_OBJECT:
					{
						TypeFromJSON( pArray->m_pT[i], pItem );
						break;
					}
					default: ZeroType( pArray->m_pT[i] );
				}
			}
			break;
		}
		case AGK_DATA_TYPE_ARRAY:
		{
			for( UINT i = 0; i < pJSONArray->m_iNumElements; i++ )
			{
				JSONElement *pItem = pJSONArray->GetElement( i );
				switch( pItem->GetType() )
				{
					case AGK_JSON_ARRAY:
					{
						ArrayFromJSON( pArray->m_pA[i], pItem );
						break;
					}
					default: ZeroArray( pArray->m_pA[i] );
				}
			}
			break;
		}
	}
}

void ProgramData::TypeFromJSON( stType *pType, JSONElement *json )
{
	if ( json->GetType() != AGK_JSON_OBJECT )
	{
		uString err; err.Format( "Cannot convert JSON %s to an AGK type", JSONElement::TypeToString( json->GetType() ) );
		agk::Warning( err );
		return;
	}

	JSONObject *pJSONObject = (JSONObject*)json;
	
	ZeroType( pType );
	stTypeDec *pStruct = m_pTypeStructs + pType->m_iTypeDec;
	for ( UINT i = 0; i < pStruct->m_iNumVars; i++ )
	{
		JSONElement *pItem = pJSONObject->GetElement( pStruct->m_pVarTypes[i].varName );
		if ( !pItem && pStruct->m_pVarTypes[i].varName.CharAt(0) == '_' )
		{
			pItem = pJSONObject->GetElement( pStruct->m_pVarTypes[i].varName.GetStr() + 1 );
		}
		
		int offset = pStruct->m_pVarTypes[ i ].varOffset;
		switch( pStruct->m_pVarTypes[ i ].varType )
		{
			case 1: // int
			{
				if ( !pItem ) continue;
				int *pInt = (int*)(pType->m_pData + offset);
				switch( pItem->GetType() )
				{
					case AGK_JSON_NUMBER:
					{
						JSONNumber *pNum = (JSONNumber*) pItem;
						if ( pNum->m_iIsInt ) *pInt = pNum->m_iValue;
						else *pInt = (int) pNum->m_fValue;
						break;
					}
					case AGK_JSON_BOOL:
					{
						*pInt = ((JSONBool*)pItem)->m_bValue ? 1 : 0;
						break;
					}
					case AGK_JSON_STRING:
					{
						*pInt = ((JSONString*)pItem)->m_sValue.ToInt();
						break;
					}
				}
				break; 
			}
			case 2: // float
			{
				if ( !pItem ) continue;
				float *pFloat = (float*)(pType->m_pData + offset);
				switch( pItem->GetType() )
				{
					case AGK_JSON_NUMBER:
					{
						*pFloat = ((JSONNumber*)pItem)->m_fValue;
						break;
					}
					case AGK_JSON_BOOL:
					{
						*pFloat = ((JSONBool*)pItem)->m_bValue ? 1.0f : 0.0f;
						break;
					}
					case AGK_JSON_STRING:
					{
						*pFloat = ((JSONString*)pItem)->m_sValue.ToFloat();
						break;
					}
				}
				break; 
			}
			case 3: // string
			{
				if ( !pItem ) continue;
				uString *pString = (uString*)(pType->m_pData + offset);
				switch( pItem->GetType() )
				{
					case AGK_JSON_NUMBER:
					{
						JSONNumber *pNum = (JSONNumber*) pItem;
						if ( pNum->m_iIsInt ) pString->Format( "%d", pNum->m_iValue );
						else pString->Format( "%g", pNum->m_fValue );
						break;
					}
					case AGK_JSON_BOOL:
					{
						pString->SetStrUTF8( ((JSONBool*)pItem)->m_bValue ? "true" : "false" );
						break;
					}
					case AGK_JSON_STRING:
					{
						pString->SetStr( ((JSONString*)pItem)->m_sValue );
						break;
					}
				}
				break; 
			}
			case 4: // type
			{
				if ( !pItem ) continue;
				stType *pSubType = (stType*)(pType->m_pData + offset);
				TypeFromJSON( pSubType, pItem );
				break;
			}
			case 5: // array
			{
				stArray *pArray = (stArray*)(pType->m_pData + offset);
				if ( pItem ) ArrayFromJSON( pArray, pItem );
				else FreeArray( pArray );
				break;
			}
		}
	}
}

int ProgramData::RunProgram()
{
	if ( !m_pInstructions ) 
	{
		GeneralError( "Tried to run a program without loading it" );
		return 1;
	}

	if ( m_iNumInstructions == 0 ) 
	{
		GeneralError( "Tried to run a program without any instructions" );
		return 1;
	}

	// limit to the number of instructions without a break, so the message loop gets some time
	int counter = m_iInstructionCounter;

	while(counter > 0)
	{
		counter--;

		CHECKRANGE( m_iProgramCounter, 0, m_iNumInstructions, "Program counter is out of range" )
		CHECKRANGE( m_iStackPtr, 0, 32750, "Stack overflow error, this can be caused by too much recursion" )

		int instruction = m_iProgramCounter++;

		// store the 100 previous instructions
#ifdef AGK_DEBUG
		if ( m_iStackPtr > m_iMaxStack ) m_iMaxStack = m_iStackPtr;
		m_iPrevInstructionPointer++;
		if ( m_iPrevInstructionPointer >= 100 ) m_iPrevInstructionPointer = 0;
		m_pPrevInstructions [ m_iPrevInstructionPointer ] = instruction;
#endif

		switch( m_pInstructions[ instruction ].iInstruction )
		{
			// comon interpreter commands
			#include "AGKCommonSwitch.h"

			// main AGK commands
			#ifdef AGK_2D_ONLY
				#include "AGKCommandSwitch2D.h"
			#else
				#include "AGKCommandSwitch.h"
			#endif

			#ifdef AGK_COMPILER_LIB
				#include "AGKAdditionalSwitch.h"
			#endif

			default: 
			{
				uString err;
				err.Format( "Unrecognised instruction: %d", m_pInstructions[ instruction ].iInstruction );
				RuntimeError( err );
				return 1;
			}
		}
	}

	return 0;
}

void ProgramData::Break()
{
	// break and keep program counter on next instruction, so errors can be skipped
	int instruction = m_iProgramCounter - 1;
	m_iLastBreakLine = m_pInstructions[ instruction ].iLineNum;
	m_iLastBreakLevel = m_iFunctionLevel;
	App.OnAppBreak( m_pIncludeFiles[ m_pInstructions[ instruction ].iIncludeFile ], m_iLastBreakLine );
}

int ProgramData::RunProgramDebug( int stepMode )
{
	if ( !m_pInstructions ) 
	{
		GeneralError( "Tried to run a program without loading it" );
		return 1;
	}

	if ( m_iNumInstructions == 0 ) 
	{
		GeneralError( "Tried to run a program without any instructions" );
		return 1;
	}

	// reset frame depth on every step
	m_iDebugFrameDepth = 0;

	// limit to the number of instructions without a break, so the message loop gets some time
	int counter = m_iInstructionCounter;

	while(counter > 0)
	{
		counter--;

		if ( m_iProgramCounter > m_iNumInstructions ) { RuntimeError("Program counter is out of range"); return 1; }
		if ( m_iStackPtr > 32750 ) { RuntimeError("Stack overflow error, this can be caused by too much recursion"); return 1; }

		int instruction = m_iProgramCounter++;

		if ( (m_pInstructions[ instruction ].iFlags & AGK_INSTRUCTION_BREAKPOINT) != 0 )
		{
			if ( (m_pInstructions[ instruction ].iFlags & AGK_INSTRUCTION_CONTINUE) != 0 )
			{
				// if we previously stopped here, clear the continue marker and continue
				m_pInstructions[ instruction ].iFlags &= ~AGK_INSTRUCTION_CONTINUE;
			}
			else 
			{
				// break and record that the next pass should continue on this instruction
				m_pInstructions[ instruction ].iFlags |= AGK_INSTRUCTION_CONTINUE;
				m_iLastBreakLine = m_pInstructions[ instruction ].iLineNum;
				m_iLastBreakLevel = m_iFunctionLevel;
				App.OnAppBreak( m_pIncludeFiles[ m_pInstructions[ instruction ].iIncludeFile ], m_iLastBreakLine );
				m_iProgramCounter--;
				return 2;
			}
		}

		if ( stepMode > 0 )
		{
			if ( m_pInstructions[ instruction ].iLineNum != m_iLastBreakLine )
			{
				if ( stepMode == 1 || (stepMode == 2 && m_iFunctionLevel == m_iLastBreakLevel) ) // step in or step over
				{
					m_iLastBreakLine = m_pInstructions[ instruction ].iLineNum;
					m_iLastBreakLevel = m_iFunctionLevel;
					App.OnAppBreak( m_pIncludeFiles[ m_pInstructions[ instruction ].iIncludeFile ], m_iLastBreakLine );
					m_iProgramCounter--;
					return 2;
				}
				else if ( stepMode == 3 && m_iFunctionLevel < m_iLastBreakLevel ) // step out
				{
					m_iLastBreakLine = m_pInstructions[ instruction ].iLineNum;
					m_iLastBreakLevel = m_iFunctionLevel;
					App.OnAppBreak( m_pIncludeFiles[ m_pInstructions[ instruction ].iIncludeFile ], m_iLastBreakLine );
					m_iProgramCounter--;
					return 2;
				}
			}
		}

		// store the 100 previous instructions
#ifdef AGK_DEBUG
		if ( m_iStackPtr > m_iMaxStack ) m_iMaxStack = m_iStackPtr;
		m_iPrevInstructionPointer++;
		if ( m_iPrevInstructionPointer >= 100 ) m_iPrevInstructionPointer = 0;
		m_pPrevInstructions [ m_iPrevInstructionPointer ] = instruction;
#endif

		switch( m_pInstructions[ instruction ].iInstruction )
		{
			// comon interpreter commands
			#include "AGKCommonSwitch.h"

			// main AGK commands
			#ifdef AGK_2D_ONLY
				#include "AGKCommandSwitch2D.h"
			#else
				#include "AGKCommandSwitch.h"
			#endif

			default: 
			{
				uString err;
				err.Format( "Unrecognised instruction: %d", m_pInstructions[ instruction ].iInstruction );
				RuntimeError( err );
				return 1;
			}
		}
	}

	return 0;
}

const char* ProgramData::GetCurrentIncludeFile()
{
	if ( m_iProgramCounter == 0 || m_iProgramCounter >= m_iNumInstructions ) return "";
	if ( !m_pInstructions || !m_pIncludeFiles ) return "";

	return m_pIncludeFiles[ m_pInstructions[ m_iProgramCounter-1 ].iIncludeFile ];
}

int ProgramData::GetCurrentLineNum()
{
	if ( m_iProgramCounter == 0 || m_iProgramCounter >= m_iNumInstructions ) return 0;
	if ( !m_pInstructions || !m_pIncludeFiles ) return 0;

	return m_pInstructions[ m_iProgramCounter-1 ].iLineNum;
}

void ProgramData::PrintDebug()
{
	agk::PrintC( "Max Stack: " );
	agk::Print( m_iMaxStack );
	agk::Print( m_pVariablesInt[ 0 ] );
}

void ProgramData::AddBreakpoint( const char *szFile, int line )
{
	uString sFile( szFile );
	sFile.Replace( '\\', '/' );

	int includeID = -1;
	for ( UINT i = 0; i < m_iNumIncludeFiles; i++ )
	{
		if ( sFile.CompareCaseTo( m_pIncludeFiles[ i ] ) == 0 )
		{
			includeID = i;
			break;
		}
	}

	if ( includeID < 0 )
	{
		uString err;
		err.Format( "Failed to add breakpoint, could not find include file %s", szFile );
		agk::Warning( err );
		return;
	}

	int found = -1;
	int diff = 50;
	for( UINT i = 0; i < m_iNumInstructions; i++ )
	{
		if ( m_pInstructions[ i ].iInstruction != AGKI_ILLEGAL_INSTRUCTION 
		  && m_pInstructions[ i ].iIncludeFile == includeID 
		  && m_pInstructions[ i ].iLineNum > line ) 
		{
			if ( m_pInstructions[ i ].iLineNum - line < diff ) diff = m_pInstructions[ i ].iLineNum - line;
		}

		if ( m_pInstructions[ i ].iInstruction != AGKI_ILLEGAL_INSTRUCTION 
		  && m_pInstructions[ i ].iIncludeFile == includeID 
		  && m_pInstructions[ i ].iLineNum == line )
		{
			m_pInstructions[ i ].iFlags |= AGK_INSTRUCTION_BREAKPOINT;
			found = i;
			break;
		}
	}

	if ( found < 0 )
	{
		// find closest line
		for( UINT i = 0; i < m_iNumInstructions; i++ )
		{
			if ( m_pInstructions[ i ].iInstruction != AGKI_ILLEGAL_INSTRUCTION 
			  && m_pInstructions[ i ].iIncludeFile == includeID 
			  && m_pInstructions[ i ].iLineNum > line )
			{
				if ( m_pInstructions[ i ].iLineNum - line == diff )
				{
					m_pInstructions[ i ].iFlags |= AGK_INSTRUCTION_BREAKPOINT;
					found = i;
					break;
				}
			}
		}

		if ( found < 0 )
		{
			uString err;
			err.Format( "Failed to add breakpoint, could not find line number %d in include file %s", line, szFile );
			agk::Warning( err );
			return;
		}
	}

	Breakpoint *pNewBreakpoint = new Breakpoint();
	pNewBreakpoint->m_iIncludeFile = includeID;
	pNewBreakpoint->m_iLine = line;
	pNewBreakpoint->m_iInstruction = found;
	pNewBreakpoint->m_pNext = m_pBreakpoints;
	m_pBreakpoints = pNewBreakpoint;
}

void ProgramData::RemoveBreakpoint( const char *szFile, int line )
{
	uString sFile( szFile );
	sFile.Replace( '\\', '/' );

	int includeID = -1;
	for ( UINT i = 0; i < m_iNumIncludeFiles; i++ )
	{
		if ( sFile.CompareCaseTo( m_pIncludeFiles[ i ] ) == 0 )
		{
			includeID = i;
			break;
		}
	}

	if ( includeID < 0 )
	{
		uString err;
		err.Format( "Failed to remove breakpoint, could not find include file %s", szFile );
		agk::Warning( err );
		return;
	}

	Breakpoint *pBreakpoint = m_pBreakpoints;
	Breakpoint *pLast = 0;
	while ( pBreakpoint )
	{
		if ( pBreakpoint->m_iIncludeFile == includeID && pBreakpoint->m_iLine == line )
		{
			m_pInstructions[ pBreakpoint->m_iInstruction ].iFlags &= ~AGK_INSTRUCTION_BREAKPOINT;

			if ( pLast ) pLast->m_pNext = pBreakpoint->m_pNext;
			else m_pBreakpoints = pBreakpoint->m_pNext;

			Breakpoint *pNext = pBreakpoint->m_pNext;
			delete pBreakpoint;
			pBreakpoint = pNext;
		}
		else
		{
			pLast = pBreakpoint;
			pBreakpoint = pBreakpoint->m_pNext;
		}
	}
}

AGKVariableWatch* ProgramData::AddVariableWatch( const char *szVar )
{
	uString sVar(szVar);
	sVar.Trim( " " );

	// check for duplicates and ignore them
	AGKVariableWatch *pVar = m_pWatchVaraibles;
	while ( pVar )
	{
		if ( pVar->sExpression.CompareCaseTo( sVar ) == 0 ) return pVar;
		pVar = pVar->m_pNext;
	}

	// add new watch variable
	AGKVariableWatch *pNewVar = new AGKVariableWatch();
	pNewVar->sExpression.SetStr( sVar );
	pNewVar->m_pNext = m_pWatchVaraibles;
	m_pWatchVaraibles = pNewVar;

	return pNewVar;
}

void ProgramData::RemoveVariableWatch( const char *szVar )
{
	uString sVar(szVar);
	sVar.Trim( " " );

	// cycle through the whole list to remove any duplicates, don't break
	AGKVariableWatch *pVar = m_pWatchVaraibles;
	AGKVariableWatch *pLast = 0;
	while ( pVar )
	{
		if ( pVar->sExpression.CompareCaseTo( sVar ) == 0 )
		{
			if ( pLast ) pLast->m_pNext = pVar->m_pNext;
			else m_pWatchVaraibles = pVar->m_pNext;

			AGKVariableWatch *pNext = pVar->m_pNext;
			delete pVar;
			pVar = pNext;
		}
		else
		{
			pLast = pVar;
			pVar = pVar->m_pNext;
		}
	}
}

void ProgramData::SetVariable( const char *szVar, const char *szValue )
{
	if ( m_iBytecodeDebug == 0 ) return;

	// find current function
	AGKFunction *pFunction = 0;
	int iDebugFramePtr = m_iFramePtr;
	int iDebugStrFramePtr = m_iStrFramePtr;
	if ( m_iFramePtr >= 0 )
	{
		// if user has selected a different function from the call stack then traverse up the stack
		for( int i = 0; i < m_iDebugFrameDepth; i++ )
		{
			iDebugStrFramePtr = m_pStack[ iDebugFramePtr+2 ].i;
			iDebugFramePtr = m_pStack[ iDebugFramePtr+1 ].i;
			if ( iDebugFramePtr == -1 ) break;
		}

		if ( iDebugFramePtr >= 0 )
		{
			// grab return address of selected function and use this to find the function call command
			// extract the jump pointer from the function call command and use that to match against known functions
			int returnAddress = m_pStack[ iDebugFramePtr ].i;
			int functionPtr = m_pInstructions[ returnAddress-1 ].i;

			for ( UINT i = 0; i < m_iNumFunctions; i++ )
			{
				if ( m_pFunctions[ i ].iInstructionPtr == functionPtr )
				{
					pFunction = m_pFunctions + i;
					break;
				}
			}
		}
	}

	uString sFinal;
	uString sNewValue( szValue );
	int consumed = 0;
	int error = SetDebugVariable( pFunction, iDebugFramePtr, iDebugStrFramePtr, szVar, sNewValue, consumed );
	if ( error == 0 && consumed != strlen(szVar) ) error = 1;
	if ( !error )
	{
		// tell debugger the new value of the variable
		sNewValue.Replace( ':', 0x01 );
		sFinal.Format( "%s:%s", szVar, sNewValue.GetStr() );
		agk::DebugInfo( "Variable", sFinal.GetStr() );

		// find any watch variables that have changed due to this change
		AGKVariableWatch *pVar = m_pWatchVaraibles;
		while ( pVar )
		{
			if ( pVar->sExpression.CompareCaseToN( szVar, pVar->sExpression.GetNumChars() ) == 0 )
			{
				int consumed = 0;
				int error = ParseDebugVariable( pFunction, iDebugFramePtr, iDebugStrFramePtr, pVar->sExpression, sNewValue, consumed );
				if ( error == 0 && consumed != pVar->sExpression.GetLength() )
				{
					sNewValue.SetStrUTF8( "<Invalid Expression>" );
				}
				sNewValue.Replace( ':', 0x01 );
				sFinal.Format( "%s:%s", pVar->sExpression.GetStr(), sNewValue.GetStr() );
				agk::DebugInfo( "Variable", sFinal.GetStr() );
			}
			
			pVar = pVar->m_pNext;
		}
	}
	else
	{	
		// failed to change, send debugger the current value of the variable
		consumed = 0;
		sNewValue.SetStr( "" );
		error = ParseDebugVariable( pFunction, iDebugFramePtr, iDebugStrFramePtr, szVar, sNewValue, consumed );
		if ( error == 0 && consumed != strlen(szVar) ) error = 1;
		if ( !error )
		{
			sNewValue.Replace( ':', 0x01 );
			sFinal.Format( "%s:%s", szVar, sNewValue.GetStr() );
			agk::DebugInfo( "Variable", sFinal.GetStr() );
		}
	}
}

int ProgramData::ParseDebugVariable( AGKFunction *pFunction, int stackPtr, int strStackPtr, const char *szVar, uString &sValue, int &consumed )
{
	const char *varEnd = strpbrk( szVar, "[].," );
	uString identifier;
	if ( varEnd ) identifier.SetStrN( szVar, (UINT)(varEnd-szVar) );
	else 
	{
		varEnd = szVar + strlen(szVar);
		identifier.SetStr( szVar );
	}
	identifier.Trim( " " );

	// check for integer literal, used in array indices
	uString sTest;
	sTest.Format( "%d", identifier.ToInt() );
	if ( sTest.CompareCaseTo( identifier ) == 0 )
	{
		sValue.SetStr( sTest );
		consumed += (int)(varEnd-szVar);
		return 0;
	}

	// search for the identifier in function local variables
	if ( pFunction )
	{
		for ( int v = 0; v < pFunction->iNumVars; v++ )
		{
			if ( pFunction->pVars[ v ].sName.CompareCaseTo( identifier ) != 0 ) continue;
			
			// found the variable
			if ( pFunction->pVars[ v ].iNumDimensions > 0 )
			{
				// it's an array
				stArray* arrayPtr = (stArray*)m_pStack[ stackPtr + pFunction->pVars[v].iVarID ].p;
				consumed += (int)(varEnd-szVar);
				if ( !arrayPtr || stackPtr + pFunction->pVars[v].iVarID >= (int)m_iStackPtr )
				{
					sValue.SetStrUTF8( "<Out Of Scope>" );
					return 1;
				}
				return ParseDebugArray( pFunction, stackPtr, strStackPtr, arrayPtr, varEnd, sValue, consumed );
			}
			else if ( pFunction->pVars[ v ].iType == AGK_VARIABLE_TYPE )
			{
				// it's a type
				stType* typePtr = (stType*)m_pStack[ stackPtr + pFunction->pVars[v].iVarID ].p;
				consumed += (int)(varEnd-szVar);
				if ( !typePtr || stackPtr + pFunction->pVars[v].iVarID >= (int)m_iStackPtr )
				{
					sValue.SetStrUTF8( "<Out Of Scope>" );
					return 1;
				}
				return ParseDebugType( pFunction, stackPtr, strStackPtr, typePtr, varEnd, sValue, consumed );
			}
			else
			{
				// evaluate simple expression
				switch( pFunction->pVars[ v ].iType )
				{
					case AGK_VARIABLE_STRING:
					{
						const char* value = m_pStrStack[ strStackPtr + pFunction->pVars[v].iVarID ].GetStr();
						consumed += (int)(varEnd-szVar);
						if ( strStackPtr + pFunction->pVars[v].iVarID >= (int)m_iStrStackPtr )
						{
							sValue.SetStrUTF8( "<Out Of Scope>" );
							return 1;
						}
						sValue.Format( "\"%s\"", value );
						return 0;
					}
					case AGK_VARIABLE_FLOAT:
					{
						float value = m_pStack[ stackPtr + pFunction->pVars[v].iVarID ].f;
						consumed += (int)(varEnd-szVar);
						if ( stackPtr + pFunction->pVars[v].iVarID >= (int)m_iStackPtr )
						{
							sValue.SetStrUTF8( "<Out Of Scope>" );
							return 1;
						}
						sValue.Format( "%f", value );
						return 0;
					}
					case AGK_VARIABLE_INTEGER:
					{
						int value = m_pStack[ stackPtr + pFunction->pVars[v].iVarID ].i;
						consumed += (int)(varEnd-szVar);
						if ( stackPtr + pFunction->pVars[v].iVarID >= (int)m_iStackPtr )
						{
							sValue.SetStrUTF8( "<Out Of Scope>" );
							return 1;
						}
						sValue.Format( "%d", value );
						return 0;
					}
				}
			}
		}
	}

	// search global integer variables
	for ( UINT i = 0; i < m_iNumVariablesInt; i++ )
	{
		if ( m_pVariablesIntNames[ i ].CompareCaseTo( identifier ) == 0 )
		{
			sValue.Format( "%d", m_pVariablesInt[i] );
			consumed += (int)(varEnd-szVar);
			return 0;
		}
	}

	// search global float variables
	for ( UINT i = 0; i < m_iNumVariablesFloat; i++ )
	{
		if ( m_pVariablesFloatNames[ i ].CompareCaseTo( identifier ) == 0 )
		{
			sValue.Format( "%f", m_pVariablesFloat[i] );
			consumed += (int)(varEnd-szVar);
			return 0;
		}
	}

	// search global string variables
	for ( UINT i = 0; i < m_iNumVariablesString; i++ )
	{
		if ( m_pVariablesStringNames[ i ].CompareCaseTo( identifier ) == 0 )
		{
			sValue.Format( "\"%s\"", m_pVariablesString[i].GetStr() );
			consumed += (int)(varEnd-szVar);
			return 0;
		}
	}

	// search global type variables
	for ( UINT i = 0; i < m_iNumVariablesType; i++ )
	{
		if ( m_pVariablesTypeNames[ i ].CompareCaseTo( identifier ) == 0 )
		{
			// evaluate type expression
			consumed += (int)(varEnd-szVar);
			return ParseDebugType( pFunction, stackPtr, strStackPtr, m_pVariablesType + i, varEnd, sValue, consumed );
		}
	}

	// search global array variables
	for ( UINT i = 0; i < m_iNumVariablesArray; i++ )
	{
		if ( m_pVariablesArrayNames[ i ].CompareCaseTo( identifier ) == 0 )
		{
			// evaluate array expression
			consumed += (int)(varEnd-szVar);
			return ParseDebugArray( pFunction, stackPtr, strStackPtr, m_pVariablesArray + i, varEnd, sValue, consumed );
		}
	}

	sValue.SetStrUTF8( "<Not Found>" );
	return 1;
}

int ProgramData::ParseDebugType( AGKFunction *pFunction, int stackPtr, int strStackPtr, stType *pType, const char *szField, uString &sValue, int &consumed )
{
	// szField should look like ".someField", could also have sub arrays such as ".someField[5]"
	uString sRemaining( szField );
	sRemaining.Trim( " " );
	if ( sRemaining.GetLength() == 0 )
	{
		//sValue.SetStrUTF8( "<Cannot show entire type contents>" );
		//return 1;

		int typetype = pType->m_iTypeDec;
		stTypeDec *pStruct = m_pTypeStructs + typetype;
		stTypeDecVar **pOrderedVars = new stTypeDecVar*[ pStruct->m_iNumVars ];
		for( unsigned int i = 0; i < pStruct->m_iNumVars; i++ )
		{
			pOrderedVars[ i ] = pStruct->m_pVarTypes + i;
		}

		qsort( pOrderedVars, pStruct->m_iNumVars, sizeof(stTypeDecVar*), CompareOrderVars );
	
		sValue.SetStrUTF8( "{ " );
		for( unsigned int i = 0; i < pStruct->m_iNumVars; i++ )
		{
			stTypeDecVar *pVar = pOrderedVars[ i ];

			sValue.AppendAscii( '[' );
			sValue.Append( pVar->varName );
			sValue.Append( "]=" );

			unsigned int offset = pVar->varOffset;
			switch( pVar->varType )
			{
				case AGK_DATA_TYPE_INT:
				{
					uString sNumber;
					sNumber.Format( "%d", *((int*)(pType->m_pData + offset)) );
					sValue.Append( sNumber );
					break;
				}
				case AGK_DATA_TYPE_FLOAT:
				{
					uString sNumber;
					sNumber.Format( "%g", *((float*)(pType->m_pData + offset)) );
					sValue.Append( sNumber );
					break;
				}
				case AGK_DATA_TYPE_STRING:
				{
					sValue.AppendUnicode( '"' );
					sValue.Append( *((uString*)(pType->m_pData + offset)) );
					sValue.AppendUnicode( '"' );
					break;
				}
				case AGK_DATA_TYPE_TYPE:
				{
					sValue.AppendUTF8( "<Add a separate watch for this variable>" );
					break;
				}
				case AGK_DATA_TYPE_ARRAY:
				{
					sValue.AppendUTF8( "<Add a separate watch for this variable>" );
					break;
				}
			}

			if ( i < pStruct->m_iNumVars-1 ) sValue.Append( ", " );
		}
		sValue.Append( " }" );
		return 0;
	}
	
	if ( sRemaining.ByteAt(0) != '.' )
	{
		sValue.SetStrUTF8( "<Invalid Expression>" );
		return 1;
	}

	// grab the field name
	const char *varStart = strchr( szField, '.' ) + 1;
	const char *varEnd = strpbrk( varStart, "[].," );
	uString identifier;
	if ( varEnd ) identifier.SetStrN( varStart, (UINT)(varEnd-varStart) );
	else 
	{
		varEnd = varStart + strlen(varStart);
		identifier.SetStr( varStart );
	}
	identifier.Trim( " " );

	// look for the field name in the type definition
	int iTypeIndex = pType->m_iTypeDec;
	stTypeDec *pTypeStruct = m_pTypeStructs + iTypeIndex;
	for ( UINT i = 0; i < pTypeStruct->m_iNumVars; i++ )
	{
		if ( pTypeStruct->m_pVarTypes[ i ].varName.CompareCaseTo( identifier ) == 0 )
		{
			switch( pTypeStruct->m_pVarTypes[ i ].varType )
			{
				case AGK_DATA_TYPE_ARRAY:
				{
					stArray *pArray = (stArray*)(pType->m_pData + pTypeStruct->m_pVarTypes[ i ].varOffset);
					consumed += (int)(varEnd-szField);
					return ParseDebugArray( pFunction, stackPtr, strStackPtr, pArray, varEnd, sValue, consumed );
				}
				case AGK_DATA_TYPE_TYPE:
				{
					stType *pSubType = (stType*)(pType->m_pData + pTypeStruct->m_pVarTypes[ i ].varOffset);
					consumed += (int)(varEnd-szField);
					return ParseDebugType( pFunction, stackPtr, strStackPtr, pSubType, varEnd, sValue, consumed );
				}
				case AGK_DATA_TYPE_STRING:
				{
					uString *pString = (uString*)(pType->m_pData + pTypeStruct->m_pVarTypes[ i ].varOffset);
					sValue.Format( "\"%s\"", pString->GetStr() );
					consumed += (int)(varEnd-szField);
					return 0;
				}
				case AGK_DATA_TYPE_FLOAT:
				{
					float *pFloat = (float*)(pType->m_pData + pTypeStruct->m_pVarTypes[ i ].varOffset);
					sValue.Format( "%f", *pFloat );
					consumed += (int)(varEnd-szField);
					return 0;
				}
				case AGK_DATA_TYPE_INT:
				{
					int *pInt = (int*)(pType->m_pData + pTypeStruct->m_pVarTypes[ i ].varOffset);
					sValue.Format( "%d", *pInt );
					consumed += (int)(varEnd-szField);
					return 0;
				}
				default:
				{
					sValue.SetStrUTF8( "<Unrecognised Type Data Format>" );
					return 1;
				}
			}
		}
	}

	sValue.SetStrUTF8( "<Type Field Not Found>" );
	return 1;
}

int ProgramData::ParseDebugArray( AGKFunction *pFunction, int stackPtr, int strStackPtr, stArray *pArray, const char *szDimensions, uString &sValue, int &consumed )
{
	// szDimensions should look something like "[a][0]" depending on number of dimensions and index types, could also be "[a][0].typeField"
	uString sRemaining( szDimensions );
	sRemaining.Trim( " " );
	if ( sRemaining.GetLength() == 0 )
	{
		int type = pArray->GetCurrType();
		switch( type )
		{
			case AGK_DATA_TYPE_ARRAY:
			{
				sValue.SetStrUTF8( "<Cannot display a multidimensional array>" );
				return 1;
			}
			case AGK_DATA_TYPE_TYPE:
			{
				sValue.SetStrUTF8( "<Cannot show array of types>" );
				return 1;
			}
			case AGK_DATA_TYPE_STRING:
			{
				// print out entire contents of array
				sValue.SetStrUTF8( "{ " );
				for ( UINT i = 0; i < pArray->m_iLength; i++ )
				{
					sValue.AppendAscii( '[' );
					sValue.AppendUInt( i );
					sValue.Append( "]=\"" );
					sValue.Append( pArray->m_pS[i]->GetStr() );
					sValue.AppendAscii( '\"' );
					if ( i < pArray->m_iLength-1 ) sValue.Append( ", " );
				}
				sValue.Append( " }" );
				return 0;
			}
			case AGK_DATA_TYPE_FLOAT:
			{
				// print out entire contents of array
				sValue.SetStrUTF8( "{ " );
				for ( UINT i = 0; i < pArray->m_iLength; i++ )
				{
					sValue.AppendAscii( '[' );
					sValue.AppendUInt( i );
					sValue.Append( "]=" );
					sValue.AppendFloat( pArray->m_pF[i] );
					if ( i < pArray->m_iLength-1 ) sValue.Append( ", " );
				}
				sValue.Append( " }" );
				return 0;
			}
			case AGK_DATA_TYPE_INT:
			{
				// print out entire contents of array
				sValue.SetStrUTF8( "{ " );
				for ( UINT i = 0; i < pArray->m_iLength; i++ )
				{
					sValue.AppendAscii( '[' );
					sValue.AppendUInt( i );
					sValue.Append( "]=" );
					sValue.AppendInt( pArray->m_pI[i] );
					if ( i < pArray->m_iLength-1 ) sValue.Append( ", " );
				}
				sValue.Append( " }" );
				return 0;
			}
			default:
			{
				sValue.SetStrUTF8( "<Unrecognised Array Type>" );
				return 1;
			}
		}		
	}
	else
	{
		const char *varStart;
		if ( sRemaining.ByteAt(0) == '[' ) varStart = strchr( szDimensions, '[' )+1;
		else if ( sRemaining.ByteAt(0) == ',' ) varStart = strchr( szDimensions, ',' )+1;
		else if ( sRemaining.CompareTo( ".length" ) == 0 ) 
		{
			sValue.Format( "%d", pArray->m_iLength-1 );
		}
		else
		{
			sValue.SetStrUTF8( "<Invalid Expression>" );
			return 1;
		}

		uString sIndex;
		int subConsumed = 0;
		int result = ParseDebugVariable( pFunction, stackPtr, strStackPtr, varStart, sIndex, subConsumed );
		if ( result > 0 ) return result;
		UINT iIndex = sIndex.ToInt();

		// check for strings that convert to index 0 but aren't valid indices
		if ( iIndex == 0 && sIndex.ByteAt(0) != '0' )
		{
			sValue.SetStrUTF8( "<Invalid Array Index>" );
			return 1;
		}

		// find end dimension bracket
		varStart += subConsumed;
		while( *varStart == ' ' ) varStart++;

		if ( *varStart == ']' ) varStart++;
		else if ( *varStart != ',' )
		{
			sValue.SetStrUTF8( "<Invalid Expression>" );
			return 1;
		}

		// check index range
		if ( iIndex >= pArray->m_iLength )
		{
			sValue.SetStrUTF8( "<Array Index Out Of Bounds>" );
			return 1;
		}

		int type = pArray->GetCurrType();
		switch( type )
		{
			case AGK_DATA_TYPE_ARRAY:
			{
				consumed += (int)(varStart - szDimensions);
				return ParseDebugArray( pFunction, stackPtr, strStackPtr, pArray->m_pA[ iIndex ], varStart, sValue, consumed );
			}
			case AGK_DATA_TYPE_TYPE:
			{
				consumed += (int)(varStart - szDimensions);
				return ParseDebugType( pFunction, stackPtr, strStackPtr, pArray->m_pT[ iIndex ], varStart, sValue, consumed );
			}
			case AGK_DATA_TYPE_STRING:
			{
				sValue.Format( "\"%s\"", pArray->m_pS[ iIndex ]->GetStr() );
				consumed += (int)(varStart - szDimensions);
				return 0;
			}
			case AGK_DATA_TYPE_FLOAT:
			{
				sValue.Format( "%f", pArray->m_pF[ iIndex ] );
				consumed += (int)(varStart - szDimensions);
				return 0;
			}
			case AGK_DATA_TYPE_INT:
			{
				sValue.Format( "%d", pArray->m_pI[ iIndex ] );
				consumed += (int)(varStart - szDimensions);
				return 0;
			}
			default:
			{
				sValue.SetStrUTF8( "<Unrecognised Array Type>" );
				return 1;
			}
		}
	}

	sValue.SetStrUTF8( "<Unknown Error>" );
	return 1;
}

int ProgramData::SetDebugVariable( AGKFunction *pFunction, int stackPtr, int strStackPtr, const char *szVar, uString &sNewValue, int &consumed )
{
	const char *varEnd = strpbrk( szVar, "[].," );
	uString identifier;
	if ( varEnd ) identifier.SetStrN( szVar, (UINT)(varEnd-szVar) );
	else 
	{
		varEnd = szVar + strlen(szVar);
		identifier.SetStr( szVar );
	}
	identifier.Trim( " " );

	// search for the identifier in function local variables
	if ( pFunction )
	{
		for ( int v = 0; v < pFunction->iNumVars; v++ )
		{
			if ( pFunction->pVars[ v ].sName.CompareCaseTo( identifier ) != 0 ) continue;
			
			// found the variable
			if ( pFunction->pVars[ v ].iNumDimensions > 0 )
			{
				// it's an array
				stArray* arrayPtr = (stArray*)m_pStack[ stackPtr + pFunction->pVars[v].iVarID ].p;
				consumed += (int)(varEnd-szVar);
				if ( !arrayPtr || stackPtr + pFunction->pVars[v].iVarID >= (int)m_iStackPtr )
				{
					return 1;
				}
				return SetDebugArray( pFunction, stackPtr, strStackPtr, arrayPtr, varEnd, sNewValue, consumed );
			}
			else if ( pFunction->pVars[ v ].iType == AGK_VARIABLE_TYPE )
			{
				// it's a type
				stType* typePtr = (stType*)m_pStack[ stackPtr + pFunction->pVars[v].iVarID ].p;
				consumed += (int)(varEnd-szVar);
				if ( !typePtr || stackPtr + pFunction->pVars[v].iVarID >= (int)m_iStackPtr )
				{
					return 1;
				}
				return SetDebugType( pFunction, stackPtr, strStackPtr, typePtr, varEnd, sNewValue, consumed );
			}
			else
			{
				// evaluate simple expression
				switch( pFunction->pVars[ v ].iType )
				{
					case AGK_VARIABLE_STRING:
					{
						m_pStrStack[ strStackPtr + pFunction->pVars[v].iVarID ].SetStr( sNewValue );
						if ( strStackPtr + pFunction->pVars[v].iVarID >= (int)m_iStrStackPtr ) return 1;
						consumed += (int)(varEnd-szVar);
						return 0;
					}
					case AGK_VARIABLE_FLOAT:
					{
						m_pStack[ stackPtr + pFunction->pVars[v].iVarID ].f = sNewValue.ToFloat();
						if ( stackPtr + pFunction->pVars[v].iVarID >= (int)m_iStackPtr ) return 1;
						consumed += (int)(varEnd-szVar);
						return 0;
					}
					case AGK_VARIABLE_INTEGER:
					{
						m_pStack[ stackPtr + pFunction->pVars[v].iVarID ].i = sNewValue.ToInt();
						if ( stackPtr + pFunction->pVars[v].iVarID >= (int)m_iStackPtr ) return 1;
						consumed += (int)(varEnd-szVar);
						return 0;
					}
				}
			}
		}
	}

	// search global integer variables
	for ( UINT i = 0; i < m_iNumVariablesInt; i++ )
	{
		if ( m_pVariablesIntNames[ i ].CompareCaseTo( identifier ) == 0 )
		{
			m_pVariablesInt[i] = sNewValue.ToInt();
			consumed += (int)(varEnd-szVar);
			return 0;
		}
	}

	// search global float variables
	for ( UINT i = 0; i < m_iNumVariablesFloat; i++ )
	{
		if ( m_pVariablesFloatNames[ i ].CompareCaseTo( identifier ) == 0 )
		{
			m_pVariablesFloat[i] = sNewValue.ToFloat();
			consumed += (int)(varEnd-szVar);
			return 0;
		}
	}

	// search global string variables
	for ( UINT i = 0; i < m_iNumVariablesString; i++ )
	{
		if ( m_pVariablesStringNames[ i ].CompareCaseTo( identifier ) == 0 )
		{
			m_pVariablesString[i].SetStr( sNewValue );
			consumed += (int)(varEnd-szVar);
			return 0;
		}
	}

	// search global type variables
	for ( UINT i = 0; i < m_iNumVariablesType; i++ )
	{
		if ( m_pVariablesTypeNames[ i ].CompareCaseTo( identifier ) == 0 )
		{
			// evaluate type expression
			consumed += (int)(varEnd-szVar);
			return SetDebugType( pFunction, stackPtr, strStackPtr, m_pVariablesType + i, varEnd, sNewValue, consumed );
		}
	}

	// search global array variables
	for ( UINT i = 0; i < m_iNumVariablesArray; i++ )
	{
		if ( m_pVariablesArrayNames[ i ].CompareCaseTo( identifier ) == 0 )
		{
			// evaluate array expression
			consumed += (int)(varEnd-szVar);
			return SetDebugArray( pFunction, stackPtr, strStackPtr, m_pVariablesArray + i, varEnd, sNewValue, consumed );
		}
	}

	return 1;
}

int ProgramData::SetDebugType( AGKFunction *pFunction, int stackPtr, int strStackPtr, stType *pType, const char *szField, uString &sNewValue, int &consumed )
{
	// szField should look like ".someField", could also have sub arrays such as ".someField[5]"
	uString sRemaining( szField );
	sRemaining.Trim( " " );
	if ( sRemaining.GetLength() == 0 )
	{
		// Cannot set a whole type
		return 1;
	}
	
	if ( sRemaining.ByteAt(0) != '.' )
	{
		return 1;
	}

	// grab the field name
	const char *varStart = strchr( szField, '.' ) + 1;
	const char *varEnd = strpbrk( varStart, "[].," );
	uString identifier;
	if ( varEnd ) identifier.SetStrN( varStart, (UINT)(varEnd-varStart) );
	else 
	{
		varEnd = varStart + strlen(varStart);
		identifier.SetStr( varStart );
	}
	identifier.Trim( " " );

	// look for the field name in the type definition
	int iTypeIndex = pType->m_iTypeDec;
	stTypeDec *pTypeStruct = m_pTypeStructs + iTypeIndex;
	for ( UINT i = 0; i < pTypeStruct->m_iNumVars; i++ )
	{
		if ( pTypeStruct->m_pVarTypes[ i ].varName.CompareCaseTo( identifier ) == 0 )
		{
			switch( pTypeStruct->m_pVarTypes[ i ].varType )
			{
				case AGK_DATA_TYPE_ARRAY:
				{
					stArray *pArray = (stArray*)(pType->m_pData + pTypeStruct->m_pVarTypes[ i ].varOffset);
					consumed += (int)(varEnd-szField);
					return SetDebugArray( pFunction, stackPtr, strStackPtr, pArray, varEnd, sNewValue, consumed );
				}
				case AGK_DATA_TYPE_TYPE:
				{
					stType *pSubType = (stType*)(pType->m_pData + pTypeStruct->m_pVarTypes[ i ].varOffset);
					consumed += (int)(varEnd-szField);
					return SetDebugType( pFunction, stackPtr, strStackPtr, pSubType, varEnd, sNewValue, consumed );
				}
				case AGK_DATA_TYPE_STRING:
				{
					uString *pString = (uString*)(pType->m_pData + pTypeStruct->m_pVarTypes[ i ].varOffset);
					pString->SetStr( sNewValue );
					consumed += (int)(varEnd-szField);
					return 0;
				}
				case AGK_DATA_TYPE_FLOAT:
				{
					float *pFloat = (float*)(pType->m_pData + pTypeStruct->m_pVarTypes[ i ].varOffset);
					*pFloat = sNewValue.ToFloat();
					consumed += (int)(varEnd-szField);
					return 0;
				}
				case AGK_DATA_TYPE_INT:
				{
					int *pInt = (int*)(pType->m_pData + pTypeStruct->m_pVarTypes[ i ].varOffset);
					*pInt = sNewValue.ToInt();
					consumed += (int)(varEnd-szField);
					return 0;
				}
				default:
				{
					return 1;
				}
			}
		}
	}

	return 1;
}

int ProgramData::SetDebugArray( AGKFunction *pFunction, int stackPtr, int strStackPtr, stArray *pArray, const char *szDimensions, uString &sNewValue, int &consumed )
{
	// szDimensions should look something like "[a][0]" depending on number of dimensions and index types, could also be "[a][0].typeField"
	uString sRemaining( szDimensions );
	sRemaining.Trim( " " );
	if ( sRemaining.GetLength() == 0 )
	{
		// cannot set an entire array
		return 1;		
	}
	else
	{
		const char *varStart;
		if ( sRemaining.ByteAt(0) == '[' ) varStart = strchr( szDimensions, '[' )+1;
		else if ( sRemaining.ByteAt(0) == ',' ) varStart = strchr( szDimensions, ',' )+1;
		else
		{
			return 1;
		}

		uString sIndex;
		int subConsumed = 0;
		int result = ParseDebugVariable( pFunction, stackPtr, strStackPtr, varStart, sIndex, subConsumed );
		if ( result > 0 ) return result;
		UINT iIndex = sIndex.ToInt();

		// check for strings that convert to index 0 but aren't valid indices
		if ( iIndex == 0 && sIndex.ByteAt(0) != '0' )
		{
			return 1;
		}

		// find end dimension bracket
		varStart += subConsumed;
		while( *varStart == ' ' ) varStart++;

		if ( *varStart == ']' ) varStart++;
		else if ( *varStart != ',' )
		{
			return 1;
		}

		// check index range
		if ( iIndex >= pArray->m_iLength )
		{
			return 1;
		}

		int type = pArray->GetCurrType();
		switch( type )
		{
			case AGK_DATA_TYPE_ARRAY:
			{
				consumed += (int)(varStart - szDimensions);
				return SetDebugArray( pFunction, stackPtr, strStackPtr, pArray->m_pA[ iIndex ], varStart, sNewValue, consumed );
			}
			case AGK_DATA_TYPE_TYPE:
			{
				consumed += (int)(varStart - szDimensions);
				return SetDebugType( pFunction, stackPtr, strStackPtr, pArray->m_pT[ iIndex ], varStart, sNewValue, consumed );
			}
			case AGK_DATA_TYPE_STRING:
			{
				pArray->m_pS[ iIndex ]->SetStr( sNewValue );
				consumed += (int)(varStart - szDimensions);
				return 0;
			}
			case AGK_DATA_TYPE_FLOAT:
			{
				pArray->m_pF[ iIndex ] = sNewValue.ToFloat();
				consumed += (int)(varStart - szDimensions);
				return 0;
			}
			case AGK_DATA_TYPE_INT:
			{
				pArray->m_pI[ iIndex ] = sNewValue.ToInt();
				consumed += (int)(varStart - szDimensions);
				return 0;
			}
			default:
			{
				return 1;
			}
		}
	}

	return 1;
}

void ProgramData::PrintWatchVariables( AGKVariableWatch *pTargetVar )
{
	if ( m_iBytecodeDebug == 0 ) return;

	// find current function
	AGKFunction *pFunction = 0;
	int iDebugFramePtr = m_iFramePtr;
	int iDebugStrFramePtr = m_iStrFramePtr;
	if ( m_iFramePtr >= 0 )
	{
		// if user has selected a different function from the call stack then traverse up the stack
		for( int i = 0; i < m_iDebugFrameDepth; i++ )
		{
			iDebugStrFramePtr = m_pStack[ iDebugFramePtr+2 ].i;
			iDebugFramePtr = m_pStack[ iDebugFramePtr+1 ].i;
			if ( iDebugFramePtr == -1 ) break;
		}

		if ( iDebugFramePtr >= 0 )
		{
			// grab return address of selected function and use this to find the function call command
			// extract the jump pointer from the function call command and use that to match against known functions
			int returnAddress = m_pStack[ iDebugFramePtr ].i;
			int functionPtr = m_pInstructions[ returnAddress-1 ].i;

			for ( UINT i = 0; i < m_iNumFunctions; i++ )
			{
				if ( m_pFunctions[ i ].iInstructionPtr == functionPtr )
				{
					pFunction = m_pFunctions + i;
					break;
				}
			}
		}
	}

	if ( pTargetVar )
	{
		if ( pTargetVar->sExpression.CompareTo( "@Version" ) == 0 )
		{
			uString sFinal;
			sFinal.Format( "@Version:%d", AGK_DEBUG_MESSAGE_VERSION );
			agk::DebugInfo( "Variable", sFinal.GetStr() );
		}
		else
		{
			uString sFinal, sValue;
			int consumed = 0;
			int error = ParseDebugVariable( pFunction, iDebugFramePtr, iDebugStrFramePtr, pTargetVar->sExpression, sValue, consumed );
			if ( error == 0 && consumed != pTargetVar->sExpression.GetLength() )
			{
				sValue.SetStrUTF8( "<Invalid Expression>" );
			}
			sValue.Replace( ':', 0x01 );
			sFinal.Format( "%s:%s", pTargetVar->sExpression.GetStr(), sValue.GetStr() );
			agk::DebugInfo( "Variable", sFinal.GetStr() );
		}
	}
	else
	{
		uString sValue;
		uString sFinal;
		AGKVariableWatch *pVar = m_pWatchVaraibles;
		while ( pVar )
		{
			if ( pVar->sExpression.CompareTo( "@Version" ) == 0 )
			{
				uString sFinal;
				sFinal.Format( "@Version:%d", AGK_DEBUG_MESSAGE_VERSION );
				agk::DebugInfo( "Variable", sFinal.GetStr() );
			}
			else
			{
				int consumed = 0;
				int error = ParseDebugVariable( pFunction, iDebugFramePtr, iDebugStrFramePtr, pVar->sExpression, sValue, consumed );
				if ( error == 0 && consumed != pVar->sExpression.GetLength() )
				{
					sValue.SetStrUTF8( "<Invalid Expression>" );
				}
				sValue.Replace( ':', 0x01 );
				sFinal.Format( "%s:%s", pVar->sExpression.GetStr(), sValue.GetStr() );
				agk::DebugInfo( "Variable", sFinal.GetStr() );
			}
			
			pVar = pVar->m_pNext;
		}
	}
}

void ProgramData::PrintCallStack()
{
	if ( m_iBytecodeDebug == 0 ) return;

	uString sFinal;
	int level = 0;
	int prevFuncCall = 0;
	int returnAddress = 0;
	int iDebugFramePtr = m_iFramePtr;
	int iDebugStrFramePtr = m_iStrFramePtr;
	while ( iDebugFramePtr >= 0 )
	{
		// grab return address of selected function and use this to find the function call command
		// extract the jump pointer from the function call command and use that to match against known functions
		returnAddress = m_pStack[ iDebugFramePtr ].i;
		int functionPtr = m_pInstructions[ returnAddress-1 ].i;
		
		AGKFunction *pFunction = 0;
		for ( UINT i = 0; i < m_iNumFunctions; i++ )
		{
			if ( m_pFunctions[ i ].iInstructionPtr == functionPtr )
			{
				pFunction = m_pFunctions + i;
				break;
			}
		}

		if ( pFunction )
		{
			if ( prevFuncCall <= 0 ) sFinal.Format( "%d:%s:%s:%d", level, pFunction->sName.GetStr(), GetCurrentIncludeFile(), GetCurrentLineNum() );
			else 
			{
				const char *szInclude = m_pIncludeFiles[ m_pInstructions[ prevFuncCall ].iIncludeFile ];
				sFinal.Format( "%d:%s:%s:%d", level, pFunction->sName.GetStr(), szInclude, m_pInstructions[ prevFuncCall ].iLineNum );
			}
			agk::DebugInfo( "Frame", sFinal );
		}
	
		iDebugStrFramePtr = m_pStack[ iDebugFramePtr+2 ].i;
		iDebugFramePtr = m_pStack[ iDebugFramePtr+1 ].i;
		level++;
		prevFuncCall = returnAddress - 1;
	}

	if ( returnAddress == 0 )
	{
		// no functions have been called
		sFinal.Format( "%d:<Main>:%s:%d", level, GetCurrentIncludeFile(), GetCurrentLineNum() );
		agk::DebugInfo( "Frame", sFinal );
	}
	else
	{
		// print root
		const char *szInclude = m_pIncludeFiles[ m_pInstructions[ returnAddress-1 ].iIncludeFile ];
		sFinal.Format( "%d:<Main>:%s:%d", level, szInclude, m_pInstructions[ returnAddress-1 ].iLineNum );
		agk::DebugInfo( "Frame", sFinal );
	}
}

void ProgramData::SetCallStackFrame( int iFrame )
{
	m_iDebugFrameDepth = iFrame;
}
