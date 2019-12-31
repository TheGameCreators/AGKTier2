#include "agk.h"
#include <include/assimp/cimport.h>
#include <include/assimp/Importer.hpp>
#include <include/assimp/Exporter.hpp>
#include <include/assimp/postprocess.h>
#include <include/assimp/scene.h>
#include <include/assimp/cfileio.h>
//#include <include/assimp/ai_assert.h>
//#include <include/assimp/cfileio.h>
//#include <include/assimp/IOSystem.hpp>
//#include <include/assimp/IOStream.hpp>
//#include <include/assimp/LogStream.hpp>
//#include <include/assimp/DefaultLogger.hpp>

//#include "btBulletDynamicsCommon.h"
#include <deque>


using namespace AGK;
using namespace std;

cObject3D* cObject3D::g_pAllObjects = 0;

void cObject3D::ReloadAll()
{
	// delete
	cObject3D *pObject = g_pAllObjects;
	while( pObject )
	{
		if ( pObject->m_pMeshes )
		{
			for ( UINT i = 0; i < pObject->m_iNumMeshes; i++ )
			{
				pObject->m_pMeshes[ i ]->DeleteGLData();
			}
		}

		pObject = pObject->m_pNextObject;
	}

	// recreate, do not merge these loops as they do not interact correctly
	pObject = g_pAllObjects;
	while( pObject )
	{
		if ( pObject->m_pMeshes )
		{
			for ( UINT i = 0; i < pObject->m_iNumMeshes; i++ )
			{
				pObject->m_pMeshes[ i ]->ReloadGLData();
			}
		}

		pObject = pObject->m_pNextObject;
	}
}

cObject3D::cObject3D() : m_cShaderVariables(32)
{
	// animation
	m_pSkeleton = 0;
	m_iNumAnims = 0;
	m_pAnims = 0;
	m_iNumChildren = 0;
	m_iLoadedChildren = 0;

	// physics
	m_pRigidBody = 0;

	m_iID = 0;

	m_pSharedColObject = 0;

	m_pMeshes = 0;
	m_iNumMeshes = 0;

	m_Color.Set( 1,1,1 );
	m_fAlpha = 1;
	m_ColorEmissive.Set( 0,0,0 );
	m_iTransparency = 0;
	m_iBlendModes = 0x01; // src = AGK_BLEND_ONE, dst = AGK_BLEND_ZERO
	m_iZFunc = AGK_DEPTH_LESS;
	m_iCullMode = 1;
	m_fZBias = 0;
	m_fDepthNear = 0;
	m_fDepthFar = 1;

	m_iObjFlags = AGK_OBJECT_USE_LIGHTS | AGK_OBJECT_Z_WRITE | AGK_OBJECT_VISIBLE | AGK_OBJECT_COLLISION_ON | AGK_OBJECT_USE_FOG | AGK_OBJECT_RECV_SHADOWS;

	m_pColObject = 0;

	m_pHeightMap = 0;
	m_iHeightMapPixelsX = 0;
	m_iHeightMapPixelsZ = 0;
	m_fHeightMapSizeX = 0;
	m_fHeightMapSizeY = 0;
	m_fHeightMapSizeZ = 0;

	// add to list of all objects
	if ( g_pAllObjects ) g_pAllObjects->m_pPrevObject = this;
	m_pNextObject = g_pAllObjects;
	m_pPrevObject = 0;
	g_pAllObjects = this;
}

cObject3D::cObject3D( cObject3D *pOther, int share ) : m_cShaderVariables(32)
{
	// animation
	m_iNumAnims = pOther->m_iNumAnims;
	m_pAnims = 0;
	if ( pOther->m_iNumAnims > 0 ) m_pAnims = new Animation3D*[ m_iNumAnims ];
	if ( share == 1 )
	{
		for ( UINT i = 0; i < m_iNumAnims; i++ )
		{
			m_pAnims[ i ] = pOther->m_pAnims[ i ];
			m_pAnims[ i ]->AddRef();
		}
	}
	else
	{
		for ( UINT i = 0; i < m_iNumAnims; i++ )
		{
			m_pAnims[ i ] = new Animation3D( pOther->m_pAnims[ i ] );
		}
	}

	// never share a skeleton, always copy. Shouldn't take much space and makes things easier
	m_pSkeleton = 0;
	if ( pOther->m_pSkeleton ) 
	{
		m_pSkeleton = new Skeleton3D( pOther->m_pSkeleton );
		m_pSkeleton->SetRoot( this );
	}
	
	// don't copy child objects
	m_iNumChildren = 0;
	m_iLoadedChildren = 0;

	// physics
	m_pRigidBody = 0;

	m_iID = 0;

	m_iNumMeshes = 0;
	m_pMeshes = 0;
	if ( pOther->m_iNumMeshes > 0 )
	{
		m_iNumMeshes = pOther->m_iNumMeshes;
		m_pMeshes = new cMesh*[ m_iNumMeshes ];
		for( UINT i = 0; i < m_iNumMeshes; i++ )
		{
			m_pMeshes[ i ] = new cMesh( this, pOther->m_pMeshes[i], share );
		}
	}

	m_Color = pOther->m_Color;
	m_fAlpha = pOther->m_fAlpha;
	m_ColorEmissive = pOther->m_ColorEmissive;
	m_iTransparency = pOther->m_iTransparency;
	m_iBlendModes = pOther->m_iBlendModes;
	m_iZFunc = pOther->m_iZFunc;
	m_iCullMode = pOther->m_iCullMode;
	m_fZBias = pOther->m_fZBias;
	m_fDepthNear = pOther->m_fDepthNear;
	m_fDepthFar = pOther->m_fDepthFar;

	m_iObjFlags = pOther->m_iObjFlags;

	SetNodeRotation( pOther->rot().w, pOther->rot().x, pOther->rot().y, pOther->rot().z );
	SetNodePosition( pOther->pos().x, pOther->pos().y, pOther->pos().z );
	SetNodeScale( pOther->scale().x, pOther->scale().y, pOther->scale().z );

	m_pColObject = 0;

	m_pHeightMap = 0;
	m_iHeightMapPixelsX = 0;
	m_iHeightMapPixelsZ = 0;
	m_fHeightMapSizeX = 0;
	m_fHeightMapSizeY = 0;
	m_fHeightMapSizeZ = 0;

	if ( pOther->m_pHeightMap )
	{
		m_iHeightMapPixelsX = pOther->m_iHeightMapPixelsX;
		m_iHeightMapPixelsZ = pOther->m_iHeightMapPixelsZ;
		m_fHeightMapSizeX = pOther->m_fHeightMapSizeX;
		m_fHeightMapSizeY = pOther->m_fHeightMapSizeY;
		m_fHeightMapSizeZ = pOther->m_fHeightMapSizeZ;

		m_pHeightMap = new unsigned short[ m_iHeightMapPixelsX*m_iHeightMapPixelsZ ];

		for ( int i = 0; i < m_iHeightMapPixelsX*m_iHeightMapPixelsZ; i++ )
		{
			m_pHeightMap[i] = pOther->m_pHeightMap[i];
		}
	}

	// add to list of all objects
	if ( g_pAllObjects ) g_pAllObjects->m_pPrevObject = this;
	m_pNextObject = g_pAllObjects;
	m_pPrevObject = 0;
	g_pAllObjects = this;

	m_pSharedColObject = 0;
	if ( share != 1 ) CreateCollisionData();
	else m_pSharedColObject = pOther;
}

cObject3D::~cObject3D()
{
	// remove from any tweens
	TweenInstance::DeleteTarget( this );

	// remove from list of all objects
	if ( m_pPrevObject ) m_pPrevObject->m_pNextObject = m_pNextObject;
	else g_pAllObjects = m_pNextObject;
	if ( m_pNextObject ) m_pNextObject->m_pPrevObject = m_pPrevObject;

	if ( m_pColObject ) delete m_pColObject;

	if ( m_pHeightMap ) delete [] m_pHeightMap;

	DeleteMeshes();
	DeleteAnimations();

	if ( m_pSkeleton ) delete m_pSkeleton;
}

void cObject3D::CreateCollisionData()
{
	if ( m_pColObject ) delete m_pColObject;
	m_pColObject = 0;

	if ( (m_iObjFlags & AGK_OBJECT_COLLISION_ON) == 0 ) return;
	if ( m_pSharedColObject ) return;

	Face *pAllFaces = 0;
	Face *pLast = 0;
	for( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		// don't include boned meshes in the collision data, use the bones for collision instead
		if ( m_pMeshes[ i ]->HasBones() ) continue;
		if (!m_pMeshes[i]->GetCollision()) {
			continue; //PE: Disable mesh collision.
		}

		Face *pMeshFaces = m_pMeshes[ i ]->GetFaceList( &pLast );
		if ( pMeshFaces && pLast ) 
		{
			// append mesh faces to cumulative list
			pLast->nextFace = pAllFaces;
			pAllFaces = pMeshFaces;
		}
	}

	if ( !pAllFaces ) return;
	
	// the collision object keeps the face list, so don't delete it
	CollisionTree* colTree = new CollisionTree();
	colTree->setFacesPerNode( m_pHeightMap ? 50 : 8 ); // if object is a terrain then 2 polygons per node uses excessive memory, do 50 instead, in fact drop everything to a maximum of 8
    colTree->makeCollisionObject( pAllFaces );
    m_pColObject = (CollisionObject*)colTree;
}

void cObject3D::DeleteMeshes()
{
	if ( m_iNumMeshes > 0 && m_pMeshes )
	{
		for( UINT i = 0; i < m_iNumMeshes; i++ )
		{
			delete m_pMeshes[ i ];
		}
		delete [] m_pMeshes;
	}

	m_pMeshes = 0;
	m_iNumMeshes = 0;
}

void cObject3D::DeleteTree()
{
	if ( m_iNumChildren > 0 && m_iLoadedChildren )
	{
		for( UINT i = 0; i < m_iNumChildren; i++ )
		{
			if ( m_iLoadedChildren[i] != 0 ) agk::DeleteObjectTree( m_iLoadedChildren[i] );
		}
		delete [] m_iLoadedChildren;
	}

	m_iLoadedChildren = 0;
	m_iNumChildren = 0;

	for( UINT i = 0; i < m_vChildren.size(); i++ )
	{
		if ( m_vChildren[ i ]->GetNodeType() == eObject )
		{
			cObject3D *pObject = (cObject3D*)(m_vChildren[ i ]);
			pObject->DeleteTree();
			pObject->RemoveFromDeletingParent();
			if ( pObject->m_iID ) agk::DeleteObject( pObject->m_iID );
			else delete pObject;
		}
		else
		{
			m_vChildren[ i ]->RemoveFromDeletingParent();
		}
	}

	m_vChildren.clear();

	if ( m_pSkeleton )
	{
		for ( int i = 0; i < m_pSkeleton->GetBoneCount(); i++ )
		{
			Bone3D *pBone = m_pSkeleton->GetBone( i );
			if ( pBone ) pBone->DeleteAttachedObjects();
		}
	}
}

void cObject3D::DeleteChildren()
{
	if ( m_iNumChildren > 0 && m_iLoadedChildren )
	{
		for( UINT i = 0; i < m_iNumChildren; i++ )
		{
			if ( m_iLoadedChildren[i] != 0 ) agk::DeleteObject( m_iLoadedChildren[i] );
		}
		delete [] m_iLoadedChildren;
	}

	m_iLoadedChildren = 0;
	m_iNumChildren = 0;
}

void cObject3D::DeleteAnimations()
{
	if ( m_iNumAnims > 0 && m_pAnims )
	{
		for( UINT i = 0; i < m_iNumAnims; i++ )
		{
			//delete m_pAnims[ i ];
			m_pAnims[ i ]->RemoveRef(); // can be used by multiple objects, it will delete itself when there are none left
		}
		delete [] m_pAnims;
	}

	m_pAnims = 0;
	m_iNumAnims = 0;
}

void cObject3D::CreateBox( float width, float height, float length )
{
	DeleteMeshes();

	m_iNumMeshes = 1;
	m_pMeshes = new cMesh*[ 1 ];
	m_pMeshes[ 0 ] = new cMesh( this );
	m_pMeshes[ 0 ]->CreateBox( width, height, length );

	CreateCollisionData();
}

void cObject3D::CreateSphere( float diameter, int rows, int columns )
{
	DeleteMeshes();

	m_iNumMeshes = 1;
	m_pMeshes = new cMesh*[ 1 ];
	m_pMeshes[ 0 ] = new cMesh( this );
	m_pMeshes[ 0 ]->CreateSphere( diameter, rows, columns );

	CreateCollisionData();
}

void cObject3D::CreateCapsule( float diameter, float height, int axis )
{
	DeleteMeshes();

	m_iNumMeshes = 1;
	m_pMeshes = new cMesh*[ 1 ];
	m_pMeshes[ 0 ] = new cMesh( this );
	m_pMeshes[ 0 ]->CreateCapsule( diameter, 11, 16, height, axis );

	CreateCollisionData();
}

void cObject3D::CreateCone( float height, float diameter, int segments )
{
	DeleteMeshes();

	m_iNumMeshes = 1;
	m_pMeshes = new cMesh*[ 1 ];
	m_pMeshes[ 0 ] = new cMesh( this );
	m_pMeshes[ 0 ]->CreateCone( height, diameter, segments );

	CreateCollisionData();
}

void cObject3D::CreateCylinder( float height, float diameter, int segments )
{
	DeleteMeshes();

	m_iNumMeshes = 1;
	m_pMeshes = new cMesh*[ 1 ];
	m_pMeshes[ 0 ] = new cMesh( this );
	m_pMeshes[ 0 ]->CreateCylinder( height, diameter, segments );

	CreateCollisionData();
}

void cObject3D::CreatePlane( float width, float height )
{
	DeleteMeshes();

	m_iNumMeshes = 1;
	m_pMeshes = new cMesh*[ 1 ];
	m_pMeshes[ 0 ] = new cMesh( this );
	m_pMeshes[ 0 ]->CreatePlane( width, height );

	CreateCollisionData();
}

void cObject3D::CreateQuad()
{
	DeleteMeshes();

	m_iNumMeshes = 1;
	m_pMeshes = new cMesh*[ 1 ];
	m_pMeshes[ 0 ] = new cMesh( this );
	m_pMeshes[ 0 ]->CreateQuad();

	m_iObjFlags |= (AGK_OBJECT_IS_QUAD | AGK_OBJECT_NO_FRUSTUM_CULLING);

	SetCullMode( 1 );
	SetDepthWrite( 0 );
	SetDepthReadMode( 7 ); // always pass

	SetShader( AGKShader::g_pObjectQuad );
}

void cObject3D::CreateFromHeightMap( cImage *pImage, float width, float height, float length, int smoothing, int split )
{
	int imgWidth = 0;
	int imgHeight = 0;

	if ( !pImage ) return;
	
	unsigned char* data;
	int size = pImage->GetRawData( &data );
	if ( !data )
	{
		agk::Warning("Failed to get image data");
		return;
	}

	imgWidth = pImage->GetWidth();
	imgHeight = pImage->GetHeight();

	// convert image into unsigned 16-bit integers
	unsigned short *pData = new unsigned short[ imgWidth*imgHeight ];
	for ( int z = 0; z < imgHeight; z++ )
	{
		for ( int x = 0; x < imgWidth; x++ )
		{
			UINT index = z*imgWidth + x;
			pData[ index ] = data[ index*4 ] * 256;
		}
	}

	CreateFromHeightMapFromData( pData, imgWidth, imgHeight, width, height, length, smoothing, split );
	delete [] pData;
}

void cObject3D::CreateFromHeightMap( const char *szHeightMap, float width, float height, float length, int smoothing, int split )
{
	int imgWidth = 0;
	int imgHeight = 0;

	// temporary solution, ideally use png_read_png directly to load a 16bit image
	cImage *pImage = new cImage();
	if ( !pImage->Load( szHeightMap ) )
	{
		delete pImage;

		// create a flat terrain with 1 segment per unit
		imgWidth = width < 1 ? 1 : agk::Floor(width);
		imgHeight = height < 1 ? 1 : agk::Floor(height);

		unsigned short *pData = new unsigned short[ imgWidth*imgHeight ];
		memset( m_pHeightMap, 0, imgWidth*imgHeight*sizeof(short) );
		CreateFromHeightMapFromData( pData, imgWidth, imgHeight, width, height, length, smoothing, split );
		delete [] pData;
	}
	else
	{
		unsigned char* data;
		int size = pImage->GetRawData( &data );
		if ( !data )
		{
			agk::Warning("Failed to get image data");
			return;
		}

		imgWidth = pImage->GetWidth();
		imgHeight = pImage->GetHeight();

		// convert image into unsigned 16-bit integers
		unsigned short *pData = new unsigned short[ imgWidth*imgHeight ];
		for ( int z = 0; z < imgHeight; z++ )
		{
			for ( int x = 0; x < imgWidth; x++ )
			{
				UINT index = z*imgWidth + x;
				pData[ index ] = data[ index*4 ] * 256;
			}
		}

		CreateFromHeightMapFromData( pData, imgWidth, imgHeight, width, height, length, smoothing, split );
		delete [] pData;
		delete pImage;
	}
}

void cObject3D::CreateFromRawHeightMap(const char *szHeightMap, float width, float height, float length, int smoothing, int split, int rawWidth, int rawHeight)
{
	int imgWidth = 0;
	int imgHeight = 0;

	uString m_szFile;
	uString ext;

	m_szFile.SetStr(szHeightMap);
	m_szFile.Lower();
	bool bGameGuruTerrain = false;

	int pos = m_szFile.RevFind('.');
	if (pos >= 0)
	{
		m_szFile.SubString(ext, pos + 1);
	}
	ext.Lower();

	if( ext.CompareTo("dat" ) == 0 )
		bGameGuruTerrain = true;

	//PE: Load in 16 bit raw terrain data.
	cFile oFile;
	if (!oFile.OpenToRead(szHeightMap))
	{
#ifdef _AGK_ERROR_CHECK
		uString errStr;
		errStr.Format("Failed to open .raw HeightMap data %s, file does not exist", szHeightMap);
		agk::Error(errStr);
#endif
		return;
	}

	imgWidth = rawWidth; //PE: raw data width size
	imgHeight = rawHeight; //PE: raw data height size

	unsigned short *pData = new unsigned short[imgWidth*imgHeight];
	if (bGameGuruTerrain) {

		float *f_pData = new float[imgWidth*imgHeight];
		oFile.Seek(4);
		oFile.ReadData((char*)f_pData, imgWidth*imgHeight * 4);

		for (int z = 0; z < imgHeight; z++)
		{
			for (int x = 0; x < imgWidth; x++)
			{
				UINT index = z*imgWidth + x;
				UINT indexinv =   ( ((imgWidth-(z+1))*imgWidth) + x); // Rotate GG terrain to fit normal heightmap.
				pData[index] = (unsigned short)(f_pData[indexinv] * 17.0); // Fit GG height to normal heightmap.
			}
		}

		delete[] f_pData;
	}
	else {
		oFile.ReadData((char*)pData, imgWidth*imgHeight * 2);
	}

	oFile.Close();
	CreateFromHeightMapFromData(pData, imgWidth, imgHeight, width, height, length, smoothing, split);
	delete[] pData;
	return;
}

void cObject3D::CreateFromHeightMapFromData( const unsigned short *pData, int imgWidth, int imgHeight, float width, float height, float length, int smoothing, int split )
{
	DeleteMeshes();

	if ( m_pHeightMap ) delete [] m_pHeightMap;
	
	if ( !pData )
	{
		// create a flat terrain with 1 segment per unit
		imgWidth = width < 1 ? 1 : agk::Floor(width);
		imgHeight = height < 1 ? 1 : agk::Floor(height);

		m_pHeightMap = new unsigned short[ imgWidth*imgHeight ];
		memset( m_pHeightMap, 0, imgWidth*imgHeight*sizeof(short) );
	}
	else
	{
		m_pHeightMap = new unsigned short[ imgWidth*imgHeight ];
		memcpy( m_pHeightMap, pData, imgWidth*imgWidth*2 );
		
		if ( smoothing > 0 )
		{
			float *pValues2 = new float[ imgWidth*imgHeight ];
			float *pValues3 = new float[ imgWidth*imgHeight ];

			for ( int z = 0; z < imgHeight; z++ )
			{
				for ( int x = 0; x < imgWidth; x++ )
				{
					UINT index = z*imgWidth + x;
					pValues2[ index ] = m_pHeightMap[ index ];
				}
			}

			for ( int s = 0; s < smoothing; s++ )
			{
				// 3x3 gaussian blur, separated into horizontal and vertical passes
				for ( int z = 0; z < imgHeight; z++ )
				{
					for ( int x = 0; x < imgWidth; x++ )
					{
						UINT index = z*imgWidth + x;
						UINT index2 = x > 0 ? z*imgWidth + x-1 : index;
						UINT index3 = x < imgWidth-1 ? z*imgWidth + x+1 : index;
						
						float total = pValues2[ index ] * 0.5f;
						total += pValues2[ index2 ] * 0.25f;
						total += pValues2[ index3 ] * 0.25f;
						
						pValues3[ index ] = total;
					}
				}

				for ( int z = 0; z < imgHeight; z++ )
				{
					for ( int x = 0; x < imgWidth; x++ )
					{
						UINT index = z*imgWidth + x;
						UINT index2 = z > 0 ? (z-1)*imgWidth + x : index;
						UINT index3 = z < imgHeight-1 ? (z+1)*imgWidth + x : index;
						
						float total = pValues3[ index ] * 0.5f;
						total += pValues3[ index2 ] * 0.25f;
						total += pValues3[ index3 ] * 0.25f;
						
						pValues2[ index ] = total;
					}
				}
			}

			delete [] pValues3;

			for ( int z = 0; z < imgHeight; z++ )
			{
				for ( int x = 0; x < imgWidth; x++ )
				{
					UINT index = z*imgWidth + x;
					m_pHeightMap[ index ] = (unsigned short) pValues2[ index ];
				}
			}

			delete [] pValues2;
		}
	}

	m_iHeightMapPixelsX = imgWidth;
	m_iHeightMapPixelsZ = imgHeight;
	m_fHeightMapSizeX = width;
	m_fHeightMapSizeY = height;
	m_fHeightMapSizeZ = length;

	// entire smoothed terrain now in m_pHeightMap, check if we need to split it
	
	if ( split < 1 ) split = 1;
	if ( split > imgWidth-1 ) split = imgWidth-1;
	if ( split > imgHeight-1 ) split = imgHeight-1;
	
	m_iNumMeshes = split*split;
	m_pMeshes = new cMesh*[ m_iNumMeshes ];

	float segsX = (imgWidth-1) / (float)split;
	float segsZ = (imgHeight-1) / (float)split;

	float currX = 0; float currZ = segsZ;
	int startX = 0; int endX = 0;
	int startZ = 0; int endZ = agk::Round(currZ);

	for ( UINT m = 0; m < m_iNumMeshes; m++ )
	{
		startX = endX;
		currX += segsX;
		if ( currX > imgWidth - 0.99f ) // to avoid float rounding issues
		{
			startZ = endZ;
			currZ += segsZ;
			endZ = agk::Round(currZ);

			startX = 0;
			currX = segsX;
		}
		endX = agk::Round(currX);
		
		m_pMeshes[ m ] = new cMesh( this );
		m_pMeshes[ m ]->CreateFromHeightMap( m_pHeightMap, imgWidth-1, imgHeight-1, startX, endX, startZ, endZ, width, height, length );
	}

	// don't delete m_pHeightMap as we can use it for height look ups later

	// collision data takes up too much space, use height map look ups instead
	//CreateCollisionData();
	SetCollisionMode( 0 );
}

void cObject3D::CreateFromMeshes( int numMeshes, cMesh **pMesh )
{
	DeleteMeshes();

	if ( numMeshes <= 0 ) return;

	m_iNumMeshes = numMeshes;
	m_pMeshes = new cMesh*[ m_iNumMeshes ];
	for ( UINT i = 0; i < m_iNumMeshes; i++ ) m_pMeshes[ i ] = pMesh[ i ];

	CreateCollisionData();
}

void cObject3D::AddMesh( cMesh *pMesh, int updateCollision )
{
	cMesh **pNewMeshes = new cMesh*[ m_iNumMeshes+1 ];
	if ( m_iNumMeshes > 0 )
	{
		for ( UINT i = 0; i < m_iNumMeshes; i++ ) pNewMeshes[ i ] = m_pMeshes[ i ];
		delete [] m_pMeshes;
	}

	pNewMeshes[ m_iNumMeshes ] = pMesh;
	m_pMeshes = pNewMeshes;
	m_iNumMeshes++;

	if ( updateCollision ) CreateCollisionData();
}

cMesh* cObject3D::GetMesh( UINT index )
{
	if ( index > m_iNumMeshes ) return 0;
	return m_pMeshes[ index ];
}

size_t AGKFileWriteProc( aiFile* pFile, const char* data, size_t size, size_t count )
{
	if ( !pFile || !pFile->UserData ) return 0;

	AGK::cFile *pAGKFile = (AGK::cFile*) (pFile->UserData);
	pAGKFile->WriteData( data, (UINT)(size*count) );
	return size*count;
}

size_t AGKFileReadProc( aiFile* pFile, char* buffer, size_t size, size_t count )
{
	if ( !pFile || !pFile->UserData ) return 0;

	AGK::cFile *pAGKFile = (AGK::cFile*) (pFile->UserData);
	return pAGKFile->ReadData( buffer, (UINT)(size*count) );
}

size_t AGKFileTellProc( aiFile* pFile )
{
	if ( !pFile || !pFile->UserData ) return 0;

	AGK::cFile *pAGKFile = (AGK::cFile*) (pFile->UserData);
	return pAGKFile->GetPos();
}

size_t AGKFileSizeProc( aiFile* pFile )
{
	if ( !pFile || !pFile->UserData ) return 0;

	AGK::cFile *pAGKFile = (AGK::cFile*) (pFile->UserData);
	return pAGKFile->GetSize();
}

void AGKFileFlushProc( aiFile* pFile )
{
	if ( !pFile || !pFile->UserData ) return;

	AGK::cFile *pAGKFile = (AGK::cFile*) (pFile->UserData);
	pAGKFile->Flush();
}

aiReturn AGKFileSeekProc( aiFile* pFile, size_t pos, aiOrigin mode )
{
	if ( !pFile || !pFile->UserData ) return aiReturn_FAILURE;

	AGK::cFile *pAGKFile = (AGK::cFile*) (pFile->UserData);
	if ( mode == aiOrigin_SET ) pAGKFile->Seek((UINT)pos);
	else if ( mode == aiOrigin_CUR )
	{
		pAGKFile->Seek( (UINT)pos + pAGKFile->GetPos() );
	}
	else if ( mode == aiOrigin_END )
	{
		pAGKFile->Seek( (UINT)pos + pAGKFile->GetSize() );
	}

	return aiReturn_SUCCESS;
}

aiFile* AGKFileOpen( aiFileIO* pFile, const char* filename, const char* rw )
{
	AGK::cFile *pAGKFile = new AGK::cFile();
	bool result = false;
	if ( strchr(rw,'a') ) result = pAGKFile->OpenToWrite( filename, true );
	else if ( strchr(rw,'w') ) result = pAGKFile->OpenToWrite( filename );
	else result = pAGKFile->OpenToRead( filename );

	if ( !result ) 
	{
		delete pAGKFile;
		return 0;
	}

	aiFile *pNewFile = new aiFile();
	pNewFile->UserData = (char*) pAGKFile;
	pNewFile->FileSizeProc = AGKFileSizeProc;
	pNewFile->FlushProc = AGKFileFlushProc;
	pNewFile->ReadProc = AGKFileReadProc;
	pNewFile->SeekProc = AGKFileSeekProc;
	pNewFile->TellProc = AGKFileTellProc;
	pNewFile->WriteProc = AGKFileWriteProc;
	return pNewFile;
}

void AGKFileClose( aiFileIO* pFileIO, aiFile* pFile )
{
	if ( pFile )
	{
		if ( pFile->UserData ) 
		{
			AGK::cFile *pAGKFile = (AGK::cFile*) (pFile->UserData);
			pAGKFile->Close();
			delete pAGKFile;
		}
		delete pFile;
	}
}

void cObject3D::LoadObject( const char *szFilename, int withChildren,  float height )
{
	const char *ext = strrchr( szFilename, '.' );
	uString sExt;
	if ( ext ) 
	{	
		sExt.SetStr( ext );
		sExt.Lower();
	}
	
	m_iUsedTextures = 0;

	if ( sExt.CompareTo( ".obj" ) == 0 ) LoadOBJ( szFilename, height );
	else if ( sExt.CompareTo( ".ago" ) == 0 ) LoadAGOAscii( szFilename, height );
	else 
	{
		DeleteMeshes();
		DeleteChildren();
		DeleteAnimations();

		// open asset import library load
		aiFileIO AGKFileIO;
		AGKFileIO.OpenProc = AGKFileOpen;
		AGKFileIO.CloseProc = AGKFileClose;

		aiPropertyStore* props = aiCreatePropertyStore();
		//aiSetImportPropertyInteger(props,AI_CONFIG_IMPORT_TER_MAKE_UVS,1);
		//aiSetImportPropertyFloat(props,AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE,g_smoothAngle);
		aiSetImportPropertyInteger(props,AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );

		unsigned int ppsteps = /*aiProcess_CalcTangentSpace         |*/ // calculate tangents and bitangents if possible
							   aiProcess_JoinIdenticalVertices    | // join identical vertices/ optimize indexing
							   aiProcess_ValidateDataStructure    | // perform a full validation of the loader's output
							   aiProcess_ImproveCacheLocality     | // improve the cache locality of the output vertices
							   aiProcess_RemoveRedundantMaterials | // remove redundant materials
							   aiProcess_FindDegenerates          | // remove degenerated polygons from the import
							   aiProcess_FindInvalidData          | // detect invalid model data, such as invalid normal vectors
							   aiProcess_GenUVCoords              | // convert spherical, cylindrical, box and planar mapping to proper UVs
							   aiProcess_TransformUVCoords        | // preprocess UV transformations (scaling, translation ...)
							   //aiProcess_FindInstances            | // search for instanced meshes and remove them by references to one master
							   aiProcess_LimitBoneWeights         | // limit bone weights to 4 per vertex
							   aiProcess_OptimizeMeshes			  | // join small meshes, if possible;
							   aiProcess_SplitByBoneCount         | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
							   aiProcess_GenSmoothNormals		  | // generate smooth normal vectors if not existing
							   aiProcess_SplitLargeMeshes         | // split large, unrenderable meshes into submeshes
							   aiProcess_Triangulate			  | // triangulate polygons with more than 3 edges
							   /*aiProcess_ConvertToLeftHanded	  |*/ // convert everything to D3D left handed space
							   aiProcess_MakeLeftHanded           | // convert everything to OpenGL left handed space
							   aiProcess_FlipUVs				  | // flips all UV coordinates along the y-axis and adjusts material settings and bitangents accordingly.
							   aiProcess_SortByPType              | // make 'clean' meshes which consist of a single typ of primitives;
							   0;

		if ( withChildren == 0 ) ppsteps |= aiProcess_PreTransformVertices; // remove scene graph and pre-transforms all vertices with the local transformation matrices of their nodes.
		else ppsteps |= aiProcess_OptimizeGraph;

		aiScene *pScene = (aiScene*)aiImportFileExWithProperties( szFilename, ppsteps, &AGKFileIO, props);

		aiReleasePropertyStore(props);

		if ( !pScene )
		{
			uString info;
			info.Format( "Failed to load object \"%s\" - %s", szFilename, aiGetErrorString() );
			agk::Error( info );
			return;
		}

		if ( pScene->mNumMeshes < 1 ) return;

		int hasBones = 0;
		for( UINT i = 0; i < pScene->mNumMeshes; i++ )
		{
			if ( pScene->mMeshes[i]->HasBones() ) 
			{
				hasBones = 1;
				break;
			}
		}

		m_sName.SetStr( pScene->mRootNode->mName.C_Str() );
		m_iUsedTextures = 0;

		//if (pScene->HasTextures()) //PE: to be used for imbedded textures.

		if (pScene->HasMaterials())
		{
			for (unsigned int ti = 0; ti < pScene->mNumMaterials; ti++)
			{
				const aiMaterial* pMaterial = pScene->mMaterials[ti];

				if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
					aiString Path;
					if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {

						if (m_iUsedTextures < AGK_OBJECT_MAX_TEXTURES) {
							m_sTextures[m_iUsedTextures++].SetStr(Path.C_Str());
						}
						else { break; }

					}
				}
			}
		}


		// if withChildren=1 but the object has no bones, animations, or children, load it as if withChildren=0
		if ( withChildren == 0 || (pScene->mRootNode->mNumChildren == 0 && !hasBones && !pScene->HasAnimations()) )
		{
			// load everything as a single object with no bones, animation, or children
			m_iNumMeshes = pScene->mNumMeshes;
			m_pMeshes = new cMesh*[ m_iNumMeshes ];

			float meshscale = height;
			for( UINT i = 0; i < m_iNumMeshes; i++ )
			{
				aiMesh *pMesh = pScene->mMeshes[i];
				m_pMeshes[ i ] = new cMesh( this );
				m_pMeshes[ i ]->CreateMesh( pMesh, meshscale, i==0 ? 0 : 1 ); // scale the first mesh to match the height (scaleMode=0), scale the other meshes accoridngly (scaleMode=1)
				if ( i == 0 ) meshscale = m_pMeshes[ i ]->GetLoadScale();
			}
		}
		else
		{
			float meshscale = 1; // don't scale boned objects, would have to transform every vertex in every mesh to discover bounds

			// temporary store for bones and child objects as we find them
			vector<Bone3D*> boneList;
			vector<cObject3D*> objectList;
			
			std::deque<aiNode*> nodeList;
			nodeList.push_back( pScene->mRootNode );

			// create a bone for each node
			static int warned = 0;
			while( nodeList.size() > 0 )
			{
				aiNode *pNode = nodeList[ 0 ];
				nodeList.pop_front();

				Bone3D *pBone = new Bone3D();
				boneList.push_back( pBone );
				pBone->m_sName.SetStr( pNode->mName.C_Str() );
				if ( pNode->mParent ) 
				{
					pBone->m_pParent = (Bone3D*)pNode->mParent->mUserData;
					if ( pBone->m_pParent ) pBone->m_pParent->AddChild( pBone );
				}
				else 
				{
					AddChild( pBone );
					pBone->m_pParent = 0;
					pBone->m_bFlags |= AGK_BONE3D_ROOT;
				}
				pNode->mUserData = (void*)pBone;

				for ( UINT i = 0; i < pNode->mNumChildren; i++ )
				{
					nodeList.push_back( pNode->mChildren[ i ] );
				}

				// check for sub objects
				if ( pNode->mNumMeshes > 0 )
				{
					cObject3D *pChildObject = 0;
					int count = 0;
					for ( UINT m = 0; m < pNode->mNumMeshes; m++ )
					{
						aiMesh *pMesh = pScene->mMeshes[ pNode->mMeshes[m] ];
						if ( pMesh->HasBones() ) 
						{
							// bone meshes are added to the root object, transform them by their parent node so we can move them
							aiVector3D pos, scale;
							aiQuaternion rot;
							pNode->mTransformation.Decompose( scale, rot, pos );

							// only transform by scale as dual quaternions can't do this, the rest is handled by the bone offset matrix
							for (unsigned int n = 0; n < pMesh->mNumVertices;++n)	{
								pMesh->mVertices[n] *= scale;
							}

							if ( pMesh->HasNormals() )
							{
								// copy normals, transform them to worldspace
								for (unsigned int n = 0; n < pMesh->mNumVertices;++n)	{
									pMesh->mNormals[n] /= scale;
									pMesh->mNormals[n].Normalize();
								}
							}
							if ( pMesh->HasTangentsAndBitangents() )
							{
								// copy tangents and bitangents, transform them to worldspace
								for (unsigned int n = 0; n < pMesh->mNumVertices;++n)	{
									pMesh->mTangents[n] /= scale;
									pMesh->mBitangents[n] /= scale;
									pMesh->mTangents[n].Normalize();
									pMesh->mBitangents[n].Normalize();
								}
							}

							/*
							aiMatrix4x4 mWorldIT = pNode->mTransformation;
							aiNode *parent = pNode->mParent;
							while( parent )
							{
								mWorldIT = mWorldIT * pNode->mParent->mTransformation;
								parent = parent->mParent;
							}
														
							// bone meshes are added to the root object, transform them by their parent node so we can move them
							for (unsigned int n = 0; n < pMesh->mNumVertices;++n)	{
								pMesh->mVertices[n] = mWorldIT * pMesh->mVertices[n];
							}

							mWorldIT.Inverse().Transpose();
							aiMatrix3x3 m = aiMatrix3x3(mWorldIT);

							if ( pMesh->HasNormals() )
							{
								// copy normals, transform them to worldspace
								for (unsigned int n = 0; n < pMesh->mNumVertices;++n)	{
									pMesh->mNormals[n] = (m * pMesh->mNormals[n]).Normalize();
								}
							}

							if ( pMesh->HasTangentsAndBitangents() )
							{
								// copy tangents and bitangents, transform them to worldspace
								for (unsigned int n = 0; n < pMesh->mNumVertices;++n)	{
									pMesh->mTangents[n] = (m * pMesh->mTangents[n]).Normalize();
									pMesh->mBitangents[n] = (m * pMesh->mBitangents[n]).Normalize();
								}
							}
							*/

							continue; 
						}

						if ( !pChildObject ) 
						{
							pChildObject = new cObject3D();
							pChildObject->m_sName.SetStr( pNode->mName.C_Str() );
							pChildObject->m_iID = agk::m_cObject3DList.GetFreeID(); // give it an ID so tier 1 users can access it
							agk::m_cObject3DList.AddItem( pChildObject, pChildObject->m_iID );
							agk::m_cObjectMgr.AddObject( pChildObject );
							objectList.push_back( pChildObject );
							pBone->AddChild( pChildObject );
						}

						cMesh *pAGKMesh = new cMesh( pChildObject );
						pAGKMesh->CreateMesh( pMesh, meshscale, 1 );
						pChildObject->AddMesh( pAGKMesh, 0 );
						count++;
					}

					if ( pChildObject ) pChildObject->CreateCollisionData();
				}

				// set bone transform
				aiVector3D pos, scale;
				aiQuaternion rot;
				pNode->mTransformation.Decompose( scale, rot, pos );

				pos *= meshscale;

				if ( warned == 0 && (agk::Abs(scale.x-1) > 0.01f || agk::Abs(scale.y-1) > 0.01f || agk::Abs(scale.z-1) > 0.01f) )
				{
					warned = 1;
					agk::Warning( "Bone transform matrix contains scale values, scaling of bones is not supported and will be ignored" );
				}

				pBone->SetNodePosition( pos.x, pos.y, pos.z );
				pBone->SetNodeRotation( rot.w, rot.x, rot.y, rot.z );
				//pBone->SetNodeScale( scale.x, scale.y, scale.z );
				pBone->SetNodeScale( 1,1,1 );
				pBone->m_origPosition.Set( pos.x, pos.y, pos.z );
				pBone->m_origRotation.Set( rot.w, rot.x, rot.y, rot.z );
				//pBone->m_origScale.Set( scale.x, scale.y, scale.z );
				pBone->m_origScale.Set( 1,1,1 );

				pBone->m_BoundingBox.resetBox();
			}

			// add found bones to a skeleton
			if ( m_pSkeleton ) delete m_pSkeleton;
			m_pSkeleton = new Skeleton3D();
			m_pSkeleton->SetBones( boneList );

			// add loaded children to object to keep track of them later
			m_iNumChildren = (UINT)objectList.size();
			m_iLoadedChildren = new UINT[ m_iNumChildren ];
			for( UINT i = 0; i < m_iNumChildren; i++ )
			{
				m_iLoadedChildren[ i ] = objectList[ i ]->m_iID;
			}

			// bone meshes will all belong to the root object, add them now that we know about the bones
			int iBoneMeshes = 0;
			static int warned2 = 0;
			for( UINT i = 0; i < pScene->mNumMeshes; i++ )
			{
				if ( pScene->mMeshes[i]->HasBones() ) 
				{
					iBoneMeshes++;
					for( UINT b = 0; b < pScene->mMeshes[i]->mNumBones; b++ )
					{
						// extract the offset matrix and save it in the skeleton bone, doesn't handle scaling since Dual Quaternions can't do scaling
						Bone3D *pBone = m_pSkeleton->GetBone( pScene->mMeshes[i]->mBones[b]->mName.C_Str() );
						if ( !pBone ) continue;

						aiVector3D pos, scale;
						aiQuaternion rot;
						pScene->mMeshes[i]->mBones[b]->mOffsetMatrix.Decompose( scale, rot, pos );

						pBone->m_offsetPosition.Set( pos.x, pos.y, pos.z );
						pBone->m_offsetRotation.Set( rot.w, rot.x, rot.y, rot.z );

						if ( warned2 == 0 && (agk::Abs(scale.x-1) > 0.01f || agk::Abs(scale.y-1) > 0.01f || agk::Abs(scale.z-1) > 0.01f) )
						{
							warned2 = 1;
							agk::Warning( "Bone offset matrix contains scale values, it may not display correctly" );
						}
					}
				}
			}

			m_iNumMeshes = iBoneMeshes;
			if ( m_iNumMeshes > 0 ) m_pMeshes = new cMesh*[ m_iNumMeshes ];
			else m_pMeshes = 0;

			// add bone meshes to root object
			int count = 0;
			for( UINT i = 0; i < pScene->mNumMeshes; i++ )
			{
				if ( pScene->mMeshes[i]->HasBones() ) 
				{
					aiMesh *pMesh = pScene->mMeshes[i];
					m_pMeshes[ count ] = new cMesh( this );
					m_pMeshes[ count ]->CreateMesh( pMesh, meshscale, 1 );

					m_pSkeleton->DiscoverBounds( m_pMeshes[count] );
					
					count++;
				}
			}

			// debug view bones
			/*
			for ( UINT i = 0; i < m_pSkeleton->m_iNumBones; i++ )
			{
				Bone3D *pBone = m_pSkeleton->m_pBones[ i ];
				UINT objID = agk::CreateObjectBox( pBone->m_BoundingBox.maxbx()-pBone->m_BoundingBox.minbx(), pBone->m_BoundingBox.maxby()-pBone->m_BoundingBox.minby(), pBone->m_BoundingBox.maxbz()-pBone->m_BoundingBox.minbz() );
				agk::SetObjectPosition( objID, (pBone->m_BoundingBox.maxbx()+pBone->m_BoundingBox.minbx())/2, (pBone->m_BoundingBox.maxby()+pBone->m_BoundingBox.minby())/2, (pBone->m_BoundingBox.maxbz()+pBone->m_BoundingBox.minbz())/2 );
				pBone->AddChild( agk::GetObjectPtr(objID) );
			}
			*/

			// load animations
			if ( pScene->HasAnimations() )
			{
				m_iNumAnims = pScene->mNumAnimations;
				m_pAnims = new Animation3D*[ m_iNumAnims ];
				for( UINT i = 0; i < m_iNumAnims; i++ )
				{
					m_pAnims[ i ] = new Animation3D();
					m_pAnims[ i ]->LoadFromScene( pScene->mAnimations[ i ] );
				}
			}
		}
		
		// finished with assimp data
		aiReleaseImport(pScene);

		CreateCollisionData();
	}
}

// Use this? It supports a modified OBJ file that has 2 UV coords per vertex
void cObject3D::LoadOBJ( const char *szFilename, float height )
{
	cFile objFile;
	if ( !objFile.OpenToRead( szFilename ) ) 
	{
		uString info;
		info.Format( "Failed to load object \"%s\", file not found", szFilename );
		agk::Warning( info );
		return;
	}

	uString *sLines = 0;
	int lines = 0;

	{
		uString sFileData;
		UINT filesize = objFile.GetSize();
		char* tempstr = new char[ filesize+1 ];
		objFile.ReadData( tempstr, filesize );
		tempstr[ filesize ] = 0;
		sFileData.SetStr( tempstr );
		delete [] tempstr;
		lines = sFileData.SplitTokens2( '\n', sLines );
	}

	objFile.Close();

	if ( lines < 1 ) 
	{
		uString info;
		info.Format( "Failed to load object \"%s\", file contains no data", szFilename );
		agk::Warning( info );
		return;
	}

	DeleteMeshes();

	m_iNumMeshes = 1;
	m_pMeshes = new cMesh*[ 1 ];
	m_pMeshes[ 0 ] = new cMesh( this );
	m_pMeshes[ 0 ]->CreateFromObj( lines, sLines, height, szFilename );

	delete [] sLines;

	CreateCollisionData();
}

void cObject3D::LoadAGOAscii( const char *szFilename, float height )
{
	DeleteMeshes();

	cFile objFile;
	if ( !objFile.OpenToRead( szFilename ) ) 
	{
		uString info;
		info.Format( "Failed to load object \"%s\", file not found", szFilename );
		agk::Error( info );
		return;	
	}

	UINT filesize = objFile.GetSize();
	char *pFileData = new char[ filesize+1 ];
	objFile.ReadData( pFileData, filesize );
	pFileData[ filesize ] = 0;
	objFile.Close();

	int index = 0;
	char sChunkName[ 31 ] = { 0 };
	index = AGO::ParseFindChunk( szFilename, pFileData, index, sChunkName, 31 );
	if ( index < 0 )
	{
		delete [] pFileData;
		return;
	}

	if ( pFileData[ index ] == 0 )
	{
		uString info;
		info.Format( "Failed to load object \"%s\", no object data chunk found", szFilename );
		agk::Error( info );
		delete [] pFileData;
		return;	
	}

	// recognised chunks for this part of the file are "Object"
	if ( strcmp( sChunkName, "Object" ) == 0 )
	{
		index = AGO::ParseChunkObject( szFilename, pFileData, index, this );
		if ( index < 0 )
		{
			delete [] pFileData;
			return;
		}
	}
	else
	{
		// skip unknown chunk
		index = AGO::ParseChunkUnknown( szFilename, pFileData, index );
		if ( index < 0 )
		{
			delete [] pFileData;
			return;
		}
	}

	delete [] pFileData;

	CreateCollisionData();
}

void cObject3D::SaveObject( const char *szFilename )
{
	Assimp::Exporter exporter;
	aiScene *pScene = 0;
	const char *szPath = 0;
	const char *szExt = strrchr( szFilename, '.' );
	if ( !szExt ) 
	{
		agk::Error( "Failed to save object, no file extension found" );
		return;
	}

	uString sExt( szExt );
	sExt.Lower();

	int found = -1;
	for( UINT i = 0, end = (UINT)exporter.GetExportFormatCount(); i < end; ++i ) 
	{
		const aiExportFormatDesc* const e = exporter.GetExportFormatDescription( i );
		if ( sExt.CompareTo( e->fileExtension ) == 0 ) 
		{
			found = i;
			break;
		}
	}

	if ( found < 0 )
	{
		agk::Error( "Failed to save object, unrecognised fileextension. Only .DAE and .OBJ are currently supported" );
		return;
	}

	const aiExportFormatDesc* const e = exporter.GetExportFormatDescription( found );

	// Todo
	//pScene = new aiScene();
	//pScene->mNumMeshes = m_iNumMeshes;
	//exporter.Export( pScene, e->id, szPath );
}

void cObject3D::SetPosition( float x, float y, float z ) 
{
	SetNodePosition(x,y,z); 

	if ( m_pRigidBody ) 
	{
		btTransform trans =	m_pRigidBody->getCenterOfMassTransform();
		trans.setOrigin( btVector3(x,y,z) );
		m_pRigidBody->setCenterOfMassTransform( trans );
		m_pRigidBody->getMotionState()->setWorldTransform( trans );
	}
}

void cObject3D::SetRotationQuat( const AGKQuaternion &rot ) 
{ 
	SetNodeRotation( rot.w, rot.x, rot.y, rot.z ); 
}

void cObject3D::SetRotationEuler( float angX, float angY, float angZ ) 
{ 
	AGKQuaternion q;
	q.MakeFromEulerYXZ( angX, angY, angZ ); 
	SetNodeRotation( q.w, q.x, q.y, q.z );
}

void cObject3D::SetScale( float x, float y, float z ) 
{ 
	SetNodeScale( x,y,z );
}

void cObject3D::SetScalePermanent( float x, float y, float z ) 
{
	if ( m_pSharedColObject ) 
	{
		agk::Error("SetObjectScalePermanent cannot be used on instance objects");
		return;
	}

	for (UINT i = 0; i < m_iNumMeshes; i++ )
	{
		m_pMeshes[ i ]->ScaleMesh( x,y,z );
	}

	// scale bones
	if ( m_pSkeleton ) m_pSkeleton->ScaleBones( x,y,z );
	
	// scale animation frames
	for( UINT i = 0; i < m_iNumAnims; i++ )
	{
		m_pAnims[ i ]->ScaleFrames( x,y,z );
	}

	// update bone bounding boxes
	if ( m_pSkeleton )
	{
		m_pSkeleton->ResetBoundingBoxes();

		for (UINT i = 0; i < m_iNumMeshes; i++ )
		{
			if ( m_pMeshes[ i ]->HasBones() ) m_pSkeleton->DiscoverBounds( m_pMeshes[ i ] );
		}
	}

	// scale collision object
	UpdateCollisionData();
}

void cObject3D::FixPivot() 
{ 
	for (UINT i = 0; i < m_iNumMeshes; i++ )
	{
		m_pMeshes[ i ]->ScaleMesh( scale().x, scale().y, scale().z, 0 );
		m_pMeshes[ i ]->RotateMesh( rot().w, rot().x, rot().y, rot().z, 0 );
		m_pMeshes[ i ]->TranslateMesh( pos().x, pos().y, pos().z, 1 );
	}

	// modify bones
	if ( m_pSkeleton ) 
	{
		m_pSkeleton->FixBonePivots( pos(), rot() );

		// need to modify root bone animation
		for ( int i = 0; i < m_pSkeleton->GetBoneCount(); i++ )
		{
			Bone3D *pBone = m_pSkeleton->GetBone( i );
			if ( !pBone->m_pParent )
			{
				// find animation for this bone
				for( UINT j = 0; j < m_iNumAnims; j++ )
				{
					Anim3DBone *pBoneAnim = m_pAnims[ j ]->GetAnimForBone( pBone->m_sName );
					if ( ! pBoneAnim ) continue;

					for ( UINT k = 0; k < pBoneAnim->m_iNumPositions; k++ )
					{
						pBoneAnim->m_pPositions[ k ].m_position = rot() * pBoneAnim->m_pPositions[ k ].m_position + pos();
					}

					for ( UINT k = 0; k < pBoneAnim->m_iNumRotations; k++ )
					{
						pBoneAnim->m_pRotations[ k ].m_qRotation = rot() * pBoneAnim->m_pRotations[ k ].m_qRotation;
					}
				}
			}
		}

		// update bone bounding boxes
		m_pSkeleton->ResetBoundingBoxes();
		for (UINT i = 0; i < m_iNumMeshes; i++ )
		{
			if ( m_pSkeleton && m_pMeshes[ i ]->HasBones() ) m_pSkeleton->DiscoverBounds( m_pMeshes[ i ] );
		}
	}
	
	TransformChildrenByParent();
	SetNodePosition( 0,0,0 );
	SetNodeRotation( 1,0,0,0 );
	SetNodeScale( 1,1,1 );

	UpdateCollisionData();
}

void cObject3D::SetImage( cImage *pImage, UINT stage )
{
	for( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		m_pMeshes[ i ]->SetImage( pImage, stage );
	}
}

void cObject3D::SetLightMap( cImage *pImage )
{
	for( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		m_pMeshes[ i ]->SetLightMap( pImage );
	}
}

void cObject3D::SetNormalMap( cImage *pImage )
{
	for( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		m_pMeshes[ i ]->SetNormalMap( pImage );
	}
}

void cObject3D::SetNormalMapScale( float scaleU, float scaleV )
{
	for( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		m_pMeshes[ i ]->SetNormalMapScale( scaleU, scaleV );
	}
}

void cObject3D::SetColor( int red, int green, int blue, int alpha )
{
	m_Color.Set( red/255.0f, green/255.0f, blue/255.0f );
	m_fAlpha  = alpha/255.0f;
}

void cObject3D::SetRed ( int iRed )
{
	m_Color.x = iRed/255.0f;
}

void cObject3D::SetGreen ( int iGreen )
{
	m_Color.y = iGreen/255.0f;
}

void cObject3D::SetBlue ( int iBlue )
{
	m_Color.z = iBlue/255.0f;
}

void cObject3D::SetAlpha ( int iAlpha )
{
	m_fAlpha = iAlpha/255.0f;
}

void cObject3D::SetColorEmissive( int red, int green, int blue )
{
	m_ColorEmissive.Set( red/255.0f, green/255.0f, blue/255.0f );
}

void cObject3D::SetLightMode( UINT mode )
{
	if ( mode > 0 ) m_iObjFlags |= AGK_OBJECT_USE_LIGHTS;
	else m_iObjFlags &= ~AGK_OBJECT_USE_LIGHTS;
}

void cObject3D::SetScreenCulling( UINT mode )
{
	if ( mode > 0 ) m_iObjFlags &= ~AGK_OBJECT_NO_FRUSTUM_CULLING;
	else m_iObjFlags |= AGK_OBJECT_NO_FRUSTUM_CULLING;
}

void cObject3D::SetUVOffset( UINT stage, float offsetU, float offsetV )
{
	for( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		m_pMeshes[ i ]->SetUVOffset( stage, offsetU, offsetV );
	}
}

void cObject3D::SetUVScale( UINT stage, float scaleU, float scaleV )
{
	for( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		m_pMeshes[ i ]->SetUVScale( stage, scaleU, scaleV );
	}
}

void cObject3D::SetFogMode( UINT mode )
{
	if ( mode > 0 ) m_iObjFlags |= AGK_OBJECT_USE_FOG;
	else m_iObjFlags &= ~AGK_OBJECT_USE_FOG;
}

void cObject3D::SetShadowCastMode( UINT mode )
{
	if ( mode > 0 ) m_iObjFlags |= AGK_OBJECT_CAST_SHADOWS;
	else m_iObjFlags &= ~AGK_OBJECT_CAST_SHADOWS;
}

void cObject3D::SetShadowReceiveMode( UINT mode )
{
	if ( mode > 0 ) m_iObjFlags |= AGK_OBJECT_RECV_SHADOWS;
	else m_iObjFlags &= ~AGK_OBJECT_RECV_SHADOWS;
}

void cObject3D::SetDepthReadMode( UINT mode )
{
	if ( mode > 8 ) mode = 8;
	m_iZFunc = mode;
}

void cObject3D::SetDepthWrite( UINT mode )
{
	if ( mode > 0 ) m_iObjFlags |= AGK_OBJECT_Z_WRITE;
	else m_iObjFlags &= ~AGK_OBJECT_Z_WRITE;
}

void cObject3D::SetDepthBias( float bias )
{
	m_fZBias = bias;
}

void cObject3D::SetDepthRange( float zNear, float zFar )
{
	m_fDepthNear = zNear;
	m_fDepthFar = zFar;
}

void cObject3D::SetTransparency( UINT mode )
{
	if ( mode > 3 ) mode = 3;
	if ( m_iTransparency != mode ) m_iObjFlags |= AGK_OBJECT_TRANSCHANGED;
	m_iTransparency = mode;
	SetDepthWrite( mode == 0 ? 1 : 0 );
}

void cObject3D::SetBlendModes( UINT src, UINT dst )
{
	if ( src > 10 ) src = AGK_BLEND_ONE;
	if ( dst > 10 ) dst = AGK_BLEND_ONE;
	m_iBlendModes = src | (dst << 4);
}

void cObject3D::SetAlphaMask( UINT mode )
{
	if ( mode ) m_iObjFlags |= AGK_OBJECT_ALPHA_MASK;
	else m_iObjFlags &= ~AGK_OBJECT_ALPHA_MASK;
}

void cObject3D::SetCullMode( UINT mode )
{
	if ( mode > 2 ) mode = 2;
	m_iCullMode = mode;
}

void cObject3D::SetVisible( UINT mode )
{
	if ( mode > 0 ) m_iObjFlags |= AGK_OBJECT_VISIBLE;
	else m_iObjFlags &= ~AGK_OBJECT_VISIBLE;
}

void cObject3D::UpdateCollisionData()
{
	// just rebuild it from scratch for now
	CreateCollisionData();
}

void cObject3D::SetShader( AGKShader *pShader ) 
{ 
	if ( !pShader && IsQuad() ) pShader = AGKShader::g_pObjectQuad;

	for( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		m_pMeshes[ i ]->SetShader( pShader );
	}
}

bool cObject3D::CheckTransparencyChanged() const
{ 
	bool bResult = (m_iObjFlags & AGK_OBJECT_TRANSCHANGED) != 0; 
	return bResult; 
}

bool cObject3D::GetTransparencyChanged() 
{ 
	bool bResult = (m_iObjFlags & AGK_OBJECT_TRANSCHANGED) != 0; 
	m_iObjFlags &= ~AGK_OBJECT_TRANSCHANGED;
	return bResult; 
}

int cObject3D::GetColorRed() const { return agk::Round(m_Color.x*255); }
int cObject3D::GetColorGreen() const { return agk::Round(m_Color.y*255); }
int cObject3D::GetColorBlue() const { return agk::Round(m_Color.z*255); }
int cObject3D::GetAlpha() const { return agk::Round(m_fAlpha*255); }

UINT cObject3D::GetInScreen() 
{ 
	cCamera *pCamera = agk::GetCurrentCamera();
	if ( !pCamera ) return 0;

	AGKVector n[6];
	float d[6];
	for ( int i = 0; i < 6; i++ )
	{
		pCamera->GetFrustumPlane( i, n[i], d[i] );

		AGKVector origin = n[i];
		origin.Mult( -d[i] );

		AGKQuaternion q = rotFinal();
		q.Invert();
		n[i] = q * n[i];
		n[i] = n[i] * scaleFinal();

		origin = origin - posFinal();
		origin = q * origin;
		origin = origin / scaleFinal();

		d[i] = -origin.Dot( n[i] );
	}

	int hasBones = 0;
	for( UINT m = 0; m < m_iNumMeshes; m++ )
	{
		// bone meshes are deformed by bones so this process doesn't work, check bone bounding boxes instead
		if ( m_pMeshes[m]->HasValidBones() ) 
		{
			hasBones = 1;
			continue;
		}

		int inside = 1;
		for ( int i = 0; i < 6; i++ )
		{
			if ( !m_pMeshes[ m ]->InFrustumPlane( &(n[i]), d[i] ) ) 
			{
				inside = 0;
				break;
			}
		}

		if ( inside == 1 )
		{
			return 1;
		}
	}

	if ( hasBones )
	{
		if ( !m_pSkeleton ) return 1; // if the mesh has bones but the object doesn't then assume it is visible

		AGKVector normal[6];
		AGKVector origin[6];
		for ( int i = 0; i < 6; i++ )
		{
			float dist;
			pCamera->GetFrustumPlane( i, normal[i], dist );
			origin[i] = normal[i];
			origin[i].Mult( -dist );
		}

		for ( UINT b = 0; b < m_pSkeleton->m_iNumBones; b++ )
		{
			AGKQuaternion q = m_pSkeleton->m_pBones[b]->rotFinal();
			q.Invert();

			int inside = 1;
			for ( int i = 0; i < 6; i++ )
			{
				AGKVector n2 = q * normal[i];
				n2 = n2 * m_pSkeleton->m_pBones[b]->scaleFinal();

				AGKVector origin2 = origin[i] - m_pSkeleton->m_pBones[b]->posFinal();
				origin2 = q * origin2;
				origin2 = origin2 / m_pSkeleton->m_pBones[b]->scaleFinal();

				float dist = -origin2.Dot( n2 );

				if ( !m_pSkeleton->m_pBones[b]->m_BoundingBox.inFrustumPlane( &n2, dist ) )
				{
					inside = 0;
					break;
				}
			}

			if ( inside == 1 )
			{
				return 1;
			}
		}
	}
	
	return 0;
}

float cObject3D::GetMinX() const
{
	if ( m_iNumMeshes <= 0 ) return 0;

	float fMin = m_pMeshes[0]->GetMinX();
	for ( UINT i = 1; i < m_iNumMeshes; i++ )
	{
		if ( m_pMeshes[i]->GetMinX() < fMin ) fMin = m_pMeshes[0]->GetMinX();
	}

	return fMin;
}

float cObject3D::GetMinY() const
{
	if ( m_iNumMeshes <= 0 ) return 0;

	float fMin = m_pMeshes[0]->GetMinY();
	for ( UINT i = 1; i < m_iNumMeshes; i++ )
	{
		if ( m_pMeshes[i]->GetMinY() < fMin ) fMin = m_pMeshes[0]->GetMinY();
	}

	return fMin;
}

float cObject3D::GetMinZ() const
{
	if ( m_iNumMeshes <= 0 ) return 0;

	float fMin = m_pMeshes[0]->GetMinZ();
	for ( UINT i = 1; i < m_iNumMeshes; i++ )
	{
		if ( m_pMeshes[i]->GetMinZ() < fMin ) fMin = m_pMeshes[0]->GetMinZ();
	}

	return fMin;
}

float cObject3D::GetMaxX() const
{
	if ( m_iNumMeshes <= 0 ) return 0;

	float fMax = m_pMeshes[0]->GetMaxX();
	for ( UINT i = 1; i < m_iNumMeshes; i++ )
	{
		if ( m_pMeshes[i]->GetMaxX() > fMax ) fMax = m_pMeshes[0]->GetMaxX();
	}

	return fMax;
}

float cObject3D::GetMaxY() const
{
	if ( m_iNumMeshes <= 0 ) return 0;

	float fMax = m_pMeshes[0]->GetMaxY();
	for ( UINT i = 1; i < m_iNumMeshes; i++ )
	{
		if ( m_pMeshes[i]->GetMaxY() > fMax ) fMax = m_pMeshes[0]->GetMaxY();
	}

	return fMax;
}

float cObject3D::GetMaxZ() const
{
	if ( m_iNumMeshes <= 0 ) return 0;

	float fMax = m_pMeshes[0]->GetMaxZ();
	for ( UINT i = 1; i < m_iNumMeshes; i++ )
	{
		if ( m_pMeshes[i]->GetMaxZ() > fMax ) fMax = m_pMeshes[0]->GetMaxZ();
	}

	return fMax;
}

void cObject3D::SetShaderConstantByName( const char *name, float v1, float v2, float v3, float v4 )
{
	sObjectUniform *pVar = m_cShaderVariables.GetItem( name );

	if ( !pVar )
	{
		pVar = new sObjectUniform();
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

void cObject3D::SetShaderConstantArrayByName( const char *name, UINT index, float v1, float v2, float v3, float v4 )
{
	if ( strlen(name) > 90 ) return;
	char str[ 100 ];
	sprintf( str, "%s_%d", name, index );

	sObjectUniform *pVar = m_cShaderVariables.GetItem( str );

	if ( !pVar )
	{
		pVar = new sObjectUniform();
		m_cShaderVariables.AddItem( pVar, str );
	}

	pVar->m_sName.SetStr( name );
	pVar->index = (int)index;
	pVar->v1 = v1;
	pVar->v2 = v2;
	pVar->v3 = v3;
	pVar->v4 = v4;
}

void cObject3D::SetShaderConstantDefault( const char *name )
{
	sObjectUniform *pVar = m_cShaderVariables.RemoveItem( name );
	if ( pVar ) delete pVar;
}

// collision
void cObject3D::SetCollisionMode( int mode )
{
	if ( mode == 0 ) m_iObjFlags &= ~AGK_OBJECT_COLLISION_ON;
	else if ( mode == 1 ) 
	{
		m_iObjFlags |= AGK_OBJECT_COLLISION_ON;
		if ( m_pSharedColObject )
		{
			if ( !m_pSharedColObject->m_pColObject )
			{
				// temporarily set collision on for parent object
				int oldMode = (m_pSharedColObject->m_iObjFlags & AGK_OBJECT_COLLISION_ON) ? 1 : 0;
				m_pSharedColObject->m_iObjFlags |= AGK_OBJECT_COLLISION_ON;

				m_pSharedColObject->CreateCollisionData();

				// set collision mode back
				if ( !oldMode ) m_pSharedColObject->m_iObjFlags &= ~AGK_OBJECT_COLLISION_ON;
			}
		}
		else
		{
			if ( !m_pColObject )
			{
				CreateCollisionData();
			}
		}
	}
}

bool cObject3D::RayCast( float oldx,float oldy,float oldz, float x, float y, float z, CollisionResults *results )
{
	AGKVector p( oldx, oldy, oldz );
	AGKVector v( x-oldx, y-oldy, z-oldz );
	AGKVector vn( v );
	vn.Normalize();

	return RayCast( p, v, vn, results );
}

bool cObject3D::RayCast( const AGKVector &p, const AGKVector &v, const AGKVector &vn, CollisionResults *results )
{
	if ( (m_iObjFlags & AGK_OBJECT_COLLISION_ON) == 0 ) return false;

	bool hit = false;

	if ( m_pSkeleton )
	{
		// check bone bounding boxes, will over estimate hits, but better than nothing
		for ( UINT b = 0; b < m_pSkeleton->m_iNumBones; b++ )
		{
			AGKQuaternion q = m_pSkeleton->m_pBones[b]->rotFinal();
			q.Invert();

			AGKVector oldp( p );
			AGKVector v1( v );
			oldp = oldp - m_pSkeleton->m_pBones[b]->posFinal();
			oldp.Mult( q );
			v1.Mult( q );
			AGKVector vi = v1.GetInverse();

			results->setScaleOff();
			results->setChanged( false );

			float adjust = 0.9f;
			Box *origBox = &(m_pSkeleton->m_pBones[b]->m_BoundingBox);
			Box adjustedBox;
			adjustedBox.set( origBox->minbx()*adjust, origBox->minby()*adjust, origBox->minbz()*adjust,
							 origBox->maxbx()*adjust, origBox->maxby()*adjust, origBox->maxbz()*adjust );
			
			float dist1 = 0;
			int side = adjustedBox.intersectBox( &oldp, &vi, &dist1 );

			if ( dist1 >= 0.0 && side > 0 && side <= 6 ) 
			{
				hit = true;

				AGKVector intersect;
				AGKVector normal;
    
				switch(side)
				{
					case 1: normal.Set(-1.0,0,0); break;
					case 2: normal.Set(1.0,0.0,0.0); break;
					case 3: normal.Set(0.0,-1.0,0.0); break;
					case 4: normal.Set(0.0,1.0,0.0); break;
					case 5: normal.Set(0.0,0.0,-1.0); break;
					case 6: normal.Set(0.0,0.0,1.0); break;
				}
			    
				intersect.x = oldp.x + v1.x*dist1;
				intersect.y = oldp.y + v1.y*dist1;
				intersect.z = oldp.z + v1.z*dist1;
			            
				dist1 = dist1*(v1.Length());
			        
				results->addPoint(&intersect,&normal,dist1,0);
			}

			q.Invert();
			AGKVector bonePos = m_pSkeleton->m_pBones[b]->posFinal();
			results->rotatePoints( &q, m_iID, 0, bonePos.x, bonePos.y, bonePos.z );
		}
	}

	// check static collision shape, boned meshes don't create these, but something else might

	CollisionObject *colObj = m_pColObject;
	if ( m_pSharedColObject ) colObj = m_pSharedColObject->m_pColObject;

	if ( !colObj ) return hit;

	AGKVector oldp( p );
	oldp = oldp - posFinal();
	
	float largestScale = GetLargestWorldScale();
	if ( colObj->quickRejectRay( &oldp, &v, &vn, largestScale ) ) return hit;

	AGKVector v1( v );
	AGKVector v1n( vn );
	oldp.Mult( -rotFinal() );
	v1.Mult( -rotFinal() );

	if ( agk::Abs(scaleFinal().x-1) > 0.0001f || agk::Abs(scaleFinal().y-1) > 0.0001f || agk::Abs(scaleFinal().z-1) > 0.0001f )
	{
		oldp = oldp / scaleFinal();
		results->setScaleOn( scaleFinal().x, scaleFinal().y, scaleFinal().z, &oldp );
		v1 = v1 / scaleFinal();
		v1n = v1;
		v1n.Normalize();
	}
	else
	{
		results->setScaleOff();
		v1n.Mult( -rotFinal() );
	}

	results->setChanged( false );
	AGKVector vi = v1.GetInverse();

	if ( colObj->intersects( &oldp, &v1, &v1n, &vi, results ) ) hit = true;

	results->rotatePoints( &rotFinal(), m_iID, 0, posFinal().x, posFinal().y, posFinal().z );

	return hit;
}

bool cObject3D::SphereCast( float oldx,float oldy,float oldz, float x, float y, float z, float rRadius, CollisionResults* results)
{
	AGKVector p( oldx, oldy, oldz );
	AGKVector v( x-oldx, y-oldy, z-oldz );
	AGKVector vn( v );
	vn.Normalize();

	return SphereCast( p, v, vn, rRadius, results );
}

bool cObject3D::SphereCast( const AGKVector &p, const AGKVector &v, const AGKVector &vn, float rRadius, CollisionResults* results)
{
	if ( (m_iObjFlags & AGK_OBJECT_COLLISION_ON) == 0 ) return false;

	CollisionObject *colObj = m_pColObject;
	if ( m_pSharedColObject ) colObj = m_pSharedColObject->m_pColObject;
	if ( !colObj ) return false;

	AGKVector oldp( p );
	oldp = oldp - posFinal();
	
	float largestScale = GetLargestWorldScale();
	if ( colObj->quickRejectSphereRay( &oldp, &v, &vn, rRadius, largestScale ) ) return false;

	oldp.Mult( -rotFinal() );
	AGKVector v1( v );
	AGKVector v1n( vn );
	v1.Mult( -rotFinal() );
	v1n.Mult( -rotFinal() );

	results->setScaleOff();
	results->setChanged( false );
	AGKVector vi = v1.GetInverse();

	bool hit = false;
	if ( agk::Abs(scaleFinal().x-1) > 0.0001f || agk::Abs(scaleFinal().y-1) > 0.0001f || agk::Abs(scaleFinal().z-1) > 0.0001f )
	{
		AGKVector scale( scaleFinal() );
        hit = colObj->sphereIntersects( &oldp, &v1, &v1n, &vi, rRadius, &scale, results );
	}
	else
	{
		hit = colObj->sphereIntersects( &oldp, &v1, &v1n, &vi, rRadius, 0, results );
	}

	results->rotatePoints( &rotFinal(), m_iID, 0, posFinal().x, posFinal().y, posFinal().z );

	return hit;
}

bool cObject3D::SphereSlide( float oldx,float oldy,float oldz, float x, float y, float z, float rRadius, CollisionResults* results)
{
	AGKVector p( oldx, oldy, oldz );
	AGKVector v( x-oldx, y-oldy, z-oldz );
	AGKVector vn( v );
	vn.Normalize();

	return SphereSlide( p, v, vn, rRadius, results );
}

bool cObject3D::SphereSlide( const AGKVector &p, const AGKVector &v, const AGKVector &vn, float rRadius, CollisionResults* results)
{
	if ( (m_iObjFlags & AGK_OBJECT_COLLISION_ON) == 0 ) return false;

	CollisionObject *colObj = m_pColObject;
	if ( m_pSharedColObject ) colObj = m_pSharedColObject->m_pColObject;
	if ( !colObj ) return false;

	bool finalHit = false;

	if ( results->getMaxCollisions() < 4 ) results->setMaxCollisions( 4 );
	
	bool hit = SphereCast( p, v, vn, rRadius, results );
	if ( hit )
	{
		finalHit = true;

		AGKVector p2( p );
		p2 += v;
		results->completeResults( &p2, &v );

		AGKVector intersect, slide, normal, slideVec, slideVecN;
        results->storePoints(0,&intersect,&normal,&slide);
        results->copyElements(0,1);
        slideVec = slide - intersect;
		slideVecN = slideVec;
		slideVecN.Normalize();
        
        results->reset(1000000000);
		hit = SphereCast( intersect, slideVec, slideVecN, rRadius, results );
        if ( !hit )
		{
			intersect += slideVec;
            results->setSlidePoint(1,&intersect);
            results->copyElements(1,0);
            results->setNumCols(1);
		}
		else
		{
			results->completeResults( &p2, &v );

			AGKVector normal2;
            results->storePoints(0,&intersect,&normal2,&slide);
            results->copyElements(0,2);
            slideVec = slide - intersect;
            
            AGKVector v1;
            float dist = normal.Dot( slideVec );
            
            if ( dist < 0.00001 ) 
			{
				slideVec.FlattenToCrossVector( normal, normal2 );
				
                intersect.x += normal.x*(agk::Abs(intersect.x)*0.00001f);
				intersect.y += normal.y*(agk::Abs(intersect.y)*0.00001f);
				intersect.z += normal.z*(agk::Abs(intersect.z)*0.00001f);
			}

			slideVecN = slideVec;
			slideVecN.Normalize();

			results->reset(1000000000);
			hit = SphereCast( intersect, slideVec, slideVecN, rRadius, results );
            if ( !hit )
			{
				intersect += slideVec;
                results->setSlidePoint(2,&intersect);
                results->copyElements(2,0);
                results->setNumCols(2);
			}
			else
			{
				results->completeResults( &p2, &v );
                
                AGKVector normal3;
                results->storePoints(0,&intersect,&normal3,&slide);
                results->copyElements(0,3);
                slideVec = slide - intersect;
                
                dist = normal.Dot( slideVec );
                float dist2 = normal2.Dot( slideVec );
                
                if ( dist < 0.00001f && dist2 < 0.00001f ) slideVec.Set(0,0,0);
                else
                {
                    if ( dist < 0.00001f )
                    {
                        slideVec.FlattenToCrossVector( normal, normal3 );
                        if ( normal2.Dot( slideVec ) < 0.00001f ) slideVec.Set(0,0,0);
                    }
                    if ( dist2 < 0.00001f )
                    {
                        slideVec.FlattenToCrossVector( normal2, normal3 );
                        if ( normal.Dot( slideVec ) < 0.00001f ) slideVec.Set(0,0,0);
                    }
                }
                
                intersect += slideVec;
                results->setSlidePoint(3,&intersect);
                results->copyElements(3,0);
                results->setNumCols(3);
			}		
		}
	}

	return finalHit;
}

/*
bool cObject3D::Intersects( cObject3D* obj2 )
{
	return false;
}
*/

float cObject3D::GetHeightMapHeight( float x, float z )
{
	if ( !m_pHeightMap ) return 0;

	// transform point into object space
	AGKVector p( x-posFinal().x, 0, z-posFinal().z );

	AGKQuaternion q = rotFinal();
	q.Invert();
	p = q * p;

	p = p / scaleFinal();

	// check point is still useful, we'll do the calculation anyway but warn the user
	static int warned = 0;
	if ( agk::Abs(p.y) > 0.001f && warned == 0 )
	{
		warned = 1;
		agk::Warning( "Using GetHeightMapHeight on an object that has rotation in the X and Z directions will not produce meaningful results" );
	}

	// transform point into height map space
	p.z = m_fHeightMapSizeZ - p.z; // height map is inverted relative to the world coordinates
	p.x = (p.x / m_fHeightMapSizeX) * (m_iHeightMapPixelsX-1);
	p.z = (p.z / m_fHeightMapSizeZ) * (m_iHeightMapPixelsZ-1);
	
	// if point is outside the object return 0
	if ( p.x < 0 || p.x > m_iHeightMapPixelsX-1 ) return 0;
	if ( p.z < 0 || p.z > m_iHeightMapPixelsZ-1 ) return 0;

	// sample the height map at the 4 closest points
	int indexX1 = agk::Floor( p.x );
	int indexX2 = agk::Ceil( p.x );

	int indexZ1 = agk::Floor( p.z );
	int indexZ2 = agk::Ceil( p.z );

	float height_00 = m_pHeightMap[ indexZ1*m_iHeightMapPixelsX + indexX1 ] * (m_fHeightMapSizeY/65535.0f);
	float height_10 = m_pHeightMap[ indexZ1*m_iHeightMapPixelsX + indexX2 ] * (m_fHeightMapSizeY/65535.0f);
	float height_01 = m_pHeightMap[ indexZ2*m_iHeightMapPixelsX + indexX1 ] * (m_fHeightMapSizeY/65535.0f);
	float height_11 = m_pHeightMap[ indexZ2*m_iHeightMapPixelsX + indexX2 ] * (m_fHeightMapSizeY/65535.0f);

	float dfx = indexX2 - p.x;
	float dfz = indexZ2 - p.z;

	// blend the points in the X direction
	float height1 = height_00 * dfx + height_10 * (1 - dfx);
	float height2 = height_01 * dfx + height_11 * (1 - dfx);

	// blend the two resulting points in the Z direction
	float height = height1 * dfz + height2 * (1 - dfz);

	return height + posFinal().y;
}

int cObject3D::CompareLightItem( const void* a, const void* b )
{
	lightItem* light1 = (lightItem*)a;
	lightItem* light2 = (lightItem*)b;

	if ( light1->dist == light2->dist ) return 0;
	else if ( light1->dist < light2->dist ) return -1;
	else return 1;
}

void cObject3D::CheckLights()
{
	if ( GetLightMode() == 0 ) return;

	//PE: using 20000 object and no light, FPS goes from 53 to 134 using this line.
	//PE: This funtion is really slow , so added !(m_iObjFlags & AGK_OBJECT_VISIBLE) to turn off the light on non visible objects.
	if (agk::m_cPointLightList.GetCount() == 0 || !(m_iObjFlags & AGK_OBJECT_VISIBLE) ) {
		for (UINT m = 0; m < m_iNumMeshes; ++m) m_pMeshes[m]->SetLights(0, NULL, 0, NULL);
		return;
	}

	AGKVector pos = posFinal();
	AGKQuaternion rot = rotFinal(); rot.Invert();
	AGKVector scale = scaleFinal();

	float largestScale = scale.x;
	if ( scale.y > largestScale ) largestScale = scale.y;
	if ( scale.z > largestScale ) largestScale = scale.z;

	AGKPointLight **pLightShortList = new AGKPointLight*[ agk::m_cPointLightList.GetCount() ];
	lightItem *pLightDist = new lightItem[ agk::m_cPointLightList.GetCount() ];

	AGKPointLight *pVSLights[ AGK_MAX_VERTEX_LIGHTS ];
	AGKPointLight *pPSLights[ AGK_MAX_PIXEL_LIGHTS ];

	for ( UINT m = 0; m < m_iNumMeshes; ++m )
	{
		int iShortListCount = 0;
		int iVSShortListCount = 0;
		int iPSShortListCount = 0;

		//PE: Faster when we use LOD meshes inside objects.
		if ( !(m_pMeshes[m]->m_iFlags & AGK_MESH_VISIBLE) ) {
			m_pMeshes[m]->SetLights(0, NULL, 0, NULL);
			continue;
		}

		AGKPointLight *pLight = agk::m_cPointLightList.GetFirst();
		while( pLight )
		{
			float radius = m_pMeshes[m]->m_fRadius * largestScale;
			float sqrRadius = (radius+pLight->m_fRadius)*(radius+pLight->m_fRadius);
			
			float diffX = pLight->m_position.x - pos.x;
			float diffY = pLight->m_position.y - pos.y;
			float diffZ = pLight->m_position.z - pos.z;
			float sqrDist = diffX*diffX + diffY*diffY + diffZ*diffZ;

			// check bounding sphere
			if ( sqrRadius < sqrDist )
			{
				// light is too far away to affect this mesh
				pLight = agk::m_cPointLightList.GetNext();
				continue;
			}

			// transform light to object space
			AGKVector lightPos = pLight->m_position - pos;
			lightPos = rot * lightPos;

			// check bounding box
			float minx = m_pMeshes[m]->m_BoundingBox.minbx() * scale.x;
			float miny = m_pMeshes[m]->m_BoundingBox.minby() * scale.y;
			float minz = m_pMeshes[m]->m_BoundingBox.minbz() * scale.z;
			float maxx = m_pMeshes[m]->m_BoundingBox.maxbx() * scale.x;
			float maxy = m_pMeshes[m]->m_BoundingBox.maxby() * scale.y;
			float maxz = m_pMeshes[m]->m_BoundingBox.maxbz() * scale.z;
		    
			float sqrDist2 = 0;
		    
			// get the distance of light from the outside of the box
			if (lightPos.x > maxx) sqrDist2 = (lightPos.x - maxx)*(lightPos.x - maxx);
			else if (lightPos.x < minx) sqrDist2 = (minx - lightPos.x)*(minx - lightPos.x);
		    
			if (lightPos.y > maxy) sqrDist2 += (lightPos.y - maxy)*(lightPos.y - maxy);
			else if (lightPos.y < miny) sqrDist2 += (miny - lightPos.y)*(miny - lightPos.y);
		    
			if (lightPos.z > maxz) sqrDist2 += (lightPos.z - maxz)*(lightPos.z - maxz);
			else if (lightPos.z < minz) sqrDist2 += (minz - lightPos.z)*(minz - lightPos.z);
		    
			if ( sqrDist2 > (pLight->m_fRadius*pLight->m_fRadius) ) 
			{
				// light is too far away to affect this mesh
				pLight = agk::m_cPointLightList.GetNext();
				continue;
			}

			pLightShortList[ iShortListCount ] = pLight;
			pLightDist[ iShortListCount ].dist = sqrDist;
			pLightDist[ iShortListCount ].index = iShortListCount;
			iShortListCount++;
			if ( pLight->m_iType == 0 ) iVSShortListCount++;
			else iPSShortListCount++;

			pLight = agk::m_cPointLightList.GetNext();
		}

		// All lights in pLightShortList should affect the mesh, if we are not over the limit send them all
		// otherwise we will have to sort by distance
		int iVSCount = 0;
		int iPSCount = 0;
		bool bSorted = false;

		if ( iVSShortListCount > 0 && iVSShortListCount <= AGK_MAX_VERTEX_LIGHTS )
		{
			for ( int L = 0; L < iShortListCount; ++L )
			{
				if ( pLightShortList[ L ]->m_iType == 0 ) pVSLights[ iVSCount++ ] = pLightShortList[ L ];
			}
		}
		else
		{
			// need to sort by distance
			qsort( pLightDist, iShortListCount, sizeof(lightItem), CompareLightItem );
			bSorted = true;

			for ( int L = 0; L < iShortListCount && iVSCount < AGK_MAX_VERTEX_LIGHTS; ++L )
			{
				int index = pLightDist[ L ].index;
				if ( pLightShortList[ index ]->m_iType == 0 ) pVSLights[ iVSCount++ ] = pLightShortList[ index ];
			}
		}

		// do the same for pixel lights
		if ( iPSShortListCount > 0 && iPSShortListCount <= AGK_MAX_PIXEL_LIGHTS )
		{
			for ( int L = 0; L < iShortListCount; ++L )
			{
				if ( pLightShortList[ L ]->m_iType > 0 ) pPSLights[ iPSCount++ ] = pLightShortList[ L ];
			}
		}
		else
		{
			// need to sort by distance
			if ( !bSorted ) qsort( pLightDist, iShortListCount, sizeof(lightItem), CompareLightItem );

			int L;
			for ( L = 0; L < iShortListCount && iPSCount < AGK_MAX_PIXEL_LIGHTS; ++L )
			{
				int index = pLightDist[ L ].index;
				if ( pLightShortList[ index ]->m_iType > 0 ) pPSLights[ iPSCount++ ] = pLightShortList[ index ];
			}

			// add any remaining pixel lights as vertex lights if possible
			if ( iVSCount < AGK_MAX_VERTEX_LIGHTS )
			{
				for ( ; L < iShortListCount && iVSCount < AGK_MAX_VERTEX_LIGHTS; ++L )
				{
					int index = pLightDist[ L ].index;
					if ( pLightShortList[ index ]->m_iType > 0 ) pVSLights[ iVSCount++ ] = pLightShortList[ index ];
				}
			}
		}

		m_pMeshes[ m ]->SetLights( iVSCount, pVSLights, iPSCount, pPSLights );
	}

	delete [] pLightShortList;
	delete [] pLightDist;
}

void cObject3D::Update( float time )
{
	if ( m_pRigidBody )
	{
		btTransform trans;
		m_pRigidBody->getMotionState()->getWorldTransform(trans);
		btVector3 pos = trans.getOrigin();
		btQuaternion rot = trans.getRotation();

		SetNodePosition( pos.x(), pos.y(), pos.z() );
		SetNodeRotation( rot.w(), rot.x(), rot.y(), rot.z() );
	}

	if ( m_pSkeleton ) m_pSkeleton->Update( time );
}

void cObject3D::Draw()
{

	CheckLights();

	for ( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		m_pMeshes[ i ]->CheckShader();
	}

	//PE: This should REALLY be moved to the very top of this functions , it really slow down everything when its down here!
	//PE: On a normal level we have many not visible object, used for collision/billboards/LOD1/LOD2/custom culling ... so this "would" make a huge FPS improvement.
	if (!(m_iObjFlags & AGK_OBJECT_VISIBLE)) return;

	int doneSetup = 0;

	for ( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		if ( !m_pMeshes[i]->GetVisible() ) continue;

		if ( !m_pMeshes[i]->m_pShader ) continue;

		if ( !(m_iObjFlags & AGK_OBJECT_NO_FRUSTUM_CULLING) && !m_pMeshes[i]->GetInScreen() ) continue;

		m_pMeshes[i]->m_pShader->MakeActive();
		SetupDrawing();

		m_pMeshes[ i ]->Draw();

	}
}

void cObject3D::DrawShadow()
{
	//if ( !(m_iObjFlags & AGK_OBJECT_VISIBLE) ) return; // draw shadow even if object is inivisible, To be able to draw hidden shadow.
	if ( !(m_iObjFlags & AGK_OBJECT_CAST_SHADOWS) ) return;

	int doneSetup = 0;
	for ( UINT i = 0; i < m_iNumMeshes; i++ )
	{
		if ( !m_pMeshes[i]->m_pShader ) continue;

		if ( !(m_iObjFlags & AGK_OBJECT_NO_FRUSTUM_CULLING) && !m_pMeshes[i]->GetInShadowFrustum() ) continue;

		if ( m_pMeshes[i]->HasValidBones() )
		{
			AGKShader *pShadowShader = AGKShader::GetShadowShader( m_pMeshes[ i ]->GetNumBones(), HasAlphaMask()?1:0 );
			if ( AGKShader::GetCurrentShader() != pShadowShader ) 
			{
				pShadowShader->MakeActive();
				doneSetup = 0;
			}
			
			if ( doneSetup == 0 )
			{
				SetupDrawingBones();
			}
		}
		else
		{
			AGKShader *pShadowShader = AGKShader::GetShadowShader( 0, HasAlphaMask()?1:0 );
			if ( AGKShader::GetCurrentShader() != pShadowShader ) 
			{
				pShadowShader->MakeActive();
				doneSetup = 0;
			}

			if ( doneSetup == 0 )
			{
				AGKMatrix4 matWorld;
				matWorld.MakeWorld( rotFinal(), posFinal(), scaleFinal() );
				AGKShader::GetCurrentShader()->SetWorldMatrix( matWorld.GetFloatPtr() );
			}
		}

		m_pMeshes[ i ]->DrawShadow();
	}
}

void cObject3D::SetupDrawingBones()
{
	if ( !(m_iObjFlags & AGK_OBJECT_VISIBLE) ) return;
	if ( !AGKShader::GetCurrentShader() ) return;

	if ( m_pSkeleton && m_pMeshes )
	{
		AGKQuaternion prevRot;

		float quats1[ AGK_OBJECT_MAX_BONES*4 ];
		float quats2[ AGK_OBJECT_MAX_BONES*4 ];

		// add bone transforms to shader as dual quaternions
		for ( UINT b = 0; b < m_pSkeleton->m_iNumBones; b++ )
		{
			// transform bone by the inverse bind pose
			/*
			AGKQuaternion q(m_pSkeleton->m_pBones[ b ]->m_origRotation);
			q.Invert();
			AGKQuaternion boneRot = m_pSkeleton->m_pBones[ b ]->rotFinal() * q;

			AGKVector v(m_pSkeleton->m_pBones[ b ]->m_origPosition);
			AGKVector bonePos = (q * -v) / m_pSkeleton->m_pBones[ b ]->m_origScale;
			bonePos = (m_pSkeleton->m_pBones[ b ]->rotFinal() * (bonePos*m_pSkeleton->m_pBones[ b ]->scaleFinal())) + m_pSkeleton->m_pBones[ b ]->posFinal();
			*/

			// use offset matrix instead
			AGKQuaternion q(m_pSkeleton->m_pBones[ b ]->m_offsetRotation);
			AGKQuaternion boneRot = m_pSkeleton->m_pBones[ b ]->rotFinal() * q;
			AGKVector bonePos = (m_pSkeleton->m_pBones[ b ]->rotFinal() * (m_pSkeleton->m_pBones[ b ]->m_offsetPosition*m_pSkeleton->m_pBones[ b ]->scaleFinal())) + m_pSkeleton->m_pBones[ b ]->posFinal();

			// dual quats can't represent scale

			if ( m_pSkeleton->m_pBones[ b ]->m_pParent && m_pSkeleton->m_pBones[ b ]->m_pParent->m_tempRot.Dot(boneRot) < 0 ) boneRot.Negate();
			m_pSkeleton->m_pBones[ b ]->m_tempRot = boneRot;

			//m_pShader->SetTempConstantArrayByName( "agk_bonequats1", b, boneRot.x, boneRot.y, boneRot.z, boneRot.w );
			quats1[ b*4 + 0 ] = boneRot.x;
			quats1[ b*4 + 1 ] = boneRot.y;
			quats1[ b*4 + 2 ] = boneRot.z;
			quats1[ b*4 + 3 ] = boneRot.w;

			AGKQuaternion q2;
			q2.x =  0.5f * ( bonePos.x*boneRot.w + bonePos.y*boneRot.z - bonePos.z*boneRot.y);
			q2.y =  0.5f * (-bonePos.x*boneRot.z + bonePos.y*boneRot.w + bonePos.z*boneRot.x);
			q2.z =  0.5f * ( bonePos.x*boneRot.y - bonePos.y*boneRot.x + bonePos.z*boneRot.w);
			q2.w = -0.5f * ( bonePos.x*boneRot.x + bonePos.y*boneRot.y + bonePos.z*boneRot.z);

			//m_pShader->SetTempConstantArrayByName( "agk_bonequats2", b, q2.x, q2.y, q2.z, q2.w );
			quats2[ b*4 + 0 ] = q2.x;
			quats2[ b*4 + 1 ] = q2.y;
			quats2[ b*4 + 2 ] = q2.z;
			quats2[ b*4 + 3 ] = q2.w;
		}

		AGKShader::GetCurrentShader()->SetTempConstantArrayByName( "agk_bonequats1", quats1 );
		AGKShader::GetCurrentShader()->SetTempConstantArrayByName( "agk_bonequats2", quats2 );
	}
}

void cObject3D::SetupDrawing()
{
	if ( !(m_iObjFlags & AGK_OBJECT_VISIBLE) ) return;
	if ( !AGKShader::GetCurrentShader() ) return;

	SetupDrawingBones();
	
	AGKShader::GetCurrentShader()->SetTempConstantByName( "agk_MeshDiffuse", m_Color.x, m_Color.y, m_Color.z, m_fAlpha );
	AGKShader::GetCurrentShader()->SetTempConstantByName( "agk_MeshEmissive", m_ColorEmissive.x, m_ColorEmissive.y, m_ColorEmissive.z, 0 );
	AGKShader::GetCurrentShader()->SetTempConstantByName( "agk_ObjPos", GetWorldX(), GetWorldY(), GetWorldZ(), 0 );

	if ( GetFogMode() && GetTransparency() == 2 )
	{
		AGKShader::GetCurrentShader()->SetTempConstantByName( "fogColor1", 0,0,0,0 );
		AGKShader::GetCurrentShader()->SetTempConstantByName( "fogColor2", 0,0,0,0 );
	}

	// setup user defined shader variables for this object
	sObjectUniform *pVar = m_cShaderVariables.GetFirst();
	while ( pVar )
	{
		if ( pVar->index < 0 ) AGKShader::GetCurrentShader()->SetTempConstantByName( pVar->m_sName, pVar->v1, pVar->v2, pVar->v3, pVar->v4 );
		else AGKShader::GetCurrentShader()->SetTempConstantArrayByName( pVar->m_sName, pVar->index, pVar->v1, pVar->v2, pVar->v3, pVar->v4 );
		pVar = m_cShaderVariables.GetNext();
	}

	PlatformSetupDrawing();
}

