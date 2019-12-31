//#include "cText.h"
//#include "cSprite.h"
//#include "Wrapper.h"
#include "agk.h"

namespace AGK
{

// ASCII new line character
#define TEXT_NEWLINE 10
#define TEXT_SPACE 32

cImage* cText::m_pDefaultFontOrig = UNDEF;
cImage* cText::m_pDefaultFontExtOrig = UNDEF;

cImage* cText::m_pDefaultFont = UNDEF;
cImage** cText::m_pDefaultLetters = UNDEF;

cImage* cText::m_pDefaultFontExt = UNDEF;
cImage** cText::m_pDefaultLettersExt = UNDEF;

UINT cText::g_iCreated = 0;

void cText::SetDefaultMinFilter( UINT mode )
{
	if ( m_pDefaultFont ) m_pDefaultFont->SetMinFilter( mode );
	if ( m_pDefaultFontExt ) m_pDefaultFontExt->SetMinFilter( mode );
}

void cText::SetDefaultMagFilter( UINT mode )
{
	if ( m_pDefaultFont ) m_pDefaultFont->SetMagFilter( mode );
	if ( m_pDefaultFontExt ) m_pDefaultFontExt->SetMagFilter( mode );
}

void cText::SetDefaultFontImage( cImage *pImage )
{
	if ( pImage == m_pDefaultFont ) return;

	//if ( !pImage ) return;
	if ( m_pDefaultLetters ) m_pDefaultLetters = 0;

	// don't delete the main image as it was loaded by the user, they must delete it.
	//if ( m_pDefaultFont ) delete m_pDefaultFont;

	if ( !pImage )
	{
		// don't reload as this is a memory leak
		m_pDefaultFont = m_pDefaultFontOrig;
	}
	else
	{
		// load image used by all text objects
		m_pDefaultFont = pImage;
	}

	m_pDefaultLetters = m_pDefaultFont->GetFontImages();
}

void cText::SetDefaultExtendedFontImage( cImage *pImage )
{
	if ( m_pDefaultFontExt == pImage ) return;

	//if ( !pImage ) return;
	if ( m_pDefaultLettersExt ) m_pDefaultLettersExt = 0;

	if ( !pImage )
	{
		// don't reload as this is a memory leak
		m_pDefaultFontExt = m_pDefaultFontExtOrig;
	}
	else
	{
		// load image used by all text objects
		m_pDefaultFontExt = pImage;

	}

	m_pDefaultLettersExt = m_pDefaultFontExt->GetExtendedFontImages();
}

cText::cText( int iLength )
{
	m_iCreated = g_iCreated;
	g_iCreated++;

	m_iID = 0;
	m_iNumSprites = 0;
	m_pSprites = UNDEF;
	m_pCharStyles = 0;
	m_fX = 0;
	m_fY = 0;
	m_fSpriteX = 0;
	m_fSpriteY = 0;
	m_fAngle = 0;
	m_iDepth = 9; // 0 is on top
	m_fOrigSize = 4;
	m_fSize = 4;
	m_fTotalWidth = 0;
	m_fTotalHeight = 0;
	m_fSpacing = 0;
	m_fVSpacing = 0;
	m_bVisible = true;
	m_iRed = 255;
	m_iGreen = 255;
	m_iBlue = 255;
	m_iAlpha = 255;
	m_iAlign = 0;
	m_iVAlign = 0;
	m_iTransparency = 1;
	m_bManagedDrawing = false;
	m_pSpriteManager = UNDEF;
	m_iFixed = 0;
	m_fMaxWidth = 0;
	m_fHorizontalRatio = 0;
	m_fVerticalRatio = 0;
	m_fFontScale = 1;
	m_bFlags = 0;

	m_fClipX = 0;
	m_fClipY = 0;
	m_fClipX2 = 0;
	m_fClipY2 = 0;

	pVertices = UNDEF;
	pUV = UNDEF;
	pColor = UNDEF;
	pIndices = UNDEF;

	m_pFontImage = 0;
	m_pLetterImages = 0;

	m_pFontImageExt = 0;
	m_pLetterImagesExt = 0;

	if ( !m_pDefaultFontOrig )
	{
		// load image used by all text objects
		uString sPath( "ascii.png" );
		m_pDefaultFontOrig = new cImage( sPath );
		m_pDefaultFontOrig->SetWrapU( 0 );
		m_pDefaultFontOrig->SetWrapV( 0 );
		m_iImageID = m_pDefaultFontOrig->GetID();

		m_pDefaultLetters = m_pDefaultFontOrig->GetFontImages();
	}

	if ( !m_pDefaultFontExtOrig )
	{
		// load image used by all text objects
		uString sPath( "asciiExt.png" );
		m_pDefaultFontExtOrig = new cImage( sPath );
		m_pDefaultFontExtOrig->SetWrapU( 0 );
		m_pDefaultFontExtOrig->SetWrapV( 0 );

		m_pDefaultLettersExt = m_pDefaultFontExtOrig->GetExtendedFontImages();
	}

	if ( !m_pDefaultFont ) m_pDefaultFont = m_pDefaultFontOrig;
	if ( !m_pDefaultFontExt ) m_pDefaultFontExt = m_pDefaultFontExtOrig;

	m_pUsingDefaultFont = m_pDefaultFont;
	m_pUsingDefaultFontExt = m_pDefaultFontExt;

	// new fonts
	m_pFTFont = 0;
	m_pFTSizedFont = 0;
	if ( agk::m_iUseNewDefaultFonts == 1 )
	{
		m_pFTFont = AGKFont::GetDefaultFont();

		m_fHorizontalRatio = agk::DeviceToDisplayRatioX();
		m_fVerticalRatio = agk::DeviceToDisplayRatioY();

		int pixelSize = agk::Round( m_fOrigSize/m_fVerticalRatio );
		m_fSize = pixelSize * m_fVerticalRatio;
		m_pFTSizedFont = m_pFTFont->GetSizedFont( pixelSize );
		if ( m_pFTSizedFont ) 
		{
			m_fFontScale = pixelSize / (float) m_pFTSizedFont->GetSize();
			m_pFTSizedFont->AddRef();
		}

		m_bFlags |= AGK_TEXT_SNAP_TO_PIXELS;
	}

	if ( iLength > 0 )
	{
		// assign arrays
		pVertices = new float[ iLength*12 ];
		pUV = new float[ iLength*8 ];
		pColor = new unsigned char[ iLength*16 ];
		pIndices = new unsigned short[ iLength*6 ];
		
		for ( int i = 0; i < iLength; i++ )
		{
			pIndices[ i*6 + 0 ] = i*4 + 0;
			pIndices[ i*6 + 1 ] = i*4 + 1;
			pIndices[ i*6 + 2 ] = i*4 + 2;
			
			pIndices[ i*6 + 3 ] = i*4 + 2;
			pIndices[ i*6 + 4 ] = i*4 + 1;
			pIndices[ i*6 + 5 ] = i*4 + 3;
		}

		m_iNumSprites = iLength;
		m_pSprites = new cSprite*[ iLength ];
		m_pCharStyles = new unsigned char[ iLength ];
		for ( int i = 0; i < iLength; i++ )
		{
			m_pCharStyles[ i ] = 0;

			m_pSprites[ i ] = new cSprite( );
			m_pSprites[ i ]->SetManageImages( 0 );
			//m_pSprites[ i ]->SetOffset( 0,0 );
			m_pSprites[ i ]->SetDepth( m_iDepth );
			m_pSprites[ i ]->SetTransparency( 1 );
			m_pSprites[ i ]->SetColor( m_iRed, m_iGreen, m_iBlue, m_iAlpha );
			if ( m_pFTSizedFont ) 
			{
				if ( m_bFlags & AGK_TEXT_SNAP_TO_PIXELS ) m_pSprites[ i ]->SetSnap( 1 );
				AGKFontImage *pFontImage = m_pFTSizedFont->GetCharImage( 32, 0 );
				m_pSprites[ i ]->SetFontImage( pFontImage, m_fFontScale );
			}
			else
			{
				if ( m_pDefaultLetters )
				{
					m_pSprites[ i ]->SetImage( m_pDefaultLetters[0] );
				}
				else
				{
					m_pSprites[ i ]->SetImage( m_pDefaultFont );
					m_pSprites[ i ]->SetAnimation( m_pDefaultFont->GetWidth()/16, m_pDefaultFont->GetHeight()/6, 96 );

					if ( !m_pDefaultLettersExt )
					{
						m_pSprites[ i ]->AppendAnimation( m_pDefaultFontExt, m_pDefaultFontExt->GetWidth()/16, m_pDefaultFontExt->GetHeight()/8, 128 );
					}
				}

				m_pSprites[ i ]->SetSize( -1, m_fSize );
			}

			m_pSprites[ i ]->SetUVBorder( 0 );
		}

		// re-align the sprites
		ReAlignSprites();
	}
}

cText::~cText()
{
	// remove from any tweens
	TweenInstance::DeleteTarget( this );

	// lee - does this need cleaning up?
	// paul - no as it is a global value that could be used by multiple objects
	// if ( m_pDefaultFont ) { }

	if ( m_bManagedDrawing )
	{
		// remove sprites from sprite manager
		if ( m_pSpriteManager )
		{
			m_pSpriteManager->RemoveText( this );
		}

		m_bManagedDrawing = false;
	}

	// don't delete letter images as they are a pointer to a global array

	if ( m_pFontImage ) m_pFontImage->RemoveText( this );
	if ( m_pFontImageExt ) m_pFontImageExt->RemoveText( this );

	if ( m_pSprites ) 
	{
		for ( int i = 0; i < (int)m_iNumSprites; i++ )
		{
			delete m_pSprites[ i ];
		}

		delete [] m_pSprites;
	}

	if ( m_pCharStyles ) delete [] m_pCharStyles;

	if ( pVertices ) delete [] pVertices;
	if ( pUV ) delete [] pUV;
	if ( pColor ) delete [] pColor;
	if ( pIndices ) delete [] pIndices;

	if ( m_pFTSizedFont ) m_pFTSizedFont->Release();
	m_pFTSizedFont = 0;
}

void cText::SetSpriteManager( cSpriteMgrEx *pMgr )
{
	if ( m_pSpriteManager == pMgr ) return;

	// remove from any old manager
	if ( m_bManagedDrawing && m_pSpriteManager ) 
	{
		m_pSpriteManager->RemoveText( this );
	}

	m_bManagedDrawing = false;
	m_pSpriteManager = pMgr;

	UpdateManager();
}

void cText::GlobalImageDeleting( cImage *pImage )
{
	if ( m_pDefaultFont == pImage )
	{
		SetDefaultFontImage( 0 );
	}

	if ( m_pDefaultFontExt == pImage )
	{
		SetDefaultExtendedFontImage( 0 );
	}
}

void cText::ImageDeleting( cImage *pImage )
{
	if ( m_pFontImage == pImage )
	{
		SetFontImage( 0 );
		/*
		for ( int i = 0; i < (int)m_iNumSprites; i++ )
		{
			m_pSprites[ i ]->ImageDeleting( pImage );
			if ( m_pLetterImages )
			{
				int c = m_pSprites[ i ]->GetCurrentFrame() - 1;
				if ( c >= 96 || c < 0 ) c = 0;
				if ( m_pLetterImages[ c ] == 0 ) c = 0;
				m_pSprites[ i ]->ImageDeleting( m_pLetterImages[ c ] );
			}
		}
		m_pLetterImages = 0;
		m_pFontImage = 0;
		*/
	}

	if ( m_pFontImageExt == pImage )
	{
		SetExtendedFontImage( 0 );
		/*
		for ( int i = 0; i < (int)m_iNumSprites; i++ )
		{
			m_pSprites[ i ]->ImageDeleting( pImage );
			if ( m_pLetterImagesExt )
			{
				int c = m_pSprites[ i ]->GetCurrentFrame() - 1;
				if ( c >= 128 || c < 0 ) c = 0;
				if ( m_pLetterImagesExt[ c ] == 0 ) c = 0;
				m_pSprites[ i ]->ImageDeleting( m_pLetterImagesExt[ c ] );
			}
		}
		m_pLetterImagesExt = 0;
		m_pFontImageExt = 0;
		*/
	}
}

void cText::UpdateManager()
{
	// always use managed drawing
	if ( !m_bManagedDrawing )
	{
		if ( m_pSpriteManager )
		{
			m_pSpriteManager->AddText( this );
			m_bManagedDrawing = true;
		}
	}

	/*
	if ( m_iDepth > 0 )
	{
		if ( !m_bManagedDrawing )
		{
			// need to hand the sprites off to the sprite manager for proper ordered rendering
			if ( m_pSpriteManager )
			{
				m_pSpriteManager->AddText( this );
				m_bManagedDrawing = true;
			}
		}
	}
	else
	{
		if ( m_bManagedDrawing )
		{
			// remove sprites from sprite manager for faster (one call) rendering
			if ( m_pSpriteManager )
			{
				m_pSpriteManager->RemoveText( this );
			}

			m_bManagedDrawing = false;
		}
	}
	*/
}

void cText::SetString( const char* szString )
{
	m_sText.SetStr( szString );
	UINT iLength = m_sText.GetNumChars();
	if ( iLength > m_iNumSprites )
	{
		if ( pVertices ) delete [] pVertices;
		if ( pUV ) delete [] pUV;
		if ( pColor ) delete [] pColor;
		if ( pIndices ) delete [] pIndices;

		// assign arrays
		pVertices = new float[ iLength*12 ];
		pUV = new float[ iLength*8 ];
		pColor = new unsigned char[ iLength*16 ];
		pIndices = new unsigned short[ iLength*6 ];
		
		for ( UINT i = 0; i < iLength; i++ )
		{
			pIndices[ i*6 + 0 ] = i*4 + 0;
			pIndices[ i*6 + 1 ] = i*4 + 1;
			pIndices[ i*6 + 2 ] = i*4 + 2;
			
			pIndices[ i*6 + 3 ] = i*4 + 2;
			pIndices[ i*6 + 4 ] = i*4 + 1;
			pIndices[ i*6 + 5 ] = i*4 + 3;
		}

		// expand styles array
		unsigned char *newStyles = new unsigned char[ iLength ];
		for ( UINT i = 0; i < m_iNumSprites; i++ ) newStyles[ i ] = m_pCharStyles[ i ];
		for ( UINT i = m_iNumSprites; i < iLength; i++ ) newStyles[ i ] = 0;
		if ( m_pCharStyles ) delete [] m_pCharStyles;
		m_pCharStyles = newStyles;

		// expands sprites array
		cSprite **pNewSpriteList = new cSprite*[ iLength ];
		for ( UINT i = 0; i < m_iNumSprites; i++ )
		{
			pNewSpriteList[ i ] = m_pSprites[ i ];
		}

		for ( UINT i = m_iNumSprites; i < iLength; i++ )
		{
			pNewSpriteList[ i ] = new cSprite( );
			pNewSpriteList[ i ]->SetManageImages( 0 );
			pNewSpriteList[ i ]->SetDepth( m_iDepth );
			pNewSpriteList[ i ]->SetTransparency( 1 );
			pNewSpriteList[ i ]->SetColor( m_iRed, m_iGreen, m_iBlue, m_iAlpha );
			pNewSpriteList[ i ]->FixToScreen( m_iFixed ); 

			if ( m_pFTSizedFont ) 
			{
				if ( m_bFlags & AGK_TEXT_SNAP_TO_PIXELS ) pNewSpriteList[ i ]->SetSnap( 1 );
				AGKFontImage *pFontImage = m_pFTSizedFont->GetCharImage( 32, 0 );
				pNewSpriteList[ i ]->SetFontImage( pFontImage, m_fFontScale );
			}
			else
			{
				if ( m_pFontImage )
				{
					if ( m_pLetterImages )
					{
						pNewSpriteList[ i ]->SetImage( m_pLetterImages[0] );
					}
					else
					{
						pNewSpriteList[ i ]->SetImage( m_pFontImage );
						pNewSpriteList[ i ]->SetAnimation( m_pFontImage->GetWidth()/16, m_pFontImage->GetHeight()/6, 96 );

						if ( m_pFontImageExt )
						{
							if ( !m_pLetterImagesExt )
							{
								pNewSpriteList[ i ]->AppendAnimation( m_pFontImageExt, m_pFontImageExt->GetWidth()/16, m_pFontImageExt->GetHeight()/8, 128 );
							}
						}
						else
						{
							if ( !m_pDefaultLettersExt )
							{
								pNewSpriteList[ i ]->AppendAnimation( m_pDefaultFontExt, m_pDefaultFontExt->GetWidth()/16, m_pDefaultFontExt->GetHeight()/8, 128 );
							}
						}
					}
				}
				else
				{
					if ( m_pDefaultLetters )
					{
						pNewSpriteList[ i ]->SetImage( m_pDefaultLetters[0] );
					}
					else
					{
						pNewSpriteList[ i ]->SetImage( m_pDefaultFont );
						pNewSpriteList[ i ]->SetAnimation( m_pDefaultFont->GetWidth()/16, m_pDefaultFont->GetHeight()/6, 96 );

						if ( m_pFontImageExt )
						{
							if ( !m_pLetterImagesExt )
							{
								pNewSpriteList[ i ]->AppendAnimation( m_pFontImageExt, m_pFontImageExt->GetWidth()/16, m_pFontImageExt->GetHeight()/8, 128 );
							}
						}
						else
						{
							if ( !m_pDefaultLettersExt )
							{
								pNewSpriteList[ i ]->AppendAnimation( m_pDefaultFontExt, m_pDefaultFontExt->GetWidth()/16, m_pDefaultFontExt->GetHeight()/8, 128 );
							}
						}
					}
				}

				pNewSpriteList[ i ]->SetSize( -1, m_fSize );
			}
		}

		if ( m_pSprites ) delete [] m_pSprites;
		m_pSprites = pNewSpriteList;
		m_iNumSprites = iLength;
	}

	if ( (m_bFlags & AGK_TEXT_REFRESHING) == 0 )
	{
		for ( UINT i = 0; i < m_iNumSprites; i++ ) m_pCharStyles[ i ] = 0;
	}

	if ( !m_pFontImage || !m_pFontImageExt )
	{
		// check if default font image has changed
		if ( m_pUsingDefaultFont != m_pDefaultFont || m_pUsingDefaultFontExt != m_pDefaultFontExt )
		{
			m_pUsingDefaultFont = m_pDefaultFont;
			m_pUsingDefaultFontExt = m_pDefaultFontExt;
			InternalRefresh();
		}
	}

	for( UINT i = 0; i < iLength; i++ )
	{
		int c = m_sText.CharAt(i);
		int orig = c;
		if ( c < 32 ) c = 32;
		if ( m_pFTSizedFont )
		{
			if ( orig < 32 )
			{
				m_pSprites[ i ]->SetFontImage( 0, 1 );
				if ( orig != 9 ) m_pSprites[ i ]->SetSize( 0, 0 );
				else 
				{
					// tab character
					float VwDw = agk::DeviceToDisplayRatioX();
					float width = m_pFTSizedFont->GetCharImage( 32, 0 )->GetDisplayAdvanceX() * VwDw * m_fFontScale;
					m_pSprites[ i ]->SetSize( width*4, 0 );
				}
			}
			else
			{
				int style = m_pCharStyles[i];
				if ( m_bFlags & AGK_TEXT_BOLD ) style |= 0x01;

				AGKFontImage *pFontImage = m_pFTSizedFont->GetCharImage( c, style );
				if ( !pFontImage ) pFontImage = m_pFTSizedFont->GetCharImage( 32, 0 );
				m_pSprites[ i ]->SetFontImage( pFontImage, m_fFontScale );
			}
		}
		else
		{
			if ( c > 0xFF )
			{
				// convert UTF8 back into Windows-1252 for old font image
				switch( c )
				{
					case 0x20AC: c = 0x80; break;
					case 0x201A: c = 0x82; break;
					case 0x192: c = 0x83; break;
					case 0x201E: c = 0x84; break;
					case 0x2026: c = 0x85; break;
					case 0x2020: c = 0x86; break;
					case 0x2021: c = 0x87; break;
					case 0x2C6: c = 0x88; break;
					case 0x2030: c = 0x89; break;
					case 0x160: c = 0x8A; break;
					case 0x2039: c = 0x8B; break;
					case 0x152: c = 0x8C; break;
					case 0x17D: c = 0x8E; break;
					case 0x2018: c = 0x91; break;
					case 0x2019: c = 0x92; break;
					case 0x201C: c = 0x93; break;
					case 0x201D: c = 0x94; break;
					case 0x2022: c = 0x95; break;
					case 0x2013: c = 0x96; break;
					case 0x2014: c = 0x97; break;
					case 0x2DC: c = 0x98; break;
					case 0x2122: c = 0x99; break;
					case 0x161: c = 0x9A; break;
					case 0x203A: c = 0x9B; break;
					case 0x153: c = 0x9C; break;
					case 0x17E: c = 0x9E; break;
					case 0x178: c = 0x9F; break;
					default: c = 63;
				}
			}
			c -= 32;

			if ( c < 96 )
			{
				if ( m_pFontImage )
				{
					if ( m_pLetterImages )
					{
						if ( m_pLetterImages[c] ) m_pSprites[ i ]->SetImage( m_pLetterImages[c] );
						else m_pSprites[ i ]->SetImage( m_pLetterImages[0] );
						m_pSprites[ i ]->SetSize( -1, m_fSize );
					}
					else
					{
						if ( m_pSprites[ i ]->GetFrameCount() > 0 ) m_pSprites[ i ]->SetFrame( c+1 );
					}
				}
				else
				{
					if ( m_pDefaultLetters )
					{
						if ( m_pDefaultLetters[c] ) m_pSprites[ i ]->SetImage( m_pDefaultLetters[c] );
						else m_pSprites[ i ]->SetImage( m_pDefaultLetters[0] );
						m_pSprites[ i ]->SetSize( -1, m_fSize );
					}
					else
					{
						if ( m_pSprites[ i ]->GetFrameCount() > 0 ) m_pSprites[ i ]->SetFrame( c+1 );
					}
				}
			}
			else
			{
				c -= 96;

				// extended letters
				if ( m_pFontImageExt )
				{
					if ( m_pLetterImagesExt )
					{
						if ( m_pLetterImagesExt[c] ) m_pSprites[ i ]->SetImage( m_pLetterImagesExt[c] );
						else m_pSprites[ i ]->SetImage( m_pLetterImagesExt[0] );
						m_pSprites[ i ]->SetSize( -1, m_fSize );
					}
					else
					{
						if ( m_pSprites[ i ]->GetFrameCount() > 0 ) m_pSprites[ i ]->SetFrame( c+96+1 );
					}
				}
				else
				{
					if ( m_pDefaultLettersExt )
					{
						if ( m_pDefaultLettersExt[c] ) m_pSprites[ i ]->SetImage( m_pDefaultLettersExt[c] );
						else m_pSprites[ i ]->SetImage( m_pDefaultLettersExt[0] );
						m_pSprites[ i ]->SetSize( -1, m_fSize );
					}
					else
					{
						if ( m_pSprites[ i ]->GetFrameCount() > 0 ) m_pSprites[ i ]->SetFrame( c+96+1 );
					}
				}
			}

			if ( orig < 32 ) m_pSprites[ i ]->SetSize( 0, m_pSprites[ i ]->GetHeight() );
			else m_pSprites[ i ]->SetSize( -1, m_pSprites[ i ]->GetHeight() );
		}
		
		m_pSprites[ i ]->SetVisible( m_bVisible );
		m_pSprites[ i ]->SetUVBorder( 0 );
	}

	// re-align the sprites
	ReAlignSprites();

	for ( UINT i = iLength; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->SetVisible( false );
	}
}

void cText::InternalRefresh()
{
	// doesn't apply to new FT fonts
	if ( m_pFTSizedFont ) return;

	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->ClearAnimationFrames();
		m_pSprites[ i ]->SetImage( 0 ); // must set to 0 first to avoid deleted image pointer problem
	}
		
	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		if ( m_pFontImage )
		{
			if ( !m_pLetterImages )
			{
				m_pSprites[ i ]->SetImage( m_pFontImage );
				m_pSprites[ i ]->SetAnimation( m_pFontImage->GetWidth()/16, m_pFontImage->GetHeight()/6, 96 );

				if ( m_pFontImageExt )
				{
					if ( !m_pLetterImagesExt )
					{
						m_pSprites[ i ]->AppendAnimation( m_pFontImageExt, m_pFontImageExt->GetWidth()/16, m_pFontImageExt->GetHeight()/8, 128 );
					}
				}
				else
				{
					if ( !m_pDefaultLettersExt )
					{
						m_pSprites[ i ]->AppendAnimation( m_pDefaultFontExt, m_pDefaultFontExt->GetWidth()/16, m_pDefaultFontExt->GetHeight()/8, 128 );
					}
				}
			}
		}
		else
		{
			if ( !m_pDefaultLetters )
			{
				m_pSprites[ i ]->SetImage( m_pDefaultFont );
				m_pSprites[ i ]->SetAnimation( m_pDefaultFont->GetWidth()/16, m_pDefaultFont->GetHeight()/6, 96 );

				if ( m_pFontImageExt )
				{
					if ( !m_pLetterImagesExt )
					{
						m_pSprites[ i ]->AppendAnimation( m_pFontImageExt, m_pFontImageExt->GetWidth()/16, m_pFontImageExt->GetHeight()/8, 128 );
					}
				}
				else
				{
					if ( !m_pDefaultLettersExt )
					{
						m_pSprites[ i ]->AppendAnimation( m_pDefaultFontExt, m_pDefaultFontExt->GetWidth()/16, m_pDefaultFontExt->GetHeight()/8, 128 );
					}
				}
			}
		}
	}
}

void cText::Refresh()
{
	m_bFlags |= AGK_TEXT_REFRESHING;
	InternalRefresh();
	SetString( m_sText.GetStr() );
	m_bFlags &= ~AGK_TEXT_REFRESHING;
}

void cText::SetPosition( float fX, float fY )
{
	float oldX = m_fSpriteX;
	float oldY = m_fSpriteY;
	m_fX = fX;
	m_fY = fY;
	ShiftPosition( fX - oldX, fY - oldY );
}

void cText::ReAlignSprites()
{
	UINT letters = m_sText.GetNumChars();

	if ( m_bFlags & AGK_TEXT_SNAP_TO_PIXELS )
	{
		int iSpriteX = agk::Round(m_fX / agk::DeviceToDisplayRatioX());
		m_fSpriteX = iSpriteX * agk::DeviceToDisplayRatioX();

		int iSpriteY = agk::Round(m_fY / agk::DeviceToDisplayRatioY());
		m_fSpriteY = iSpriteY * agk::DeviceToDisplayRatioY();
	}
	else
	{
		m_fSpriteX = m_fX;
		m_fSpriteY = m_fY;
	}
	
	m_iLines = 1;
	m_fTotalWidth = 0;
	m_fTotalHeight = 0;
	float currWidth = 0;
	float lineWidth = 0;
	float letterWidth = 0;

	float stretch = agk::m_fStretchValue;
	float fSinA = 0;
	float fCosA = 1;
	float fSinA1 = 0;
	float fSinA2 = 0;

	if ( m_fAngle != 0 )
	{
		stretch = agk::m_fStretchValue;
		fSinA = agk::SinRad(m_fAngle);
		fCosA = agk::CosRad(m_fAngle);
		fSinA1 = fSinA/stretch;
		fSinA2 = fSinA*stretch;
	}

	const AGKFontImage *pFontImage = 0;
	if ( m_iAlign < 0 ) m_iAlign = 0;
	if ( m_iAlign > 2 ) m_iAlign = 2;
	switch ( m_iAlign )
	{
		case 0:
		{
			// left
			UINT first = 0;
			UINT curr = 0;
			int lastSpace = -1;
			float y = m_fSpriteY;
			if ( letters > 0 ) 
			{
				letterWidth = m_pSprites[ 0 ]->GetFontImage() ? m_pSprites[ 0 ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ 0 ]->GetWidth();
				lineWidth = letterWidth;
			}
			
			// look for new line characters and position line by line
			while ( curr < letters )
			{
				if ( curr < letters-1 ) 
				{
					letterWidth = m_pSprites[ curr+1 ]->GetFontImage() ? m_pSprites[ curr+1 ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ curr+1 ]->GetWidth();
					lineWidth += m_fSpacing + letterWidth;
				}

				bool bLineBreak = false;
				if ( m_sText.CharAt( curr ) == TEXT_SPACE ) lastSpace = curr;
				if ( (m_fMaxWidth > 0 && lineWidth > m_fMaxWidth) || (m_fMaxWidth < 0 && (curr-first) > abs(m_fMaxWidth)) )
				{
					if ( m_sText.CharAt( curr ) != TEXT_NEWLINE )
					{
						if ( lastSpace > (int)first ) curr = lastSpace;
					}
					bLineBreak = true;
				}

				if ( m_sText.CharAt( curr ) == TEXT_NEWLINE || bLineBreak )
				{
					// position this line, indices first->curr
					for ( UINT i = first; i <= curr; i++ )
					{
						float offsetX = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetX()*m_fFontScale : 0;
						float offsetY = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetY()*m_fFontScale : 0;

						if ( m_fAngle == 0 )
						{
							m_pSprites[ i ]->SetPosition( m_fSpriteX + currWidth + offsetX, y + offsetY );
						}
						else
						{
							float fTempX = currWidth + m_pSprites[ i ]->GetOffsetX() + offsetX;
							float fTempY = y - m_fSpriteY + m_pSprites[ i ]->GetOffsetY() + offsetY;
							
							float fNewX = fTempX*fCosA - fTempY*fSinA1 - m_pSprites[ i ]->GetOffsetX();
							float fNewY = fTempY*fCosA + fTempX*fSinA2 - m_pSprites[ i ]->GetOffsetY();

							m_pSprites[ i ]->SetPosition( m_fSpriteX + fNewX, m_fSpriteY + fNewY );
						}

						letterWidth = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ i ]->GetWidth();
						currWidth += letterWidth + m_fSpacing;
					}

					if ( currWidth > 0 ) currWidth -= m_fSpacing;
					if ( currWidth > m_fTotalWidth ) m_fTotalWidth = currWidth;
					currWidth = 0;

					// setup next line
					lastSpace = -1;
					first = curr+1;
					y += m_fSize+m_fVSpacing;
					lineWidth = 0;
					if ( first < letters ) 
					{
						letterWidth = m_pSprites[ first ]->GetFontImage() ? m_pSprites[ first ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ first ]->GetWidth();
						lineWidth = letterWidth;
					}
					m_iLines++;
				}

				curr++;
			}

			if ( first < curr ) 
			{
				// final line (and first if no new line characters found)
				for ( UINT i = first; i < curr; i++ )
				{
					float offsetX = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetX()*m_fFontScale : 0;
					float offsetY = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetY()*m_fFontScale : 0;

					if ( m_fAngle == 0 )
					{
						m_pSprites[ i ]->SetPosition( m_fSpriteX + currWidth + offsetX, y + offsetY );
					}
					else
					{
						float fTempX = currWidth + m_pSprites[ i ]->GetOffsetX() + offsetX;
						float fTempY = y - m_fSpriteY + m_pSprites[ i ]->GetOffsetY() + offsetY;
						
						float fNewX = fTempX*fCosA - fTempY*fSinA1 - m_pSprites[ i ]->GetOffsetX();
						float fNewY = fTempY*fCosA + fTempX*fSinA2 - m_pSprites[ i ]->GetOffsetY();

						m_pSprites[ i ]->SetPosition( m_fSpriteX + fNewX, m_fSpriteY + fNewY );
					}

					letterWidth = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ i ]->GetWidth();
					currWidth += letterWidth + m_fSpacing;
				}

				currWidth -= m_fSpacing;
				if ( currWidth > m_fTotalWidth ) m_fTotalWidth = currWidth;
				m_fTotalHeight = (y - m_fSpriteY) + m_fSize;
			}
			else 
			{
				m_fTotalHeight = (y - m_fSpriteY) - m_fVSpacing;
			}
			break;
		}
		case 1:
		{
			// center
			UINT first = 0;
			UINT curr = 0;
			int lastSpace = -1;
			float y = m_fSpriteY;
			if ( letters > 0 ) 
			{
				letterWidth = m_pSprites[ 0 ]->GetFontImage() ? m_pSprites[ 0 ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ 0 ]->GetWidth();
				lineWidth = letterWidth;
			}

			// look for new line characters and position line by line
			while ( curr < letters )
			{
				if ( curr < letters-1 ) 
				{
					letterWidth = m_pSprites[ curr+1 ]->GetFontImage() ? m_pSprites[ curr+1 ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ curr+1 ]->GetWidth();
					lineWidth += m_fSpacing + letterWidth;
				}

				bool bLineBreak = false;
				if ( m_sText.CharAt( curr ) == TEXT_SPACE ) lastSpace = curr;
				if ( (m_fMaxWidth > 0 && lineWidth > m_fMaxWidth) || (m_fMaxWidth < 0 && (curr-first) > abs(m_fMaxWidth)) )
				{
					if ( m_sText.CharAt( curr ) != TEXT_NEWLINE )
					{
						if ( lastSpace > (int)first ) curr = lastSpace;
					}
					bLineBreak = true;
				}

				if ( m_sText.CharAt( curr ) == TEXT_NEWLINE || bLineBreak )
				{
					currWidth = 0;
					for( UINT i = first; i <= curr; i++ )
					{
						letterWidth = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ i ]->GetWidth();
						currWidth += letterWidth + m_fSpacing;
					}

					if ( currWidth > 0 ) currWidth -= m_fSpacing;
					if ( currWidth > m_fTotalWidth ) m_fTotalWidth = currWidth;

					// position this line, indices first->curr
					float pos = -currWidth/2.0f;
					if ( m_sText.CharAt( curr ) == TEXT_NEWLINE || m_sText.CharAt( curr ) == TEXT_SPACE ) 
					{
						letterWidth = m_pSprites[ curr ]->GetFontImage() ? m_pSprites[ curr ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ curr ]->GetWidth();
						pos += (letterWidth + m_fSpacing)/2.0f;
					}

					for ( UINT i = first; i <= curr; i++ )
					{
						float offsetX = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetX()*m_fFontScale : 0;
						float offsetY = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetY()*m_fFontScale : 0;

						if ( m_fAngle == 0 )
						{
							m_pSprites[ i ]->SetPosition( m_fSpriteX + pos + offsetX, y + offsetY );
						}
						else
						{
							float fTempX = pos + m_pSprites[ i ]->GetOffsetX() + offsetX;
							float fTempY = y - m_fSpriteY + m_pSprites[ i ]->GetOffsetY() + offsetY;
							
							float fNewX = fTempX*fCosA - fTempY*fSinA1 - m_pSprites[ i ]->GetOffsetX();
							float fNewY = fTempY*fCosA + fTempX*fSinA2 - m_pSprites[ i ]->GetOffsetY();

							m_pSprites[ i ]->SetPosition( m_fSpriteX + fNewX, m_fSpriteY + fNewY );
						}

						letterWidth = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ i ]->GetWidth();
						pos += letterWidth + m_fSpacing;
					}

					// setup next line
					lastSpace = -1;
					first = curr+1;
					y += m_fSize+m_fVSpacing;
					currWidth = 0;
					lineWidth = 0;
					if ( first < letters ) 
					{
						letterWidth = m_pSprites[ first ]->GetFontImage() ? m_pSprites[ first ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ first ]->GetWidth();
						lineWidth = letterWidth;
					}
					m_iLines++;
				}
				else
				{
					letterWidth = m_pSprites[ curr ]->GetFontImage() ? m_pSprites[ curr ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ curr ]->GetWidth();
					currWidth += letterWidth + m_fSpacing;
				}

				curr++;
			}

			if ( currWidth > 0 ) 
			{
				currWidth -= m_fSpacing;
				if ( currWidth > m_fTotalWidth ) m_fTotalWidth = currWidth;

				// final line (and first if no new line characters found)
				float pos = -currWidth/2.0f;
				for ( UINT i = first; i < curr; i++ )
				{
					float offsetX = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetX()*m_fFontScale : 0;
					float offsetY = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetY()*m_fFontScale : 0;

					if ( m_fAngle == 0 )
					{
						m_pSprites[ i ]->SetPosition( m_fSpriteX + pos + offsetX, y + offsetY );
					}
					else
					{
						float fTempX = pos + m_pSprites[ i ]->GetOffsetX() + offsetX;
						float fTempY = y - m_fSpriteY + m_pSprites[ i ]->GetOffsetY() + offsetY;
						
						float fNewX = fTempX*fCosA - fTempY*fSinA1 - m_pSprites[ i ]->GetOffsetX();
						float fNewY = fTempY*fCosA + fTempX*fSinA2 - m_pSprites[ i ]->GetOffsetY();

						m_pSprites[ i ]->SetPosition( m_fSpriteX + fNewX, m_fSpriteY + fNewY );
					}

					letterWidth = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ i ]->GetWidth();
					pos += letterWidth + m_fSpacing;
				}

				m_fTotalHeight = (y - m_fSpriteY) + m_fSize;
			}
			else
			{
				m_fTotalHeight = (y - m_fSpriteY) - m_fVSpacing;
			}
			
			break;
		}
		case 2:
		{
			// right
			UINT first = 0;
			UINT curr = 0;
			int lastSpace = -1;
			float y = m_fSpriteY;
			if ( letters > 0 ) 
			{
				letterWidth = m_pSprites[ 0 ]->GetFontImage() ? m_pSprites[ 0 ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ 0 ]->GetWidth();
				lineWidth = letterWidth;
			}

			// look for new line characters and position line by line
			while ( curr < letters )
			{
				if ( curr < letters-1 ) 
				{
					letterWidth = m_pSprites[ curr+1 ]->GetFontImage() ? m_pSprites[ curr+1 ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ curr+1 ]->GetWidth();
					lineWidth += m_fSpacing + letterWidth;
				}

				bool bLineBreak = false;
				if ( m_sText.CharAt( curr ) == TEXT_SPACE ) lastSpace = curr;
				if ( (m_fMaxWidth > 0 && lineWidth > m_fMaxWidth) || (m_fMaxWidth < 0 && (curr-first) > abs(m_fMaxWidth)) )
				{
					if ( m_sText.CharAt( curr ) != TEXT_NEWLINE )
					{
						if ( lastSpace > (int)first ) curr = lastSpace;
					}
					bLineBreak = true;
				}

				if ( m_sText.CharAt( curr ) == TEXT_NEWLINE || bLineBreak )
				{
					currWidth = 0;
					for( UINT i = first; i <= curr; i++ )
					{
						letterWidth = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ i ]->GetWidth();
						currWidth += letterWidth + m_fSpacing;
					}

					if ( currWidth > 0 ) currWidth -= m_fSpacing;
					if ( currWidth > m_fTotalWidth ) m_fTotalWidth = currWidth;

					// position this line, indices first->curr
					float pos = -currWidth;
					if ( m_sText.CharAt( curr ) == TEXT_NEWLINE || m_sText.CharAt( curr ) == TEXT_SPACE ) 
					{
						letterWidth = m_pSprites[ curr ]->GetFontImage() ? m_pSprites[ curr ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ curr ]->GetWidth();
						pos += letterWidth + m_fSpacing;
					}

					for ( UINT i = first; i <= curr; i++ )
					{
						float offsetX = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetX()*m_fFontScale : 0;
						float offsetY = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetY()*m_fFontScale : 0;

						if ( m_fAngle == 0 )
						{
							m_pSprites[ i ]->SetPosition( m_fSpriteX + pos + offsetX, y + offsetY );
						}
						else
						{
							float fTempX = pos + m_pSprites[ i ]->GetOffsetX() + offsetX;
							float fTempY = y - m_fSpriteY + m_pSprites[ i ]->GetOffsetY() + offsetY;
							
							float fNewX = fTempX*fCosA - fTempY*fSinA1 - m_pSprites[ i ]->GetOffsetX();
							float fNewY = fTempY*fCosA + fTempX*fSinA2 - m_pSprites[ i ]->GetOffsetY();

							m_pSprites[ i ]->SetPosition( m_fSpriteX + fNewX, m_fSpriteY + fNewY );
						}

						letterWidth = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ i ]->GetWidth();
						pos += letterWidth + m_fSpacing;
					}

					// setup next line
					lastSpace = -1;
					first = curr+1;
					y += m_fSize+m_fVSpacing;
					currWidth = 0;
					lineWidth = 0;
					if ( first < letters ) 
					{
						letterWidth = m_pSprites[ first ]->GetFontImage() ? m_pSprites[ first ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ first ]->GetWidth();
						lineWidth = letterWidth;
					}
					m_iLines++;
				}
				else
				{
					letterWidth = m_pSprites[ curr ]->GetFontImage() ? m_pSprites[ curr ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ curr ]->GetWidth();
					currWidth += letterWidth + m_fSpacing;
				}

				curr++;
			}

			if ( currWidth > 0 ) 
			{
				currWidth -= m_fSpacing;
				if ( currWidth > m_fTotalWidth ) m_fTotalWidth = currWidth;

				// final line (and first if no new line characters found)
				float pos = -currWidth;
				for ( UINT i = first; i < curr; i++ )
				{
					float offsetX = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetX()*m_fFontScale : 0;
					float offsetY = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayOffsetY()*m_fFontScale : 0;

					if ( m_fAngle == 0 )
					{
						m_pSprites[ i ]->SetPosition( m_fSpriteX + pos + offsetX, y + offsetY );
					}
					else
					{
						float fTempX = pos + m_pSprites[ i ]->GetOffsetX() + offsetX;
						float fTempY = y - m_fSpriteY + m_pSprites[ i ]->GetOffsetY() + offsetY;
						
						float fNewX = fTempX*fCosA - fTempY*fSinA1 - m_pSprites[ i ]->GetOffsetX();
						float fNewY = fTempY*fCosA + fTempX*fSinA2 - m_pSprites[ i ]->GetOffsetY();

						m_pSprites[ i ]->SetPosition( m_fSpriteX + fNewX, m_fSpriteY + fNewY );
					}

					letterWidth = m_pSprites[ i ]->GetFontImage() ? m_pSprites[ i ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ i ]->GetWidth();
					pos += letterWidth + m_fSpacing;
				}

				m_fTotalHeight = (y - m_fSpriteY) + m_fSize;
			}
			else
			{
				m_fTotalHeight = (y - m_fSpriteY) - m_fVSpacing;
			}
			break;
		}
	}

	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->SetAngleRad( m_fAngle );
	}
}

void cText::SetX( float fX )
{
	float oldX = m_fSpriteX;
	m_fX = fX;
	// don't do a full rebuild of position data
	//SetPosition( m_fX, m_fY );
	ShiftPosition( fX - oldX, 0 );
}

void cText::SetY( float fY )
{
	float oldY = m_fSpriteY;
	m_fY = fY;
	//SetPosition( m_fX, m_fY );
	ShiftPosition( 0, fY - oldY );
}

void cText::SetAngle( float fA )
{
	m_fAngle = fA * PI / 180.0f;
	//SetPosition( m_fX, m_fY );
	ReAlignSprites();
}

void cText::SetAngleRad( float fA )
{
	m_fAngle = fA;
	//SetPosition( m_fX, m_fY );
	ReAlignSprites();
}

void cText::ChangedAspect()
{
	SetSize( m_fOrigSize );
}

void cText::ShiftPosition( float fDiffX, float fDiffY )
{
	if ( m_bFlags & AGK_TEXT_SNAP_TO_PIXELS )
	{
		int iDiffX = agk::Round(fDiffX / agk::DeviceToDisplayRatioX());
		int iDiffY = agk::Round(fDiffY / agk::DeviceToDisplayRatioY());
		if ( iDiffX == 0 && iDiffY == 0 ) return;

		fDiffX = iDiffX * agk::DeviceToDisplayRatioX();
		fDiffY = iDiffY * agk::DeviceToDisplayRatioY();
	}

	m_fSpriteX += fDiffX;
	m_fSpriteY += fDiffY;

	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->SetPosition( m_pSprites[ i ]->GetX() + fDiffX, m_pSprites[ i ]->GetY() + fDiffY );
	}
}

void cText::SetSize( float fSize )
{
	if ( fSize < 0 ) fSize = 0;
	m_fOrigSize = fSize;
	m_fSize = fSize;
	int updateRequired = 0;

	if ( m_pFTFont )
	{
		if ( m_fHorizontalRatio != agk::DeviceToDisplayRatioX() || m_fVerticalRatio != agk::DeviceToDisplayRatioY() ) updateRequired = 1;

		m_fHorizontalRatio = agk::DeviceToDisplayRatioX();
		m_fVerticalRatio = agk::DeviceToDisplayRatioY();
		
		int pixelSize = agk::Round( m_fOrigSize/m_fVerticalRatio );
		m_fSize = pixelSize * m_fVerticalRatio;

		AGKSizedFont *pNewFont = m_pFTFont->GetSizedFont( pixelSize );
		float newScale = m_fFontScale;
		if ( pNewFont ) newScale = pixelSize / (float) pNewFont->GetSize();

		if ( pNewFont != m_pFTSizedFont ) 
		{
			m_fFontScale = newScale;

			for ( UINT i = 0; i < m_iNumSprites; i++ )
			{
				m_pSprites[ i ]->SetFontImage( 0, 1 );
			}
			
			if ( m_pFTSizedFont ) m_pFTSizedFont->Release();
			m_pFTSizedFont = pNewFont;
			if ( m_pFTSizedFont ) m_pFTSizedFont->AddRef();

			// have to resign all sprite images
			Refresh();
		}
		else
		{
			if ( m_fFontScale != newScale || updateRequired )
			{
				for ( UINT i = 0; i < m_iNumSprites; i++ )
				{
					if ( m_pSprites[ i ]->GetImagePtr() )
					{
						float width = m_pSprites[ i ]->GetImagePtr()->GetWidth() * m_fHorizontalRatio * newScale;
						float height = m_pSprites[ i ]->GetImagePtr()->GetHeight() * m_fVerticalRatio * newScale;
						m_pSprites[ i ]->SetSize( width, height );
					}
					else
					{
						float adjust = 1;
						if ( m_fFontScale != 0 ) adjust = newScale / m_fFontScale;
						m_pSprites[ i ]->SetSize( m_pSprites[i]->GetWidth()*adjust, m_pSprites[i]->GetHeight()*adjust );
					}
				}

				m_fFontScale = newScale;
				ReAlignSprites();
			}
		}
	}

	// if for some reason we failed to get a sized font then fall back to bitmap fonts
	if ( !m_pFTSizedFont )
	{
		for ( UINT i = 0; i < m_iNumSprites; i++ )
		{
			m_pSprites[ i ]->SetSize( -1, m_fSize );
		}

		ReAlignSprites();
	}
}

void cText::SetMaxWidth( float width )
{
	if ( width < 0 ) 
	{
		// LEE: new feature, when negative, will wrap on character count, not pixel width
		m_fMaxWidth = width;
	}
	else
	{
		m_fMaxWidth = width;
	}
	//SetPosition( m_fX, m_fY );
	ReAlignSprites();
}

void cText::SetBold( UINT bold )
{
	int changed = (m_bFlags & AGK_TEXT_BOLD) ^ (bold ? AGK_TEXT_BOLD : 0);
	if ( !changed ) return;

	if ( bold ) m_bFlags |= AGK_TEXT_BOLD;
	else m_bFlags &= ~AGK_TEXT_BOLD;

	Refresh();
}

void cText::SetOverrideScissor( int mode )
{
	if ( mode == 0 ) m_bFlags &= ~AGK_TEXT_OVERRIDE_SCISSOR;
	else m_bFlags |= AGK_TEXT_OVERRIDE_SCISSOR;
}

void cText::SetSnap( int mode ) 
{ 
	if ( mode ) m_bFlags |= AGK_TEXT_SNAP_TO_PIXELS; 
	else m_bFlags &= ~AGK_TEXT_SNAP_TO_PIXELS; 

	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->SetSnap( mode );
	}
}

void cText::SetSpacing( float fSpacing )
{
	m_fSpacing = fSpacing;
	//SetPosition( m_fX, m_fY );
	ReAlignSprites();
}

void cText::SetLineSpacing( float fSpacing )
{
	m_fVSpacing = fSpacing;
	//SetPosition( m_fX, m_fY );
	ReAlignSprites();
}

// 0=left, 1=center, 2=right
void cText::SetAlignment( int iMode )
{
	if ( iMode < 0 ) iMode = 0;
	if ( iMode > 2 ) iMode = 2;

	m_iAlign = iMode;
	//SetPosition( m_fX, m_fY );
	ReAlignSprites();
}

void cText::SetDepth( int iDepth )
{
	m_iDepth = iDepth;

	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->SetDepth( m_iDepth );
	}

	m_bDepthChanged = true;

	UpdateManager();
}

void cText::SetVisible( bool bVisible )
{
	m_bVisible = bVisible;

	for ( UINT i = 0; i < m_sText.GetNumChars(); i++ )
	{
		m_pSprites[ i ]->SetVisible( bVisible );
	}
}

void cText::SetColor( UINT iRed, UINT iGreen, UINT iBlue, UINT iAlpha )
{
	if ( iRed > 255 ) iRed = 255;
	if ( iGreen > 255 ) iGreen = 255;
	if ( iBlue > 255 ) iBlue = 255;
	if ( iAlpha > 255 ) iAlpha = 255;

	m_iRed = iRed;
	m_iGreen = iGreen;
	m_iBlue = iBlue; 
	m_iAlpha = iAlpha;

	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->SetColor( m_iRed, m_iGreen, m_iBlue, m_iAlpha );
	}
}

void cText::SetRed( UINT iRed )
{
	if ( iRed > 255 ) iRed = 255;
	m_iRed = iRed;
	
	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->SetRed( m_iRed );
	}
}

void cText::SetGreen( UINT iGreen )
{
	if ( iGreen > 255 ) iGreen = 255;
	m_iGreen = iGreen;
	
	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->SetGreen( m_iGreen );
	}
}

void cText::SetBlue( UINT iBlue )
{
	if ( iBlue > 255 ) iBlue = 255;
	m_iBlue = iBlue;
	
	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->SetBlue( m_iBlue );
	}
}

void cText::SetAlpha( UINT iAlpha )
{
	if ( iAlpha > 255 ) iAlpha = 255;
	m_iAlpha = iAlpha;
	
	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->SetAlpha( m_iAlpha );
	}
}


void cText::SetTransparency( int iMode )
{
	if ( iMode == m_iTransparency ) return;

	m_iTransparency = iMode;
	m_bTransparencyChanged = true;

	UpdateManager();
}

void cText::SetFontImage( cImage *pImage )
{
	// clear out freetype font
	if ( m_pFTFont )
	{
		if ( m_pFTSizedFont )
		{
			for ( UINT i = 0; i < m_iNumSprites; i++ )
			{
				m_pSprites[ i ]->SetFontImage( 0, 1 );
			}

			m_pFTSizedFont->Release();
			m_pFTSizedFont = 0;
		}
		m_pFTFont = 0;
	}

	if ( m_pLetterImages ) m_pLetterImages = 0;

	if ( m_pFontImage != pImage ) 
	{
		if ( m_pFontImage ) m_pFontImage->RemoveText( this );
		if ( pImage ) pImage->AddText( this );
	}

	if ( !pImage )
	{
		m_pFontImage = 0;
		m_iImageID = 0;
		Refresh();
		return;
	}
	
	m_pFontImage = pImage;
	m_iImageID = m_pFontImage->GetID();
	m_pLetterImages = m_pFontImage->GetFontImages();
	
	Refresh();
}

void cText::SetExtendedFontImage( cImage *pImage )
{
	if ( m_pLetterImagesExt ) m_pLetterImagesExt = 0;

	if ( m_pFontImageExt != pImage ) 
	{
		if ( m_pFontImageExt ) m_pFontImageExt->RemoveText( this );
		if ( pImage ) pImage->AddText( this );
	}

	if ( !pImage )
	{
		m_pFontImageExt = 0;
		Refresh();
		return;
	}

	m_pFontImageExt = pImage;
	m_pLetterImagesExt = m_pFontImageExt->GetExtendedFontImages();
	
	Refresh();
}

void cText::SetFont( AGKFont *pFont )
{
	// clear out bitmap font
	if ( m_pLetterImages ) m_pLetterImages = 0;
	if ( m_pFontImage ) m_pFontImage->RemoveText( this );

	m_pFontImage = 0;
	m_iImageID = 0;

	if ( !pFont ) pFont = AGKFont::GetDefaultFont();
	if ( m_pFTFont == pFont ) return;
	if ( m_pFTFont )
	{
		if ( m_pFTSizedFont )
		{
			for ( UINT i = 0; i < m_iNumSprites; i++ )
			{
				m_pSprites[ i ]->SetFontImage( 0, 1 );
			}

			m_pFTSizedFont->Release();
			m_pFTSizedFont = 0;
		}
	}
	else
	{
		// switching from bitmap fonts to new fonts
		SetSnap( 1 );
	}

	m_pFTFont = pFont;
	
	if ( m_pFTFont )
	{
		SetSize( m_fOrigSize ); // this regenerates the font for all sprites
	}
}

void cText::SetCharPosition( UINT iIndex, float x, float y )
{
	if ( iIndex >= m_iNumSprites ) return;

	float offsetX = m_pSprites[ iIndex ]->GetFontImage() ? m_pSprites[ iIndex ]->GetFontImage()->GetDisplayOffsetX()*m_fFontScale : 0;
	float offsetY = m_pSprites[ iIndex ]->GetFontImage() ? m_pSprites[ iIndex ]->GetFontImage()->GetDisplayOffsetY()*m_fFontScale : 0;
	m_pSprites[ iIndex ]->SetPosition( m_fX + x + offsetX, m_fY + y + offsetY );
}

void cText::SetCharX( UINT iIndex, float x )
{
	if ( iIndex >= m_iNumSprites ) return;

	float offsetX = m_pSprites[ iIndex ]->GetFontImage() ? m_pSprites[ iIndex ]->GetFontImage()->GetDisplayOffsetX()*m_fFontScale : 0;
	m_pSprites[ iIndex ]->SetX( m_fX + x + offsetX );
}

void cText::SetCharY( UINT iIndex, float y )
{
	if ( iIndex >= m_iNumSprites ) return;

	float offsetY = m_pSprites[ iIndex ]->GetFontImage() ? m_pSprites[ iIndex ]->GetFontImage()->GetDisplayOffsetY()*m_fFontScale : 0;
	m_pSprites[ iIndex ]->SetY( m_fY + y + offsetY );
}

void cText::SetCharAngle( UINT iIndex, float angle )
{
	if ( iIndex >= m_iNumSprites ) return;

	m_pSprites[ iIndex ]->SetAngle( angle );
}

void cText::SetCharAngleRad( UINT iIndex, float angle )
{
	if ( iIndex >= m_iNumSprites ) return;

	m_pSprites[ iIndex ]->SetAngleRad( angle );
}

void cText::SetCharColor( UINT iIndex, UINT red, UINT green, UINT blue, UINT alpha )
{
	if ( iIndex >= m_iNumSprites ) return;

	m_pSprites[ iIndex ]->SetColor( red, green, blue, alpha );
}

void cText::SetCharRed( UINT iIndex, UINT red )
{
	if ( iIndex >= m_iNumSprites ) return;

	m_pSprites[ iIndex ]->SetRed( red );
}

void cText::SetCharGreen( UINT iIndex, UINT green )
{
	if ( iIndex >= m_iNumSprites ) return;

	m_pSprites[ iIndex ]->SetGreen( green );
}

void cText::SetCharBlue( UINT iIndex, UINT blue )
{
	if ( iIndex >= m_iNumSprites ) return;

	m_pSprites[ iIndex ]->SetBlue( blue );
}

void cText::SetCharAlpha( UINT iIndex, UINT alpha )
{
	if ( iIndex >= m_iNumSprites ) return;

	m_pSprites[ iIndex ]->SetAlpha( alpha );
}

void cText::SetCharBold( UINT iIndex, UINT bold )
{
	if ( iIndex >= m_sText.GetNumChars() ) return;

	int changed = (m_pCharStyles[ iIndex ] & 0x1) ^ (bold ? 1 : 0);
	if ( !changed ) return;

	if ( bold ) m_pCharStyles[ iIndex ] |= 0x01;
	else m_pCharStyles[ iIndex ] &= ~0x01;

	if ( m_pFTSizedFont )
	{
		int c = m_sText.CharAt(iIndex);
		if ( c >= 32 )
		{
			int style = m_pCharStyles[iIndex];
			if ( m_bFlags & AGK_TEXT_BOLD ) style |= 0x01;

			AGKFontImage *pFontImage = m_pFTSizedFont->GetCharImage( c, style );
			if ( !pFontImage ) pFontImage = m_pFTSizedFont->GetCharImage( 32, 0 );
			m_pSprites[ iIndex ]->SetFontImage( pFontImage, m_fFontScale );
			m_pSprites[ iIndex ]->SetUVBorder( 0 );

			ReAlignSprites();
		}
	}
}

// Get Char

float cText::GetCharX( UINT iIndex )
{
	if ( iIndex >= m_iNumSprites ) return 0;

	float offsetX = m_pSprites[ iIndex ]->GetFontImage() ? m_pSprites[ iIndex ]->GetFontImage()->GetDisplayOffsetX()*m_fFontScale : 0;
	return m_pSprites[ iIndex ]->GetX() - m_fX - offsetX;
}

float cText::GetCharY( UINT iIndex )
{
	if ( iIndex >= m_iNumSprites ) return 0;

	float offsetY = m_pSprites[ iIndex ]->GetFontImage() ? m_pSprites[ iIndex ]->GetFontImage()->GetDisplayOffsetY()*m_fFontScale : 0;
	return m_pSprites[ iIndex ]->GetY() - m_fY - offsetY;
}

float cText::GetCharWidth( UINT iIndex )
{
	if ( iIndex >= m_iNumSprites ) return 0;

	float width = m_pSprites[ iIndex ]->GetFontImage() ? m_pSprites[ iIndex ]->GetFontImage()->GetDisplayAdvanceX()*m_fFontScale : m_pSprites[ iIndex ]->GetWidth();
	return width;
}

float cText::GetCharAngle( UINT iIndex )
{
	if ( iIndex >= m_iNumSprites ) return 0;

	return m_pSprites[ iIndex ]->GetAngle();
}

float cText::GetCharAngleRad( UINT iIndex )
{
	if ( iIndex >= m_iNumSprites ) return 0;

	return m_pSprites[ iIndex ]->GetAngleRad();
}

UINT cText::GetCharRed( UINT iIndex )
{
	if ( iIndex >= m_iNumSprites ) return 0;

	return m_pSprites[ iIndex ]->GetColorRed();
}

UINT cText::GetCharGreen( UINT iIndex )
{
	if ( iIndex >= m_iNumSprites ) return 0;

	return m_pSprites[ iIndex ]->GetColorGreen();
}

UINT cText::GetCharBlue( UINT iIndex )
{
	if ( iIndex >= m_iNumSprites ) return 0;

	return m_pSprites[ iIndex ]->GetColorBlue();
}

UINT cText::GetCharAlpha( UINT iIndex )
{
	if ( iIndex >= m_iNumSprites ) return 0;

	return m_pSprites[ iIndex ]->GetColorAlpha();
}

unsigned char cText::GetChar( UINT iIndex )
{
	if ( iIndex >= m_sText.GetNumChars() ) return 0;

	return m_sText.CharAt( iIndex );
}

void cText::FixToScreen( int mode )
{
	m_iFixed = mode != 0 ? 1 : 0;
	
	for ( UINT i = 0; i < m_iNumSprites; i++ )
	{
		m_pSprites[ i ]->FixToScreen( mode );
	}
}

float cText::GetX( )
{
	return m_fX;
}

float cText::GetY( )
{
	return m_fY;
}

float cText::GetAngle( )
{
	return m_fAngle * 180.0f / PI;
}

float cText::GetAngleRad( )
{
	return m_fAngle;
}

UINT cText::GetLength( )
{
	return m_sText.GetNumChars();
}

bool cText::GetHitTest( float x, float y )
{
	if ( m_iFixed != 0 )
	{
		x = agk::WorldToScreenX( x );
		y = agk::WorldToScreenY( y );
	}

	// scissor test
	if ( m_fClipX2 != 0 || m_fClipX != 0 || m_fClipY != 0 || m_fClipY2 != 0 )
	{
		if ( x < m_fClipX || x > m_fClipX2 || y < m_fClipY || y > m_fClipY2 ) return false;
	}

	float fTempX;
	float fTempY;

	switch( m_iAlign )
	{
		case 0:
		{
			fTempX = x - m_fX;
			fTempY = y - m_fY;
			break;
		}

		case 1:
		{
			fTempX = (x - m_fX) + (m_fTotalWidth/2.0f);
			fTempY = y - m_fY;
			break;
		}

		case 2:
		{
			fTempX = (x - m_fX) + m_fTotalWidth;
			fTempY = y - m_fY;
			break;
		}
	}

	return (fTempX > 0) && (fTempY > 0) && (fTempX < m_fTotalWidth) && (fTempY < m_fTotalHeight);
}

void cText::SetScissor( float x, float y, float x2, float y2 )
{
	if ( x == 0 && y == 0 && x2 == 0 && y2 == 0 )
	{
		m_fClipX = 0;
		m_fClipY = 0;
		m_fClipX2 = 0;
		m_fClipY2 = 0;
		return;
	}

	float temp;
	if ( x2 < x ) 
	{
		temp = x2;
		x2 = x;
		x = temp;
	}

	if ( y2 < y )
	{
		temp = y2;
		y2 = y;
		y = temp;
	}

	m_fClipX = x;
	m_fClipY = y;
	m_fClipX2 = x2;
	m_fClipY2 = y2;
}

void cText::GetClipValues( int &x, int &y, int &width, int &height )
{
	if ( m_fClipX2 == 0 && m_fClipX == 0 && m_fClipY == 0 && m_fClipY2 == 0 )
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
		return;
	}

	float fX = m_fClipX;
	float fY = m_fClipY;
	float fX2 = m_fClipX2;
	float fY2 = m_fClipY2;

	if ( m_iFixed == 0 )
	{
		fX = agk::WorldToScreenX( fX );
		fY = agk::WorldToScreenY( fY );

		fX2 = agk::WorldToScreenX( fX2 );
		fY2 = agk::WorldToScreenY( fY2 );
	}

	// swap Y values for viewport
	if ( agk::m_bUsingFBO )
	{
		x = agk::ScreenToViewportX( fX );
		y = agk::ScreenToViewportY( fY );
		width = agk::ScreenToViewportX( fX2 ) - x;
		height = agk::ScreenToViewportY( fY2 ) - y;
	}
	else
	{
		x = agk::ScreenToViewportX( fX );
		y = agk::ScreenToViewportY( fY2 );
		width = agk::ScreenToViewportX( fX2 ) - x;
		height = agk::ScreenToViewportY( fY ) - y;
	}
}

void cText::Update( float time )
{
	for ( UINT i = 0; i < m_sText.GetNumChars(); i++ )
	{
		m_pSprites[ i ]->Update( time );
	}
}

void cText::Draw()
{
	if ( m_pFTFont )
	{
		float hRatio = agk::DeviceToDisplayRatioX();
		float vRatio = agk::DeviceToDisplayRatioY();

		if ( m_fHorizontalRatio != hRatio || m_fVerticalRatio != vRatio )
		{
			SetSize( m_fOrigSize );
		}
	}
	else
	{
		if ( !m_pFontImage || !m_pFontImageExt )
		{
			// check if default font image has changed
			if ( m_pUsingDefaultFont != m_pDefaultFont || m_pUsingDefaultFontExt != m_pDefaultFontExt )
			{
				m_pUsingDefaultFont = m_pDefaultFont;
				m_pUsingDefaultFontExt = m_pDefaultFontExt;
				Refresh();
			}
		}
	}

	int iScissorX, iScissorY, iScissorWidth, iScissorHeight;
	if ( (m_bFlags & AGK_TEXT_OVERRIDE_SCISSOR) == 0 )
	{
		GetClipValues( iScissorX, iScissorY, iScissorWidth, iScissorHeight );
		
		if ( iScissorX != 0 || iScissorY != 0 || iScissorWidth != 0 || iScissorHeight != 0 )
		{
			agk::PlatformScissor( iScissorX, iScissorY, iScissorWidth, iScissorHeight );
		}
		else
		{
			agk::ResetScissor();
		}
	}

	if ( m_pFTSizedFont ) PlatformDrawFT();
	else PlatformDraw();

	if ( (m_bFlags & AGK_TEXT_OVERRIDE_SCISSOR) == 0 )
	{
		if ( iScissorX != 0 || iScissorY != 0 || iScissorWidth != 0 || iScissorHeight != 0 )
		{
			agk::ResetScissor();
		}
	}
}

void cText::PlatformDraw()
{
	if ( !m_bVisible ) return; 

	agk::PlatformBindBuffer( 0 );
	agk::PlatformBindIndexBuffer( 0 );

	AGKShader *pShader = AGKShader::g_pShaderTexColor; 
	
	if ( !pShader ) return;
	pShader->MakeActive();
	
	int locPos = pShader->GetAttribPosition();
	int locColor = pShader->GetAttribColor();
	int locTex = pShader->GetAttribTexCoord();

	if ( locPos >= 0 ) pShader->SetAttribFloat( locPos, 3, 0, pVertices );
	if ( locColor >= 0 ) pShader->SetAttribUByte( locColor, 4, 0, true, pColor );
	if ( locTex >= 0 ) pShader->SetAttribFloat( locTex, 2, 0, pUV );

	agk::PlatformSetBlendMode( m_iTransparency );
	agk::PlatformSetCullMode( 0 );
	agk::PlatformSetDepthRange( 0, 1 );
	agk::PlatformSetDepthTest( 0 );

	// extended characters
	UINT iCurrImage = 0;
	if ( m_pDefaultFontExt ) iCurrImage = m_pDefaultFontExt->GetTextureID();
	if ( m_pFontImageExt ) iCurrImage = m_pFontImageExt->GetTextureID();

	int iLength = m_sText.GetNumChars();
	int count = 0;
	for ( int i = 0; i < iLength; i++ )
	{
		if ( m_pSprites[ i ]->GetImagePtr() && m_pSprites[ i ]->GetImagePtr()->GetTextureID() == iCurrImage ) 
		{
			if ( m_pSprites[ i ]->GetInScreen() )
			{
				m_pSprites[ i ]->BatchDrawQuad( pVertices + count*12, pUV + count*8, pColor + count*16 );
				count++;

				if ( count >= 15000 )
				{
					cImage::BindTexture( iCurrImage );
					pShader->DrawIndices( count*6, pIndices );
					count = 0;
				}
			}
		}
	}
	
	if ( count > 0 ) 
	{
		cImage::BindTexture( iCurrImage );
		pShader->DrawIndices( count*6, pIndices );
	}

	int oldImage = iCurrImage;

	// normal characters
	iCurrImage = 0;
	if ( m_pDefaultFont ) iCurrImage = m_pDefaultFont->GetTextureID();
	if ( m_pFontImage ) iCurrImage = m_pFontImage->GetTextureID();

	if ( oldImage == iCurrImage ) return;

	count = 0;
	for ( int i = 0; i < iLength; i++ )
	{
		if ( m_pSprites[ i ]->GetImagePtr() && m_pSprites[ i ]->GetImagePtr()->GetTextureID() == iCurrImage ) 
		{
			if ( m_pSprites[ i ]->GetInScreen() )
			{
				m_pSprites[ i ]->BatchDrawQuad( pVertices + count*12, pUV + count*8, pColor + count*16 );
				count++;

				if ( count >= 15000 )
				{
					cImage::BindTexture( iCurrImage );
					pShader->DrawIndices( count*6, pIndices );
					count = 0;
				}
			}
		}
	}

	if ( count > 0 ) 
	{
		cImage::BindTexture( iCurrImage );
		pShader->DrawIndices( count*6, pIndices );
	}
}

void cText::PlatformDrawFT()
{
	if ( !m_bVisible ) return; 

	agk::PlatformBindBuffer( 0 );
	agk::PlatformBindIndexBuffer( 0 );

	AGKShader *pShader = AGKShader::g_pShaderFont; 
	
	if ( !pShader ) return;
	pShader->MakeActive();
	
	int locPos = pShader->GetAttribPosition();
	int locColor = pShader->GetAttribColor();
	int locTex = pShader->GetAttribTexCoord();

	if ( locPos >= 0 ) pShader->SetAttribFloat( locPos, 3, 0, pVertices );
	if ( locColor >= 0 ) pShader->SetAttribUByte( locColor, 4, 0, true, pColor );
	if ( locTex >= 0 ) pShader->SetAttribFloat( locTex, 2, 0, pUV );

	agk::PlatformSetBlendMode( m_iTransparency );
	agk::PlatformSetCullMode( 0 );
	agk::PlatformSetDepthRange( 0, 1 );
	agk::PlatformSetDepthTest( 0 );

	for( unsigned int image = 0; image < m_pFTSizedFont->GetNumMainImages(); image++ )
	{
		UINT iCurrImage = m_pFTSizedFont->GetMainImage( image )->GetTextureID();

		int iLength = m_sText.GetNumChars();
		int count = 0;
		for ( int i = 0; i < iLength; i++ )
		{
			if ( m_pSprites[ i ]->GetImagePtr() && m_pSprites[ i ]->GetImagePtr()->GetTextureID() == iCurrImage && m_pSprites[ i ]->GetInScreen() ) 
			{
				m_pSprites[ i ]->BatchDrawQuad( pVertices + count*12, pUV + count*8, pColor + count*16 );
				count++;
				if ( count >= 15000 )
				{
					cImage::BindTexture( iCurrImage );
					pShader->DrawIndices( count*6, pIndices );
					count = 0;
				}
			}
		}

		if ( count > 0 ) 
		{
			cImage::BindTexture( iCurrImage );
			pShader->DrawIndices( count*6, pIndices );
		}
	}

	/*
	static cSprite *pMain = 0;
	if ( !pMain )
	{
		pMain = new cSprite();
		pMain->SetPosition( 0,500 );
	}

	if ( m_sText.GetNumChars() > 0 )
	{
		cImage *pImage = m_pFTSizedFont->GetMainImage( 0 );
		pMain->SetImage( pImage );
		pMain->SetSize( pImage->GetWidth()/2.0f, pImage->GetHeight()/2.0f );
		pMain->Draw();
	}
	*/
}

}
