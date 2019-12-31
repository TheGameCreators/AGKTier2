#include "agk.h"

using namespace AGK;

AGKShader* AGKShader::g_pCurrentShader = 0;
AGKShader* AGKShader::g_pAllShaders = 0;
AGKShader* AGKShader::g_pLastShader = 0;
int AGKShader::g_iNumShaders = 0;
AGKMatrix4 AGKShader::g_matOrtho;
AGKMatrix4 AGKShader::g_matShadowProj;
AGKMatrix4 AGKShader::g_matShadow2Proj;
AGKMatrix4 AGKShader::g_matShadow3Proj;
AGKMatrix4 AGKShader::g_matShadow4Proj;
float AGKShader::g_fShadowParams2[4];

AGKShader* AGKShader::g_pShaderColor = 0;
AGKShader* AGKShader::g_pShaderTexColor = 0;
AGKShader* AGKShader::g_pShaderFont = 0;
AGKShader* AGKShader::g_pObjectQuad = 0;
AGKShader* AGKShader::g_pShaderShadow = 0;
AGKShader* AGKShader::g_pShaderShadowAlpha = 0;
AGKShader* AGKShader::g_pShaderShadowBone = 0;
AGKShader* AGKShader::g_pShaderShadowBoneAlpha = 0;
AGKShader* AGKShader::g_pShader3DParticlesTex = 0;
AGKShader* AGKShader::g_pShader3DParticlesColor = 0;

float AGKShader::g_fFogColorR = 0.63f;
float AGKShader::g_fFogColorG = 0.72f;
float AGKShader::g_fFogColorB = 0.82f;

float AGKShader::g_fFogColor2R = 1.0f;
float AGKShader::g_fFogColor2G = 0.9f;
float AGKShader::g_fFogColor2B = 0.7f;

float AGKShader::g_fFogMinDist = 50.0f;
float AGKShader::g_fFogMaxDist = 700.0f;

int AGKShader::g_iNumShadowBones = 0;
int AGKShader::g_iNumShadowBonesAlpha = 0;

char AGKShader::g_iAttributeActive[ AGK_MAX_ATTRIBUTES ] = { 0 };

void AGKShader::CreateDefaultShaders()
{
	if ( !g_pShaderColor ) g_pShaderColor = new AGKShader();
	if ( !g_pShaderTexColor ) g_pShaderTexColor = new AGKShader();
	if ( !g_pShaderFont ) g_pShaderFont = new AGKShader();
	if ( !g_pObjectQuad ) g_pObjectQuad = new AGKShader();
	//if ( !g_pShaderShadow ) g_pShaderShadow = new AGKShader();
	//if ( !g_pShaderShadowBone ) g_pShaderShadowBone = new AGKShader();
	if ( !g_pShader3DParticlesTex ) g_pShader3DParticlesTex = new AGKShader();
	if ( !g_pShader3DParticlesColor ) g_pShader3DParticlesColor = new AGKShader();

	g_pShaderColor->MakeColorShader();
	g_pShaderTexColor->MakeTexColorShader();
	g_pShaderFont->MakeFontShader();
	g_pObjectQuad->MakeQuadShader();
	//g_pShaderShadow->MakeShadowShader(0);
	//g_pShaderShadowBone->MakeShadowShader(40);
	g_pShader3DParticlesTex->Make3DParticlesTexShader();
	g_pShader3DParticlesColor->Make3DParticlesColorShader();
}

AGKShader* AGKShader::GetShadowShader( int numBones, int alphamask )
{
	if ( numBones > 0 )
	{
		if ( alphamask )
		{
			if ( !g_pShaderShadowBoneAlpha ) 
			{
				g_pShaderShadowBoneAlpha = new AGKShader();
				g_pShaderShadowBoneAlpha->MakeShadowShader( numBones, 1 );
			}
			else
			{
				if ( g_iNumShadowBonesAlpha < numBones ) g_pShaderShadowBoneAlpha->MakeShadowShader( numBones, 1 );
			}
			return g_pShaderShadowBoneAlpha;
		}
		else
		{
			if ( !g_pShaderShadowBone ) 
			{
				g_pShaderShadowBone = new AGKShader();
				g_pShaderShadowBone->MakeShadowShader( numBones, 0 );
			}
			else
			{
				if ( g_iNumShadowBones < numBones ) g_pShaderShadowBone->MakeShadowShader( numBones, 0 );
			}
			return g_pShaderShadowBone;
		}
	}
	else
	{
		if ( alphamask )
		{
			if ( !g_pShaderShadow ) 
			{
				g_pShaderShadow = new AGKShader();
				g_pShaderShadow->MakeShadowShader( 0, 1 );
			}
			return g_pShaderShadow;
		}
		else
		{
			if ( !g_pShaderShadowAlpha ) 
			{
				g_pShaderShadowAlpha = new AGKShader();
				g_pShaderShadowAlpha->MakeShadowShader( 0, 0 );
			}
			return g_pShaderShadowAlpha;
		}
	}
}

void AGKShader::SetOrthoMatrices( const float *matrix4x4 )
{
	if ( g_matOrtho == matrix4x4 ) return;

	g_matOrtho.Set( matrix4x4 );

	AGKShader *pShader = g_pAllShaders;
	while( pShader )
	{
		pShader->SetOrthoMatrixChanged();
		pShader = pShader->m_pNextShader;
	}
}

void AGKShader::SetShadowProjMatrices( const float *matrix4x4 )
{
	if ( g_matShadowProj == matrix4x4 ) return;

	g_matShadowProj.Set( matrix4x4 );

	AGKShader *pShader = g_pAllShaders;
	while( pShader )
	{
		pShader->SetShadowProjMatrixChanged();
		pShader = pShader->m_pNextShader;
	}
}

void AGKShader::SetCascadeShadowProjMatrices( AGKMatrix4 *matrices )
{
	g_matShadowProj.Set( matrices[0].GetFloatPtr() );
	g_matShadow2Proj.Set( matrices[1].GetFloatPtr() );
	g_matShadow3Proj.Set( matrices[2].GetFloatPtr() );
	g_matShadow4Proj.Set( matrices[3].GetFloatPtr() );

	AGKShader *pShader = g_pAllShaders;
	while( pShader )
	{
		pShader->SetShadowProjMatrixChanged();
		pShader = pShader->m_pNextShader;
	}
}

void AGKShader::SetShadowParams2( float f1, float f2, float f3, float f4 )
{
	g_fShadowParams2[ 0 ] = f1;
	g_fShadowParams2[ 1 ] = f2;
	g_fShadowParams2[ 2 ] = f3;
	g_fShadowParams2[ 3 ] = f4;
}

void AGKShader::ReloadAll()
{
	NoShader();

	// delete
	AGKShader *pShader = g_pAllShaders;
	while( pShader )
	{
		pShader->PlatformDelete();
		pShader = pShader->m_pNextShader;
	}

	// recreate, do not merge this loop and the one above as they do not interact correctly
	pShader = g_pAllShaders;
	while( pShader )
	{
		pShader->PlatformInit();

		pShader->m_bFlags |= AGK_SHADER_RELOAD_UNIFORMS;
		pShader->m_pCurrentCamera = 0;

		if ( pShader->m_bValid )
		{
			pShader->m_bReloading = true;
			if ( !pShader->NeedsAdditionalCode() )
			{
				pShader->SetShaderSource( pShader->m_sVSSource, pShader->m_sPSSource );
			}
		}
				
		pShader = pShader->m_pNextShader;
	}

	//CreateDefaultShaders();

	pShader = g_pAllShaders;
	while( pShader )
	{
		pShader->m_pChangedUniforms = 0;
		cShaderUniform* pUniform = pShader->m_cUniformList.GetFirst();
		while ( pUniform )
		{
			pUniform->m_bChanged = true;
			pUniform->m_pNextUniform = pShader->m_pChangedUniforms;
			pShader->m_pChangedUniforms = pUniform;

			pUniform = pShader->m_cUniformList.GetNext();
		}

		pShader->m_bReloading = false;
		pShader = pShader->m_pNextShader;
	}
}

void AGKShader::UpdateAllCamera()
{
	AGKShader *pShader = g_pAllShaders;
	while( pShader )
	{
		if ( agk::m_pCurrentCamera && (agk::m_pCurrentCamera->HasUpdated() || agk::m_pCurrentCamera != pShader->m_pCurrentCamera) )
		{
			AGKMatrix4 view;
			agk::m_pCurrentCamera->GetViewMatrix( view );
			pShader->SetViewMatrix( view.GetFloatPtr() );
		}

		if ( agk::m_pCurrentCamera && (agk::m_pCurrentCamera->HasProjUpdated() || agk::m_pCurrentCamera != pShader->m_pCurrentCamera) )
		{
			const AGKMatrix4 *proj = agk::m_pCurrentCamera->GetProjMatrix();
			pShader->SetProjMatrix( &(proj->mat[0][0]) );
			pShader->m_pCurrentCamera = agk::m_pCurrentCamera;
		}
		
		pShader = pShader->m_pNextShader;
	}
}

void AGKShader::SetFogColor( float red, float green, float blue )
{
	g_fFogColorR = red; g_fFogColorG = green; g_fFogColorB = blue;

	AGKShader *pShader = g_pAllShaders;
	while( pShader )
	{
		pShader->SetConstantByName( "fogColor1", red, green, blue, 0 );
		pShader = pShader->m_pNextShader;
	}
}

void AGKShader::SetFogSunColor( float red, float green, float blue )
{
	g_fFogColor2R = red; g_fFogColor2G = green; g_fFogColor2B = blue;

	AGKShader *pShader = g_pAllShaders;
	while( pShader )
	{
		pShader->SetConstantByName( "fogColor2", red, green, blue, 0 );
		pShader = pShader->m_pNextShader;
	}
}

void AGKShader::SetFogRange( float minDist, float maxDist )
{
	g_fFogMinDist = minDist; g_fFogMaxDist = maxDist;

	AGKShader *pShader = g_pAllShaders;
	while( pShader )
	{
		pShader->SetConstantByName( "fogRange", minDist, -4.0f/(maxDist - minDist), 0, 0 ); // do some preprocessing so it isn't done every pixel
		pShader = pShader->m_pNextShader;
	}
}


AGKShader::AGKShader() : m_cUniformList(256), m_cAttribList(8), m_cDerivedShaders(16)
{ 
	m_iShaderID = 0;
	m_bValid = false;
	m_bReloading = false;
	m_bDeleting = false;

	m_pBaseShader = 0;
	m_iShaderHash = 0;

	// uniforms default to non-existant
	m_iUniformWorldMat = -1;
	m_iUniformNormalMat = -1;
	m_iUniformViewMat = -1;
	m_iUniformProjMat = -1;
	m_iUniformOrthoMat = -1;
	m_iUniformVPMat = -1;
	m_iUniformWVPMat = -1;
	m_iUniformWVOMat = -1;
	m_iUniformShadowProjMat = -1;
	m_iUniformShadowProj2Mat = -1;
	m_iUniformShadowProj3Mat = -1;
	m_iUniformShadowProj4Mat = -1;
	m_iNumUniforms = 0;

	m_pChangedUniforms = 0;

	// agk uniforms
	m_iAGKTime = -1;
	m_iAGKSinTime = -1;
	m_iAGKResolution = -1;
	m_iAGKInvert = -1;
	m_iAGKCameraPos = -1;
	m_iAGKShadowParams = -1;
	m_iAGKShadowParams2 = -1;

	// textures
	for ( int i = 0; i < AGK_MAX_TEXTURES; i++ ) 
	{
		m_iTexture2D[ i ] = -1;
		m_iTextureCube[ i ] = -1;
		m_iTextureBounds[ i ] = -1;

		m_fTextureU1[ i ] = 0;
		m_fTextureV1[ i ] = 0;
		m_fTextureU2[ i ] = 1;
		m_fTextureV2[ i ] = 1;

		m_fU1[ i ] = 0;
		m_fV1[ i ] = 0;
		m_fU2[ i ] = 1;
		m_fV2[ i ] = 1;
	}

	m_iShadowMapTex = -1;
	m_iShadowMap2Tex = -1;
	m_iShadowMap3Tex = -1;
	m_iShadowMap4Tex = -1;

	g_fShadowParams2[0] = 0;
	g_fShadowParams2[1] = 0;
	g_fShadowParams2[2] = 0;
	g_fShadowParams2[3] = 0;

	m_bTextureBoundsChanged = 0;
	m_bUVBoundsChanged = 0;
	
	// attributes
	m_iPositionLoc = -1;
	m_iNormalLoc = -1;
	m_iTangentLoc = -1;
	m_iBiNormalLoc = -1;
	m_iColorLoc = -1;
	m_iTexCoordLoc = -1;
	m_iBoneWeightsLoc = -1;
	m_iBoneIndicesLoc = -1;
	m_iNumAttribs = 0;

	m_bFlags = AGK_SHADER_RELOAD_UNIFORMS;

	m_pCurrentCamera = 0;

	m_iMeshUseCount = 0;

	// add to list of all shaders
	m_pNextShader = g_pAllShaders;
	m_pPrevShader = 0;
	if ( g_pAllShaders ) g_pAllShaders->m_pPrevShader = this;
	g_pAllShaders = this;
	if ( !g_pLastShader ) g_pLastShader = this;
	g_iNumShaders++;

	PlatformInit();
}

AGKShader::~AGKShader() 
{ 
	m_bDeleting = true;

	if ( g_pCurrentShader == this )
	{
		NoShader();
	}

	// remove from list of all shaders
	if ( m_pNextShader ) m_pNextShader->m_pPrevShader = m_pPrevShader;
	else g_pLastShader = m_pPrevShader;
	
	if ( m_pPrevShader ) m_pPrevShader->m_pNextShader = m_pNextShader;
	else g_pAllShaders = m_pNextShader;

	g_iNumShaders--;

	if ( !m_pBaseShader )
	{
		// this is a base shader, so others may exist that were based on it, need to delete them too
		AGKShader *pShader = m_cDerivedShaders.GetFirst();
		while( pShader )
		{
			delete pShader;
			pShader = m_cDerivedShaders.GetNext();
		}
	}
	else
	{
		m_pBaseShader->RemoveDerived( this );
	}

	m_pChangedUniforms = 0;
	cShaderUniform* pUniform = m_cUniformList.GetFirst();
	while ( pUniform )
	{
		delete pUniform;
		pUniform = m_cUniformList.GetNext();
	}
	m_cUniformList.ClearAll();

	cShaderAttrib* pAttrib = m_cAttribList.GetFirst();
	while ( pAttrib )
	{
		delete pAttrib;
		pAttrib = m_cAttribList.GetNext();
	}
	m_cAttribList.ClearAll();

	PlatformDelete();
}

void AGKShader::AddDerived( AGKShader *pShader )
{
	if ( m_bDeleting ) return;
	m_cDerivedShaders.AddItem( pShader, pShader );
}

void AGKShader::RemoveDerived( AGKShader *pShader )
{
	if ( m_bDeleting ) return;
	m_cDerivedShaders.RemoveItem( pShader );
}

void AGKShader::AddRef() 
{ 
	m_iMeshUseCount++;

	if ( g_pAllShaders == this ) return;

	// remove from list of all shaders
	if ( m_pNextShader ) m_pNextShader->m_pPrevShader = m_pPrevShader;
	else g_pLastShader = m_pPrevShader;
	
	if ( m_pPrevShader ) m_pPrevShader->m_pNextShader = m_pNextShader;
	else g_pAllShaders = m_pNextShader;

	// re-add at head of the list so we can keep track of most recently used
	m_pNextShader = g_pAllShaders;
	m_pPrevShader = 0;
	if ( g_pAllShaders ) g_pAllShaders->m_pPrevShader = this;
	g_pAllShaders = this;
	if ( !g_pLastShader ) g_pLastShader = this;
}

void AGKShader::RemoveRef() 
{ 
	m_iMeshUseCount--; 
	if ( m_iMeshUseCount < 0 ) agk::Error("Shader released too many times"); 
}

void AGKShader::LoadShader( const char* vertexFile, const char* pixelFile )
{
	if ( !vertexFile || !pixelFile ) return;

	cFile vertex, pixel;
	if ( !vertex.OpenToRead( vertexFile ) )
	{
		uString info;
		info.Format( "Failed to open vertex shader file %s for reading", vertexFile );
		agk::Warning( info.GetStr() );
		return;
	}

	if ( !pixel.OpenToRead( pixelFile ) )
	{
		uString info;
		info.Format( "Failed to open pixel shader file %s for reading", pixelFile );
		agk::Warning( info.GetStr() );
		return;
	}

	m_sVSFilename.SetStr( vertexFile );
	m_sPSFilename.SetStr( pixelFile );

	UINT length = vertex.GetSize();
	char *vertexSource = new char[ length+1 ];
	vertex.ReadData( vertexSource, length );
	vertexSource[ length ] = 0;

	length = pixel.GetSize();
	char *pixelSource = new char[ length+1 ];
	pixel.ReadData( pixelSource, length );
	pixelSource[ length ] = 0;

	m_bFlags |= AGK_SHADER_IS_CUSTOM;

	if ( strstr( vertexSource, "vec3 GetVSLighting( mediump vec3 normal, highp vec3 pos );" ) != 0 ) 
		m_bFlags |= AGK_SHADER_USES_VS_LIGHTING;
	else
		m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;

	if ( strstr( pixelSource, "vec3 GetPSLighting( mediump vec3 normal, highp vec3 pos );" ) != 0 ) 
		m_bFlags |= AGK_SHADER_USES_PS_LIGHTING;
	else
		m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;

	if ( strstr( pixelSource, "vec3 ApplyFog( mediump vec3 color, highp vec3 pointPos );" ) != 0 ) 
		m_bFlags |= AGK_SHADER_USES_FOG;
	else
		m_bFlags &= ~AGK_SHADER_USES_FOG;

	// if the shader needs lighting then it can't be compiled until it has the lighting section generated
	if ( !NeedsAdditionalCode() )
	{
		SetShaderSource( vertexSource, pixelSource );
	}
	else
	{
		m_sVSSource.SetStr( vertexSource );
		m_sPSSource.SetStr( pixelSource );
		m_bValid = true;
	}

	delete [] vertexSource;
	delete [] pixelSource;

	vertex.Close();
	pixel.Close();
}

void AGKShader::LoadShaderFromString( const char* vertexSource, const char* pixelSource )
{
	m_sVSFilename.SetStr( "UserString" );
	m_sPSFilename.SetStr( "UserString" );

	m_bFlags |= AGK_SHADER_IS_CUSTOM;

	if ( strstr( vertexSource, "vec3 GetVSLighting( mediump vec3 normal, highp vec3 pos );" ) != 0 ) 
		m_bFlags |= AGK_SHADER_USES_VS_LIGHTING;
	else
		m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;

	if ( strstr( pixelSource, "vec3 GetPSLighting( mediump vec3 normal, highp vec3 pos );" ) != 0 ) 
		m_bFlags |= AGK_SHADER_USES_PS_LIGHTING;
	else
		m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;

	if ( strstr( pixelSource, "vec3 ApplyFog( mediump vec3 color, highp vec3 pointPos );" ) != 0 ) 
		m_bFlags |= AGK_SHADER_USES_FOG;
	else
		m_bFlags &= ~AGK_SHADER_USES_FOG;

	// if the shader needs lighting then it can't be compiled until it has the lighting section generated
	if ( !NeedsAdditionalCode() )
	{
		SetShaderSource( vertexSource, pixelSource );
	}
	else
	{
		m_sVSSource.SetStr( vertexSource );
		m_sPSSource.SetStr( pixelSource );
		m_bValid = true;
	}
}

void AGKShader::LoadFullScreenShader( const char* pixelFile )
{
	if ( !pixelFile ) return;

	cFile pixel;
	if ( !pixel.OpenToRead( pixelFile ) )
	{
		uString info;
		info.Format( "Failed to open pixel shader file %s for reading", pixelFile );
		agk::Warning( info.GetStr() );
		return;
	}

	m_sVSFilename.SetStr( "Fullscreen" );
	m_sPSFilename.SetStr( pixelFile );

	char vertexSource[] = "\
	attribute highp vec3 position;\
	varying highp vec2 uvVarying;\
	uniform highp vec4 uvBounds0;\
	uniform mediump float agk_invert;\
	void main() {\
		gl_Position = vec4(position.xy*vec2(1.0,agk_invert),0.5,1.0);\
		uvVarying = (position.xy*vec2(0.5,-0.5) + 0.5) * uvBounds0.xy + uvBounds0.zw;\
	}";
	
	int length = pixel.GetSize();
	char *pixelSource = new char[ length+1 ];
	pixel.ReadData( pixelSource, length );
	pixelSource[ length ] = 0;

	m_bFlags |= AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;

	SetShaderSource( vertexSource, pixelSource );

	delete [] pixelSource;

	pixel.Close();
}

void AGKShader::LoadSpriteShader( const char* pixelFile )
{
	if ( !pixelFile ) return;

	cFile pixel;
	if ( !pixel.OpenToRead( pixelFile ) )
	{
		uString info;
		info.Format( "Failed to open pixel shader file %s for reading", pixelFile );
		agk::Warning( info.GetStr() );
		return;
	}

	m_sVSFilename.SetStr( "Sprite" );
	m_sPSFilename.SetStr( pixelFile );

	char vertexSource[] = "\
	attribute highp vec4 position;\
	attribute mediump vec4 color;\
	attribute highp vec2 uv;\
	varying highp vec2 uvVarying;\
	varying mediump vec4 colorVarying;\
	uniform highp mat4 agk_Ortho;\
	void main() { \
		gl_Position = agk_Ortho * position;\
		uvVarying = uv;\
		colorVarying = color;\
	}";
	
	int length = pixel.GetSize();
	char *pixelSource = new char[ length+1 ];
	pixel.ReadData( pixelSource, length );
	pixelSource[ length ] = 0;

	m_bFlags |= AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;

	SetShaderSource( vertexSource, pixelSource );

	delete [] pixelSource;

	pixel.Close();
}

void AGKShader::SetVideoTextureShader()
{
	m_sVSFilename.SetStr( "Fullscreen" );
	m_sPSFilename.SetStr( "Video Texture" );

	char vertexSource[] = "\
	attribute highp vec3 position;\n\
	varying highp vec2 uvVarying;\n\
	uniform highp vec4 uvBounds;\n\
	uniform mediump float agk_invert;\n\
	void main() {\n\
		gl_Position = vec4(position.xy*vec2(1.0,agk_invert),0.5,1.0);\n\
		uvVarying = (position.xy*vec2(0.5,-0.5) + 0.5) * uvBounds.xy + uvBounds.zw;\n\
	}";
	
	char pixelSource[] = "\
	#extension GL_OES_EGL_image_external : require\n\
	#ifdef GL_ES\n\
	    precision highp float;\n\
	#endif\n\
	uniform samplerExternalOES videoTexture;\n\
	varying highp vec2 uvVarying;\n\
	void main() {\n\
		gl_FragColor = texture2D( videoTexture, uvVarying );\n\
	}";

	m_bFlags |= AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;

	SetShaderSource( vertexSource, pixelSource );
}

void AGKShader::SetCameraTextureShader()
{
	m_sVSFilename.SetStr( "Fullscreen" );
	m_sPSFilename.SetStr( "Camera Texture" );

	char vertexSource[] = "\
	attribute highp vec3 position;\n\
	varying highp vec2 uvVarying;\n\
	uniform highp vec4 orientation;\n\
	uniform mediump float agk_invert;\n\
	void main() {\n\
		gl_Position = vec4(position.xy*vec2(1.0,agk_invert),0.5,1.0);\n\
		highp vec2 uv = position.xy*vec2(0.5,-0.5);\n\
		uvVarying.x = uv.x*orientation.x + uv.y*orientation.y + 0.5;\n\
		uvVarying.y = uv.x*orientation.z + uv.y*orientation.w + 0.5;\n\
	}";
	
	char pixelSource[] = "\
	#extension GL_OES_EGL_image_external : require\n\
	#ifdef GL_ES\n\
	    precision highp float;\n\
	#endif\n\
	uniform samplerExternalOES videoTexture;\n\
	varying highp vec2 uvVarying;\n\
	void main() {\n\
		gl_FragColor = texture2D( videoTexture, uvVarying );\n\
	}";

	m_bFlags |= AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;

	SetShaderSource( vertexSource, pixelSource );

	SetConstantByName( "orientation", 1,0,0,1 );
}

void AGKShader::SetARTextureShader()
{
	m_sVSFilename.SetStr( "Fullscreen" );
	m_sPSFilename.SetStr( "AR Texture" );

	char vertexSource[] = "\
	attribute highp vec3 position;\n\
	varying highp vec2 uvVarying;\n\
	uniform highp vec4 orientation;\n\
	uniform mediump float agk_invert;\n\
	void main() {\n\
		gl_Position = vec4(position.xy*vec2(1.0,agk_invert),0.5,1.0);\n\
		highp vec2 uv = position.xy*vec2(0.5,-0.5);\n\
		uvVarying.x = uv.x*orientation.x + uv.y*orientation.y + 0.5;\n\
		uvVarying.y = uv.x*orientation.z + uv.y*orientation.w + 0.5;\n\
	}";
	
	char pixelSource[] = "\
	#extension GL_OES_EGL_image_external : require\n\
	#ifdef GL_ES\n\
	    precision highp float;\n\
	#endif\n\
	uniform samplerExternalOES videoTexture;\n\
	varying highp vec2 uvVarying;\n\
	void main() {\n\
		gl_FragColor = texture2D( videoTexture, uvVarying );\n\
	}";

	m_bFlags |= AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;

	SetShaderSource( vertexSource, pixelSource );

	SetConstantByName( "orientation", 1,0,0,1 );
}

void AGKShader::SetARTextureShaderiOS()
{
    m_sVSFilename.SetStr( "Fullscreen" );
    m_sPSFilename.SetStr( "AR Texture iOS" );
    
    char vertexSource[] = "\
    attribute highp vec3 position;\n\
    varying highp vec2 uvVarying;\n\
    uniform highp vec4 orientation;\n\
    uniform mediump float agk_invert;\n\
    void main() {\n\
        gl_Position = vec4(position.xy*vec2(1.0,agk_invert),0.5,1.0);\n\
        highp vec2 uv = position.xy*vec2(0.5,-0.5);\n\
        uvVarying.x = uv.x*orientation.x + uv.y*orientation.y + 0.5;\n\
        uvVarying.y = uv.x*orientation.z + uv.y*orientation.w + 0.5;\n\
    }";
    
    char pixelSource[] = "\
    uniform sampler2D texture0;\n\
    uniform sampler2D texture1;\n\
    varying highp vec2 uvVarying;\n\
    void main() {\n\
        highp vec3 yuv = vec3( texture2D(texture0, uvVarying).r, texture2D(texture1, uvVarying).ra );\n\
        lowp float red = yuv.r + yuv.b*1.402 - 0.701;\n\
        lowp float green = yuv.r - yuv.g*0.3441 - yuv.b*0.7141 + 0.5291;\n\
        lowp float blue = yuv.r + yuv.g*1.772 - 0.886;\n\
        gl_FragColor = vec4( red, green, blue, 1.0 );\n\
    }";
    
    m_bFlags |= AGK_SHADER_IS_CUSTOM;
    m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
    m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
    m_bFlags &= ~AGK_SHADER_USES_FOG;
    
    SetShaderSource( vertexSource, pixelSource );
    
    SetConstantByName( "orientation", 1,0,0,1 );
}

const char* AGKShader::GetFirstConstantName()
{
	cShaderUniform *pUniform = m_cUniformList.GetFirst();
	if ( pUniform ) return pUniform->m_sName.GetStr();
	else return 0;
}

const char* AGKShader::GetNextConstantName()
{
	cShaderUniform *pUniform = m_cUniformList.GetNext();
	if ( pUniform ) return pUniform->m_sName.GetStr();
	else return 0;
}

cShaderUniform* AGKShader::GetFirstConstant()
{
	return m_cUniformList.GetFirst();
}

cShaderUniform* AGKShader::GetNextConstant()
{
	return m_cUniformList.GetNext();
}

cShaderAttrib* AGKShader::GetFirstAttribute()
{
	return m_cAttribList.GetFirst();
}

cShaderAttrib* AGKShader::GetNextAttribute()
{
	return m_cAttribList.GetNext();
}

int AGKShader::GetConstantExists( const char* name ) const
{
	if ( m_cUniformList.GetItem( name ) ) return 1;
	else return 0;
}

void AGKShader::SetConstantByName( const char* szName, float f1, float f2, float f3, float f4 )
{
	if ( !szName ) return;

	if ( NeedsAdditionalCode() )
	{
		// change any derived shaders
		AGKShader *pShader = m_cDerivedShaders.GetFirst();
		while( pShader )
		{
			pShader->SetConstantByName( szName, f1, f2, f3, f4 );
			pShader = m_cDerivedShaders.GetNext();
		}

		// save the change for any future derived shaders
		cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
		if ( !pUniform ) 
		{
			pUniform = new cShaderUniform();
			pUniform->m_sName.SetStr( szName );
			pUniform->m_iComponents = 4;
			pUniform->m_iArrayMembers = 0xffffffff; // not an array
			pUniform->m_iType = 0; // not a matrix
			pUniform->m_pValues = new float[ 4 ];
			m_cUniformList.AddItem( pUniform, szName );
		}

		pUniform->m_pValues[ 0 ] = f1;
		pUniform->m_pValues[ 1 ] = f2;
		pUniform->m_pValues[ 2 ] = f3;
		pUniform->m_pValues[ 3 ] = f4;
	}
	else
	{
		// change this shader
		cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
		if ( !pUniform ) return;
		if ( pUniform->m_iType != 0 )
		{
			agk::Error( "Failed to set shader constant - tried to set vector values on a matrix" );
			return;
		}

		// if it's already marked as changed we can skip a few steps
		if ( pUniform->m_bChanged )
		{
			switch( pUniform->m_iComponents )
			{
				// use case cascade to set them all, no breaks
				case 4: pUniform->m_pValues[ 3 ] = f4;
				case 3: pUniform->m_pValues[ 2 ] = f3;
				case 2: pUniform->m_pValues[ 1 ] = f2;
				case 1: pUniform->m_pValues[ 0 ] = f1;
			}
		}
		else
		{
			switch( pUniform->m_iComponents )
			{
				// use case cascade to set them all, no breaks
				case 4: if ( pUniform->m_pValues[ 3 ] != f4 ) pUniform->m_bChanged = true; pUniform->m_pValues[ 3 ] = f4;
				case 3: if ( pUniform->m_pValues[ 2 ] != f3 ) pUniform->m_bChanged = true; pUniform->m_pValues[ 2 ] = f3;
				case 2: if ( pUniform->m_pValues[ 1 ] != f2 ) pUniform->m_bChanged = true; pUniform->m_pValues[ 1 ] = f2;
				case 1: if ( pUniform->m_pValues[ 0 ] != f1 ) pUniform->m_bChanged = true; pUniform->m_pValues[ 0 ] = f1;
			}

			if ( pUniform->m_bChanged )
			{
				// add it to the list of changed uniforms
				pUniform->m_pNextUniform = m_pChangedUniforms;
				m_pChangedUniforms = pUniform;
			}
		}
	}
}

void AGKShader::SetConstantArrayByName( const char* szName, UINT index, float f1, float f2, float f3, float f4 )
{
	if ( !szName ) return;

	if ( NeedsAdditionalCode() )
	{
		// change any derived shaders
		AGKShader *pShader = m_cDerivedShaders.GetFirst();
		while( pShader )
		{
			pShader->SetConstantArrayByName( szName, index, f1, f2, f3, f4 );
			pShader = m_cDerivedShaders.GetNext();
		}

		// save the change for any future derived shaders
		char *szNameArray = new char[ strlen(szName)+15 ];
		sprintf( szNameArray, "%s[%d]", szName, index );
		cShaderUniform *pUniform = m_cUniformList.GetItem( szNameArray );
		if ( !pUniform ) 
		{
			pUniform = new cShaderUniform();
			pUniform->m_sName.SetStr( szNameArray );
			pUniform->m_iComponents = 4;
			pUniform->m_iArrayMembers = index; // store array index
			pUniform->m_iType = 0; // not a matrix
			pUniform->m_pValues = new float[ 4 ];
			m_cUniformList.AddItem( pUniform, szNameArray );
		}

		delete [] szNameArray;

		pUniform->m_pValues[ 0 ] = f1;
		pUniform->m_pValues[ 1 ] = f2;
		pUniform->m_pValues[ 2 ] = f3;
		pUniform->m_pValues[ 3 ] = f4;
	}
	else
	{
		// change this shader
		cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
		if ( !pUniform ) return;
		if ( pUniform->m_iType != 0 )
		{
			agk::Error( "Failed to set shader constant - tried to set vector values on a matrix" );
			return;
		}

		if ( index >= pUniform->m_iArrayMembers ) return;

		UINT offset = pUniform->m_iComponents * index;

		if ( pUniform->m_bChanged )
		{
			switch( pUniform->m_iComponents )
			{
				// use case cascade to set them all, no breaks
				case 4: pUniform->m_pValues[ offset + 3 ] = f4;
				case 3: pUniform->m_pValues[ offset + 2 ] = f3;
				case 2: pUniform->m_pValues[ offset + 1 ] = f2;
				case 1: pUniform->m_pValues[ offset + 0 ] = f1;
			}
		}
		else
		{
			switch( pUniform->m_iComponents )
			{
				// use case cascade to set them all, no breaks
				case 4: if ( pUniform->m_pValues[ offset + 3 ] != f4 ) pUniform->m_bChanged = true; pUniform->m_pValues[ offset + 3 ] = f4;
				case 3: if ( pUniform->m_pValues[ offset + 2 ] != f3 ) pUniform->m_bChanged = true; pUniform->m_pValues[ offset + 2 ] = f3;
				case 2: if ( pUniform->m_pValues[ offset + 1 ] != f2 ) pUniform->m_bChanged = true; pUniform->m_pValues[ offset + 1 ] = f2;
				case 1: if ( pUniform->m_pValues[ offset + 0 ] != f1 ) pUniform->m_bChanged = true; pUniform->m_pValues[ offset + 0 ] = f1;
			}

			if ( pUniform->m_bChanged )
			{
				// add it to the list of changed uniforms
				pUniform->m_pNextUniform = m_pChangedUniforms;
				m_pChangedUniforms = pUniform;
			}
		}
	}
}

void AGKShader::SetConstantMatrixByName( const char* szName, int numValues, const float* values )
{
	if ( !szName ) return;

	if ( NeedsAdditionalCode() )
	{
		// change any derived shaders
		AGKShader *pShader = m_cDerivedShaders.GetFirst();
		while( pShader )
		{
			pShader->SetConstantMatrixByName( szName, numValues, values );
			pShader = m_cDerivedShaders.GetNext();
		}

		// save the change for any future derived shaders
		cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
		if ( !pUniform ) 
		{
			pUniform = new cShaderUniform();
			pUniform->m_sName.SetStr( szName );
			pUniform->m_iComponents = numValues;
			pUniform->m_iArrayMembers = 0xffffffff; // not an array
			pUniform->m_iType = 1; // is a matrix
			pUniform->m_pValues = new float[ numValues ];
			m_cUniformList.AddItem( pUniform, szName );
		}

		for ( int i = 0; i < numValues; i++ ) pUniform->m_pValues[ i ] = values[ i ];
	}
	else
	{
		// change this shader
		cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
		if ( !pUniform ) return;
		if ( pUniform->m_iType != 1 )
		{
			agk::Error( "Failed to set shader constant - tried to set matrix values on a vector" );
			return;
		}

		UINT size = pUniform->m_iComponents*pUniform->m_iComponents;

		// if it's already marked as changed we can skip a few steps
		if ( pUniform->m_bChanged )
		{
			memcpy( pUniform->m_pValues, values, size );
		}
		else
		{
			for( UINT i = 0; i < size; i++ ) 
			{
				if ( pUniform->m_pValues[ i ] != values[ i ] ) 
				{
					pUniform->m_bChanged = true;
					break;
				}
			}

			if ( pUniform->m_bChanged )
			{
				memcpy( pUniform->m_pValues, values, size );

				// add it to the list of changed uniforms
				pUniform->m_pNextUniform = m_pChangedUniforms;
				m_pChangedUniforms = pUniform;
			}
		}
	}
}

void AGKShader::SetConstantMatrixArrayByName( const char* szName, UINT index, int numValues, const float* values )
{
	if ( !szName ) return;

	if ( NeedsAdditionalCode() )
	{
		// change any derived shaders
		AGKShader *pShader = m_cDerivedShaders.GetFirst();
		while( pShader )
		{
			pShader->SetConstantMatrixArrayByName( szName, index, numValues, values );
			pShader = m_cDerivedShaders.GetNext();
		}

		// save the change for any future derived shaders
		char *szNameArray = new char[ strlen(szName)+15 ];
		sprintf( szNameArray, "%s[%d]", szName, index );
		cShaderUniform *pUniform = m_cUniformList.GetItem( szNameArray );
		if ( !pUniform ) 
		{
			pUniform = new cShaderUniform();
			pUniform->m_sName.SetStr( szNameArray );
			pUniform->m_iComponents = numValues;
			pUniform->m_iArrayMembers = index; // store array index
			pUniform->m_iType = 1; // is a matrix
			pUniform->m_pValues = new float[ numValues ];
			m_cUniformList.AddItem( pUniform, szNameArray );
		}
		delete [] szNameArray;

		for ( int i = 0; i < numValues; i++ ) pUniform->m_pValues[ i ] = values[ i ];
	}
	else
	{
		// change this shader
		cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
		if ( !pUniform ) return;
		if ( pUniform->m_iType != 1 )
		{
			agk::Error( "Failed to set shader constant - tried to set matrix values on a vector" );
			return;
		}

		if ( index >= pUniform->m_iArrayMembers ) return;

		UINT size = pUniform->m_iComponents*pUniform->m_iComponents;
		UINT offset = size*index;

		// if it's already marked as changed we can skip a few steps
		if ( pUniform->m_bChanged )
		{
			memcpy( pUniform->m_pValues + offset, values, size );
		}
		else
		{
			for( UINT i = 0; i < size; i++ ) 
			{
				if ( pUniform->m_pValues[ i + offset ] != values[ i ] ) 
				{
					pUniform->m_bChanged = true;
					break;
				}
			}

			if ( pUniform->m_bChanged )
			{
				memcpy( pUniform->m_pValues + offset, values, size );

				// add it to the list of changed uniforms
				pUniform->m_pNextUniform = m_pChangedUniforms;
				m_pChangedUniforms = pUniform;
			}
		}
	}
}

int AGKShader::GetAttribByName( const char* name ) const
{
	cShaderAttrib *pAttrib = m_cAttribList.GetItem( name );
	if ( pAttrib ) return pAttrib->m_iLocation;
	else return -1;
}

void AGKShader::SetTextureStage( cImage *pImage, UINT stage, int useImageUV )
{
	if ( stage >= AGK_MAX_TEXTURES ) return;

	if ( !pImage ) cImage::BindTexture( 0, stage );
	else
	{
		pImage->Bind( stage );

		float newU1 = 0;
		float newV1 = 0;
		float newU2 = 1;
		float newV2 = 1;
		if ( useImageUV ) 
		{
			newU1 = pImage->GetU1();
			newV1 = pImage->GetV1();
			newU2 = pImage->GetU2();
			newV2 = pImage->GetV2();
		}

		if ( m_fTextureU1[stage] != newU1 || m_fTextureV1[stage] != newV1 
			|| m_fTextureU2[stage] != newU2 || m_fTextureV2[stage] != newV2 )
		{
			m_fTextureU1[stage] = newU1;
			m_fTextureV1[stage] = newV1;
			m_fTextureU2[stage] = newU2;
			m_fTextureV2[stage] = newV2;

			m_bTextureBoundsChanged |= (1 << stage);
		}
	}
}

void AGKShader::SetUVScale( UINT stage, float offsetU, float offsetV, float scaleU, float scaleV )
{
	float newU1 = offsetU * scaleU;
	float newV1 = offsetV * scaleV;
	float newU2 = (1 + offsetU) * scaleU;
	float newV2 = (1 + offsetV) * scaleV;

	if ( m_fU1[stage] != newU1 || m_fV1[stage] != newV1 
	  || m_fU2[stage] != newU2 || m_fV2[stage] != newV2 )
	{
		m_fU1[stage] = newU1;
		m_fV1[stage] = newV1;
		m_fU2[stage] = newU2;
		m_fV2[stage] = newV2;

		m_bUVBoundsChanged |= (1 << stage);
	}
}

void AGKShader::MakeColorShader()
{
	// vertex shader
	char srcVertShader[ 1024 ] = "";
	strcat ( srcVertShader, "attribute highp vec4 position;\n" );
	strcat ( srcVertShader, "attribute mediump vec4 color;\n" );
	strcat ( srcVertShader, "" );
	strcat ( srcVertShader, "varying mediump vec4 colorVarying;\n" );
	strcat ( srcVertShader, "" );
	strcat ( srcVertShader, "uniform highp mat4 agk_Ortho;\n" );

	strcat ( srcVertShader, "void main()\n" );
	strcat ( srcVertShader, "{ \n" );
	strcat ( srcVertShader, "	gl_Position = agk_Ortho * position;\n");
	strcat ( srcVertShader, "	colorVarying = color;\n" );
	strcat ( srcVertShader, "}" );

	// fragment shader
	char srcFragShader[ 1024 ] = "";
	strcat ( srcFragShader, "varying mediump vec4 colorVarying;\n" );

	strcat ( srcFragShader, "void main()\n" );
	strcat ( srcFragShader, "{ \n" );
	strcat ( srcFragShader, "	gl_FragColor = colorVarying;\n" );
	strcat ( srcFragShader, "}" );

	m_bFlags &= ~AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;
	m_bFlags |= AGK_SHADER_IS_DEFAULT;

	SetShaderSource( srcVertShader, srcFragShader );
}

void AGKShader::MakeTexColorShader()
{
	// vertex shader
	char srcVertShader[ 1024 ] = "";
	strcat ( srcVertShader, "attribute highp vec4 position;\n" );
	strcat ( srcVertShader, "attribute mediump vec4 color;\n" );
	strcat ( srcVertShader, "attribute highp vec2 uv;\n" );
	
	strcat ( srcVertShader, "" );
	strcat ( srcVertShader, "varying highp vec2 uvVarying;\n" );
	strcat ( srcVertShader, "varying mediump vec4 colorVarying;\n" );
	strcat ( srcVertShader, "" );
	strcat ( srcVertShader, "uniform highp mat4 agk_Ortho;\n" );

	strcat ( srcVertShader, "void main()\n" );
	strcat ( srcVertShader, "{ \n" );
	strcat ( srcVertShader, "	gl_Position = agk_Ortho * position;\n");
	strcat ( srcVertShader, "	uvVarying = uv;\n" );
	strcat ( srcVertShader, "	colorVarying = color;\n" );
	strcat ( srcVertShader, "}" );

	// fragment shader
	char srcFragShader[ 1024 ] = "";
	strcat ( srcFragShader, "uniform sampler2D texture0;\n" );
	strcat ( srcFragShader, "varying highp vec2 uvVarying;\n" );
	strcat ( srcFragShader, "varying mediump vec4 colorVarying;\n" );

	strcat ( srcFragShader, "void main()\n" );
	strcat ( srcFragShader, "{ \n" );
	strcat ( srcFragShader, "	gl_FragColor = texture2D(texture0, uvVarying) * colorVarying;\n" );
	strcat ( srcFragShader, "}" );

	m_bFlags &= ~AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;
	m_bFlags |= AGK_SHADER_IS_DEFAULT;

	SetShaderSource( srcVertShader, srcFragShader );
}

void AGKShader::MakeFontShader()
{
	m_sVSFilename.SetStr( "Sprite" );
	m_sPSFilename.SetStr( "Font Shader" );

	char vertexSource[] = "\
	attribute highp vec4 position;\
	attribute mediump vec4 color;\
	attribute highp vec2 uv;\
	varying highp vec2 uvVarying;\
	varying mediump vec4 colorVarying;\
	uniform highp mat4 agk_Ortho;\
	void main() { \
		gl_Position = agk_Ortho * position;\
		uvVarying = uv;\
		colorVarying = color;\
	}";
	
	char fragmentSource[] = "uniform sampler2D texture0;\
	varying highp vec2 uvVarying;\
	varying mediump vec4 colorVarying;\
	void main()\
	{ \
		gl_FragColor = colorVarying;\
		gl_FragColor.a = gl_FragColor.a * texture2D(texture0, uvVarying).a;\
	}";

	m_bFlags &= ~AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;
	m_bFlags |= AGK_SHADER_IS_DEFAULT;

	SetShaderSource( vertexSource, fragmentSource );
}

void AGKShader::MakeShadowShader( int maxBones, int alphamask )
{
	if ( g_pCurrentShader == this ) NoShader();
	PlatformDelete();
	PlatformInit();

	// vertex shader
	char srcVertShader[ 4096 ] = "";
	strcat ( srcVertShader, "attribute highp vec3 position;\n" );
	if ( alphamask ) 
	{
		strcat ( srcVertShader, "attribute highp vec2 uv;\n" );
		strcat ( srcVertShader, "varying highp vec2 uvVarying;\n" );
		strcat ( srcVertShader, "uniform highp vec4 uvBounds0;\n" );
		strcat ( srcVertShader, "uniform highp vec4 textureBounds0;\n" );
	}

	if ( maxBones == 0 ) strcat ( srcVertShader, "uniform highp mat4 agk_World;\n" );
	strcat ( srcVertShader, "uniform highp mat4 agk_ShadowProj;\n" );

	if ( maxBones > 0 )
	{
		char szBoneCount[ 10 ];
		sprintf( szBoneCount, "%d", maxBones );
		if ( alphamask ) g_iNumShadowBonesAlpha = maxBones;
		else g_iNumShadowBones = maxBones;

		strcat ( srcVertShader, "attribute highp vec4 boneweights;\n" );
		strcat ( srcVertShader, "attribute mediump vec4 boneindices;\n" );
		strcat ( srcVertShader, "uniform highp vec4 agk_bonequats1["); strcat(srcVertShader,szBoneCount); strcat(srcVertShader,"];\n" );
		strcat ( srcVertShader, "uniform highp vec4 agk_bonequats2["); strcat(srcVertShader,szBoneCount); strcat(srcVertShader,"];\n" );
		strcat ( srcVertShader, "\n");
		strcat ( srcVertShader, "highp vec3 transformDQ( highp vec3 p, highp vec4 q1, highp vec4 q2 )\n" );
		strcat ( srcVertShader, "{\n" );
		strcat ( srcVertShader, "    p += 2.0 * cross( q1.xyz, cross(q1.xyz, p) + q1.w*p );\n" );
		strcat ( srcVertShader, "    p += 2.0 * (q1.w*q2.xyz - q2.w*q1.xyz + cross(q1.xyz,q2.xyz));\n" );
		strcat ( srcVertShader, "    return p;\n" );
		strcat ( srcVertShader, "}\n" );
		strcat ( srcVertShader, "\n");
	}

	strcat ( srcVertShader, "void main()\n" );
	strcat ( srcVertShader, "{ \n" );
	if ( maxBones == 0 ) strcat ( srcVertShader, "    highp vec4 pos = agk_World * vec4(position,1.0);\n");
	else
	{
		strcat ( srcVertShader, "    highp vec4 q1 = agk_bonequats1[ int(boneindices.x) ] * boneweights.x;\n" );
		strcat ( srcVertShader, "    q1 += agk_bonequats1[ int(boneindices.y) ] * boneweights.y;\n" );
		strcat ( srcVertShader, "    q1 += agk_bonequats1[ int(boneindices.z) ] * boneweights.z;\n" );
		strcat ( srcVertShader, "    q1 += agk_bonequats1[ int(boneindices.w) ] * boneweights.w;\n" );

		strcat ( srcVertShader, "    highp vec4 q2 = agk_bonequats2[ int(boneindices.x) ] * boneweights.x;\n" );
		strcat ( srcVertShader, "    q2 += agk_bonequats2[ int(boneindices.y) ] * boneweights.y;\n" );
		strcat ( srcVertShader, "    q2 += agk_bonequats2[ int(boneindices.z) ] * boneweights.z;\n" );
		strcat ( srcVertShader, "    q2 += agk_bonequats2[ int(boneindices.w) ] * boneweights.w;\n" );

		strcat ( srcVertShader, "    highp float len = 1.0/length(q1);\n" );
		strcat ( srcVertShader, "    q1 *= len;\n" );
		strcat ( srcVertShader, "    q2 = (q2 - q1*dot(q1,q2)) * len;\n" );

		strcat ( srcVertShader, "    highp vec4 pos = vec4( transformDQ(position,q1,q2), 1.0 );\n" );
	}
	strcat ( srcVertShader, "    pos = agk_ShadowProj * pos;\n");
	strcat ( srcVertShader, "    gl_Position = pos;\n");
	if( alphamask ) 
	{
		strcat ( srcVertShader, "    uvVarying = uv * uvBounds0.xy + uvBounds0.zw;\n" );
		strcat ( srcVertShader, "    uvVarying = uvVarying * textureBounds0.xy + textureBounds0.zw;\n" );
	}
	strcat ( srcVertShader, "}" );

	// fragment shader
	char srcFragShader[ 1024 ] = "";
	if ( alphamask ) 
	{
		strcat ( srcFragShader, "uniform sampler2D texture0;\n" );
		strcat ( srcFragShader, "varying highp vec2 uvVarying;\n" );
		strcat ( srcFragShader, "void main()\n" );
		strcat ( srcFragShader, "{ \n" );
		strcat ( srcFragShader, "    mediump float alpha = texture2D(texture0, uvVarying).a;\n" );
		strcat ( srcFragShader, "    if ( alpha < 0.5 ) discard;\n" );
		strcat ( srcFragShader, "}" );
	}
	else
	{
		strcat ( srcFragShader, "void main(){}\n" );
	}

	m_bFlags |= AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;
	m_bFlags |= AGK_SHADER_IS_DEFAULT;

	SetShaderSource( srcVertShader, srcFragShader );
}

void AGKShader::Make3DParticlesTexShader()
{
	// vertex shader
	char srcVertShader[ 1024 ] = "";
	strcat ( srcVertShader, "attribute highp vec4 position;\n" );
	strcat ( srcVertShader, "attribute mediump vec4 color;\n" );
	strcat ( srcVertShader, "attribute highp vec2 uv;\n" );
	
	strcat ( srcVertShader, "" );
	strcat ( srcVertShader, "varying highp vec2 uvVarying;\n" );
	strcat ( srcVertShader, "varying mediump vec4 colorVarying;\n" );
	strcat ( srcVertShader, "" );
	strcat ( srcVertShader, "uniform highp mat4 agk_ViewProj;\n" );

	strcat ( srcVertShader, "void main()\n" );
	strcat ( srcVertShader, "{ \n" );
	strcat ( srcVertShader, "	gl_Position = agk_ViewProj * position;\n");
	strcat ( srcVertShader, "	uvVarying = uv;\n" );
	strcat ( srcVertShader, "	colorVarying = color;\n" );
	strcat ( srcVertShader, "}" );

	// fragment shader
	char srcFragShader[ 1024 ] = "";
	strcat ( srcFragShader, "uniform sampler2D texture0;\n" );
	strcat ( srcFragShader, "varying highp vec2 uvVarying;\n" );
	strcat ( srcFragShader, "varying mediump vec4 colorVarying;\n" );

	strcat ( srcFragShader, "void main()\n" );
	strcat ( srcFragShader, "{ \n" );
	strcat ( srcFragShader, "	gl_FragColor = texture2D(texture0, uvVarying) * colorVarying;\n" );
	strcat ( srcFragShader, "}" );

	m_bFlags &= ~AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;
	m_bFlags |= AGK_SHADER_IS_DEFAULT;

	SetShaderSource( srcVertShader, srcFragShader );
}

void AGKShader::Make3DParticlesColorShader()
{
	// vertex shader
	char srcVertShader[ 1024 ] = "";
	strcat ( srcVertShader, "attribute highp vec4 position;\n" );
	strcat ( srcVertShader, "attribute mediump vec4 color;\n" );
	
	strcat ( srcVertShader, "" );
	strcat ( srcVertShader, "varying mediump vec4 colorVarying;\n" );
	strcat ( srcVertShader, "" );
	strcat ( srcVertShader, "uniform highp mat4 agk_ViewProj;\n" );

	strcat ( srcVertShader, "void main()\n" );
	strcat ( srcVertShader, "{ \n" );
	strcat ( srcVertShader, "	gl_Position = agk_ViewProj * position;\n");
	strcat ( srcVertShader, "	colorVarying = color;\n" );
	strcat ( srcVertShader, "}" );

	// fragment shader
	char srcFragShader[ 1024 ] = "";
	strcat ( srcFragShader, "varying mediump vec4 colorVarying;\n" );

	strcat ( srcFragShader, "void main()\n" );
	strcat ( srcFragShader, "{ \n" );
	strcat ( srcFragShader, "	gl_FragColor = colorVarying;\n" );
	strcat ( srcFragShader, "}" );

	m_bFlags &= ~AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;
	m_bFlags |= AGK_SHADER_IS_DEFAULT;

	SetShaderSource( srcVertShader, srcFragShader );
}

// Fullscreen quad shader
void AGKShader::MakeQuadShader()
{
	// vertex shader
	char srcVertShader[ 1024 ] = "";
	strcat ( srcVertShader, "attribute highp vec3 position;\n" );
	strcat ( srcVertShader, "varying highp vec2 uvVarying;\n" );
	strcat ( srcVertShader, "uniform highp vec4 uvBounds0;\n" );
	strcat ( srcVertShader, "uniform highp vec4 textureBounds0;\n" );
	strcat ( srcVertShader, "uniform mediump float agk_invert;\n" );
	
	strcat ( srcVertShader, "void main()\n" );
	strcat ( srcVertShader, "{ \n" );
	strcat ( srcVertShader, "	gl_Position = vec4(position.xy*vec2(1,agk_invert),0.5,1.0);\n");
	strcat ( srcVertShader, "	uvVarying = (position.xy*vec2(0.5,-0.5) + 0.5) * uvBounds0.xy + uvBounds0.zw;\n");
	strcat ( srcVertShader, "	uvVarying = uvVarying * textureBounds0.xy + textureBounds0.zw;\n");
	strcat ( srcVertShader, "}" );

	// fragment shader
	char srcFragShader[ 1024 ] = "";
	strcat ( srcFragShader, "uniform sampler2D texture0;\n" );
	strcat ( srcFragShader, "varying highp vec2 uvVarying;\n" );

	strcat ( srcFragShader, "void main()\n" );
	strcat ( srcFragShader, "{ \n" );
	strcat ( srcFragShader, "	gl_FragColor = texture2D(texture0, uvVarying);\n" );
	strcat ( srcFragShader, "}" );

	m_bFlags |= AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;
	m_bFlags |= AGK_SHADER_IS_DEFAULT;

	SetShaderSource( srcVertShader, srcFragShader );
}

void AGKShader::MakeSkyBoxSunShader()
{
	// vertex shader
	char srcVertShader[ 1024 ] = "";
	strcat ( srcVertShader, "attribute vec3 position;\n" );

	strcat ( srcVertShader, "varying highp vec2 uvVarying;\n" );
	strcat ( srcVertShader, "varying highp vec2 uvVarying2;\n" );
	strcat ( srcVertShader, "varying highp vec2 horizonVarying;\n" );

	strcat ( srcVertShader, "uniform highp mat4 agk_World;\n" );
	strcat ( srcVertShader, "uniform highp mat4 agk_ViewProj;\n" );

	strcat ( srcVertShader, "uniform highp vec2 sunSize;\n" );
	strcat ( srcVertShader, "uniform highp float horizonHeight;\n" );
	strcat ( srcVertShader, "uniform highp float objectScale;\n" ); // 1.0 / objectScaleY

	strcat ( srcVertShader, "void main()\n" );
	strcat ( srcVertShader, "{ \n" );
	strcat ( srcVertShader, "    highp vec4 pos = agk_World * vec4(position,1.0);\n");
	strcat ( srcVertShader, "    gl_Position = agk_ViewProj * pos;\n");
	
	strcat ( srcVertShader, "    horizonVarying.x = (pos.y-horizonHeight)*objectScale;\n");
	strcat ( srcVertShader, "    horizonVarying.y = step(position.y,0.0);\n");

	strcat ( srcVertShader, "    uvVarying = position.xz*sunSize.x + 0.5;\n");
	strcat ( srcVertShader, "    uvVarying2 = position.xz*sunSize.y + 0.5;\n");
	strcat ( srcVertShader, "}" );

	// fragment shader
	char srcFragShader[ 1024 ] = "";
	strcat ( srcFragShader, "uniform sampler2D texture0;\n" );

	strcat ( srcFragShader, "uniform mediump vec3 skyColor;\n" );
	strcat ( srcFragShader, "uniform mediump vec3 horizonColor;\n" );
	strcat ( srcFragShader, "uniform mediump vec3 sunColor;\n" );
	strcat ( srcFragShader, "uniform highp float horizonSize;\n" );

	strcat ( srcFragShader, "varying highp vec2 uvVarying;\n" );
	strcat ( srcFragShader, "varying highp vec2 uvVarying2;\n" );
	strcat ( srcFragShader, "varying highp vec2 horizonVarying;\n" );

	strcat ( srcFragShader, "void main()\n" );
	strcat ( srcFragShader, "{ \n" );
	strcat ( srcFragShader, "    highp float horizon = 1.0 - min( horizonSize*horizonVarying.x, 1.0 );\n" );
	strcat ( srcFragShader, "    horizon *= horizon;\n" );
	strcat ( srcFragShader, "    mediump vec3 color = mix( skyColor, horizonColor, horizon );\n" );

	strcat ( srcFragShader, "    mediump vec3 sunColor2 = sunColor*1.5 - color;\n" );
	strcat ( srcFragShader, "    sunColor2 *= horizonVarying.y;\n" );

	strcat ( srcFragShader, "    highp float sunPoint = texture2D(texture0,uvVarying).r;\n" );
	strcat ( srcFragShader, "    color += sunColor2 * sunPoint*sunPoint;\n" );

	strcat ( srcFragShader, "    sunPoint = texture2D(texture0,uvVarying2).r;\n" );
	strcat ( srcFragShader, "    color += 0.2 * sunColor2 * sunPoint;\n" );

	strcat ( srcFragShader, "    gl_FragColor = vec4(color,1.0);\n" );
	strcat ( srcFragShader, "}" );

	m_bFlags |= AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;
	m_bFlags |= AGK_SHADER_IS_DEFAULT;

	SetShaderSource( srcVertShader, srcFragShader );
}

void AGKShader::MakeSkyBoxNoSunShader()
{
	// vertex shader
	char srcVertShader[ 1024 ] = "";
	strcat ( srcVertShader, "attribute highp vec3 position;\n" );

	strcat ( srcVertShader, "varying highp vec2 horizonVarying;\n" );

	strcat ( srcVertShader, "uniform highp mat4 agk_World;\n" );
	strcat ( srcVertShader, "uniform highp mat4 agk_ViewProj;\n" );

	strcat ( srcVertShader, "uniform highp float horizonHeight;\n" );
	strcat ( srcVertShader, "uniform highp float objectScale;\n" ); // 1.0 / objectScaleY

	strcat ( srcVertShader, "void main()\n" );
	strcat ( srcVertShader, "{ \n" );
	strcat ( srcVertShader, "    highp vec4 pos = agk_World * vec4(position,1.0);\n");
	strcat ( srcVertShader, "    gl_Position = agk_ViewProj * pos;\n");
	
	strcat ( srcVertShader, "    horizonVarying.x = (pos.y-horizonHeight)*objectScale;\n");
	strcat ( srcVertShader, "}" );

	// fragment shader
	char srcFragShader[ 1024 ] = "";
	strcat ( srcFragShader, "uniform mediump vec3 skyColor;\n" );
	strcat ( srcFragShader, "uniform mediump vec3 horizonColor;\n" );
	strcat ( srcFragShader, "uniform highp float horizonSize;\n" ); // should be negative

	strcat ( srcFragShader, "varying highp vec2 horizonVarying;\n" );

	strcat ( srcFragShader, "void main()\n" );
	strcat ( srcFragShader, "{ \n" );
	strcat ( srcFragShader, "    highp float horizon = 1.0 - min( horizonSize*horizonVarying.x, 1.0 );\n" );
	strcat ( srcFragShader, "    horizon *= horizon;\n" );
	strcat ( srcFragShader, "    mediump vec3 color = mix( skyColor, horizonColor, horizon );\n" );
	strcat ( srcFragShader, "    gl_FragColor = vec4(color,1.0);\n" );
	strcat ( srcFragShader, "}" );

	m_bFlags |= AGK_SHADER_IS_CUSTOM;
	m_bFlags &= ~AGK_SHADER_USES_VS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_PS_LIGHTING;
	m_bFlags &= ~AGK_SHADER_USES_FOG;
	m_bFlags |= AGK_SHADER_IS_DEFAULT;

	SetShaderSource( srcVertShader, srcFragShader );
}

UINT AGKShader::GetMeshShaderHash( cMesh *pMesh )
{
	// hash must never be 0 or it will match an uninitialised shader
	UINT hash = 1;
	if ( pMesh->HasNormals() && pMesh->WantsLighting() )	
	{
		hash |= 0x0002;
		if ( pMesh->HasNormalMap() ) 
		{
			hash |= 0x10000;
			if ( pMesh->GetImage(2)->HasUVBounds() ) hash |= 0x400000;
		}
	}
	if ( pMesh->HasUVs() )			hash |= 0x0004; 
	if ( pMesh->HasUV1s() )			hash |= 0x0008;
	if ( pMesh->HasValidBones() )	hash |= 0x0010;
	if ( pMesh->HasLightMap() )		hash |= 0x0020;
	if ( pMesh->GetObject() && pMesh->GetObject()->HasAlphaMask() ) hash |= 0x0040;
	if ( pMesh->WantsFog() && agk::GetFogMode() )	hash |= 0x0080;

	if ( pMesh->HasValidBones() )
	{
		hash |= (pMesh->GetNumBones() & 0xff) << 8;
	}

	if ( pMesh->HasVertColors() )	hash |= 0x20000;
	if ( pMesh->HasTexture0() )		
	{
		hash |= 0x40000; 
		if ( pMesh->GetImage(0)->HasUVBounds() ) hash |= 0x80000;
	}
	if ( pMesh->HasTexture1() )		
	{
		hash |= 0x100000;
		if ( pMesh->GetImage(1)->HasUVBounds() ) hash |= 0x200000;
	}

	return hash;
}

UINT AGKShader::GetFinalShaderHash( int sunActive, int VSLights, int PSLights, int useShadows )
{
	if ( sunActive == 0 ) useShadows = 0;
	if ( agk::GetShadowMappingMode() == 0 ) useShadows = 0;

	// hash must never be 0 or it will match an uninitialised shader
	UINT hash = ((VSLights+1) & 0xff) | ((PSLights & 0xff) << 8);
	if ( agk::GetFogMode() ) hash |= 0x00010000;
	if ( agk::GetFogColorsEqual() ) hash |= 0x00020000;
	if ( useShadows )
	{
		if ( agk::GetShadowMappingMode() == 1 ) hash |= 0x00040000; // Uniform shadows
		else if ( agk::GetShadowMappingMode() == 2 ) hash |= 0x00080000; // LiPSM shadows
		else if ( agk::GetShadowMappingMode() == 3 ) hash |= 0x000C0000; // Cascade shadows

		if ( agk::GetShadowSmoothing() == 1 ) hash |= 0x00100000; // multisampled shadows
		else if ( agk::GetShadowSmoothing() == 2 ) hash |= 0x00200000; // random multisampled shadows
	}
	
	return hash;
}

AGKShader* AGKShader::Make3DShader( cMesh *pMesh )
{
	UINT hash = GetMeshShaderHash( pMesh );

	AGKShader *pShader = g_pAllShaders;
	while( pShader )
	{
		if ( pShader->m_pBaseShader == 0 && pShader->m_iShaderHash == hash ) return pShader;
		pShader = pShader->m_pNextShader;
	}

	// not found in cache, generate a new one
	pShader = new AGKShader();
	pShader->m_iShaderHash = hash;

	// Vertex shader
	char srcVertShader[ 4096 ] = "";
	strcat ( srcVertShader, "attribute highp vec3 position;\n" );
	if ( pMesh->HasNormals() && pMesh->WantsLighting() ) 
	{
		pShader->m_bFlags |= AGK_SHADER_USES_VS_LIGHTING;

		strcat ( srcVertShader, "attribute mediump vec3 normal;\n" );
		strcat ( srcVertShader, "varying highp vec3 posVarying;\n" );
		strcat ( srcVertShader, "varying mediump vec3 normalVarying;\n" );
		strcat ( srcVertShader, "varying mediump vec3 lightVarying;\n" );
		strcat ( srcVertShader, "mediump vec3 GetVSLighting( mediump vec3 normal, highp vec3 pos );\n" );
		strcat ( srcVertShader, "\n");
		if ( !pMesh->HasValidBones() )
		{
			strcat ( srcVertShader, "uniform highp mat3 agk_WorldNormal;\n" );
			strcat ( srcVertShader, "uniform highp mat4 agk_World;\n" );
		}
		strcat ( srcVertShader, "uniform highp mat4 agk_ViewProj;\n" );

		if ( pMesh->HasNormalMap() )
		{
			if ( pMesh->HasTangents() && pMesh->HasBiNormals() )
			{
				strcat ( srcVertShader, "attribute mediump vec3 tangent;\n" );
				strcat ( srcVertShader, "attribute mediump vec3 binormal;\n" );
			}
			strcat ( srcVertShader, "varying mediump vec3 tangentVarying;\n" );
			strcat ( srcVertShader, "varying mediump vec3 binormalVarying;\n" );
		}
	}
	else
	{
		strcat ( srcVertShader, "\n");
		if ( pMesh->WantsFog() && agk::GetFogMode() ) strcat ( srcVertShader, "varying highp vec3 posVarying;\n" );
		if ( pMesh->HasValidBones() )
		{
			strcat ( srcVertShader, "uniform highp mat4 agk_ViewProj;\n" );
		}
		else
		{
			if ( pMesh->WantsFog() && agk::GetFogMode() )
			{
				strcat ( srcVertShader, "uniform highp mat4 agk_World;\n" );
				strcat ( srcVertShader, "uniform highp mat4 agk_ViewProj;\n" );
			}
			else
			{
				strcat ( srcVertShader, "uniform highp mat4 agk_WorldViewProj;\n" );
			}
		}
	}

	if ( pMesh->HasUVs() ) 
	{
		strcat ( srcVertShader, "attribute highp vec2 uv;\n" );
		strcat ( srcVertShader, "varying highp vec2 uvVarying;\n" );
		strcat ( srcVertShader, "uniform highp vec4 uvBounds0;\n" );
		strcat ( srcVertShader, "\n");
	}
	
	if ( pMesh->HasUV1s() )
	{
		strcat ( srcVertShader, "attribute highp vec2 uv1;\n" );
		strcat ( srcVertShader, "varying highp vec2 uv1Varying;\n" );
		strcat ( srcVertShader, "uniform highp vec4 uvBounds1;\n" );
		strcat ( srcVertShader, "\n");
	}

	if ( pMesh->HasVertColors() ) 
	{
		strcat ( srcVertShader, "attribute mediump vec4 color;\n" );
		strcat ( srcVertShader, "varying mediump vec4 colorVarying;\n" );
		strcat ( srcVertShader, "\n");
	}

	if ( pMesh->HasValidBones() )
	{
		char szBoneCount[ 10 ];
		sprintf( szBoneCount, "%d", pMesh->GetNumBones() );

		strcat ( srcVertShader, "attribute highp vec4 boneweights;\n" );
		strcat ( srcVertShader, "attribute mediump vec4 boneindices;\n" );
		strcat ( srcVertShader, "uniform highp vec4 agk_bonequats1["); strcat(srcVertShader,szBoneCount); strcat(srcVertShader,"];\n" );
		strcat ( srcVertShader, "uniform highp vec4 agk_bonequats2["); strcat(srcVertShader,szBoneCount); strcat(srcVertShader,"];\n" );
		strcat ( srcVertShader, "\n");
		strcat ( srcVertShader, "highp vec3 transformDQ( highp vec3 p, highp vec4 q1, highp vec4 q2 )\n" );
		strcat ( srcVertShader, "{\n" );
		strcat ( srcVertShader, "    p += 2.0 * cross( q1.xyz, cross(q1.xyz, p) + q1.w*p );\n" );
		strcat ( srcVertShader, "    p += 2.0 * (q1.w*q2.xyz - q2.w*q1.xyz + cross(q1.xyz,q2.xyz));\n" );
		strcat ( srcVertShader, "    return p;\n" );
		strcat ( srcVertShader, "}\n" );
		strcat ( srcVertShader, "\n");
	}

	strcat ( srcVertShader, "void main()\n" );
	strcat ( srcVertShader, "{ \n" );
	if ( pMesh->HasUVs() ) 
	{
		strcat ( srcVertShader, "    uvVarying = uv * uvBounds0.xy + uvBounds0.zw;\n" );
	}
	if ( pMesh->HasUV1s() ) 
	{
		strcat ( srcVertShader, "    uv1Varying = uv1 * uvBounds1.xy + uvBounds1.zw;\n" );
	}
	if ( pMesh->HasVertColors() ) 
	{
		strcat ( srcVertShader, "    colorVarying = color;\n" );
	}

	if ( pMesh->HasNormals() && pMesh->WantsLighting() && pMesh->HasNormalMap() )
	{
		if ( !pMesh->HasTangents() || !pMesh->HasBiNormals() )
		{
			strcat ( srcVertShader, "    mediump vec3 tangent;\n" );
			strcat ( srcVertShader, "    if ( abs(normal.y) > 0.999 ) tangent = vec3( normal.y,0.0,0.0 );\n" );
			strcat ( srcVertShader, "    else tangent = normalize( vec3(-normal.z, 0.0, normal.x) );\n" );
			strcat ( srcVertShader, "    mediump vec3 binormal = normalize( vec3(normal.y*tangent.z, normal.z*tangent.x-normal.x*tangent.z, -normal.y*tangent.x) );\n" );
		}
	}

	if ( pMesh->HasValidBones() )
	{
		strcat ( srcVertShader, "    highp vec4 q1 = agk_bonequats1[ int(boneindices.x) ] * boneweights.x;\n" );
		strcat ( srcVertShader, "    q1 += agk_bonequats1[ int(boneindices.y) ] * boneweights.y;\n" );
		strcat ( srcVertShader, "    q1 += agk_bonequats1[ int(boneindices.z) ] * boneweights.z;\n" );
		strcat ( srcVertShader, "    q1 += agk_bonequats1[ int(boneindices.w) ] * boneweights.w;\n" );
		
		strcat ( srcVertShader, "    highp vec4 q2 = agk_bonequats2[ int(boneindices.x) ] * boneweights.x;\n" );
		strcat ( srcVertShader, "    q2 += agk_bonequats2[ int(boneindices.y) ] * boneweights.y;\n" );
		strcat ( srcVertShader, "    q2 += agk_bonequats2[ int(boneindices.z) ] * boneweights.z;\n" );
		strcat ( srcVertShader, "    q2 += agk_bonequats2[ int(boneindices.w) ] * boneweights.w;\n" );
		
		strcat ( srcVertShader, "    highp float len = 1.0/length(q1);\n" );
		strcat ( srcVertShader, "    q1 *= len;\n" );
		//strcat ( srcVertShader, "    q2 *= len;\n" ); // quicker but less accurate?
		strcat ( srcVertShader, "    q2 = (q2 - q1*dot(q1,q2)) * len;\n" );
		
		strcat ( srcVertShader, "    highp vec4 pos = vec4( transformDQ(position,q1,q2), 1.0 );\n" );
		strcat ( srcVertShader, "    gl_Position = agk_ViewProj * pos;\n");
		if ( pMesh->HasNormals() && pMesh->WantsLighting() )
		{
			strcat ( srcVertShader, "\n");
			strcat ( srcVertShader, "    normalVarying = normal + 2.0*cross( q1.xyz, cross(q1.xyz,normal) + q1.w*normal );\n");
			strcat ( srcVertShader, "    posVarying = pos.xyz;\n");
			strcat ( srcVertShader, "    lightVarying = GetVSLighting( normalVarying, posVarying );\n");
			if ( pMesh->HasNormalMap() )
			{
				strcat ( srcVertShader, "    tangentVarying = tangent + 2.0*cross( q1.xyz, cross(q1.xyz,tangent) + q1.w*tangent );\n");
				strcat ( srcVertShader, "    binormalVarying = binormal + 2.0*cross( q1.xyz, cross(q1.xyz,binormal) + q1.w*binormal );\n");
			}
		}
		else if ( pMesh->WantsFog() && agk::GetFogMode() )
		{
			strcat ( srcVertShader, "    posVarying = pos.xyz;\n");
		}
	}
	else
	{
		if ( pMesh->HasNormals() && pMesh->WantsLighting() )
		{
			strcat ( srcVertShader, "    highp vec4 pos = agk_World * vec4(position,1.0);\n");
			strcat ( srcVertShader, "    gl_Position = agk_ViewProj * pos;\n");
			strcat ( srcVertShader, "    mediump vec3 norm = normalize(agk_WorldNormal * normal);\n");
			strcat ( srcVertShader, "    posVarying = pos.xyz;\n");
			strcat ( srcVertShader, "    normalVarying = norm;\n");
			strcat ( srcVertShader, "    lightVarying = GetVSLighting( norm, posVarying );\n");
			if ( pMesh->HasNormalMap() )
			{
				strcat ( srcVertShader, "    tangentVarying = normalize(agk_WorldNormal * tangent);\n");
				strcat ( srcVertShader, "    binormalVarying = normalize(agk_WorldNormal * binormal);\n");
			}
		}
		else if ( pMesh->WantsFog() && agk::GetFogMode() )
		{
			strcat ( srcVertShader, "    highp vec4 pos = agk_World * vec4(position,1.0);\n");
			strcat ( srcVertShader, "    gl_Position = agk_ViewProj * pos;\n");
			strcat ( srcVertShader, "    posVarying = pos.xyz;\n");
		}
		else
		{
			strcat ( srcVertShader, "    gl_Position = agk_WorldViewProj * vec4(position,1.0);\n");
		}
	}
	
	strcat ( srcVertShader, "}\n" );

	// Pixel shader
	char srcFragShader[ 4096 ] = "";
	if ( pMesh->HasUVs() )
	{
		strcat ( srcFragShader, "varying highp vec2 uvVarying;\n" );
		if ( pMesh->HasTexture0() )
		{
			strcat ( srcFragShader, "uniform sampler2D texture0;\n" );
			if ( pMesh->GetImage(0)->HasUVBounds() ) strcat ( srcFragShader, "uniform highp vec4 textureBounds0;\n" );
		}
	}

	if ( (pMesh->HasTexture1() || pMesh->HasNormalMap()) && pMesh->HasUV1s() )
	{
		strcat ( srcFragShader, "varying highp vec2 uv1Varying;\n" );
	}

	if ( pMesh->HasTexture1() )
	{
		strcat ( srcFragShader, "uniform sampler2D texture1;\n" );
		if ( pMesh->GetImage(1)->HasUVBounds() ) strcat ( srcFragShader, "uniform highp vec4 textureBounds1;\n" );
	}

	if ( pMesh->HasVertColors() )
	{
		strcat ( srcFragShader, "varying highp vec4 colorVarying;\n" );
	}

	if ( pMesh->HasNormals() && pMesh->WantsLighting() )
	{
		pShader->m_bFlags |= AGK_SHADER_USES_PS_LIGHTING;

		strcat ( srcFragShader, "varying mediump vec3 normalVarying;\n" );
		strcat ( srcFragShader, "varying mediump vec3 lightVarying;\n" );
		strcat ( srcFragShader, "varying highp vec3 posVarying;\n" );
		strcat ( srcFragShader, "mediump vec3 GetPSLighting( mediump vec3 normal, highp vec3 pos );\n" );

		if ( pMesh->HasNormalMap() )
		{
			strcat ( srcFragShader, "uniform sampler2D texture2;\n" );
			if ( pMesh->GetImage(2)->HasUVBounds() ) strcat ( srcFragShader, "uniform highp vec4 textureBounds2;\n" );
			strcat ( srcFragShader, "varying mediump vec3 tangentVarying;\n" );
			strcat ( srcFragShader, "varying mediump vec3 binormalVarying;\n" );
			strcat ( srcFragShader, "uniform mediump vec2 agk_NormalScale;\n" );
		}
	}
	
	if ( pMesh->WantsFog() && agk::GetFogMode() )
	{
		pShader->m_bFlags |= AGK_SHADER_USES_FOG;

		if ( pShader->NeedsPSLighting() == 0 ) strcat ( srcFragShader, "varying highp vec3 posVarying;\n" );
		strcat ( srcFragShader, "mediump vec3 ApplyFog( mediump vec3 color, highp vec3 pointPos );\n" );
	}

	strcat ( srcFragShader, "uniform mediump vec4 agk_MeshDiffuse;\n" );
	strcat ( srcFragShader, "uniform mediump vec4 agk_MeshEmissive;\n" );

	strcat ( srcFragShader, "void main()\n" );
	strcat ( srcFragShader, "{ \n" );
	if ( pMesh->HasVertColors() )
	{
		strcat ( srcFragShader, "    mediump vec4 blendTex = colorVarying;\n" );
	}
	else
	{
		strcat ( srcFragShader, "    mediump vec4 blendTex = vec4(1.0,1.0,1.0,1.0);\n" );
	}
	
	int hasLight = 0;
	if ( pMesh->HasNormals() && pMesh->WantsLighting() )
	{
		strcat ( srcFragShader, "    mediump vec3 norm = normalize(normalVarying);\n" );

		if ( pMesh->HasNormalMap() )
		{
			// if there is a separate UV channel, and it isn't being used by texture 1, then use it for normal mapping, otherwise use the base UV
			if ( pMesh->HasUV1s() && !pMesh->HasTexture1() )
			{
				if ( !pMesh->GetImage(2)->HasUVBounds() ) strcat ( srcFragShader, "    mediump vec2 texture2UV = uv1Varying;\n" );
				else strcat ( srcFragShader, "    mediump vec2 texture2UV = uv1Varying*textureBounds2.xy + textureBounds2.zw;\n" );
			}
			else
			{
				if ( !pMesh->GetImage(2)->HasUVBounds() ) strcat ( srcFragShader, "    mediump vec2 texture2UV = uvVarying;\n" );
				else strcat ( srcFragShader, "    mediump vec2 texture2UV = uvVarying*textureBounds2.xy + textureBounds2.zw;\n" );
			}

			strcat ( srcFragShader, "    mediump vec3 normalmap = texture2D(texture2, texture2UV*agk_NormalScale).xyz;\n" );
			strcat ( srcFragShader, "    normalmap = normalmap * 2.0 - 1.0;\n" );

			strcat ( srcFragShader, "    mediump vec3 tangent = normalize(tangentVarying);\n" );
			strcat ( srcFragShader, "    mediump vec3 binormal = normalize(binormalVarying);\n" );
			strcat ( srcFragShader, "    mediump mat3 TBN = mat3( tangent, binormal, norm );\n" );
			strcat ( srcFragShader, "    norm = TBN * normalmap;\n" );
		}

		strcat ( srcFragShader, "    mediump vec3 light = lightVarying + GetPSLighting( norm, posVarying ); \n" );
		hasLight = 1;
	}
	else 
	{
		strcat ( srcFragShader, "    mediump vec3 light = vec3(1.0,1.0,1.0);\n" );
	}
	
	if ( pMesh->HasTexture1() )
	{
		if ( pMesh->HasUV1s() ) 
		{
			if ( !pMesh->GetImage(1)->HasUVBounds() ) strcat ( srcFragShader, "    mediump vec4 tex1 = texture2D(texture1, uv1Varying);\n" );
			else strcat ( srcFragShader, "    mediump vec4 tex1 = texture2D(texture1, uv1Varying*textureBounds1.xy + textureBounds1.zw);\n" );
		}
		else if ( pMesh->HasUVs() ) 
		{
			if ( !pMesh->GetImage(1)->HasUVBounds() ) strcat ( srcFragShader, "    mediump vec4 tex1 = texture2D(texture1, uvVarying);\n" );
			else strcat ( srcFragShader, "    mediump vec4 tex1 = texture2D(texture1, uvVarying*textureBounds1.xy + textureBounds1.zw);\n" );
		}
		else strcat ( srcFragShader, "    mediump vec4 tex1 = vec4(1.0,1.0,1.0,1.0);\n" );

		// is it a light map, or a multiplied blending texture
		if ( pMesh->HasLightMap() )
		{
			if ( hasLight ) strcat ( srcFragShader, "    light += tex1.rgb;\n" );
			else strcat ( srcFragShader, "    light = tex1.rgb;\n" );
		}
		else
		{
			strcat ( srcFragShader, "    blendTex *= tex1 * 2.0;\n" );
		}
	}

	//strcat ( srcFragShader, "   light = clamp(light,0.0,1.0); \n" );
	
	if ( pMesh->HasUVs() && pMesh->HasTexture0() ) 
	{
		if ( !pMesh->GetImage(0)->HasUVBounds() ) strcat ( srcFragShader, "    mediump vec4 texColor = texture2D(texture0, uvVarying);\n" );
		else strcat ( srcFragShader, "    mediump vec4 texColor = texture2D(texture0, uvVarying*textureBounds0.xy + textureBounds0.zw);\n" );

		strcat ( srcFragShader, "    gl_FragColor = texColor * blendTex * vec4(light,1.0) * agk_MeshDiffuse + agk_MeshEmissive;\n" );
	}
	else 
	{
		strcat ( srcFragShader, "    gl_FragColor = blendTex * vec4(light,1.0) * agk_MeshDiffuse + agk_MeshEmissive;\n" );
	}

	if ( pMesh->GetObject() && pMesh->GetObject()->HasAlphaMask() )
	{
		strcat ( srcFragShader, "    if ( gl_FragColor.a < 0.5 ) discard;\n" );
	}

	if ( pMesh->WantsFog() && agk::GetFogMode() )
	{
		strcat ( srcFragShader, "    gl_FragColor.rgb = ApplyFog( gl_FragColor.rgb, posVarying );\n" );
	}
	
	strcat ( srcFragShader, "}\n" );

	pShader->m_bFlags &= ~AGK_SHADER_IS_CUSTOM;
	if ( pShader->NeedsAdditionalCode() ) 
	{
		// if this shader uses lighting or fog it will fail compilation until it has the code generated for it
		pShader->m_sVSSource.SetStr( srcVertShader );
		pShader->m_sPSSource.SetStr( srcFragShader );
		pShader->m_bValid = true;
	}
	else 
	{
		// no additional code required, good to compile
		pShader->SetShaderSource( srcVertShader, srcFragShader );
	}

	return pShader;
}

AGKShader* AGKShader::MakeFinalShader( AGKShader *pBaseShader, int sunActive, int VSLights, int PSLights, int useShadows, int normalMap )
{
	if ( VSLights > AGK_MAX_VERTEX_LIGHTS ) VSLights = AGK_MAX_VERTEX_LIGHTS;
	if ( PSLights > AGK_MAX_PIXEL_LIGHTS ) PSLights = AGK_MAX_PIXEL_LIGHTS;

	if ( useShadows > 1 ) useShadows = 1;
	if ( sunActive == 0 ) useShadows = 0;
	if ( agk::GetShadowMappingMode() == 0 ) useShadows = 0;

	// look in the shader cache
	UINT hash = GetFinalShaderHash( sunActive, VSLights, PSLights, useShadows );
	
	AGKShader *pShader = g_pAllShaders;
	while( pShader )
	{
		if ( pShader->m_pBaseShader == pBaseShader && pShader->m_iShaderHash == hash ) return pShader;
		pShader = pShader->m_pNextShader;
	}

	// not found in cache, generate a new one
	pShader = new AGKShader();
	pShader->m_iShaderHash = hash;
	pShader->m_pBaseShader = pBaseShader;
	pBaseShader->AddDerived( pShader );
	
	const char nums[ 16 ][ 2 ] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G" };

	char *srcVertShader = new char[ pBaseShader->m_sVSSource.GetLength() + 2048 + 512*VSLights ];
	char *srcFragShader = new char[ pBaseShader->m_sPSSource.GetLength() + 2048 + 512*PSLights + 4096*useShadows ];

	*srcVertShader = 0;
	*srcFragShader = 0;

	strcat( srcVertShader, pBaseShader->m_sVSSource.GetStr() );
	strcat( srcVertShader, "\n" );

	if ( pBaseShader->NeedsVSLighting() )
	{
		strcat ( srcVertShader, "uniform mediump vec3 agk_LightAmbient;\n" );
		strcat ( srcVertShader, "uniform mediump vec3 agk_DLightDir;\n" );
		strcat ( srcVertShader, "uniform mediump vec3 agk_DLightColor;\n" );

		for ( int i = 0; i < VSLights; ++i )
		{
			strcat ( srcVertShader, "uniform highp vec4 agk_VSLight" ); strcat(srcVertShader, nums[i]); strcat( srcVertShader, "Pos;\n" );
			strcat ( srcVertShader, "uniform mediump vec3 agk_VSLight" ); strcat(srcVertShader, nums[i]); strcat( srcVertShader, "Color;\n" );
		}

		if ( useShadows )
		{
			strcat ( srcVertShader, "uniform highp mat4 agk_ShadowProj;\n" );
			strcat ( srcVertShader, "varying highp vec4 shadowVarying;\n" );
			strcat ( srcVertShader, "varying highp float depth;\n" );
			if ( agk::GetShadowMappingMode() == 3 )
			{
				// cascade shadow mapping
				strcat ( srcVertShader, "uniform highp mat4 agk_Shadow2Proj;\n" );
				strcat ( srcVertShader, "uniform highp mat4 agk_Shadow3Proj;\n" );
				strcat ( srcVertShader, "uniform highp mat4 agk_Shadow4Proj;\n" );

				strcat ( srcVertShader, "varying highp vec3 shadow2Varying;\n" );
				strcat ( srcVertShader, "varying highp vec3 shadow3Varying;\n" );
				strcat ( srcVertShader, "varying highp vec3 shadow4Varying;\n" );
			}
		}

		// Vertex shader
		strcat ( srcVertShader, "mediump vec3 GetVSLighting( mediump vec3 normal, highp vec3 pos )\n" );
		strcat ( srcVertShader, "{\n" );

		if ( useShadows ) 
		{
			strcat ( srcVertShader, "    shadowVarying = agk_ShadowProj * vec4(pos,1.0);\n");
			if ( agk::GetShadowMappingMode() != 2 ) // not LiSPM
			{
				strcat ( srcVertShader, "    shadowVarying.xy = shadowVarying.xy*0.5 + 0.5;\n");
				strcat ( srcVertShader, "    shadowVarying.z = shadowVarying.z*0.495 + 0.505;\n");
			}
			if ( agk::GetShadowMappingMode() == 3 ) // cascade
			{
				strcat ( srcVertShader, "    shadow2Varying = (agk_Shadow2Proj * vec4(pos,1.0)).xyz;\n");
				strcat ( srcVertShader, "    shadow3Varying = (agk_Shadow3Proj * vec4(pos,1.0)).xyz;\n");
				strcat ( srcVertShader, "    shadow4Varying = (agk_Shadow4Proj * vec4(pos,1.0)).xyz;\n");

				strcat ( srcVertShader, "    shadow2Varying.xy = shadow2Varying.xy*0.5 + 0.5;\n");
				strcat ( srcVertShader, "    shadow2Varying.z = shadow2Varying.z*0.495 + 0.505;\n");

				strcat ( srcVertShader, "    shadow3Varying.xy = shadow3Varying.xy*0.5 + 0.5;\n");
				strcat ( srcVertShader, "    shadow3Varying.z = shadow3Varying.z*0.495 + 0.505;\n");

				strcat ( srcVertShader, "    shadow4Varying.xy = shadow4Varying.xy*0.5 + 0.5;\n");
				strcat ( srcVertShader, "    shadow4Varying.z = shadow4Varying.z*0.495 + 0.505;\n");
			}
			strcat ( srcVertShader, "    depth = gl_Position.w;\n");
		}

		// start with ambient and directional light
		strcat ( srcVertShader, "    mediump vec3 light = agk_LightAmbient;\n" );
		// if shadows or normal maps are active then the directional light has to be calculated in the pixel shader
		if ( useShadows == 0 && normalMap == 0 ) strcat ( srcVertShader, "    light += max(dot(-agk_DLightDir, normal),0.0) * agk_DLightColor;\n" );
		
		for ( int i = 0; i < VSLights; ++i )
		{
			if ( i == 0 ) strcat ( srcVertShader, "    highp vec3 dir2;\n highp float atten;\n highp float lightRange;\n highp float intensity;\n" );
			strcat ( srcVertShader, "    dir2 = (agk_VSLight" ); strcat(srcVertShader, nums[i]); strcat( srcVertShader, "Pos.xyz - pos);\n" );
			strcat ( srcVertShader, "    lightRange = agk_VSLight" ); strcat(srcVertShader, nums[i]); strcat( srcVertShader, "Pos.w;\n" );
			strcat ( srcVertShader, "    atten = max(0.0, 1.0 - dot(dir2,dir2)/lightRange);\n" ); // squared exponetial fall off
			strcat ( srcVertShader, "    atten *= atten; atten *= atten;\n" ); // squared exponetial fall off
			//strcat ( srcVertShader, "    atten = clamp(atten,0.0,1.0);\n" );
			strcat ( srcVertShader, "    intensity = max(0.0,dot(normalize(dir2),normal));\n" );
			strcat ( srcVertShader, "    light += agk_VSLight" ); strcat(srcVertShader, nums[i]); strcat( srcVertShader, "Color * intensity * atten; \n" );
		}

		strcat ( srcVertShader, "    return light;\n" );
		strcat ( srcVertShader, "}\n" );
	}

	// Pixel shader
	strcat( srcFragShader, pBaseShader->m_sPSSource.GetStr() );
	strcat( srcFragShader, "\n" );

	// fog and shadows use the directional light
	strcat ( srcFragShader, "uniform mediump vec3 agk_DLightDir;\n" );
	strcat ( srcFragShader, "uniform mediump vec3 agk_DLightColor;\n" );

	if ( pBaseShader->NeedsPSLighting() )
	{
		if ( useShadows ) 
		{
			strcat ( srcFragShader, "uniform sampler2D shadowMap;\n" );
			strcat ( srcFragShader, "varying highp vec4 shadowVarying;\n" );
			strcat ( srcFragShader, "varying highp float depth;\n" );
			strcat ( srcFragShader, "uniform highp vec4 agk_ShadowParams;\n" );
			
			if ( agk::GetShadowMappingMode() == 3 ) // cascade shadows
			{
				strcat ( srcFragShader, "uniform sampler2D shadowMap2;\n" );
				strcat ( srcFragShader, "uniform sampler2D shadowMap3;\n" );
				strcat ( srcFragShader, "uniform sampler2D shadowMap4;\n" );

				strcat ( srcFragShader, "varying highp vec3 shadow2Varying;\n" );
				strcat ( srcFragShader, "varying highp vec3 shadow3Varying;\n" );
				strcat ( srcFragShader, "varying highp vec3 shadow4Varying;\n" );

				strcat ( srcFragShader, "uniform highp vec4 agk_ShadowParams2;\n" );
			}
		}

		for ( int i = 0; i < PSLights; ++i )
		{
			strcat ( srcFragShader, "uniform highp vec4 agk_PSLight" ); strcat(srcFragShader, nums[i]); strcat( srcFragShader, "Pos;\n" );
			strcat ( srcFragShader, "uniform mediump vec3 agk_PSLight" ); strcat(srcFragShader, nums[i]); strcat( srcFragShader, "Color;\n" );
		}
		
		strcat ( srcFragShader, "mediump vec3 GetPSLighting( mediump vec3 normal, highp vec3 pos )\n" );
		strcat ( srcFragShader, "{\n" );
		strcat ( srcFragShader, "    highp float scale = 0.01;\n" );
		strcat ( srcFragShader, "    highp float scale2 = 0.0001;\n" );
		strcat ( srcFragShader, "    mediump vec3 light = vec3(0.0,0.0,0.0);\n" );

		if ( useShadows ) 
		{
			// calculate directional light and its shadow
			strcat ( srcFragShader, "    highp float dotp = dot(-agk_DLightDir, normal);\n" );
			strcat ( srcFragShader, "    if ( dotp > 0.0 ) {\n" );
			strcat ( srcFragShader, "    mediump vec3 dirLight = dotp * agk_DLightColor;\n" );
			//strcat ( srcFragShader, "    highp float bias = min(agk_ShadowParams.y*tan(acos(dotp)),0.01);\n" );
			strcat ( srcFragShader, "    dotp = 1.0-dotp;\n" );
			strcat ( srcFragShader, "    dotp = dotp*dotp; dotp = dotp*dotp;\n" );
			strcat ( srcFragShader, "    highp float bias = agk_ShadowParams.y*10.0*dotp + agk_ShadowParams.y;\n" );

			if ( agk::GetShadowMappingMode() == 1 || agk::GetShadowMappingMode() == 2 ) // Uniform and LiPSM
			{
				strcat ( srcFragShader, "    highp vec3 shadowUV = shadowVarying.xyz;\n");
				if ( agk::GetShadowMappingMode() == 2 ) // LiPSM
				{
					strcat ( srcFragShader, "    shadowUV.xyz = shadowUV / shadowVarying.w;\n");
					strcat ( srcFragShader, "    shadowUV.xy = shadowUV.xy*0.5 + 0.5;\n");
					strcat ( srcFragShader, "    shadowUV.z = shadowUV.z*0.49995 + 0.50005;\n");
				}

				strcat ( srcFragShader, "    highp float pixelDepth = shadowUV.z - bias;\n" );
				if ( !agk::PlatformSupportsPSHighp() ) strcat ( srcFragShader, "    pixelDepth -= bias;\n" );

				if ( agk::GetShadowSmoothing() == 0 )
				{
					strcat ( srcFragShader, "    highp float shadow = step(0.0,pixelDepth - texture2D( shadowMap, shadowUV.xy ).r);\n" );
				}
				else if ( agk::GetShadowSmoothing() == 1 || agk::GetShadowSmoothing() == 2 ) // multisampled
				{
					strcat ( srcFragShader, "    highp vec2 pd[4];\n" );
					if ( agk::GetShadowSmoothing() == 1 ) // fixed multisample
					{
						strcat ( srcFragShader, "    pd[0] = vec2(0.314935, 0.20592825);\n" );
						strcat ( srcFragShader, "    pd[1] = vec2(-0.143142, -0.1402359);\n" );
						strcat ( srcFragShader, "    pd[2] = vec2(-0.2870628, 0.20983215);\n" );
						strcat ( srcFragShader, "    pd[3] = vec2(0.25725175, -0.25337295);\n" );
						
					}
					else if ( agk::GetShadowSmoothing() == 2 ) // random multisample
					{
						strcat ( srcFragShader, "    for( int i = 0; i < 4; i++ ) {\n" );
						strcat ( srcFragShader, "        highp float random = dot(vec4(pos,i), vec4(12.9898,78.233,45.164,94.673));\n" );
						strcat ( srcFragShader, "        highp float randomX = fract(sin(random) * 43758.5453);\n" );
						strcat ( srcFragShader, "        highp float randomY = fract(cos(random) * 55204.6122);\n" );
						strcat ( srcFragShader, "        pd[i].x = randomX*0.5 - 0.25;\n" );
						strcat ( srcFragShader, "        pd[i].y = randomY*0.5 - 0.25;\n" );
						strcat ( srcFragShader, "    }\n" );
					}

					strcat ( srcFragShader, "    highp float shadow = 0.0;\n" );
					strcat ( srcFragShader, "    for( int i = 0; i < 4; i++ ) {\n" );
					strcat ( srcFragShader, "        shadow += step(0.0,pixelDepth - texture2D( shadowMap, vec2(shadowUV.xy + pd[i]*agk_ShadowParams.zw) ).r);\n" );
					strcat ( srcFragShader, "    }\n" );
					strcat ( srcFragShader, "    shadow = shadow*0.25;\n" );
				}

				strcat ( srcFragShader, "    shadow *= step(0.0, agk_ShadowParams.x - depth);\n" );
			}
			else if ( agk::GetShadowMappingMode() == 3 ) // Cascade
			{
				if ( agk::GetShadowSmoothing() == 1 ) // fixed multisample
				{
					strcat ( srcFragShader, "    highp vec2 pd[4];\n" );
					strcat ( srcFragShader, "    pd[0] = vec2(0.5785557, 0.3818373);\n" );
					strcat ( srcFragShader, "    pd[1] = vec2(-0.5812719, -0.451035);\n" );
					strcat ( srcFragShader, "    pd[2] = vec2(-0.4778019, 0.4936384);\n" );
					strcat ( srcFragShader, "    pd[3] = vec2(0.5724188, -0.4114415);\n" );
				}
				else if ( agk::GetShadowSmoothing() == 2 ) // random multisample
				{
					strcat ( srcFragShader, "    highp vec2 pd[4];\n" );
					strcat ( srcFragShader, "    highp float base = dot(floor(pos*1000.0),vec3(0.0129898,0.078233,0.045164));\n" );
					strcat ( srcFragShader, "    for( int i = 0; i < 4; i++ ) {\n" );
					strcat ( srcFragShader, "        highp float random = sin(float(i)*94.673 + base);\n" );
					//strcat ( srcFragShader, "        highp float random = dot(vec4(gl_FragCoord.xyy,i), vec4(12.9898,78.233,45.164,94.673));\n" );
					strcat ( srcFragShader, "        highp float randomX = fract(random * 43758.5453);\n" );
					strcat ( srcFragShader, "        highp float randomY = fract(random * 55204.6122);\n" );
					strcat ( srcFragShader, "        pd[i].x = randomX*2.0 - 1.0;\n" );
					strcat ( srcFragShader, "        pd[i].y = randomY*2.0 - 1.0;\n" );
					strcat ( srcFragShader, "    }\n" );
				}

				strcat ( srcFragShader, "    highp float shadow = 0.0;\n" );
				strcat ( srcFragShader, "    if( depth < agk_ShadowParams2.w ) {\n");
				strcat ( srcFragShader, "        highp float pixelDepth = shadow4Varying.z - bias;\n" );
				if ( agk::GetShadowSmoothing() > 0 ) 
				{
					strcat ( srcFragShader, "        for( int i = 0; i < 4; i++ ) shadow += step(0.0,pixelDepth - texture2D( shadowMap4, shadow4Varying.xy+pd[i]*agk_ShadowParams.zw ).r);\n" );
					strcat ( srcFragShader, "        shadow = shadow*0.25;\n" );
				}
				else strcat ( srcFragShader, "        shadow = step(0.0,pixelDepth - texture2D( shadowMap4, shadow4Varying.xy ).r);\n" );
				strcat ( srcFragShader, "    }\n");
				strcat ( srcFragShader, "    else if( depth < agk_ShadowParams2.z ) {\n");
				strcat ( srcFragShader, "        highp float pixelDepth = shadow3Varying.z - 1.25*bias;\n" );
				if ( agk::GetShadowSmoothing() > 0 ) 
				{
					strcat ( srcFragShader, "        for( int i = 0; i < 4; i++ ) shadow += step(0.0,pixelDepth - texture2D( shadowMap3, shadow3Varying.xy+pd[i]*agk_ShadowParams.zw ).r);\n" );
					strcat ( srcFragShader, "        shadow = shadow*0.25;\n" );
				}
				else strcat ( srcFragShader, "        shadow = step(0.0,pixelDepth - texture2D( shadowMap3, shadow3Varying.xy ).r);\n" );
				strcat ( srcFragShader, "    }\n");
				strcat ( srcFragShader, "    else if( depth < agk_ShadowParams2.y ) {\n");
				strcat ( srcFragShader, "        highp float pixelDepth = shadow2Varying.z - 1.5*bias;\n" );
				if ( agk::GetShadowSmoothing() > 0 ) 
				{
					strcat ( srcFragShader, "        for( int i = 0; i < 4; i++ ) shadow += step(0.0,pixelDepth - texture2D( shadowMap2, shadow2Varying.xy+pd[i]*agk_ShadowParams.zw ).r);\n" );
					strcat ( srcFragShader, "        shadow = shadow*0.25;\n" );
				}
				else strcat ( srcFragShader, "        shadow = step(0.0,pixelDepth - texture2D( shadowMap2, shadow2Varying.xy ).r);\n" );
				strcat ( srcFragShader, "    }\n");
				strcat ( srcFragShader, "    else if( depth < agk_ShadowParams2.x ) {\n");
				strcat ( srcFragShader, "        highp float pixelDepth = shadowVarying.z - 1.25*bias;\n" );
				strcat ( srcFragShader, "        shadow = step(0.0,pixelDepth - texture2D( shadowMap, shadowVarying.xy ).r);\n" );
				//if ( agk::GetShadowSmoothing() == 1 ) strcat ( srcFragShader, "        for( int i = 0; i < 5; i++ ) shadow += step(0.0,pixelDepth - texture2D( shadowMap, shadowVarying.xy+pd[i]*agk_ShadowParams.zw ).r);\n" );
				strcat ( srcFragShader, "    }\n");
				//strcat ( srcFragShader, "    light += texture2D( shadowMap2, shadow2Varying.xy ).r;\n" );
			}

			/*
			// linear depth - multisampled
			if ( agk::GetShadowSmoothing() == 1 )
			{
				strcat ( srcFragShader, "    highp vec2 uv = shadowUV.xy;\n" );
				strcat ( srcFragShader, "    shadow += step(0.0,pixelDepth - texture2D( shadowMap, vec2(uv.x-agk_ShadowParams.z*0.23550406,uv.y-agk_ShadowParams.w*0.09976554) ).r);\n" );
				strcat ( srcFragShader, "    shadow += step(0.0,pixelDepth - texture2D( shadowMap, vec2(uv.x+agk_ShadowParams.z*0.23639652,uv.y-agk_ShadowParams.w*0.19222681) ).r);\n" );
				strcat ( srcFragShader, "    shadow += step(0.0,pixelDepth - texture2D( shadowMap, vec2(uv.x-agk_ShadowParams.z*0.02354603,uv.y-agk_ShadowParams.w*0.23234718) ).r);\n" );
				strcat ( srcFragShader, "    shadow += step(0.0,pixelDepth - texture2D( shadowMap, vec2(uv.x+agk_ShadowParams.z*0.08623985,uv.y+agk_ShadowParams.w*0.07346941) ).r);\n" );
				strcat ( srcFragShader, "    shadow = shadow*0.2;\n" );
			}
			*/

			/*
			// nearest depth - bilinear filtering
			if ( agk::GetShadowSmoothing() == 2 )
			{
				strcat ( srcFragShader, "    highp vec2 uv = shadowUV.xy;\n" );
				strcat ( srcFragShader, "    highp float shadow2 = step(0.0,pixelDepth - texture2D( shadowMap, vec2(uv.x+agk_ShadowParams.z,uv.y) ).r);\n" );
				strcat ( srcFragShader, "    highp float shadow3 = step(0.0,pixelDepth - texture2D( shadowMap, vec2(uv.x,uv.y+agk_ShadowParams.w) ).r);\n" );
				strcat ( srcFragShader, "    highp float shadow4 = step(0.0,pixelDepth - texture2D( shadowMap, vec2(uv.x+agk_ShadowParams.z,uv.y+agk_ShadowParams.w) ).r);\n" );
				strcat ( srcFragShader, "    highp vec2 f = fract(shadowUV.xy/agk_ShadowParams.zw);\n" );
				strcat ( srcFragShader, "    shadow = mix(shadow, shadow2, f.x);\n" );
				strcat ( srcFragShader, "    shadow3 = mix(shadow3, shadow4, f.x);\n" );
				strcat ( srcFragShader, "    shadow = mix(shadow, shadow3, f.y);\n" );
			}
			*/
									
			
			strcat ( srcFragShader, "    light += dirLight*(1.0-shadow);\n" );
			//strcat ( srcFragShader, "    highp float color = 1.0-shadowUV.z;\n" );
			//strcat ( srcFragShader, "    light += texture2D( shadowMap, shadowVarying.xy ).r;\n" );
			strcat ( srcFragShader, "    }\n" );
		}
		else if ( normalMap )
		{
			strcat ( srcFragShader, "    light += max(dot(-agk_DLightDir, normal),0.0) * agk_DLightColor;\n\n" );
		}
		
		for ( int i = 0; i < PSLights; ++i )
		{
			if ( i == 0 ) strcat ( srcFragShader, "    highp vec3 dir;\n highp float atten;\n highp float lightRange;\n highp float intensity;\n" );
			strcat ( srcFragShader, "    dir = (agk_PSLight" ); strcat(srcFragShader, nums[i]); strcat( srcFragShader, "Pos.xyz - pos) * scale;\n" );
			strcat ( srcFragShader, "    lightRange = agk_PSLight" ); strcat(srcFragShader, nums[i]); strcat( srcFragShader, "Pos.w * scale2;\n" );
			strcat ( srcFragShader, "    atten = max(0.0, 1.0 - dot(dir,dir)/lightRange);\n" ); // squared exponetial fall off
			strcat ( srcFragShader, "    atten *= atten; atten *= atten;\n" ); // squared exponetial fall off

			//strcat ( srcFragShader, "    intensity = max(0.0,dot(agk_PSLight" ); strcat(srcFragShader, nums[i]); strcat( srcFragShader, "DirVarying,normal));\n" );
			strcat ( srcFragShader, "    intensity = max(0.0,dot(normalize(dir),normal));\n" );

			strcat ( srcFragShader, "    light += agk_PSLight" ); strcat(srcFragShader, nums[i]); strcat( srcFragShader, "Color * intensity * atten; \n" );
		}

		strcat ( srcFragShader, "    return light;\n" );
		strcat ( srcFragShader, "}\n" );
	}

	if ( pBaseShader->NeedsFog() )
	{
		if ( agk::GetFogMode() == 0 )
		{
			strcat ( srcFragShader, "mediump vec3 ApplyFog( mediump vec3 color, highp vec3 pointPos )\n" );
			strcat ( srcFragShader, "{\n" );
			strcat ( srcFragShader, "    return color;\n" );
			strcat ( srcFragShader, "}\n" );
		}
		else
		{
			int fogColorsEqual = agk::GetFogColorsEqual();

			strcat ( srcFragShader, "\n" );
			if ( strstr(srcFragShader,"vec3 agk_CameraPos") == 0 ) strcat ( srcFragShader, "uniform highp vec3 agk_CameraPos;\n" );
			strcat ( srcFragShader, "uniform highp vec2 fogRange;\n" ); // = vec2( minDist, -4.0 / (maxDist-minDist) );
			strcat ( srcFragShader, "uniform mediump vec3 fogColor1;\n" ); // = vec3(0.63,0.73,0.82);
			if ( !fogColorsEqual ) strcat ( srcFragShader, "uniform mediump vec3 fogColor2;\n" ); // = vec3(1.0,0.9,0.7);
			strcat ( srcFragShader, "\n" );
			strcat ( srcFragShader, "mediump vec3 ApplyFog( mediump vec3 color, highp vec3 pointPos )\n" );
			strcat ( srcFragShader, "{\n" );
			strcat ( srcFragShader, "    highp vec3 viewDir = agk_CameraPos - pointPos;\n" );
			strcat ( srcFragShader, "    highp float invDist = inversesqrt(dot(viewDir,viewDir));\n" );
			if ( !fogColorsEqual ) strcat ( srcFragShader, "    highp float sunPoint = dot(viewDir*invDist,agk_DLightDir)*0.499 + 0.5;\n" ); // iPad 3 (and maybe others) having floating point issues if exactly 0.5 is used
			strcat ( srcFragShader, "    invDist = max( 0.0, 1.0/invDist - fogRange.x );\n" );
			strcat ( srcFragShader, "    invDist = exp( invDist*fogRange.y );\n" ); // variable reuse
			if ( !fogColorsEqual ) strcat ( srcFragShader, "    mediump vec3 fogColor = mix(fogColor1, fogColor2, pow(sunPoint,24.0));\n" );
			else strcat ( srcFragShader, "    mediump vec3 fogColor = fogColor1;\n" );
			strcat ( srcFragShader, "    color = mix( fogColor, color, invDist );\n" );
			strcat ( srcFragShader, "    return color;\n" );
			strcat ( srcFragShader, "}\n" );
		}
	}

	if ( pBaseShader->m_bFlags & AGK_SHADER_IS_CUSTOM ) pShader->m_bFlags |= AGK_SHADER_IS_CUSTOM;
	else pShader->m_bFlags &= ~AGK_SHADER_IS_CUSTOM;

	pShader->SetShaderSource( srcVertShader, srcFragShader );

	delete [] srcVertShader;
	delete [] srcFragShader;

	if ( !pShader->m_bValid )
	{
		delete pShader;
		return 0;
	}

	if ( pBaseShader->NeedsFog() && agk::GetFogMode() )
	{
		pShader->SetConstantByName( "fogRange", g_fFogMinDist, -4.0f / (g_fFogMaxDist-g_fFogMinDist), 0,0 ); // minDist = 50, maxDist = 700
		pShader->SetConstantByName( "fogColor1", g_fFogColorR, g_fFogColorG, g_fFogColorB, 0 );
		pShader->SetConstantByName( "fogColor2", g_fFogColor2R, g_fFogColor2G, g_fFogColor2B, 0 );
	}

	// set uniforms from base shader
	cShaderUniform *pUniform = pBaseShader->m_cUniformList.GetFirst();
	while( pUniform )
	{
		UINT arrayIndex = pUniform->m_iArrayMembers;
		int iType = pUniform->m_iType;

		if( iType == 0 )
		{
			// not a matrix
			if ( arrayIndex == 0xffffffff ) pShader->SetConstantByName( pUniform->m_sName, pUniform->m_pValues[0], pUniform->m_pValues[1], pUniform->m_pValues[2], pUniform->m_pValues[3] );
			else
			{
				uString sName = pUniform->m_sName;
				sName.Trunc( '[' );
				pShader->SetConstantArrayByName( sName, arrayIndex, pUniform->m_pValues[0], pUniform->m_pValues[1], pUniform->m_pValues[2], pUniform->m_pValues[3] );
			}
		}
		else
		{
			// is a matrix
			if ( arrayIndex == 0xffffffff ) pShader->SetConstantMatrixByName( pUniform->m_sName, pUniform->m_iComponents, pUniform->m_pValues );
			else
			{
				uString sName = pUniform->m_sName;
				sName.Trunc( '[' );
				pShader->SetConstantMatrixArrayByName( sName, arrayIndex, pUniform->m_iComponents, pUniform->m_pValues );
			}
		}
		
		pUniform = pBaseShader->m_cUniformList.GetNext();
	}

	return pShader;
}

void AGKShader::DrawTriangles( int first, UINT count )
{
	if ( !m_bValid ) return;

	if ( g_pCurrentShader != this ) 
	{
		agk::Error( "Tried to draw a shader that is not active" );
		agk::Message( "Tried to draw a shader that is not active" );
		return;
	}

	UpdateMatrices();
	UpdateAGKUniforms();
	PlatformDrawPrimitives( AGK_TRIANGLES, first, count );
}

void AGKShader::DrawPrimitives( int primitive, int first, UINT count )
{
	if ( !m_bValid ) return;

	if ( g_pCurrentShader != this ) 
	{
		agk::Error( "Tried to draw a shader that is not active" );
		agk::Message( "Tried to draw a shader that is not active" );
		return;
	}

	UpdateMatrices();
	UpdateAGKUniforms();
	PlatformDrawPrimitives( primitive, first, count );
}

void AGKShader::DrawIndices( UINT count, unsigned short *pIndices, int primitive )
{
	if ( !m_bValid ) 
	{
		agk::Error( "Tried to draw a shader that is not valid" );
		agk::Message( "Tried to draw a shader that is not valid" );
		return;
	}

	if ( g_pCurrentShader != this ) 
	{
		agk::Error( "Tried to draw a shader that is not active" );
		agk::Message( "Tried to draw a shader that is not active" );
		return;
	}

	UpdateMatrices();
	UpdateAGKUniforms();
	PlatformDrawIndices( primitive, count, pIndices );
}

void AGKShader::DrawIndicesInt( UINT count, unsigned int *pIndices, int primitive )
{
	if ( !m_bValid ) 
	{
		agk::Error( "Tried to draw a shader that is not valid" );
		agk::Message( "Tried to draw a shader that is not valid" );
		return;
	}

	if ( g_pCurrentShader != this ) 
	{
		agk::Error( "Tried to draw a shader that is not active" );
		agk::Message( "Tried to draw a shader that is not active" );
		return;
	}

	UpdateMatrices();
	UpdateAGKUniforms();
	PlatformDrawIndicesInt( primitive, count, pIndices );
}
