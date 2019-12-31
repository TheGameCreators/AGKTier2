#include "agk.h"

cImage* cEditBox::g_pDefaultFont = 0;
cEditBox* cEditBox::g_pCurrentEditBox = 0;

cEditBox::cEditBox()
{
	if ( !g_pDefaultFont ) g_pDefaultFont = new cImage( "/Arial.png" );

	m_iID = 0;
	m_fX = 0;
	m_fY = 0;
	m_fZ = 0.001f; // 0 is top
	m_iDepth = 10;
	m_fWidth = 150;
	m_fHeight = 18;
	m_fBorder = 1;
	m_bActive = false;
	m_bActivePrev = false;
	m_bVisible = true;
	m_bEnabled = true;
	m_fTextSize = m_fHeight - m_fHeight/5;
	m_bMultiLine = false;
	m_iMaxLines = 0;
	m_bFixed = false;
	m_bIsPassword = false;
    m_bUseAlternateInput = true;
    m_bSupportsExtendedAscii = false;
    m_bSupportWarned = false;
	m_iWrapMode = 0;
	m_iInputType = 0;
	m_iTextChanged = 0;
    m_iLastLength = 0;
	m_bFlags = 0;

	m_fScissorX = 0;
	m_fScissorY = 0;
	m_fScissorX2 = 0;
	m_fScissorY2 = 0;
	
	m_iCursorPos = 0;
	m_fCursorBlinkTime = 0;
	m_iShowCursor = 1;
	m_iMaxChars = 0;
	m_fCursorBlinkDelay = 0.6f;

	m_pInputBorder = new cSprite();
	m_pInputArea = new cSprite();
	m_pInputText = new cText( 30 );
	m_pInputCursor = new cSprite();

	m_iOldLength = 0;
	m_iOldCursorPos = -1;

	// set some defaults
	m_pInputCursor->SetColor( 102, 213, 255, 255 );
	m_pInputBorder->SetColor( 190,190,190, 255 );
	m_pInputArea->SetColor( 255,255,255, 255 );
	m_pInputText->SetColor( 0,0,0 );

	m_pInputText->SetOverrideScissor( 1 );

	m_pInputText->SetSize( m_fTextSize );
	float cursorwidth = m_fTextSize/25;
	if ( cursorwidth < agk::GetVirtualWidth() / agk::m_fTargetViewportWidth ) cursorwidth = agk::GetVirtualWidth() / agk::m_fTargetViewportWidth;
	m_pInputCursor->SetSize( cursorwidth, m_fTextSize );

	if ( agk::m_iUseNewDefaultFonts ) 
	{
		m_bFlags |= AGK_EDITBOX_USING_NEW_FONT;
		m_pInputText->SetFont( 0 );
	}
	else m_pInputText->SetFontImage( g_pDefaultFont );

	ParametersChanged();
}

cEditBox::~cEditBox()
{
	if ( g_pCurrentEditBox == this ) SetFocus(false);

	if ( m_pInputCursor ) delete m_pInputCursor;
	if ( m_pInputBorder ) delete m_pInputBorder;
	if ( m_pInputArea ) delete m_pInputArea;
	if ( m_pInputText ) delete m_pInputText;
}

void cEditBox::SetID( UINT iID )
{
	m_iID = iID;
}

void cEditBox::SetPosition( float x, float y )
{
	m_fX = x;
	m_fY = y;

	ParametersChanged();
}

void cEditBox::SetSize( float width, float height )
{
	m_fWidth = width;
	m_fHeight = height;

	if ( m_fTextSize + m_fTextSize/5 > m_fHeight ) SetTextSize( m_fHeight - m_fHeight/5 );
	else 
	{
		ParametersChanged();
		SetWrapMode( m_iWrapMode );
	}
}

void cEditBox::SetDepth( int depth )
{
	m_iDepth = depth;
	m_fZ = depth / 10000.0f;
	m_pInputCursor->SetDepth( depth );
	m_pInputBorder->SetDepth( depth );
	m_pInputArea->SetDepth( depth );
	m_pInputText->SetDepth( depth );
	m_bDepthChanged = true;
}

void cEditBox::SetBorderSize( float size )
{
	if ( size < 0 ) size = 0;
	m_fBorder = size;

	ParametersChanged();
}

void cEditBox::SetBorderColor( UINT red, UINT green, UINT blue, UINT alpha )
{
	/*
	if ( red > 255 ) red = 255;
	if ( green > 255 ) green = 255;
	if ( blue > 255 ) blue = 255;
	if ( alpha > 255 ) alpha = 255;

	m_iBorderColor = (((((red << 8) + green) << 8) + blue) << 8) + alpha;
	*/
	m_pInputBorder->SetColor( red, green, blue, alpha );
}

void cEditBox::SetBackgroundColor( UINT red, UINT green, UINT blue, UINT alpha )
{
	m_pInputArea->SetColor( red, green, blue, alpha );
}

void cEditBox::SetTextColor( UINT red, UINT green, UINT blue )
{
	m_pInputText->SetColor( red, green, blue );
}

void cEditBox::SetCursorColor( UINT red, UINT green, UINT blue )
{
	m_pInputCursor->SetColor( red, green, blue );
}

void cEditBox::SetFontImage( cImage* image )
{
	if ( image == 0 ) m_pInputText->SetFontImage( g_pDefaultFont );
	else m_pInputText->SetFontImage( image );
}

void cEditBox::SetExtendedFontImage( cImage* image )
{
    m_bSupportsExtendedAscii = (image != 0);
    
	if ( image == 0 ) m_pInputText->SetExtendedFontImage( g_pDefaultFont );
	else m_pInputText->SetExtendedFontImage( image );
}

void cEditBox::SetFont( AGKFont *pFont )
{
	m_pInputText->SetFont( pFont );
}

void cEditBox::SetTextSize( float size )
{
	if ( size < 0 ) size = 0;
	if ( size + size/5 > m_fHeight ) size = m_fHeight - m_fHeight/5;

	m_fTextSize = size;
	m_pInputText->SetSize( size );
	float cursorwidth = size/25;
	if ( cursorwidth < agk::GetVirtualWidth() / agk::m_fTargetViewportWidth ) cursorwidth = agk::GetVirtualWidth() / agk::m_fTargetViewportWidth;
	m_pInputCursor->SetSize( cursorwidth, size );

	ParametersChanged();

	SetWrapMode( m_iWrapMode );
}

void cEditBox::SetFocus( bool active )
{
	if ( m_bActive && !active ) m_iTextChanged = 1;
	m_bActive = active;

	// activate input method?
	if ( active ) 
	{
		if ( g_pCurrentEditBox != this ) PlatformStartText();
		g_pCurrentEditBox = this;
	}
	else 
	{
		if ( g_pCurrentEditBox == this ) 
		{
			PlatformEndText();
			g_pCurrentEditBox = 0;
		}
	}
}

void cEditBox::SetActive( bool enabled )
{
	m_bEnabled = enabled;
}

void cEditBox::SetVisible( bool visible )
{
	m_bVisible = visible;
}

void cEditBox::SetBorderImage( cImage* image )
{
	m_pInputBorder->SetImage( image );
}

void cEditBox::SetBackgroundImage( cImage* image )
{
	m_pInputArea->SetImage( image );
}

void cEditBox::SetCursorBlinkTime( float time )
{
	if ( time < 0 ) time = 0;
	m_fCursorBlinkDelay = time;
}

void cEditBox::SetCursorWidth( float width )
{
	if ( width < 0 ) width = 0;
	m_pInputCursor->SetSize( width, m_pInputCursor->GetHeight() );
}

void cEditBox::SetText( const char *str )
{
	m_sCurrInput.SetStr( str );
	m_iCursorPos = m_sCurrInput.GetNumChars();
	float spacer = m_fTextSize/10;
	if ( m_bFlags & AGK_EDITBOX_USING_NEW_FONT ) 
	{
		m_pInputText->SetPosition( m_fX + spacer, m_fY + spacer );
	}
	else
	{
		m_pInputText->SetPosition( m_fX + spacer, m_fY + spacer );
	}
    if ( m_bActive ) agk::PlatformChangeTextInput( str );
}

// coordinates are in world space
void cEditBox::SetCursorLocation( float x, float y )
{
	if ( GetFixed() )
	{
		x = agk::WorldToScreenX( x );
		y = agk::WorldToScreenY( y );
	}

	float textX = m_pInputText->GetX();
	float textY = m_pInputText->GetY();

	int tempPos = 0;
	float rowHeight = m_pInputText->GetSize() + m_pInputText->GetLineSpacing();

	// find nearest row
	bool bFound = false;
	float tempY = rowHeight;
	if ( y > textY + tempY )
	{
		while ( tempPos < (int)m_pInputText->GetLength() )
		{
			if ( m_pInputText->GetCharY( tempPos ) > tempY - (rowHeight*0.1f) ) 
			{
				tempY += rowHeight;
				if ( y < textY + tempY )
				{
					break;
				}
			}
			
			tempPos++;
		}
	}

	// find character in that row
	while ( tempPos < (int)m_pInputText->GetLength() )
	{
		if ( m_pInputText->GetChar( tempPos ) == '\n' ) break;

		if ( m_pInputText->GetCharY( tempPos ) > tempY - (rowHeight*0.1f) ) 
		{
			tempPos--;
			break;
		}
		
		float tempX = textX + m_pInputText->GetCharX( tempPos ) + m_pInputText->GetCharWidth( tempPos );
		if ( x < tempX )
		{
			tempX = x - textX - m_pInputText->GetCharX( tempPos );
			if ( tempX > m_pInputText->GetCharWidth( tempPos )/2 ) tempPos++;
			break;
		}

		tempPos++;
	}

	if ( tempPos < 0 ) tempPos = 0;
	if ( tempPos > (int)m_pInputText->GetLength() ) tempPos = m_pInputText->GetLength();
	m_iCursorPos = tempPos;

	m_iShowCursor = 1;
	m_fCursorBlinkTime = m_fCursorBlinkDelay;

	PlatformUpdateExternal();
}

void cEditBox::SetCursorPosition( int pos )
{
	if ( pos < 0 ) pos = 0;
	if ( pos > (int)m_sCurrInput.GetNumChars() ) pos = m_sCurrInput.GetNumChars();
	m_iCursorPos = pos;
	PlatformUpdateExternal();
}

void cEditBox::SetMaxChars( UINT max )
{
	m_iMaxChars = max;
}

void cEditBox::SetMultiLine( bool multiline )
{
	m_bMultiLine = multiline;
	if ( multiline || m_iWrapMode==1 ) m_pInputText->SetMaxWidth( m_fWidth - m_fTextSize/5 );
	else m_pInputText->SetMaxWidth( 0 );

	// realign characters
	uString text;
	m_pInputText->GetText( text );
	m_pInputText->SetString( text );
}

void cEditBox::SetWrapMode( int mode ) 
{ 
	m_iWrapMode = mode; 
	if ( m_bMultiLine || m_iWrapMode==1 ) m_pInputText->SetMaxWidth( m_fWidth - m_fTextSize/5 );
	else m_pInputText->SetMaxWidth( 0 );
}

void cEditBox::SetInputType( int mode ) 
{ 
	m_iInputType = mode; 
}

void cEditBox::SetMaxLines( UINT lines )
{
	m_iMaxLines = lines;
}

void cEditBox::SetScissor( float x, float y ,float x2, float y2 )
{
	if ( x == 0 && y == 0 && x2 == 0 && y2 == 0 )
	{
		m_fScissorX = 0;
		m_fScissorY = 0;
		m_fScissorX2 = 0;
		m_fScissorY2 = 0;
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

	m_fScissorX = x;
	m_fScissorY = y;
	m_fScissorX2 = x2;
	m_fScissorY2 = y2;
}

void cEditBox::FixToScreen( bool fix )
{
	m_bFixed = fix;
	m_pInputCursor->FixToScreen( fix );
	m_pInputBorder->FixToScreen( fix );
	m_pInputArea->FixToScreen( fix );
	m_pInputText->FixToScreen( fix );
}

bool cEditBox::GetDepthChanged() 
{
	bool bChanged = m_bDepthChanged;
	m_bDepthChanged = false;
	return bChanged; 
}

char* cEditBox::GetText()
{
	uString out;
	out.SetStr( m_sCurrInput );

	char *str = new char[ out.GetLength()+1 ];
	strcpy( str, out.GetStr() );
	return str;	
}

bool cEditBox::GetHitTest( float x, float y )
{
	float realx = x;
	float realy = y;

	// GetHitTest accepts coordinates in world space so if the edit box is fixed then convert them to screen space
	if ( GetFixed() )
	{
		realx = agk::WorldToScreenX( x );
		realy = agk::WorldToScreenY( y );
	}

	if ( m_fScissorX != m_fScissorX2 && m_fScissorY != m_fScissorY2 )
	{
		if ( realx < m_fScissorX || realx > m_fScissorX2 || realy < m_fScissorY || realy > m_fScissorY2 ) return false;
	}

	return m_pInputBorder->GetHitTest( x, y );
}

void cEditBox::ParametersChanged()
{
	// update all components
	float spacer = m_fTextSize/10;

	m_pInputBorder->SetSize( m_fWidth + m_fBorder*2, m_fHeight + m_fBorder*2 );
	m_pInputBorder->SetPosition( m_fX - m_fBorder, m_fY - m_fBorder );

	m_pInputArea->SetSize( m_fWidth, m_fHeight );
	m_pInputArea->SetPosition( m_fX, m_fY );

	if ( m_bFlags & AGK_EDITBOX_USING_NEW_FONT )
	{
		m_pInputText->SetPosition( m_fX + spacer, m_fY + spacer );
	}
	else
	{
		m_pInputText->SetPosition( m_fX + spacer, m_fY + spacer );
		m_pInputText->SetSpacing( spacer );
	}
}

void cEditBox::Update()
{
	m_bActivePrev = m_bActive;
	if ( !m_bActive ) return;
	if ( !m_bVisible ) return;
	if ( !m_bEnabled ) return;
    
    if ( PlatformUpdateText() )
    {
        return;
    }

	if ( agk::GetPrevChar() != agk::GetLastChar() )
	{
		if ( (m_iMaxChars > 0 && m_sCurrInput.GetNumChars() >= m_iMaxChars) || (m_iMaxLines > 0 && m_pInputText->GetLines() > m_iMaxLines) )
		{
			m_iShowCursor = 1;
			m_fCursorBlinkTime = m_fCursorBlinkDelay;
		}
		else
		{
			if ( m_iCursorPos < (int) m_sCurrInput.GetNumChars() )
			{
				m_sCurrInput.InsertUnicodeAt( m_iCursorPos, agk::GetLastChar() );
			}
			else
			{
				m_sCurrInput.AppendUnicode( agk::GetLastChar() );
			}

			m_iCursorPos++;
			if ( m_iCursorPos > (int) m_sCurrInput.GetNumChars() ) m_iCursorPos = m_sCurrInput.GetNumChars();
			m_iShowCursor = 1;
			m_fCursorBlinkTime = m_fCursorBlinkDelay;
		}
	}

	// enter pressed
	if ( agk::GetRawKeyPressed( AGK_KEY_ENTER ) )
	{
		if ( !m_bMultiLine ) 
		{
			//m_bActive = false;
			//PlatformEndText();
			SetFocus(false);
		}
		else
		{
			if ( (m_iMaxChars > 0 && m_sCurrInput.GetNumChars() >= m_iMaxChars) || (m_iMaxLines > 0 && m_pInputText->GetLines() >= m_iMaxLines) )
			{
				m_iShowCursor = 1;
				m_fCursorBlinkTime = m_fCursorBlinkDelay;
			}
			else
			{
				if ( m_iCursorPos < (int) m_sCurrInput.GetNumChars() )
				{
					m_sCurrInput.InsertUnicodeAt( m_iCursorPos, '\n' );
				}
				else
				{
					m_sCurrInput.AppendAscii( '\n' );
				}

				m_iCursorPos++;
				if ( m_iCursorPos > (int) m_sCurrInput.GetNumChars() ) m_iCursorPos = m_sCurrInput.GetNumChars();
				m_iShowCursor = 1;
				m_fCursorBlinkTime = m_fCursorBlinkDelay;
			}
		}
	}

	// escape pressed
	if ( agk::GetRawKeyPressed( AGK_KEY_ESCAPE ) )
	{
		//m_bActive = false;
		//PlatformEndText();
		SetFocus( false );
	}

	// backspace repeat
	static float fBackspaceTimer = 0;
	bool bBackspaceRepeat = false;
	if ( !agk::GetRawKeyState( AGK_KEY_BACK ) ) fBackspaceTimer = 0;

	if ( fBackspaceTimer > 0 )
	{
		fBackspaceTimer -= agk::GetFrameTime();
		if ( fBackspaceTimer <= 0 ) 
		{
			fBackspaceTimer = 0.05f;
			bBackspaceRepeat = true;
		}
	}

	// backspace pressed
	if ( agk::GetRawKeyPressed( AGK_KEY_BACK ) || bBackspaceRepeat )
	{
		if ( m_iCursorPos > 0 )
		{
			m_sCurrInput.DeleteCharAt( m_iCursorPos-1 );
			m_iCursorPos--;
		}
		m_iShowCursor = 1;
		m_fCursorBlinkTime = m_fCursorBlinkDelay;

		if ( fBackspaceTimer <= 0 ) fBackspaceTimer = 0.5f;
	}

	// delete repeat
	static float fDeleteTimer = 0;
	bool bDeleteRepeat = false;
	if ( !agk::GetRawKeyState( AGK_KEY_DELETE ) ) fDeleteTimer = 0;

	if ( fDeleteTimer > 0 )
	{
		fDeleteTimer -= agk::GetFrameTime();
		if ( fDeleteTimer <= 0 ) 
		{
			fDeleteTimer = 0.05f;
			bDeleteRepeat = true;
		}
	}

	// delete pressed
	if ( agk::GetRawKeyPressed( AGK_KEY_DELETE ) || bDeleteRepeat )
	{
		if ( m_iCursorPos < (int)m_sCurrInput.GetNumChars() )
		{
			m_sCurrInput.DeleteCharAt( m_iCursorPos );
		}

		if ( fDeleteTimer <= 0 ) fDeleteTimer = 0.5f;
	}

	// left repeat
	static float fLeftTimer = 0;
	bool bLeftRepeat = false;
	if ( !agk::GetRawKeyState( AGK_KEY_LEFT ) ) fLeftTimer = 0;

	if ( fLeftTimer > 0 )
	{
		fLeftTimer -= agk::GetFrameTime();
		if ( fLeftTimer <= 0 ) 
		{
			fLeftTimer = 0.05f;
			bLeftRepeat = true;
		}
	}

	// left arrow pressed
	if ( agk::GetRawKeyPressed( AGK_KEY_LEFT ) || bLeftRepeat )
	{
		m_iCursorPos--;
		if ( m_iCursorPos < 0 ) m_iCursorPos = 0;
		m_iShowCursor = 1;
		m_fCursorBlinkTime = m_fCursorBlinkDelay;

		if ( fLeftTimer <= 0 ) fLeftTimer = 0.5f;
	}

	// right repeat
	static float fRightTimer = 0;
	bool bRightRepeat = false;
	if ( !agk::GetRawKeyState( AGK_KEY_RIGHT ) ) fRightTimer = 0;

	if ( fRightTimer > 0 )
	{
		fRightTimer -= agk::GetFrameTime();
		if ( fRightTimer <= 0 ) 
		{
			fRightTimer = 0.05f;
			bRightRepeat = true;
		}
	}

	// right arrow pressed
	if ( agk::GetRawKeyPressed( AGK_KEY_RIGHT ) || bRightRepeat )
	{
		m_iCursorPos++;
		if ( m_iCursorPos > (int) m_sCurrInput.GetNumChars() ) m_iCursorPos = m_sCurrInput.GetNumChars();
		m_iShowCursor = 1;
		m_fCursorBlinkTime = m_fCursorBlinkDelay;

		if ( fRightTimer <= 0 ) fRightTimer = 0.5f;
	}

	// up repeat
	static float fUpTimer = 0;
	bool bUpRepeat = false;
	if ( !agk::GetRawKeyState( AGK_KEY_UP ) ) fUpTimer = 0;

	if ( fUpTimer > 0 )
	{
		fUpTimer -= agk::GetFrameTime();
		if ( fUpTimer <= 0 ) 
		{
			fUpTimer = 0.05f;
			bUpRepeat = true;
		}
	}

	// up arrow pressed
	if ( agk::GetRawKeyPressed( AGK_KEY_UP ) || bUpRepeat )
	{
		SetCursorLocation( m_pInputCursor->GetX(), m_pInputCursor->GetY() - m_pInputText->GetSize()*0.5f - m_pInputText->GetLineSpacing() );
		m_iShowCursor = 1;
		m_fCursorBlinkTime = m_fCursorBlinkDelay;

		if ( fUpTimer <= 0 ) fUpTimer = 0.5f;
	}

	// down repeat
	static float fDownTimer = 0;
	bool bDownRepeat = false;
	if ( !agk::GetRawKeyState( AGK_KEY_DOWN ) ) fDownTimer = 0;

	if ( fDownTimer > 0 )
	{
		fDownTimer -= agk::GetFrameTime();
		if ( fDownTimer <= 0 ) 
		{
			fDownTimer = 0.05f;
			bDownRepeat = true;
		}
	}

	// down arrow pressed
	if ( agk::GetRawKeyPressed( AGK_KEY_DOWN ) || bDownRepeat )
	{
		SetCursorLocation( m_pInputCursor->GetX(), m_pInputCursor->GetY() + m_pInputText->GetSize()*1.5f + m_pInputText->GetLineSpacing() );
		m_iShowCursor = 1;
		m_fCursorBlinkTime = m_fCursorBlinkDelay;

		if ( fDownTimer <= 0 ) fDownTimer = 0.5f;
	}

	// end pressed
	if ( agk::GetRawKeyPressed( AGK_KEY_END ) )
	{
		if ( !m_bMultiLine ) 
		{
			m_iCursorPos = m_sCurrInput.GetNumChars();	
		}
		else
		{
			SetCursorLocation( m_pInputCursor->GetX() + 10000, m_pInputCursor->GetY() + m_pInputText->GetSize()*0.5f );
		}
		m_iShowCursor = 1;
		m_fCursorBlinkTime = m_fCursorBlinkDelay;
	}

	// home pressed
	if ( agk::GetRawKeyPressed( AGK_KEY_HOME ) )
	{
		if ( !m_bMultiLine ) 
		{
			m_iCursorPos = 0;
		}
		else
		{
			SetCursorLocation( m_pInputCursor->GetX() - 10000, m_pInputCursor->GetY() + m_pInputText->GetSize()*0.5f );
		}
		m_iShowCursor = 1;
		m_fCursorBlinkTime = m_fCursorBlinkDelay;
	}
    
    if ( !m_bSupportsExtendedAscii && (m_bFlags & AGK_EDITBOX_USING_NEW_FONT) == 0 )
    {
        int length = m_sCurrInput.GetNumChars();
        m_sCurrInput.StripUTF8();
        if ( length != m_sCurrInput.GetNumChars() )
        {
            if ( !m_bSupportWarned )
            {
                m_bSupportWarned = true;
                agk::Message("This edit box does not support extended characters, use SetEditBoxExtendedFontImage to add this");
            }
        }
    }

	m_pInputText->SetString( m_sCurrInput );
	while ( m_iMaxLines > 0 && m_pInputText->GetLines() > m_iMaxLines )
	{
		m_sCurrInput.Trunc2( 1 );
		m_pInputText->SetString( m_sCurrInput );
	}

	PlatformUpdateTextEnd();
}

void cEditBox::Draw()
{
	if ( !m_bVisible ) return;

	// calculate scissor
	int iScissorX = 0;
	int iScissorY = 0;
	int iScissorWidth = 0;
	int iScissorHeight = 0;

	if ( (m_fScissorX2 == m_fScissorX) && (m_fScissorY == m_fScissorY2) )
	{
		agk::ResetScissor();
	}
	else
	{
		float fX = m_fScissorX;
		float fY = m_fScissorY;
		float fX2 = m_fScissorX2;
		float fY2 = m_fScissorY2;

		if ( !m_bFixed )
		{
			fX = agk::WorldToScreenX( fX );
			fY = agk::WorldToScreenY( fY );

			fX2 = agk::WorldToScreenX( fX2 );
			fY2 = agk::WorldToScreenY( fY2 );
		}

		// swap Y values for viewport
		if ( agk::m_bUsingFBO )
		{
			iScissorX = agk::ScreenToViewportX( fX );
			iScissorY = agk::ScreenToViewportY( fY );
			iScissorWidth = agk::ScreenToViewportX( fX2 ) - iScissorX;
			iScissorHeight = agk::ScreenToViewportY( fY2 ) - iScissorY;
		}
		else
		{
			iScissorX = agk::ScreenToViewportX( fX );
			iScissorY = agk::ScreenToViewportY( fY2 );
			iScissorWidth = agk::ScreenToViewportX( fX2 ) - iScissorX;
			iScissorHeight = agk::ScreenToViewportY( fY ) - iScissorY;
		}

		agk::PlatformScissor( iScissorX, iScissorY, iScissorWidth, iScissorHeight );
	}
	
	// draw
	m_pInputBorder->Draw();
	m_pInputArea->Draw();

	m_fCursorBlinkTime -= agk::GetFrameTime();
	if ( m_fCursorBlinkTime <= 0 )
	{
		m_iShowCursor = 1 - m_iShowCursor;
		m_fCursorBlinkTime = m_fCursorBlinkDelay;
	}

	float fX = m_fX;
	float fY = m_fY;
	float fX2 = m_fX + m_fWidth;
	float fY2 = m_fY + m_fHeight;

	if ( (m_fScissorX2 != m_fScissorX) && (m_fScissorY != m_fScissorY2) )
	{
		if ( fX > m_fScissorX2 || fX2 < m_fScissorX || fY > m_fScissorY2 || fY2 < m_fScissorY )
		{
			//both scissor boxes are mutually exclusive, so nothing will be drawn.
			agk::ResetScissor();
			return;
		}
		
		// merge the edit box scissor area with the text scissor area, should result in the intersection of the two.
		if ( fX < m_fScissorX ) fX = m_fScissorX;
		if ( fX2 > m_fScissorX2 ) fX2 = m_fScissorX2;
		if ( fY < m_fScissorY ) fY = m_fScissorY;
		if ( fY2 > m_fScissorY2 ) fY2 = m_fScissorY2;
	}

	if ( !m_bFixed )
	{
		fX = agk::WorldToScreenX( fX );
		fY = agk::WorldToScreenY( fY );

		fX2 = agk::WorldToScreenX( fX2 );
		fY2 = agk::WorldToScreenY( fY2 );
	}

	// swap Y for viewport
	if ( agk::m_bUsingFBO )
	{
		iScissorX = agk::ScreenToViewportX( fX );
		iScissorY = agk::ScreenToViewportY( fY );
		iScissorWidth = agk::ScreenToViewportX( fX2 ) - iScissorX;
		iScissorHeight = agk::ScreenToViewportY( fY2 ) - iScissorY;
	}
	else
	{
		iScissorX = agk::ScreenToViewportX( fX );
		iScissorY = agk::ScreenToViewportY( fY2 );
		iScissorWidth = agk::ScreenToViewportX( fX2 ) - iScissorX;
		iScissorHeight = agk::ScreenToViewportY( fY ) - iScissorY;
	}

	agk::PlatformScissor( iScissorX, iScissorY, iScissorWidth, iScissorHeight );

	float posX = m_pInputText->GetX();
	float posY = m_pInputText->GetY();
	if ( m_iCursorPos >= 0 )
	{
		if ( m_iCursorPos >= (int)m_pInputText->GetLength() ) 
		{
			posX += m_pInputText->GetCharX( m_pInputText->GetLength()-1 ) + m_pInputText->GetCharWidth( m_pInputText->GetLength()-1 );
			posY += m_pInputText->GetCharY( m_pInputText->GetLength()-1 );

			if ( m_sCurrInput.CharAt( m_sCurrInput.GetNumChars()-1 ) == '\n' )
			{
				posX = m_pInputText->GetX();
				posY += m_pInputText->GetSize();
			}
		}
		else 
		{
			posX += m_pInputText->GetCharX( m_iCursorPos );
			posY += m_pInputText->GetCharY( m_iCursorPos );
		}
	}

	if ( m_bMultiLine || m_iWrapMode == 1 )
	{
		if ( posY + m_pInputText->GetSize() > m_fY + m_fHeight )
		{
			float newY = m_pInputText->GetY() - ((posY + m_pInputText->GetSize()) - (m_fY + m_fHeight)) - m_fTextSize/10;
			m_pInputText->SetPosition( m_pInputText->GetX(), newY );
		}
		else if ( posY < m_fY )
		{
			float newY = m_pInputText->GetY() + (m_fY - posY) + m_fTextSize/10;
			m_pInputText->SetPosition( m_pInputText->GetX(), newY );
		}
	}
	else
	{
		float padX = m_fTextSize/5;
		float padX2 = m_fTextSize;
		if ( m_fWidth < m_fTextSize*2 )
		{
			padX2 = m_fWidth / 2;
		}

		if ( posX + padX2 > m_fX + m_fWidth )
		{
			float newX = m_pInputText->GetX() - ((posX + padX2) - (m_fX + m_fWidth));
			m_pInputText->SetPosition( newX, m_pInputText->GetY() );
		}
		else if ( posX < m_fX + padX2 )
		{
			float newX = m_pInputText->GetX() + ((m_fX + padX2) - posX);
			if ( newX > m_fX + padX/2 ) newX = m_fX + padX/2;
			m_pInputText->SetPosition( newX, m_pInputText->GetY() );
		}
	}

	if ( m_bIsPassword )
	{
		int length = m_sCurrInput.GetNumChars();
		uString pass("", length);
		for ( int i = 0; i < length; i++ ) pass.Append( "*" );
		m_pInputText->SetString( pass );
	}
	else m_pInputText->SetString( m_sCurrInput );
	m_pInputText->Draw();

	if ( m_iShowCursor && m_bActive && m_bEnabled ) 
	{
		float posX = m_pInputText->GetX();
		float posY = m_pInputText->GetY();
		if ( m_iCursorPos >= 0 )
		{
			if ( m_iCursorPos >= (int)m_pInputText->GetLength() ) 
			{
				posX += m_pInputText->GetCharX( m_pInputText->GetLength()-1 ) + m_pInputText->GetCharWidth( m_pInputText->GetLength()-1 );
				posY += m_pInputText->GetCharY( m_pInputText->GetLength()-1 );

				if ( m_sCurrInput.CharAt( m_sCurrInput.GetNumChars()-1 ) == '\n' )
				{
					posX = m_pInputText->GetX();
					posY += m_pInputText->GetSize() + m_pInputText->GetLineSpacing();
				}
			}
			else 
			{
				posX += m_pInputText->GetCharX( m_iCursorPos ) - m_fTextSize/20;
				posY += m_pInputText->GetCharY( m_iCursorPos );
			}
		}

		m_pInputCursor->SetPosition( posX, posY );
		m_pInputCursor->Draw();
	}

	agk::ResetScissor();
}
