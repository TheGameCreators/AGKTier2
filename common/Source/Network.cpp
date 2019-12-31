#include "agk.h"

using namespace AGK;

//**********************
// File sender
//**********************

//#define NETWORK_DEBUG

cFileSender::cFileSender( UINT numFiles )
{
	m_kNameLock = new cLock();

	m_pFiles = 0;
	m_iSize = 0;

	m_pFiles = new uString[ numFiles ];
	m_iSize = numFiles;
	
	m_iAdded = 0;
	
	m_iCurrentFile = 0;
	m_iCurrentFileSize = 0;
	m_iTotalSending = 0;
	m_iTotalSent = 0;

	m_pConnection = 0;
	m_szIP[0] = '\0';
	m_iPort = 0;
	m_iTimeout = 3000;
	//m_bClose = true;
	m_bCloseAfter = true;

	m_pNext = 0;
}

cFileSender::~cFileSender()
{
	Stop();
	Join();
	if ( m_pConnection && m_bCloseAfter ) delete m_pConnection;
	if ( m_pFiles ) delete [] m_pFiles;

	if ( m_kNameLock ) delete m_kNameLock;
}

float cFileSender::GetProgress()
{
	if ( !IsRunning() || m_iTotalSending == 0 ) return 100;

	float progress = (m_iTotalSent*100.0f + m_iCurrentFileSize*m_pConnection->GetProgress()) / m_iTotalSending;
	if ( progress > 100 ) progress = 100;
	return progress;
}

int cFileSender::GetCurrentFile( uString &s )
{
	s.ClearTemp();
	if ( IsFinished() ) return 0;
	if ( m_iCurrentFile >= m_iAdded ) return 0;
	s.SetStr( m_pFiles[ m_iCurrentFile ] );
	return s.GetLength();
}

void cFileSender::Reset( UINT numFiles )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot reset the file sender whilst it is running, you must wait for IsFinished() to return true" );
#endif
		return;
	}

	if ( m_pFiles ) delete [] m_pFiles;
	m_iSize = 0;

	m_pFiles = new uString[ numFiles ];
	m_iSize = numFiles;
	
	m_iAdded = 0; 

	m_iCurrentFileSize = 0;
	m_iTotalSending = 0;
	m_iTotalSent = 0;

	if ( m_pConnection && m_bCloseAfter ) delete m_pConnection;
	m_pConnection = 0;
}

// does not connect immediately, waits for Start() before trying
bool cFileSender::ConnectTo( const char *szIP, UINT port, UINT timeout )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot connect the file sender whilst it is running, you must wait for IsFinished() to return true" );
#endif
		return false;
	}

	if ( !szIP || strlen(szIP) == 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to connect file sender, invalid IP address" );
#endif
		return false;
	}

	if ( m_pConnection && m_bCloseAfter ) delete m_pConnection;
	m_pConnection = 0;

	strcpy( m_szIP, szIP );
	m_iPort = port;
	m_iTimeout = timeout;
	//m_bClose = true;

	m_iCurrentFileSize = 0;
	m_iTotalSending = 0;
	m_iTotalSent = 0;

	return true;
}

void cFileSender::SetReceiver( AGKSocket *pReceiver, bool closeafter )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot change file sender receiver whilst it is running, you must wait for IsFinished() to return true" );
#endif
		return;
	}

	if ( !pReceiver )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to set receiver for file sender, invalid pointer" );
#endif
		return;
	}

	if ( m_pConnection && m_bCloseAfter ) delete m_pConnection;
	m_pConnection = pReceiver;
	//m_bClose = close;

	m_iCurrentFileSize = 0;
	m_iTotalSending = 0;
	m_iTotalSent = 0;

	m_bCloseAfter = closeafter;
}

bool cFileSender::AddFile( const char *szFilename )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot add file whilst file sender is running, you must wait for IsFinished() to return true" );
#endif
		return false;
	}

	if ( m_iAdded >= m_iSize )
	{
#ifdef _AGK_ERROR_CHECK
		// list full
		agk::Error( "Added too many files to file sender" );
#endif
		return false;
	}

	m_pFiles[ m_iAdded ].SetStr( szFilename );
	m_iAdded++;
	
	return true;
}

void cFileSender::Stop()
{
	AGKThread::Stop();
	if ( m_pConnection && m_bCloseAfter ) m_pConnection->Close( false ); // must close the connection to guarantee the thread will stop.
}

UINT cFileSender::Run()
{
	// make the progress bar show 0
	m_iTotalSending = 0;
	m_iTotalSent = 0;
	m_iTotalSending = 1;
	m_iCurrentFile = 0;
	
	if ( !m_pConnection )
	{
		m_pConnection = new AGKSocket();
		m_bCloseAfter = true;
		if ( !m_pConnection->Connect( m_szIP, m_iPort, m_iTimeout ) )
		{
			delete m_pConnection;
			m_bCloseAfter = true;
			m_pConnection = 0;
#ifdef _AGK_ERROR_CHECK
			agk::Warning( "Failed to connect file sender" );
#endif
			return 0;
		}

		if ( m_bTerminate ) return 0;
	}

	// send number of files
	if ( !m_pConnection->SendUInt( m_iAdded ) ) { return 0; }
	if ( m_bTerminate ) return 0;

	// send file names, modified dates, and sizes
	for ( UINT i = 0; i < m_iAdded; i++ )
	{
		UINT fileSize = cFile::GetFileSize( m_pFiles[ i ] );
		int modified = 0;
		cFile::GetModified( m_pFiles[ i ], modified );

		if ( !m_pConnection->SendUInt( i ) ) { return 0; }
		if ( !m_pConnection->SendString( m_pFiles[ i ] ) ) {  return 0; }
		if ( !m_pConnection->SendUInt( fileSize ) ) { return 0; }
		if ( !m_pConnection->SendInt( modified ) ) { return 0; }
	}

	m_pConnection->Flush();
	if ( m_bTerminate ) return 0;

	// wait for file request
	UINT numRequested =  m_pConnection->RecvUInt();
	if ( m_bTerminate ) return 0;
	if ( numRequested == 0 ) return 0;

	int *iIDs = new int[ numRequested ];
	UINT totalSize = 0;

	// get file IDs and calculate total size for progress
	for ( UINT i = 0; i < numRequested; i++ )
	{
		UINT ID = m_pConnection->RecvUInt();
		if ( m_bTerminate ) { delete [] iIDs; return 0; }

		if ( ID >= m_iAdded ) ID = 0;
		totalSize += cFile::GetFileSize( m_pFiles[ ID ] );
		iIDs[ i ] = ID;
	}

	m_iTotalSending = totalSize;
	m_iTotalSent = 0;
	cFile pFile;

	// send files
	for ( UINT i = 0; i < numRequested; i++ )
	{
		m_iCurrentFile = iIDs[ i ];
		m_iCurrentFileSize = cFile::GetFileSize( m_pFiles[ iIDs[i] ] );

		m_pConnection->SendString( m_pFiles[ iIDs[i] ] );
		if ( m_bTerminate ) { delete [] iIDs; return 0; }

		m_pConnection->SendFile( m_pFiles[ iIDs[i] ] );
		if ( m_bTerminate ) { delete [] iIDs; return 0; }
		m_iTotalSent += m_iCurrentFileSize;
	}

	delete [] iIDs;

	m_iCurrentFileSize = 0;
	
	m_pConnection->Flush();
	if ( m_bCloseAfter ) delete m_pConnection;
	m_pConnection = 0;
	
	return 0;
}

//*************************
// File broadcaster 
//*************************

cFileBroadcaster::cFileBroadcaster( UINT numFiles, const char *szGameName, UINT returnPort, UINT brodcastPort )
{
	m_kSenderLock = new cLock();
	m_iBrodcastPort = brodcastPort;
	m_iReturnPort = returnPort;
	m_pSenders = 0;

	AGKPacket packet;
	packet.AddString( szGameName );
	packet.AddUInt( returnPort );
	m_cBroadcaster.SetData( 0, m_iBrodcastPort, &packet, 1000 );
	m_cBroadcasterIPv6.SetData( 1, m_iBrodcastPort, &packet, 1000 );

	m_pFiles = 0;
	m_iNumFiles = 0;
	m_iAdded = 0;

	m_iCompleted = 0;

	if ( numFiles > 0 )
	{
		m_pFiles = new uString[ numFiles ];
		m_iNumFiles = numFiles;
	}
}

cFileBroadcaster::~cFileBroadcaster()
{
	Stop();
	Join();
	if ( m_pFiles ) delete [] m_pFiles;
	if ( m_kSenderLock ) delete m_kSenderLock;
}

// this count is not guaranteed to remain the same between calls to other functions in this class, 
// if using it to size arrays add a couple extra just to make sure any extra are covered.
UINT cFileBroadcaster::GetNumClients()
{
	if ( !m_pSenders ) return 0;

	cAutoLock autolock( m_kSenderLock );

	UINT count = 0;
	cFileSender *pSender = m_pSenders;
	while ( pSender )
	{
		count++;
		pSender = pSender->m_pNext;
	}

	return count;
}

// fills arrays with the latest data, each array must be at least max in size.
// if more senders than max exist they are ignored.
// returns the number of array elements filled.
int cFileBroadcaster::GetProgress( UINT max, uString *pDevices, uString *pCurrentFile, float *pProgress )
{
	cAutoLock autolock( m_kSenderLock );

	UINT iCurr = 0;
	cFileSender *pSender = m_pSenders;
	while ( pSender && iCurr < max )
	{
		pDevices[ iCurr ].SetStr( "Unknown" );
		if (pCurrentFile) pSender->GetCurrentFile( pCurrentFile[ iCurr ] );
		if (pProgress) pProgress[ iCurr ] = pSender->GetProgress();
		iCurr++;
		pSender = pSender->m_pNext;
	}

	return iCurr;
}

void cFileBroadcaster::Start()
{
	if ( m_iNumFiles == 0 ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to start file broadcaster, no files have been added" );
#endif
		return;
	}

	m_cBroadcaster.Start();
	m_cBroadcasterIPv6.Start();
	if ( !m_cListener.IsRunning() ) m_cListener.AcceptConnections( m_iReturnPort );

	AGKThread::Start();
}

// stops broadcasting and stops all transfers in progress.
void cFileBroadcaster::Stop()
{
	AGKThread::Stop();

	StopBroadcasting();
	if ( m_cListener.IsRunning() ) m_cListener.Stop();
	
	// get all current senders
	cFileSender *pSender = 0;
	if ( m_pSenders )
	{
		cAutoLock autolock( m_kSenderLock );
		pSender = m_pSenders;
		m_pSenders = 0; // to the outside this looks like all senders have been delete, so we can release the lock and delete later
	}
	
	// delete automatically stops
	while ( pSender )
	{
		cFileSender *pNext = pSender->m_pNext;
		delete pSender;
		pSender = pNext;
	}
}

void cFileBroadcaster::StopBroadcasting()
{
	if ( m_cBroadcaster.IsRunning() ) m_cBroadcaster.Stop();
	if ( m_cBroadcasterIPv6.IsRunning() ) m_cBroadcasterIPv6.Stop();
}

void cFileBroadcaster::ResumeBroadcasting()
{
	if ( !m_cBroadcaster.IsRunning() ) m_cBroadcaster.Start();
	if ( !m_cBroadcasterIPv6.IsRunning() ) m_cBroadcasterIPv6.Start();
}

bool cFileBroadcaster::AddFile( const char *szFilename )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot add file to file broadcaster once it has started" );
#endif
		return false;
	}

	if ( m_iAdded >= m_iNumFiles )
	{
#ifdef _AGK_ERROR_CHECK
		// list full
		agk::Error( "Added too many files to file broadcaster" );
#endif
		return false;
	}

	m_pFiles[ m_iAdded ].SetStr( szFilename );
	m_iAdded++;

	return true;
}

UINT cFileBroadcaster::Run()
{
	while ( !m_bTerminate )
	{
		AGKSocket *pAGKSocket = m_cListener.GetNewConnection();
		while ( pAGKSocket )
		{
#ifdef _AGK_ERROR_CHECK
			//agk::Warning( "Received connection, Sending files..." );
#endif

			cFileSender *sender = new cFileSender( m_iAdded );
			sender->SetReceiver( pAGKSocket );
			for ( UINT i = 0; i < m_iAdded; i++ )
			{
				sender->AddFile( m_pFiles[ i ] );
			}
			sender->Start();

			{
				// add it to the senders list
				cAutoLock autolock( m_kSenderLock ); // unlocks when deleted
				sender->m_pNext = m_pSenders;
				m_pSenders = sender;
			}
			
			pAGKSocket = m_cListener.GetNewConnection();
		}

		// check for finished senders
		if ( m_pSenders )
		{
			cAutoLock autolock( m_kSenderLock ); // unlocks when deleted
			cFileSender *pLast = 0;
			cFileSender *pSender = m_pSenders;
			cFileSender *pNext = 0;
			while ( pSender )
			{
				pNext = pSender->m_pNext;

				if ( pSender->IsFinished() )
				{
					// delete
					if ( pLast ) pLast->m_pNext = pSender->m_pNext;
					else m_pSenders = pSender->m_pNext;

					delete pSender;
					m_iCompleted++;
				}

				pSender = pNext;
			}
		}

		// suitably small number
		SleepSafe( 300 );
	}

	return 0;
}

//*************************
// File receiver 
//*************************

cFileReceiver::cFileReceiver( )
{
	m_pConnection = 0;
	
	m_iTotalReceiving = 0;
	m_iTotalReceived = 0;

	m_szIP[0] = '\0';
	m_iPort = 0;
	m_iTimeout = 3000;
	//m_bClose = true;
	m_bCloseAfter = true;
}

cFileReceiver::~cFileReceiver()
{
	Stop();
	Join();
	if ( m_pConnection && m_bCloseAfter ) delete m_pConnection;
	m_pConnection = 0;
}

void cFileReceiver::Stop()
{
	AGKThread::Stop();
	if ( m_pConnection && m_bCloseAfter ) m_pConnection->Close(false); // must close the connection to guarantee the thread will stop
}

float cFileReceiver::GetProgress()
{
	if ( !IsRunning() || m_iTotalReceiving == 0 ) return 100;

	float progress = (m_iTotalReceived*100.0f) / m_iTotalReceiving;
	if ( progress > 100 ) progress = 100;
	return progress;
}

// does not connect immediately, waits for Start() before trying
bool cFileReceiver::ConnectTo( const char *szIP, UINT port, UINT timeout )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot connect the file sender whilst it is running, you must wait for IsFinished() to return true" );
#endif
		return false;
	}

	if ( !szIP || strlen(szIP) == 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to connect file sender, invalid IP address" );
#endif
		return false;
	}

	if ( m_pConnection && m_bCloseAfter ) delete m_pConnection;
	m_pConnection = 0;

	strcpy( m_szIP, szIP );
	m_iPort = port;
	m_iTimeout = timeout;
	//m_bClose = true;

	m_iTotalReceiving = 0;
	m_iTotalReceived = 0;

	return true;
}

void cFileReceiver::SetSender( AGKSocket *pSender, bool closeafter )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot change file receiver sender whilst it is running, you must wait for IsFinished() to return true" );
#endif
		return;
	}

	if ( !pSender )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to set sender for file receiver, invalid pointer" );
#endif
		return;
	}

	if ( m_pConnection && m_bCloseAfter ) delete m_pConnection;
	m_pConnection = pSender;

	m_iTotalReceiving = 0;
	m_iTotalReceived = 0;
	m_bCloseAfter = closeafter;
}

void cFileReceiver::Reset( )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot reset the file receiver whilst it is running, you must wait for IsFinished() to return true" );
#endif
		return;
	}

	m_iTotalReceiving = 0;
	m_iTotalReceived = 0;

	if ( m_pConnection && m_bCloseAfter ) delete m_pConnection;
	m_pConnection = 0;
}

UINT cFileReceiver::Run()
{
	m_bSuccess = true;
	
	// make the progress bar show 0
	m_iTotalReceived = 0;
	m_iTotalReceiving = 1;

	if ( !m_pConnection )
	{
		m_pConnection = new AGKSocket();
		m_bCloseAfter = true;
		if ( !m_pConnection->Connect( m_szIP, m_iPort, m_iTimeout ) )
		{
			delete m_pConnection;
			m_pConnection = 0;
#ifdef _AGK_ERROR_CHECK
			agk::Warning( "Failed to connect file receiver" );
#endif
			m_bSuccess = false;
			return 0;
		}

		if ( m_bTerminate ) return 0;
	}

	// get number of files advertised
	UINT numFiles = m_pConnection->RecvUInt();
	if ( numFiles == 0 ) return 0;

	int *iIDs = new int[ numFiles ];
	int *iModifiedTimes = new int[ numFiles ];
	int iRequested = 0;
	UINT iTotalSize = 0;
	
	// get file ID, name, size, date modified
	for ( UINT i = 0; i < numFiles; i++ )
	{
		UINT ID = m_pConnection->RecvUInt();
		if ( m_bTerminate ) { delete [] iIDs; delete [] iModifiedTimes; m_bSuccess = false; return 0; }

		uString filename;
		m_pConnection->RecvString( filename );
		if ( m_bTerminate ) { delete [] iIDs; delete [] iModifiedTimes; m_bSuccess = false; return 0; }
		
		UINT filesize = m_pConnection->RecvUInt();
		if ( m_bTerminate ) { delete [] iIDs; delete [] iModifiedTimes; m_bSuccess = false; return 0; }
		
		int modified = m_pConnection->RecvInt();
		if ( m_bTerminate ) { delete [] iIDs; delete [] iModifiedTimes; m_bSuccess = false; return 0; }

		if ( filename.GetLength() == 0 ) continue;
		if ( filesize == 0 ) continue;

		// check if file is needed
		bool needed = true;
		if ( filename.CompareToN( "/Plugins/", (int)strlen("/Plugins/") ) == 0 )
		{
#ifdef AGK_WINDOWS
	#ifdef _WIN64
			if ( filename.FindStr( "Windows64.dll" ) < 0 ) needed = false;
	#else
			if ( filename.FindStr( "Windows.dll" ) < 0 ) needed = false;
	#endif
#endif
#ifdef AGK_LINUX
			if ( filename.FindStr( "Linux64.so" ) < 0 ) needed = false;
#endif
#ifdef AGK_MACOS
			if ( filename.FindStr( "Mac64.dylib" ) < 0 ) needed = false;
#endif
#ifdef AGK_ANDROID
			needed = false;
#endif
#ifdef AGK_IOS
			needed = false;
#endif
		}
		
		if ( needed )
		{
			if ( cFile::Exists( filename ) ) 
			{
				int modified2 = 0;
				cFile::GetModified( filename, modified2 );
				if ( cFile::GetFileSize( filename ) == filesize && modified <= modified2 ) 
				{
					needed = false;
				}
			}
		}
		
		// request the file if needed
		if ( needed )
		{
			// store this file
			iIDs[ iRequested ] = ID;
			iModifiedTimes[ iRequested ] = modified;
			iRequested++;
			iTotalSize += filesize;
		}
	}
	
	// send num of requested files 
	m_pConnection->SendUInt( iRequested );

	if ( iRequested == 0 )
	{
		delete [] iIDs; 
		delete [] iModifiedTimes;
		return 0;
	}

	m_iTotalReceiving = iTotalSize;

	// send requested IDs
	for ( int i = 0; i < iRequested; i++ )
	{
		m_pConnection->SendUInt( iIDs[i] );
	}
	m_pConnection->Flush();

	delete [] iIDs;

	for ( int i = 0; i < iRequested; i++ )
	{
		uString sFilename;
		int result = m_pConnection->RecvString( sFilename );
		if ( m_bTerminate ) { m_bSuccess = false; delete [] iModifiedTimes; return 0; }
		if ( result <= 0 ) { m_bSuccess = false; break; }
		
		cFile pFile;
		pFile.OpenToWrite( sFilename );

		// file size
		UINT size = m_pConnection->RecvUInt();
		if ( size == 0 ) 
		{
			pFile.Close();
			continue;
		}

		// file contents
		int remaining = size;
		while ( remaining >= 1500 )
		{
			result = m_pConnection->RecvData( m_buffer, 1500 );
			if ( m_bTerminate ) { m_bSuccess = false; delete [] iModifiedTimes; return 0; }
			if ( result <= 0 ) { m_bSuccess = false; break; }
			remaining -= result;
			pFile.WriteData( m_buffer, result );
			
			m_iTotalReceived += result;
		}

		if ( result > 0 )
		{
			while ( remaining > 0 )
			{
				result = m_pConnection->RecvData( m_buffer, remaining );
				if ( m_bTerminate ) { m_bSuccess = false; delete [] iModifiedTimes; return 0; }
				if ( result <= 0 ) { m_bSuccess = false; break; }
				remaining -= result;
				pFile.WriteData( m_buffer, result );

				m_iTotalReceived += result;
			}
		}

		pFile.Close();

		// change modified time to match the sending machine
		cFile::SetModified( sFilename, iModifiedTimes[ i ] );

		if ( result <= 0 ) break;
	}

	delete [] iModifiedTimes;

	if ( m_pConnection && m_bCloseAfter ) delete m_pConnection;
	m_pConnection = 0;
	
	return 0;
}


//*********************
// AGKPacket
//*********************

AGKPacket::AGKPacket()
{
	m_iPtr = 0;
}

void AGKPacket::Copy( const AGKPacket *fromPacket )
{
	if ( fromPacket->GetPos() > 0 )
	{
		memcpy( m_Buffer, fromPacket->GetBuffer(), fromPacket->GetPos() );
	}

	m_iPtr = fromPacket->GetPos();
}

void AGKPacket::AddData( const char* s, UINT length )
{
	if ( !s || length == 0 ) return;
	if ( m_iPtr+length > AGK_NET_PACKET_SIZE )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to add data to packet, amount of total data exceeds the maximum packet size of 1400" );
#endif
		return;
	}

	memcpy( m_Buffer+m_iPtr, s, length );
	m_iPtr += length;
}

void AGKPacket::AddString( const char *s )
{
	UINT length = (UINT)strlen( s );
	if ( m_iPtr+length+4 > AGK_NET_PACKET_SIZE )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to add string to packet, amount of total data exceeds the maximum packet size of 1400" );
#endif
		return;
	}

	AddUInt( length );
	AddData( s, length );
}

void AGKPacket::AddChar( char c )
{
	if ( m_iPtr+1 > AGK_NET_PACKET_SIZE )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to add char to packet, amount of total data exceeds the maximum packet size of 1400" );
#endif
		return;
	}

	m_Buffer[m_iPtr] = c;
	m_iPtr += 1;
}

void AGKPacket::AddUInt( UINT u )
{
	if ( m_iPtr+4 > AGK_NET_PACKET_SIZE )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to add int to packet, amount of total data exceeds the maximum packet size of 1400" );
#endif
		return;
	}

	memcpy( m_Buffer + m_iPtr, &u, 4 );
	m_iPtr += 4;
}

void AGKPacket::AddInt( int i )
{
	if ( m_iPtr+4 > AGK_NET_PACKET_SIZE )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to add int to packet, amount of total data exceeds the maximum packet size of 1400" );
#endif
		return;
	}

	memcpy( m_Buffer + m_iPtr, &i, 4 );
	m_iPtr += 4;
}

void AGKPacket::AddFloat( float f )
{
	if ( m_iPtr+4 > AGK_NET_PACKET_SIZE )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to add int to packet, amount of total data exceeds the maximum packet size of 1400" );
#endif
		return;
	}

	memcpy( m_Buffer + m_iPtr, &f, 4 );
	m_iPtr += 4;
}

UINT AGKPacket::GetPos() const
{
	return m_iPtr;
}

void AGKPacket::SetPos( UINT pos )
{
	if ( pos > AGK_NET_PACKET_SIZE )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to set packet pointer position, value too large" );
#endif
		return;
	}

	m_iPtr = pos;
}

UINT AGKPacket::GetData( char* data, UINT length )
{
	if ( !data || length == 0 ) return 0;
	if ( m_iPtr >= AGK_NET_PACKET_SIZE ) return 0;
	if ( length > AGK_NET_PACKET_SIZE-m_iPtr ) length = AGK_NET_PACKET_SIZE-m_iPtr;
	memcpy( data, m_Buffer+m_iPtr, length );
	m_iPtr+= length;
	return length;
}

int AGKPacket::GetString( uString &s )
{
	s.ClearTemp();
	UINT length = GetUInt();
	if ( length == 0 ) return 0;
	if ( m_iPtr >= AGK_NET_PACKET_SIZE ) return 0;
	if ( length > AGK_NET_PACKET_SIZE-m_iPtr ) length = AGK_NET_PACKET_SIZE-m_iPtr;
	s.AppendN( m_Buffer+m_iPtr, length );
	m_iPtr += length;
	return length;
}

char AGKPacket::GetChar()
{
	if ( m_iPtr+1 > AGK_NET_PACKET_SIZE ) return 0;
	char c = m_Buffer[m_iPtr];
	m_iPtr++;
	return c;
}

int AGKPacket::GetInt()
{
	if ( m_iPtr+4 > AGK_NET_PACKET_SIZE ) return 0;
	int i;
	memcpy( &i, m_Buffer + m_iPtr, 4 );
	m_iPtr += 4;
	return i;
}

UINT AGKPacket::GetUInt()
{
	if ( m_iPtr+4 > AGK_NET_PACKET_SIZE ) return 0;
	UINT u; 
	memcpy( &u, m_Buffer + m_iPtr, 4 );
	m_iPtr += 4;
	return u;
}

float AGKPacket::GetFloat()
{
	if ( m_iPtr+4 > AGK_NET_PACKET_SIZE ) return 0;
	float f;
	memcpy( &f, m_Buffer + m_iPtr, 4 );
	m_iPtr += 4;
	return f;
}

//**********************
// Network client
//**********************

cNetworkClient::cNetworkClient() : m_pHashedVars(64)
{
	m_iID = 0;
	m_sName.SetStr( "" );
	m_pVars = new cNetworkVariable*[ 10 ];
	m_iNumVars = 0;
	m_iArraySize = 10;
	m_kVarLock = new cLock();
	m_bKick = false;

	m_fPingStart = 0;
	m_iPingResponded = 1;
	m_fPing = 0;
	m_bDisconnected = false;
	m_bDelete = false;
	
	for ( UINT i = 0; i < 5; i++ ) m_iUserValue[ i ] = 0;
}

cNetworkClient::~cNetworkClient()
{
	m_kVarLock->Acquire();
	m_pHashedVars.ClearAll();
	if ( m_pVars ) 
	{
		for ( UINT i = 0; i < m_iNumVars; i++ )
		{
			if ( m_pVars[ i ] ) delete m_pVars[ i ];
		}

		delete [] m_pVars;
	}
	m_kVarLock->Release();

	delete m_kVarLock;
}

int cNetworkClient::GetVariableI( const char *name, bool bDontReset )
{
	cAutoLock autolock( m_kVarLock );
	cNetworkVariable *pVar = m_pHashedVars.GetItem( name );
	if ( !pVar ) return 0;
	if ( pVar->m_iType != 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Tried to get an int value on a network variable that is not an int" );
#endif
		return 0;
	}

	if ( pVar->m_bReset ) return 0;
	if ( pVar->m_iMode == 1 ) pVar->m_bReset = true;
	return pVar->m_value.i;
}

float cNetworkClient::GetVariableF( const char *name, bool bDontReset )
{
	cAutoLock autolock( m_kVarLock );
	cNetworkVariable *pVar = m_pHashedVars.GetItem( name );
	if ( !pVar ) return 0;
	if ( pVar->m_iType != 1 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Tried to get a float value on a network variable that is not a float" );
#endif
		return 0;
	}

	if ( pVar->m_bReset ) return 0;
	if ( pVar->m_iMode == 1 ) pVar->m_bReset = true;
	return pVar->m_value.f;
}

UINT cNetworkClient::GetVariableType( const char *name )
{
	cAutoLock autolock( m_kVarLock );
	cNetworkVariable *pVar = m_pHashedVars.GetItem( name );
	if ( !pVar ) return 0;

	return pVar->m_iType;
}

UINT cNetworkClient::GetVariableExists( const char *name )
{
	cAutoLock autolock( m_kVarLock );
	cNetworkVariable *pVar = m_pHashedVars.GetItem( name );
	if ( !pVar ) return 0;
	return 1;
}

/*
int cNetworkClient::GetVariableI( UINT index )
{
	cAutoLock autolock( m_kVarLock );
	if ( index > m_iNumVars ) return 0;
	if ( pVar->m_iType != 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Tried to get an int value on a network variable that is not an int" );
#endif
		return 0;
	}

	return pVars[ index ]->m_value.i;
}

float cNetworkClient::GetVariableF( UINT index )
{
	return 0;
}
*/

UINT cNetworkClient::GetVariableType( UINT index )
{
	cAutoLock autolock( m_kVarLock );
	if ( index >= m_iNumVars ) return 0;
	return m_pVars[ index ]->m_iType;
}

void cNetworkClient::SetVariableI( const char *name, int i, int mode )
{
	cAutoLock autolock( m_kVarLock );
	cNetworkVariable *pVar = m_pHashedVars.GetItem( name );
	if ( pVar )
	{
		if ( pVar->m_iType != 0 )
		{
#ifdef _AGK_ERROR_CHECK
			agk::Error( "Tried to set an int value on a network variable that is not an int" );
#endif
			return;
		}

		if ( pVar->m_value.i != i || pVar->m_iMode == 1 ) pVar->m_bChanged = true;
		pVar->m_bReset = false;
		pVar->m_value.i = i;
	}
	else
	{
		// create new variable
		cNetworkVariable *pNewVar = new cNetworkVariable();
		pNewVar->m_bNew = true;
		pNewVar->m_bChanged = false;
		pNewVar->m_iMode = mode;
		pNewVar->m_iType = 0;
		pNewVar->m_sName.SetStr( name );
		pNewVar->m_value.i = i;
		pNewVar->m_bReset = false;

		// store new variable
		if ( m_iNumVars >= m_iArraySize )
		{
			// resize array
			cNetworkVariable **pNewArray = new cNetworkVariable*[ m_iArraySize + 10 ];
			if ( m_pVars ) 
			{
				for ( UINT j = 0; j < m_iNumVars; j++ )
				{
					pNewArray[ j ] = m_pVars[ j ];
				}

				delete [] m_pVars;
			}
			m_pVars = pNewArray;

			m_iArraySize += 10;
		}

		UINT ref = m_iNumVars;
		m_pVars[ ref ] = pNewVar;
		m_pHashedVars.AddItem( pNewVar, name );
		m_iNumVars++;
	}
}

void cNetworkClient::SetVariableF( const char *name, float f, int mode )
{
	cAutoLock autolock( m_kVarLock );
	cNetworkVariable *pVar = m_pHashedVars.GetItem( name );
	if ( pVar )
	{
		if ( pVar->m_iType != 1 )
		{
#ifdef _AGK_ERROR_CHECK
			agk::Error( "Tried to set a float value on a network variable that is not a float" );
#endif
			return;
		}

		if ( pVar->m_value.f != f || pVar->m_iMode == 1 ) pVar->m_bChanged = true;
		pVar->m_bReset = false;
		pVar->m_value.f = f;
	}
	else
	{
		// create new variable
		cNetworkVariable *pNewVar = new cNetworkVariable();
		pNewVar->m_bNew = true;
		pNewVar->m_bChanged = false;
		pNewVar->m_iMode = mode;
		pNewVar->m_iType = 1;
		pNewVar->m_sName.SetStr( name );
		pNewVar->m_value.f = f;
		pNewVar->m_bReset = false;

		// store new variable
		if ( m_iNumVars >= m_iArraySize )
		{
			// resize array
			cNetworkVariable **pNewArray = new cNetworkVariable*[ m_iArraySize + 10 ];
			if ( m_pVars ) 
			{
				for ( UINT j = 0; j < m_iNumVars; j++ )
				{
					pNewArray[ j ] = m_pVars[ j ];
				}

				delete [] m_pVars;
			}
			m_pVars = pNewArray;

			m_iArraySize += 10;
		}

		UINT ref = m_iNumVars;
		m_pVars[ ref ] = pNewVar;
		m_pHashedVars.AddItem( pNewVar, name );
		m_iNumVars++;
	}
}

void cNetworkClient::SetVariableI( UINT index, int i, int mode )
{
	if ( index >= m_iNumVars )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Got an int variable update for a non-existant variable" );
#endif
		return;
	}

	bool bFailed = false;

	{
		cAutoLock autolock( m_kVarLock );

		if ( m_pVars[ index ]->m_iType != 0 )
		{
			bFailed = true;
		}
		else
		{
			if ( m_pVars[ index ]->m_value.i != i || m_pVars[ index ]->m_iMode == 1 ) m_pVars[ index ]->m_bChanged = true;
			m_pVars[ index ]->m_value.i = i;
			m_pVars[ index ]->m_bReset = false;
		}
	}

	if ( bFailed ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Tried to change an int value on a network variable that is not an int" );
#endif
	}
}

void cNetworkClient::SetVariableF( UINT index, float f, int mode )
{
	if ( index >= m_iNumVars )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Got a float variable update for a non-existant variable" );
#endif
		return;
	}

	bool bFailed = false;
	{
		cAutoLock autolock( m_kVarLock );
		
		if ( m_pVars[ index ]->m_iType != 1 )
		{
			bFailed = true;
		}
		else
		{
			if ( m_pVars[ index ]->m_value.f != f || m_pVars[ index ]->m_iMode == 1 ) m_pVars[ index ]->m_bChanged = true;
			m_pVars[ index ]->m_value.f = f;
			m_pVars[ index ]->m_bReset = false;
		}
	}

	if ( bFailed ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Tried to change a float value on a network variable that is not a float" );
#endif
	}
}

//**********************
// Network Message
//**********************

void cNetworkMessage::CopyMessage( cNetworkMessage *pOther )
{
	int max = pOther->GetPos();
	if ( pOther->m_iSize > max ) max = pOther->m_iSize;
	if ( max > 0 )
	{
		memcpy( m_Buffer, pOther->GetBuffer(), max );
	}

	m_iSize = max;
	m_iPtr = m_iSize;
	m_iFromClientID = 0;
	m_iToClientID = 0;
}

//**********************
// Network
//**********************

cNetwork::cNetwork() : m_cClientRef(32)
{
	m_iServerPort = 0;
	m_iHostPort = 0;
	m_iHostPortv6 = 0;
	strcpy( m_szServerIP, "" );

	m_pBroadcaster = 0;
	m_pBroadcasterv6 = 0;
	m_pBroadcastListener = 0;
	m_pBroadcastListenerv6 = 0;
	m_pListener = 0;
	m_pListenerv6 = 0;
	m_pServerSock = 0;
	m_ppClientSock = 0;
	m_ppClients = 0;
	m_iNumClients = 0;
	m_iArraySize = 0;
	m_bServer = false;
	m_iNextClientID = 1;
	m_iMyClientID = 0;
	m_kClientLock = new cLock();
	m_iClientIter = 0xffffffff;
	m_iLatency = 15;
	m_fLastKeepAlive = 0;

	m_kCleanUpLock = new cLock();

	m_kMessageLock = new cLock();
	m_pSendMessages = 0;
	m_pSendTail = 0;
	m_pRecvMessages = 0;
	m_pRecvTail = 0;
}

cNetwork::~cNetwork()
{
	if ( m_iClientIter != 0xffffffff )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Warning( "Closed network before completing GetNextClient loop, could cause a crash" );
#endif
		m_kClientLock->Release();
		m_iClientIter = 0xffffffff;
	}

	CloseNetwork();
	//Stop();

	Join();

	if ( m_pBroadcaster ) delete m_pBroadcaster;
	if ( m_pBroadcasterv6 ) delete m_pBroadcasterv6;
	if ( m_pBroadcastListener ) delete m_pBroadcastListener;
	if ( m_pBroadcastListenerv6 ) delete m_pBroadcastListenerv6;
	if ( m_pListener ) delete m_pListener;
	if ( m_pListenerv6 ) delete m_pListenerv6;
	if ( m_pServerSock ) delete m_pServerSock;

	if ( m_ppClientSock ) 
	{
		for ( UINT i = 0; i < m_iNumClients; i++ )
		{
			if ( m_ppClientSock[ i ] ) delete m_ppClientSock[ i ];
		}

		delete [] m_ppClientSock;
	}

	if ( m_ppClients ) 
	{
		for ( UINT i = 0; i < m_iNumClients; i++ )
		{
			if ( m_ppClients[ i ] ) delete m_ppClients[ i ];
		}

		delete [] m_ppClients;
	}

	if ( m_kClientLock ) delete m_kClientLock;
	if ( m_kMessageLock ) delete m_kMessageLock;
	if ( m_kCleanUpLock ) delete m_kCleanUpLock;

	while ( m_pSendMessages )
	{
		cNetworkMessage *pMsg = m_pSendMessages;
		m_pSendMessages = m_pSendMessages->m_pNext;
		delete pMsg;
	}

	while ( m_pRecvMessages )
	{
		cNetworkMessage *pMsg = m_pRecvMessages;
		m_pRecvMessages = m_pRecvMessages->m_pNext;
		delete pMsg;
	}

	m_pBroadcaster = 0;
	m_pBroadcasterv6 = 0;
	m_pBroadcastListener = 0;
	m_pBroadcastListenerv6 = 0;
	m_pListener = 0;
	m_pListenerv6 = 0;
	m_pServerSock = 0;
	m_ppClientSock = 0;
	m_ppClients = 0;
	m_iNumClients = 0;
	m_iArraySize = 0;
	m_bServer = false;
	m_iMyClientID = 0;
	m_iLatency = 15;
	m_fLastKeepAlive = 0;

	m_kMessageLock = 0;
	m_pSendMessages = 0;
	m_pSendTail = 0;
	m_pRecvMessages = 0;
	m_pRecvTail = 0;
}

void cNetwork::Reset()
{
	Stop();
	Join();

	m_sNetworkName.SetStr( "" );
	m_sMyName.SetStr( "" );
	strcpy( m_szServerIP, "" );
	m_iServerPort = 0;
	m_iHostPort = 0;
	m_iHostPortv6 = 0;

	if ( m_pBroadcaster ) delete m_pBroadcaster;
	if ( m_pBroadcasterv6 ) delete m_pBroadcasterv6;
	if ( m_pBroadcastListener ) delete m_pBroadcastListener;
	if ( m_pBroadcastListenerv6 ) delete m_pBroadcastListenerv6;
	if ( m_pListener ) delete m_pListener;
	if ( m_pListenerv6 ) delete m_pListenerv6;
	if ( m_pServerSock ) delete m_pServerSock;

	if ( m_ppClientSock ) 
	{
		for ( UINT i = 0; i < m_iNumClients; i++ )
		{
			if ( m_ppClientSock[ i ] ) delete m_ppClientSock[ i ];
		}

		delete [] m_ppClientSock;
	}

	if ( m_ppClients ) 
	{
		for ( UINT i = 0; i < m_iNumClients; i++ )
		{
			if ( m_ppClients[ i ] ) delete m_ppClients[ i ];
		}

		delete [] m_ppClients;
	}

	m_pBroadcaster = 0;
	m_pBroadcasterv6 = 0;
	m_pBroadcastListener = 0;
	m_pBroadcastListenerv6 = 0;
	m_pListener = 0;
	m_pListenerv6 = 0;
	m_pServerSock = 0;
	m_ppClientSock = 0;
	m_ppClients = 0;
	m_iNumClients = 0;
	m_iArraySize = 0;
	m_bServer = false;
	m_iMyClientID = 0;
	m_iLatency = 15;
	m_fLastKeepAlive = 0;
	m_iKeepAliveResponded = 1;

	if ( m_iClientIter != 0xffffffff )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Warning( "Reseting network before completing GetNextClient loop could cause a crash" );
#endif
		m_kClientLock->Release();
		m_iClientIter = 0xffffffff;
	}

	while ( m_pSendMessages )
	{
		cNetworkMessage *pMsg = m_pSendMessages;
		m_pSendMessages = m_pSendMessages->m_pNext;
		delete pMsg;
	}

	while ( m_pRecvMessages )
	{
		cNetworkMessage *pMsg = m_pRecvMessages;
		m_pRecvMessages = m_pRecvMessages->m_pNext;
		delete pMsg;
	}

	m_pRecvMessages = 0;
	m_pSendMessages = 0;
	m_pSendTail = 0;
	m_pRecvTail = 0;
}

void cNetwork::NewClient( AGKSocket *pNew )
{
	bool bValid = true;
	cNetworkClient *pNewClient = new cNetworkClient();
	pNew->RecvString( pNewClient->m_sName );

#ifdef _AGK_ERROR_CHECK
#ifdef NETWORK_DEBUG
	uString msg;
	msg.Format( "Received new client connection: %s", pNewClient->m_sName.GetStr() );
	agk::Warning( msg );
#endif
#endif

	// check unique name
	/*
	bool bUnique = true;
	for ( UINT i = 0; i < m_iNumClients; i++ )
	{
		if ( m_ppClients[ i ]->m_sName.CompareTo( pNewClient->m_sName ) == 0 && !m_ppClients[ i ]->m_bDisconnected ) 
		{
			bUnique = false;
			break;
		}
	}

	if ( !bUnique )
	{
		pNew->SendUInt( 0 ); //failure
		pNew->Flush();
		delete pNewClient;
		delete pNew;
		return;
	}
	*/
	
	pNewClient->m_iID = m_iNextClientID++;
	pNew->SendUInt( 1 ); //success
	pNew->SendUInt( pNewClient->m_iID ); // send client its unique ID
	pNew->Flush();

	// send new client details of all existing clients
	pNew->SendUInt( m_iNumClients );
	for ( UINT i = 0; i < m_iNumClients; i++ )
	{
		pNew->SendUInt( m_ppClients[ i ]->m_iID );
		pNew->SendString( m_ppClients[ i ]->m_sName );

		cAutoLock autolock( m_ppClients[ i ]->GetVarLock() ); // unlocks when destroyed
		UINT numVars = m_ppClients[ i ]->GetNumVars();
		cNetworkVariable **ppVars = m_ppClients[ i ]->GetVars();

		// send all variables for this client to new client
		pNew->SendUInt( numVars );
		for ( UINT v = 0; v < numVars; v++ )
		{
			pNew->SendString( ppVars[ v ]->m_sName.GetStr() );
			pNew->SendUInt( ppVars[ v ]->m_iType );
			pNew->SendUInt( ppVars[ v ]->m_iMode );

			if ( ppVars[ v ]->m_iType == 0 ) pNew->SendInt( ppVars[ v ]->m_value.i );
			else pNew->SendFloat( ppVars[ v ]->m_value.f );

			if ( pNew->GetDisconnected() ) 
			{
				bValid = false;
				break;
			}
		}

		if ( pNew->GetDisconnected() ) 
		{
			bValid = false;
			break;
		}
	}

	pNew->Flush();
	if ( pNew->GetDisconnected() ) 
	{
		bValid = false;
	}

	// did client disconnect?
	if ( !bValid ) 
	{
		delete pNewClient;
		delete pNew;
		return;
	}

	{
		cAutoLock autolock( m_kClientLock );
		
		if ( !m_ppClients || m_iNumClients >= m_iArraySize )
		{
			// resize array
			cNetworkClient **pNewArray = new cNetworkClient*[ m_iArraySize + 10 ];
			for ( UINT i = 0; i < m_iNumClients; i++ )
			{
				pNewArray[ i ] = m_ppClients[ i ];
			}

			AGKSocket **pNewSocketArray = new AGKSocket*[ m_iArraySize + 10 ];
			for ( UINT i = 0; i < m_iNumClients; i++ )
			{
				pNewSocketArray[ i ] = m_ppClientSock[ i ];
			}

			cAutoLock autolock( m_kClientLock );

			if ( m_ppClients ) delete [] m_ppClients;
			m_ppClients = pNewArray;

			if ( m_ppClientSock ) delete [] m_ppClientSock;
			m_ppClientSock = pNewSocketArray;

			m_iArraySize += 10;
		}
		
		// add new client
		UINT ref = m_iNumClients;
		m_ppClients[ ref ] = pNewClient;
		m_ppClientSock[ ref ] = pNew;
		UINT *index = new UINT;
		*index = ref;
		m_cClientRef.AddItem( index, pNewClient->m_iID );
		m_iNumClients++;
	}

	// send new client to existing clients
	for ( UINT i = 1; i < m_iNumClients; i++ )
	{
		if ( m_ppClients[ i ]->m_iID == m_iMyClientID ) continue;
		if ( m_ppClients[ i ] == pNewClient ) continue;
		m_ppClientSock[ i ]->SendUInt( 1 ); // 1 for new client
		m_ppClientSock[ i ]->SendUInt( pNewClient->m_iID );
		m_ppClientSock[ i ]->SendString( pNewClient->m_sName.GetStr() );
	}
}

void cNetwork::CheckMessagesServer()
{
	// check for new messages
	for ( UINT i = 1; i < m_iNumClients; i++ )
	{
		while ( m_ppClientSock[ i ]->GetBytes() >= 4 )
		{
			UINT type = m_ppClientSock[ i ]->RecvUInt();
			if ( m_ppClientSock[ i ]->GetDisconnected() ) continue;
			switch( type )
			{
				case 0: // ping response
				{
					m_ppClients[ i ]->m_fPing = agk::GetRunTime() - m_ppClients[ i ]->m_fPingStart;
					m_ppClients[ i ]->m_iPingResponded = 1;
					break;
				}

				case 2: // new variables
				{
#ifdef _AGK_ERROR_CHECK
#ifdef NETWORK_DEBUG
					uString msg;
					msg.Format( "Server recevied new variables from: %s, %dms", m_ppClients[ i ]->m_sName.GetStr(), agk::Round(agk::GetRunTime()*1000) );
					agk::Warning( msg );
#endif
#endif
					UINT numVars = m_ppClientSock[ i ]->RecvUInt();
					for ( UINT v = 0; v < numVars; v++ )
					{
						uString varname;
						m_ppClientSock[ i ]->RecvString( varname );
						UINT mode = m_ppClientSock[ i ]->RecvUInt();
						UINT vartype = m_ppClientSock[ i ]->RecvUInt();
						if ( m_ppClientSock[ i ]->GetDisconnected() ) break;
						if ( vartype == 0 ) m_ppClients[ i ]->SetVariableI( varname, m_ppClientSock[ i ]->RecvInt(), mode );
						else m_ppClients[ i ]->SetVariableF( varname, m_ppClientSock[ i ]->RecvFloat(), mode );
					}
					break;
				}

				case 3: // changed variables
				{
#ifdef _AGK_ERROR_CHECK
#ifdef NETWORK_DEBUG
					uString msg;
					msg.Format( "Server recevied changed variables from: %s, %dms", m_ppClients[ i ]->m_sName.GetStr(), agk::Round(agk::GetRunTime()*1000) );
					agk::Warning( msg );
#endif
#endif
					UINT numVars = m_ppClientSock[ i ]->RecvUInt();
					for ( UINT v = 0; v < numVars; v++ )
					{
						UINT ID = m_ppClientSock[ i ]->RecvUInt();
						if ( m_ppClientSock[ i ]->GetDisconnected() ) break;
						UINT vartype = m_ppClients[ i ]->GetVariableType( ID );
						if ( vartype == 0 ) m_ppClients[ i ]->SetVariableI( ID, m_ppClientSock[ i ]->RecvInt() );
						else m_ppClients[ i ]->SetVariableF( ID, m_ppClientSock[ i ]->RecvFloat() );
					}
					break;
				}

				case 5: // network message
				{
					UINT from = m_ppClientSock[ i ]->RecvUInt();
					UINT to = m_ppClientSock[ i ]->RecvUInt();
					UINT size = m_ppClientSock[ i ]->RecvUInt();
					if ( size > 0 && size <= AGK_NET_PACKET_SIZE )
					{
						cNetworkMessage *pMsg = new cNetworkMessage();
						pMsg->m_iFromClientID = from;
						pMsg->m_iToClientID = to;
						UINT written = m_ppClientSock[ i ]->RecvData(pMsg->GetRaw(), size);
						UINT total = written;
						while ( written > 0 && total < size )
						{
							written = m_ppClientSock[ i ]->RecvData(pMsg->GetRaw()+total, size-total);
							total += written;
						}
						pMsg->SetPos( size );
						pMsg->SetSize( size );
						
						cAutoLock autolock2( m_kMessageLock );

						if ( to == m_iMyClientID )
						{
							// sent to server
							pMsg->m_pNext = 0;
							if ( m_pRecvTail ) m_pRecvTail->m_pNext = pMsg;
							else m_pRecvMessages = pMsg;
							m_pRecvTail = pMsg;
							pMsg->SetPos( 0 );
						}
						else if ( to == 0 )
						{
							// sent to all
							pMsg->m_pNext = 0;
							if ( m_pRecvTail ) m_pRecvTail->m_pNext = pMsg;
							else m_pRecvMessages = pMsg;
							m_pRecvTail = pMsg;

							pMsg = new cNetworkMessage();
							pMsg->m_iFromClientID = from;
							pMsg->m_iToClientID = to;
							pMsg->Copy( m_pRecvTail );
							pMsg->SetPos( size );
							pMsg->SetSize( size );

							m_pRecvTail->SetPos( 0 );

							pMsg->m_pNext = 0;
							if ( m_pSendTail ) m_pSendTail->m_pNext = pMsg;
							else m_pSendMessages = pMsg;
							m_pSendTail = pMsg;
						}
						else
						{
							// sent to someone else, forward it on
							pMsg->m_pNext = 0;
							if ( m_pSendTail ) m_pSendTail->m_pNext = pMsg;
							else m_pSendMessages = pMsg;
							m_pSendTail = pMsg;
						}
					}
					break;
				}

				case 6: // client disconnected
				{
					if ( !m_ppClients[ i ]->m_bDisconnected )
					{
						// tell the rest of the clients
						for ( UINT j = 1; j < m_iNumClients; j++ )
						{
							if ( i == j ) continue;
							if ( m_ppClientSock[ j ]->GetDisconnected() ) continue;
							m_ppClientSock[ j ]->SendUInt( 4 ); // disconnected client
							m_ppClientSock[ j ]->SendUInt( m_ppClients[ i ]->m_iID );
						}

						m_ppClients[ i ]->m_bDisconnected = true;
					}
					break;
				}

				case 7: // client keep alive request
				{
					m_ppClientSock[ i ]->SendUInt( 6 ); // respond
					break;
				}
			}
		}
	}
}

void cNetwork::SendChangesServer()
{
	// send pings every 2 seconds
	for ( UINT i = 1; i < m_iNumClients; i++ )
	{
		if ( m_ppClients[ i ]->m_iPingResponded )
		{
			if ( m_ppClients[ i ]->m_fPingStart + 2 < agk::GetRunTime() )
			{
				m_ppClients[ i ]->m_iPingResponded = 0;
				m_ppClients[ i ]->m_fPingStart = agk::GetRunTime();
				m_ppClientSock[ i ]->SendUInt( 0 ); //ping
				m_ppClientSock[ i ]->SendUInt( m_iNumClients );

				// tell client all ping times, including its own
				for ( UINT j = 0; j < m_iNumClients; j++ )
				{
					m_ppClientSock[ i ]->SendUInt( m_ppClients[ j ]->m_iID );
					m_ppClientSock[ i ]->SendFloat( m_ppClients[ j ]->m_fPing ); 
				}
			}
		}
		else
		{
			if ( m_ppClients[ i ]->m_fPingStart + 20 < agk::GetRunTime() )
			{
				// client hasn't responded in 30 seconds, assume disconnected.
				if ( !m_ppClientSock[ i ]->GetDisconnected() ) m_ppClientSock[ i ]->ForceClose();
			}
		}
	}

	// send new and changed variables to all clients
	for ( UINT i = 0; i < m_iNumClients; i++ )
	{
		cAutoLock autolock( m_ppClients[ i ]->GetVarLock() ); // unlocks when destroyed
		UINT iNumVars = m_ppClients[ i ]->GetNumVars();
		cNetworkVariable **pVars = m_ppClients[ i ]->GetVars();

		// count
		UINT iNewVars = 0;
		UINT iChangedVars = 0;
		for ( UINT v = 0; v < iNumVars; v++ )
		{
			if ( pVars[ v ]->m_bNew )
			{
				iNewVars++;
				pVars[ v ]->m_bChanged = false;
			}
			
			if ( pVars[ v ]->m_bChanged )
			{
				iChangedVars++;
			}
		}

		// send
		// 0 is local client and already has changes
		for ( UINT j = 1; j < m_iNumClients; j++ )
		{
			if ( i == j ) continue;
			if ( iNewVars > 0 )
			{
#ifdef _AGK_ERROR_CHECK
#ifdef NETWORK_DEBUG
				uString msg;
				msg.Format( "Server sending new variables from %s to %s, %dms", m_ppClients[ i ]->m_sName.GetStr(), m_ppClients[ j ]->m_sName.GetStr(), agk::Round(agk::GetRunTime()*1000) );
				agk::Warning( msg );
#endif
#endif
				m_ppClientSock[ j ]->SendUInt( 2 ); // new vars
				m_ppClientSock[ j ]->SendUInt( m_ppClients[ i ]->m_iID );
				m_ppClientSock[ j ]->SendUInt( iNewVars );
				for ( UINT v = 0; v < iNumVars; v++ )
				{
					if ( pVars[ v ]->m_bNew )
					{
						if ( m_ppClientSock[ j ]->GetDisconnected() ) break;
						m_ppClientSock[ j ]->SendString( pVars[ v ]->m_sName );
						m_ppClientSock[ j ]->SendUInt( pVars[ v ]->m_iMode );
						m_ppClientSock[ j ]->SendUInt( pVars[ v ]->m_iType );
						if ( pVars[ v ]->m_iType == 0 ) m_ppClientSock[ j ]->SendInt( pVars[ v ]->m_value.i );
						else m_ppClientSock[ j ]->SendFloat( pVars[ v ]->m_value.f );
					}
				}
			}

			if ( m_ppClientSock[ j ]->GetDisconnected() ) continue;

			if ( iChangedVars > 0 )
			{
#ifdef _AGK_ERROR_CHECK
#ifdef NETWORK_DEBUG
				uString msg;
				msg.Format( "Server sending changed variables from %s to %s, %dms", m_ppClients[ i ]->m_sName.GetStr(), m_ppClients[ j ]->m_sName.GetStr(), agk::Round(agk::GetRunTime()*1000) );
				agk::Warning( msg );
#endif
#endif
				m_ppClientSock[ j ]->SendUInt( 3 ); // changed vars
				m_ppClientSock[ j ]->SendUInt( m_ppClients[ i ]->m_iID );
				m_ppClientSock[ j ]->SendUInt( iChangedVars );
				for ( UINT v = 0; v < iNumVars; v++ )
				{
					if ( pVars[ v ]->m_bChanged )
					{
						if ( m_ppClientSock[ j ]->GetDisconnected() ) break;
						m_ppClientSock[ j ]->SendUInt( v );
						if ( pVars[ v ]->m_iType == 0 ) m_ppClientSock[ j ]->SendInt( pVars[ v ]->m_value.i );
						else m_ppClientSock[ j ]->SendFloat( pVars[ v ]->m_value.f );
					}
				}
			}
		}

		for ( UINT v = 0; v < iNumVars; v++ )
		{
			pVars[ v ]->m_bNew = false;
			pVars[ v ]->m_bChanged = false;
		}
	}

	// send network messages
	if ( m_pSendMessages )
	{
		cAutoLock autolock( m_kMessageLock );
		cNetworkMessage *pMsg = m_pSendMessages;
		while ( m_pSendMessages )
		{
			pMsg = m_pSendMessages;
			if ( pMsg->m_iFromClientID == 0 ) pMsg->m_iFromClientID = m_iMyClientID;
			if ( pMsg->m_iToClientID == 0 )
			{
				// send to all
				for ( UINT i = 1; i < m_iNumClients; i++ )
				{
					if ( m_ppClients[ i ]->m_iID == pMsg->m_iFromClientID ) continue;
					m_ppClientSock[ i ]->SendUInt( 5 ); // network message
					m_ppClientSock[ i ]->SendUInt( pMsg->m_iFromClientID );
					m_ppClientSock[ i ]->SendUInt( pMsg->GetPos() );
					m_ppClientSock[ i ]->SendData( pMsg->GetBuffer(), pMsg->GetPos() );
				}
			}
			else
			{
				UINT *index = m_cClientRef.GetItem( pMsg->m_iToClientID );
				if ( index && *index < m_iNumClients )
				{
					if ( m_ppClientSock[ *index ] )
					{
						m_ppClientSock[ *index ]->SendUInt( 5 ); // network message
						m_ppClientSock[ *index ]->SendUInt( pMsg->m_iFromClientID );
						m_ppClientSock[ *index ]->SendUInt( pMsg->GetPos() );
						m_ppClientSock[ *index ]->SendData( pMsg->GetBuffer(), pMsg->GetPos() );
					}
				}
			}
			
			m_pSendMessages = m_pSendMessages->m_pNext;
			delete pMsg;
		}
		m_pSendTail = 0;
	}
}

void cNetwork::SendDisconnects()
{
	// check for disconnects
	// 0 is local client and cannot disconnect
	for ( UINT i = 1; i < m_iNumClients; i++ )
	{
		if ( m_ppClients[ i ]->m_bDisconnected ) continue;
		if ( m_ppClientSock[ i ]->GetDisconnected() || m_ppClients[ i ]->m_bKick )
		{
			if ( !m_ppClientSock[ i ]->GetDisconnected() ) m_ppClientSock[ i ]->Close(false);

			// tell the rest of the clients
			for ( UINT j = 1; j < m_iNumClients; j++ )
			{
				if ( i == j ) continue;
				if ( m_ppClientSock[ j ]->GetDisconnected() ) continue;
				m_ppClientSock[ j ]->SendUInt( 4 ); // disconnected client
				m_ppClientSock[ j ]->SendUInt( m_ppClients[ i ]->m_iID );
			}

			// mark the client as disconnected, wait for the user to signal they are ready to delete it
			cAutoLock autolock( m_kClientLock );
			m_ppClients[ i ]->m_bDisconnected = true;
			m_ppClients[ i ]->m_bDelete = false;
		}
	}

	// check for deleted clients
	for ( UINT i = 1; i < m_iNumClients; i++ )
	{
		if ( m_ppClients[ i ]->m_bDelete )
		{
			cAutoLock autolock( m_kClientLock );

			// remove the client
			if ( m_iNumClients > 1 )
			{
				AGKSocket *pSock = m_ppClientSock[ i ];
				cNetworkClient *pClient = m_ppClients[ i ];

				// move the end client to this position
				m_ppClientSock[ i ] = m_ppClientSock[ m_iNumClients-1 ];
				m_ppClients[ i ] = m_ppClients[ m_iNumClients-1 ];
				UINT *index = m_cClientRef.GetItem( m_ppClients[ i ]->m_iID );
				*index = i;

				m_iNumClients--;

				// delete client
				index = m_cClientRef.RemoveItem( pClient->m_iID );
				delete index;
				delete pSock;
				delete pClient;
			}
			else
			{
				m_iNumClients = 0;
				UINT *index = m_cClientRef.RemoveItem( m_ppClients[ i ]->m_iID );
				delete index;
				delete m_ppClientSock[ i ];
				delete m_ppClients[ i ];
			}
			
			i--;
		}
	}
}


void cNetwork::UpdateServer()
{
	CheckMessagesServer();
	if ( m_bTerminate ) return;
	
	SendChangesServer();	
	if ( m_bTerminate ) return;

	SendDisconnects();
	if ( m_bTerminate ) return;

	// check for new clients
	if ( m_pListener )
	{
		AGKSocket *pNew;
		while( (pNew = m_pListener->GetNewConnection()) ) NewClient( pNew );
	}

	if ( m_pListenerv6 )
	{
		AGKSocket *pNew;
		while( (pNew = m_pListenerv6->GetNewConnection()) ) NewClient( pNew );
	}

	// send any remaining data
	for ( UINT i = 1; i < m_iNumClients; i++ )
	{
		m_ppClientSock[ i ]->Flush();
	}
}

void cNetwork::ConnectClient()
{
	{
		cAutoLock alock(m_kCleanUpLock);
		if ( m_pServerSock ) delete m_pServerSock;
		m_pServerSock = 0;
	}

	if ( m_pBroadcastListener || m_pBroadcastListenerv6 )
	{
		// search and connect
		if ( m_pBroadcastListener ) m_pBroadcastListener->SetListenPort( "anyip4", 45631 );
		if ( m_pBroadcastListenerv6 ) m_pBroadcastListenerv6->SetListenPort( "anyip6", 45632 );

		uString sName;
		AGKPacket packet;
		char fromIP[ 65 ];

		while( 1 )
		{
			UINT fromPort = 0;
			if ( m_pBroadcastListener && m_pBroadcastListener->ReceivedBroadcast() )
			{
				if ( !m_pBroadcastListener->GetPacket( packet, fromPort, fromIP ) ) return;
			}
			else if ( m_pBroadcastListenerv6 && m_pBroadcastListenerv6->ReceivedBroadcast() )
			{
				if ( !m_pBroadcastListenerv6->GetPacket( packet, fromPort, fromIP ) ) return;
			}
			else
			{
				if ( m_bTerminate ) return;
				agk::Sleep( 50 );
				continue;
			}
			packet.GetString( sName );
			if ( sName.CompareTo( m_sNetworkName ) == 0 ) break;
		}

		if ( m_pBroadcastListener ) m_pBroadcastListener->Close();
		if ( m_pBroadcastListenerv6 ) m_pBroadcastListenerv6->Close();

		UINT port = packet.GetUInt();

		{
			cAutoLock alock(m_kCleanUpLock);
			m_pServerSock = new AGKSocket();
		}

		if ( !m_pServerSock->Connect( fromIP, port ) )
		{
#ifdef _AGK_ERROR_CHECK
			agk::Warning( "Failed to connect to specified network - check your firewall permissions" );
#endif
			// 211011 - fixed 'tried to flush' breed of issues, by ensuring socket is deleted before we error out
			// this will cause the thread to loop back around and continue until a connection is successfuk
			{
				cAutoLock alock(m_kCleanUpLock);
				delete m_pServerSock;
				m_pServerSock = 0;
			}
			return;
		}
	}
	else
	{
		// direct connect
		{
			cAutoLock alock(m_kCleanUpLock);
			m_pServerSock = new AGKSocket();
		}

		if ( !m_pServerSock->Connect( m_szServerIP, m_iServerPort ) )
		{
#ifdef _AGK_ERROR_CHECK
			agk::Warning( "Failed to connect to server IP" );
#endif
			{
				cAutoLock alock(m_kCleanUpLock);
				delete m_pServerSock;
				m_pServerSock = 0;
			}
			return;
		}
	}

#ifdef NETWORK_DEBUG
	agk::Warning( "Client connected, sending name" );
#endif

	m_pServerSock->SendString( m_sMyName );
	m_pServerSock->Flush();

	if ( m_pServerSock->RecvUInt() == 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Warning( "Failed to connect to specified network, client name is not unique" );
#endif
		return;
	}

#ifdef NETWORK_DEBUG
	agk::Warning( "Name received" );
#endif

	//connected, get assigned ID
	m_iMyClientID = m_pServerSock->RecvUInt();
#ifdef NETWORK_DEBUG
	uString msg;
	msg.Format( "Got client ID: %d", m_iMyClientID );
	agk::Warning( msg );
#endif

	cAutoLock autolock( m_kClientLock );

#ifdef NETWORK_DEBUG
	agk::Warning( "Got client lock" );
#endif

	// create local client
	m_ppClients[ 0 ]->m_iID = m_iMyClientID;
	UINT *index = new UINT;
	*index = 0;
	m_cClientRef.AddItem( index, m_iMyClientID );

	{
		// set any variables as new as the server won't have them
		cAutoLock varlock(m_ppClients[ 0 ]->GetVarLock());
		UINT numVars = m_ppClients[ 0 ]->GetNumVars();
		cNetworkVariable **pVars = m_ppClients[ 0 ]->GetVars();
		for ( UINT v = 0; v < numVars; v++ )
		{
			pVars[ v ]->m_bNew = true;
		}
	}

	// receive all current clients
	UINT iClients = m_pServerSock->RecvUInt();

#ifdef NETWORK_DEBUG
	msg.Format( "New clients: %d", iClients );
	agk::Warning( msg );
#endif

	for ( UINT i = 0; i < iClients; i++ )
	{
#ifdef _AGK_ERROR_CHECK
#ifdef NETWORK_DEBUG
		agk::Warning( "Client recevied clients" );
#endif
#endif
		cNetworkClient *pNewClient = new cNetworkClient();
		pNewClient->m_iID = m_pServerSock->RecvUInt();
		m_pServerSock->RecvString( pNewClient->m_sName );
		if ( m_pServerSock->GetDisconnected() ) return;

		// get all its variables
		UINT numVars = m_pServerSock->RecvUInt();
		for ( UINT v = 0; v < numVars; v++ )
		{
			uString varName;
			m_pServerSock->RecvString( varName );
			UINT type = m_pServerSock->RecvUInt();
			UINT mode = m_pServerSock->RecvUInt();
			if ( type == 0 ) pNewClient->SetVariableI( varName, m_pServerSock->RecvInt(), mode );
			else pNewClient->SetVariableF( varName, m_pServerSock->RecvFloat(), mode );
		}

		// add new client to local store
		if ( m_iNumClients >= m_iArraySize )
		{
			// resize array
			cNetworkClient **pNewArray = new cNetworkClient*[ m_iArraySize + 10 ];
			if ( m_ppClients )
			{
				for ( UINT i = 0; i < m_iNumClients; i++ )
				{
					pNewArray[ i ] = m_ppClients[ i ];
				}

				delete [] m_ppClients;
			}
			m_ppClients = pNewArray;

			m_iArraySize += 10;
		}

		UINT ref = m_iNumClients;
		m_ppClients[ ref ] = pNewClient;
		UINT *index = new UINT;
		*index = ref;
		m_cClientRef.AddItem( index, pNewClient->m_iID );
		m_iNumClients++;
	}
}

void cNetwork::CheckMessagesClient()
{
	if ( m_pServerSock->GetDisconnected() ) 
	{
		// mark all clients as disconnected
		cAutoLock autolock( m_kClientLock );
		for ( UINT i = 1; i < m_iNumClients; i++ )
		{
			m_ppClients[ i ]->m_bDisconnected = true;
		}
	}

	while ( m_pServerSock->GetBytes() >= 4 )
	{
		UINT type = m_pServerSock->RecvUInt();
		if ( m_pServerSock->GetDisconnected() ) 
		{
#ifdef _AGK_ERROR_CHECK
			agk::Warning( "Server disconnected whilst trying to check shared variables" );
#endif
			return;
		}

		switch( type )
		{
			case 0: // ping
			{
				m_pServerSock->SendUInt( 0 ); // respond
				
				// get all client pings
				UINT numClients = m_pServerSock->RecvUInt( );
				for ( UINT i = 0; i < numClients; i++ )
				{
					UINT clientID = m_pServerSock->RecvUInt();
					float ping = m_pServerSock->RecvFloat();

					UINT *index = m_cClientRef.GetItem( clientID );
					if ( index == 0 || *index >= m_iNumClients ) continue;
					m_ppClients[ i ]->m_fPing = ping;
				}

				break;
			}

			case 1: // new client
			{
#ifdef _AGK_ERROR_CHECK
#ifdef NETWORK_DEBUG
				agk::Warning( "Client recevied new client" );
#endif
#endif
				cNetworkClient *pNewClient = new cNetworkClient();
				pNewClient->m_iID = m_pServerSock->RecvUInt();
				m_pServerSock->RecvString( pNewClient->m_sName );

				cAutoLock autolock( m_kClientLock );

				// add to internal array
				if ( !m_ppClients || m_iNumClients >= m_iArraySize )
				{
					// resize array
					cNetworkClient **pNewArray = new cNetworkClient*[ m_iArraySize + 10 ];
					if ( m_ppClients )
					{
						for ( UINT i = 0; i < m_iNumClients; i++ )
						{
							pNewArray[ i ] = m_ppClients[ i ];
						}

						delete [] m_ppClients;
					}
					m_ppClients = pNewArray;

					m_iArraySize += 10;
				}
				
				// add new client
				UINT ref = m_iNumClients;
				m_ppClients[ ref ] = pNewClient;
				UINT *index = new UINT;
				*index = ref;
				m_cClientRef.AddItem( index, pNewClient->m_iID );
				m_iNumClients++;
				break;
			}

			case 2: // new variables
			{
				UINT iClientID = m_pServerSock->RecvUInt();

				UINT *index = m_cClientRef.GetItem( iClientID );
				if ( index == 0 ) 
				{
#ifdef _AGK_ERROR_CHECK
					agk::Warning( "Received variable update for non existant client" );
#endif
					break;
				}
				if ( *index >= m_iNumClients )
				{
#ifdef _AGK_ERROR_CHECK
					agk::Warning( "Received variable update for missing client" );
#endif
					break;
				}
				if ( iClientID == m_iMyClientID )
				{
#ifdef _AGK_ERROR_CHECK
					agk::Warning( "Received variable update for own variables" );
#endif
					break;
				}

#ifdef NETWORK_DEBUG
				uString msg;
				msg.Format( "%s received new variables from %s, %dms", m_ppClients[ 0 ]->m_sName.GetStr(), m_ppClients[ *index ]->m_sName.GetStr(), agk::Round(agk::GetRunTime()*1000) );
				agk::Warning( msg );
#endif

				UINT numVars = m_pServerSock->RecvUInt();
				for ( UINT v = 0; v < numVars; v++ )
				{
					uString varname;
					m_pServerSock->RecvString( varname );
					UINT mode = m_pServerSock->RecvUInt();
					UINT vartype = m_pServerSock->RecvUInt();
					if ( m_pServerSock->GetDisconnected() ) break;
					int valueI = 0;
					float valueF = 0;
					if ( vartype == 0 ) valueI = m_pServerSock->RecvInt();
					else valueF = m_pServerSock->RecvFloat();

					if ( vartype == 0 ) m_ppClients[ *index ]->SetVariableI( varname, valueI, mode );
					else m_ppClients[ *index ]->SetVariableF( varname, valueF, mode );
				}
				break;
			}

			case 3: // changed variables
			{
				UINT iClientID = m_pServerSock->RecvUInt();

				UINT *index = m_cClientRef.GetItem( iClientID );
				if ( index == 0 ) 
				{
#ifdef _AGK_ERROR_CHECK
					agk::Warning( "Received variable update for non existant client" );
#endif
					break;
				}
				if ( *index >= m_iNumClients )
				{
#ifdef _AGK_ERROR_CHECK
					agk::Warning( "Received variable update for missing client" );
#endif
					break;
				}
				if ( iClientID == m_iMyClientID )
				{
#ifdef _AGK_ERROR_CHECK
					agk::Warning( "Received variable update for own variables" );
#endif
					break;
				}

#ifdef NETWORK_DEBUG
				uString msg;
				msg.Format( "%s received changed variables from %s, %dms", m_ppClients[ 0 ]->m_sName.GetStr(), m_ppClients[ *index ]->m_sName.GetStr(), agk::Round(agk::GetRunTime()*1000) );
				agk::Warning( msg );
#endif

				UINT numVars = m_pServerSock->RecvUInt();
				for ( UINT v = 0; v < numVars; v++ )
				{
					UINT iVarID = m_pServerSock->RecvUInt();
					if ( m_pServerSock->GetDisconnected() ) break;
					int valueI = 0;
					float valueF = 0;
					UINT vartype = m_ppClients[ *index ]->GetVariableType( iVarID );
					if ( vartype == 0 ) valueI = m_pServerSock->RecvInt();
					else valueF = m_pServerSock->RecvFloat();

					if ( vartype == 0 ) m_ppClients[ *index ]->SetVariableI( iVarID, valueI, 2 );
					else m_ppClients[ *index ]->SetVariableF( iVarID, valueF, 2 );
				}
				break;
			}

			case 4: // disconnected client
			{
#ifdef _AGK_ERROR_CHECK
#ifdef NETWORK_DEBUG
				agk::Warning( "Client recevied disconnect" );
#endif
#endif

				// remove the client
				UINT iClientID = m_pServerSock->RecvUInt();

				if ( iClientID == m_iMyClientID )
				{
#ifdef _AGK_ERROR_CHECK
					agk::Warning( "Received disconnect message for self" );
#endif
					break;
				}

				UINT *index = m_cClientRef.GetItem( iClientID );
				if ( index == 0 ) 
				{
#ifdef _AGK_ERROR_CHECK
					agk::Warning( "Received disconnect message for non-existant client" );
#endif
					break;
				}

				if ( *index >= m_iNumClients ) 
				{
#ifdef _AGK_ERROR_CHECK
					agk::Warning( "Received disconnect message for missing client" );
#endif
					break;
				}

				// unlocks when destroyed
				cAutoLock autolock( m_kClientLock );

				// let the user know this client is disconnected, wait for them to signal they are ready to delete it
				m_ppClients[ *index ]->m_bDisconnected = true;
				m_ppClients[ *index ]->m_bDelete = false;

				break;
			}

			case 5: // network message
			{
				UINT from = m_pServerSock->RecvUInt();
				UINT size = m_pServerSock->RecvUInt();
				if ( size > 0 && size <= AGK_NET_PACKET_SIZE )
				{
					cNetworkMessage *pMsg = new cNetworkMessage();
					pMsg->m_iFromClientID = from;
					pMsg->m_iToClientID = m_iMyClientID;
					UINT written = m_pServerSock->RecvData(pMsg->GetRaw(), size);
					UINT total = written;
					while ( written > 0 && total < size )
					{
						written = m_pServerSock->RecvData(pMsg->GetRaw()+total, size-total);
						total += written;
					}
					pMsg->SetPos( 0 );
					pMsg->SetSize( size );

					cAutoLock autolock2( m_kMessageLock );

					pMsg->m_pNext = 0;
					if ( m_pRecvTail ) m_pRecvTail->m_pNext = pMsg;
					else m_pRecvMessages = pMsg;
					m_pRecvTail = pMsg;
				}
				break;
			}

			case 6: // keep alive response
			{
				m_iKeepAliveResponded = 1;
				break;
			}
		}
	}

	// check for deleted clients
	for ( UINT i = 0; i < m_iNumClients; i++ )
	{
		if ( m_ppClients[ i ]->m_bDelete )
		{
			cAutoLock autolock( m_kClientLock );

			if ( m_iNumClients > 1 )
			{
				cNetworkClient *pClient = m_ppClients[ i ];

				// move the end client to this position
				m_ppClients[ i ] = m_ppClients[ m_iNumClients-1 ];
				UINT *index = m_cClientRef.GetItem( m_ppClients[ i ]->m_iID );
				*index = i;

				m_iNumClients--;

				// delete client
				index = m_cClientRef.RemoveItem( pClient->m_iID );
				delete index;
				delete pClient;
			}
			else
			{
				m_iNumClients = 0;
				UINT *index = m_cClientRef.RemoveItem( m_ppClients[ i ]->m_iID );
				delete index;
				delete m_ppClients[ i ];
			}
			i--;
		}
	}
}

void cNetwork::SendChangesClient()
{
	if ( m_pServerSock->GetDisconnected() ) return;

	if ( m_iKeepAliveResponded )
	{
		if ( m_fLastKeepAlive + 2 < agk::GetRunTime() )
		{
			m_pServerSock->SendUInt( 7 ); // keep alive check
			m_fLastKeepAlive = agk::GetRunTime();
			m_iKeepAliveResponded = 0;
		}
	}
	else
	{
		if ( m_fLastKeepAlive + 20 < agk::GetRunTime() )
		{
			// server hasn't responded in 5 seconds, assume disconnected
			m_pServerSock->ForceClose();
		}
	}

	// send new and changed variables to server
	// client 0 is always the local client
	cAutoLock autolock( m_ppClients[ 0 ]->GetVarLock() ); // unlocks when destroyed
	UINT iNumVars = m_ppClients[ 0 ]->GetNumVars();
	cNetworkVariable **pVars = m_ppClients[ 0 ]->GetVars();

	// count
	UINT iNewVars = 0;
	UINT iChangedVars = 0;
	for ( UINT v = 0; v < iNumVars; v++ )
	{
		if ( pVars[ v ]->m_bNew )
		{
			iNewVars++;
			pVars[ v ]->m_bChanged = false;
		}
		
		if ( pVars[ v ]->m_bChanged )
		{
			iChangedVars++;
		}
	}

	// send
	if ( iNewVars > 0 )
	{
#ifdef _AGK_ERROR_CHECK
#ifdef NETWORK_DEBUG
		uString msg;
		msg.Format( "Client sending new variables from %s, %dms", m_ppClients[ 0 ]->m_sName.GetStr(), agk::Round(agk::GetRunTime()*1000) );
		agk::Warning( msg );
#endif
#endif
		m_pServerSock->SendUInt( 2 ); // new vars
		m_pServerSock->SendUInt( iNewVars );
		for ( UINT v = 0; v < iNumVars; v++ )
		{
			if ( pVars[ v ]->m_bNew )
			{
				pVars[ v ]->m_bNew = false;
				if ( m_pServerSock->GetDisconnected() ) break;
				m_pServerSock->SendString( pVars[ v ]->m_sName );
				m_pServerSock->SendUInt( pVars[ v ]->m_iMode );
				m_pServerSock->SendUInt( pVars[ v ]->m_iType );
				if ( pVars[ v ]->m_iType == 0 ) m_pServerSock->SendInt( pVars[ v ]->m_value.i );
				else m_pServerSock->SendFloat( pVars[ v ]->m_value.f );
			}
		}
	}

	if ( m_pServerSock->GetDisconnected() ) return;

	if ( iChangedVars > 0 )
	{
#ifdef _AGK_ERROR_CHECK
#ifdef NETWORK_DEBUG
		uString msg;
		msg.Format( "Client sending changed variables from %s, %dms", m_ppClients[ 0 ]->m_sName.GetStr(), agk::Round(agk::GetRunTime()*1000) );
		agk::Warning( msg );
#endif
#endif
		m_pServerSock->SendUInt( 3 ); // changed vars
		m_pServerSock->SendUInt( iChangedVars );
		for ( UINT v = 0; v < iNumVars; v++ )
		{
			if ( pVars[ v ]->m_bChanged )
			{
				pVars[ v ]->m_bChanged = false;
				if ( m_pServerSock->GetDisconnected() ) break;
				m_pServerSock->SendUInt( v );
				if ( pVars[ v ]->m_iType == 0 ) m_pServerSock->SendInt( pVars[ v ]->m_value.i );
				else m_pServerSock->SendFloat( pVars[ v ]->m_value.f );
			}
		}
	}

	// send any network messages
	if ( m_pSendMessages )
	{
		cAutoLock autolock( m_kMessageLock );
		cNetworkMessage *pMsg = m_pSendMessages;
		while ( m_pSendMessages )
		{
			pMsg = m_pSendMessages;
			if ( pMsg->m_iFromClientID == 0 ) pMsg->m_iFromClientID = m_iMyClientID;
			
			m_pServerSock->SendUInt( 5 ); // network message
			m_pServerSock->SendUInt( pMsg->m_iFromClientID );
			m_pServerSock->SendUInt( pMsg->m_iToClientID );
			m_pServerSock->SendUInt( pMsg->GetPos() );
			m_pServerSock->SendData( pMsg->GetBuffer(), pMsg->GetPos() );
			
			m_pSendMessages = m_pSendMessages->m_pNext;
			delete pMsg;
		}
		m_pSendTail = 0;
	}

	m_pServerSock->Flush();
}

void cNetwork::UpdateClient()
{
	CheckMessagesClient();
	if ( m_bTerminate ) return;

	SendChangesClient();
	if ( m_bTerminate ) return;
}

void cNetwork::Stop()
{
	cAutoLock alock(m_kCleanUpLock);

    AGKThread::Stop();
    
	if ( m_pBroadcaster ) m_pBroadcaster->Stop();
	if ( m_pBroadcasterv6 ) m_pBroadcasterv6->Stop();
	if ( m_pListener ) m_pListener->Stop();
	if ( m_pListenerv6 ) m_pListenerv6->Stop();
	if ( m_pBroadcastListener ) m_pBroadcastListener->Close();
	if ( m_pBroadcastListenerv6 ) m_pBroadcastListenerv6->Close();

	if ( m_pServerSock ) m_pServerSock->Close(false);
	if ( m_ppClientSock )
	{
		for ( UINT i = 0; i < m_iNumClients; i++ ) 
		{
			if ( m_ppClientSock[ i ] ) m_ppClientSock[ i ]->Close(false);
		}
	}
}

UINT cNetwork::Run()
{
	if ( m_bServer )
	{
		while ( !m_bTerminate )
		{
			UpdateServer();
			SleepSafe( m_iLatency );
		}
	}
	else
	{
		// client
		if ( !m_pServerSock )
		{
			// searches for the network name in the broadcasts and returns when connected
			ConnectClient();
			if ( m_bTerminate ) return 0; 
		}

		if ( !m_pServerSock ) return 0;

		while ( !m_bTerminate )
		{
			UpdateClient();
			if ( m_pServerSock->GetDisconnected() ) return 0;
			SleepSafe( m_iLatency ); // may want receiver to wait on a separate thread for faster updates
		}
	}

	{
		cAutoLock alock(m_kCleanUpLock);
		
		if ( m_pServerSock ) delete m_pServerSock;
		m_pServerSock = 0;

		if ( m_ppClientSock )
		{
			for ( UINT i = 0; i < m_iNumClients; i++ ) 
			{
				if ( m_ppClientSock[ i ] ) 
				{
					delete m_ppClientSock[ i ];
					m_ppClientSock[ i ] = 0;
				}
			}
		}
	}

	return 0;
}

void cNetwork::HostNetwork( const char *szNetworkName, const char *szMyName, int port, int portv6 )
{
	Reset();
	m_sNetworkName.SetStr( szNetworkName );
	m_sMyName.SetStr( szMyName );
	m_bServer = true;

	m_iHostPort = port;
	m_iHostPortv6 = portv6;

	if ( port > 0 )
	{
		m_pListener = new cNetworkListener();
		m_pListener->AcceptConnections( "anyip4", port );

		AGKPacket packet;
		packet.AddString( szNetworkName );
		packet.AddUInt( port );
		m_pBroadcaster = new Broadcaster();
		m_pBroadcaster->SetData( 0, 45631, &packet, 1000 );
		m_pBroadcaster->Start();
	}
	
	if ( portv6 > 0 )
	{
		m_pListenerv6 = new cNetworkListener();
		m_pListenerv6->AcceptConnections( "anyip6", portv6 );

		AGKPacket packet2;
		packet2.AddString( szNetworkName );
		packet2.AddUInt( portv6 );
		m_pBroadcasterv6 = new Broadcaster();
		m_pBroadcasterv6->SetData( 1, 45632, &packet2, 1000 );
		m_pBroadcasterv6->Start();
	}

	m_ppClients = new cNetworkClient*[ 10 ];
	m_ppClientSock = new AGKSocket*[ 10 ];
	m_iArraySize = 10;
	
	m_ppClients[ 0 ] = new cNetworkClient();
	m_ppClientSock[ 0 ] = 0; // no connection to local client

	m_ppClients[ 0 ]->m_iID = m_iNextClientID++;
	m_ppClients[ 0 ]->m_sName.SetStr( szMyName );
	m_iMyClientID = m_ppClients[ 0 ]->m_iID;
	UINT *index = new UINT;
	*index = 0;
	m_cClientRef.AddItem( index, m_iMyClientID );
	m_iNumClients = 1;

	Start();
}

void cNetwork::NoMoreConnections()
{
	if ( m_pBroadcaster ) m_pBroadcaster->Stop();
	if ( m_pBroadcasterv6 ) m_pBroadcasterv6->Stop();
	if ( m_pListener ) m_pListener->Stop();
	if ( m_pListenerv6 ) m_pListenerv6->Stop();
}

void cNetwork::AllowConnections()
{
	if ( m_pListener && !m_pListener->IsRunning() && m_iHostPort > 0 ) 
	{
		m_pListener->AcceptConnections( "anyip4", m_iHostPort );
		if ( m_pBroadcaster && !m_pBroadcaster->IsRunning() ) m_pBroadcaster->Start();
	}
	if ( m_pListenerv6 && !m_pListenerv6->IsRunning() && m_iHostPortv6 > 0 ) 
	{
		m_pListenerv6->AcceptConnections( "anyip6", m_iHostPortv6 );
		if ( m_pBroadcasterv6 && !m_pBroadcasterv6->IsRunning() ) m_pBroadcasterv6->Start();
	}
}

void cNetwork::JoinCommon()
{
	m_bServer = false;

	m_ppClients = new cNetworkClient*[ 10 ];
	m_ppClients[ 0 ] = new cNetworkClient();
	m_ppClients[ 0 ]->m_iID = 0;
	m_ppClients[ 0 ]->m_sName.SetStr( m_sMyName );
	
	m_iArraySize = 10;
	m_iNumClients = 1;

	Start();
}

void cNetwork::JoinNetwork( const char *szNetworkName, const char *szMyName )
{
	Reset();
	m_sNetworkName.SetStr( szNetworkName );
	m_sMyName.SetStr( szMyName );

	m_pBroadcastListener = new BroadcastListener();
	m_pBroadcastListenerv6 = new BroadcastListener();

	JoinCommon();
}

void cNetwork::JoinNetwork( const char *szIP, UINT port, const char *szMyName )
{
	Reset();
	m_sNetworkName.SetStr( "" );
	strcpy( m_szServerIP, szIP );
	m_iServerPort = port;
	m_sMyName.SetStr( szMyName );
	
	JoinCommon();
}

void cNetwork::CloseNetwork()
{
	if ( m_pServerSock && m_pServerSock->IsConnected() && !m_pServerSock->GetDisconnected() ) 
	{
		m_pServerSock->SendUInt( 6 ); // closing
		m_pServerSock->Flush();
	}
	Stop();
}

void cNetwork::SetLatency( UINT latency )
{
	if ( latency < 1 ) latency = 1;
	if ( latency > 1000 ) latency = 1000;

	m_iLatency = latency;
}

UINT cNetwork::GetNumClients()
{
	if ( m_iNumClients == 1 && m_ppClients[ 0 ]->m_iID == 0 ) return 0;
	if ( !IsActive() ) return 0;
	return m_iNumClients;
}

UINT cNetwork::GetNumActiveClients()
{
	if ( m_iNumClients == 1 && m_ppClients[ 0 ]->m_iID == 0 ) return 0;
	if ( !IsActive() ) return 0;

	cAutoLock autolock(m_kClientLock);

	UINT count = 0;
	for ( UINT i = 0; i < m_iNumClients; i++ )
	{
		if ( !m_ppClients[ i ]->m_bDisconnected ) count++;
	}

	return count;
}

UINT cNetwork::GetFirstClient()
{
	if ( m_iClientIter != 0xffffffff ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to GetFirstClient(), GetNextClient() must be called until it returns 0 first." );
#endif
		return 0;
	}

	if ( !m_ppClients || m_iNumClients == 0 ) return 0;
	if ( m_iNumClients == 1 && m_ppClients[ 0 ]->m_iID == 0 ) return 0;

	// dangerous lock? GetNextClient must be called to completion to unlock
	m_kClientLock->Acquire();

	m_iClientIter = 0;
	return m_ppClients[ m_iClientIter ]->m_iID;
}

UINT cNetwork::GetNextClient()
{
	if ( m_iClientIter == 0xffffffff ) return 0;

	m_iClientIter++;
	if ( m_iClientIter >= m_iNumClients ) 
	{
		m_kClientLock->Release();
		m_iClientIter = 0xffffffff;
		return 0;
	}
	return m_ppClients[ m_iClientIter ]->m_iID;
}

UINT cNetwork::GetClientDisconnected( UINT client )
{
	cAutoLock autolock( m_kClientLock );

	UINT *index = m_cClientRef.GetItem( client );
	if ( index == 0 ) return 1;
	if ( *index >= m_iNumClients ) return 1;
		
	return m_ppClients[ *index ]->m_bDisconnected ? 1 : 0;
}

void cNetwork::DeleteDisconnectedClient( UINT client )
{
	bool bFailed = false;

	{
		cAutoLock autolock( m_kClientLock );

		UINT *index = m_cClientRef.GetItem( client );
		if ( index == 0 ) return;
		if ( *index >= m_iNumClients ) return;
		
		if ( m_ppClients[ *index ]->m_bDisconnected ) m_ppClients[ *index ]->m_bDelete = true;
		else bFailed = true;
	}

	if ( bFailed )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot delete a client that is still connected, you must wait for it to disconnect from the server" );
#endif
	}
}

int cNetwork::GetClientName( UINT client, uString &sName )
{
	int result = -1;

	{
		cAutoLock autolock( m_kClientLock ); // can platform double lock? (confirmed works on Windows and iOS)
		
		UINT *index = m_cClientRef.GetItem( client );
		if ( index != 0 && *index < m_iNumClients ) 
		{
			sName.SetStr( m_ppClients[ *index ]->m_sName );
			result = sName.GetLength();
		}
	}

	if ( result < 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Invalid client ID passed to cNetwork::GetClientName()" );
#endif
	}

	return result;
}

int cNetwork::GetClientIP( uint32_t client, uString &sIP )
{
	int result = -1;
	if ( !m_ppClientSock ) 
	{
		agk::Error( "Failed to get client IP, only the server knows the client IPs" );
		return result;
	}

	{
		cAutoLock autolock( m_kClientLock ); // can platform double lock? (confirmed works on Windows and iOS)
		
		UINT *index = m_cClientRef.GetItem( client );
		if ( index != 0 && *index < m_iNumClients ) 
		{
			sIP.SetStr( m_ppClientSock[ *index ]->GetRemoteIP() );
			result = sIP.GetLength();
		}
	}

	if ( result < 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Invalid client ID passed to cNetwork::GetClientIP()" );
#endif
	}

	return result;
}

int cNetwork::GetServerIP( uString &sIP )
{
	int result = -1;
	if ( !m_pServerSock ) 
	{
		agk::Error( "Failed to get server IP, use GetDeviceIP or GetDeviceIPv6 instead" );
		return result;
	}

	{
		cAutoLock alock(m_kCleanUpLock);
		sIP.SetStr( m_pServerSock->GetRemoteIP() );
		result = sIP.GetLength();
	}
	
	if ( result < 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to get server IP" );
#endif
	}

	return result;
}

void cNetwork::KickClient( UINT clientID )
{
	if ( !m_bServer )
	{
		agk::Error( "Only the host can kick clients" );
		return;
	}

	cAutoLock autolock( m_kClientLock );

	UINT *index = m_cClientRef.GetItem( clientID );
	if ( index == 0 ) 
	{
		agk::Error( "You cannot kick yourself off the network, use CloseNetwork instead" );
		return;
	}
	if ( *index >= m_iNumClients ) return; 
	m_ppClients[ *index ]->m_bKick = true;
}

float cNetwork::GetClientPing( UINT client )
{
	cAutoLock autolock( m_kClientLock );
	
	UINT *index = m_cClientRef.GetItem( client );
	if ( index == 0 ) return 0;
	if ( *index >= m_iNumClients ) return 0; 
	return m_ppClients[ *index ]->m_fPing;
}

void cNetwork::SetLocalVariableI( const char *name, int i, int mode )
{
	if ( !m_ppClients ) return;
	m_ppClients[ 0 ]->SetVariableI( name, i, mode );
}

void cNetwork::SetLocalVariableF( const char *name, float f, int mode )
{
	if ( !m_ppClients ) return;
	m_ppClients[ 0 ]->SetVariableF( name, f, mode );
}

int cNetwork::GetClientVariableI( UINT client, const char *name )
{
	cAutoLock autolock( m_kClientLock );

	UINT *index = m_cClientRef.GetItem( client );
	if ( index == 0 ) return 0;
	if ( *index >= m_iNumClients ) return 0;
	return m_ppClients[ *index ]->GetVariableI( name, client==m_iMyClientID );
}

float cNetwork::GetClientVariableF( UINT client, const char *name )
{
	cAutoLock autolock( m_kClientLock );

	UINT *index = m_cClientRef.GetItem( client );
	if ( index == 0 ) return 0;
	if ( *index >= m_iNumClients ) return 0;
	return m_ppClients[ *index ]->GetVariableF( name, client==m_iMyClientID );
}

void cNetwork::SendMessage( UINT toClient, cNetworkMessage* pMsg )
{
	if ( toClient == m_iMyClientID ) 
	{
		if ( pMsg ) delete pMsg;
		return;
	}

	{
		cAutoLock autolock( m_kClientLock );
		if ( toClient > 0 && !m_cClientRef.GetItem( toClient ) )
		{
#ifdef _AGK_ERROR_CHECK
			agk::Warning( "Invalid client ID detected in network SendMessage(), message will be ignored" );
#endif
			// ignored by the other thread, add it anyway.
		}
	}

	cAutoLock autolock( m_kMessageLock );

	pMsg->m_iToClientID = toClient;
	pMsg->m_pNext = 0;
	if ( m_pSendTail ) m_pSendTail->m_pNext = pMsg;
	else m_pSendMessages = pMsg;
	m_pSendTail = pMsg;
}

cNetworkMessage* cNetwork::GetMessage( )
{
	cAutoLock autolock( m_kMessageLock );

	cNetworkMessage* pMsg = m_pRecvMessages;
	if ( pMsg ) m_pRecvMessages = m_pRecvMessages->m_pNext;
	if ( !m_pRecvMessages ) m_pRecvTail = 0;
	return pMsg;
}

void cNetwork::SetClientUserData( UINT client, UINT index, int value )
{
	if ( index >= 5 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Can only store a maximum of 5 user values in a client" );
#endif
		return;
	}

	cAutoLock autolock( m_kClientLock );

	UINT *index2 = m_cClientRef.GetItem( client );
	if ( index2 == 0 ) return;
	if ( *index2 >= m_iNumClients ) return;
	m_ppClients[ *index2 ]->m_iUserValue[ index ] = value;
}

int cNetwork::GetClientUserData( UINT client, UINT index )
{
	if ( index >= 5 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Can only get a maximum of 5 user values from a client" );
#endif
		return 0;
	}

	cAutoLock autolock( m_kClientLock );

	UINT *index2 = m_cClientRef.GetItem( client );
	if ( index2 == 0 ) return 0;
	if ( *index2 >= m_iNumClients ) return 0;
	return m_ppClients[ *index2 ]->m_iUserValue[ index ];
}

void cNetwork::Update()
{
#ifdef _AGK_ERROR_CHECK
	agk::Warning( "Function cNetwork::Update() not complete" );
#endif
	return;
	m_bTerminate = false;

	if ( m_bServer ) 
	{
		UpdateServer();
	}
	else 
	{
		if ( !m_pServerSock ) ConnectClient();
		if ( !m_pServerSock ) return;
		UpdateClient();
	}
}
