// Common Include
//#include "cSprite.h"
//#include "Wrapper.h"
#include "agk.h"
#include "Box2D/Box2D.h"

// Namespace
using namespace AGK;

int cSprite::g_iPixelsDrawn = 0;
cSprite* cSprite::g_pAllSprites = UNDEF;
cSprite* cSprite::g_pLastSprite = UNDEF;

UINT cSprite::g_iCreated = 0;

void cSprite::RemoveImage( cImage *pDelImage )
{
	cSprite *pSprite = g_pAllSprites;
	while ( pSprite )
	{
		if ( pSprite->m_pImage == pDelImage )
		{
			pSprite->SwitchImage( 0 );
		}
		
		pSprite = pSprite->m_pNextSprite;
	}
}

//
// Internal functions
//

void cSprite::Reset ( void )
{
	m_iCreated = g_iCreated;
	g_iCreated++;

	m_pNextSprite = UNDEF;

	if ( g_pLastSprite ) 
	{
		m_pPrevSprite = g_pLastSprite;
		g_pLastSprite->m_pNextSprite = this;
	}
	else
	{
		m_pPrevSprite = UNDEF;
		g_pAllSprites = this;
	}

	g_pLastSprite = this;
	
	// reset all variables
	m_iID		= 0;
	m_bManaged	= false;
	m_fX		= 0;
	m_fY		= 0;
	m_fZ		= 0.001f; // 0 is top
	m_iDepth    = 10;
	m_fWidth	= 1;
	m_fHeight	= 1;
	m_fOffsetX	= 0;
	m_fOffsetY	= 0;
	m_fAngle	= 0;
	m_fOrigWidth = 1;
	m_fOrigHeight = 1;
	m_fOrigRadius = 0;
	m_bFlags = AGK_SPRITE_VISIBLE | AGK_SPRITE_ACTIVE | AGK_SPRITE_LOOP | AGK_SPRITE_SCROLL | AGK_SPRITE_MANAGE_IMAGES;
	m_iTransparencyMode = 0;
	m_fVisualRadius = 2;
	m_fColRadius = 2;

	m_pBone = 0;
	m_pSkeleton = 0;
	
	m_pImage = UNDEF;
	for ( int i = 0; i < 8; i++ ) m_pAdditionalImages[ i ] = 0;
	m_pFontImage = 0;
	m_iImageID = 0;
	//m_iImageInternalID = 0;
	m_iColor = 0xffffffff;

	m_fClipX = 0;
	m_fClipY = 0;
	m_fClipX2 = 0;
	m_fClipY2 = 0;

	//m_iUVMode = 0;
	m_fUVBorder = 0;
	m_fUOffset = 0;
	m_fVOffset = 0;
	m_fUScale = 1;
	m_fVScale = 1;

	m_bUVOverride = false;
	m_fU1 = 0;
	m_fV1 = 0;
	m_fU2 = 0;
	m_fV2 = 1;
	m_fU2 = 1;
	m_fV2 = 0;
	m_fU2 = 1;
	m_fV2 = 1;

	m_iGroup = 0;
	m_iCategories = 0x0001;
	m_iCategoryMask = 0xFFFF;
	m_fPolygonPointsTemp = UNDEF;
	m_iPolygonPointsNum = 0;

	// if m_iFrameCount == 0 then there is no sprite animation, use full image as a texture
	m_iFrameCount = 0;
	m_iFrameArraySize = 0;
	m_iFrameWidth = 0;
	m_iFrameHeight = 0;
	m_iCurrentFrame = 0;
	m_iFrameStart = 0;
	m_iFrameEnd = 0;
	m_fFrameTimer = 0.0f;
	m_fFrameChangeTime = 0;
	m_pFrames = UNDEF;
	
	m_pUserData = UNDEF;
	m_pUserInts = 0;
	m_pUserFloats = 0;
	m_pUserStrings = 0;
	m_iNumUserInts = 0;
	m_iNumUserFloats = 0;
	m_iNumUserStrings = 0;

	// shader variables
	m_pShader = AGKShader::g_pShaderColor;

	m_phyBody = UNDEF;
	m_phyShape = UNDEF;
	m_phyAdditionalShapes = 0;
	m_iNumAdditionalShapes = 0;
	m_eShape = eNone;
	m_colResult = UNDEF;
	m_lastContact = UNDEF;
	m_pContactIter = UNDEF;
}

//****if* cSprite/Sprite
// FUNCTION
//   Creates an empty sprite 
// SOURCE
cSprite::cSprite() : m_cShaderVariables( 32 )
//****
{
	// reset sprite
	Reset();
	SetSize( -1, -1 );
	SetPosition( 0,0 );

	m_colResult = new b2DistanceOutput();
}

//****if* cSprite/Sprite
// FUNCTION
//   Creates a sprite from a specified image. See SetDimensions for width and height calculation info.
// INPUTS
//   szImage -- a path to an image stored in the devices media folder, the image will be local to this sprite only and deleted when the sprite is deleted.
//   fWidth -- the width to use for the sprite, use -1 to have this value calculated.
//   fHeight -- the height to use for the sprite, use -1 to have this value calculated.
// SOURCE
cSprite::cSprite( const uString &szImage ) : m_cShaderVariables( 32 )
//****
{
	// reset sprite and assign image name
	Reset();
	SetImage(szImage);
	SetSize( -1, -1 );
	SetPosition( 0,0 );

	m_colResult = new b2DistanceOutput();
}

//****if* cSprite/Sprite
// FUNCTION
//   Creates a sprite from an existing image. 
// INPUTS
//   pImage -- a pointer to a previously loaded image, the image can be shared with any number of sprites, it will not be modified when the sprite is deleted.
// SOURCE
cSprite::cSprite( cImage *pImage ) : m_cShaderVariables( 32 )
//****
{
	// reset sprite and assign image name
	Reset();
	SetImage(pImage);
	SetSize( -1, -1 );
	SetPosition( 0,0 );

	m_colResult = new b2DistanceOutput();
}

cSprite::cSprite( const cSprite *pOtherSprite )
{
	memcpy( this, pOtherSprite, sizeof(cSprite) );

	m_iCreated = g_iCreated;
	g_iCreated++;

	// check for non shared image and create another copy
	if ( m_pImage && (m_bFlags & AGK_SPRITE_SHAREDIMAGE) == 0 )
	{
		if ( m_pImage->GetPath() && strlen( m_pImage->GetPath() ) > 0 )
		{
			m_pImage = new cImage( m_pImage->GetPath() );
			static int warned = 0;
			if ( warned == 0 )
			{
				warned = 1;
				agk::Warning( "Warning, cloning a sprite created with LoadSprite will load the image multiple times, use LoadImage and CreateSprite instead for better performance" );
			}
		}
		else
		{
			// todo find another way to clone the image
			m_pImage = 0;
		}
	}

	if ( m_pFontImage ) m_pFontImage->m_iRefCount++;

	CheckTransparency();

	// register with image
	if ( m_bFlags & AGK_SPRITE_MANAGE_IMAGES )
	{
		if ( m_pImage ) m_pImage->AddSprite( this );
		for ( int i = 0; i < 8; i++ ) 
		{
			if ( m_pAdditionalImages[ i ] ) m_pAdditionalImages[ i ]->AddSprite( this );
		}
	}

	// clear any temp polygon points
	m_fPolygonPointsTemp = 0;
	m_iPolygonPointsNum = 0;

	// copy animation frames
	if ( m_pFrames )
	{
		m_pFrames = new cSpriteFrame[ m_iFrameArraySize ];
		for ( int i = 0; i < m_iFrameCount; i++ )
		{
			m_pFrames[ i ] = pOtherSprite->m_pFrames[ i ];
			if ( pOtherSprite->m_pFrames[ i ].m_pFrameImage == pOtherSprite->m_pImage )
			{
				m_pFrames[ i ].m_pFrameImage = m_pImage;
			}
			else
			{
				if ( m_pFrames[ i ].m_pFrameImage && (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) ) m_pFrames[ i ].m_pFrameImage->AddSprite( this );
			}
		}
	}

	m_pPrevSprite = 0;
	m_pNextSprite = 0;
	m_pUserData = 0;
	m_pUserInts = 0;
	m_pUserFloats = 0;
	m_pUserStrings = 0;
	m_iNumUserInts = 0;
	m_iNumUserFloats = 0;
	m_iNumUserStrings = 0;

	if ( g_pLastSprite ) 
	{
		m_pPrevSprite = g_pLastSprite;
		g_pLastSprite->m_pNextSprite = this;
	}
	else
	{
		m_pPrevSprite = UNDEF;
		g_pAllSprites = this;
	}
	g_pLastSprite = this;

	// set physics values
	// can't clone body!
	m_phyBody = 0;
	m_colResult = new b2DistanceOutput();
	m_lastContact = 0;
	m_pContactIter = 0;
	if ( pOtherSprite->m_phyShape ) m_phyShape = pOtherSprite->m_phyShape->CloneHeap();

	if ( pOtherSprite->m_iNumAdditionalShapes == 0 || pOtherSprite->m_phyAdditionalShapes == 0 )
	{
		m_iNumAdditionalShapes = 0;
		m_phyAdditionalShapes = 0;
	}
	else
	{
		m_iNumAdditionalShapes = pOtherSprite->m_iNumAdditionalShapes;
		m_phyAdditionalShapes = new b2Shape*[ m_iNumAdditionalShapes ];
		for( int i = 0; i < (int)pOtherSprite->m_iNumAdditionalShapes; i++ )
		{
			m_phyAdditionalShapes[i] = pOtherSprite->m_phyAdditionalShapes[i]->CloneHeap();
		}
	}
}

cSprite::~cSprite()
{
	if ( m_pSkeleton ) m_pSkeleton->RemoveExternalSprite( this );

	// remove from any tweens
	TweenInstance::DeleteTarget( this );

	// remove from global sprite list
	if ( m_pNextSprite )
	{
		m_pNextSprite->m_pPrevSprite = m_pPrevSprite;
	}
	else
	{
		g_pLastSprite = m_pPrevSprite;
	}

	if ( m_pPrevSprite )
	{
		m_pPrevSprite->m_pNextSprite = m_pNextSprite;
	}
	else
	{
		g_pAllSprites = m_pNextSprite;
	}

	// delete any non-shared image
	if ( (m_bFlags & AGK_SPRITE_SHAREDIMAGE) == 0 )
	{
		if ( m_pImage != UNDEF && !m_pImage->IsDeleting() ) delete m_pImage;
		m_pImage = UNDEF;
	}
	else
	{
		if ( m_pImage && (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) ) m_pImage->RemoveSprite( this );
	}

	if ( m_bFlags & AGK_SPRITE_MANAGE_IMAGES )
	{
		for ( int i = 0; i < 8; i++ )
		{
			if ( m_pAdditionalImages[ i ] ) m_pAdditionalImages[ i ]->RemoveSprite( this );
		}
	}

	if ( m_pFontImage ) m_pFontImage->Release();
	m_pFontImage = 0;

	/*
	// BAG hack for LoadSprite command
	// delete any internal image number (created with LoadSprite command)
	if ( m_iImageInternalID != 0 )
	{
		agk::DeleteImage(m_iImageInternalID);
		m_iImageInternalID = 0;
	}
	*/

	// delete any animation frames
	if ( m_pFrames != UNDEF ) 
	{
		if ( (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) )
		{
			cImage *pLast = m_pImage;
			for ( int i = 0; i < m_iFrameCount; i++ )
			{
				if ( m_pFrames[ i ].m_pFrameImage != pLast && m_pFrames[ i ].m_pFrameImage ) 
				{
					m_pFrames[ i ].m_pFrameImage->RemoveSprite( this );
					pLast = m_pFrames[ i ].m_pFrameImage;
				}
			}
		}

		delete [] m_pFrames;
	}

	if ( m_phyBody ) 
	{
		PrepareToClearPhysicsContacts();
		agk::m_phyWorld->DestroyBody( m_phyBody );
	}
	if ( m_phyShape ) delete m_phyShape;
	if ( m_phyAdditionalShapes ) 
	{
		for(int i = 0; i < (int)m_iNumAdditionalShapes; i++ ) delete m_phyAdditionalShapes[i];
		delete [] m_phyAdditionalShapes;
	}
	if ( m_colResult ) delete m_colResult;

	if ( m_fPolygonPointsTemp ) delete [] m_fPolygonPointsTemp;
}

void cSprite::ImageDeleting( cImage *pImage )
{
	if ( m_pImage == pImage ) SwitchImage(0);

	for ( int i = 0; i < m_iFrameCount; i++ )
	{
		if ( m_pFrames[ i ].m_pFrameImage == pImage ) m_pFrames[ i ].m_pFrameImage = 0;
	}
}

void cSprite::RecalcVisualRadius()
{
	// recalculate visual radius
	float furthestX = 0;
	float furthestY = 0;
	if ( m_fOffsetX < m_fWidth/2 ) furthestX = m_fWidth;
	if ( m_fOffsetY < m_fHeight/2 ) furthestY = m_fHeight;
	
	float diffX = furthestX - m_fOffsetX;
	float diffY = furthestY - m_fOffsetY;
	
	m_fVisualRadius = agk::Sqrt( diffX*diffX + diffY*diffY );
	
	if ( agk::GetStretchValue() != 1 )
	{
		diffX /= agk::GetStretchValue();
		diffY *= agk::GetStretchValue();
		float stretchedRadius = agk::Sqrt( diffX*diffX + diffY*diffY );
		if ( stretchedRadius > m_fVisualRadius ) m_fVisualRadius = stretchedRadius;
	}
}

void cSprite::RecalcColRadius()
{
	// recalculate collision radius
	if ( !m_phyShape )
	{
		float furthestX = 0;
		float furthestY = 0;
		if ( m_fOffsetX < m_fWidth/2 ) furthestX = m_fWidth;
		if ( m_fOffsetY < m_fHeight/2 ) furthestY = m_fHeight;
		
		float diffX = agk::WorldToPhyX(furthestX - m_fOffsetX);
		float diffY = agk::WorldToPhyY(furthestY - m_fOffsetY);
		
		m_fColRadius = agk::Sqrt( diffX*diffX + diffY*diffY );
	}
	else
	{
		m_fColRadius = 0;

		for( int i = -1; i < (int)m_iNumAdditionalShapes; i++ )
		{
			b2Shape *pShape;
			if ( i == -1 ) pShape = m_phyShape;
			else pShape = m_phyAdditionalShapes[i];

			switch( pShape->GetType() )
			{
				case b2Shape::e_circle:
				{
					b2CircleShape *pCircleShape = (b2CircleShape*)pShape;
					float radius = agk::Sqrt( pCircleShape->m_p.x*pCircleShape->m_p.x + pCircleShape->m_p.y*pCircleShape->m_p.y ) + pCircleShape->m_radius;
					if ( radius > m_fColRadius ) m_fColRadius = radius;
					break;
				}
				case b2Shape::e_polygon:
				{
					b2PolygonShape *pPolyShape = (b2PolygonShape*)pShape;

					float maxDist = 0;
					for ( int i = 0; i < pPolyShape->m_vertexCount; i++ )
					{
						float x = pPolyShape->m_vertices[ i ].x;// - agk::WorldToPhyX(m_fOffsetX);
						float y = pPolyShape->m_vertices[ i ].y;// - agk::WorldToPhyY(m_fOffsetY);

						float dist = x*x + y*y;
						if ( dist > maxDist ) maxDist = dist;
					}
					
					float radius = agk::Sqrt( maxDist );
					if ( radius > m_fColRadius ) m_fColRadius = radius;
					break;
				}
				case b2Shape::e_chain:
				{
					b2ChainShape *pChainShape = (b2ChainShape*)pShape;

					float maxDist = 0;
					for ( int i = 0; i < pChainShape->GetVertexCount(); i++ )
					{
						float x = pChainShape->GetVertex( i ).x;// - agk::WorldToPhyX(m_fOffsetX);
						float y = pChainShape->GetVertex( i ).y;// - agk::WorldToPhyY(m_fOffsetY);

						float dist = x*x + y*y;
						if ( dist > maxDist ) maxDist = dist;
					}
					
					float radius = agk::Sqrt( maxDist );
					if ( radius > m_fColRadius ) m_fColRadius = radius;
					break;
				}
				case b2Shape::e_edge:
				{
					b2EdgeShape *pEdgeShape = (b2EdgeShape*)pShape;
					float dist = pEdgeShape->m_vertex1.x*pEdgeShape->m_vertex1.x + pEdgeShape->m_vertex1.y*pEdgeShape->m_vertex1.y;
					float distY = pEdgeShape->m_vertex2.x*pEdgeShape->m_vertex2.x + pEdgeShape->m_vertex2.y*pEdgeShape->m_vertex2.y;
					if ( distY > dist ) dist = distY;
					float radius = agk::Sqrt( dist );
					if ( radius > m_fColRadius ) m_fColRadius = radius;
					break;
				}
				default: agk::Error( "Unsupported Box2D shape" );
			}
		}
	}
}

UINT cSprite::GetCreated ( void )			
{ 
	return m_iCreated; 
}

//****if* cSprite/GetID
// FUNCTION
//   Returns the ID used by the agk class.
// SOURCE
UINT cSprite::GetID ( void )			
//****
{ 
	return m_iID; 
}

//****if* cSprite/GetX
// FUNCTION
//   Returns the current X coordinate of the unrotated top left corner of the sprite.
// SOURCE
float cSprite::GetX ( void )			
//****
{ 
	return m_fX - m_fOffsetX; 
}

//****if* cSprite/GetY
// FUNCTION
//   Returns the current Y coordinate of the unrotated top left corner of the sprite.
// SOURCE
float cSprite::GetY ( void )	
//****
{ 
	return m_fY - m_fOffsetY; 
}

float cSprite::GetZ ( void )	
{ 
	return m_fZ;
}

//****if* cSprite/GetXByOffset
// FUNCTION
//   Returns the current X coordinate of the sprites offset point on the screen.
// SOURCE
float cSprite::GetXByOffset ( void )			
//****
{ 
	return m_fX; 
}

//****if* cSprite/GetYByOffset
// FUNCTION
//   Returns the current Y coordinate of the sprites offset point on the screen.
// SOURCE
float cSprite::GetYByOffset ( void )	
//****
{ 
	return m_fY; 
}

//****if* cSprite/GetOffsetX
// FUNCTION
//   Returns the current X offset of the sprite.
// SOURCE
float cSprite::GetOffsetX ( void )			
//****
{ 
	return m_fOffsetX; 
}

//****if* cSprite/GetOffsetY
// FUNCTION
//   Returns the current Y offset of the sprite.
// SOURCE
float cSprite::GetOffsetY ( void )	
//****
{ 
	return m_fOffsetY; 
}

//****if* cSprite/GetAngle
// FUNCTION
//   Returns the current angle of the sprite in degrees. An angle of 90 would be a 90 degree clockwise rotation of the sprite.
// SOURCE
float cSprite::GetAngle ( void )	
//****	
{ 
	return m_fAngle * (180.0f/PI); 
}

//****if* cSprite/GetAngleRad
// FUNCTION
//   Returns the current angle of the sprite in radians. An angle of PI/2 radians would be a 90 degree clockwise rotation of the sprite.
// SOURCE
float cSprite::GetAngleRad ( void )	
//****
{ 
	return m_fAngle; 
}

//****if* cSprite/GetWidth
// FUNCTION
//   Returns the current width of the sprite in world coordinates. Rotation does not affect the width value returned.
//   If the sprite has been scaled the width value returned will be scaled from it's original size.
// SOURCE
float cSprite::GetWidth ( void )	
//****	
{ 
	return m_fWidth;
}

//****if* cSprite/GetHeight
// FUNCTION
//   Returns the current height of the sprite in world coordinates. Rotation does not affect the height value returned. 
//   If the sprite has been scaled the height value returned will be scaled from it's original size.
// SOURCE
float cSprite::GetHeight ( void )	
//****
{ 
	return m_fHeight; 
}

float cSprite::GetScaleX( void )	
{ 
	return m_fWidth / m_fOrigWidth; 
}

float cSprite::GetScaleY( void )	
{ 
	return m_fHeight / m_fOrigHeight; 
}

//****if* cSprite/GetDepth
// FUNCTION
//   Returns the current depth of the sprite between 0-10000. 0 being the front of the screen, 10000 being the back.
// SOURCE
int cSprite::GetDepth ( void )		
//****
{ 
	// correct for floating point inaccuracies
	//return agk::Floor((m_fZ+0.0000001f) * 10000.0f);
	return m_iDepth;
}

//****if* cSprite/GetFrameCount
// FUNCTION
//   Returns the number of animation frames this sprite has, 0 for no animation.
// SOURCE
int cSprite::GetFrameCount ( void )	
//****
{ 
	return m_iFrameCount; 
}

//****if* cSprite/GetCurrentFrame
// FUNCTION
//   Returns the index of current animation frame of the sprite.
// SOURCE
int cSprite::GetCurrentFrame ( void ) 
//****
{ 
	return m_iCurrentFrame + 1; 
}

//****if* cSprite/GetTransparencyMode
// FUNCTION
//   Returns the current transparency mode of the sprite, 0=no transparency, 1=alpha channel transparency, 2=alpha mask (on/off) transparency.
// SOURCE
int cSprite::GetTransparencyMode ( void ) 
//****
{ 
	return m_iTransparencyMode; 
}

//****if* cSprite/GetImagePtr
// FUNCTION
//   Returns a pointer to the current image being used on the sprite.
// SOURCE
cImage* cSprite::GetImagePtr ( void ) 
//****
{ 
	return m_pImage; 
}

//****if* cSprite/GetDepthChanged
// FUNCTION
//   Returns true if the depth has changed since this function was lasted called. Used by the sprite manager class.
//   Clears the depth changed flag
// SOURCE
bool cSprite::GetDepthChanged( void ) 
//****
{ 
	bool bResult = (m_bFlags & AGK_SPRITE_DEPTHCHANGED) != 0; 
	m_bFlags &= ~AGK_SPRITE_DEPTHCHANGED;
	return bResult; 
}

//****if* cSprite/CheckDepthChanged
// FUNCTION
//   Returns true if the depth has changed since this function was lasted called. Used by the sprite manager class.
//   Does not clear the depth changed flag
// SOURCE
bool cSprite::CheckDepthChanged( void ) 
//****
{ 
	bool bResult = (m_bFlags & AGK_SPRITE_DEPTHCHANGED) != 0; 
	return bResult; 
}

//****if* cSprite/GetTextureChanged
// FUNCTION
//   Returns true if the texture has changed since this function was lasted called. Used by the sprite manager class.
//   Clears the changed flag
// SOURCE
bool cSprite::GetTextureChanged( void ) 
//****
{ 
	bool bResult = (m_bFlags & AGK_SPRITE_TEXCHANGED) != 0; 
	m_bFlags &= ~AGK_SPRITE_TEXCHANGED;
	return bResult; 
}

//****if* cSprite/CheckTextureChanged
// FUNCTION
//   Returns true if the texture has changed since this function was lasted called. Used by the sprite manager class.
//   Does not clear the changed flag
// SOURCE
bool cSprite::CheckTextureChanged( void ) 
//****
{ 
	bool bResult = (m_bFlags & AGK_SPRITE_TEXCHANGED) != 0; 
	return bResult; 
}

//****if* cSprite/GetTransparencyChanged
// FUNCTION
//   Returns true if the transparency mode has changed since this function was lasted called. Used by the sprite manager class.
// SOURCE
bool cSprite::GetTransparencyChanged( void ) 
//****
{ 
	bool bResult = (m_bFlags & AGK_SPRITE_TRANSCHANGED) != 0; 
	m_bFlags &= ~AGK_SPRITE_TRANSCHANGED;
	return bResult; 
}

//****if* cSprite/GetVisible
// FUNCTION
//   Returns true if the sprite is currently visible.
// SOURCE
bool cSprite::GetVisible( void )
//****		
{ 
	return (m_bFlags & AGK_SPRITE_VISIBLE) != 0; 
}

bool cSprite::GetInScreen( void )
//****		
{ 
	//float stretch = agk::m_fStretchValue;
	
	/*
	float fMinX = 10000000;
	float fMinY = 10000000;
	float fMaxX = -10000000;
	float fMaxY = -10000000;

	float x = m_fX;
	float y = m_fY;

	if ( (m_bFlags & AGK_SPRITE_SCROLL) > 0 )
	{
		x = agk::WorldToScreenX( x );
		y = agk::WorldToScreenY( y );
	}
	
	if ( x < fMinX ) fMinX = x;
	if ( x > fMaxX ) fMaxX = x;
	if ( y < fMinY ) fMinY = y;
	if ( y > fMaxY ) fMaxY = y;

	fTempX = (-m_fOffsetX);
	fTempY = (-m_fOffsetY + m_fHeight);

	x = m_fX + (fTempX*fCosA - fTempY*fSinA/stretch);
	y = m_fY + (fTempY*fCosA + fTempX*fSinA*stretch);

	if ( (m_bFlags & AGK_SPRITE_SCROLL) > 0 )
	{
		x = agk::WorldToScreenX( x );
		y = agk::WorldToScreenY( y );
	}

	if ( x < fMinX ) fMinX = x;
	if ( x > fMaxX ) fMaxX = x;
	if ( y < fMinY ) fMinY = y;
	if ( y > fMaxY ) fMaxY = y;

	fTempX = (-m_fOffsetX + m_fWidth);
	fTempY = (-m_fOffsetY);

	x = m_fX + (fTempX*fCosA - fTempY*fSinA/stretch);
	y = m_fY + (fTempY*fCosA + fTempX*fSinA*stretch);

	if ( (m_bFlags & AGK_SPRITE_SCROLL) > 0 )
	{
		x = agk::WorldToScreenX( x );
		y = agk::WorldToScreenY( y );
	}
	
	if ( x < fMinX ) fMinX = x;
	if ( x > fMaxX ) fMaxX = x;
	if ( y < fMinY ) fMinY = y;
	if ( y > fMaxY ) fMaxY = y;

	fTempX = (-m_fOffsetX + m_fWidth);
	fTempY = (-m_fOffsetY + m_fHeight);

	x = m_fX + (fTempX*fCosA - fTempY*fSinA/stretch);
	y = m_fY + (fTempY*fCosA + fTempX*fSinA*stretch);

	if ( (m_bFlags & AGK_SPRITE_SCROLL) > 0 )
	{
		x = agk::WorldToScreenX( x );
		y = agk::WorldToScreenY( y );
	}
	
	if ( x < fMinX ) fMinX = x;
	if ( x > fMaxX ) fMaxX = x;
	if ( y < fMinY ) fMinY = y;
	if ( y > fMaxY ) fMaxY = y;
	 
	if ( fMaxX < 0 ) return false;
	if ( fMaxY < 0 ) return false;
	if ( fMinX > agk::GetVirtualWidth() ) return false;
	if ( fMinY > agk::GetVirtualHeight() ) return false;
	 */

	float fX2 = m_fX;
	float fY2 = m_fY;
	float radius = m_fVisualRadius;

	if ( m_pBone )
	{
		fX2 = m_pBone->m00 * m_fX + m_pBone->m01 * m_fY + m_pBone->worldX;
		fY2 = m_pBone->m10 * m_fX + m_pBone->m11 * m_fY + m_pBone->worldY;

		float largestScale = m_pBone->worldSX;
		if ( m_pBone->worldSY > m_pBone->worldSX ) largestScale = m_pBone->worldSY;
		radius = m_fVisualRadius * largestScale;
	}
	
	if ( (m_bFlags & AGK_SPRITE_SCROLL) > 0 )
	{
		float x = agk::WorldToScreenX( fX2 + radius );
		if ( x < agk::GetScreenBoundsLeft() ) return false;
		
		x = agk::WorldToScreenX( fX2 - radius );
		if ( x > agk::GetScreenBoundsRight() ) return false;
		
		float y = agk::WorldToScreenY( fY2 + radius );
		if ( y < agk::GetScreenBoundsTop() ) return false;
		
		y = agk::WorldToScreenY( fY2 - radius );
		if ( y > agk::GetScreenBoundsBottom() ) return false;
	}
	else
	{
		float x = fX2 + radius;
		if ( x < agk::GetScreenBoundsLeft() ) return false;
		
		x = fX2 - radius;
		if ( x > agk::GetScreenBoundsRight() ) return false;
			
		float y = fY2 + radius;
		if ( y < agk::GetScreenBoundsTop() ) return false;
		
		y = fY2 - radius;
		if ( y > agk::GetScreenBoundsBottom() ) return false;
	}
	
	return true; 
}

//****if* cSprite/GetActive
// FUNCTION
//   Returns true if the sprite is currently set to update its animation, physics, and other properties.
// SOURCE
bool cSprite::GetActive( void )
//****		
{ 
	return (m_bFlags & AGK_SPRITE_ACTIVE) != 0; 
}

//****if* cSprite/GetHitTest
// FUNCTION
//   Returns true if the point x,y in world coordinates lies within the sprite. Takes into account the rotation and offset values of the sprite.
// SOURCE
bool cSprite::GetHitTest( float x, float y )
//****
{
	if ( (m_bFlags & AGK_SPRITE_SCROLL) == 0 )
	{
		x = agk::WorldToScreenX( x );
		y = agk::WorldToScreenY( y );
	}

	// scissor test
	if ( GetScissorOn() )
	{
		if ( x < m_fClipX || x > m_fClipX2 || y < m_fClipY || y > m_fClipY2 ) return false;
	}

	// quick reject
	float diffX = agk::WorldToPhyX(m_fX - x);
	float diffY = agk::WorldToPhyY(m_fY - y);
	if ( diffX*diffX + diffY*diffY > m_fColRadius*m_fColRadius ) return false;

	if ( m_phyShape )
	{
		b2Shape *pShape;
		b2Transform xf;
		xf.Set( b2Vec2(agk::WorldToPhyX(m_fX), agk::WorldToPhyY(m_fY)), m_fAngle );
		for( int i = -1; i < (int)m_iNumAdditionalShapes; i++ )
		{
			if ( i == -1 ) pShape = m_phyShape;
			else pShape = m_phyAdditionalShapes[i];

			if ( pShape->TestPoint( xf, b2Vec2(agk::WorldToPhyX(x), agk::WorldToPhyY(y)) ) ) return true;
		}

		return false;
	}
	else
	{
		float fNewX, fNewY;
		if ( m_fAngle != 0 )
		{
			float stretch = agk::m_fStretchValue;

			float fSinA = agk::SinRad(m_fAngle);
			float fCosA = agk::CosRad(m_fAngle);

			// transform point into sprite space
			float fTempX = x - m_fX;
			float fTempY = y - m_fY;

			fNewX = (fTempX*fCosA + fTempY*fSinA/stretch);
			fNewY = (fTempY*fCosA - fTempX*fSinA*stretch);
		}
		else
		{
			fNewX = x - m_fX;
			fNewY = y - m_fY;
		}

		fNewX += m_fOffsetX;
		fNewY += m_fOffsetY;

		return (fNewX >= 0) && (fNewY >= 0) && (fNewX <= m_fWidth) && (fNewY <= m_fHeight);
	}
}

//****if* cSprite/GetShouldCollide
// FUNCTION
//   Returns true if the two sprites should collide based on their group and category settings.
// SOURCE
bool cSprite::GetShouldCollide( cSprite *pSprite2 )
//****
{
	assert(pSprite2);

	if ( m_iGroup != 0 && m_iGroup == pSprite2->m_iGroup ) return m_iGroup > 0;

	bool collide = (m_iCategories & pSprite2->m_iCategoryMask) != 0 && (pSprite2->m_iCategories & m_iCategoryMask) != 0;
	return collide;
}

UINT cSprite::GetColorRed( )
{
	return m_iColor >> 24;
}

UINT cSprite::GetColorGreen( )
{
	return (m_iColor >> 16) & 0xff;
}

UINT cSprite::GetColorBlue( )
{
	return (m_iColor >> 8) & 0xff;
}

UINT cSprite::GetColorAlpha( )
{
	return m_iColor & 0xff;
}

int cSprite::GetScrollMode( )
{
	if ( (m_bFlags & AGK_SPRITE_SCROLL) > 0 ) return 1;
	else return 0;
}

int cSprite::GetPlaying( )
{
	if ( (m_bFlags & AGK_SPRITE_PLAYING) > 0 ) return 1;
	else return 0;
}

bool cSprite::GetScissorOn( )
{
	if ( m_fClipX2 != 0 ) return true;
	if ( m_fClipX != 0 )  return true;
	if ( m_fClipY2 != 0 ) return true;
	if ( m_fClipY != 0 ) return true;

	return false;
}


float cSprite::GetXFromPixel( int x )
{
	if ( !m_pImage ) return 0;

	return x * GetWidth() / m_pImage->GetWidth();
}

float cSprite::GetYFromPixel( int y )
{
	if ( !m_pImage ) return 0;

	return y * GetHeight() / m_pImage->GetHeight();
}

int cSprite::GetPixelFromX( float x )
{
	if ( !m_pImage ) return 0;

	return agk::Round( x * m_pImage->GetWidth() / GetWidth() );
}

int cSprite::GetPixelFromY( float y )
{
	if ( !m_pImage ) return 0;

	return agk::Round( y * m_pImage->GetHeight() / GetHeight() );
}

float cSprite::GetWorldXFromPoint( float x, float y )
{
	// test
	float fSinA = agk::SinRad(m_fAngle);
	float fCosA = agk::CosRad(m_fAngle);
	
	float worldX = m_fX + (x*fCosA - y*fSinA);
	return worldX;
}

float cSprite::GetWorldYFromPoint( float x, float y )
{
	// test
	float fSinA = agk::SinRad(m_fAngle);
	float fCosA = agk::CosRad(m_fAngle);
	
	float worldY = m_fY + (y*fCosA + x*fSinA);
	return worldY;
}

float cSprite::GetXFromWorld( float x, float y )
{
	// test
	x -= m_fX;
	y -= m_fY;

	float fSinA = agk::SinRad(m_fAngle);
	float fCosA = agk::CosRad(m_fAngle);

	float spriteX = (x*fCosA + y*fSinA);
	return spriteX;
}

float cSprite::GetYFromWorld( float x, float y )
{
	// test
	x -= m_fX;
	y -= m_fY;

	float fSinA = agk::SinRad(m_fAngle);
	float fCosA = agk::CosRad(m_fAngle);

	float spriteY = (y*fCosA - x*fSinA);
	return spriteY;
}


void cSprite::SetScale( float x, float y )
{
	// maintain the top left corner position
	float oldX = GetX();
	float oldY = GetY();
	SetScaleByOffset( x, y );
	SetPosition( oldX, oldY );
}

void cSprite::SetScaleByOffset( float x, float y )
{
	// must not be 0, dividing by scale would cause problems.
	if ( x < 0.00001f ) x = 0.00001f;
	if ( y < 0.00001f ) y = 0.00001f;

	float oldWidth = m_fWidth;
	float oldHeight = m_fHeight;

	m_fWidth = m_fOrigWidth * x;
	m_fHeight = m_fOrigHeight * y;

	// shapes use a relative scale factor, this *may* make the shape slowly drift out of sync with 
	// the true scale size if lots of scaling is done.
	float diffX = m_fWidth / oldWidth;
	float diffY = m_fHeight / oldHeight;

	m_fOffsetX = m_fOffsetX * diffX;
	m_fOffsetY = m_fOffsetY * diffY;
	
	RecalcVisualRadius();

	// scale any sprite shape
	if ( !m_phyShape ) 
	{
		RecalcColRadius();
		return;
	}

	float max = x;
	if ( y > x ) max = y;

	float diffRadius = 1.0f;
	for( int i = -1; i < (int)m_iNumAdditionalShapes; i++ )
	{
		b2Shape *pShape;
		if( i == -1 ) pShape = m_phyShape;
		else pShape = m_phyAdditionalShapes[i];

		switch( pShape->GetType() )
		{
			case b2Shape::e_polygon:
			{
				int vertexCount = ((b2PolygonShape*)pShape)->m_vertexCount;
				b2Vec2 *vertices = ((b2PolygonShape*)pShape)->m_vertices;

				for ( int i = 0; i < vertexCount; i++ )
				{
					vertices[ i ].x *= diffX;
					vertices[ i ].y *= diffY;
				}
				break;
			}
			case b2Shape::e_chain:
			{
				int vertexCount = ((b2ChainShape*)pShape)->GetVertexCount();
				b2Vec2 *vertices = ((b2ChainShape*)pShape)->GetVerticesRW();

				for ( int i = 0; i < vertexCount; i++ )
				{
					vertices[ i ].x *= diffX;
					vertices[ i ].y *= diffY;
				}
				break;
			}
			case b2Shape::e_circle:
			{
				((b2CircleShape*)pShape)->m_p.x *= diffX;
				((b2CircleShape*)pShape)->m_p.y *= diffY;

				if ( i == -1 )
				{
					float oldRadius = pShape->m_radius;
					pShape->m_radius = m_fOrigRadius * max;
					diffRadius = pShape->m_radius / oldRadius;
				}
				else
				{
					pShape->m_radius *= diffRadius;
				}
				break;
			}
			default: break;
		}
	}

	if ( m_phyBody ) m_phyBody->SetAwake(true);

	RecalcColRadius();
}

//****************************
// Set functions
//****************************

void cSprite::SetBone( Bone2D *bone )
{
	m_pBone = bone;
}

void cSprite::FixToSkeleton( Skeleton2D *skeleton, Bone2D *bone, int zorder )
{
	if ( m_pSkeleton ) m_pSkeleton->RemoveExternalSprite( this );
	m_pSkeleton = skeleton;
	if ( m_pSkeleton ) m_pSkeleton->AddExternalSprite( this, zorder );
	m_pBone = bone;
}

void cSprite::ResetSkeleton()
{
	m_pSkeleton = 0;
	m_pBone = 0;
}

void cSprite::SetName( const char* name )
{
	m_sName.SetStr( name );
}

//****if* cSprite/SetID
// FUNCTION
//   Used by the AGK to record which ID created this sprite. If not using IDs this function can be 
//   used to store any 32 bit value.
// INPUTS
//   id -- The value to store
// SOURCE
void cSprite::SetID ( UINT ID )
//****
{
	m_iID = ID;
}

//****if* cSprite/SetVisible
// FUNCTION
//   Set this sprite to be visible or not during drawing.
// INPUTS
//   bVisible -- true to make this sprite visible, false to hide it.
// SOURCE
void cSprite::SetVisible ( bool bVisible )
//****
{
	if ( bVisible ) m_bFlags |= AGK_SPRITE_VISIBLE;
	else m_bFlags &= ~AGK_SPRITE_VISIBLE;
}

//****if* cSprite/SetActive
// FUNCTION
//   Set whether this sprite will update its animation, physics, and other properties when asked to update.
// INPUTS
//   bActive -- true to make this sprite update when update is called.
// SOURCE
void cSprite::SetActive ( bool bActive )
//****
{
	if ( bActive ) m_bFlags |= AGK_SPRITE_ACTIVE;
	else m_bFlags &= ~AGK_SPRITE_ACTIVE;
}

//****if* cSprite/SetColor
// FUNCTION
//   Set this sprite's color values for use when drawing
// INPUTS
//   iRed -- The red component of the color, 0-255.
//   iGreen -- The green component of the color, 0-255.
//   iBlue -- The blue component of the color, 0-255.
//   iAlpha -- The alpha component of the color, 0-255. 255 is visible, 0 is invisible.
// SOURCE
void cSprite::SetColor ( UINT iRed, UINT iGreen, UINT iBlue, UINT iAlpha )
//****
{
	// set vertex color for sprite
	if ( iRed > 255 ) iRed = 255;
	if ( iGreen > 255 ) iGreen = 255;
	if ( iBlue > 255 ) iBlue = 255;
	if ( iAlpha > 255 ) iAlpha = 255;

	m_iColor = (((((iRed << 8) + iGreen) << 8) + iBlue) << 8) + iAlpha;

	CheckTransparency();
}

void cSprite::SetRed ( UINT iRed )
//****
{
	if ( iRed > 255 ) iRed = 255;
	
	m_iColor &= 0x00ffffff;
	m_iColor |= iRed << 24;
}

void cSprite::SetGreen ( UINT iGreen )
//****
{
	if ( iGreen > 255 ) iGreen = 255;
	
	m_iColor &= 0xff00ffff;
	m_iColor |= iGreen << 16;
}

void cSprite::SetBlue ( UINT iBlue )
//****
{
	if ( iBlue > 255 ) iBlue = 255;
	
	m_iColor &= 0xffff00ff;
	m_iColor |= iBlue << 8;
}

void cSprite::SetAlpha ( UINT iAlpha )
//****
{
	if ( iAlpha > 255 ) iAlpha = 255;
	
	m_iColor &= 0xffffff00;
	m_iColor |= iAlpha;

	CheckTransparency();
}

void cSprite::SetFlip( int horz, int vert )
{
	int currHorz = (m_bFlags & AGK_SPRITE_FLIPH) > 0 ? 1 : 0;
	int currVert = (m_bFlags & AGK_SPRITE_FLIPV) > 0 ? 1 : 0;

	if ( horz != 0 ) horz = 1;
	if ( vert != 0 ) vert = 1;

	if ( (horz ^ currHorz) == 0 && (vert ^ currVert) == 0 ) return;

	if ( horz == 0 ) m_bFlags &= ~AGK_SPRITE_FLIPH;
	else m_bFlags |= AGK_SPRITE_FLIPH;

	if ( vert == 0 ) m_bFlags &= ~AGK_SPRITE_FLIPV;
	else m_bFlags |= AGK_SPRITE_FLIPV;

	// move offset point
	if ( (horz ^ currHorz) != 0 ) m_fOffsetX = m_fWidth - m_fOffsetX;
	if ( (vert ^ currVert) != 0 ) m_fOffsetY = m_fHeight - m_fOffsetY;

	if ( !m_phyShape ) return;

	for( int i = -1; i < (int)m_iNumAdditionalShapes; i++ )
	{
		b2Shape *pShape;
		if( i == -1 ) pShape = m_phyShape;
		else pShape = m_phyAdditionalShapes[i];

		switch( pShape->GetType() )
		{
			case b2Shape::e_polygon:
			{
				int vertexCount = ((b2PolygonShape*)pShape)->m_vertexCount;
				b2Vec2 *vertices = ((b2PolygonShape*)pShape)->m_vertices;
		
				if ( ((horz ^ currHorz) ^ (vert ^ currVert)) != 0 )
				{
					// change winding order
					float temp;
					for ( int i = 0; i < vertexCount/2; i++ )
					{
						temp = vertices[ i ].x;
						vertices[ i ].x = vertices[ (vertexCount-i)-1 ].x;
						vertices[ (vertexCount-i)-1 ].x = temp;

						temp = vertices[ i ].y;
						vertices[ i ].y = vertices[ (vertexCount-i)-1 ].y;
						vertices[ (vertexCount-i)-1 ].y = temp;
					}
				}

		
				for ( int i = 0; i < vertexCount; i++ )
				{
					if ( (horz ^ currHorz) != 0 ) vertices[ i ].x = -vertices[ i ].x;
					if ( (vert ^ currVert) != 0 ) vertices[ i ].y = -vertices[ i ].y;
				}

				((b2PolygonShape*)pShape)->Set( vertices, vertexCount ); 
				break;
			}

			case b2Shape::e_chain:
			{
				int vertexCount = ((b2ChainShape*)pShape)->GetVertexCount();
				b2Vec2 *vertices = ((b2ChainShape*)pShape)->GetVerticesRW();
		
				for ( int i = 0; i < vertexCount; i++ )
				{
					if ( (horz ^ currHorz) != 0 ) vertices[ i ].x = -vertices[ i ].x;
					if ( (vert ^ currVert) != 0 ) vertices[ i ].y = -vertices[ i ].y;
				}

				break;
			}

			case b2Shape::e_circle:
			{
				if ( (horz ^ currHorz) != 0 ) ((b2CircleShape*)pShape)->m_p.x = -((b2CircleShape*)pShape)->m_p.x;
				if ( (vert ^ currVert) != 0 ) ((b2CircleShape*)pShape)->m_p.y = -((b2CircleShape*)pShape)->m_p.y;
			}
		}
	}

	// scale all additional physics shapes
	if ( m_phyBody ) m_phyBody->SetAwake(true);
}

//****if* cSprite/SetSnap
// FUNCTION
//   Turns on a special render mode that keeps the sprite on whole pixels only so it does not draw itself
//   across pixel boundaries which may cause flickering as the sprite moves across the screen.
//   As a consequence this may make the sprite appear to jump from the one pixel to the next as it moves
//   instead of smoothly moving across the screen. If the sprite has alpha blended pixels along its edge 
//   then snapping is not required and can be turned off. By default this is turned on.
// INPUTS
//   snap -- 1 to turn snapping on, 0 to turn it off
// SOURCE
void cSprite::SetSnap( int snap )
//****
{
	if ( snap > 0 ) m_bFlags = m_bFlags | AGK_SPRITE_SNAP;
	else m_bFlags = m_bFlags & (~AGK_SPRITE_SNAP);
}

void cSprite::SetManageImages( int mode )
{
	if ( mode == 0 && (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) )
	{
		if ( m_pFrames != UNDEF ) 
		{
			cImage *pLast = m_pImage;
			for ( int i = 0; i < m_iFrameCount; i++ )
			{
				if ( m_pFrames[ i ].m_pFrameImage != pLast && m_pFrames[ i ].m_pFrameImage ) 
				{
					if ( m_pFrames[ i ].m_pFrameImage ) m_pFrames[ i ].m_pFrameImage->RemoveSprite( this );
					pLast = m_pFrames[ i ].m_pFrameImage;
				}
			}
		}

		if ( m_pImage ) m_pImage->RemoveSprite( this );
	}

	if ( mode > 0 && !(m_bFlags & AGK_SPRITE_MANAGE_IMAGES) )
	{
		if ( m_pFrames != UNDEF ) 
		{
			cImage *pLast = m_pImage;
			for ( int i = 0; i < m_iFrameCount; i++ )
			{
				if ( m_pFrames[ i ].m_pFrameImage != pLast && m_pFrames[ i ].m_pFrameImage ) 
				{
					if ( m_pFrames[ i ].m_pFrameImage ) m_pFrames[ i ].m_pFrameImage->AddSprite( this );
					pLast = m_pFrames[ i ].m_pFrameImage;
				}
			}
		}

		if ( m_pImage ) m_pImage->AddSprite( this );
	}

	if ( mode > 0 ) m_bFlags = m_bFlags | AGK_SPRITE_MANAGE_IMAGES;
	else m_bFlags = m_bFlags & (~AGK_SPRITE_MANAGE_IMAGES);
}

//****if* cSprite/SetPosition
// FUNCTION
//   Immediately positions the specified sprite to the given X,Y world coordinates. The default coordinate system has 0,0 as 
//   the top left corner, and 100,100 as the bottom right hand corner of the screen. This function always positions the sprite
//   using its top left corner, regardless of the current sprite offset. The top left corner used to position the sprite does 
//   not rotate with the sprite, for example as the sprite rotates around its center the imaginary top left corner remains 
//   fixed and the sprite's position value does not change.
//
//   Changing the position of physics sprites once they have been setup is not a simple process and may cause a performance hit.
// INPUTS
//   fX -- The X coordinate to position the sprite, can use decimal values.
//   fY -- the Y coordinate to position the sprite, can use decimal values.
// SOURCE
void cSprite::SetPosition ( float fX, float fY )
//****
{
	m_fX = fX + m_fOffsetX;
	m_fY = fY + m_fOffsetY;

	m_bFlags |= AGK_SPRITE_POSTOPLEFT;

	if ( m_phyBody )
	{
		//m_phyBody->SetTransform( b2Vec2(agk::WorldToPhyX(m_fX), agk::WorldToPhyY(m_fY)), m_fAngle );
		bool bIsActive = m_phyBody->IsActive();
		PrepareToClearPhysicsContacts();
		m_phyBody->SetActive( false );
		m_phyBody->m_xf.p.Set( agk::WorldToPhyX(m_fX), agk::WorldToPhyY(m_fY) );
		m_phyBody->m_sweep.c0 = m_phyBody->m_sweep.c = b2Mul(m_phyBody->m_xf, m_phyBody->m_sweep.localCenter);
		m_phyBody->m_linearVelocity.Set( 0, 0 );
		if ( bIsActive ) 
		{
			m_phyBody->SetActive( true );
			m_phyBody->SetAwake( true );
		}
	}
}

//****if* cSprite/SetPositionByOffset
// FUNCTION
//   Immediately positions the specified sprite to the given X,Y world coordinates. The default coordinate system has 0,0 as 
//   the top left corner, and 100,100 as the bottom right hand corner of the screen. This function always positions the sprite
//   using its current offset. For example if the current offset is the center of the sprite this command will place the 
//   center of the sprite at the given coordinates.
//
//   Changing the position of physics sprites once they have been setup is not a simple process and may cause a performance hit.
// INPUTS
//   fX -- The X coordinate to position the sprite, can use decimal values.
//   fY -- the Y coordinate to position the sprite, can use decimal values.
// SOURCE
void cSprite::SetPositionByOffset ( float fX, float fY )
//****
{
	m_fX = fX;
	m_fY = fY;

	m_bFlags &= ~AGK_SPRITE_POSTOPLEFT;

	if ( m_phyBody )
	{
		//m_phyBody->SetTransform( b2Vec2(agk::WorldToPhyX(m_fX), agk::WorldToPhyY(m_fY)), m_fAngle );
		bool bIsActive = m_phyBody->IsActive();
		PrepareToClearPhysicsContacts();
		m_phyBody->SetActive( false );
		m_phyBody->m_xf.p.Set( agk::WorldToPhyX(m_fX), agk::WorldToPhyY(m_fY) );
		m_phyBody->m_sweep.c0 = m_phyBody->m_sweep.c = b2Mul(m_phyBody->m_xf, m_phyBody->m_sweep.localCenter);
		m_phyBody->m_linearVelocity.Set( 0, 0 );
		if ( bIsActive ) 
		{
			m_phyBody->SetActive( true );
			m_phyBody->SetAwake( true );
		}
	}
}

//****if* cSprite/SetX
// FUNCTION
//   Immediately positions the specified sprite to the given X coordinate. The default coordinate system has 0,0 as 
//   the top left corner, and 100,100 as the bottom right hand corner of the screen. This command positions the sprite
//   using its top left corner.
//
//   Changing the position of physics sprites once they have been setup is not a simple process and may cause a performance hit.
// INPUTS
//   fX -- The X coordinate to position the sprite, can use decimal values.
// SOURCE
void cSprite::SetX ( float fX )
//****
{
	m_fX = fX + m_fOffsetX;

	m_bFlags |= AGK_SPRITE_POSTOPLEFT;

	if ( m_phyBody )
	{
		bool bIsActive = m_phyBody->IsActive();
		PrepareToClearPhysicsContacts();
		m_phyBody->SetActive( false );
		m_phyBody->m_xf.p.Set( agk::WorldToPhyX(m_fX), agk::WorldToPhyY(m_fY) );
		m_phyBody->m_sweep.c0 = m_phyBody->m_sweep.c = b2Mul(m_phyBody->m_xf, m_phyBody->m_sweep.localCenter);
		m_phyBody->m_linearVelocity.Set( 0, 0 );
		if ( bIsActive ) 
		{
			m_phyBody->SetActive( true );
			m_phyBody->SetAwake( true );
		}
	}
}

//****if* cSprite/SetY
// FUNCTION
//   Immediately positions the specified sprite to the given Y coordinate. The default coordinate system has 0,0 as 
//   the top left corner, and 100,100 as the bottom right hand corner of the screen. This command positions the sprite
//   using its top left corner.
//
//   Changing the position of physics sprites once they have been setup is not a simple process and may cause a performance hit.
// INPUTS
//   fY -- The Y coordinate to position the sprite, can use decimal values.
// SOURCE
void cSprite::SetY ( float fY )
//****
{
	m_fY = fY + m_fOffsetY;

	m_bFlags |= AGK_SPRITE_POSTOPLEFT;

	if ( m_phyBody )
	{
		bool bIsActive = m_phyBody->IsActive();
		PrepareToClearPhysicsContacts();
		m_phyBody->SetActive( false );
		m_phyBody->m_xf.p.Set( agk::WorldToPhyX(m_fX), agk::WorldToPhyY(m_fY) );
		m_phyBody->m_sweep.c0 = m_phyBody->m_sweep.c = b2Mul(m_phyBody->m_xf, m_phyBody->m_sweep.localCenter);
		m_phyBody->m_linearVelocity.Set( 0, 0 );
		if ( bIsActive ) 
		{
			m_phyBody->SetActive( true );
			m_phyBody->SetAwake( true );
		}
	}
}

void cSprite::SetZ ( float fZ )
{
	m_fZ = fZ;
	m_iDepth = agk::Round((m_fZ+0.000001f)*10000);
	
	m_bFlags |= AGK_SPRITE_DEPTHCHANGED;
}

//****if* cSprite/SetAngle
// FUNCTION
//   Immediately rotates the sprite to the specified angle in degrees, clockwise.
// INPUTS
//   fAngle -- The angle in degrees to rotate the sprite.
// SOURCE
void cSprite::SetAngle( float fAngle )
//****
{
	m_fAngle = fAngle * (PI / 180.0f);
	m_fAngle = agk::FMod( m_fAngle, 2*PI );
	if ( m_fAngle < 0 ) m_fAngle += 2*PI;

	if ( m_phyBody )
	{
		bool bIsActive = m_phyBody->IsActive();
		PrepareToClearPhysicsContacts();
		m_phyBody->SetActive( false );
		m_phyBody->m_xf.q.Set( m_fAngle );
		m_phyBody->m_sweep.a0 = m_phyBody->m_sweep.a = m_fAngle;
		m_phyBody->m_angularVelocity = 0;
		if ( bIsActive ) 
		{
			m_phyBody->SetActive( true );
			m_phyBody->SetAwake( true );
		}
	}
}

//****if* cSprite/SetAngleRad
// FUNCTION
//   Immediately rotates the sprite to the specified angle in radians, clockwise.
// INPUTS
//   fAngle -- The angle in radians to rotate the sprite.
// SOURCE
void cSprite::SetAngleRad( float fAngle )
//****
{
	m_fAngle = fAngle;
	m_fAngle = agk::FMod( m_fAngle, 2*PI );
	if ( m_fAngle < 0 ) m_fAngle += 2*PI;

	if ( m_phyBody )
	{
		bool bIsActive = m_phyBody->IsActive();
		PrepareToClearPhysicsContacts();
		m_phyBody->SetActive( false );
		m_phyBody->m_xf.q.Set( m_fAngle );
		m_phyBody->m_sweep.a0 = m_phyBody->m_sweep.a = m_fAngle;
		m_phyBody->m_angularVelocity = 0;
		if ( bIsActive ) 
		{
			m_phyBody->SetActive( true );
			m_phyBody->SetAwake( true );
		}
	}
}

//****if* cSprite/SetDepth
// FUNCTION
//   Sets the draw order for the sprite between 0-10000, 0 being the front of the screen, 10000 being the back.
//   Anything more than 10000 will result in the sprite being clipped from view.
// INPUTS
//   iDepth -- The depth this sprite should be drawn at.
// SOURCE
void cSprite::SetDepth ( int iDepth )
//****
{
	float fNewZ = iDepth / 10000.0f;
	if ( fNewZ == m_fZ ) return;

	m_iDepth = iDepth;
	m_fZ = fNewZ;
	m_bFlags |= AGK_SPRITE_DEPTHCHANGED;
}

//****if* cSprite/SetSize
// FUNCTION
//   Sets the sprite to a new width and height. In the default coordinate system a width and height of 100,100 
//   would fill the entire drawable screen. If either width or height is set, with the other set to -1, the -1 
//   value will be recalculated to maintain the image's aspect ratio so it doesn't look stretched. If both 
//   width AND height are set to -1, then the sprite will take on the width of the assigned image and calculate 
//   its height so it isn't stretched. This function recalculates the sprite's collision shape, which can be a 
//   costly process for circle and polygon shapes. A more efficient way to change the size of a sprite is to 
//   scale it using the SetScale() function. This function resets the scale of the sprite.
// INPUTS
//   fWidth -- the width to use for the sprite, use -1 to have this value calculated.
//   fHeight -- the height to use for the sprite, use -1 to have this value calculated.
// SOURCE
void cSprite::SetSize ( float fWidth, float fHeight, bool bUpdateShape )
//****
{
	m_bFlags &= ~AGK_SPRITE_WIDTHCALC;
	m_bFlags &= ~AGK_SPRITE_HEIGHTCALC;

	// default scenario, width and height are set to 10% of device, may cause stretching
	if ( fWidth < 0 && fHeight < 0 )
	{
		if ( !m_pImage )
		{
			fWidth = 10;
		}
		else
		{
			if ( m_iFrameCount > 0 ) fWidth = (float) m_iFrameWidth;
			else fWidth = (float) m_pImage->GetWidth();// * agk::GetVirtualWidth() / (float) agk::m_iRenderWidth;
		}
		
		fHeight = -1;
		m_bFlags |= AGK_SPRITE_WIDTHCALC;
		m_bFlags |= AGK_SPRITE_HEIGHTCALC;
	}

	// width defined, height not, set height to aspect ratio of image, no stretching
	if ( fHeight < 0 )
	{
		if ( fWidth < 0.00001f ) fWidth = 0.00001f;

		float imageAspect = 1;
		if ( m_pImage ) imageAspect = m_pImage->GetWidth() / (float) m_pImage->GetHeight();
		// image aspect is different for animated sprites
		if ( m_iFrameCount > 0 ) imageAspect = m_iFrameWidth / (float) m_iFrameHeight;
		fHeight = fWidth / imageAspect;

		// adjust for a non-pixel based aspect ratio
		float realAspect = agk::GetVirtualWidth() / (float) agk::GetVirtualHeight();
		fHeight = fHeight * (agk::GetDisplayAspect() / realAspect);

		m_bFlags |= AGK_SPRITE_HEIGHTCALC;
	}

	// height defined, width not, set width to aspect ratio of image, no stretching
	if ( fWidth < 0 )
	{
		if ( fHeight < 0.00001f ) fHeight = 0.00001f;

		float imageAspect = 1;
		if ( m_pImage ) imageAspect = m_pImage->GetWidth() / (float) m_pImage->GetHeight();
		// image aspect is different for animated sprites
		if ( m_iFrameCount > 0 ) imageAspect = m_iFrameWidth / (float) m_iFrameHeight;
		fWidth = fHeight * imageAspect;

		// adjust for a non-pixel based aspect ratio
		float realAspect = agk::GetVirtualWidth() / (float) agk::GetVirtualHeight();
		fWidth = fWidth / (agk::GetDisplayAspect() / realAspect);

		m_bFlags |= AGK_SPRITE_WIDTHCALC;
	}

	if ( fWidth < 0.00001f ) fWidth = 0.00001f;
	if ( fHeight < 0.00001f ) fHeight = 0.00001f;

	float diffX = fWidth / m_fWidth;
	float diffY = fHeight / m_fHeight;

	float oldX = 0;
	float oldY = 0;
	if ( m_bFlags & AGK_SPRITE_POSTOPLEFT )
	{
		oldX = GetX();
		oldY = GetY();
	}

	// set sprite width and height
	m_fWidth  = fWidth;
	m_fHeight = fHeight;
	m_fOrigWidth = fWidth;
	m_fOrigHeight = fHeight;

	if ( m_bFlags & AGK_SPRITE_MANUALOFFSET )
	{
		m_fOffsetX *= diffX;
		m_fOffsetY *= diffY;
	}
	else 
	{
		m_fOffsetX = m_fWidth / 2.0f;
		m_fOffsetY = m_fHeight / 2.0f;
	}

	if ( m_bFlags & AGK_SPRITE_POSTOPLEFT )
	{
		SetPosition( oldX, oldY );
	}
	
	RecalcVisualRadius();
	
	if ( bUpdateShape ) SetShape( m_eShape );
	if ( !m_phyShape ) RecalcColRadius();
}

//****if* cSprite/SetImage
// FUNCTION
//   Will load the image into a new cImage object and assign it to the sprite, image will be deleted when replaced, or the sprite is deleted
//   This will cause the sprite's physics shape to be recalculated which can be costly for circle and polygon shapes.
// INPUTS
//   szImage -- The filename of the image to load, must be stored in the devices media folder.
// SOURCE
void cSprite::SetImage( const uString &szImage, bool bUpdateCollisionShape )
//****
{
	// delete any existing non-shared image
	if ( (m_bFlags & AGK_SPRITE_SHAREDIMAGE) == 0 )
	{
		if ( m_pImage != UNDEF && !m_pImage->IsDeleting() ) delete m_pImage;
		m_pImage = UNDEF;
	}
	
	cImage* pOldImage = m_pImage;

	// set sprite to use image named, unshared image
	m_iImageID = 0;
	m_pImage = new cImage( szImage );
	m_bFlags &= ~AGK_SPRITE_SHAREDIMAGE;
	CheckAndRemoveImage( pOldImage );

	CheckTransparency();
	
	/*
	// leefix - 131212 - LoadSprite does not reveal GetSpriteImageID(), so create image number for it!
	// reverted BAG hack as it caused a crash with virtual joysticks
	m_iImageID = 0;
	m_iImageInternalID = 0; // will use this to delete the numbered image when we DeletSprite()
	m_pImage = NULL;

	CheckAndRemoveImage( pOldImage );

	UINT iImgID = agk::m_cImageList.GetFreeID( MAX_IMAGES );
	if ( iImgID != 0 )
	{
		cImage *pInternalImage = new cImage( );
		pInternalImage->m_iID = iImgID;
		if ( pInternalImage->Load( szImage, 0 ) )
		{
			agk::m_cImageList.AddItem( pInternalImage, iImgID );
			m_iImageID = iImgID;
			m_iImageInternalID = iImgID;
			m_pImage = pInternalImage;
		}
		else return;
	}

	if ( m_pImage && (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) ) m_pImage->AddSprite( this );

	// as image is independent with access to ID, make it shared (can delete later using m_iImageInternalID)
	m_bFlags |= AGK_SPRITE_SHAREDIMAGE;
	*/

	if ( /*m_pImage->IsResized() ||*/ m_pImage->HasParent() )
	{
		if ( m_fUVBorder < 0.5f ) m_fUVBorder = 0.5f;
	}
	else
	{
		m_fUVBorder = 0;
	}

	if ( !(m_bFlags & AGK_SPRITE_CUSTOM_SHADER) ) m_pShader = AGKShader::g_pShaderTexColor;

	m_bFlags |= AGK_SPRITE_TEXCHANGED;

	if ( bUpdateCollisionShape && (m_eShape == eCircle || m_eShape == ePolygon) ) SetShape( m_eShape );
}

void cSprite::CheckAndRemoveImage( cImage* pImage )
{
	// check if image is being used by this sprite in any way, if not then tell image
	if ( (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) == 0 ) return;
	if ( !pImage ) return;
	if ( pImage == m_pImage ) return;

	bool bFound = false;
	for ( int i = 0; i < m_iFrameCount; i++ )
	{
		if ( m_pFrames[ i ].m_pFrameImage == pImage )
		{
			bFound = true;
			break;
		}
	}

	if ( !bFound )
	{
		for ( int i = 1; i < 8; i++ )
		{
			if ( m_pAdditionalImages[i] == pImage )
			{
				bFound = true;
				break;
			}
		}
	}

	if ( !bFound ) pImage->RemoveSprite( this );
}

void cSprite::CheckTransparency()
{
	// if the user has not specified a transparency mode set it based on the image
	if ( (m_bFlags & AGK_SPRITE_FORCE_TRANSPARENCY) != 0 ) return;

	// check color first
	if ( (m_iColor & 0xff) < 0xff ) 
	{
		if ( m_iTransparencyMode != 1 ) 
		{
			m_iTransparencyMode = 1;
			m_bFlags |= AGK_SPRITE_TRANSCHANGED;
		}
		return;
	}
		
	bool bFound = false;
	if ( m_pImage && m_pImage->IsTransparent() ) bFound = true;
	else
	{
		for ( int i = 0; i < m_iFrameCount; i++ )
		{
			if ( m_pFrames[ i ].m_pFrameImage && m_pFrames[ i ].m_pFrameImage->IsTransparent() )
			{
				bFound = true;
				break;
			}
		}
	}

	if ( bFound ) 
	{
		if ( m_iTransparencyMode != 1 ) 
		{
			m_iTransparencyMode = 1;
			m_bFlags |= AGK_SPRITE_TRANSCHANGED;
		}
	}
	else 
	{
		if ( m_iTransparencyMode != 0 ) 
		{
			m_iTransparencyMode = 0;
			m_bFlags |= AGK_SPRITE_TRANSCHANGED;
		}
	}
}

//****if* cSprite/SetImage
// FUNCTION
//   Will assign the image to the sprite as a shared image, image will not be deleted at any time
//   This will cause the sprite's physics shape to be recalculated which can be costly for circle and polygon shapes.
// INPUTS
//   pImage -- A pointer to a previously loaded image.
// SOURCE
void cSprite::SetImage( cImage* pImage, bool bUpdateCollisionShape )
//****
{
	// setting an image with an existing set of frames can cause strange effects, either the user should clear them or we do.
	ClearAnimationFrames();

	// delete any existing non-shared image
	if ( (m_bFlags & AGK_SPRITE_SHAREDIMAGE) == 0 )
	{
		if ( m_pImage != UNDEF && !m_pImage->IsDeleting() ) delete m_pImage;
		m_pImage = UNDEF;
	}

	if ( pImage && m_pImage )
	{
		if ( pImage->GetTextureID() != m_pImage->GetTextureID() ) m_bFlags |= AGK_SPRITE_TEXCHANGED;
	}
	else
	{
		if ( pImage != m_pImage ) m_bFlags |= AGK_SPRITE_TEXCHANGED;
	}

	cImage *pOldImage = m_pImage;

	if ( pImage && (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) ) pImage->AddSprite( this );

	m_iImageID = 0;
	if ( pImage ) m_iImageID = pImage->GetID();
	m_pImage = pImage;
	m_bFlags |= AGK_SPRITE_SHAREDIMAGE;

	CheckAndRemoveImage( pOldImage );

	CheckTransparency();

	if ( m_pImage && (/*m_pImage->IsResized() ||*/ m_pImage->HasParent()) )
	{
		if ( m_fUVBorder < 0.5f ) m_fUVBorder = 0.5f;
	}
	else
	{
		m_fUVBorder = 0;
	}

	if ( !(m_bFlags & AGK_SPRITE_CUSTOM_SHADER) ) 
	{
		if ( m_pImage ) m_pShader = AGKShader::g_pShaderTexColor;
		else m_pShader = AGKShader::g_pShaderColor;
	}

	if ( bUpdateCollisionShape && (m_eShape == eCircle || m_eShape == ePolygon) ) SetShape( m_eShape );
	
	// change sprite dimensions to image width and height
	//m_fWidth  = (float) m_pImage->GetWidth();
	//m_fHeight = (float) m_pImage->GetHeight();
}

void cSprite::SetAdditionalImage( cImage *pImage, int stage )
{
	if ( stage < 1 || stage > 7 ) return;

	cImage *pOldImage = m_pAdditionalImages[stage];
	if ( pImage && (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) ) pImage->AddSprite( this );

	m_pAdditionalImages[stage] = pImage;
	CheckAndRemoveImage( pOldImage );
}

void cSprite::SwitchImage( cImage* pImage, bool bUpdateCollisionShape )
{
	// delete any existing non-shared image
	if ( (m_bFlags & AGK_SPRITE_SHAREDIMAGE) == 0 )
	{
		if ( m_pImage != UNDEF && !m_pImage->IsDeleting() ) delete m_pImage;
		m_pImage = UNDEF;
	}

	if ( pImage && m_pImage )
	{
		if ( pImage->GetTextureID() != m_pImage->GetTextureID() ) m_bFlags |= AGK_SPRITE_TEXCHANGED;
	}
	else
	{
		if ( pImage != m_pImage ) m_bFlags |= AGK_SPRITE_TEXCHANGED;
	}

	m_iImageID = 0;
	if ( pImage ) m_iImageID = pImage->GetID();
	m_pImage = pImage;
	m_bFlags |= AGK_SPRITE_SHAREDIMAGE;

	CheckTransparency();

	if ( m_pImage && (/*m_pImage->IsResized() ||*/ m_pImage->HasParent()) )
	{
		if ( m_fUVBorder < 0.5f ) m_fUVBorder = 0.5f;
	}
	else
	{
		m_fUVBorder = 0;
	}

	if ( !(m_bFlags & AGK_SPRITE_CUSTOM_SHADER) ) 
	{
		if ( m_pImage ) m_pShader = AGKShader::g_pShaderTexColor;
		else m_pShader = AGKShader::g_pShaderColor;
	}

	if ( bUpdateCollisionShape && (m_eShape == eCircle || m_eShape == ePolygon) ) SetShape( m_eShape );
}

void cSprite::InternalSetShader( AGKShader* shader )
{
	m_pShader = shader;
	if ( !m_pShader )
	{
		if ( m_pImage ) m_pShader = AGKShader::g_pShaderTexColor;
		else m_pShader = AGKShader::g_pShaderColor;
	}
	m_bFlags &= ~AGK_SPRITE_CUSTOM_SHADER;
}

void cSprite::SetShader( AGKShader* shader )
{
	InternalSetShader( shader );
	m_bFlags |= AGK_SPRITE_CUSTOM_SHADER;
}

void cSprite::SetShaderConstantByName( const char *name, float v1, float v2, float v3, float v4 ) {
	sSpriteUniform *pVar = m_cShaderVariables.GetItem( name );

	if( !pVar ) {
		pVar = new sSpriteUniform();
		m_cShaderVariables.AddItem( pVar, name );
	}

	pVar->m_sName.SetStr( name );
	pVar->index = -1;
	pVar->v1 = v1;
	pVar->v2 = v2;
	pVar->v3 = v3;
	pVar->v4 = v4;
	m_cShaderVariables.AddItem( pVar, name );
}

void cSprite::SetShaderConstantArrayByName( const char *name, UINT index, float v1, float v2, float v3, float v4 ) {
	if( strlen( name ) > 90 ) return;
	char str[100];
	sprintf( str, "%s_%d", name, index );

	sSpriteUniform *pVar = m_cShaderVariables.GetItem( str );

	if( !pVar ) {
		pVar = new sSpriteUniform();
		m_cShaderVariables.AddItem( pVar, str );
	}

	pVar->m_sName.SetStr( name );
	pVar->index = (int)index;
	pVar->v1 = v1;
	pVar->v2 = v2;
	pVar->v3 = v3;
	pVar->v4 = v4;
}

void cSprite::SetShaderConstantDefault( const char *name ) {
	sSpriteUniform *pVar = m_cShaderVariables.RemoveItem( name );
	if( pVar ) delete pVar;
}

void cSprite::SetUserData( void* data )
{
	m_pUserData = data;
}

void* cSprite::GetUserData( )
{
	return m_pUserData;
}

void cSprite::SetUserInt( int index, int value )
{
	if ( index > 127 )
	{
		agk::Error( "Cannot store more than 128 integers in a sprite" );
		return;
	}

	if ( index >= m_iNumUserInts ) 
	{
		int newSize = m_iNumUserInts + m_iNumUserInts/2;
		if ( newSize < 4 ) newSize = 4;
		if ( newSize > 128 ) newSize = 128;
		int *newData = new int[ newSize ];
		for ( int i = 0; i < m_iNumUserInts; i++ ) newData[ i ] = m_pUserInts[ i ];
		for ( int i = m_iNumUserInts; i < newSize; i++ ) newData[ i ] = 0;
		if ( m_pUserInts ) delete [] m_pUserInts;
		m_pUserInts = newData;
		m_iNumUserInts = newSize;
	}

	m_pUserInts[ index ] = value;
}

int cSprite::GetUserInt( int index )
{
	if ( index < 0 || index >= m_iNumUserInts ) return 0;

	return m_pUserInts[ index ];
}

void cSprite::SetUserFloat( int index, float value )
{
	if ( index > 127 )
	{
		agk::Error( "Cannot store more than 128 floats in a sprite" );
		return;
	}

	if ( index >= m_iNumUserFloats ) 
	{
		int newSize = m_iNumUserFloats + m_iNumUserFloats/2;
		if ( newSize < 4 ) newSize = 4;
		if ( newSize > 128 ) newSize = 128;
		float *newData = new float[ newSize ];
		for ( int i = 0; i < m_iNumUserFloats; i++ ) newData[ i ] = m_pUserFloats[ i ];
		for ( int i = m_iNumUserFloats; i < newSize; i++ ) newData[ i ] = 0;
		if ( m_pUserFloats ) delete [] m_pUserFloats;
		m_pUserFloats = newData;
		m_iNumUserFloats = newSize;
	}

	m_pUserFloats[ index ] = value;
}

float cSprite::GetUserFloat( int index )
{
	if ( index < 0 || index >= m_iNumUserFloats ) return 0;

	return m_pUserFloats[ index ];
}

void cSprite::SetUserString( int index, const uString& value )
{
	if ( index > 127 )
	{
		agk::Error( "Cannot store more than 128 strings in a sprite" );
		return;
	}

	if ( index >= m_iNumUserStrings ) 
	{
		int newSize = m_iNumUserStrings + m_iNumUserStrings/2;
		if ( newSize < 4 ) newSize = 4;
		if ( newSize > 128 ) newSize = 128;
		uString *newData = new uString[ newSize ];
		for ( int i = 0; i < m_iNumUserStrings; i++ ) newData[ i ].SetStr( m_pUserStrings[ i ] );
		if ( m_pUserStrings ) delete [] m_pUserStrings;
		m_pUserStrings = newData;
		m_iNumUserStrings = newSize;
	}

	m_pUserStrings[ index ] = value;
}

const uString& cSprite::GetUserString( int index )
{
	static const uString g_EmptyString;
	if ( index < 0 || index >= m_iNumUserStrings ) return g_EmptyString;

	return m_pUserStrings[ index ];
}

void cSprite::SetFontImage( AGKFontImage *pFontImage, float scale )
{
	if ( m_pFontImage == pFontImage ) return;

	if ( m_pFontImage ) m_pFontImage->Release();
	m_pFontImage = pFontImage;
	if ( m_pFontImage ) 
	{
		m_pFontImage->AddRef();

		SetImage( m_pFontImage->m_pImage );

		if ( m_pFontImage->m_pImage )
		{
			float VwDw = agk::DeviceToDisplayRatioX();
			float VhDh = agk::DeviceToDisplayRatioY();
			
			float width = m_pFontImage->m_pImage->GetWidth() * VwDw * scale;
			float height = m_pFontImage->m_pImage->GetHeight() * VhDh * scale;
			SetSize( width, height );
		}
		else
		{
			SetSize(0,0);
		}
	}
	else
	{
		SetImage( 0 );
		SetSize( 0,0 );
	}
}

//****if* cSprite/SetAnimation
// FUNCTION
//   Sets the sprite to extract frame images from it's set image, using the frameWidth and frameHeight given.
//   Frames within the image must be tiled in rows from left to right. All frames must be the same size.
//   This will cause the sprite's physics shape to be recalculated which can be costly for circle and polygon shapes.
//   If called with a frame count of 0 it will delete the animation and use the whole image.
// INPUTS
//   iFrameWidth -- the width in pixels of the individual frames.
//   iFrameHeight -- the height in pixels of the individual frames.
//   iFrameCount -- the total number of frames within the image that the sprite should extract.
// SOURCE
void cSprite::SetAnimation ( int iFrameWidth, int iFrameHeight, int iFrameCount )
//****
{
	// delete old frames
	if ( m_pFrames != UNDEF ) 
	{
		if ( (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) )
		{
			for ( int i = 0; i < m_iFrameCount; i++ )
			{
				if ( m_pFrames[i].m_pFrameImage && m_pFrames[i].m_pFrameImage != m_pImage ) m_pFrames[i].m_pFrameImage->RemoveSprite( this );
			}
		}

		delete [] m_pFrames;
	}
	m_pFrames = UNDEF;
	m_iFrameCount = 0;
	m_iFrameArraySize = 0;

	if ( iFrameCount == 0 ) return;
	if ( m_pImage == UNDEF ) return;
	
	// total size is the size of any atlas texture being used, used for calculating UV coords
	int imageTotalWidth = m_pImage->GetTotalWidth();
	int imageTotalHeight = m_pImage->GetTotalHeight();

	int x = (int) (m_pImage->GetU1() * imageTotalWidth);
	int y = (int) (m_pImage->GetV1() * imageTotalHeight);

	int endX = (int) (m_pImage->GetU2() * imageTotalWidth);
	int endY = (int) (m_pImage->GetV2() * imageTotalHeight);

	/*
	if ( m_pImage->HasParent() )
	{
		x = (int) ((m_pImage->GetU1() * imageTotalWidth) - (0.5f / imageTotalWidth));
		y = (int) ((m_pImage->GetV1() * imageTotalHeight) - (0.5f / imageTotalHeight));

		//endX = (int) ((m_pImage->GetU2() * imageTotalWidth) + (0.5f / imageTotalWidth));
		//endY = (int) ((m_pImage->GetV2() * imageTotalHeight) + (0.5f / imageTotalHeight));
	}
	*/

	if ( iFrameWidth > m_pImage->GetWidth() || iFrameHeight > m_pImage->GetHeight() ) 
	{
#ifdef _AGK_ERROR_CHECK
		uString errStr( "Image is not big enough to have that many animation frames ", 100 ); errStr.Append( m_pImage->GetPath() );
		agk::Error( errStr );
#endif
		return;
	}

	m_iFrameCount = iFrameCount;
	m_iFrameArraySize = iFrameCount;

	// load new frames
	m_pFrames = new cSpriteFrame[ m_iFrameArraySize ];
	
	int count = 0;
	for ( int i = 0; i < iFrameCount; i++ )
	{
		count++;
		/*
		m_pFrames[ i ].m_fU1 = (x / (float) imageTotalWidth) + (0.5f / imageTotalWidth);
		m_pFrames[ i ].m_fV1 = (y / (float) imageTotalHeight) + (0.5f / imageTotalHeight);

		m_pFrames[ i ].m_fU2 = ((x+iFrameWidth) / (float) imageTotalWidth) - (0.5f / imageTotalWidth);
		m_pFrames[ i ].m_fV2 = ((y+iFrameHeight) / (float) imageTotalHeight) - (0.5f / imageTotalHeight);
		*/

		m_pFrames[ i ].m_fU1 = (x / (float) imageTotalWidth);
		m_pFrames[ i ].m_fV1 = (y / (float) imageTotalHeight);

		m_pFrames[ i ].m_fU2 = ((x+iFrameWidth) / (float) imageTotalWidth);
		m_pFrames[ i ].m_fV2 = ((y+iFrameHeight) / (float) imageTotalHeight);

		m_pFrames[ i ].m_iWidth = iFrameWidth;
		m_pFrames[ i ].m_iHeight = iFrameHeight;

		m_pFrames[ i ].m_pFrameImage = m_pImage;

		x += iFrameWidth;

		if ( x+iFrameWidth > endX )
		{
			x = (int) (m_pImage->GetU1() * imageTotalWidth);
			y = y+iFrameHeight;

			if ( y+iFrameHeight > endY ) break;
		}
	}

	if ( iFrameCount != count )
	{
		uString errStr( "Image is not big enough to have that many animation frames ", 100 ); errStr.Append( m_pImage->GetPath() );
		agk::Error( errStr );
	}

	// may not have loaded all frames
	m_iFrameCount = count;
	m_iFrameWidth = iFrameWidth;
	m_iFrameHeight = iFrameHeight;

	// sprite may need resizing
	float fNewWidth = m_fOrigWidth;
	float fNewHeight = m_fOrigHeight;

	if ( (m_bFlags & AGK_SPRITE_WIDTHCALC) != 0 ) fNewWidth = -1;
	if ( (m_bFlags & AGK_SPRITE_HEIGHTCALC) != 0 ) fNewHeight = -1;

	float oldScaleX = m_fWidth / m_fOrigWidth;
	float oldScaleY = m_fHeight / m_fOrigHeight;
	SetSize( fNewWidth, fNewHeight );
	if ( oldScaleX != 1 || oldScaleY != 1 ) 
	{
		if ( m_bFlags & AGK_SPRITE_POSTOPLEFT ) SetScale( oldScaleX, oldScaleY );
		else SetScaleByOffset( oldScaleX, oldScaleY );
	}
}

void cSprite::AppendAnimation ( cImage *pImage, int iFrameWidth, int iFrameHeight, int iFrameCount )
//****
{
	if ( iFrameCount == 0 ) return;
	if ( pImage == UNDEF ) return;
		
	// total size is the size of any atlas texture being used, used for calculating UV coords
	int imageTotalWidth = pImage->GetTotalWidth();
	int imageTotalHeight = pImage->GetTotalHeight();

	int x = (int) (pImage->GetU1() * imageTotalWidth);
	int y = (int) (pImage->GetV1() * imageTotalHeight);

	int endX = (int) (pImage->GetU2() * imageTotalWidth);
	int endY = (int) (pImage->GetV2() * imageTotalHeight);

	/*
	if ( iFrameWidth > pImage->GetWidth() || iFrameHeight > pImage->GetHeight() ) 
	{
#ifdef _AGK_ERROR_CHECK
		uString errStr( "Image does not contain enough animation frames ", 100 ); errStr.Append( pImage->GetPath() );
		agk::Error( errStr );
#endif
		return;
	}
	*/

	if ( (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) ) pImage->AddSprite( this );

	int newFrameCount = m_iFrameCount + iFrameCount;

	if ( newFrameCount > m_iFrameArraySize )
	{
		cSpriteFrame *pNewFrames = new cSpriteFrame[ newFrameCount ];

		if ( m_pFrames != UNDEF )
		{
			for( int i = 0; i < m_iFrameCount; i++ )
			{
				memcpy( pNewFrames+i, m_pFrames+i, sizeof(cSpriteFrame) );
			}

			delete [] m_pFrames;
		}

		// load new frames
		m_pFrames = pNewFrames;
		m_iFrameArraySize = newFrameCount;
	}
	
	int count = 0;
	for ( int i = m_iFrameCount; i < newFrameCount; i++ )
	{
		count++;
		/*
		m_pFrames[ i ].m_fU1 = (x / (float) imageTotalWidth) + (0.5f / imageTotalWidth);
		m_pFrames[ i ].m_fV1 = (y / (float) imageTotalHeight) + (0.5f / imageTotalHeight);

		m_pFrames[ i ].m_fU2 = ((x+iFrameWidth) / (float) imageTotalWidth) - (0.5f / imageTotalWidth);
		m_pFrames[ i ].m_fV2 = ((y+iFrameHeight) / (float) imageTotalHeight) - (0.5f / imageTotalHeight);
		*/

		m_pFrames[ i ].m_fU1 = (x / (float) imageTotalWidth);
		m_pFrames[ i ].m_fV1 = (y / (float) imageTotalHeight);

		m_pFrames[ i ].m_fU2 = ((x+iFrameWidth) / (float) imageTotalWidth);
		m_pFrames[ i ].m_fV2 = ((y+iFrameHeight) / (float) imageTotalHeight);

		m_pFrames[ i ].m_iWidth = iFrameWidth;
		m_pFrames[ i ].m_iHeight = iFrameHeight;

		m_pFrames[ i ].m_pFrameImage = pImage;

		x += iFrameWidth;

		if ( x+iFrameWidth > endX )
		{
			x = (int) (pImage->GetU1() * imageTotalWidth);
			y = y+iFrameHeight;

			if ( y+iFrameHeight > endY ) break;
		}
	}

	// may not have loaded all frames
	m_iFrameCount = m_iFrameCount + count;
	//m_iFrameWidth = iFrameWidth;
	//m_iFrameHeight = iFrameHeight;

	CheckTransparency();
}

void cSprite::ExpandAnimationArray ( int newTotalFrames )
{
	if ( newTotalFrames > m_iFrameArraySize )
	{
		cSpriteFrame *pNewFrames = new cSpriteFrame[ newTotalFrames ];

		// copy old frames
		if ( m_iFrameCount > 0 )
		{
			for ( int i = 0; i < m_iFrameCount; i++ )
			{
				memcpy( pNewFrames+i, m_pFrames+i, sizeof(cSpriteFrame) );
			}
		}

		delete [] m_pFrames;
		m_pFrames = pNewFrames;
		m_iFrameArraySize = newTotalFrames;
	}
}

void cSprite::AddAnimationFrame( cImage *pImage )
//****
{
	if ( !pImage ) return;

	if ( m_iFrameCount+1 > m_iFrameArraySize )
	{
		cSpriteFrame *pNewFrames = new cSpriteFrame[ m_iFrameCount+1 ];

		// copy old frames
		if ( m_iFrameCount > 0 )
		{
			for ( int i = 0; i < m_iFrameCount; i++ )
			{
				memcpy( pNewFrames+i, m_pFrames+i, sizeof(cSpriteFrame) );
			}
		}

		delete [] m_pFrames;
		m_pFrames = pNewFrames;
		m_iFrameArraySize = m_iFrameCount+1;
	}

	if ( (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) ) pImage->AddSprite( this );

	m_pFrames[ m_iFrameCount ].m_fU1 = pImage->GetU1();
	m_pFrames[ m_iFrameCount ].m_fV1 = pImage->GetV1();
	m_pFrames[ m_iFrameCount ].m_fU2 = pImage->GetU2();
	m_pFrames[ m_iFrameCount ].m_fV2 = pImage->GetV2();
	m_pFrames[ m_iFrameCount ].m_iWidth = pImage->GetWidth();
	m_pFrames[ m_iFrameCount ].m_iHeight = pImage->GetHeight();
	m_pFrames[ m_iFrameCount ].m_pFrameImage = pImage;

	if ( !m_pImage ) SwitchImage( pImage );

    m_iFrameCount++;
	
	if ( m_iFrameCount == 1 && (m_iFrameWidth != pImage->GetWidth() || m_iFrameHeight != pImage->GetHeight()) )
	{
		m_iFrameWidth = pImage->GetWidth();
		m_iFrameHeight = pImage->GetHeight();

		// sprite may need resizing
		float fNewWidth = m_fWidth;
		float fNewHeight = m_fHeight;

		if ( (m_bFlags & AGK_SPRITE_WIDTHCALC) != 0 ) fNewWidth = -1;
		if ( (m_bFlags & AGK_SPRITE_HEIGHTCALC) != 0 ) fNewHeight = -1;

		float oldScaleX = m_fWidth / m_fOrigWidth;
		float oldScaleY = m_fHeight / m_fOrigHeight;
		SetSize( fNewWidth, fNewHeight );
		if ( oldScaleX != 1 || oldScaleY != 1 ) SetScaleByOffset( oldScaleX, oldScaleY );
	}

	CheckTransparency();
}

//****if* cSprite/SetFrame
// FUNCTION
//   Sets the sprite to the specified animation frame, frames start at frame 1 up to and including GetFrameCount()
// INPUTS
//   iFrame -- the width in pixels of the individual frames.
// SOURCE
void cSprite::SetFrame( int iFrame )
//****
{
	if ( m_iFrameCount == 0 )
	{
#ifdef _AGK_ERROR_CHECK
		uString err;
		err.Format( "Tried to set an animation frame on a sprite (%d) that has no animation", GetID() );
		agk::Error( err );
#endif
		return;
	}

	if ( iFrame < 1 ) 
	{
		//iFrame = 1;
#ifdef _AGK_ERROR_CHECK
		uString err;
		err.Format( "Invalid frame number %d for sprite (%d), should be in the range 1 to %d.", iFrame, GetID(), m_iFrameCount );
		agk::Error( err );
#endif
		iFrame = 1;
	}

	if ( iFrame > m_iFrameCount ) 
	{
		//iFrame = m_iFrameCount;
#ifdef _AGK_ERROR_CHECK
		uString err;
		err.Format( "Invalid frame number %d for sprite (%d), should be in the range 1 to %d.", iFrame, GetID(), m_iFrameCount );
		agk::Error( err );
#endif
		iFrame = m_iFrameCount;
	}

	// shift from base 1 to base 0 indexing, internally frames start at 0, externally they start at 1
	m_iCurrentFrame = iFrame - 1;

	if ( m_pFrames[ m_iCurrentFrame ].m_pFrameImage != m_pImage ) 
	{
		SwitchImage( m_pFrames[ m_iCurrentFrame ].m_pFrameImage, false );
		
		if ( m_iFrameWidth != m_pFrames[ m_iCurrentFrame ].m_iWidth || m_iFrameHeight != m_pFrames[ m_iCurrentFrame ].m_iHeight )
		{
			m_iFrameWidth = m_pFrames[ m_iCurrentFrame ].m_iWidth;
			m_iFrameHeight = m_pFrames[ m_iCurrentFrame ].m_iHeight;

			// sprite may need resizing
			float fNewWidth = m_fWidth;
			float fNewHeight = m_fHeight;

			if ( (m_bFlags & AGK_SPRITE_WIDTHCALC) != 0 ) fNewWidth = -1;
			if ( (m_bFlags & AGK_SPRITE_HEIGHTCALC) != 0 ) fNewHeight = -1;

			float oldScaleX = m_fWidth / m_fOrigWidth;
			float oldScaleY = m_fHeight / m_fOrigHeight;
			SetSize( fNewWidth, fNewHeight, false );
			if ( oldScaleX != 1 || oldScaleY != 1 ) SetScaleByOffset( oldScaleX, oldScaleY );
		}
	}
}

void cSprite::ClearAnimationFrames()
//****
{
	if ( m_pFrames ) 
	{
		if ( (m_bFlags & AGK_SPRITE_MANAGE_IMAGES) )
		{
			for ( int i = 0; i < m_iFrameCount; i++ )
			{
				if ( m_pFrames[i].m_pFrameImage && m_pFrames[i].m_pFrameImage != m_pImage ) m_pFrames[i].m_pFrameImage->RemoveSprite( this );
			}
		}

		delete [] m_pFrames;
	}
	m_pFrames = 0;
	m_iCurrentFrame = 0;
	m_iFrameArraySize = 0;
	m_iFrameCount = 0;
	m_bFlags &= ~AGK_SPRITE_PLAYING;
}

//****if* cSprite/SetSpeed
// FUNCTION
//   Sets the sprite animation to a specified speed, can be used whilst the animation is running. Can be set to 0 to temporarily pause the animtion.
// INPUTS
//   fFps -- the rate at which the sprite should update, sprite frames will be skipped if the sprite frame rate is too high compared to the game frame rate.
// SOURCE
void cSprite::SetSpeed ( float fFps )
//****
{
	// minimum speed of 1 frame every 1000 seconds
	if ( fFps <= 0 ) fFps = 0.001f;

	m_fFrameChangeTime = 1.0f / fFps;
	
	// If the change in speed would cause a lot of frame skipping only move 1 frame before continuing
	if ( m_fFrameTimer > m_fFrameChangeTime ) m_fFrameTimer = m_fFrameChangeTime;
}

//****if* cSprite/Play
// FUNCTION
//   Sets the sprite to change frame a certain number of times per second and starts the animation
// INPUTS
//   fFps -- the rate at which the sprite should update, sprite frames will be skipped if the sprite frame rate is too high compared to the game frame rate.
//   bLoop -- should the animation loop when it reaches the last frame
//   iStart -- the frame to start at, and to return to when it reaches the end, use -1 to start at the first frame
//   iEnd -- the frame to stop at, use -1 to end at the last frame
// SOURCE
void cSprite::Play ( float fFps, bool bLoop, int iStart, int iEnd )
//****
{
	if ( m_iFrameCount == 0 || m_pFrames == UNDEF ) return;

	// minimum speed of 1 frame every 1000 seconds
	if ( fFps <= 0 ) fFps = 0.001f;
	
	m_fFrameChangeTime = 1.0f / fFps;
	if ( bLoop ) m_bFlags |= AGK_SPRITE_LOOP;
	else m_bFlags &= ~AGK_SPRITE_LOOP;
	
	if ( iStart < 1 ) m_iFrameStart = 0;
	else if ( iStart > m_iFrameCount ) m_iFrameStart = m_iFrameCount - 1;
	else m_iFrameStart = iStart - 1;

	if ( iEnd < 1 ) m_iFrameEnd = m_iFrameCount - 1;
	else if ( iEnd > m_iFrameCount ) m_iFrameEnd = m_iFrameCount - 1;
	else m_iFrameEnd = iEnd - 1;

	m_fFrameTimer = 0.0f;
	m_iCurrentFrame = m_iFrameStart;
	m_bFlags |= AGK_SPRITE_PLAYING;

	if ( m_pFrames[ m_iCurrentFrame ].m_pFrameImage != m_pImage ) SwitchImage( m_pFrames[ m_iCurrentFrame ].m_pFrameImage, false );
}

//****if* cSprite/Stop
// FUNCTION
//   Stop the sprite from animating, retains the values used to set it up.
// SOURCE
void cSprite::Stop()
//****
{
	m_bFlags &= ~AGK_SPRITE_PLAYING;
}

//****if* cSprite/Resume
// FUNCTION
//   Resumes the sprite animation using the values last used to set it up, resumes from the current frame 
//   rather than starting again at the first frame.
// SOURCE
void cSprite::Resume()
//****
{
	if ( m_fFrameChangeTime <= 0 )
	{
		m_fFrameChangeTime = 1.0f / 60.0f;
		agk::Error( "ResumeSprite called without first calling PlaySprite" );
	}

	m_bFlags |= AGK_SPRITE_PLAYING;
}

//*if* cSprite/SetBlendFunc
// FUNCTION
//   Set the blend function to be used when drawing this sprite.
// INPUTS
//   src -- The source alpha value, use GL_SRC_ALPHA style values.
//   dest -- The destination alpha value, use GL_SRC_ALPHA style values.
// SOURCE
//void cSprite::SetBlendFunction ( GLenum src, GLenum dest )
//
//{
	//m_eSrcAlpha = src;
	//m_eDestAlpha = dest;
//}

//****if* cSprite/SetTransparency
// FUNCTION
//   Set the sprite transparency to a particular setting.
// INPUTS
//   mode -- The transparency mode for this sprite, 0=off, 1=alpha channel transparency, 2=additive transparency
// SOURCE
void cSprite::SetTransparency ( int mode )
//****
{
	m_bFlags |= AGK_SPRITE_FORCE_TRANSPARENCY;

	if ( m_iTransparencyMode == mode ) return;
	m_iTransparencyMode = mode;
	m_bFlags |= AGK_SPRITE_TRANSCHANGED;
}

//****if* cSprite/SetOffset
// FUNCTION
//   The offset point is the point that the sprite will rotate around, with (0,0) being the top left corner and (width,height)
//   being the bottom right corner. The offset can also be used to position the sprite using SetPositionByOffset(), whilst
//   SetPosition() will always position the sprite using its top left corner. By default the offset position is set to the
//   center of the sprite. If the sprite is scaled remember to take the scale into account, a sprite created as 10 by 10 units 
//   wide scaled by 0.5 will have a bottom right corner at 5,5 so to position the offset in the center would mean placing it
//   at 2.5,2.5, when the sprite is scale back up the offset point will scale with it to maintain its relative position (in this
//   example the offset point would still be in teh center)
//
//   Any physics shapes assign to the sprite are centered on the offset point, so moving the offset will move the shapes around
//   the sprite.
// INPUTS
//   x -- The distance to shift the sprite in the X direction.
//   y -- The distance to shift the sprite in the Y direction.
// SOURCE
void cSprite::SetOffset ( float x, float y )
//****
{
	float oldX;
	float oldY;
	if ( m_bFlags & AGK_SPRITE_POSTOPLEFT )
	{
		oldX = GetX();
		oldY = GetY();
	}

	m_fOffsetX = x;
	m_fOffsetY = y;
	m_bFlags |= AGK_SPRITE_MANUALOFFSET;
	
	RecalcVisualRadius();
	if ( !m_phyShape ) RecalcColRadius();

	if ( m_bFlags & AGK_SPRITE_POSTOPLEFT )
	{
		SetPosition( oldX, oldY );
	}

	//SetShape( m_eShape );
}

//****if* cSprite/SetUVBorder
// FUNCTION
//   Controls how the sprite handles UVs that might extend beyond the bounds of the image during sampling, for 
//   example when the edge of the sprite is halfway across a pixel. By default (border=0.5) the sprite compensates
//   for this by offsetting UV coords by 0.5 pixels inwards, however this means that the outer most pixels of 
//   the sprite's image may not show. border=0 removes this offset creating a pixel perfect reproduction of the
//   image, but this may cause texture seams when trying to line up sprites of the same image that don't fall
//   exactly on a whole pixel. Setting the border to 0 may also cause animated sprites, or sprites using a texture 
//   atlas, to 'steal' pixels from neighbouring images. You may also increase this effect by setting the border 
//   greater than 0.5 pixels.
// INPUTS
//   border -- The number of pixels to offset the UV coordinates inwards. Must be greater than or equal to 0.
// SOURCE
void cSprite::SetUVBorder( float border )
//****
{
	if ( border < 0 ) border = 0;
	m_fUVBorder = border;
}

//****if* cSprite/SetUVOffset
// FUNCTION
//   Offsets the sprites UV coordinates by the given amount. For example, offsetting by 0.5 in the U direction will 
//   make the sprite begin sampling the texture halfway across the top of the texture instead of the top left corner
//   as normal. UV values outside the range of 0,0 (top left) and 1,1 (bottom right) can either wrap around or clamp
//   the texture, which is decided by the image assigned to the sprite using SetImageWrapU() and SetImageWrapV().
//   Clamping or wraping cannot be set on a per-sprite basis.
//
//   By default a sprite is set to use the UV coordinates 0,0 to 1,1 using the full image available to it. However 
//   there are several cases where this is changed by the AGK to hide certain limitations. If the texture assigned 
//   to the sprite is not a power of 2 width or height the image is increased in size until it is a power of 2 size 
//   and the UV coordinates for the sprite reduced so that the sprite only uses the portion of the texture 
//   containing the original image. This is because most mobile platforms do not support textures that are not a 
//   power of 2 width or height. Therefore offsetting the sprites's UV coords in this case will shift the sprite's 
//   usage of the texture into the undefined portion which is being used as padding.
//   Additionally if the sprtie is using an image that belongs to an atlas texture the sprite's UV coordinates will
//   be set so that it only uses the portion of the texture containing its assigned image. Offsetting the UV
//   coordinates in this case will shift the sprite's usage of the texture into other images that are part of the 
//   atlas texture. The same can be said of sprites using an animation contained within a single texture using 
//   SetSpriteAnimation().
//   Due to these possibilities it is recommended that UV coords only be modified on sprites that are using whole 
//   images (not atlas textures) and which are a power of 2 size in both width and height. With these constraints 
//   it is possible to use UV values outside 0-1 to clamp or repeat the texture successfully.
// INPUTS
//   u -- The amount to offset the UV coordinates in the U direction.
//   v -- The amount to offset the UV coordinates in the V direction.
// SOURCE
void cSprite::SetUVOffset( float u, float v )
//****
{
	m_fUOffset = u;
	m_fVOffset = v;
}

//****if* cSprite/SetUVScale
// FUNCTION
//   Scales the sprites UV coordinates by the given amount. A sprite with UV scaled by 2 will make its texture look 
//   twice as big as normal. The UV scale does not affect the UV offset chosen using SetSpriteUVOffset(), so that an
//   offset of 0.5 in the U direction will always begin sampling halfway across the texture, the scale defines how
//   far the sprite continues sampling. So with an offset of 0.5, a scale of 2 will make the sprite sample from 0.5 
//   to 1 instead of 0.5 to 1.5.
//
//   By default a sprite is set to use the UV coordinates 0,0 to 1,1 using the full image available to it. However 
//   there are several cases where this is changed by the AGK to hide certain limitations. If the texture assigned 
//   to the sprite is not a power of 2 width or height the image is increased in size until it is a power of 2 size 
//   and the UV coordinates for the sprite reduced so that the sprite only uses the portion of the texture 
//   containing the original image. This is because most mobile platforms do not support textures that are not a 
//   power of 2 width or height. Therefore offsetting the sprites's UV coords in this case will shift the sprite's 
//   usage of the texture into the undefined portion which is being used as padding.
//   Additionally if the sprtie is using an image that belongs to an atlas texture the sprite's UV coordinates will
//   be set so that it only uses the portion of the texture containing its assigned image. Offsetting the UV
//   coordinates in this case will shift the sprite's usage of the texture into other images that are part of the 
//   atlas texture. The same can be said of sprites using an animation contained within a single texture using 
//   SetSpriteAnimation().
//   Due to these possibilities it is recommended that UV coords only be modified on sprites that are using whole 
//   images (not atlas textures) and which are a power of 2 size in both width and height. With these constraints 
//   it is possible to use UV values outside 0-1 to clamp or repeat the texture successfully.
// INPUTS
//   scaleU -- The amount to scale in the U direction.
//   scaleV -- The amount to scale in the V direction.
// SOURCE
void cSprite::SetUVScale( float scaleU, float scaleV )
//****
{
	if ( scaleU < 0.0001f ) scaleU = 0.0001f;
	if ( scaleV < 0.0001f ) scaleV = 0.0001f;

	m_fUScale = scaleU;
	m_fVScale = scaleV;
}

void cSprite::FixToScreen( int mode )
//****
{
	if ( mode != 0 ) m_bFlags &= ~AGK_SPRITE_SCROLL;
	else m_bFlags |= AGK_SPRITE_SCROLL;
}

void cSprite::SetScissor( float x, float y, float x2, float y2 )
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

void cSprite::GetClipValues( int &x, int &y, int &width, int &height )
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

	if ( (m_bFlags & AGK_SPRITE_SCROLL) > 0 )
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

int cSprite::GetGroup()
{
	return m_iGroup;
}

int cSprite::HasAdditionalImages()
{
	for( int i = 1; i < 8; i++ )
	{
		if ( m_pAdditionalImages[i] ) return 1;
	}

	return 0;
}

bool cSprite::GetFlippedHorizontally()
{
	return 0 != (m_bFlags & AGK_SPRITE_FLIPH);
}

bool cSprite::GetFlippedVertically()
{
	return 0 != (m_bFlags & AGK_SPRITE_FLIPV);
}

void cSprite::SetUV( float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4 )
{
	m_bUVOverride = true;
	m_fU1 = u1;
	m_fV1 = v1;

	m_fU2 = u2;
	m_fV2 = v2;

	m_fU3 = u3;
	m_fV3 = v3;

	m_fU4 = u4;
	m_fV4 = v4;
}

void cSprite::ResetUV()
{
	m_bUVOverride = false;
}

void cSprite::CheckImages()
//****
{
	// can only check ID'ed images
	if ( m_iImageID == 0 ) return;

	// check if the image has been deleted or changed
	cImage *pRealImage = agk::GetImagePtr( m_iImageID );
	if ( pRealImage == 0 ) SwitchImage( UNDEF, false );
	if ( pRealImage != m_pImage ) SwitchImage( pRealImage, false );
}

//****if* cSprite/Draw
// FUNCTION
//   Draws the sprite to the screen.
// SOURCE
void cSprite::Draw()
//****
{
	// only if sprite visible
	if ( (m_bFlags & AGK_SPRITE_VISIBLE) == 0 ) return;
	if ( !GetInScreen() ) return;

	int pixelsW = agk::Round( m_fWidth * (agk::GetDeviceWidth() / (float) agk::GetVirtualWidth()) );
	int pixelsH = agk::Round( m_fHeight * (agk::GetDeviceHeight() / (float) agk::GetVirtualHeight()) );
	g_iPixelsDrawn += (pixelsW * pixelsH);

	// check assigned images still exist
	//CheckImages();

	float *pUV = 0;
	float *pVertices = 0;
	unsigned char *pColors = 0;

	if ( m_pImage != UNDEF )
	{
        cImage::BindTexture( m_pImage->GetTextureID() );
        pUV = new float[ 8 ];
        
		if ( m_bUVOverride )
		{
            pUV[ 0 ] = m_fU1;		pUV[ 1 ] = m_fV1;
			pUV[ 2 ] = m_fU2;		pUV[ 3 ] = m_fV2;
			pUV[ 4 ] = m_fU3;		pUV[ 5 ] = m_fV3;
			pUV[ 6 ] = m_fU4;		pUV[ 7 ] = m_fV4;
		}
		else
		{
			float fU1 = m_pImage->GetU1();
			float fV1 = m_pImage->GetV1();
			float fU2 = m_pImage->GetU2();
			float fV2 = m_pImage->GetV2();

			// modify UVs if animated
			if ( m_iFrameCount > 0 )
			{
				fU1 = m_pFrames[ m_iCurrentFrame ].m_fU1;
				fV1 = m_pFrames[ m_iCurrentFrame ].m_fV1;
				fU2 = m_pFrames[ m_iCurrentFrame ].m_fU2;
				fV2 = m_pFrames[ m_iCurrentFrame ].m_fV2;
			}

			float diff = fU2 - fU1;
			diff /= m_fUScale;
			fU2 = fU1 + diff + m_fUOffset;
			fU1 += m_fUOffset;

			diff = fV2 - fV1;
			diff /= m_fVScale;
			fV2 = fV1 + diff + m_fVOffset;
			fV1 += m_fVOffset;

			if ( m_fUVBorder > 0 )
			{
				fU1 += m_fUVBorder / m_pImage->GetTotalWidth();
				fV1 += m_fUVBorder / m_pImage->GetTotalHeight();
				fU2 -= m_fUVBorder / m_pImage->GetTotalWidth();
				fV2 -= m_fUVBorder / m_pImage->GetTotalHeight();
			}

			if ( (m_bFlags & AGK_SPRITE_FLIPH) > 0 )
			{
				float temp = fU1;
				fU1 = fU2;
				fU2 = temp;
			}

			if ( (m_bFlags & AGK_SPRITE_FLIPV) > 0 )
			{
				float temp = fV1;
				fV1 = fV2;
				fV2 = temp;
			}
			
			pUV[ 0 ] = fU1;		pUV[ 1 ] = fV1;
			pUV[ 2 ] = fU1;		pUV[ 3 ] = fV2;
			pUV[ 4 ] = fU2;		pUV[ 5 ] = fV1;
			pUV[ 6 ] = fU2;		pUV[ 7 ] = fV2;
		}
	}
	else
	{
		cImage::BindTexture( 0 );
	}

	float boneScaleX = 1;
	float boneScaleY = 1;
	float boneScaleXInv = 1;
	float boneScaleYInv = 1;
	if ( m_pBone && (m_pBone->m_bFlags & AGK_BONE_PRE_SCALE) != 0 )
	{
		boneScaleX = m_pBone->worldSX;
		boneScaleY = m_pBone->worldSY;
		boneScaleXInv = 1.0f / boneScaleX;
		boneScaleYInv = 1.0f / boneScaleY;
	}

	pVertices = new float[ 12 ];
	float stretch = agk::m_fStretchValue;
	float fSinA = agk::SinRad(m_fAngle);
	float fCosA = agk::CosRad(m_fAngle);
	float fSinA1 = fSinA/stretch;
	float fSinA2 = fSinA*stretch;

	float fTempX = (-m_fOffsetX)*boneScaleX;
	float fTempY = (-m_fOffsetY)*boneScaleY;
	float x1 = m_fX + (fTempX*fCosA - fTempY*fSinA1)*boneScaleXInv;
	float y1 = m_fY + (fTempY*fCosA + fTempX*fSinA2)*boneScaleYInv;

	fTempX = (-m_fOffsetX)*boneScaleX;
	fTempY = (-m_fOffsetY + m_fHeight)*boneScaleY;
	float x2 = m_fX + (fTempX*fCosA - fTempY*fSinA1)*boneScaleXInv;
	float y2 = m_fY + (fTempY*fCosA + fTempX*fSinA2)*boneScaleYInv;

	fTempX = (-m_fOffsetX + m_fWidth)*boneScaleX;
	fTempY = (-m_fOffsetY)*boneScaleY;
	float x3 = m_fX + (fTempX*fCosA - fTempY*fSinA1)*boneScaleXInv;
	float y3 = m_fY + (fTempY*fCosA + fTempX*fSinA2)*boneScaleYInv;

	fTempX = (-m_fOffsetX + m_fWidth)*boneScaleX;
	fTempY = (-m_fOffsetY + m_fHeight)*boneScaleY;
	float x4 = m_fX + (fTempX*fCosA - fTempY*fSinA1)*boneScaleXInv;
	float y4 = m_fY + (fTempY*fCosA + fTempX*fSinA2)*boneScaleYInv;

	// check for bone transforms
	if ( m_pBone )
	{
		float x1t = m_pBone->m00 * x1 + m_pBone->m01 * y1 + m_pBone->worldX;
		float y1t = m_pBone->m10 * x1 + m_pBone->m11 * y1 + m_pBone->worldY;

		float x2t = m_pBone->m00 * x2 + m_pBone->m01 * y2 + m_pBone->worldX;
		float y2t = m_pBone->m10 * x2 + m_pBone->m11 * y2 + m_pBone->worldY;

		float x3t = m_pBone->m00 * x3 + m_pBone->m01 * y3 + m_pBone->worldX;
		float y3t = m_pBone->m10 * x3 + m_pBone->m11 * y3 + m_pBone->worldY;

		float x4t = m_pBone->m00 * x4 + m_pBone->m01 * y4 + m_pBone->worldX;
		float y4t = m_pBone->m10 * x4 + m_pBone->m11 * y4 + m_pBone->worldY;

		x1 = x1t; y1 = y1t;
		x2 = x2t; y2 = y2t;
		x3 = x3t; y3 = y3t;
		x4 = x4t; y4 = y4t;
	}

	if ( (m_bFlags & AGK_SPRITE_SCROLL) > 0 )
	{
		x1 = agk::WorldToScreenX( x1 );
		y1 = agk::WorldToScreenY( y1 );

		x2 = agk::WorldToScreenX( x2 );
		y2 = agk::WorldToScreenY( y2 );

		x3 = agk::WorldToScreenX( x3 );
		y3 = agk::WorldToScreenY( y3 );

		x4 = agk::WorldToScreenX( x4 );
		y4 = agk::WorldToScreenY( y4 );
	}
	
	// correct for floating point pixel positions
	//if ( agk::Abs(m_fAngle) < 0.01f || agk::Abs(m_fAngle-PI/2) < 0.01f 
	//	  || agk::Abs(m_fAngle-PI) < 0.01f || agk::Abs(m_fAngle-PI*1.5f) < 0.01f )
	if ( (m_bFlags & AGK_SPRITE_SNAP) /*&& (m_fAngle == 0 || m_fAngle == PI/2 || m_fAngle == PI || m_fAngle == PI*1.5f)*/ )
	{
		// m_fTargetViewportHeight has sub pixel accuracy, round to a whole pixel first
		float VwDw = agk::DeviceToDisplayRatioX();
		float VhDh = agk::DeviceToDisplayRatioY();

		int x1i = agk::Floor( x1/VwDw + 0.5f );
		int y1i = agk::Floor( y1/VhDh + 0.5f );
		x1 = (x1i) * VwDw;
		y1 = (y1i) * VhDh;
			
		int x2i = agk::Floor( x2/VwDw + 0.5f );
		int y2i = agk::Floor( y2/VhDh + 0.5f );
		x2 = (x2i) * VwDw;
		y2 = (y2i) * VhDh;
			
		int x3i = agk::Floor( x3/VwDw + 0.5f );
		int y3i = agk::Floor( y3/VhDh + 0.5f );
		x3 = (x3i) * VwDw;
		y3 = (y3i) * VhDh;
			
		int x4i = agk::Floor( x4/VwDw + 0.5f );
		int y4i = agk::Floor( y4/VhDh + 0.5f );
		x4 = (x4i) * VwDw;
		y4 = (y4i) * VhDh;
	}
		
	// triangle 1
	pVertices[ 0 ] = x1;
	pVertices[ 1 ] = y1;
	pVertices[ 2 ] = m_fZ;
	
	pVertices[ 3 ] = x2;
	pVertices[ 4 ] = y2;
	pVertices[ 5 ] = m_fZ;
	
	pVertices[ 6 ] = x3;
	pVertices[ 7 ] = y3;
	pVertices[ 8 ] = m_fZ;
	
	pVertices[ 9 ] = x4;
	pVertices[ 10 ] = y4;
	pVertices[ 11 ] = m_fZ;

	// color setup
	UINT alpha = m_iColor;
	UINT blue = alpha >> 8;
	UINT green = blue >> 8;
	UINT red = green >> 8;

	alpha &= 0xff;
	blue &= 0xff;
	green &= 0xff;
	red &= 0xff;

	pColors = new unsigned char[ 16 ];
	pColors[ 0 ] = red;		pColors[ 1 ] = green;	pColors[ 2 ] = blue;	pColors[ 3 ] = alpha;
	pColors[ 4 ] = red;		pColors[ 5 ] = green;	pColors[ 6 ] = blue;	pColors[ 7 ] = alpha;
	pColors[ 8 ] = red;		pColors[ 9 ] = green;	pColors[ 10 ] = blue;	pColors[ 11 ] = alpha;
	pColors[ 12 ] = red;	pColors[ 13 ] = green;	pColors[ 14 ] = blue;	pColors[ 15 ] = alpha;

	// only bind additional images if base image is present, otherwise the UV coords aren't set and the shader will crash
	if ( m_pImage )
	{
		for ( int i = 1; i < 8; i++ )
		{
			if ( !m_pAdditionalImages[i] ) cImage::BindTexture( 0, i );
			else m_pAdditionalImages[i]->Bind( i );
		}
	}

	// platform specific
	PlatformDraw( pVertices, pUV, pColors );

	if ( pUV ) delete [] pUV;
	if ( pVertices ) delete [] pVertices;
	if ( pColors ) delete [] pColors;
}

void cSprite::PlatformDraw( float *vertices, float *uv, unsigned char *color )
{
	AGKShader *pShader = m_pShader;
	
	agk::PlatformBindBuffer( 0 );
	agk::PlatformBindIndexBuffer( 0 );

	if ( !pShader ) return;
	pShader->MakeActive();
	
	int locPos = pShader->GetAttribPosition();
	int locColor = pShader->GetAttribColor();
	int locTex = pShader->GetAttribTexCoord();

	if ( locPos >= 0 ) pShader->SetAttribFloat( locPos, 3, 0, vertices );
	if ( locColor >= 0 ) pShader->SetAttribUByte( locColor, 4, 0, true, color );
	if ( locTex >= 0 ) pShader->SetAttribFloat( locTex, 2, 0, uv );

	if ( pShader->HasPerSpriteUniforms() )
	{
		pShader->SetTempConstantByName( "agk_spritepos", GetXByOffset(), GetYByOffset(), 0, 0 );
		pShader->SetTempConstantByName( "agk_spritesize", GetWidth(), GetHeight(), 0, 0 );
	}

	// setup user defined shader variables for this object
	sSpriteUniform *pVar = m_cShaderVariables.GetFirst();
	while( pVar ) {
		if( pVar->index < 0 ) pShader->SetTempConstantByName( pVar->m_sName, pVar->v1, pVar->v2, pVar->v3, pVar->v4 );
		else pShader->SetTempConstantArrayByName( pVar->m_sName, pVar->index, pVar->v1, pVar->v2, pVar->v3, pVar->v4 );
		pVar = m_cShaderVariables.GetNext();
	}

	agk::PlatformSetCullMode( 0 );

	agk::PlatformSetBlendMode( m_iTransparencyMode );
	agk::PlatformSetDepthRange( 0, 1 );
	agk::PlatformSetDepthTest( 0 );

	int iScissorX, iScissorY, iScissorWidth, iScissorHeight;
	GetClipValues( iScissorX, iScissorY, iScissorWidth, iScissorHeight );
	if ( iScissorX != 0 || iScissorY != 0 || iScissorWidth != 0 || iScissorHeight != 0 )
	{
		agk::PlatformScissor( iScissorX, iScissorY, iScissorWidth, iScissorHeight );
	}
	else
	{
		agk::ResetScissor();
	}

	// Draw
	pShader->DrawPrimitives( AGK_TRIANGLE_STRIP, 0, 4 ); 
}

//****if* cSprite/BatchDraw
// FUNCTION
//   The sprite filles the given arrays with its vertex, color, and texture values.
// SOURCE
void cSprite::BatchDraw( float *pVertices, float *pUV, unsigned char *pColor )
//****
{
	agk::Error("This command has been depreciated - use BatchDrawQuad instead");
	/*
	// only if sprite visible
	if ( (m_bFlags & AGK_SPRITE_VISIBLE) == 0 ) return;

	int pixelsW = agk::Round( m_fWidth * (agk::GetDeviceWidth() / (float) agk::GetVirtualWidth()) );
	int pixelsH = agk::Round( m_fHeight * (agk::GetDeviceHeight() / (float) agk::GetVirtualHeight()) );
	g_iPixelsDrawn += (pixelsW * pixelsH);

	// check assigned images still exist
	CheckImages();

	// texture coords
	if ( pUV != UNDEF )
	{
		if ( m_pImage != UNDEF )
		{
			if ( m_bUVOverride )
			{
				// triangle 1
				pUV[ 0 ] = m_fU1;		pUV[ 1 ] = m_fV1;
				pUV[ 2 ] = m_fU2;		pUV[ 3 ] = m_fV2;
				pUV[ 4 ] = m_fU3;		pUV[ 5 ] = m_fV3;

				// triangle 2
				pUV[ 6 ] = m_fU3;		pUV[ 7 ] = m_fV3;
				pUV[ 8 ] = m_fU2;		pUV[ 9 ] = m_fV2;
				pUV[ 10 ] = m_fU4;		pUV[ 11 ] = m_fV4;
			}
			else
			{
				float fU1 = m_pImage->GetU1();
				float fV1 = m_pImage->GetV1();
				float fU2 = m_pImage->GetU2();
				float fV2 = m_pImage->GetV2();

				// modify UVs if animated
				if ( m_iFrameCount > 0 )
				{
					fU1 = m_pFrames[ m_iCurrentFrame ].m_fU1;
					fV1 = m_pFrames[ m_iCurrentFrame ].m_fV1;
					fU2 = m_pFrames[ m_iCurrentFrame ].m_fU2;
					fV2 = m_pFrames[ m_iCurrentFrame ].m_fV2;
				}

				if ( m_fUScale != 1 )
				{
					float diff = fU2 - fU1;
					diff /= m_fUScale;
					fU2 = fU1 + diff;
				}
				fU1 += m_fUOffset;
				fU2 += m_fUOffset;

				if ( m_fVScale != 1 )
				{
					float diff = fV2 - fV1;
					diff /= m_fVScale;
					fV2 = fV1 + diff;
				}
				fV1 += m_fVOffset;
				fV2 += m_fVOffset;

				if ( m_fUVBorder > 0 )
				{
					fU1 += m_fUVBorder / m_pImage->GetTotalWidth();
					fV1 += m_fUVBorder / m_pImage->GetTotalHeight();
					fU2 -= m_fUVBorder / m_pImage->GetTotalWidth();
					fV2 -= m_fUVBorder / m_pImage->GetTotalHeight();
				}

				if ( (m_bFlags & AGK_SPRITE_FLIPH) > 0 )
				{
					float temp = fU1;
					fU1 = fU2;
					fU2 = temp;
				}

				if ( (m_bFlags & AGK_SPRITE_FLIPV) > 0 )
				{
					float temp = fV1;
					fV1 = fV2;
					fV2 = temp;
				}

				// triangle 1
				pUV[ 0 ] = fU1;		pUV[ 1 ] = fV1;
				pUV[ 2 ] = fU1;		pUV[ 3 ] = fV2;
				pUV[ 4 ] = fU2;		pUV[ 5 ] = fV1;

				// triangle 2
				pUV[ 6 ] = fU2;		pUV[ 7 ] = fV1;
				pUV[ 8 ] = fU1;		pUV[ 9 ] = fV2;
				pUV[ 10 ] = fU2;	pUV[ 11 ] = fV2;
			}
		}
		else
		{
			pUV[ 0 ] = 0.0f;		pUV[ 1 ] = 0.0f;
			pUV[ 2 ] = 0.0f;		pUV[ 3 ] = 0.0f;
			pUV[ 4 ] = 0.0f;		pUV[ 5 ] = 0.0f;

			pUV[ 6 ] = 0.0f;		pUV[ 7 ] = 0.0f;
			pUV[ 8 ] = 0.0f;		pUV[ 9 ] = 0.0f;
			pUV[ 10 ] = 0.0f;		pUV[ 11 ] = 0.0f;
		}
	}

	// vertex setup
	if ( pVertices != UNDEF )
	{
		float x1,x2,x3,x4;
		float y1,y2,y3,y4;
		
		if ( m_fAngle != 0 )
		{
			float stretch = agk::m_fStretchValue;
			float fSinA = agk::SinRad(m_fAngle);
			float fCosA = agk::CosRad(m_fAngle);
			float fSinA1 = fSinA/stretch;
			float fSinA2 = fSinA*stretch;

			float fTempX = (-m_fOffsetX);
			float fTempY = (-m_fOffsetY);
			x1 = m_fX + (fTempX*fCosA - fTempY*fSinA1);
			y1 = m_fY + (fTempY*fCosA + fTempX*fSinA2);

			fTempX = (-m_fOffsetX);
			fTempY = (-m_fOffsetY + m_fHeight);
			x2 = m_fX + (fTempX*fCosA - fTempY*fSinA1);
			y2 = m_fY + (fTempY*fCosA + fTempX*fSinA2);

			fTempX = (-m_fOffsetX + m_fWidth);
			fTempY = (-m_fOffsetY);
			x3 = m_fX + (fTempX*fCosA - fTempY*fSinA1);
			y3 = m_fY + (fTempY*fCosA + fTempX*fSinA2);

			fTempX = (-m_fOffsetX + m_fWidth);
			fTempY = (-m_fOffsetY + m_fHeight);
			x4 = m_fX + (fTempX*fCosA - fTempY*fSinA1);
			y4 = m_fY + (fTempY*fCosA + fTempX*fSinA2);
		}
		else
		{
			x1 = m_fX - m_fOffsetX;
			y1 = m_fY - m_fOffsetY;
			
			x2 = m_fX - m_fOffsetX;
			y2 = m_fY - m_fOffsetY + m_fHeight;
			
			x3 = m_fX - m_fOffsetX + m_fWidth;
			y3 = m_fY - m_fOffsetY;
			
			x4 = m_fX - m_fOffsetX + m_fWidth;
			y4 = m_fY - m_fOffsetY + m_fHeight;
		}

		if ( (m_bFlags & AGK_SPRITE_SCROLL) > 0 )
		{
			x1 = agk::WorldToScreenX( x1 );
			y1 = agk::WorldToScreenY( y1 );

			x2 = agk::WorldToScreenX( x2 );
			y2 = agk::WorldToScreenY( y2 );

			x3 = agk::WorldToScreenX( x3 );
			y3 = agk::WorldToScreenY( y3 );

			x4 = agk::WorldToScreenX( x4 );
			y4 = agk::WorldToScreenY( y4 );
		}
		
		//if ( agk::Abs(m_fAngle) < 0.01f || agk::Abs(m_fAngle-PI/2) < 0.01f 
		//  || agk::Abs(m_fAngle-PI) < 0.01f || agk::Abs(m_fAngle-PI*1.5f) < 0.01f )
		if ( (m_bFlags & AGK_SPRITE_SNAP) 
		{
			// correct for floating point pixel positions
			// m_fTargetViewportHeight has sub pixel accuracy, round to a whole pixel first
			float VwDw = agk::Round(agk::m_fTargetViewportWidth) / (float) agk::m_iDisplayWidth;
			float VhDh = agk::Round(agk::m_fTargetViewportHeight) / (float) agk::m_iDisplayHeight;

			if ( agk::m_bUsingFBO )
			{
				VwDw = agk::m_iFBOWidth / (agk::m_iDisplayExtraX*2 + agk::m_iDisplayWidth);
				VhDh = agk::m_iFBOHeight / (agk::m_iDisplayExtraY*2 + agk::m_iDisplayHeight);
			}
			
			if( agk::m_iOrientation > 2 && agk::GetAGKShouldRotate() )
			{
				VwDw = agk::Round(agk::m_fTargetViewportHeight) / (float) agk::m_iDisplayWidth;
				VhDh = agk::Round(agk::m_fTargetViewportWidth) / (float) agk::m_iDisplayHeight;
			}
			
			int x1i = agk::Round( x1*VwDw );
			int y1i = agk::Round( y1*VhDh );
			x1 = x1i / VwDw;
			y1 = y1i / VhDh;
			
			int x2i = agk::Round( x2*VwDw );
			int y2i = agk::Round( y2*VhDh );
			x2 = x2i / VwDw;
			y2 = y2i / VhDh;
			
			int x3i = agk::Round( x3*VwDw );
			int y3i = agk::Round( y3*VhDh );
			x3 = x3i / VwDw;
			y3 = y3i / VhDh;
			
			int x4i = agk::Round( x4*VwDw );
			int y4i = agk::Round( y4*VhDh );
			x4 = x4i / VwDw;
			y4 = y4i / VhDh;
		}

		// triangle 1
		pVertices[ 0 ] = x1;
		pVertices[ 1 ] = y1;
		pVertices[ 2 ] = m_fZ;

		pVertices[ 3 ] = x2;
		pVertices[ 4 ] = y2;
		pVertices[ 5 ] = m_fZ;

		pVertices[ 6 ] = x3;
		pVertices[ 7 ] = y3;
		pVertices[ 8 ] = m_fZ;

		// triangle 2
		pVertices[ 9 ] = x3;
		pVertices[ 10 ] = y3;
		pVertices[ 11 ] = m_fZ;

		pVertices[ 12 ] = x2;
		pVertices[ 13 ] = y2;
		pVertices[ 14 ] = m_fZ;

		pVertices[ 15 ] = x4;
		pVertices[ 16 ] = y4;
		pVertices[ 17 ] = m_fZ;
	}

	UINT alpha = m_iColor;
	UINT blue = alpha >> 8;
	UINT green = blue >> 8;
	UINT red = green >> 8;

	alpha &= 0xff;
	blue &= 0xff;
	green &= 0xff;
	red &= 0xff;

	// color setup
	if ( pColor != UNDEF )
	{
		pColor[ 0 ] = red;	pColor[ 1 ] = green;	pColor[ 2 ] = blue;		pColor[ 3 ] = alpha;
		pColor[ 4 ] = red;	pColor[ 5 ] = green;	pColor[ 6 ] = blue;		pColor[ 7 ] = alpha;
		pColor[ 8 ] = red;	pColor[ 9 ] = green;	pColor[ 10 ] = blue;	pColor[ 11 ] = alpha;

		pColor[ 12 ] = red;	pColor[ 13 ] = green;	pColor[ 14 ] = blue;	pColor[ 15 ] = alpha;
		pColor[ 16 ] = red;	pColor[ 17 ] = green;	pColor[ 18 ] = blue;	pColor[ 19 ] = alpha;
		pColor[ 20 ] = red;	pColor[ 21 ] = green;	pColor[ 22 ] = blue;	pColor[ 23 ] = alpha;
	}
	*/
}

//****if* cSprite/BatchDrawQuad
// FUNCTION
//   The sprite fills the given arrays with its vertex, color, and texture values using only 4 vertices.
// SOURCE
void cSprite::BatchDrawQuad( float *pVertices, float *pUV, unsigned char *pColor )
//****
{
	// only if sprite visible
	if ( (m_bFlags & AGK_SPRITE_VISIBLE) == 0 ) return;

	int pixelsW = agk::Round( m_fWidth * (agk::GetDeviceWidth() / (float) agk::GetVirtualWidth()) );
	int pixelsH = agk::Round( m_fHeight * (agk::GetDeviceHeight() / (float) agk::GetVirtualHeight()) );
	g_iPixelsDrawn += (pixelsW * pixelsH);
	
	// check assigned images still exist, this is now handled by cSprite::RemoveImage()
	//CheckImages();
	
	// texture coords
	if ( pUV != UNDEF )
	{
		if ( m_pImage != UNDEF )
		{
			if ( m_bUVOverride )
			{
				pUV[ 0 ] = m_fU1;		pUV[ 1 ] = m_fV1;
				pUV[ 2 ] = m_fU2;		pUV[ 3 ] = m_fV2;
				pUV[ 4 ] = m_fU3;		pUV[ 5 ] = m_fV3;
				pUV[ 6 ] = m_fU4;		pUV[ 7 ] = m_fV4;
			}
			else
			{
				float fU1 = m_pImage->GetU1();
				float fV1 = m_pImage->GetV1();
				float fU2 = m_pImage->GetU2();
				float fV2 = m_pImage->GetV2();
				
				// modify UVs if animated
				if ( m_iFrameCount > 0 )
				{
					fU1 = m_pFrames[ m_iCurrentFrame ].m_fU1;
					fV1 = m_pFrames[ m_iCurrentFrame ].m_fV1;
					fU2 = m_pFrames[ m_iCurrentFrame ].m_fU2;
					fV2 = m_pFrames[ m_iCurrentFrame ].m_fV2;
				}
				
				if ( m_fUScale != 1 )
				{
					float diff = fU2 - fU1;
					diff /= m_fUScale;
					fU2 = fU1 + diff;
				}
				fU1 += m_fUOffset;
				fU2 += m_fUOffset;
				
				if ( m_fVScale != 1 )
				{
					float diff = fV2 - fV1;
					diff /= m_fVScale;
					fV2 = fV1 + diff;
				}
				fV1 += m_fVOffset;
				fV2 += m_fVOffset;
				
				if ( m_fUVBorder > 0 )
				{
					fU1 += m_fUVBorder / m_pImage->GetTotalWidth();
					fV1 += m_fUVBorder / m_pImage->GetTotalHeight();
					fU2 -= m_fUVBorder / m_pImage->GetTotalWidth();
					fV2 -= m_fUVBorder / m_pImage->GetTotalHeight();
				}
				
				if ( (m_bFlags & AGK_SPRITE_FLIPH) > 0 )
				{
					float temp = fU1;
					fU1 = fU2;
					fU2 = temp;
				}
				
				if ( (m_bFlags & AGK_SPRITE_FLIPV) > 0 )
				{
					float temp = fV1;
					fV1 = fV2;
					fV2 = temp;
				}
				
				pUV[ 0 ] = fU1;		pUV[ 1 ] = fV1;
				pUV[ 2 ] = fU1;		pUV[ 3 ] = fV2;
				pUV[ 4 ] = fU2;		pUV[ 5 ] = fV1;
				pUV[ 6 ] = fU2;		pUV[ 7 ] = fV2;
			}
		}
		else
		{
			pUV[ 0 ] = 0.0f;		pUV[ 1 ] = 0.0f;
			pUV[ 2 ] = 0.0f;		pUV[ 3 ] = 0.0f;
			pUV[ 4 ] = 0.0f;		pUV[ 5 ] = 0.0f;
			pUV[ 6 ] = 0.0f;		pUV[ 7 ] = 0.0f;
		}
	}
	
	// vertex setup
	if ( pVertices != UNDEF )
	{
		float x1,x2,x3,x4;
		float y1,y2,y3,y4;
		
		if ( m_fAngle != 0 )
		{
			float boneScaleX = 1;
			float boneScaleY = 1;
			float boneScaleXInv = 1;
			float boneScaleYInv = 1;
			if ( m_pBone && (m_pBone->m_bFlags & AGK_BONE_PRE_SCALE) != 0 )
			{
				boneScaleX = m_pBone->worldSX;
				boneScaleY = m_pBone->worldSY;
				boneScaleXInv = 1.0f / boneScaleX;
				boneScaleYInv = 1.0f / boneScaleY;
			}

			float stretch = agk::m_fStretchValue;
			float fSinA = agk::SinRad(m_fAngle);
			float fCosA = agk::CosRad(m_fAngle);
			float fSinA1 = fSinA/stretch;
			float fSinA2 = fSinA*stretch;
			
			float fTempX = (-m_fOffsetX)*boneScaleX;
			float fTempY = (-m_fOffsetY)*boneScaleY;
			x1 = m_fX + (fTempX*fCosA - fTempY*fSinA1)*boneScaleXInv;
			y1 = m_fY + (fTempY*fCosA + fTempX*fSinA2)*boneScaleYInv;

			fTempX = (-m_fOffsetX)*boneScaleX;
			fTempY = (-m_fOffsetY + m_fHeight)*boneScaleY;
			x2 = m_fX + (fTempX*fCosA - fTempY*fSinA1)*boneScaleXInv;
			y2 = m_fY + (fTempY*fCosA + fTempX*fSinA2)*boneScaleYInv;

			fTempX = (-m_fOffsetX + m_fWidth)*boneScaleX;
			fTempY = (-m_fOffsetY)*boneScaleY;
			x3 = m_fX + (fTempX*fCosA - fTempY*fSinA1)*boneScaleXInv;
			y3 = m_fY + (fTempY*fCosA + fTempX*fSinA2)*boneScaleYInv;

			fTempX = (-m_fOffsetX + m_fWidth)*boneScaleX;
			fTempY = (-m_fOffsetY + m_fHeight)*boneScaleY;
			x4 = m_fX + (fTempX*fCosA - fTempY*fSinA1)*boneScaleXInv;
			y4 = m_fY + (fTempY*fCosA + fTempX*fSinA2)*boneScaleYInv;
		}
		else
		{
			x1 = m_fX - m_fOffsetX;
			y1 = m_fY - m_fOffsetY;
			
			x2 = m_fX - m_fOffsetX;
			y2 = m_fY - m_fOffsetY + m_fHeight;
			
			x3 = m_fX - m_fOffsetX + m_fWidth;
			y3 = m_fY - m_fOffsetY;
			
			x4 = m_fX - m_fOffsetX + m_fWidth;
			y4 = m_fY - m_fOffsetY + m_fHeight;
		}

		// check for bone transforms
		if ( m_pBone )
		{
			float x1t = m_pBone->m00 * x1 + m_pBone->m01 * y1 + m_pBone->worldX;
			float y1t = m_pBone->m10 * x1 + m_pBone->m11 * y1 + m_pBone->worldY;

			float x2t = m_pBone->m00 * x2 + m_pBone->m01 * y2 + m_pBone->worldX;
			float y2t = m_pBone->m10 * x2 + m_pBone->m11 * y2 + m_pBone->worldY;

			float x3t = m_pBone->m00 * x3 + m_pBone->m01 * y3 + m_pBone->worldX;
			float y3t = m_pBone->m10 * x3 + m_pBone->m11 * y3 + m_pBone->worldY;

			float x4t = m_pBone->m00 * x4 + m_pBone->m01 * y4 + m_pBone->worldX;
			float y4t = m_pBone->m10 * x4 + m_pBone->m11 * y4 + m_pBone->worldY;

			x1 = x1t; y1 = y1t;
			x2 = x2t; y2 = y2t;
			x3 = x3t; y3 = y3t;
			x4 = x4t; y4 = y4t;
		}
		
		if ( (m_bFlags & AGK_SPRITE_SCROLL) > 0 )
		{
			x1 = agk::WorldToScreenX( x1 );
			y1 = agk::WorldToScreenY( y1 );
			
			x2 = agk::WorldToScreenX( x2 );
			y2 = agk::WorldToScreenY( y2 );
			
			x3 = agk::WorldToScreenX( x3 );
			y3 = agk::WorldToScreenY( y3 );
			
			x4 = agk::WorldToScreenX( x4 );
			y4 = agk::WorldToScreenY( y4 );
		}
		
		//if ( agk::Abs(m_fAngle) < 0.01f || agk::Abs(m_fAngle-PI/2) < 0.01f 
		//  || agk::Abs(m_fAngle-PI) < 0.01f || agk::Abs(m_fAngle-PI*1.5f) < 0.01f )
		if ( (m_bFlags & AGK_SPRITE_SNAP) /*&& (m_fAngle == 0 || m_fAngle == PI/2 || m_fAngle == PI || m_fAngle == PI*1.5f)*/ )
		{
			// correct for floating point pixel positions
			// m_fTargetViewportHeight has sub pixel accuracy, round to a whole pixel first
			float VwDw = agk::DeviceToDisplayRatioX();
			float VhDh = agk::DeviceToDisplayRatioY();
			
			int x1i = agk::Floor( x1/VwDw + 0.5f );
			int y1i = agk::Floor( y1/VhDh + 0.5f );
			x1 = (x1i) * VwDw;
			y1 = (y1i) * VhDh;
			
			int x2i = agk::Floor( x2/VwDw + 0.5f );
			int y2i = agk::Floor( y2/VhDh + 0.5f );
			x2 = (x2i) * VwDw;
			y2 = (y2i) * VhDh;
			
			int x3i = agk::Floor( x3/VwDw + 0.5f );
			int y3i = agk::Floor( y3/VhDh + 0.5f );
			x3 = (x3i) * VwDw;
			y3 = (y3i) * VhDh;
			
			int x4i = agk::Floor( x4/VwDw + 0.5f );
			int y4i = agk::Floor( y4/VhDh + 0.5f );
			x4 = (x4i) * VwDw;
			y4 = (y4i) * VhDh;
		}
		
		pVertices[ 0 ] = x1;
		pVertices[ 1 ] = y1;
		pVertices[ 2 ] = m_fZ;
		
		pVertices[ 3 ] = x2;
		pVertices[ 4 ] = y2;
		pVertices[ 5 ] = m_fZ;
		
		pVertices[ 6 ] = x3;
		pVertices[ 7 ] = y3;
		pVertices[ 8 ] = m_fZ;
		
		pVertices[ 9 ] = x4;
		pVertices[ 10 ] = y4;
		pVertices[ 11 ] = m_fZ;
	}
	
	UINT alpha = m_iColor;
	UINT blue = alpha >> 8;
	UINT green = blue >> 8;
	UINT red = green >> 8;
	
	alpha &= 0xff; 
	blue &= 0xff;
	green &= 0xff;
	red &= 0xff;
	
	// color setup
	if ( pColor != UNDEF )
	{
		pColor[ 0 ] = red;	pColor[ 1 ] = green;	pColor[ 2 ] = blue;		pColor[ 3 ] = alpha;
		pColor[ 4 ] = red;	pColor[ 5 ] = green;	pColor[ 6 ] = blue;		pColor[ 7 ] = alpha;
		pColor[ 8 ] = red;	pColor[ 9 ] = green;	pColor[ 10 ] = blue;	pColor[ 11 ] = alpha;
		pColor[ 12 ] = red;	pColor[ 13 ] = green;	pColor[ 14 ] = blue;	pColor[ 15 ] = alpha;
	}
}

// Update sprite animation, physics position, etc
void cSprite::Update( float time )
{
	//CheckImages();

	if ( (m_bFlags & AGK_SPRITE_ACTIVE) != 0 )
	{
		UpdatePhysics();
		UpdateAnimation( time );
	}
}

// Advance the sprite animation by the necessary frames to keep up with frame time 
void cSprite::UpdateAnimation( float time )
{
	if ( m_iFrameCount > 0 && (m_bFlags & AGK_SPRITE_PLAYING) != 0 )
	{
		m_fFrameTimer += time;
		while ( m_fFrameTimer > m_fFrameChangeTime )
		{
			// advance one frame
			m_fFrameTimer -= m_fFrameChangeTime;

			if ( m_iFrameEnd >= m_iFrameStart ) 
			{
				m_iCurrentFrame++;

				if ( m_iCurrentFrame > m_iFrameEnd )
				{
					// reached the end
					if ( (m_bFlags & AGK_SPRITE_LOOP) != 0 ) m_iCurrentFrame = m_iFrameStart;
					else 
					{
						m_iCurrentFrame = m_iFrameEnd;
						m_bFlags &= ~AGK_SPRITE_PLAYING;
						break;
					}
				}
			}
			else 
			{
				m_iCurrentFrame--;

				if ( m_iCurrentFrame < m_iFrameEnd )
				{
					// reached the end
					if ( (m_bFlags & AGK_SPRITE_LOOP) != 0 ) m_iCurrentFrame = m_iFrameStart;
					else 
					{
						m_iCurrentFrame = m_iFrameEnd;
						m_bFlags &= ~AGK_SPRITE_PLAYING;
						break;
					}
				}
			}			

			if ( m_pFrames[ m_iCurrentFrame ].m_pFrameImage != m_pImage ) 
			{
				SwitchImage( m_pFrames[ m_iCurrentFrame ].m_pFrameImage, false );

				if ( m_iFrameWidth != m_pFrames[ m_iCurrentFrame ].m_iWidth || m_iFrameHeight != m_pFrames[ m_iCurrentFrame ].m_iHeight )
				{
					m_iFrameWidth = m_pFrames[ m_iCurrentFrame ].m_iWidth;
					m_iFrameHeight = m_pFrames[ m_iCurrentFrame ].m_iHeight;

					// sprite may need resizing
					float fNewWidth = m_fWidth;
					float fNewHeight = m_fHeight;

					if ( (m_bFlags & AGK_SPRITE_WIDTHCALC) != 0 ) fNewWidth = -1;
					if ( (m_bFlags & AGK_SPRITE_HEIGHTCALC) != 0 ) fNewHeight = -1;

					float oldScaleX = m_fWidth / m_fOrigWidth;
					float oldScaleY = m_fHeight / m_fOrigHeight;
					SetSize( fNewWidth, fNewHeight, false );
					if ( oldScaleX != 1 || oldScaleY != 1 ) SetScaleByOffset( oldScaleX, oldScaleY );
				}
			}
		}
	}
}

// Sprite physics functions
void cSprite::ReplacePhysicsShape( b2Shape *pOldShape, b2Shape *pNewShape )
{
	if ( !pOldShape && !pNewShape ) return;

	if ( pNewShape && pNewShape == m_phyShape )
	{
		float scale = m_fWidth / m_fOrigWidth;
		float scaleY = m_fHeight / m_fOrigHeight;
		if ( scaleY > scale ) scale = scaleY;
		m_fOrigRadius = m_phyShape->m_radius / scale;
	}
	
	if ( !m_phyBody ) 
	{
		if ( pOldShape ) delete pOldShape;
		return;
	}

	b2FixtureDef sFixDef;
	sFixDef.shape = pNewShape;
	sFixDef.localShapeMemory = false;
	if ( pOldShape )
	{
		// replace fixtures
		b2Fixture *pFix = m_phyBody->GetFixtureList();
		b2Fixture *pNext = 0;
		while( pFix )
		{
			pNext = pFix->GetNext();
			if ( pFix->GetShape() == pOldShape )
			{
				if ( !pNewShape ) m_phyBody->DestroyFixture( pFix );
				else
				{
					sFixDef.density = pFix->GetDensity();
					sFixDef.filter = pFix->GetFilterData();
					sFixDef.friction = pFix->GetFriction();
					sFixDef.isSensor = pFix->IsSensor();
					sFixDef.restitution = pFix->GetRestitution();
					sFixDef.userData = pFix->GetUserData();

					m_phyBody->DestroyFixture( pFix );
					m_phyBody->CreateFixture( &sFixDef );
				}
			}
			pFix = pNext;
		}
		delete pOldShape;
	}
	else
	{
		// new fixture
		b2Fixture *pFix = m_phyBody->GetFixtureList();
		if ( pFix )
		{
			// copy from other fixture
			sFixDef.density = pFix->GetDensity();
			sFixDef.filter = pFix->GetFilterData();
			sFixDef.friction = pFix->GetFriction();
			sFixDef.isSensor = pFix->IsSensor();
			sFixDef.restitution = pFix->GetRestitution();
		}
		else
		{
			// use default values
			sFixDef.density = 1.0f;
			sFixDef.friction = 0.3f;
			sFixDef.restitution = 0.1f;
			sFixDef.isSensor = 0;
			sFixDef.filter.categoryBits = m_iCategories & 0x0000ffff;
			sFixDef.filter.maskBits = m_iCategoryMask & 0x0000ffff;
			sFixDef.filter.groupIndex = m_iGroup;
		}

		m_phyBody->CreateFixture( &sFixDef );
	}
}

void cSprite::CalculatePhysicsCOM()
{
	if ( !m_phyBody ) return;
		
	m_phyBody->ResetMassData();
}

void cSprite::SetPhysicsCOM( float x, float y )
{
	if ( !m_phyBody ) return;
		
	b2MassData mass;
	mass.center.Set( agk::WorldToPhyX(x),agk::WorldToPhyY(y) );
	mass.mass = m_phyBody->GetMass();
	mass.I = m_phyBody->GetInertia();
	m_phyBody->SetMassData( &mass );
}

float cSprite::GetPhysicsCOMX()
{
	if ( !m_phyBody ) return 0;
	return agk::PhyToWorldX( m_phyBody->m_sweep.localCenter.x );
}

float cSprite::GetPhysicsCOMY()
{
	if ( !m_phyBody ) return 0;
	return agk::PhyToWorldY( m_phyBody->m_sweep.localCenter.y );
}

bool cSprite::InBox( float x1, float y1, float x2, float y2 )
{
	if ( !m_phyShape ) 
	{
		SetShape( eBox );
	}

	// quick reject
	if ( m_fX + agk::PhyToWorldX(m_fColRadius) < x1 ) return false;
	if ( m_fX - agk::PhyToWorldX(m_fColRadius) > x2 ) return false;
	if ( m_fY + agk::PhyToWorldY(m_fColRadius) < y1 ) return false;
	if ( m_fY - agk::PhyToWorldY(m_fColRadius) > y2 ) return false;

	b2Transform transform1; transform1.Set( b2Vec2(agk::WorldToPhyX(m_fX),agk::WorldToPhyY(m_fY)), m_fAngle );
	b2Transform transform2; transform2.SetIdentity();

	if ( x1 > x2 )
	{
		float temp = x2;
		x2 = x1;
		x1 = temp;
	}

	if ( y1 > y2 )
	{
		float temp = y2;
		y2 = y1;
		y1 = temp;
	}

	x1 = agk::WorldToPhyX( x1 );
	y1 = agk::WorldToPhyY( y1 );
	x2 = agk::WorldToPhyX( x2 );
	y2 = agk::WorldToPhyY( y2 );

	b2PolygonShape poly;
	poly.SetAsBox( (x2-x1)/2.0f, (y2-y1)/2.0f, b2Vec2((x2+x1)/2.0f,(y2+y1)/2.0f), 0 );

	// box-polygon
	if ( m_phyShape->GetType() == b2Shape::e_polygon )
	{
		b2Manifold manifold;
		b2CollidePolygons( &manifold, (b2PolygonShape*)m_phyShape, transform1, &poly, transform2 );
		return (manifold.pointCount > 0);
	}

	// must be box-circle
	b2PolygonShape *polygonA = &poly;
	b2CircleShape *circleB = (b2CircleShape*)m_phyShape;
		
	// Compute circle position in the frame of the polygon.
	b2Vec2 c = b2Mul(transform1, circleB->m_p);
	b2Vec2 cLocal = b2MulT(transform2, c);

	// Find the min separating edge.
	int32 normalIndex = 0;
	float32 separation = -b2_maxFloat;
	float32 radius = polygonA->m_radius + circleB->m_radius;
	int32 vertexCount = polygonA->m_vertexCount;
	const b2Vec2* vertices = polygonA->m_vertices;
	const b2Vec2* normals = polygonA->m_normals;

	for (int32 i = 0; i < vertexCount; ++i)
	{
		float32 s = b2Dot(normals[i], cLocal - vertices[i]);

		// Early out.
		if (s > radius) return false;

		if (s > separation)
		{
			separation = s;
			normalIndex = i;
		}
	}

	// Vertices that subtend the incident face.
	int32 vertIndex1 = normalIndex;
	int32 vertIndex2 = vertIndex1 + 1 < vertexCount ? vertIndex1 + 1 : 0;
	b2Vec2 v1 = vertices[vertIndex1];
	b2Vec2 v2 = vertices[vertIndex2];

	// If the center is inside the polygon ...
	if (separation < b2_epsilon)
	{
		return true;
	}

	// Compute barycentric coordinates
	float32 u1 = b2Dot(cLocal - v1, v2 - v1);
	float32 u2 = b2Dot(cLocal - v2, v1 - v2);
	if (u1 <= 0.0f)
	{
		return (b2DistanceSquared(cLocal, v1) <= radius * radius);
	}
	else if (u2 <= 0.0f)
	{
		return (b2DistanceSquared(cLocal, v2) <= radius * radius);
	}
	else
	{
		b2Vec2 faceCenter = 0.5f * (v1 + v2);
		float32 separation = b2Dot(cLocal - faceCenter, normals[vertIndex1]);
		return (separation <= radius);
	}
}

bool cSprite::InCircle( float x1, float y1, float radius )
{
	if ( !m_phyShape ) 
	{
		SetShape( eBox );
	}

	// quick reject
	float diffX = agk::WorldToPhyX(m_fX - x1);
	float diffY = agk::WorldToPhyY(m_fY - y1);
	float radius2 = agk::WorldToPhyX(radius);
	float radiusSqr = m_fColRadius*m_fColRadius + radius2*radius2 + 2*m_fColRadius*radius2;
	if ( diffX*diffX + diffY*diffY > radiusSqr ) return false;

	b2Transform transform1; transform1.Set( b2Vec2(agk::WorldToPhyX(m_fX),agk::WorldToPhyY(m_fY)), m_fAngle );
	b2Transform transform2; transform2.SetIdentity();

	x1 = agk::WorldToPhyX( x1 );
	y1 = agk::WorldToPhyY( y1 );
	radius = agk::WorldToPhyX( radius );

	b2CircleShape circle;
	circle.m_p.Set( x1, y1 );
	circle.m_radius = radius;

	// circle-circle
	if ( m_phyShape->GetType() == b2Shape::e_circle )
	{
		b2Vec2 pA = b2Mul(transform1, ((b2CircleShape*)m_phyShape)->m_p);
		b2Vec2 pB = b2Mul(transform2, (circle.m_p));

		b2Vec2 d = pB - pA;
		float32 distSqr = b2Dot(d, d);
		float32 rA = m_phyShape->m_radius, rB = circle.m_radius;
		float32 radius = rA + rB;
		return (distSqr < radius * radius);
	}

	// must be polygon-circle
	b2PolygonShape *polygonA = (b2PolygonShape*)m_phyShape;
	b2CircleShape *circleB = &circle;;
		
	// Compute circle position in the frame of the polygon.
	b2Vec2 c = b2Mul(transform2, circleB->m_p);
	b2Vec2 cLocal = b2MulT(transform1, c);

	// Find the min separating edge.
	int32 normalIndex = 0;
	float32 separation = -b2_maxFloat;
	radius = polygonA->m_radius + circleB->m_radius;
	int32 vertexCount = polygonA->m_vertexCount;
	const b2Vec2* vertices = polygonA->m_vertices;
	const b2Vec2* normals = polygonA->m_normals;

	for (int32 i = 0; i < vertexCount; ++i)
	{
		float32 s = b2Dot(normals[i], cLocal - vertices[i]);

		// Early out.
		if (s > radius) return false;

		if (s > separation)
		{
			separation = s;
			normalIndex = i;
		}
	}

	// Vertices that subtend the incident face.
	int32 vertIndex1 = normalIndex;
	int32 vertIndex2 = vertIndex1 + 1 < vertexCount ? vertIndex1 + 1 : 0;
	b2Vec2 v1 = vertices[vertIndex1];
	b2Vec2 v2 = vertices[vertIndex2];

	// If the center is inside the polygon ...
	if (separation < b2_epsilon) return true;

	// Compute barycentric coordinates
	float32 u1 = b2Dot(cLocal - v1, v2 - v1);
	float32 u2 = b2Dot(cLocal - v2, v1 - v2);
	if (u1 <= 0.0f)
	{
		return (b2DistanceSquared(cLocal, v1) <= radius * radius);
	}
	else if (u2 <= 0.0f)
	{
		return (b2DistanceSquared(cLocal, v2) <= radius * radius);
	}
	else
	{
		b2Vec2 faceCenter = 0.5f * (v1 + v2);
		float32 separation = b2Dot(cLocal - faceCenter, normals[vertIndex1]);
		return (separation <= radius);
	}
}

bool cSprite::GetCollision( cSprite *pSprite2 )
{
	if ( !m_phyShape ) 
	{
		// create a default box
		SetShape( eBox );
	}

	if ( !pSprite2->m_phyShape ) 
	{
		// create a default box
		pSprite2->SetShape( eBox );
	}

	float fX1 = m_fX;
	float fY1 = m_fY;
	float fX2 = pSprite2->m_fX;
	float fY2 = pSprite2->m_fY;
	float fColRadius1 = m_fColRadius;
	float fColRadius2 = pSprite2->m_fColRadius;

	b2Shape* shape1 = 0;
	switch( m_phyShape->GetType() )
	{
		case b2Shape::e_polygon:
		{
			shape1 = new b2PolygonShape();
			*((b2PolygonShape*)shape1) = *((b2PolygonShape*)m_phyShape);
			break;
		}

		case b2Shape::e_circle:
		{
			shape1 = new b2CircleShape();
			*((b2CircleShape*)shape1) = *((b2CircleShape*)m_phyShape);
			break;
		}
        default: agk::Error( "Unsupported Box2D shape" );
	}

	b2Shape* shape2 = 0;
	switch( pSprite2->m_phyShape->GetType() )
	{
		case b2Shape::e_polygon:
		{
			shape2 = new b2PolygonShape();
			*((b2PolygonShape*)shape2) = *((b2PolygonShape*)pSprite2->m_phyShape);
			break;
		}

		case b2Shape::e_circle:
		{
			shape2 = new b2CircleShape();
			*((b2CircleShape*)shape2) = *((b2CircleShape*)pSprite2->m_phyShape);
			break;
		}
        default: agk::Error( "Unsupported Box2D shape" );
	}

	if ( !shape1 || !shape2 ) 
	{
		if ( shape1 ) delete shape1;
		if ( shape2 ) delete shape2;
		return false;
	}

	if ( (m_bFlags & AGK_SPRITE_SCROLL) == 0 )
	{
		fX1 = agk::ScreenToWorldX( fX1 );
		fY1 = agk::ScreenToWorldY( fY1 );
		fColRadius1 /= agk::GetViewZoom();

		if ( shape1->GetType() == b2Shape::e_polygon )
		{
			int vertexCount = ((b2PolygonShape*)shape1)->m_vertexCount;
			b2Vec2 *vertices = ((b2PolygonShape*)shape1)->m_vertices;

			for ( int i = 0; i < vertexCount; i++ )
			{
				vertices[ i ].x /= agk::GetViewZoom();
				vertices[ i ].y /= agk::GetViewZoom();
			}
		}
			
		if ( shape1->GetType() == b2Shape::e_circle )
		{
			shape1->m_radius /= agk::GetViewZoom();
		}
	}

	if ( (pSprite2->m_bFlags & AGK_SPRITE_SCROLL) == 0 )
	{
		fX2 = agk::ScreenToWorldX( fX2 );
		fY2 = agk::ScreenToWorldY( fY2 );
		fColRadius2 /= agk::GetViewZoom();

		if ( shape2->GetType() == b2Shape::e_polygon )
		{
			int vertexCount = ((b2PolygonShape*)shape2)->m_vertexCount;
			b2Vec2 *vertices = ((b2PolygonShape*)shape2)->m_vertices;

			for ( int i = 0; i < vertexCount; i++ )
			{
				vertices[ i ].x /= agk::GetViewZoom();
				vertices[ i ].y /= agk::GetViewZoom();
			}
		}
			
		if ( shape2->GetType() == b2Shape::e_circle )
		{
			shape2->m_radius /= agk::GetViewZoom();
		}
	}

	// quick reject
	float diffX = agk::WorldToPhyX(fX1 - fX2);
	float diffY = agk::WorldToPhyY(fY1 - fY2);
	float radiusSqr = fColRadius1*fColRadius1 + fColRadius2*fColRadius2 + 2*fColRadius1*fColRadius2;
	if ( diffX*diffX + diffY*diffY > radiusSqr ) 
	{
		delete shape1;
		delete shape2;
		return false;
	}

	b2Transform transform1; transform1.Set( b2Vec2(agk::WorldToPhyX(fX1),agk::WorldToPhyY(fY1)), m_fAngle );
	b2Transform transform2; transform2.Set( b2Vec2(agk::WorldToPhyX(fX2),agk::WorldToPhyY(fY2)), pSprite2->m_fAngle );

	// circle-circle
	if ( shape1->GetType() == b2Shape::e_circle && shape2->GetType() == b2Shape::e_circle )
	{
		b2Vec2 pA = b2Mul(transform1, ((b2CircleShape*)shape1)->m_p);
		b2Vec2 pB = b2Mul(transform2, ((b2CircleShape*)shape2)->m_p);

		b2Vec2 d = pB - pA;
		float32 distSqr = b2Dot(d, d);
		float32 rA = shape1->m_radius, rB = shape2->m_radius;
		float32 radius = rA + rB;
		delete shape1;
		delete shape2;
		return (distSqr < radius * radius);
	}

	// polygon-polygon
	if ( shape1->GetType() == b2Shape::e_polygon && shape2->GetType() == b2Shape::e_polygon )
	{
		b2Manifold manifold;
		b2CollidePolygons( &manifold, (b2PolygonShape*)shape1, transform1, (b2PolygonShape*)shape2, transform2 );
		delete shape1;
		delete shape2;
		return (manifold.pointCount > 0);
	}

	// must be polygon-circle
	b2PolygonShape *polygonA;
	b2CircleShape *circleB;
	if ( shape1->GetType() == b2Shape::e_polygon )
	{
		polygonA = (b2PolygonShape*)shape1;
		circleB = (b2CircleShape*)(shape2);

		transform1.Set( b2Vec2(agk::WorldToPhyX(fX1),agk::WorldToPhyY(fY1)), m_fAngle );
		transform2.Set( b2Vec2(agk::WorldToPhyX(fX2),agk::WorldToPhyY(fY2)), pSprite2->m_fAngle );
	}
	else
	{
		polygonA = (b2PolygonShape*)(shape2);
		circleB = (b2CircleShape*)shape1;

		transform1.Set( b2Vec2(agk::WorldToPhyX(fX2),agk::WorldToPhyY(fY2)), pSprite2->m_fAngle );
		transform2.Set( b2Vec2(agk::WorldToPhyX(fX1),agk::WorldToPhyY(fY1)), m_fAngle );
	}

	// Compute circle position in the frame of the polygon.
	b2Vec2 c = b2Mul(transform2, circleB->m_p);
	b2Vec2 cLocal = b2MulT(transform1, c);

	// Find the min separating edge.
	int32 normalIndex = 0;
	float32 separation = -b2_maxFloat;
	float32 radius = polygonA->m_radius + circleB->m_radius;
	int32 vertexCount = polygonA->m_vertexCount;
	const b2Vec2* vertices = polygonA->m_vertices;
	const b2Vec2* normals = polygonA->m_normals;

	for (int32 i = 0; i < vertexCount; ++i)
	{
		float32 s = b2Dot(normals[i], cLocal - vertices[i]);

		// Early out.
		if (s > radius) 
		{
			delete shape1;
			delete shape2;
			return false;
		}

		if (s > separation)
		{
			separation = s;
			normalIndex = i;
		}
	}

	// Vertices that subtend the incident face.
	int32 vertIndex1 = normalIndex;
	int32 vertIndex2 = vertIndex1 + 1 < vertexCount ? vertIndex1 + 1 : 0;
	b2Vec2 v1 = vertices[vertIndex1];
	b2Vec2 v2 = vertices[vertIndex2];

	// If the center is inside the polygon ...
	if (separation < b2_epsilon)
	{
		delete shape1;
		delete shape2;
		return true;
	}

	// Compute barycentric coordinates
	float32 u1 = b2Dot(cLocal - v1, v2 - v1);
	float32 u2 = b2Dot(cLocal - v2, v1 - v2);
	if (u1 <= 0.0f)
	{
		delete shape1;
		delete shape2;
		return (b2DistanceSquared(cLocal, v1) <= radius * radius);
	}
	else if (u2 <= 0.0f)
	{
		delete shape1;
		delete shape2;
		return (b2DistanceSquared(cLocal, v2) <= radius * radius);
	}
	else
	{
		b2Vec2 faceCenter = 0.5f * (v1 + v2);
		float32 separation = b2Dot(cLocal - faceCenter, normals[vertIndex1]);
		delete shape1;
		delete shape2;
		return (separation <= radius);
	}
}

float cSprite::GetDistance( cSprite *pSprite2 )
{
	if ( !m_phyShape ) 
	{
		// create a box for them
		SetShape( eBox );
	}

	if ( !pSprite2->m_phyShape ) 
	{
		pSprite2->SetShape( eBox );
	}

	b2DistanceInput input;
	input.proxyA.Set(m_phyShape,0);
	input.proxyB.Set(pSprite2->m_phyShape,0);
	input.transformA.Set( b2Vec2(agk::WorldToPhyX(m_fX),agk::WorldToPhyY(m_fY)), m_fAngle );
	input.transformB.Set( b2Vec2(agk::WorldToPhyX(pSprite2->m_fX),agk::WorldToPhyY(pSprite2->m_fY)), pSprite2->m_fAngle );
	input.useRadii = true;

	b2SimplexCache cache;
	cache.count = 0;

	b2Distance(m_colResult, &cache, &input);

	return agk::PhyToWorldX(m_colResult->distance);
}

float cSprite::GetDistancePoint1X( void )
{
	return agk::PhyToWorldX(m_colResult->pointA.x);
}

float cSprite::GetDistancePoint1Y( void )
{
	return agk::PhyToWorldY(m_colResult->pointA.y);
}

float cSprite::GetDistancePoint2X( void )
{
	return agk::PhyToWorldX(m_colResult->pointB.x);
}

float cSprite::GetDistancePoint2Y( void )
{
	return agk::PhyToWorldY(m_colResult->pointB.y);
}

bool cSprite::GetPhysicsCollision( cSprite *pSprite2 )
{
	m_lastContact = UNDEF;

	if ( !pSprite2 ) return false;

	if ( !m_phyBody ) return false;
	if ( !pSprite2->m_phyBody ) return false;
		
	b2ContactEdge *pContact = m_phyBody->GetContactList();
	while ( pContact )
	{
		if ( pContact->contact->IsTouching() && pContact->other == pSprite2->m_phyBody ) 
		{
			m_lastContact = pContact->contact;
			return true;
		}
		pContact = pContact->next;
	}

	return false;
}

float cSprite::GetPhysicsCollisionX( )
{
	if ( m_lastContact ) return GetXFromWorld( GetPhysicsCollisionWorldX(), GetPhysicsCollisionWorldY() );
	return 0;
}

float cSprite::GetPhysicsCollisionY( )
{
	if ( m_lastContact ) return GetYFromWorld( GetPhysicsCollisionWorldX(), GetPhysicsCollisionWorldY() );
	return 0;
}

float cSprite::GetPhysicsCollisionWorldX( )
{
	if ( m_lastContact ) 
	{
		b2WorldManifold manifold;
		m_lastContact->GetWorldManifold( &manifold );
		return agk::PhyToWorldX(manifold.points[0].x);
	}
	return 0;
}

float cSprite::GetPhysicsCollisionWorldY( )
{
	if ( m_lastContact ) 
	{
		b2WorldManifold manifold;
		m_lastContact->GetWorldManifold( &manifold );
		return agk::PhyToWorldY(manifold.points[0].y);
	}
	return 0;
}

void cSprite::SetShape( ePhysicsShape shape, int shapeID )
{
	if ( shape == eManual ) return;

	if ( shapeID > (int)m_iNumAdditionalShapes )
	{
		uString err; err.Format( "Cannot set the shape, shapeID %d does not exist", shapeID+1 );
		agk::Error( err );
		return;
	}

	// legacy functionality, shapeID of -1 replaces all shapes with just this one
	if ( shapeID < 0 ) 
	{
		ClearAdditionalShapes();
		shapeID = 0; // set the main shape
	}

	b2Shape **ppShapePtr = &m_phyShape;
	if ( shapeID > 0 ) ppShapePtr = m_phyAdditionalShapes + (shapeID-1);

	b2Shape *pOldShape = *ppShapePtr;
	*ppShapePtr = UNDEF;

	if ( shape == eNone ) 
	{
		ReplacePhysicsShape( pOldShape, 0 );
		return;
	}

	// can't create a polygon without an image.
	if ( (!m_pImage || m_pImage->GetTextureID() == 0) && shape == ePolygon ) shape = eBox;

	switch( shape )
	{
		case cSprite::ePolygon:
		{
			const Point2D *pPoints = UNDEF;
			if ( m_pImage )
			{
				if ( m_iFrameCount == 0 ) pPoints = m_pImage->GetBoundingPoints( );
				else pPoints = m_pImage->GetBoundingPoints( m_pFrames[m_iCurrentFrame].m_fU1, m_pFrames[m_iCurrentFrame].m_fV1, m_pFrames[m_iCurrentFrame].m_fU2, m_pFrames[m_iCurrentFrame].m_fV2 );
			}

			b2PolygonShape *polyShape = new b2PolygonShape();

			int count = Point2D::Count( pPoints );
			if ( count < 3 )
			{
				// not enough points for convex hull
				polyShape->SetAsBox( agk::WorldToPhyX(m_fWidth/2.0f), agk::WorldToPhyY(m_fHeight/2.0f), b2Vec2(agk::WorldToPhyX(m_fWidth/2.0f-m_fOffsetX),agk::WorldToPhyY(m_fHeight/2.0f-m_fOffsetY)), 0.0f );
			}
			else
			{
				b2Vec2 *vertices = new b2Vec2[ count ];

				int flipH = (m_bFlags & AGK_SPRITE_FLIPH) > 0 ? 1 : 0;
				int flipV = (m_bFlags & AGK_SPRITE_FLIPV) > 0 ? 1 : 0;
				int reverse = flipH ^ flipV;
				float maxDist = 0;

				int index = reverse ? count-1 : 0;
				const Point2D *pPoint = pPoints;
				while ( pPoint )
				{
					float x = pPoint->x;
					float y = pPoint->y;
					if ( m_iFrameCount > 0 )
					{
						x = x * m_fWidth / m_iFrameWidth;
						y = y * m_fHeight / m_iFrameHeight;
					}
					else
					{
						x = x * m_fWidth / m_pImage->GetWidth();
						y = y * m_fHeight / m_pImage->GetHeight();
					}

						
					x = x - m_fOffsetX;
					y = y - m_fOffsetY;

					x = agk::WorldToPhyX(x);
					y = agk::WorldToPhyY(y);

					float dist = x*x + y*y;
					if ( dist > maxDist ) maxDist = dist;

					if ( flipH ) x = -x;
					if ( flipV ) y = -y;

					vertices[ index ].Set( x, y );

					if ( reverse ) index--;
					else index++;
					pPoint = pPoint->pNext;
				}

				polyShape->Set( vertices, count ); 

				delete [] vertices;
			}
			*ppShapePtr = (b2Shape*) polyShape;
			break;
		}

		case cSprite::eBox:
		{
			b2PolygonShape *polyShape = new b2PolygonShape();
			polyShape->SetAsBox( agk::WorldToPhyX(m_fWidth/2.0f), agk::WorldToPhyY(m_fHeight/2.0f), b2Vec2(agk::WorldToPhyX(m_fWidth/2.0f-m_fOffsetX),agk::WorldToPhyY(m_fHeight/2.0f-m_fOffsetY)), 0.0f );
			*ppShapePtr = (b2Shape*) polyShape;
			break;
		}

		case cSprite::eCircle:
		{
			b2CircleShape *circleShape = new b2CircleShape();

			float maxDist = 0;
			if ( !m_pImage )
			{
				float maxX = m_fOffsetX;
				float maxY = m_fOffsetY;
				if ( m_fOffsetX < m_fWidth/2 ) maxX = m_fWidth - m_fOffsetX;
				if ( m_fOffsetY < m_fHeight/2 ) maxY = m_fHeight - m_fOffsetY;

				maxX = agk::WorldToPhyX(maxX);
				maxY = agk::WorldToPhyY(maxY);
				maxDist = maxX*maxX + maxY*maxY;
			}
			else
			{
				// get the convex hull points and work out which one is furthest from the offset point
				const Point2D *pPoints = UNDEF;
				if ( m_pImage )
				{
					if ( m_iFrameCount == 0 ) pPoints = m_pImage->GetBoundingPoints( );
					else pPoints = m_pImage->GetBoundingPoints( m_pFrames[m_iCurrentFrame].m_fU1, m_pFrames[m_iCurrentFrame].m_fV1, m_pFrames[m_iCurrentFrame].m_fU2, m_pFrames[m_iCurrentFrame].m_fV2 );
				}

				const Point2D *pPoint = pPoints;
				while ( pPoint )
				{
					float x = pPoint->x;
					float y = pPoint->y;

					if ( m_iFrameCount > 0 )
					{
						x = x * m_fOrigWidth / m_iFrameWidth;
						y = y * m_fOrigHeight / m_iFrameHeight;
					}
					else
					{
						x = x * m_fOrigWidth / m_pImage->GetWidth();
						y = y * m_fOrigHeight / m_pImage->GetHeight();
					}

					x = x - m_fOffsetX;
					y = y - m_fOffsetY;

					x = agk::WorldToPhyX(x);
					y = agk::WorldToPhyY(y);

					float dist = x*x + y*y;
					if ( dist > maxDist ) maxDist = dist;
					pPoint = pPoint->pNext;
				}
			}

			circleShape->m_radius = agk::Sqrt( maxDist );
			circleShape->m_p.Set(0,0);
			*ppShapePtr = (b2Shape*) circleShape;
			break;
		}

		default: 
		{
#ifdef _AGK_ERROR_CHECK
			agk::Error( "Unrecognised physics shape for sprite in SetSpriteShape()" );
#endif
			ReplacePhysicsShape( pOldShape, 0 );
			return;
		}
	}

	m_eShape = shape;
	ReplacePhysicsShape( pOldShape, *ppShapePtr );
	RecalcColRadius();
}

void cSprite::SetShapeBox( float x, float y, float x2, float y2, float angle, int shapeID )
{
	if ( shapeID > (int)m_iNumAdditionalShapes )
	{
		uString err; err.Format( "Cannot set the shape, shapeID %d does not exist", shapeID+1 );
		agk::Error( err );
		return;
	}

	// legacy functionality, shapeID of -1 replaces all shapes with just this one
	if ( shapeID < 0 ) 
	{
		ClearAdditionalShapes();
		shapeID = 0; // set the main shape
	}

	b2Shape **ppShapePtr = &m_phyShape;
	if ( shapeID > 0 ) ppShapePtr = m_phyAdditionalShapes + (shapeID-1);

	b2Shape *pOldShape = *ppShapePtr;
	*ppShapePtr = UNDEF;

	if ( x2 < x )
	{
		// swap
		float temp = x;
		x = x2; 
		x2 = temp;
	}

	if ( y2 < y )
	{
		// swap
		float temp = y;
		y = y2; 
		y2 = temp;
	}

	float width = x2 - x;
	float height = y2 - y;
	float posX = (x+x2)/2.0f;// - m_fOffsetX;
	float posY = (y+y2)/2.0f;// - m_fOffsetY;

	b2PolygonShape* polyShape = new b2PolygonShape();
	polyShape->SetAsBox( agk::WorldToPhyX(width/2.0f), agk::WorldToPhyY(height/2.0f), b2Vec2(agk::WorldToPhyX(posX),agk::WorldToPhyY(posY)), angle );
	*ppShapePtr = (b2Shape*) polyShape;
		
	m_eShape = eManual;
	ReplacePhysicsShape( pOldShape, *ppShapePtr );
	RecalcColRadius();
}

void cSprite::SetShapeCircle( float x, float y, float radius, int shapeID )
{
	if ( shapeID > (int)m_iNumAdditionalShapes )
	{
		uString err; err.Format( "Cannot set the shape, shapeID %d does not exist", shapeID+1 );
		agk::Error( err );
		return;
	}

	// legacy functionality, shapeID of -1 replaces all shapes with just this one
	if ( shapeID < 0 ) 
	{
		ClearAdditionalShapes();
		shapeID = 0; // set the main shape
	}

	b2Shape **ppShapePtr = &m_phyShape;
	if ( shapeID > 0 ) ppShapePtr = m_phyAdditionalShapes + (shapeID-1);

	b2Shape *pOldShape = *ppShapePtr;
	*ppShapePtr = UNDEF;

	b2CircleShape* circleShape = new b2CircleShape();
	circleShape->m_p.Set( agk::WorldToPhyX(x), agk::WorldToPhyY(y) );
	circleShape->m_radius = agk::WorldToPhyX(radius);
	*ppShapePtr = (b2Shape*) circleShape;
		
	m_eShape = eManual;
	ReplacePhysicsShape( pOldShape, *ppShapePtr );
	RecalcColRadius();
}

void cSprite::SetShapePolygon( UINT numPoints, float *pPoints, int shapeID )
{
	if ( !pPoints ) return;
	if ( numPoints < 2 ) return;
	if ( numPoints > b2_maxPolygonVertices ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Too many points for a physics polygon" );
#endif
		return;
	}

	if ( shapeID > (int)m_iNumAdditionalShapes )
	{
		uString err; err.Format( "Cannot set the shape, shapeID %d does not exist", shapeID+1 );
		agk::Error( err );
		return;
	}

	// legacy functionality, shapeID of -1 replaces all shapes with just this one
	if ( shapeID < 0 ) 
	{
		ClearAdditionalShapes();
		shapeID = 0; // set the main shape
	}

	b2Shape **ppShapePtr = &m_phyShape;
	if ( shapeID > 0 ) ppShapePtr = m_phyAdditionalShapes + (shapeID-1);

	b2Shape *pOldShape = *ppShapePtr;
	*ppShapePtr = UNDEF;

	b2Vec2 *vertices = new b2Vec2[ numPoints ];
	for ( UINT i = 0; i < numPoints; i++ )
	{
		vertices[ i ].x = agk::WorldToPhyX( pPoints[ i*2 ] );
		vertices[ i ].y = agk::WorldToPhyY( pPoints[ i*2 + 1 ] );
	}

	b2PolygonShape* polyShape = new b2PolygonShape();
	polyShape->Set( vertices, (int)numPoints );
	*ppShapePtr = (b2Shape*) polyShape;
			
	m_eShape = eManual;
	ReplacePhysicsShape( pOldShape, *ppShapePtr );
	RecalcColRadius();
}

void cSprite::SetShapePolygon( UINT numPoints, UINT index, float x, float y, int shapeID )
{
	if ( numPoints > b2_maxPolygonVertices )
	{
#ifdef _AGK_ERROR_CHECK
		uString errStr;
		errStr.Format( "Could not add polygon point, polygon shapes have a maximum of %d points", b2_maxPolygonVertices );
		agk::Error( errStr );
#endif
		return;
	}

	if ( m_iPolygonPointsNum < numPoints )
	{
		if ( m_fPolygonPointsTemp ) delete [] m_fPolygonPointsTemp;
		m_fPolygonPointsTemp = new float[ numPoints*2 ];
		for ( UINT i = 0; i < numPoints*2; i++ ) m_fPolygonPointsTemp[ i ] = 0;
		m_iPolygonPointsNum = numPoints;
	}

	if ( index >= numPoints )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( uString("Could not add point to polygon shape, index must be less than the number of points. index starts at 0.") );
#endif
		return;
	}

	m_fPolygonPointsTemp[ index*2 ] = x;
	m_fPolygonPointsTemp[ index*2 + 1 ] = y;

	if ( numPoints == index+1 ) 
	{
		SetShapePolygon( numPoints, m_fPolygonPointsTemp, shapeID );
		delete [] m_fPolygonPointsTemp;
		m_fPolygonPointsTemp = 0;
		m_iPolygonPointsNum = 0;
	}
}

void cSprite::SetShapeChain( UINT numPoints, float *pPoints, int loop, int shapeID )
{
	if ( !pPoints ) return;
	if ( numPoints < 2 ) return;
	
	if ( shapeID > (int)m_iNumAdditionalShapes )
	{
		uString err; err.Format( "Cannot set the shape, shapeID %d does not exist", shapeID+1 );
		agk::Error( err );
		return;
	}

	// legacy functionality, shapeID of -1 replaces all shapes with just this one
	if ( shapeID < 0 ) 
	{
		ClearAdditionalShapes();
		shapeID = 0; // set the main shape
	}

	b2Shape **ppShapePtr = &m_phyShape;
	if ( shapeID > 0 ) ppShapePtr = m_phyAdditionalShapes + (shapeID-1);

	b2Shape *pOldShape = *ppShapePtr;
	*ppShapePtr = UNDEF;

	b2Vec2 *vertices = new b2Vec2[ numPoints ];
	for ( UINT i = 0; i < numPoints; i++ )
	{
		vertices[ i ].x = agk::WorldToPhyX( pPoints[ i*2 ] );
		vertices[ i ].y = agk::WorldToPhyY( pPoints[ i*2 + 1 ] );
	}

	b2ChainShape* chainShape = new b2ChainShape();
	if ( loop == 1 ) chainShape->CreateLoop( vertices, (int)numPoints );
	else chainShape->CreateChain( vertices, (int)numPoints );
	*ppShapePtr = (b2Shape*) chainShape;
			
	m_eShape = eManual;
	ReplacePhysicsShape( pOldShape, *ppShapePtr );
	RecalcColRadius();
}

void cSprite::SetShapeChain( UINT numPoints, UINT index, int loop, float x, float y, int shapeID )
{
	if ( m_iPolygonPointsNum < numPoints )
	{
		if ( m_fPolygonPointsTemp ) delete [] m_fPolygonPointsTemp;
		m_fPolygonPointsTemp = new float[ numPoints*2 ];
		for ( UINT i = 0; i < numPoints*2; i++ ) m_fPolygonPointsTemp[ i ] = 0;
		m_iPolygonPointsNum = numPoints;
	}

	if ( index >= numPoints )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( uString("Could not add point to chain shape, index must be less than the number of points. index starts at 0.") );
#endif
		return;
	}

	m_fPolygonPointsTemp[ index*2 ] = x;
	m_fPolygonPointsTemp[ index*2 + 1 ] = y;

	if ( numPoints == index+1 ) 
	{
		SetShapeChain( numPoints, m_fPolygonPointsTemp, loop, shapeID );
		delete [] m_fPolygonPointsTemp;
		m_fPolygonPointsTemp = 0;
		m_iPolygonPointsNum = 0;
	}
}

void cSprite::AddShapeInternal( b2Shape* pShape )
{
	if ( m_iNumAdditionalShapes > 0 )
	{
		b2Shape **pNewShapes = new b2Shape*[ m_iNumAdditionalShapes+1 ];
		for( UINT i = 0; i < m_iNumAdditionalShapes; i++ ) pNewShapes[i] = m_phyAdditionalShapes[i];
		delete [] m_phyAdditionalShapes;
		m_phyAdditionalShapes = pNewShapes;
	}
	else
	{
		m_phyAdditionalShapes = new b2Shape*[1];
	}
	m_phyAdditionalShapes[ m_iNumAdditionalShapes ] = pShape;
	m_iNumAdditionalShapes++;
}

// add shapes
void cSprite::AddShapeBox( float x, float y, float x2, float y2, float angle )
{
	if ( x2 < x )
	{
		// swap
		float temp = x;
		x = x2; 
		x2 = temp;
	}

	if ( y2 < y )
	{
		// swap
		float temp = y;
		y = y2; 
		y2 = temp;
	}

	float width = x2 - x;
	float height = y2 - y;
	float posX = (x+x2)/2.0f;// - m_fOffsetX;
	float posY = (y+y2)/2.0f;// - m_fOffsetY;

	b2PolygonShape *polyShape = new b2PolygonShape();
	polyShape->SetAsBox( agk::WorldToPhyX(width/2.0f), agk::WorldToPhyY(height/2.0f), b2Vec2(agk::WorldToPhyX(posX),agk::WorldToPhyY(posY)), angle );

	AddShapeInternal( polyShape );
	ReplacePhysicsShape( 0, polyShape );
	RecalcColRadius();
}

void cSprite::AddShapeCircle( float x, float y, float radius )
{
	b2CircleShape *circleShape = new b2CircleShape();
	circleShape->m_p.Set( agk::WorldToPhyX(x), agk::WorldToPhyY(y) );
	circleShape->m_radius = agk::WorldToPhyX(radius);
		
	AddShapeInternal( circleShape );
	ReplacePhysicsShape( 0, circleShape );
	RecalcColRadius();
}

void cSprite::AddShapePolygon( UINT numPoints, float *pPoints )
{
	if ( !pPoints ) return;
	if ( numPoints < 2 ) return;
	if ( numPoints > b2_maxPolygonVertices ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Too many points for a physics polygon" );
#endif
		return;
	}

	b2Vec2 *vertices = new b2Vec2[ numPoints ];
	for ( UINT i = 0; i < numPoints; i++ )
	{
		vertices[ i ].x = agk::WorldToPhyX( pPoints[ i*2 ] );
		vertices[ i ].y = agk::WorldToPhyY( pPoints[ i*2 + 1 ] );
	}

	b2PolygonShape *polyShape = new b2PolygonShape();
	polyShape->Set( vertices, (int)numPoints );

	AddShapeInternal( polyShape );
	ReplacePhysicsShape( 0, polyShape );
	RecalcColRadius();
}

void cSprite::AddShapePolygon( UINT numPoints, UINT index, float x, float y )
{
	if ( numPoints > b2_maxPolygonVertices )
	{
#ifdef _AGK_ERROR_CHECK
		uString errStr;
		errStr.Format( "Could not add polygon point, polygon shapes have a maximum of %d points", b2_maxPolygonVertices );
		agk::Error( errStr );
#endif
		return;
	}

	if ( m_iPolygonPointsNum < numPoints )
	{
		if ( m_fPolygonPointsTemp ) delete [] m_fPolygonPointsTemp;
		m_fPolygonPointsTemp = new float[ numPoints*2 ];
		for ( UINT i = 0; i < numPoints*2; i++ ) m_fPolygonPointsTemp[ i ] = 0;
		m_iPolygonPointsNum = numPoints;
	}

	if ( index >= numPoints )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( uString("Could not add point to polygon shape, index must be less than the number of points. index starts at 0.") );
#endif
		return;
	}

	m_fPolygonPointsTemp[ index*2 ] = x;
	m_fPolygonPointsTemp[ index*2 + 1 ] = y;

	if ( numPoints == index+1 ) 
	{
		AddShapePolygon( numPoints, m_fPolygonPointsTemp );
		delete [] m_fPolygonPointsTemp;
		m_fPolygonPointsTemp = 0;
		m_iPolygonPointsNum = 0;
	}
}

void cSprite::AddShapeChain( UINT numPoints, float *pPoints, int loop )
{
	if ( !pPoints ) return;
	if ( numPoints < 2 ) return;
	
	b2Vec2 *vertices = new b2Vec2[ numPoints ];
	for ( UINT i = 0; i < numPoints; i++ )
	{
		vertices[ i ].x = agk::WorldToPhyX( pPoints[ i*2 ] );
		vertices[ i ].y = agk::WorldToPhyY( pPoints[ i*2 + 1 ] );
	}

	b2ChainShape *chainShape = new b2ChainShape();
	if ( loop == 1 ) chainShape->CreateLoop( vertices, (int)numPoints );
	else chainShape->CreateChain( vertices, (int)numPoints );

	AddShapeInternal( chainShape );
	ReplacePhysicsShape( 0, chainShape );
	RecalcColRadius();
}

void cSprite::AddShapeChain( UINT numPoints, UINT index, int loop, float x, float y )
{
	if ( m_iPolygonPointsNum < numPoints )
	{
		if ( m_fPolygonPointsTemp ) delete [] m_fPolygonPointsTemp;
		m_fPolygonPointsTemp = new float[ numPoints*2 ];
		for ( UINT i = 0; i < numPoints*2; i++ ) m_fPolygonPointsTemp[ i ] = 0;
		m_iPolygonPointsNum = numPoints;
	}

	if ( index >= numPoints )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( uString("Could not add point to chain shape, index must be less than the number of points. index starts at 0.") );
#endif
		return;
	}

	m_fPolygonPointsTemp[ index*2 ] = x;
	m_fPolygonPointsTemp[ index*2 + 1 ] = y;

	if ( numPoints == index+1 ) 
	{
		AddShapeChain( numPoints, m_fPolygonPointsTemp, loop );
		delete [] m_fPolygonPointsTemp;
		m_fPolygonPointsTemp = 0;
		m_iPolygonPointsNum = 0;
	}
}

void cSprite::ClearAdditionalShapes()
{
	if ( m_phyBody )
	{
		// destory additional fixtures
		b2Fixture* pFix = m_phyBody->GetFixtureList();
		b2Fixture *pNext = UNDEF;
		while ( pFix )
		{
			pNext = pFix->GetNext();

			if ( pFix->GetShape() != m_phyShape ) m_phyBody->DestroyFixture( pFix );
				
			pFix = pNext;
		}
	}

	for( UINT i = 0; i < m_iNumAdditionalShapes; i++ )
	{
		if( m_phyAdditionalShapes[i] ) delete m_phyAdditionalShapes[i];
	}
	if ( m_phyAdditionalShapes ) delete [] m_phyAdditionalShapes;
	m_iNumAdditionalShapes = 0;
	m_phyAdditionalShapes = 0;

	RecalcColRadius();
}

int cSprite::GetShapeNumVertices( int shapeID )
{
	if ( shapeID > (int)m_iNumAdditionalShapes )
	{
		uString err; err.Format( "Cannot get shape vertex, shapeID %d does not exist", shapeID+1 );
		agk::Error( err );
		return 0;
	}

	const b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	if ( pShape == 0 ) return 0;
	if ( pShape->GetType() == b2Shape::e_polygon )
	{
		const b2PolygonShape *pPoly = (b2PolygonShape*)pShape;
		return pPoly->GetVertexCount();
	}
	else if ( pShape->GetType() == b2Shape::e_chain ) 
	{
		const b2ChainShape *pChain = (b2ChainShape*)pShape;
		return pChain->GetVertexCount();
	}
	else return 0;
}

float cSprite::GetShapeVertexX( int shapeID, int vertex )
{
	if ( shapeID > (int)m_iNumAdditionalShapes )
	{
		uString err; err.Format( "Cannot get shape vertex, shapeID %d does not exist", shapeID+1 );
		agk::Error( err );
		return 0;
	}

	const b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	if ( pShape == 0 ) return 0;
	if ( pShape->GetType() == b2Shape::e_polygon )
	{
		const b2PolygonShape *pPoly = (b2PolygonShape*)pShape;
		if ( vertex < 0 || vertex >= pPoly->GetVertexCount() )
		{
			uString err; err.Format( "Cannot get shape vertex, vertex %d does not exist", vertex+1 );
			agk::Error( err );
			return 0;
		}
		return agk::PhyToWorldX( pPoly->GetVertex( vertex ).x );
	}
	else if ( pShape->GetType() == b2Shape::e_chain ) 
	{
		const b2ChainShape *pChain = (b2ChainShape*)pShape;
		if ( vertex < 0 || vertex >= pChain->GetVertexCount() )
		{
			uString err; err.Format( "Cannot get shape vertex, vertex %d does not exist", vertex+1 );
			agk::Error( err );
			return 0;
		}
		return agk::PhyToWorldX( pChain->GetVertex( vertex ).x );
	}
	else return 0;	
}

float cSprite::GetShapeVertexY( int shapeID, int vertex )
{
	if ( shapeID > (int)m_iNumAdditionalShapes )
	{
		uString err; err.Format( "Cannot get shape vertex, shapeID %d does not exist", shapeID+1 );
		agk::Error( err );
		return 0;
	}

	const b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	if ( pShape == 0 ) return 0;
	if ( pShape->GetType() == b2Shape::e_polygon )
	{
		const b2PolygonShape *pPoly = (b2PolygonShape*)pShape;
		if ( vertex < 0 || vertex >= pPoly->GetVertexCount() )
		{
			uString err; err.Format( "Cannot get shape vertex, vertex %d does not exist", vertex+1 );
			agk::Error( err );
			return 0;
		}
		return agk::PhyToWorldY( pPoly->GetVertex( vertex ).y );
	}
	else if ( pShape->GetType() == b2Shape::e_chain ) 
	{
		const b2ChainShape *pChain = (b2ChainShape*)pShape;
		if ( vertex < 0 || vertex >= pChain->GetVertexCount() )
		{
			uString err; err.Format( "Cannot get shape vertex, vertex %d does not exist", vertex+1 );
			agk::Error( err );
			return 0;
		}
		return agk::PhyToWorldY( pChain->GetVertex( vertex ).y );
	}
	else return 0;	
}

// physics setters
void cSprite::SetPhysicsOn( ePhysicsMode mode )
{
	if ( m_phyBody ) 
	{
		m_phyBody->SetActive( true );
		return;
	}
		
	if ( m_phyShape == UNDEF ) 
	{
		if ( m_eShape == eNone || m_eShape == eManual ) SetShape( eBox );
		else SetShape( m_eShape );
	}

	b2BodyDef bodyDef;
	switch( mode )
	{
		case cSprite::eDynamic: bodyDef.type = b2_dynamicBody; break;
		case cSprite::eStatic: bodyDef.type = b2_staticBody; break;
		case cSprite::eKinematic: bodyDef.type = b2_kinematicBody; break;
#ifdef _AGK_ERROR_CHECK
		default: agk::Error( "Unrecognised physics mode for sprite in SetPhysicsOn()" );
#endif
		return;
	}

	bodyDef.position.Set( agk::WorldToPhyX(m_fX), agk::WorldToPhyY(m_fY) );
	bodyDef.angle = m_fAngle;
	bodyDef.userData = (void*) this;
	//bodyDef.linearVelocity.x = 10.0f;
	//bodyDef.angularVelocity = 4.0f;
	m_phyBody = agk::m_phyWorld->CreateBody( &bodyDef );
		
	if ( m_phyShape )
	{
		b2FixtureDef fixtureDef;
		fixtureDef.shape = m_phyShape;
		fixtureDef.localShapeMemory = false;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.1f;
		fixtureDef.isSensor = 0;
		fixtureDef.filter.categoryBits = m_iCategories & 0x0000ffff;
		fixtureDef.filter.maskBits = m_iCategoryMask & 0x0000ffff;
		fixtureDef.filter.groupIndex = m_iGroup;
		m_phyBody->CreateFixture( &fixtureDef );

		for( UINT i = 0; i < m_iNumAdditionalShapes; i++ )
		{
			fixtureDef.shape = m_phyAdditionalShapes[i];
			m_phyBody->CreateFixture( &fixtureDef );
		}
	}

	b2MassData mass;
	mass.center.Set( 0,0 );
	mass.mass = m_phyBody->GetMass();
	mass.I = m_phyBody->GetInertia();
	m_phyBody->SetMassData( &mass );
}

void cSprite::PrepareToClearPhysicsContacts()
{
	agk::PrepareToDeleteSpriteContacts( this );

	// check if any other sprites are iterating on these contacts
	b2ContactEdge* ce = m_phyBody->m_contactList;
	while (ce)
	{
		cSprite* pOther = (cSprite*) ce->other->GetUserData();
		if ( pOther )
		{
			while ( pOther->m_pContactIter && pOther->m_pContactIter->other == m_phyBody ) 
			{
				// sprite is iterating and this contact is going to get deleted so move it along
				pOther->m_pContactIter = pOther->m_pContactIter->next;
			}
		}
		ce = ce->next;
	}

	if ( m_pContactIter ) m_pContactIter = 0;
}

void cSprite::SetPhysicsOff( )
{
	if ( !m_phyBody ) return;

	PrepareToClearPhysicsContacts();
	m_phyBody->SetActive( false );
}

void cSprite::SetPhysicsDelete( )
{
	if ( !m_phyBody ) return;
		
	PrepareToClearPhysicsContacts();
	agk::m_phyWorld->DestroyBody( m_phyBody );
	m_phyBody = UNDEF;
}

void cSprite::SetPhysicsFriction( float friction, int shapeID )
{
	if ( !m_phyBody ) return;
	if ( shapeID > (int)m_iNumAdditionalShapes ) 
	{
		uString err; err.Format( "Failed to set shape friction, shape %d does not exist", shapeID+1 );
		return;
	}

	b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];
		
	b2Fixture *pFix = m_phyBody->GetFixtureList();
	while ( pFix )
	{
		if ( shapeID < 0 || pFix->GetShape() == pShape ) 
		{
			pFix->SetFriction( friction );
			if ( shapeID >= 0 ) break;
		}
		pFix = pFix->GetNext();
	}

	for ( b2ContactEdge* contactEdge = m_phyBody->GetContactList(); contactEdge; contactEdge = contactEdge->next )
	{
		if ( shapeID < 0 
		  || contactEdge->contact->GetFixtureA() == pFix 
	      || contactEdge->contact->GetFixtureB() == pFix ) 
		{
			contactEdge->contact->ResetFriction();
		}
	}
}

void cSprite::SetPhysicsRestitution( float restitution, int shapeID )
{
	if ( !m_phyBody ) return;
	if ( shapeID > (int)m_iNumAdditionalShapes ) 
	{
		uString err; err.Format( "Failed to set shape restitution, shape %d does not exist", shapeID+1 );
		return;
	}

	b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	b2Fixture *pFix = m_phyBody->GetFixtureList();
	while ( pFix )
	{
		if ( shapeID < 0 || pFix->GetShape() == pShape ) 
		{
			pFix->SetRestitution( restitution );
			if ( shapeID >= 0 ) break;
		}
		pFix = pFix->GetNext();
	}

	for ( b2ContactEdge* contactEdge = m_phyBody->GetContactList(); contactEdge; contactEdge = contactEdge->next )
	{
		if ( shapeID < 0 
		  || contactEdge->contact->GetFixtureA() == pFix 
	      || contactEdge->contact->GetFixtureB() == pFix ) 
		{
			contactEdge->contact->ResetRestitution();
		}
	}
}

void cSprite::SetPhysicsDensity( float density, int shapeID )
{
	if ( !m_phyBody ) return;
	if ( shapeID > (int)m_iNumAdditionalShapes ) 
	{
		uString err; err.Format( "Failed to set shape density, shape %d does not exist", shapeID+1 );
		return;
	}

	b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	b2Fixture *pFix = m_phyBody->GetFixtureList();
	while ( pFix )
	{
		if ( shapeID < 0 || pFix->GetShape() == pShape ) 
		{
			pFix->SetDensity( density );
			if ( shapeID >= 0 ) break;
		}
		pFix = pFix->GetNext();
	}

	m_phyBody->ResetMassData();
}

void cSprite::SetPhysicsCanRotate( bool rotate )
{
	if ( !m_phyBody ) return;

	// command names indicate negative of each other
	m_phyBody->SetFixedRotation( !rotate );
}

void cSprite::SetPhysicsVelocity( float vx, float vy )
{
	if ( !m_phyBody ) return;

	m_phyBody->SetLinearVelocity( b2Vec2( agk::WorldToPhyX(vx), agk::WorldToPhyY(vy) ) );
}

void cSprite::SetPhysicsAngularVelocity( float va )
{
	if ( !m_phyBody ) return;

	m_phyBody->SetAngularVelocity( va );
}

void cSprite::SetPhysicsDamping( float damp )
{
	if ( !m_phyBody ) return;

	m_phyBody->SetLinearDamping( damp );
}

void cSprite::SetPhysicsAngularDamping( float damp )
{
	if ( !m_phyBody ) return;

	m_phyBody->SetAngularDamping( damp );
}

void cSprite::SetPhysicsIsBullet( bool bullet )
{
	if ( !m_phyBody ) return;

	m_phyBody->SetBullet( bullet );
}

void cSprite::SetPhysicsMass( float mass )
{
	if ( !m_phyBody ) return;

	b2MassData massData;
	m_phyBody->GetMassData( &massData );
	massData.mass = mass;
	m_phyBody->SetMassData( &massData );
}

void cSprite::SetPhysicsIsSensor( bool sensor, int shapeID )
{
	if ( !m_phyBody ) return;
	if ( shapeID > (int)m_iNumAdditionalShapes ) 
	{
		uString err; err.Format( "Failed to set shape sensor, shape %d does not exist", shapeID+1 );
		return;
	}

	b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	b2Fixture *pFix = m_phyBody->GetFixtureList();
	while ( pFix )
	{
		if ( shapeID < 0 || pFix->GetShape() == pShape ) pFix->SetSensor( sensor );
		pFix = pFix->GetNext();
	}
}

float cSprite::GetPhysicsVelocityX()
{
	if ( !m_phyBody ) return 0;

	return agk::PhyToWorldX( m_phyBody->GetLinearVelocity().x );
}

float cSprite::GetPhysicsVelocityY()
{
	if ( !m_phyBody ) return 0;

	return agk::PhyToWorldY( m_phyBody->GetLinearVelocity().y );
}

float cSprite::GetPhysicsAngularVelocity()
{
	if ( !m_phyBody ) return 0;

	return m_phyBody->GetAngularVelocity();
}

float cSprite::GetPhysicsMass( )
{
	if ( !m_phyBody ) return 0;

	b2MassData massData;
	m_phyBody->GetMassData( &massData );
	return massData.mass;
}

void cSprite::SetGroup( int group, int shapeID )
{
	if ( shapeID <= 0 ) m_iGroup = group;

	if ( !m_phyBody ) return;
	if ( shapeID > (int)m_iNumAdditionalShapes ) 
	{
		uString err; err.Format( "Failed to set shape group, shape %d does not exist", shapeID+1 );
		return;
	}

	b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	if ( group < -32767 ) group = 0;
	if ( group > 32767 ) group = 0;

	b2Fixture *pFix = m_phyBody->GetFixtureList();
	while ( pFix )
	{
		if ( shapeID < 0 || pFix->GetShape() == pShape )
		{
			const b2Filter pFilter = pFix->GetFilterData();
			
			b2Filter pNewFilter;
			pNewFilter.categoryBits = pFilter.categoryBits;
			pNewFilter.maskBits = pFilter.maskBits;
			pNewFilter.groupIndex = group;
			pFix->SetFilterData( pNewFilter );
		}

		pFix = pFix->GetNext();
	}
}

void cSprite::SetCategoryBits( UINT categories, int shapeID )
{
	if ( shapeID <= 0 ) m_iCategories = categories;

	if ( !m_phyBody ) return;
	if ( shapeID > (int)m_iNumAdditionalShapes ) 
	{
		uString err; err.Format( "Failed to set shape category bits, shape %d does not exist", shapeID+1 );
		return;
	}

	b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	b2Fixture *pFix = m_phyBody->GetFixtureList();
	while ( pFix )
	{
		if ( shapeID < 0 || pFix->GetShape() == pShape )
		{
			const b2Filter pFilter = pFix->GetFilterData();
			
			b2Filter pNewFilter;
			pNewFilter.categoryBits = categories & 0x0000ffff;
			pNewFilter.maskBits = pFilter.maskBits;
			pNewFilter.groupIndex = pFilter.groupIndex;
			pFix->SetFilterData( pNewFilter );
		}

		pFix = pFix->GetNext();
	}
}

void cSprite::SetCategoryBit( UINT category, int flag, int shapeID )
{
	if ( category == 0 ) return;
	if ( category > 16 ) return;

	int mask = 1 << (category-1);
	if ( shapeID <= 0 ) 
	{
		if ( flag ) m_iCategories |= mask;
		else m_iCategories &= ~mask;
	}
		
	if ( !m_phyBody ) return;
	if ( shapeID > (int)m_iNumAdditionalShapes ) 
	{
		uString err; err.Format( "Failed to set shape category bit, shape %d does not exist", shapeID+1 );
		return;
	}

	b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	b2Fixture *pFix = m_phyBody->GetFixtureList();
	while ( pFix )
	{
		if ( shapeID < 0 || pFix->GetShape() == pShape )
		{
			const b2Filter pFilter = pFix->GetFilterData();
			b2Filter pNewFilter;

			if ( flag ) pNewFilter.categoryBits = pFilter.categoryBits | mask;
			else pNewFilter.categoryBits = pFilter.categoryBits & ~mask;
			
			pNewFilter.maskBits = pFilter.maskBits;
			pNewFilter.groupIndex = pFilter.groupIndex;
			pFix->SetFilterData( pNewFilter );
		}

		pFix = pFix->GetNext();
	}
}

void cSprite::SetCollideBits( UINT mask, int shapeID )
{
	if ( shapeID <= 0 ) m_iCategoryMask = mask;

	if ( !m_phyBody ) return;
	if ( shapeID > (int)m_iNumAdditionalShapes ) 
	{
		uString err; err.Format( "Failed to set shape collide bits, shape %d does not exist", shapeID+1 );
		return;
	}

	b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	b2Fixture *pFix = m_phyBody->GetFixtureList();
	while ( pFix )
	{
		if ( shapeID < 0 || pFix->GetShape() == pShape )
		{
			const b2Filter pFilter = pFix->GetFilterData();
			
			b2Filter pNewFilter;
			pNewFilter.categoryBits = pFilter.categoryBits;
			pNewFilter.maskBits = mask & 0x0000ffff;
			pNewFilter.groupIndex = pFilter.groupIndex;
			pFix->SetFilterData( pNewFilter );
		}

		pFix = pFix->GetNext();
	}
}

void cSprite::SetCollideBit( UINT category, int flag, int shapeID )
{
	if ( category == 0 ) return;
	if ( category > 16 ) return;

	int mask = 1 << (category-1);
	if ( shapeID <= 0 ) 
	{
		if ( flag ) m_iCategoryMask |= mask;
		else m_iCategoryMask &= ~mask;
	}

	if ( !m_phyBody ) return;
	if ( shapeID > (int)m_iNumAdditionalShapes ) 
	{
		uString err; err.Format( "Failed to set shape collide bit, shape %d does not exist", shapeID+1 );
		return;
	}

	b2Shape *pShape = m_phyShape;
	if ( shapeID > 0 ) pShape = m_phyAdditionalShapes[ shapeID-1 ];

	b2Fixture *pFix = m_phyBody->GetFixtureList();
	while ( pFix )
	{
		if ( shapeID < 0 || pFix->GetShape() == pShape )
		{
			const b2Filter pFilter = pFix->GetFilterData();
			b2Filter pNewFilter;
			
			if ( flag ) pNewFilter.maskBits = pFilter.maskBits | mask;
			else pNewFilter.maskBits = pFilter.maskBits & ~mask;

			pNewFilter.categoryBits = pFilter.categoryBits;
			pNewFilter.groupIndex = pFilter.groupIndex;
			pFix->SetFilterData( pNewFilter );
		}

		pFix = pFix->GetNext();
	}
}

void cSprite::SetPhysicsForce( float x, float y, float vx, float vy )
{
	if ( !m_phyBody ) return;

	x = agk::WorldToPhyX( x );
	y = agk::WorldToPhyY( y );
	vx = agk::WorldToPhyX( vx );
	vy = agk::WorldToPhyY( vy );

	m_phyBody->ApplyForce( b2Vec2(vx,vy), b2Vec2(x,y), true );
}

void cSprite::SetPhysicsTorque( float a )
{
	if ( !m_phyBody ) return;

	m_phyBody->ApplyTorque( a, true );
}

void cSprite::SetPhysicsLinearImpulse( float x, float y, float vx, float vy )
{
	if ( !m_phyBody ) return;

	x = agk::WorldToPhyX( x );
	y = agk::WorldToPhyY( y );
	vx = agk::WorldToPhyX( vx );
	vy = agk::WorldToPhyY( vy );

	m_phyBody->ApplyLinearImpulse( b2Vec2(vx,vy), b2Vec2(x,y), true );
}

void cSprite::SetPhysicsAngularImpulse( float a )
{
	if ( !m_phyBody ) return;

	m_phyBody->ApplyAngularImpulse( a, true );
}

int cSprite::GetFirstContact()
{
	if ( !m_phyBody ) return 0;
	m_pContactIter = m_phyBody->GetContactList();
	while ( m_pContactIter && !m_pContactIter->contact->IsTouching() ) m_pContactIter = m_pContactIter->next;
	return m_pContactIter ? 1 : 0;
}

int cSprite::GetNextContact()
{
	if ( !m_pContactIter ) return 0;
	m_pContactIter = m_pContactIter->next;
	while ( m_pContactIter && !m_pContactIter->contact->IsTouching() ) m_pContactIter = m_pContactIter->next;
	return m_pContactIter ? 1 : 0;
}

float cSprite::GetContactWorldX()
{
	if ( !m_pContactIter ) return 0;
	b2WorldManifold manifold;
	m_pContactIter->contact->GetWorldManifold(&manifold);
	return agk::PhyToWorldX(manifold.points[0].x);
}

float cSprite::GetContactWorldY()
{
	if ( !m_pContactIter ) return 0;
	b2WorldManifold manifold;
	m_pContactIter->contact->GetWorldManifold(&manifold);
	return agk::PhyToWorldY(manifold.points[0].y);
}

cSprite* cSprite::GetContactSprite2()
{
	if ( !m_pContactIter ) return UNDEF;
	cSprite *pSprite = (cSprite*) m_pContactIter->contact->GetFixtureA()->GetBody()->GetUserData();
	if ( pSprite == this ) pSprite = (cSprite*) m_pContactIter->contact->GetFixtureB()->GetBody()->GetUserData();
	return pSprite;
}

void cSprite::UpdatePhysics()
{
	m_lastContact = UNDEF;

	if ( !m_phyBody ) return;

	b2Vec2 position = m_phyBody->GetPosition();
	float32 angle = m_phyBody->GetAngle();

	m_fX = agk::PhyToWorldX(position.x);
	m_fY = agk::PhyToWorldY(position.y);
	m_fAngle = angle;
}
