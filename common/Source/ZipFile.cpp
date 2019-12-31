#include "agk.h"

#include "zip.h"
#include "unzip.h"
#ifdef _WIN32
    #define USEWIN32IOAPI
    #include "iowin32.h"
#endif

namespace AGK
{
	ZipExtracter g_ZipExtracter;
}

using namespace AGK;

UINT ZipExtracter::Run()
{
#if defined(AGK_IOS) || defined(AGK_MACOS)
	@autoreleasepool 
#endif
	{
		if ( m_sFilename.GetLength() == 0 ) return 0;

		ZipFile::ExtractAll( m_sFilename.GetStr(), m_sExtractPath.GetStr(), m_sPassword.GetLength() > 0 ? m_sPassword.GetStr() : NULL, &m_fProgress, &m_bTerminate );

		m_sFilename.SetStr("");
    }
	return 0;
}

ZipFile::ZipFile()
{
	m_zf = 0;
}

ZipFile::~ZipFile()
{
	if ( m_zf ) zipClose( m_zf, NULL ); // fixes crashing in T1 app // zeroing the m_zf parameter in Close() should now fix this.
	m_zf = 0;
}

void ZipFile::Create( const char* filename )
{
	uString sPath( filename );
	if ( strncmp(filename, "raw:", 4) == 0 ) sPath.SetStr( filename+4 );
	else agk::PlatformGetFullPathWrite(sPath);

	if ( !agk::PlatformCreateRawPath( sPath ) ) return;
	
	m_zf = zipOpen( sPath.GetStr(), 0 );
	
	if ( !m_zf ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to open zip file" );
#endif
		return;
	}
}

bool ZipFile::AddEntry( const char* realPath, const char* zipPath )
{
	return AddEntry( realPath, zipPath, -1 );
}

bool ZipFile::AddEntry( const char* realPath, const char* zipPath, int compressionlevel )
{
	if ( compressionlevel < -1 ) compressionlevel = -1;
	if ( compressionlevel > 9 ) compressionlevel = 9;

	const char *ext = strrchr( realPath, '.' );
	if ( ext )
	{
		uString sExt( ext );
		sExt.Lower();
		if ( sExt.CompareTo( ".mp3" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".m4a" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".jpg" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".png" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".gif" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".wav" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".ogg" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".mpg" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".mpeg" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".mp4" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".m4v" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".dat" ) == 0 ) compressionlevel = 0;
		if ( sExt.CompareTo( ".zip" ) == 0 ) compressionlevel = 0;
	}

	if ( !m_zf ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to add item to zip file, zip file has not been created" );
#endif
		return false;
	}

	cFile fFile;
	bool result = fFile.OpenToRead( realPath );
	if ( !result )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to add item to zip file, item could not be opened for reading" );
#endif
		return false;		
	}

	int err = zipOpenNewFileInZip( m_zf, zipPath, 0, 0, 0, 0, 0, 0, compressionlevel != 0 ? Z_DEFLATED : 0, compressionlevel );
	if (err != ZIP_OK)
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to add item to zip file, item could not be opened for reading" );
#endif
	}

	char buf[ 16000 ];
	while ( !fFile.IsEOF() )
	{
		int length = fFile.ReadData( buf, 16000 );
		if ( length > 0 )
		{
			err = zipWriteInFileInZip( m_zf, buf, length );
			if ( err < 0 ) 
			{
#ifdef _AGK_ERROR_CHECK
				agk::Error( "Failed to write data stream to zip file" );
#endif		
				break;
			}
		}
	}

	fFile.Close();
	zipCloseFileInZip( m_zf );

	return true;
}

void ZipFile::Close()
{
	if ( !m_zf ) return;
	zipClose( m_zf, NULL );
	m_zf = 0; // should fix the crash bug
}

bool ZipFile::ExtractAll( const char* filename, const char* extractPath, const char* password, volatile float* progress, volatile bool *stop)
{
	if ( progress ) *progress = 0;

	uString sPath( filename );
	if ( cFile::ExistsRaw(filename) ) sPath.SetStr( filename+4 );
	else if ( cFile::ExistsWrite(filename) ) agk::PlatformGetFullPathWrite(sPath);
	else
	{
		if ( cFile::ExistsRead( filename ) && !cFile::ExistsWrite( filename ) )
#ifdef AGK_ANDROID
		{
			cFile oFile;
			if ( !oFile.OpenToRead( filename ) )
			{
				uString err( "Failed to extract zip file - Could not open Android zip file:" );
				err.Append(filename);
				agk::Error( err );
				return false;
			}
			
			int size = oFile.GetSize();
			unsigned char *data = new unsigned char[ size ];
			oFile.ReadData( (char*)data, size );
			oFile.Close();

			if ( !oFile.OpenToWrite( filename ) )
			{
				delete [] data;
				uString err( "Failed to extract zip file - Could not open write Android zip file:" );
				err.Append(filename);
				agk::Error( err );
				return false;
			}

			oFile.WriteData( (const char*)data, size );
			oFile.Close();
			delete [] data;

			agk::PlatformGetFullPathWrite(sPath);
		}
#else
		{
			agk::PlatformGetFullPathRead( sPath );
		}
#endif
		else
		{
			uString err; err.Format( "Failed to extract zip file - Could not find file at path: %s", filename );
			agk::Error( err );
			return false;
		}
	}

	if ( stop && *stop ) return false;

	if ( progress ) *progress = 1;
		
	unzFile uf = unzOpen( sPath.GetStr() );
	if ( !uf )
	{
        uString sBigError( "Failed to extract zip file - Could not open the zip file: " );
        sBigError.Append(sPath);
		agk::Error( sBigError );
		return false;
	}

	unz_global_info64 gi;
    int err = unzGetGlobalInfo64(uf,&gi);
    if (err != UNZ_OK)
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to extract zip file - File may be corrupt" );
#endif		
		unzClose(uf);
		return false;
	}

	uString sWritePath;
	char buf[ 16000 ];

    for ( int i=0; i < gi.number_entry; i++ )
    {
		if ( progress ) *progress = (i / (float)gi.number_entry) * 98 + 2;

		unz_file_info64 file_info;
		char szFileNameInZip[ 512 ];
        err = unzGetCurrentFileInfo64( uf, &file_info, szFileNameInZip, 512, NULL,0,NULL,0 );
		if (err == UNZ_OK)
        {
            err = unzOpenCurrentFilePassword( uf, password );
			if ( err == UNZ_OK )
			{
				sWritePath.SetStr( extractPath );
				sWritePath.Replace( '\\', '/' );
				if ( sWritePath.GetLength() > 0 && sWritePath.CharAt( sWritePath.GetLength()-1 ) != '/' ) sWritePath.AppendAscii( '/' );
				sWritePath.Append( szFileNameInZip );

				// don't handle folders, they will be created if any files exist inside them
				if ( sWritePath.CharAt( sWritePath.GetLength()-1 ) != '/' )
				{
					cFile fFile;
					if ( !fFile.OpenToWrite( sWritePath ) )
					{
	#ifdef _AGK_ERROR_CHECK
						agk::Error( "Failed to extract zip file - Could not write extracted file" );
	#endif	
					}
					else
					{
						int readbytes = 0;
						do
						{
							readbytes = unzReadCurrentFile( uf, buf, 16000 );
							if ( readbytes > 0 )
							{
								fFile.WriteData( buf, readbytes );
							}
						} while( readbytes > 0 );

						fFile.Close();
					}
				}
			}

			err = unzCloseCurrentFile(uf);
        }
		
        if ((i+1)<gi.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err != UNZ_OK)
            {
#ifdef _AGK_ERROR_CHECK
				agk::Error( "Failed to extract zip file - Error moving to the next zip entry" );
#endif		
                break;
            }
        }

		if ( stop && *stop ) 
		{
			unzClose(uf);
			return false;
		}
    }

	unzClose(uf);

	if ( progress ) *progress = 100;

	return true;
}
