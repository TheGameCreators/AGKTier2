#include "agk.h"
#include "errno.h"
#include "curl/curl.h"
#include <net/if.h>
#include <ifaddrs.h>
#include <netdb.h>

using namespace AGK;

int agk::PlatformGetIP( uString &sIP )
{
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    
    sIP.SetStr("");
        
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while (temp_addr != NULL) {
            if( temp_addr->ifa_addr->sa_family == AF_INET) {
                const char* szIP = inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr);
                if ( strcmp(szIP,"127.0.0.1") != 0 ) {
                    sIP.SetStr( szIP );
                    break;
                }
            }
            
            temp_addr = temp_addr->ifa_next;
        }
    }
    
    // Free memory
    freeifaddrs(interfaces);
    
    if ( sIP.GetLength() == 0 ) 
	{
		sIP.SetStr( "127.0.0.1" );
		//return 0;
	}
	
	return 1;
}

int agk::PlatformGetIPv6( uString &sIP, int *iInterface )
{
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    
    sIP.SetStr("");
    int currLevel = 0;
    
    char szName[16]; 
    *szName = 0;
        
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while (temp_addr != NULL) {
			if( temp_addr->ifa_addr->sa_family == AF_INET6) {
                char szIP[ 65 ];
				inet_ntop( AF_INET6, &(((struct sockaddr_in6*)temp_addr->ifa_addr)->sin6_addr), szIP, 65 );
                int level = 0;
                if ( strncmp( szIP, "::1", 3 ) == 0 ) level = -1; // loopback
                else if ( strncmp( szIP, "ff", 2 ) == 0 ) level = -1; // multicast address
                else if ( strncmp( szIP, "fe", 2 ) == 0 ) level = 1; // link local address
                else if ( strncmp( szIP, "fc", 2 ) == 0 ) level = 2; // site local address
                else if ( strncmp( szIP, "fd", 2 ) == 0 ) level = 2; // site local address
                else level = 3;
                
                //NSLog( @"  Addr: %s, Level: %d", szIP, level );
                
                if ( level > currLevel )
                {
                    currLevel = level;
                    strcpy(szName, temp_addr->ifa_name);
                    sIP.SetStr( szIP );
                }
            }
            
            temp_addr = temp_addr->ifa_next;
        }
    }
    
    // Free memory
    freeifaddrs(interfaces);
    
    if ( sIP.GetLength() == 0 ) 
	{
		sIP.SetStr( "::1" );
		return 1;
	}
    
    if ( iInterface ) 
    {
		*iInterface = if_nametoindex( szName );
    }
    
    return 1;
}

//*********************
// UDPManager
//*********************

UDPManager::UDPManager( const char* szIP, UINT listenPort )
{
	m_iValid = 0;
	if ( listenPort == 0 ) listenPort = 65535;
	if ( listenPort > 65535 ) listenPort = 65535;
	m_port = listenPort;

	if ( !szIP || !*szIP ) szIP = "anyip4";

	m_iIPv6 = 0;
	if ( strcmp(szIP,"anyip6") == 0 || strchr(szIP,':') ) m_iIPv6 = 1;

	unsigned short family;
	int size = 0;
	sockaddr_storage addr;
	if ( !m_iIPv6 )
	{
		family = AF_INET;
		sockaddr_in *addrv4 = (sockaddr_in*) &addr;
		addrv4->sin_family = family;
		addrv4->sin_port = htons( m_port );
		if ( strcmp(szIP,"anyip4") == 0 ) addrv4->sin_addr.s_addr = INADDR_ANY;
		else addrv4->sin_addr.s_addr = inet_addr( szIP );
		size = sizeof(sockaddr_in);
	}
	else
	{
		family = AF_INET6;
		sockaddr_in6 *addrv6 = (sockaddr_in6*) &addr;
		memset(addrv6, 0, sizeof(sockaddr_in6));
		addrv6->sin6_family = family;
		addrv6->sin6_port = htons( m_port );
		if ( strcmp( szIP, "anyip6" ) == 0 ) addrv6->sin6_addr = in6addr_any;
		else inet_pton( AF_INET6, szIP, &(addrv6->sin6_addr) );
		size = sizeof(sockaddr_in6);
	}

	m_socket = socket( family, SOCK_DGRAM, IPPROTO_UDP );
	if ( m_socket == INVALID_SOCKET ) 
	{
		agk::Warning( "Failed to create UDP socket" );
		return;
	}

	int value = 1;
    setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(value) );
	
	if ( family == AF_INET6 ) 
	{
		int value = 1;
		setsockopt( m_socket, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&value, sizeof(value) ); // don't listen on IPV4 addresses

		// if address is multicast then join the group
		if ( strncmp(szIP,"FF",2) == 0 || strncmp(szIP,"ff",2) == 0 ) 
		{
			ipv6_mreq mreq;
			mreq.ipv6mr_interface = 0;
			inet_pton( AF_INET6, szIP, &(mreq.ipv6mr_multiaddr) );
			setsockopt(m_socket, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char*)&mreq, sizeof(mreq)) ;
		}
	}

	if ( bind( m_socket, (sockaddr*) &addr, size ) == SOCKET_ERROR )
	{
		close( m_socket );
		m_socket = INVALID_SOCKET;
		agk::Warning( "Failed to bind UDP socket" );
		return;
	}

	m_iValid = 1;
}

UDPManager::~UDPManager()
{
	if ( m_socket != INVALID_SOCKET )
	{
		close( m_socket );
		m_socket = INVALID_SOCKET;
	}
}

bool UDPManager::SendPacket( const char *IP, UINT port, const AGKPacket *packet )
{
	if ( m_socket == INVALID_SOCKET )
	{
		agk::Error( "Tried to send UDP packet on an uninitialised socket" );
		return false;
	}

	if ( !IP ) return false;
	if ( port == 0 || port > 65535 ) return false;
	if ( !packet ) return false;

	int size;
	sockaddr_storage addr;
	if ( !m_iIPv6 )
	{
		sockaddr_in *addrv4 = (sockaddr_in*) &addr;
		addrv4->sin_family = AF_INET;
		addrv4->sin_port = htons( port );
		addrv4->sin_addr.s_addr = inet_addr( IP );
		size = sizeof(sockaddr_in);
	}
	else
	{
		sockaddr_in6 *addrv6 = (sockaddr_in6*) &addr;
		memset(addrv6, 0, sizeof(sockaddr_in6));
		addrv6->sin6_family = AF_INET6;
		addrv6->sin6_port = htons( port );
		inet_pton( AF_INET6, IP, &(addrv6->sin6_addr) );
		size = sizeof(sockaddr_in6);
	}
	
	int result = sendto( m_socket, packet->GetBuffer(), packet->GetPos(), 0, (sockaddr*)&addr, size );
	if ( result == 0 || result == SOCKET_ERROR ) return false;
	return true;
}

bool UDPManager::RecvPacket( char *fromIP, int *fromPort, AGKPacket *packet )
{
	if ( m_socket == INVALID_SOCKET )
	{
		agk::Error( "Tried to receive UDP packet on an uninitialised socket" );
		return false;
	}

	if ( !packet ) return false;

	unsigned long waiting = 0;
	if ( ioctl( m_socket, FIONREAD, &waiting ) == SOCKET_ERROR )
	{
		agk::Warning( "Failed to get receivable bytes on socket" );
		return false;
	}

	if ( waiting == 0 ) return false;

	sockaddr_storage addr;
	socklen_t length = sizeof(addr);
	int result = recvfrom( m_socket, packet->GetRaw(), AGK_NET_PACKET_SIZE, 0, (sockaddr*)&addr, &length );
	if ( result == SOCKET_ERROR )
	{
		agk::Warning( "Failed to receive UDP packet" );
		return false;
	}

	if ( result > 0 ) 
	{
		packet->SetPos( 0 );
		if ( addr.ss_family == AF_INET6 )
		{
			sockaddr_in6 *addrv6 = (sockaddr_in6*) &addr;
			if ( fromPort ) *fromPort = ntohs(addrv6->sin6_port);
			if ( fromIP ) inet_ntop( AF_INET6, &(addrv6->sin6_addr), fromIP, 100 ); // don't know size so guess 100
		}
		else
		{
			sockaddr_in *addrv4 = (sockaddr_in*) &addr;
			if ( fromPort ) *fromPort = ntohs(addrv4->sin_port);
			if ( fromIP ) strcpy( fromIP, inet_ntoa( addrv4->sin_addr ) );
		}
	}

	return true;
}

bool UDPManager::PacketReady()
{
	if ( m_socket == INVALID_SOCKET )
	{
		agk::Error( "Tried to check UDP packet on an uninitialised socket" );
		return false;
	}
	
	unsigned long waiting = 0;
	if ( ioctl( m_socket, FIONREAD, &waiting ) == SOCKET_ERROR )
	{
		agk::Warning( "Failed to get receivable bytes on socket" );
		return false;
	}

	if ( waiting == 0 ) return false;
	return true;
}

//*********************
// AGKSocketTimeout
//*********************

AGKSocketTimeout::AGKSocketTimeout()
{
	m_pSocket = 0;
	m_iTimeout = 0;
}

AGKSocketTimeout::~AGKSocketTimeout()
{
	Stop();
	Join();
}

void AGKSocketTimeout::SetData( AGKSocket *pSocket, UINT iTimeout )
{
	m_pSocket = pSocket;
	m_iTimeout = iTimeout;
}

UINT AGKSocketTimeout::Run()
{
	if ( !m_pSocket ) return 0;
	if ( !m_pSocket->m_bConnecting ) return 0;

	if ( m_iTimeout > 0 ) SleepSafe( m_iTimeout );
	if ( m_bTerminate ) return 0;
	agk::Warning("Socket Timeout Exceeded");
	if ( m_pSocket->m_bConnecting ) 
	{
		if ( m_pSocket->m_bASync ) m_pSocket->Stop();
		m_pSocket->Close(false);
	}

	return 0;
}

//*********************
// AGKSocket
//*********************

UINT AGKSocket::Run()
{
	int family = 0;
	int size = 0;
	sockaddr_storage addr;
	if ( !IsIPV6() )
	{
		family = AF_INET;
		sockaddr_in *addrv4 = (sockaddr_in*) &addr;
		addrv4->sin_family = family;
		addrv4->sin_port = htons( m_port );
		addrv4->sin_addr.s_addr = inet_addr( m_szIP );
		size = sizeof(sockaddr_in);
	}
	else
	{
		family = AF_INET6;
		sockaddr_in6 *addrv6 = (sockaddr_in6*) &addr;
		memset(addrv6, 0, sizeof(sockaddr_in6));
		addrv6->sin6_family = family;
		addrv6->sin6_port = htons( m_port );
        if ( strncmp(m_szIP, "fe", 2) == 0 || strncmp(m_szIP, "FE", 2) == 0 ) addrv6->sin6_scope_id = agk::GetNetworkInterface();
		inet_pton( AF_INET6, m_szIP, &(addrv6->sin6_addr) );
		size = sizeof(sockaddr_in6);
	}

	float endTime = agk::Timer() + m_iTimeout/1000.0f - 0.1f;

	// blocks, sometimes
	int result = 0;
	do
	{
		m_client = socket( family, SOCK_STREAM, IPPROTO_TCP );
		result = connect( m_client, (sockaddr*) &addr, size );
		if ( result != 0 || !m_bConnecting ) 
		{
			close( m_client );
			m_client = INVALID_SOCKET;
			agk::Sleep( 100 );
		}
	} while( result != 0 && endTime > agk::Timer() && m_bConnecting );

	if ( m_cTimeout.IsRunning() ) 
	{
		m_cTimeout.Stop();
		m_cTimeout.Join();
	}

	if ( result != 0 )
	{
		uString err( "Failed to connect to ", 70 );
		err += m_szIP;
		agk::Warning( err );
		Close();
		return false;
	}

	int opt = 1;
	int optlen = sizeof(int);
	setsockopt( m_client, IPPROTO_TCP, TCP_NODELAY, (char*)&opt, optlen );
	
	m_bConnected = true;
	m_bConnecting = false;
	m_bDisconnected = false;

	return 0;
}

void AGKSocket::Reset()
{
	m_client = INVALID_SOCKET;
	strcpy( m_szIP, "" );
	m_port = 0;
	m_bConnected = false;
	m_bConnecting = false;
	m_bDisconnected = false;
	m_pNext = 0;
	m_iTimeout = 3000;
	m_fProgress = 0;
	m_bASync = false;
	
	m_sendBuffer[ 0 ] = 0;
	m_iSendWritePtr = 0;
}

AGKSocket::AGKSocket()
{
	Reset();
}

AGKSocket::AGKSocket( int s )
{
	Reset();
	m_client = s;
	m_bConnected = true;
	
	sockaddr_storage addr;
	socklen_t length = sizeof(addr);
	getpeername( m_client, (sockaddr*)&addr, &length );
	if ( addr.ss_family == AF_INET6 )
	{
		sockaddr_in6 *addrv6 = (sockaddr_in6*) &addr;
		m_port = ntohs(addrv6->sin6_port);
		inet_ntop( AF_INET6, &(addrv6->sin6_addr), m_szIP, sizeof(m_szIP) );
	}
	else
	{
		sockaddr_in *addrv4 = (sockaddr_in*) &addr;
		m_port = ntohs(addrv4->sin_port);
		strcpy( m_szIP, inet_ntoa( addrv4->sin_addr ) );
	}
}

AGKSocket::~AGKSocket()
{
	Stop();
	Join();
	Close(false);
}

void AGKSocket::Close( bool bGraceful )
{
	m_bConnecting = false;
	m_bConnected = false;
	m_bDisconnected = true;
	if ( m_client != INVALID_SOCKET ) 
	{
		// try a graceful disconnect
		shutdown( m_client, 2 );
		/*
		if ( bGraceful )
		{
			char temp[ 32 ];
			int count = 0;
			while ( recv( m_client, temp, 32, 0 ) > 0 && count < 20 ) 
			{
				agk::Sleep( 10 );
				count++;
			}
		}
		*/
		//disconnect
		close( m_client );
	}
	m_client = INVALID_SOCKET;
}

void AGKSocket::ForceClose()
{
	m_bConnecting = false;
	m_bConnected = false;
	m_bDisconnected = true;
	if ( m_client != INVALID_SOCKET )
	{
		//disconnect
		close( m_client );
	}
	m_client = INVALID_SOCKET;
}

bool AGKSocket::Connect( const char* IP, UINT port, UINT timeout )
{
	if ( m_bConnected || m_client != INVALID_SOCKET ) 
	{
#ifdef _AGK_ERROR_CHECK
		uString err( "Failed to connect socket to ", 50 );
		err += IP;
		err += ", socket is already connected to ";
		err += m_szIP;
		err += ", you must close a socket before connecting it again.";
		agk::Error( err );
#endif
		return false;
	}

	if ( !IP || strlen( IP ) >= 65 )
	{
#ifdef _AGK_ERROR_CHECK
		uString err( "Invalid IP address used to connect socket.", 50 );
		agk::Error( err );
#endif
		return false;
	}

	m_bASync = false;
	m_iTimeout = timeout;
	m_bConnecting = true;
	strcpy( m_szIP, IP );
	m_port = port;

	int family = AF_INET;
	int size = 0;
	sockaddr_storage addr;
	if ( !IsIPV6() )
	{
		sockaddr_in *addrv4 = (sockaddr_in*) &addr;
		addrv4->sin_family = family;
		addrv4->sin_port = htons( m_port );
		addrv4->sin_addr.s_addr = inet_addr( m_szIP );
		size = sizeof(sockaddr_in);
	}
	else
	{
		family = AF_INET6;
		sockaddr_in6 *addrv6 = (sockaddr_in6*) &addr;
		memset(addrv6, 0, sizeof(sockaddr_in6));
		addrv6->sin6_family = family;
		addrv6->sin6_port = htons( m_port );
        if ( strncmp(m_szIP, "fe", 2) == 0 || strncmp(m_szIP, "FE", 2) == 0 ) addrv6->sin6_scope_id = agk::GetNetworkInterface();
		inet_pton( AF_INET6, m_szIP, &(addrv6->sin6_addr) );
		size = sizeof(sockaddr_in6);
	}

	float endTime = agk::Timer() + timeout/1000.0f - 0.1f;

	if ( m_cTimeout.IsRunning() ) 
	{
		m_cTimeout.Stop();
		m_cTimeout.Join();
	}
	m_cTimeout.SetData( this, timeout );
	m_cTimeout.Start();

	// blocks, sometimes
	int result = 0;
	do
	{
		m_client = socket( family, SOCK_STREAM, IPPROTO_TCP );
		result = connect( m_client, (sockaddr*) &addr, size );
		if ( result != 0 || !m_bConnecting ) 
		{
			close( m_client );
			m_client = INVALID_SOCKET;
			if ( !m_bConnecting ) return false;
			agk::Sleep( 100 );
		}
	} while( result != 0 && endTime > agk::Timer() );

	if ( m_cTimeout.IsRunning() ) 
	{
		m_cTimeout.Stop();
		m_cTimeout.Join();
	}

	if ( result != 0 )
	{
		uString err( "Failed to connect to ", 70 );
		err += m_szIP;
		agk::Warning( err );
		Close();
		return false;
	}

	int opt = 1;
	int optlen = sizeof(int);
	setsockopt( m_client, IPPROTO_TCP, TCP_NODELAY, (char*)&opt, optlen );
	
	m_bConnected = true;
	m_bConnecting = false;
	m_bDisconnected = false;

	return true;
}

bool AGKSocket::ConnectASync( const char* IP, UINT port, UINT timeout )
{
	if ( m_bConnected || m_client != INVALID_SOCKET ) 
	{
#ifdef _AGK_ERROR_CHECK
		uString err( "Failed to connect socket to ", 50 );
		err += IP;
		err += ", socket is already connected to ";
		err += m_szIP;
		err += ", you must close a socket before connecting it again.";
		agk::Error( err );
#endif
		return false;
	}

	if ( !IP || strlen( IP ) >= 65 )
	{
#ifdef _AGK_ERROR_CHECK
		uString err( "Invalid IP address used to connect socket.", 50 );
		agk::Error( err );
#endif
		return false;
	}

	m_bASync = true;
	m_iTimeout = timeout;
	m_bConnecting = true;
	strcpy( m_szIP, IP );
	m_port = port;

	if ( m_cTimeout.IsRunning() ) 
	{
		m_cTimeout.Stop();
		m_cTimeout.Join();
	}
	m_cTimeout.SetData( this, timeout );
	m_cTimeout.Start();

	Start();

	return true;
}

bool AGKSocket::SendFile( const char* szFilename )
{
	if ( m_bDisconnected ) return false;
	if ( !szFilename ) return false;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to send file on an unconnected socket" );
		return false;
	}
	
	cFile pFile;	
	if ( !pFile.OpenToRead( szFilename ) )
	{
		agk::Warning( "Could not send network file" );
		return false;
	}
	
	UINT size = pFile.GetSize();
	
	SendUInt( size );
	Flush();
	
	if ( size == 0 ) return true;
	
	UINT iSent = 0;
	while ( !pFile.IsEOF() )
	{
		int written = pFile.ReadData( m_sendBuffer, AGK_NET_SEND_BUFFER_SIZE );
		if ( written == 0 ) break;

		int result = 0;
		int sent = 0;
		do
		{
			result = send( m_client, m_sendBuffer+sent, written-sent, 0 );
			sent += result;
		} while ( result > 0 && result != SOCKET_ERROR && sent < written );
		
		if ( result == 0 || result == SOCKET_ERROR ) 
		{
			pFile.Close();
			agk::Warning( "Failed to send socket file data" );
			m_bDisconnected = true;
			return false;
		}
		else
		{
			iSent += written;
			m_fProgress = iSent*100.0f / size;
			if ( m_fProgress > 100 ) m_fProgress = 100; 
		}
	}
	
	pFile.Close();
	return true;
}

bool AGKSocket::SendData( const char* s, int length )
{
	if ( m_bDisconnected ) return false;
	if ( length <= 0 ) return false;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to send data on an unconnected socket" );
		return false;
	}
	
	if ( length+m_iSendWritePtr > AGK_NET_SEND_BUFFER_SIZE )
	{
		// fill rest of the send buffer with some data
		int space = AGK_NET_SEND_BUFFER_SIZE - m_iSendWritePtr;
		if ( space > 0 ) memcpy( m_sendBuffer+m_iSendWritePtr, s, space );
		m_iSendWritePtr = AGK_NET_SEND_BUFFER_SIZE;

		Flush();
		if ( m_bDisconnected ) return false;
		
		UINT iSent = space;
		while ( length-iSent >= AGK_NET_SEND_BUFFER_SIZE )
		{
			int result = 0;
			unsigned int sent = 0;
			do
			{
				result = send( m_client, s+iSent+sent, AGK_NET_SEND_BUFFER_SIZE-sent, 0 );
				sent += result;
			} while ( result > 0 && result != SOCKET_ERROR && sent < AGK_NET_SEND_BUFFER_SIZE );

			if ( result == SOCKET_ERROR || result == 0 ) 
			{
				agk::Warning( "Failed to send socket data" );
				m_bDisconnected = true;
				return false;
			}
			
			iSent += result;
			
			m_fProgress = iSent*100.0f / length;
			if ( m_fProgress > 100 ) m_fProgress = 100;
		}
		
		// add rest of data to new buffer
		memcpy( m_sendBuffer, s+iSent, length-iSent );
		m_iSendWritePtr = length-iSent;
	}
	else
	{
		// add data to buffer
		memcpy( m_sendBuffer+m_iSendWritePtr, s, length );
		m_iSendWritePtr += length;
	}
	return true;
}

bool AGKSocket::SendString( const char *s )
{
	if ( m_bDisconnected ) return false;
	if ( !s ) return false;
	
	if ( !m_bConnected )
	{
		agk::Error( "Tried to send string on an unconnected socket" );
		return false;
	}
	
	UINT length = strlen( s );
	if ( !SendUInt( length ) ) return false;
	if ( length > 0 ) return SendData( s, length );

	return true;
}

bool AGKSocket::SendChar( char c )
{
	if ( m_bDisconnected ) return false;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to send char on an unconnected socket" );
		return false;
	}
	
	if ( m_iSendWritePtr+1 > AGK_NET_SEND_BUFFER_SIZE )
	{
		Flush();
	}
	
	// add char to buffer
	m_sendBuffer[ m_iSendWritePtr ] = c;
	m_iSendWritePtr += 1;
	return true;
}

bool AGKSocket::SendUInt( UINT u )
{
	if ( m_bDisconnected ) return false;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to send uint on an unconnected socket" );
		return false;
	}
	
	if ( m_iSendWritePtr+4 > AGK_NET_SEND_BUFFER_SIZE )
	{
		Flush();
	}
	
	// add char to buffer
	memcpy( m_sendBuffer + m_iSendWritePtr, &u, 4 ); // ARM does not like unaligned 4 byte access, so copy per byte
	m_iSendWritePtr += 4;
	return true;
}

bool AGKSocket::SendInt( int i )
{
	if ( m_bDisconnected ) return false;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to send int on an unconnected socket" );
		return false;
	}
	
	if ( m_iSendWritePtr+4 > AGK_NET_SEND_BUFFER_SIZE )
	{
		Flush();
	}
	
	// add char to buffer
	memcpy( m_sendBuffer + m_iSendWritePtr, &i, 4 ); // ARM does not like unaligned 4 byte access, so copy per byte
	m_iSendWritePtr += 4;
	return true;
}

bool AGKSocket::SendFloat( float f )
{
	if ( m_bDisconnected ) return false;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to send float on an unconnected socket" );
		return false;
	}
	
	if ( m_iSendWritePtr+4 > AGK_NET_SEND_BUFFER_SIZE )
	{
		Flush();
	}
	
	// add char to buffer
	memcpy( m_sendBuffer + m_iSendWritePtr, &f, 4 ); // ARM does not like unaligned 4 byte access, so copy per byte
	m_iSendWritePtr += 4;
	return true;
}

bool AGKSocket::Flush()
{
	if ( m_bDisconnected ) return false;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to flush data on an unconnected socket" );
		return false;
	}

	if ( m_iSendWritePtr > 0 )
	{
		int result = 0;
		unsigned int sent = 0;
		do
		{
			result = send( m_client, m_sendBuffer+sent, m_iSendWritePtr-sent, 0 );
			sent += result;
		} while ( result > 0 && result != SOCKET_ERROR && sent < m_iSendWritePtr );

		if ( result == 0 || result == SOCKET_ERROR ) 
		{
			uString err;
			err.Format( "Failed to flush socket data: %d", errno );
			agk::Warning( err );
			m_bDisconnected = true;
			return false;
		}
		m_iSendWritePtr = 0;
	}
	return true;
}

// if this is zero the recv commands will block
int AGKSocket::GetBytes()
{
	if ( m_bDisconnected ) return 0;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to get available bytes on an unconnected socket" );
		return 0;
	}
	
	unsigned long waiting = 0;
	if ( ioctl( m_client, FIONREAD, &waiting ) == SOCKET_ERROR )
	{
		agk::Warning( "Failed to get receivable bytes on socket" );
		m_bDisconnected = true;
		return 0;
	}
	
	return waiting;
}

// blocks until data is available.
// returns the number of bytes read, if this is zero the connection closed or an error occured.
int AGKSocket::RecvData( char* data, int length )
{
	if ( m_bDisconnected ) return 0;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to receive data on an unconnected socket" );
		return 0;
	}
	
	if ( !data || length <= 0 )
	{
		agk::Error( "Invalid network buffer passed to RecvData()" );
		return 0;
	}
	
	int result = recv( m_client, data, length, 0 );
	if ( result == SOCKET_ERROR || result == 0 )
	{
		agk::Warning( "Failed to get socket data" );
		m_bDisconnected = true;
		return 0;
	}
	return result;
}

// blocks until data is available.
// returns the number of characters read, not including the null terminator.
int AGKSocket::RecvString( uString &s )
{
	s.ClearTemp();
	if ( m_bDisconnected ) return 0;
	
	if ( !m_bConnected )
	{
		agk::Error( "Tried to receive string on an unconnected socket" );
		return -1;
	}
	
	// get string length
	UINT length = RecvUInt();
	if ( length == 0 ) return 0;
	
	// get string data
	char *buf = new char[ length ];
	int result = recv( m_client, buf, length, 0 );
	int sofar = result;
	while ( result != SOCKET_ERROR && result > 0 && sofar < (int)length )
	{
		s.AppendN( buf, result );
		result = recv( m_client, buf, length-sofar, 0 );
		if ( result > 0 ) sofar += result;
	}
	
	if ( result != SOCKET_ERROR && result > 0 ) 
	{
		s.AppendN( buf, result );
	}
	else m_bDisconnected = true;
	
	delete [] buf;
	return sofar;
}

// blocks until data is available
char AGKSocket::RecvChar()
{
	if ( m_bDisconnected ) return 0;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to receive char on an unconnected socket" );
		return 0;
	}
	
	char c = 0;
	int result = recv( m_client, &c, 1, 0 );
	if ( result == SOCKET_ERROR || result == 0 ) m_bDisconnected = true;
	return c;
}

// blocks until data is available
int AGKSocket::RecvInt()
{
	if ( m_bDisconnected ) return 0;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to receive int on an unconnected socket" );
		return 0;
	}
	
	int i = 0;
	int received = 0;
	while ( received < 4 )
	{
		int result = recv( m_client, ((char*)&i)+received, 4-received, 0 );
		if ( result == SOCKET_ERROR || result == 0 ) 
		{
			m_bDisconnected = true;
			return 0;
		}
		received += result;
	}

	return agk::PlatformLocalEndian( i );
}

// blocks until data is available
UINT AGKSocket::RecvUInt()
{
	if ( m_bDisconnected ) return 0;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to receive uint on an unconnected socket" );
		return 0;
	}

	UINT u = 0;
	int received = 0;
	while ( received < 4 )
	{
		int result = recv( m_client, ((char*)&u)+received, 4-received, 0 );
		if ( result == SOCKET_ERROR || result == 0 ) 
		{
			m_bDisconnected = true;
			return 0;
		}
		received += result;
	}

	return agk::PlatformLocalEndian( u );
}

// blocks until data is available
float AGKSocket::RecvFloat()
{
	if ( m_bDisconnected ) return 0;
	if ( !m_bConnected )
	{
		agk::Error( "Tried to receive float on an unconnected socket" );
		return 0;
	}

	float f = 0;
	int received = 0;
	while ( received < 4 )
	{
		int result = recv( m_client, ((char*)&f)+received, 4-received, 0 );
		if ( result == SOCKET_ERROR || result == 0 ) 
		{
			m_bDisconnected = true;
			return 0;
		}
		received += result;
	}

	return f;
}		


//**********************
// Network Listener
//**********************

UINT cNetworkListener::Run()
{
	while ( !m_bTerminate )
	{
		if ( m_socket == INVALID_SOCKET ) SleepSafe(1000);
		else
		{
			int client = accept( m_socket, NULL, NULL );
			if ( m_bTerminate ) return 0;
			if ( client == INVALID_SOCKET )
			{
				agk::Warning( "Failed to accept connection" );
				//m_socket = INVALID_SOCKET;
				//return 0;
				continue;
			}
			
			// don't wait to send packets (turns off nagle algorithm)
			int opt = 1;
			int optlen = sizeof(int);
			setsockopt( client, IPPROTO_TCP, TCP_NODELAY, (char*)&opt, optlen );
			
			// don't generate an exception when other end disconnects
			//opt = 1;
			//optlen = sizeof(int);
			//setsockopt( client, SOL_SOCKET, SO_NOSIGPIPE, (char*)&opt, optlen );

			AGKSocket *pAGKSocket = new AGKSocket( client );
			
			{
				cAutoLock autoLock(&m_lock); // will unlock when destroyed
				pAGKSocket->m_pNext = m_pConnections;
				m_pConnections = pAGKSocket;
			}
		}
	}

	return 0;
}

cNetworkListener::cNetworkListener()
{
	m_socket = INVALID_SOCKET;
	m_pConnections = 0;
	m_port = 0;
}

cNetworkListener::~cNetworkListener()
{
	Stop();
	Join();

	// delete waiting connections
	while ( m_pConnections )
	{
		AGKSocket *pSocket = m_pConnections;
		m_pConnections = m_pConnections->m_pNext;
		delete pSocket;
	}
}

void cNetworkListener::Stop()
{
	AGKThread::Stop();
	if ( m_socket != INVALID_SOCKET ) 
	{
		shutdown( m_socket, 2 );
		close( m_socket );
	}
	m_socket = INVALID_SOCKET;
}

AGKSocket* cNetworkListener::GetNewConnection()
{
	if ( !m_pConnections ) return 0;

	AGKSocket *pAGKSocket = 0;
	{
		cAutoLock autoLock(&m_lock); // will unlock when destroyed
		if ( !m_pConnections ) return 0;

		pAGKSocket = m_pConnections;
		m_pConnections = pAGKSocket->m_pNext;
	}

	pAGKSocket->m_pNext = 0;
	return pAGKSocket;
}

int cNetworkListener::AcceptConnections( UINT port )
{
	return AcceptConnections( "", port );
}

int cNetworkListener::AcceptConnections( const char *szIP, UINT port )
{
	if ( m_socket != INVALID_SOCKET ) 
	{
		uString err;
		err.Format( "Failed to listen on port %d, this socket is already listening on port %d", port, m_port );
		agk::Error( err );
		return 0;
	}

	if ( !szIP || !*szIP ) szIP = "anyip4";

	int family = 0;
	int size = 0;
	sockaddr_storage addr;
	if ( strcmp(szIP,"anyip6") == 0 || strchr(szIP,':') ) 
	{
		size = sizeof(sockaddr_in6);
		family = AF_INET6;
		sockaddr_in6 *addrv6 = (sockaddr_in6*) &addr;
		memset(addrv6, 0, sizeof(sockaddr_in6));
		addrv6->sin6_family = AF_INET6;
		addrv6->sin6_port = htons( port );
		if ( strcmp(szIP,"anyip6") == 0 ) addrv6->sin6_addr = in6addr_any;
		else inet_pton( AF_INET6, szIP, &(addrv6->sin6_addr) );
	}
	else
	{
		size = sizeof(sockaddr_in);
		family = AF_INET;
		sockaddr_in *addrv4 = (sockaddr_in*) &addr;
		addrv4->sin_family = AF_INET;
		addrv4->sin_port = htons( port );
		if ( strcmp(szIP,"anyip4") == 0 ) addrv4->sin_addr.s_addr = INADDR_ANY;
		else addrv4->sin_addr.s_addr = inet_addr( szIP );
	}

	m_socket = socket( family, SOCK_STREAM, IPPROTO_TCP );
	if ( m_socket == INVALID_SOCKET ) 
	{
		agk::Warning( "Failed to create listening socket" );
		return 0;
	}

	int value = 1;
	setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(value) );
	if ( family == AF_INET6 ) setsockopt( m_socket, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&value, sizeof(value) ); // don't listen on IPV4 addresses

	if ( bind( m_socket, (sockaddr*) &addr, size ) == SOCKET_ERROR )
	{
		agk::Warning( "Failed to bind listening socket" );
		close( m_socket );
		m_socket = INVALID_SOCKET;
		return 0;
	}

	m_port = port;

	if ( listen( m_socket, 5 ) == SOCKET_ERROR )
	{
		agk::Warning( "Failed to start socket listening" );
		close( m_socket );
		m_socket = INVALID_SOCKET;
		return 0;
	}

	Start();

	return 1;
}



//************************
// Broadcast Listener
//************************

BroadcastListener::BroadcastListener()
{
	m_socket = INVALID_SOCKET;
}

BroadcastListener::~BroadcastListener()
{
	if ( m_socket != INVALID_SOCKET ) 
	{
		shutdown( m_socket, 2 );
		close( m_socket );
	}
}

void BroadcastListener::Close()
{
	if ( m_socket != INVALID_SOCKET ) 
	{
		shutdown( m_socket, 2 );
		close( m_socket );
	}
	m_socket = INVALID_SOCKET;
}

bool BroadcastListener::SetListenPort( UINT port )
{
	return SetListenPort( "", port );
}

bool BroadcastListener::SetListenPort( const char *szIP, UINT port )
{
	if ( m_socket != INVALID_SOCKET ) 
	{
		shutdown( m_socket, 2 );
		close( m_socket );
	}

	if ( !szIP || !*szIP ) szIP = "anyip4";
	if ( strcmp( szIP, "anyip6" ) == 0 ) szIP = "FF02::41:474B"; // if they have selected any then join link local AGK multicast address

	int family = 0;
	int size = 0;
	sockaddr_storage addr;
	if ( strcmp(szIP,"anyip6") == 0 || strchr(szIP,':') ) 
	{
		size = sizeof(sockaddr_in6);
		family = AF_INET6;
		sockaddr_in6 *addrv6 = (sockaddr_in6*) &addr;
		memset(addrv6, 0, sizeof(sockaddr_in6));
		addrv6->sin6_family = AF_INET6;
		addrv6->sin6_port = htons( port );
		if ( strncmp(szIP,"FF",2) == 0 || strncmp(szIP,"ff",2) == 0 ) addrv6->sin6_addr = in6addr_any;
		else inet_pton( AF_INET6, szIP, &(addrv6->sin6_addr) );
	}
	else
	{
		size = sizeof(sockaddr_in);
		family = AF_INET;
		sockaddr_in *addrv4 = (sockaddr_in*) &addr;
		addrv4->sin_family = AF_INET;
		addrv4->sin_port = htons( port );
		if ( strcmp(szIP,"anyip4") == 0 ) addrv4->sin_addr.s_addr = INADDR_ANY;
		else addrv4->sin_addr.s_addr = inet_addr( szIP );
	}

	m_socket = socket( family, SOCK_DGRAM, IPPROTO_UDP );
	if ( m_socket == INVALID_SOCKET ) 
	{
		agk::Warning( "Failed to create listening broadcast socket" );
		return false;
	}
	
	int value = 1;
    setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(value) );

	if ( family == AF_INET6 ) 
	{
		int value = 1;
		setsockopt( m_socket, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&value, sizeof(value) ); // don't listen on IPV4 addresses

		// if address is multicast then join the group
		if ( strncmp(szIP,"FF",2) == 0 || strncmp(szIP,"ff",2) == 0 ) 
		{
			ipv6_mreq mreq;
			mreq.ipv6mr_interface = 0;
			inet_pton( AF_INET6, szIP, &(mreq.ipv6mr_multiaddr) );
			setsockopt(m_socket, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char*)&mreq, sizeof(mreq)) ;
		}
	}

	if ( bind( m_socket, (sockaddr*) &addr, size ) == SOCKET_ERROR )
	{
		agk::Warning( "Failed to bind listening socket" );
		return false;
	}

	return true;
}

bool BroadcastListener::ReceivedBroadcast()
{
	if ( m_socket == INVALID_SOCKET ) return false;
	
	unsigned long waiting = 0;
	if ( ioctl( m_socket, FIONREAD, &waiting ) == SOCKET_ERROR ) return false;
	return waiting > 0;
}

bool BroadcastListener::GetPacket( AGKPacket &packet, UINT &fromPort, char *fromIP )
{
	if ( m_socket == INVALID_SOCKET ) return false;
	
	socklen_t length = sizeof(addr);
	int result = recvfrom( m_socket, packet.GetRaw(), AGK_NET_PACKET_SIZE, 0, (sockaddr*)&addr, &length );
	if ( result == SOCKET_ERROR )
	{
		agk::Warning( "Failed to receive broadcast" );
		return false;
	}
	if ( result == 0 ) 
	{
		return false;
	}
	
	packet.SetPos( 0 );
	
	if ( result > 0 ) 
	{
		if ( addr.ss_family == AF_INET6 )
		{
			sockaddr_in6 *addrv6 = (sockaddr_in6*) &addr;
			inet_ntop( AF_INET6, &(addrv6->sin6_addr), fromIP, 65 );
			fromPort = ntohs(addrv6->sin6_port);
		}
		else
		{
			sockaddr_in *addrv4 = (sockaddr_in*) &addr;
			strcpy( fromIP, inet_ntoa( addrv4->sin_addr ) );
			fromPort = ntohs(addrv4->sin_port);
		}
	}
	
	return true;
}

//*****************************
// Broadcaster
//*****************************

UINT Broadcaster::Run()
{
	int count = 0;
	int sock = 0;

	if ( m_ipv6 )
	{
		sock = socket( AF_INET6, SOCK_DGRAM, IPPROTO_UDP );
		if ( sock == INVALID_SOCKET ) 
		{
			agk::Warning( "Failed to create broadcast packet for IPv6" );
			return 0;
		}
	}
	else
	{
		sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
		if ( sock == INVALID_SOCKET ) 
		{
			agk::Warning( "Failed to create broadcast packet for IPv4" );
			return 0;
		}
	}

	int opt = 1;
	int optlen = sizeof(int);
	if ( m_ipv6 ) 
	{
        int interface = agk::GetNetworkInterface();        
        setsockopt( sock, IPPROTO_IPV6, IPV6_MULTICAST_IF, &interface, sizeof(interface) );
        
		sockaddr_in6 addr;
		memset(&addr, 0, sizeof(sockaddr_in6));
		addr.sin6_family = AF_INET6;
		addr.sin6_port = htons( m_port );
		inet_pton( AF_INET6, "FF02::41:474B", &(addr.sin6_addr) ); // link local AGK multicast address
		int result = connect( sock, (sockaddr*) &addr, sizeof(addr) );
		if ( result == SOCKET_ERROR )
		{
			agk::Warning( "Failed to set broadcast packet address for IPv6" );
			close( sock );
			sock = INVALID_SOCKET;
			return 0;
		}
	}
	else
	{
		setsockopt( sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, optlen );
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons( m_port );
		addr.sin_addr.s_addr = inet_addr( "255.255.255.255" );
		int result = connect( sock, (sockaddr*) &addr, sizeof(addr) );
		if ( result == SOCKET_ERROR )
		{
			agk::Warning( "Failed to set broadcast packet address for IPv4" );
			close( sock );
			sock = INVALID_SOCKET;
			return 0;
		}
	}

	do 
	{
		int result = 0;
		unsigned int sent = 0;
		do
		{
			result = send( sock, m_packet.GetBuffer()+sent, m_packet.GetPos()-sent, 0 );
			sent += result;
		} while ( result > 0 && result != SOCKET_ERROR && sent < m_packet.GetPos() );

		if ( result == SOCKET_ERROR )
		{
			agk::Warning( "Failed to send broadcast packet" );
			close( sock );
			sock = INVALID_SOCKET;
			return 0;
		}

		// sleep for interval milliseconds unless woken by the stop event
		SleepSafe( m_interval );
		if ( m_bTerminate ) 
		{
			break;
		}

		count++;
	} while ( m_max == 0 || count < m_max );

	shutdown( sock, 2 );
	close( sock );

	return 0;
}

Broadcaster::Broadcaster()
{
	m_interval = 1000;
	m_max = 0;
}

// sets the data that will be broadcast every interval milliseconds for a maximum of max broadcasts (0=forever)
// start with Start() and stop with Stop()
void Broadcaster::SetData( int ipv6, UINT port, const AGKPacket* packet, UINT interval, int max )
{
	if ( !packet ) return;
	if ( interval < 1000 ) interval = 1000; //minimum interval of 1 seond to stop flooding the network
	if ( packet->GetPos() > 512 )
	{
		agk::Error( "Attempted to broadcast more than 512 bytes" );
		return;
	}
	
	m_ipv6 = ipv6;
	m_port = port;
	m_interval = interval;
	m_packet.Copy( packet );
	m_max = max;
}

//************************
// HTTPConnection
//************************

cHTTPConnection::cHTTPConnection()
{
	request = curl_easy_init();
	m_iSecure = 0;
	m_bConnected = false;
	m_iTimeout = 6000;
	m_iVerifyMode = 1;
	
	m_fProgress = 0;
	m_iStatusCode = 0;

	m_bSaveToFile = false;
	m_iSent = 0;
	m_pFile = 0;
	m_iReceived = 0;
	m_pUploadFile = 0;
	m_iSendLength = 0;
	m_bFailed = false;
	
	m_szContentType[0] = '\0';
}

cHTTPConnection::~cHTTPConnection()
{
	Close();
	curl_easy_cleanup(request);

	cHTTPHeader *pHeader = m_cHeaders.GetFirst();
	while( pHeader )
	{
		delete pHeader;
		pHeader = m_cHeaders.GetNext();
	}
	m_cHeaders.ClearAll();
}

UINT cHTTPConnection::Run()
{
	if ( m_szUploadFile.GetLength() > 0 ) SendFileInternal();
	else SendRequestInternal();
	return 0;
}

void cHTTPConnection::Stop()
{
	AGKThread::Stop();

	// force any existing connection to time out
	curl_easy_setopt( request, CURLOPT_CONNECTTIMEOUT_MS, m_iTimeout );

	m_bConnected = false;
}

void cHTTPConnection::SetTimeout( int milliseconds )
{
	if ( milliseconds < 0 ) milliseconds = 0;
	m_iTimeout = milliseconds;
}

void cHTTPConnection::SetVerifyCertificate( int mode )
{
	m_iVerifyMode = mode;
}

void cHTTPConnection::AddHeader( const char* headerName, const char* headerValue )
{
	if ( IsRunning() )
	{
		agk::Warning( "Cannot change HTTP headers whilst an async request or download is still in progress, wait for GetRepsonseReady() or DownloadComplete() to return 1" );
		return;
	}

	cHTTPHeader *pHeader = m_cHeaders.GetItem( headerName );
	if ( !pHeader )
	{
		pHeader = new cHTTPHeader();
		pHeader->sName.SetStr( headerName );
		m_cHeaders.AddItem( pHeader, headerName );
	}

	pHeader->sValue.SetStr( headerValue );
}

void cHTTPConnection::RemoveHeader( const char* headerName )
{
	if ( IsRunning() )
	{
		agk::Warning( "Cannot change HTTP headers whilst an async request or download is still in progress, wait for GetRepsonseReady() or DownloadComplete() to return 1" );
		return;
	}

	cHTTPHeader *pHeader = m_cHeaders.RemoveItem( headerName );
	if ( pHeader ) delete pHeader;
}

bool cHTTPConnection::SetHost( const char *szHost, int iSecure, const char *szUser, const char *szPass )
{
	m_sHost.SetStr( szHost );
	if ( iSecure == 0 ) m_sHost.Prepend( "http://" );
	else m_sHost.Prepend( "https://" );
	
	m_iSecure = iSecure;

	if ( szUser ) m_sUsername.SetStr( szUser );
	else m_sUsername.SetStr( "" );

	if ( szPass ) m_sPassword.SetStr( szPass );
	else m_sPassword.SetStr( "" );
	
	return true;
}

void cHTTPConnection::Close()
{
	Stop();
	Join();
	
	m_sHost.SetStr( "" );
	curl_easy_reset( request );
	
	if ( m_pFile ) 
	{
		delete m_pFile;
	}
	m_pFile = 0;
}

size_t httprecvfunc( char *ptr, size_t size, size_t nmemb, void *userdata)
{
	cHTTPConnection* http = (cHTTPConnection*)userdata; 
	return http->RecvData( ptr, size*nmemb );
}

size_t httpsendfunc( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	cHTTPConnection* http = (cHTTPConnection*)userdata; 
	return http->SendData( ptr, size*nmemb );
}

int cHTTPConnection::RecvData( void* buf, int size )
{
	if ( m_bTerminate ) 
	{
		m_bFailed = true;
		return 0;
	}
	
	double totalLength = 0;
	curl_easy_getinfo( request, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &totalLength );
	
	m_iReceived += size;
	if ( totalLength > 0 )
	{
		m_fProgress = m_iReceived*100.0f / totalLength;
	}
	
	if ( m_bSaveToFile )
	{
		if ( m_pFile ) m_pFile->WriteData( (const char*)buf, size );
		else 
		{
			m_bFailed = true;
			return 0;
		}
	}
	else m_sResponse.AppendN( (const char*)buf, size );
	
	return size;
}

int cHTTPConnection::SendData( void* buf, int size )
{
	if ( !m_pUploadFile ) 
	{
		m_bFailed = true;
		return 0;
	}
	if ( m_bTerminate ) 
	{
		m_bFailed = true;
		return 0;
	}
	if ( m_iSent >= m_iSendLength ) return 0;
	
	if ( m_iSent + size > m_iSendLength ) size = m_iSendLength - m_iSent;
	
	m_pUploadFile->ReadData( (char*)buf, size );
	
	m_iSent += size;
	if ( m_iSendLength > 0 )
	{
		m_fProgress = m_iSent*100.0f / m_iSendLength;
	}
	
	return size;
}

void cHTTPConnection::SendRequestInternal()
{
	m_sResponse.SetStr( "" );
	m_bFailed = false;
	m_iReceived = 0;
	m_iSent = 0;
	m_fProgress = 0;
	m_iStatusCode = 0;
	
	if ( m_szServerFile.GetLength() == 0 ) 
	{
		m_bFailed = true;
		return;
	}
		
	if ( m_bSaveToFile )
	{
		if ( !m_szLocalFile ) 
		{
#ifdef _AGK_ERROR_CHECK
			agk::Warning( "Cannot download file, no local filename set." );
#endif
			m_bFailed = true;
			return;
		}

		if ( m_pFile ) delete m_pFile;
		m_pFile = new cFile();
		if ( !m_pFile->OpenToWrite( m_szLocalFile ) )
		{
#ifdef _AGK_ERROR_CHECK
			agk::Warning( "Cannot download file, failed to open local file for writing." );
#endif
			delete m_pFile;
			m_pFile = 0;
			m_bFailed = true;
			return;
		}
	}

	curl_slist *slist = NULL;
	uString sHeader;
	cHTTPHeader *pHeader = m_cHeaders.GetFirst();
	while( pHeader )
	{
		sHeader.Format( "%s: %s", pHeader->sName.GetStr(), pHeader->sValue.GetStr() );
		slist = curl_slist_append( slist, sHeader.GetStr() );
		pHeader = m_cHeaders.GetNext();
	}
	
	uString sURL( m_sHost );
	sURL.Append( "/" );
	sURL.Append( m_szServerFile );
	
	curl_easy_reset( request );
	curl_easy_setopt( request, CURLOPT_HTTPHEADER, slist );
	curl_easy_setopt( request, CURLOPT_URL, sURL.GetStr() );
	curl_easy_setopt( request, CURLOPT_FOLLOWLOCATION, 1 );
	curl_easy_setopt( request, CURLOPT_WRITEFUNCTION, httprecvfunc );
	curl_easy_setopt( request, CURLOPT_WRITEDATA, this );
	curl_easy_setopt( request, CURLOPT_SSL_VERIFYPEER, m_iVerifyMode ? 1 : 0 );
	curl_easy_setopt( request, CURLOPT_SSL_VERIFYHOST, m_iVerifyMode ? 2 : 0 );
	curl_easy_setopt( request, CURLOPT_COOKIEFILE, "" );
	curl_easy_setopt( request, CURLOPT_CONNECTTIMEOUT, m_iTimeout/1000 );
	curl_easy_setopt( request, CURLOPT_NOPROGRESS, 1 );
	
	if ( m_sUsername.GetLength() > 0 && m_sPassword.GetLength() > 0 )
	{
		curl_easy_setopt( request, CURLOPT_USERNAME, m_sUsername.GetStr() );
		curl_easy_setopt( request, CURLOPT_PASSWORD, m_sPassword.GetStr() );
	}
	
	if ( m_szPostData.GetLength() > 0 )
	{
		curl_easy_setopt( request, CURLOPT_POST, 1 );
		curl_easy_setopt( request, CURLOPT_POSTFIELDS, m_szPostData.GetStr() );
	}
	
	CURLcode result = curl_easy_perform( request );
	if ( result != 0 )
	{
		uString err;
		err.Format( "Failed to send HTTP request, code: %d", result );
		agk::Warning( err );
		if ( m_pFile ) 
		{
			delete m_pFile;
			m_pFile = 0;
		}
		m_bFailed = true;
		curl_slist_free_all(slist);
		return;
	}

	long status = 0;
	curl_easy_getinfo( request, CURLINFO_RESPONSE_CODE, &status );
	m_iStatusCode = status;
	
	char *szType = 0;
	curl_easy_getinfo( request, CURLINFO_CONTENT_TYPE, &szType );
	if ( szType && strlen(szType) < 150 )
	{
		strcpy( m_szContentType, szType );
	}

	if ( slist ) curl_slist_free_all(slist);
	
	if ( m_pFile ) 
	{
		delete m_pFile;
		m_pFile = 0;
	}
}

void cHTTPConnection::SendFileInternal()
{
	m_sResponse.SetStr( "" );
	m_bFailed = false;
	m_iReceived = 0;
	m_iSent = 0;
	m_fProgress = 0;
	m_iStatusCode = 0;
	m_iSendLength = 0;
	
	if ( m_szUploadFile.GetLength() == 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Warning( "Cannot send HTTP file as no upload file name has been set" );
#endif
		m_bFailed = true;
		return;
	}

	if ( m_szServerFile.GetLength() == 0 ) 
	{
		m_bFailed = true;
		return;
	}
	
	cFile uploadFile;
	if ( !uploadFile.OpenToRead( m_szUploadFile ) )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Warning( "Cannot send HTTP file, failed to open file for reading" );
#endif
		m_bFailed = true;
		return;
	}
		
	// create temp file that will stream the http body
	int random = agk::Random();
	int random2 = agk::Random();
	m_sRndFilename.Format("/uploadtemp%d-%d",random,random2);
	
	if ( m_pUploadFile ) delete m_pUploadFile;
	m_pUploadFile = new cFile();
	m_pUploadFile->OpenToWrite( m_sRndFilename );
	
	uString sFinalPostData;
	
	// parse the post variables into a file upload friendly format.
	char sName[ 256 ];
	char sValue[ 512 ];
	const char* remaining = m_szPostData.GetStr();
	int count = m_szPostData.Count( (int)'&' ) + 1;
	for ( int i = 0; i < count; i++ )
	{
		int pos = strcspn( remaining, "=" );
		strncpy( sName, remaining, pos ); 
		sName[ pos ] = '\0';
		remaining += pos+1;
		pos = strcspn( remaining, "&" );
		strncpy( sValue, remaining, pos ); 
		sValue[ pos ] = '\0';
		remaining += pos+1;
		
		if ( strlen( sName ) == 0 || strlen( sValue ) == 0 ) continue;
		
		sFinalPostData.Append( "--------------------AaB03x\r\nContent-Disposition: form-data; name=\"" );
		sFinalPostData.Append( sName );
		sFinalPostData.Append( "\"\r\n\r\n" );
		sFinalPostData.Append( sValue );
		sFinalPostData.Append( "\r\n" );
	}
	
	// extract filename from the path
	uString uploadFilename( m_szUploadFile );
	uploadFilename.Replace( (int)'\\', (int)'/' );
	uString sFilename;
	int pos = uploadFilename.RevFind( (int)'/' );
	if ( pos >= 0 ) uploadFilename.SubString( sFilename, pos+1 );
	else sFilename.SetStr( uploadFilename );
	
	// add file variable to post data
	sFinalPostData.Append( "--------------------AaB03x\r\nContent-Disposition: form-data; name=\"myfile\"; filename=\"" );
	sFinalPostData.Append( sFilename );
	sFinalPostData.Append( "\"\r\nContent-Type: application/x-zip-compressed\r\n\r\n" );
	
	// write the initial data to the temp file
	m_pUploadFile->WriteData( sFinalPostData.GetStr(), sFinalPostData.GetLength() );
	
	// copy upload file to temp file
	char buf[5000];
	int written = 0;
	do 
	{
		written = uploadFile.ReadData(buf, 5000);
		if ( written > 0 ) m_pUploadFile->WriteData(buf, written);
	} while ( written > 0 && !uploadFile.IsEOF() );
	uploadFile.Close();
	
	// finialise the http body data
	uString sEndPostData( "\r\n--------------------AaB03x--" );
	m_pUploadFile->WriteData(sEndPostData, sEndPostData.GetLength());
	m_pUploadFile->Close();
	
	int length = cFile::GetFileSize(m_sRndFilename);
	curl_slist *slist = NULL;
	uString sHeader;
	cHTTPHeader *pHeader = m_cHeaders.GetFirst();
	while( pHeader )
	{
		sHeader.Format( "%s: %s", pHeader->sName.GetStr(), pHeader->sValue.GetStr() );
		slist = curl_slist_append( slist, sHeader.GetStr() );
		pHeader = m_cHeaders.GetNext();
	}
	slist = curl_slist_append(slist, "Content-Type: multipart/form-data; boundary=------------------AaB03x");
	m_iSendLength = length;
	
	m_pUploadFile->OpenToRead( m_sRndFilename );
	
	uString sURL( m_sHost );
	sURL.Append( "/" );
	sURL.Append( m_szServerFile );
	
	curl_easy_reset( request );
	curl_easy_setopt( request, CURLOPT_HTTPHEADER, slist );
	curl_easy_setopt( request, CURLOPT_URL, sURL.GetStr() );
	curl_easy_setopt( request, CURLOPT_FOLLOWLOCATION, 1 );
	curl_easy_setopt( request, CURLOPT_WRITEFUNCTION, httprecvfunc );
	curl_easy_setopt( request, CURLOPT_WRITEDATA, this );
	curl_easy_setopt( request, CURLOPT_SSL_VERIFYPEER, m_iVerifyMode ? 1 : 0 );
	curl_easy_setopt( request, CURLOPT_SSL_VERIFYHOST, m_iVerifyMode ? 2 : 0 );
	curl_easy_setopt( request, CURLOPT_COOKIEFILE, "" );
	curl_easy_setopt( request, CURLOPT_CONNECTTIMEOUT, m_iTimeout/1000 );
	curl_easy_setopt( request, CURLOPT_NOPROGRESS, 1 );
	
	curl_easy_setopt( request, CURLOPT_POST, 1 );
	curl_easy_setopt( request, CURLOPT_POSTFIELDSIZE, length );
	curl_easy_setopt( request, CURLOPT_READFUNCTION, httpsendfunc );
	curl_easy_setopt( request, CURLOPT_READDATA, this );

	if ( m_sUsername.GetLength() > 0 && m_sPassword.GetLength() > 0 )
	{
		curl_easy_setopt( request, CURLOPT_USERNAME, m_sUsername.GetStr() );
		curl_easy_setopt( request, CURLOPT_PASSWORD, m_sPassword.GetStr() );
	}
		
	CURLcode result = curl_easy_perform( request );
	if ( result != 0 )
	{
		uString err;
		err.Format( "Failed to send HTTP request, code: %d", result );
		agk::Warning( err );
		if ( m_pUploadFile ) 
		{
			delete m_pUploadFile;
			m_pUploadFile = 0;
		}
		m_bFailed = true;
		curl_slist_free_all(slist);
		return;
	}

	long status = 0;
	curl_easy_getinfo( request, CURLINFO_RESPONSE_CODE, &status );
	m_iStatusCode = status;
		
	char *szType = 0;
	curl_easy_getinfo( request, CURLINFO_CONTENT_TYPE, &szType );
	if ( szType && strlen(szType) < 150 )
	{
		strcpy( m_szContentType, szType );
	}
	
	curl_slist_free_all(slist);
	
	if ( m_pUploadFile ) 
	{
		delete m_pUploadFile;
		m_pUploadFile = 0;
	}
	
	if ( m_sRndFilename.GetLength() > 0 ) agk::DeleteFile( m_sRndFilename );
	m_sRndFilename.SetStr( "" );
}

char* cHTTPConnection::SendRequest( const char *szServerFile, const char *szPostData )
{
	if ( !szServerFile ) return 0;
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Warning( "Cannot send HTTP whilst an async request or download is still in progress, wait for GetRepsonseReady() or DownloadComplete() to return 1" );
#endif
		return NULL;
	}
	
	m_sResponse.SetStr( "" );
	m_bFailed = false;
	m_szServerFile.SetStr( szServerFile );
	m_szPostData.SetStr( szPostData );
	m_szUploadFile.SetStr( "" );
	m_bSaveToFile = false;
		
	SendRequestInternal();
	
	char *str = new char[ m_sResponse.GetLength() + 1 ];
	strcpy( str, m_sResponse.GetStr() );
	return str;
}

bool cHTTPConnection::SendRequestASync( const char *szServerFile, const char *szPostData )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Warning( "Cannot send HTTP whilst an async request or download is still in progress, wait for GetRepsonseReady() or DownloadComplete() to return 1" );
#endif
		return false;
	}

	m_sResponse.SetStr( "" );
	m_bFailed = false;
	m_fProgress = 0;
	m_iStatusCode = 0;
	m_szServerFile.SetStr( szServerFile );
	m_szPostData.SetStr( szPostData );
	m_szUploadFile.SetStr( "" );
	m_bSaveToFile = false;

	Start();

	return true;
}

bool cHTTPConnection::SendFile( const char *szServerFile, const char *szPostData, const char *szLocalFile )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Warning( "Cannot send HTTP file whilst an async request or download is still in progress, wait for GetRepsonseReady() or DownloadComplete() to return 1" );
#endif
		return false;
	}

	if ( !szLocalFile || !*szLocalFile )
	{
		agk::Error( "Cannot send HTTP file, filename is empty" );
		return false;
	}

	m_sResponse.SetStr( "" );
	m_bFailed = false;
	m_fProgress = 0;
	m_iStatusCode = 0;
	m_szServerFile.SetStr( szServerFile );
	m_szLocalFile.SetStr( "" );
	m_szPostData.SetStr( szPostData );
	m_szUploadFile.SetStr( szLocalFile );
	m_bSaveToFile = false;

	Start();
		
	return true;
}

int cHTTPConnection::GetResponseReady()
{
	if ( IsRunning() ) return 0;
	else if ( !m_bFailed ) return 1;
	else return -1;
}

const char* cHTTPConnection::GetResponse()
{
	return m_sResponse.GetStr();
}

const char* cHTTPConnection::GetContentType()
{
	return m_szContentType;
}

bool cHTTPConnection::DownloadFile( const char *szServerFile, const char *szLocalFile, const char *szPostData )
{
	if ( IsRunning() )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Warning( "Cannot send HTTP whilst an async request or download is still in progress, wait for GetRepsonseReady() or DownloadComplete() to return 1" );
#endif
		return false;
	}

	m_sResponse.SetStr( "" );
	m_bFailed = false;
	m_fProgress = 0;
	m_iStatusCode = 0;
	m_szServerFile.SetStr( szServerFile );
	m_szLocalFile.SetStr( szLocalFile );
	m_szPostData.SetStr( szPostData );
	m_szUploadFile.SetStr( "" );
	m_bSaveToFile = true;

	Start();
	
	return true;
}

bool cHTTPConnection::DownloadComplete()
{
	return !(IsRunning());
}
