#include "agk.h"

using namespace AGK;

// virtual button

cImage* cVirtualButton::g_pButtonImage = 0;
cImage* cVirtualButton::g_pButtonDownImage = 0;

void cVirtualButton::Zero()
{
	m_bPrevDown = false;
	m_bDown = false;
	
	m_pCapturedMouse = 0;
	m_pCapturedTouch = 0;

	m_pButtonSprite = UNDEF;
	m_pButtonDownSprite = UNDEF;
	m_pText = UNDEF;
	
	m_bActive = true;
	m_bVisible = true;
}

cVirtualButton::cVirtualButton( float x, float y, float size )
{
	Zero();

	if ( size < 1 ) size = 1;
	
	if ( g_pButtonImage == 0 ) g_pButtonImage = new cImage( "/Button.png" );

	m_pButtonSprite = new cSprite( g_pButtonImage );
	m_pButtonSprite->SetPositionByOffset( x, y );
	m_pButtonSprite->SetSize( size, -1 );
	m_pButtonSprite->SetColor( 255, 255, 255, 200 );
	m_pButtonSprite->SetDepth( 0 );
	m_pButtonSprite->FixToScreen( 1 );

	if ( g_pButtonDownImage == 0 ) g_pButtonDownImage = new cImage( "/ButtonDown.png" );

	m_pButtonDownSprite = new cSprite( g_pButtonDownImage );
	m_pButtonDownSprite->SetPositionByOffset( x, y );
	m_pButtonDownSprite->SetSize( size, -1 );
	m_pButtonDownSprite->SetColor( 255, 255, 255, 200 );
	m_pButtonDownSprite->SetDepth( 0 );
	m_pButtonDownSprite->FixToScreen( 1 );
}

cVirtualButton::~cVirtualButton()
{
	if ( m_pButtonSprite ) delete m_pButtonSprite;
	if ( m_pButtonDownSprite ) delete m_pButtonDownSprite;
	if ( m_pText ) delete m_pText;
}

void cVirtualButton::SetPosition( float x, float y )
{
	m_pButtonSprite->SetPositionByOffset( x, y );
	m_pButtonDownSprite->SetPositionByOffset( x, y );
}

void cVirtualButton::SetSize( float size )
{
	if ( size < 0 ) size = 0;
	m_pButtonSprite->SetSize( size, -1 );
	m_pButtonDownSprite->SetSize( size, -1 );
}

void cVirtualButton::SetSize( float sizeX, float sizeY )
{
	if ( sizeX < 0 ) sizeX = 0;
	if ( sizeY < 0 ) sizeY = 0;
	m_pButtonSprite->SetSize( sizeX, sizeY );
	m_pButtonDownSprite->SetSize( sizeX, sizeY );
}

void cVirtualButton::SetColor( UINT red, UINT green, UINT blue )
{
	m_pButtonSprite->SetRed( red );
	m_pButtonSprite->SetGreen( green );
	m_pButtonSprite->SetBlue( blue );

	m_pButtonDownSprite->SetRed( red );
	m_pButtonDownSprite->SetGreen( green );
	m_pButtonDownSprite->SetBlue( blue );
}

void cVirtualButton::SetAlpha( UINT alpha )
{
	m_pButtonSprite->SetAlpha( alpha );
	m_pButtonDownSprite->SetAlpha( alpha );
}

void cVirtualButton::SetActive( bool active )
{
	m_bActive = active;
}

void cVirtualButton::SetVisible( bool visible )
{
	m_bVisible = visible;
}

void cVirtualButton::SetUpImage( cImage *pImage )
{
	if ( pImage ) m_pButtonSprite->SetImage( pImage );
	else m_pButtonSprite->SetImage( g_pButtonImage );

	m_pButtonSprite->SetSize( m_pButtonSprite->GetWidth(), -1 );
}

void cVirtualButton::SetDownImage( cImage *pImage )
{
	if ( pImage ) m_pButtonDownSprite->SetImage( pImage );
	else m_pButtonDownSprite->SetImage( g_pButtonDownImage );

	m_pButtonDownSprite->SetSize( m_pButtonDownSprite->GetWidth(), -1 );
}

void cVirtualButton::SetText( const char *str )
{
	if ( !m_pText )
	{
		m_pText = new cText();
		m_pText->FixToScreen( 1 );
	}

	m_pText->SetString( str );
}

void cVirtualButton::Update()
{
	if ( !m_bActive )
	{
		m_bPrevDown = false;
		m_bDown = false;
		return;
	}

	bool input = false;

	if ( m_pCapturedMouse )
	{
		if ( !m_pCapturedMouse->GetLeftTrue() ) m_pCapturedMouse = 0;
		else
		{
			input = true;
		}
	}

	if ( m_pCapturedTouch )
	{
		if ( m_pCapturedTouch->GetReleased() ) m_pCapturedTouch = 0;
		else
		{
			input = true;
		}
	}

	m_bPrevDown = m_bDown;
	m_bDown = input;
}

bool cVirtualButton::GetHitTest( float x, float y )
{
	if ( !m_bActive ) return false;

	return m_pButtonSprite->GetHitTest( x, y );
}

void cVirtualButton::Draw()
{
	if ( !m_bVisible ) return;

	// find the correct sprite to use
	cSprite *pSprite = m_pButtonSprite;
	if ( m_bDown ) pSprite = m_pButtonDownSprite;
	
	pSprite->Draw();

	// draw text
	if ( m_pText )
	{
		m_pText->SetAlignment( 1 );
		m_pText->SetPosition( m_pButtonSprite->GetXByOffset(), m_pButtonSprite->GetYByOffset() - m_pButtonSprite->GetHeight()/8.0f );
		m_pText->SetSize( m_pButtonSprite->GetHeight() / 4.0f );
		m_pText->Draw();
	}
}

// virtual joystick

cImage *cVirtualJoystick::g_pOuterImage = 0;
cImage *cVirtualJoystick::g_pInnerImage = 0;
float cVirtualJoystick::g_fDeadZone = 0.15f;

void cVirtualJoystick::Zero()
{
	m_fX = 0;
	m_fY = 0;
	
	m_pCapturedMouse = 0;
	m_pCapturedTouch = 0;

	m_pOuterSprite = UNDEF;
	m_pInnerSprite = UNDEF;

	//m_iAlpha1 = 150;
	//m_iAlpha2 = 200;

	m_bActive = true;
	m_bVisible = true;
}

cVirtualJoystick::cVirtualJoystick( float x, float y, float size )
{
	Zero();

	if ( size < 0 ) size = 0;
	
	if ( g_pOuterImage == 0 ) g_pOuterImage = new cImage( "/JoystickOuter.png" );
	
	m_pOuterSprite = new cSprite( g_pOuterImage );
	m_pOuterSprite->SetPositionByOffset( x, y );
	m_pOuterSprite->SetSize( size, -1 );
	m_pOuterSprite->SetColor( 255,255,255,150 );
	m_pOuterSprite->SetDepth( 0 );
	m_pOuterSprite->FixToScreen( 1 );
	
	if ( g_pInnerImage == 0 ) g_pInnerImage = new cImage( "/JoystickInner.png" );

	m_pInnerSprite = new cSprite( g_pInnerImage );
	m_pInnerSprite->SetPositionByOffset( x, y );
	m_pInnerSprite->SetSize( size * 0.7f, -1 );
	m_pInnerSprite->SetColor( 255,255,255,200 );
	m_pInnerSprite->SetDepth( 0 );
	m_pInnerSprite->FixToScreen( 1 );
}

cVirtualJoystick::~cVirtualJoystick()
{
	if ( m_pOuterSprite ) delete m_pOuterSprite;
	if ( m_pInnerSprite ) delete m_pInnerSprite;
}

void cVirtualJoystick::SetPosition( float x, float y )
{
	m_pOuterSprite->SetPositionByOffset( x, y );
	m_pInnerSprite->SetPositionByOffset( x, y );
}

void cVirtualJoystick::SetSize( float size )
{
	if ( size < 0 ) size = 0;
	m_pOuterSprite->SetSize( size, -1 );
	m_pInnerSprite->SetSize( size, -1 );
}

void cVirtualJoystick::SetAlpha( UINT alpha1, UINT alpha2 )
{
	m_pOuterSprite->SetAlpha( alpha1 );
	m_pInnerSprite->SetAlpha( alpha2 );
}

void cVirtualJoystick::SetActive( bool active )
{
	m_bActive = active;
}

void cVirtualJoystick::SetVisible( bool visible )
{
	m_bVisible = visible;
}

void cVirtualJoystick::SetInnerImage( cImage *pImage )
{
	if ( pImage ) m_pInnerSprite->SetImage( pImage );
	else m_pInnerSprite->SetImage( g_pInnerImage );

	m_pInnerSprite->SetSize( m_pInnerSprite->GetWidth(), -1 );
}

void cVirtualJoystick::SetOuterImage( cImage *pImage )
{
	if ( pImage ) m_pOuterSprite->SetImage( pImage );
	else m_pOuterSprite->SetImage( g_pOuterImage );

	m_pOuterSprite->SetSize( m_pOuterSprite->GetWidth(), -1 );
}

void cVirtualJoystick::Update()
{
	if ( !m_bActive )
	{
		m_fX = 0;
		m_fY = 0;
		return;
	}

	float x = m_pOuterSprite->GetXByOffset();
	float y = m_pOuterSprite->GetYByOffset();

	if ( m_pCapturedMouse )
	{
		if ( !m_pCapturedMouse->GetLeftTrue() ) m_pCapturedMouse = 0;
		else
		{
			x = m_pCapturedMouse->GetX();
			y = m_pCapturedMouse->GetY();
		}
	}

	if ( m_pCapturedTouch )
	{
		if ( m_pCapturedTouch->GetReleased() ) m_pCapturedTouch = 0;
		else
		{
			x = m_pCapturedTouch->GetCurrentX();
			y = m_pCapturedTouch->GetCurrentY();
		}
	}

	float relX = x - m_pOuterSprite->GetXByOffset();
	float relY = y - m_pOuterSprite->GetYByOffset();

	relY /= agk::GetStretchValue();

	relX /= (m_pOuterSprite->GetWidth()/2.0f);
	relY /= (m_pOuterSprite->GetWidth()/2.0f);

	float length = relX*relX + relY*relY;
	if ( length > 1.0f )
	{
		length = agk::Sqrt( length );
		relX = relX / length;
		relY = relY / length;
	}

	m_fX = relX;
	m_fY = relY;
}

float cVirtualJoystick::GetX() 
{ 
	if ( agk::Abs( m_fX ) < g_fDeadZone ) return 0;
	else return m_fX; 
}

float cVirtualJoystick::GetY() 
{ 
	if ( agk::Abs( m_fY ) < g_fDeadZone ) return 0;
	else return m_fY; 
}

bool cVirtualJoystick::GetHitTest( float x, float y )
{
	if ( !m_bActive ) return false;

	return m_pOuterSprite->GetHitTest( x, y );
}

void cVirtualJoystick::Draw()
{
	if ( !m_bVisible ) return;

	m_pOuterSprite->Draw();

	m_pInnerSprite->SetPositionByOffset( m_pOuterSprite->GetXByOffset() + m_fX*(m_pOuterSprite->GetWidth()/8.0f), m_pOuterSprite->GetYByOffset() + (m_fY*(m_pOuterSprite->GetWidth()/8.0f))*agk::GetStretchValue() );
	m_pInnerSprite->Draw();
}

// real joystick

float cJoystick::g_fDeadZone = 0.15f;

void cJoystick::Zero()
{
	m_pDevice = UNDEF;
	m_iDeviceType = 0;

	m_iConnected = 1;

	m_fX = 0;
	m_fY = 0;
	m_fZ = 0;

	m_fRX = 0;
	m_fRY = 0;
	m_fRZ = 0;

	m_iSlider[ 0 ] = 0;
	m_iSlider[ 1 ] = 0;

	m_iPOV[ 0 ] = 0;
	m_iPOV[ 1 ] = 0;
	m_iPOV[ 2 ] = 0;
	m_iPOV[ 3 ] = 0;

	m_iNumButtons = 0;
	for ( UINT i = 0; i < AGK_MAX_JOYSTICK_BUTTONS; i++ ) m_iPrevButtons[ i ] = 0;
	for ( UINT i = 0; i < AGK_MAX_JOYSTICK_BUTTONS; i++ ) m_iButtons[ i ] = 0;
	for ( UINT i = 0; i < AGK_MAX_JOYSTICK_BUTTONS; i++ ) m_iResetButtons[ i ] = 0;
}

cJoystick::cJoystick( void *pDevice )
{
	Zero();
	
	m_pDevice = pDevice;
}

cJoystick::cJoystick( void *pDevice, int devicetype )
{
	Zero();
	
	m_pDevice = pDevice;
	m_iDeviceType = devicetype;
}

float cJoystick::GetX() 
{ 
	if ( agk::Abs(m_fX) < g_fDeadZone ) return 0;
	else return m_fX; 
}

float cJoystick::GetY() 
{ 
	if ( agk::Abs(m_fY) < g_fDeadZone ) return 0;
	else return m_fY; 
}

float cJoystick::GetZ() 
{ 
	if ( agk::Abs(m_fZ) < g_fDeadZone ) return 0;
	else return m_fZ; 
}

float cJoystick::GetRX() 
{ 
	if ( agk::Abs(m_fRX) < g_fDeadZone ) return 0;
	else return m_fRX; 
}

float cJoystick::GetRY() 
{ 
	if ( agk::Abs(m_fRY) < g_fDeadZone ) return 0;
	else return m_fRY; 
}

float cJoystick::GetRZ() 
{ 
	if ( agk::Abs(m_fRZ) < g_fDeadZone ) return 0;
	else return m_fRZ; 
}

int cJoystick::GetSlider( UINT slider )
{
	if ( slider > 1 ) return 0;
	return m_iSlider[ slider ];
}

int cJoystick::GetPOV( UINT pov )
{
	if ( pov > 1 ) return 0;
	return m_iPOV[ pov ];
}

void cJoystick::Update()
{
	for ( UINT i = 0; i < AGK_MAX_JOYSTICK_BUTTONS; i++ ) 
	{
		m_iPrevButtons[ i ] = m_iButtons[ i ];

		if ( m_iResetButtons[ i ] )
		{
			m_iButtons[ i ] = 0;
			m_iResetButtons[ i ] = 0;
		}
	}

	PlatformUpdate();
}
