#include "agk.h"
#include "include/assimp/anim.h"

using namespace AGK;

// Bones

Bone3D::Bone3D()
{
	m_iIndex = 0;
	m_bFlags = AGK_BONE3D_ANIMATE;
	m_pParent = 0;

	m_origScale.Set( 1,1,1 );

	m_pAnim = 0;
	m_iCurrFrameRotation = 0;
	m_iCurrFramePosition = 0;
	m_iCurrFrameScale = 0;

	m_tweenScale.Set( 1,1,1 );
}

Bone3D::Bone3D( const Bone3D *pOther )
{
	SetNodePosition( pOther->pos().x, pOther->pos().y, pOther->pos().z );
	SetNodeRotation( pOther->rot().w, pOther->rot().x, pOther->rot().y, pOther->rot().z );
	SetNodeScale( pOther->scale().x, pOther->scale().y, pOther->scale().z );

	m_iIndex = pOther->m_iIndex;
	m_sName.SetStr( pOther->m_sName.GetStr() );
	m_origPosition = pOther->m_origPosition;
	m_origRotation = pOther->m_origRotation;
	m_origScale = pOther->m_origScale;
	m_offsetPosition = pOther->m_offsetPosition;
	m_offsetRotation = pOther->m_offsetRotation;
	m_bFlags = pOther->m_bFlags;
	m_tempRot = pOther->m_tempRot;

	m_tweenScale.Set(1,1,1);

	// don't copy playing animations
	m_pAnim = 0;
	m_iCurrFramePosition = 0;
	m_iCurrFrameRotation = 0;
	m_iCurrFrameScale = 0;

	// can't know correct parent pointer, let the skeleton do it
	m_pParent = 0;

	m_BoundingBox = pOther->m_BoundingBox;
}

Bone3D::~Bone3D()
{
	
}

void Bone3D::DeleteAttachedObjects()
{
	for( UINT i = 0; i < m_vChildren.size(); i++ )
	{
		if ( m_vChildren[ i ]->GetNodeType() == eObject )
		{
			cObject3D *pObject = (cObject3D*)(m_vChildren[ i ]);
			pObject->DeleteTree();
			pObject->RemoveFromDeletingParent();
			if ( pObject->GetID() ) agk::DeleteObject( pObject->GetID() );
			else delete pObject;
		}
		else
		{
			m_vChildren[ i ]->RemoveFromDeletingParent();
		}
	}

	m_vChildren.clear();
}

void Bone3D::SetCanAnimate( int animate )
{
	if ( animate != 0 ) m_bFlags |= AGK_BONE3D_ANIMATE;
	else m_bFlags &= ~AGK_BONE3D_ANIMATE;
}

void Bone3D::ResetToOrig()
{
	m_pAnim = 0;
	SetNodePosition( m_origPosition.x, m_origPosition.y, m_origPosition.z );
	SetNodeRotation( m_origRotation.w, m_origRotation.x, m_origRotation.y, m_origRotation.z );
	SetNodeScale( m_origScale.x, m_origScale.y, m_origScale.z );
}

void Bone3D::Tween( float currtime, float s )
{
	if ( !m_pAnim || (m_bFlags & AGK_BONE_ANIMATE) == 0 ) return;

	if ( currtime < 0 ) currtime = 0;

	// calculate the new transform
	AGKVector newPos = pos();
	AGKQuaternion newRot = rot();
	AGKVector newScale = scale();

	if ( m_iCurrFramePosition >= m_pAnim->m_iNumPositions ) m_iCurrFramePosition = 0;
	if ( m_iCurrFrameRotation >= m_pAnim->m_iNumRotations ) m_iCurrFrameRotation = 0;
	if ( m_iCurrFrameScale >= m_pAnim->m_iNumScales ) m_iCurrFrameScale = 0;

	// positions
	if ( m_pAnim->m_iNumPositions > 0 && m_pAnim->m_pPositions[ 0 ].m_fTime <= currtime )
	{
		if ( m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime == currtime || m_pAnim->m_iNumPositions == 1 )
		{
			// animation frame exactly matches time we are looking for, no interpolation required
			newPos = m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_position;
		}
		else
		{
			if ( m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime < currtime )
			{
				if ( m_pAnim->m_pPositions[ m_pAnim->m_iNumPositions-1 ].m_fTime < currtime ) m_iCurrFramePosition = m_pAnim->m_iNumPositions-1;

				// time is ahead of our pointer, advance until we find the right frame
				if ( m_iCurrFramePosition < m_pAnim->m_iNumPositions-1 )
				{
					// time is ahead of our pointer, advance until we find the right frame
					float keydiff = m_pAnim->m_pPositions[ m_iCurrFramePosition+1 ].m_fTime - m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime;
					float currdiff = currtime - m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime;

					if ( keydiff == 0 || currdiff < keydiff*10 )
					{
						// frame should be quite close, use linear search
						while ( m_iCurrFramePosition < m_pAnim->m_iNumPositions-1 
							 && m_pAnim->m_pPositions[ m_iCurrFramePosition+1 ].m_fTime < currtime ) m_iCurrFramePosition++;
					}
					else
					{
						// frame might be quite far, use binary search
						int high = m_pAnim->m_iNumPositions-1;
						int low = m_iCurrFramePosition;
						int result = 0;
						while( high >= low )
						{
							int mid = (high+low)/2;
							if( m_pAnim->m_pPositions[ mid ].m_fTime > currtime ) high = mid-1;
							else if ( m_pAnim->m_pPositions[ mid ].m_fTime < currtime ) low = mid+1;
							else { low = mid; high = mid-1; break; }
						}

						if( m_pAnim->m_pPositions[ low ].m_fTime > currtime ) m_iCurrFramePosition = low-1;
						else m_iCurrFramePosition = low;
					}
				}
			}
			else
			{
				// time is behind our pointer, go backwards until we find the right frame
				if ( m_iCurrFramePosition > 0 )
				{
					float keydiff = m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime - m_pAnim->m_pPositions[ m_iCurrFramePosition-1 ].m_fTime;
					float currdiff = m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime - currtime;

					if ( keydiff == 0 || currdiff < keydiff*10 )
					{
						// frame should be quite close, use linear search
						while ( m_iCurrFramePosition > 0
							 && m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime > currtime ) m_iCurrFramePosition--;
					}
					else
					{
						// frame might be quite far, use binary search
						int high = m_iCurrFramePosition;
						int low = 0;
						int result = 0;
						while( high >= low )
						{
							int mid = (high+low)/2;
							if( m_pAnim->m_pPositions[ mid ].m_fTime > currtime ) high = mid-1;
							else if ( m_pAnim->m_pPositions[ mid ].m_fTime < currtime ) low = mid+1;
							else { low = mid; high = mid-1; break; }
						}

						if( m_pAnim->m_pPositions[ low ].m_fTime > currtime ) m_iCurrFramePosition = low-1;
						else m_iCurrFramePosition = low;
					}
				}
			}

			if ( m_iCurrFramePosition == m_pAnim->m_iNumPositions-1 )
			{
				// reached last keyframe and animation hasn't looped yet, must be some dead time for this bone
				newPos = m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_position;
			}
			else
			{
				float timeDiff = m_pAnim->m_pPositions[ m_iCurrFramePosition+1 ].m_fTime - m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime;
				if ( timeDiff <= 0 )
				{
					// key frames have the same time, should never happen but account for it anyway
					newPos = m_pAnim->m_pPositions[ m_iCurrFramePosition+1 ].m_position;
				}
				else
				{
					// interpolate between these key frames
					float t = (currtime - m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime) / timeDiff;
					m_pAnim->m_pPositions[ m_iCurrFramePosition ].Interpolate( &(m_pAnim->m_pPositions[m_iCurrFramePosition+1]), t, newPos );
				}
			}
		}
	}

	// rotation
	if ( m_pAnim->m_iNumRotations > 0 && m_pAnim->m_pRotations[ 0 ].m_fTime <= currtime )
	{
		if ( m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime == currtime || m_pAnim->m_iNumRotations == 1 )
		{
			// animation frame exactly matches time we are looking for, no interpolation required
			newRot = m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_qRotation;
		}
		else
		{
			if ( m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime < currtime )
			{
				if ( m_pAnim->m_pRotations[ m_pAnim->m_iNumRotations-1 ].m_fTime < currtime ) m_iCurrFrameRotation = m_pAnim->m_iNumRotations-1;

				if ( m_iCurrFrameRotation < m_pAnim->m_iNumRotations-1 )
				{
					// time is ahead of our pointer, advance until we find the right frame
					float keydiff = m_pAnim->m_pRotations[ m_iCurrFrameRotation+1 ].m_fTime - m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime;
					float currdiff = currtime - m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime;

					if ( keydiff == 0 || currdiff < keydiff*10 )
					{
						// frame should be quite close, use linear search
						while ( m_iCurrFrameRotation < m_pAnim->m_iNumRotations-1 
							 && m_pAnim->m_pRotations[ m_iCurrFrameRotation+1 ].m_fTime < currtime ) m_iCurrFrameRotation++;
					}
					else
					{
						// frame might be quite far, use binary search
						int high = m_pAnim->m_iNumRotations-1;
						int low = m_iCurrFrameRotation;
						int result = 0;
						while( high >= low )
						{
							int mid = (high+low)/2;
							if( m_pAnim->m_pRotations[ mid ].m_fTime > currtime ) high = mid-1;
							else if ( m_pAnim->m_pRotations[ mid ].m_fTime < currtime ) low = mid+1;
							else { low = mid; high = mid-1; break; }
						}

						if( m_pAnim->m_pRotations[ low ].m_fTime > currtime ) m_iCurrFrameRotation = low-1;
						else m_iCurrFrameRotation = low;
					}
				}
			}
			else
			{
				if ( m_iCurrFrameRotation > 0 )
				{
					// time is behind our pointer, go backwards until we find the right frame
					float keydiff = m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime - m_pAnim->m_pRotations[ m_iCurrFrameRotation-1 ].m_fTime;
					float currdiff = m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime - currtime;

					if ( keydiff == 0 || currdiff < keydiff*10 )
					{
						// frame should be quite close, use linear search
						while ( m_iCurrFrameRotation > 0
							 && m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime > currtime ) m_iCurrFrameRotation--;
					}
					else
					{
						// frame might be quite far, use binary search
						int high = m_iCurrFrameRotation;
						int low = 0;
						int result = 0;
						while( high >= low )
						{
							int mid = (high+low)/2;
							if( m_pAnim->m_pRotations[ mid ].m_fTime > currtime ) high = mid-1;
							else if ( m_pAnim->m_pRotations[ mid ].m_fTime < currtime ) low = mid+1;
							else { low = mid; high = mid-1; break; }
						}

						if( m_pAnim->m_pRotations[ low ].m_fTime > currtime ) m_iCurrFrameRotation = low-1;
						else m_iCurrFrameRotation = low;
					}
				}
			}

			if ( m_iCurrFrameRotation == m_pAnim->m_iNumRotations-1 )
			{
				// reached last keyframe and animation hasn't looped yet, must be some dead time for this bone
				newRot = m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_qRotation;
			}
			else
			{
				float timeDiff = m_pAnim->m_pRotations[ m_iCurrFrameRotation+1 ].m_fTime - m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime;
				if ( timeDiff <= 0 )
				{
					// key frames have the same time, should never happen but account for it anyway
					newRot = m_pAnim->m_pRotations[ m_iCurrFrameRotation+1 ].m_qRotation;
				}
				else
				{
					// interpolate between these key frames
					float t = (currtime - m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime) / timeDiff;
					m_pAnim->m_pRotations[ m_iCurrFrameRotation ].Interpolate( &(m_pAnim->m_pRotations[m_iCurrFrameRotation+1]), t, newRot );
				}
			}
		}
	}

	// scales
	if ( m_pAnim->m_iNumScales > 0 && m_pAnim->m_pScales[ 0 ].m_fTime <= currtime )
	{
		if ( m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime == currtime || m_pAnim->m_iNumScales == 1 )
		{
			// animation frame exactly matches time we are looking for, no interpolation required
			newScale = m_pAnim->m_pScales[ m_iCurrFrameScale ].m_scale;
		}
		else
		{
			if ( m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime < currtime )
			{
				if ( m_pAnim->m_pScales[ m_pAnim->m_iNumScales-1 ].m_fTime < currtime ) m_iCurrFrameScale = m_pAnim->m_iNumScales-1;

				if ( m_iCurrFrameScale < m_pAnim->m_iNumScales-1 )
				{
					// time is ahead of our pointer, advance until we find the right frame
					float keydiff = m_pAnim->m_pScales[ m_iCurrFrameScale+1 ].m_fTime - m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime;
					float currdiff = currtime - m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime;

					if ( keydiff == 0 || currdiff < keydiff*10 )
					{
						// frame should be quite close, use linear search
						while ( m_iCurrFrameScale < m_pAnim->m_iNumScales-1 
							 && m_pAnim->m_pScales[ m_iCurrFrameScale+1 ].m_fTime < currtime ) m_iCurrFrameScale++;
					}
					else
					{
						// frame might be quite far, use binary search
						int high = m_pAnim->m_iNumScales-1;
						int low = m_iCurrFrameScale;
						int result = 0;
						while( high >= low )
						{
							int mid = (high+low)/2;
							if( m_pAnim->m_pScales[ mid ].m_fTime > currtime ) high = mid-1;
							else if ( m_pAnim->m_pScales[ mid ].m_fTime < currtime ) low = mid+1;
							else { low = mid; high = mid-1; break; }
						}

						if( m_pAnim->m_pScales[ low ].m_fTime > currtime ) m_iCurrFrameScale = low-1;
						else m_iCurrFrameScale = low;
					}
				}
			}
			else
			{
				if ( m_iCurrFrameScale > 0 )
				{
					// time is behind our pointer, go backwards until we find the right frame
					float keydiff = m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime - m_pAnim->m_pScales[ m_iCurrFrameScale-1 ].m_fTime;
					float currdiff = m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime - currtime;

					if ( keydiff == 0 || currdiff < keydiff*10 )
					{
						// frame should be quite close, use linear search
						while ( m_iCurrFrameScale > 0
							 && m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime > currtime ) m_iCurrFrameScale--;
					}
					else
					{
						// frame might be quite far, use binary search
						int high = m_iCurrFrameScale;
						int low = 0;
						int result = 0;
						while( high >= low )
						{
							int mid = (high+low)/2;
							if( m_pAnim->m_pScales[ mid ].m_fTime > currtime ) high = mid-1;
							else if ( m_pAnim->m_pScales[ mid ].m_fTime < currtime ) low = mid+1;
							else { low = mid; high = mid-1; break; }
						}

						if( m_pAnim->m_pScales[ low ].m_fTime > currtime ) m_iCurrFrameScale = low-1;
						else m_iCurrFrameScale = low;
					}
				}
			}

			if ( m_iCurrFrameScale == m_pAnim->m_iNumScales-1 )
			{
				// reached last keyframe and animation hasn't looped yet, must be some dead time for this bone
				newScale = m_pAnim->m_pScales[ m_iCurrFrameScale ].m_scale;
			}
			else
			{
				float timeDiff = m_pAnim->m_pScales[ m_iCurrFrameScale+1 ].m_fTime - m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime;
				if ( timeDiff <= 0 )
				{
					// key frames have the same time, should never happen but account for it anyway
					newScale = m_pAnim->m_pScales[ m_iCurrFrameScale+1 ].m_scale;
				}
				else
				{
					// interpolate between these key frames
					float t = (currtime - m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime) / timeDiff;
					m_pAnim->m_pScales[ m_iCurrFrameScale ].Interpolate( &(m_pAnim->m_pScales[m_iCurrFrameScale+1]), t, newScale );
				}
			}
		}
	}

	AGKVector finalPos, finalScale;
	AGKQuaternion finalRot;

	finalPos.Interpolate( m_tweenPosition, newPos, s );
	finalRot.Lerp( m_tweenRotation, newRot, s );
	finalScale.Interpolate( m_tweenScale, newScale, s );

	SetNodePosition( finalPos.x, finalPos.y, finalPos.z );
	SetNodeRotation( finalRot.w, finalRot.x, finalRot.y, finalRot.z );
	SetNodeScale( finalScale.x, finalScale.y, finalScale.z );
}

void Bone3D::Interpolate( float currtime )
{
	if ( m_pAnim && (m_bFlags & AGK_BONE_ANIMATE) )
	{
		AGKVector newPos = pos();
		AGKQuaternion newRot = rot();
		AGKVector newScale = scale();

		if ( m_iCurrFramePosition >= m_pAnim->m_iNumPositions ) m_iCurrFramePosition = 0;
		if ( m_iCurrFrameRotation >= m_pAnim->m_iNumRotations ) m_iCurrFrameRotation = 0;
		if ( m_iCurrFrameScale >= m_pAnim->m_iNumScales ) m_iCurrFrameScale = 0;

		// positions
		if ( m_pAnim->m_iNumPositions > 0 && m_pAnim->m_pPositions[ 0 ].m_fTime <= currtime )
		{
			if ( m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime == currtime || m_pAnim->m_iNumPositions == 1 )
			{
				// animation frame exactly matches time we are looking for, no interpolation required
				newPos = m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_position;
			}
			else
			{
				if ( m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime < currtime )
				{
					if ( m_pAnim->m_pPositions[ m_pAnim->m_iNumPositions-1 ].m_fTime < currtime ) m_iCurrFramePosition = m_pAnim->m_iNumPositions-1;

					// forwards
					if ( m_iCurrFramePosition < m_pAnim->m_iNumPositions-1 )
					{
						// time is ahead of our pointer, advance until we find the right frame
						float keydiff = m_pAnim->m_pPositions[ m_iCurrFramePosition+1 ].m_fTime - m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime;
						float currdiff = currtime - m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime;

						if ( keydiff == 0 || currdiff < keydiff*10 )
						{
							// frame should be quite close, use linear search
							while ( m_iCurrFramePosition < m_pAnim->m_iNumPositions-1 
								 && m_pAnim->m_pPositions[ m_iCurrFramePosition+1 ].m_fTime < currtime ) m_iCurrFramePosition++;
						}
						else
						{
							// frame might be quite far, use binary search
							int high = m_pAnim->m_iNumPositions-1;
							int low = m_iCurrFramePosition;
							int result = 0;
							while( high >= low )
							{
								int mid = (high+low)/2;
								if( m_pAnim->m_pPositions[ mid ].m_fTime > currtime ) high = mid-1;
								else if ( m_pAnim->m_pPositions[ mid ].m_fTime < currtime ) low = mid+1;
								else { low = mid; high = mid-1; break; }
							}

							if( m_pAnim->m_pPositions[ low ].m_fTime > currtime ) m_iCurrFramePosition = low-1;
							else m_iCurrFramePosition = low;
						}
					}
				}
				else
				{
					// backwards
					if ( m_iCurrFramePosition > 0 )
					{
						// time is behind our pointer, go backwards until we find the right frame
						float keydiff = m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime - m_pAnim->m_pPositions[ m_iCurrFramePosition-1 ].m_fTime;
						float currdiff = m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime - currtime;

						if ( keydiff == 0 || currdiff < keydiff*10 )
						{
							// frame should be quite close, use linear search
							while ( m_iCurrFramePosition > 0
								 && m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime > currtime ) m_iCurrFramePosition--;
						}
						else
						{
							// frame might be quite far, use binary search
							int high = m_iCurrFramePosition;
							int low = 0;
							int result = 0;
							while( high >= low )
							{
								int mid = (high+low)/2;
								if( m_pAnim->m_pPositions[ mid ].m_fTime > currtime ) high = mid-1;
								else if ( m_pAnim->m_pPositions[ mid ].m_fTime < currtime ) low = mid+1;
								else { low = mid; high = mid-1; break; }
							}

							if( m_pAnim->m_pPositions[ low ].m_fTime > currtime ) m_iCurrFramePosition = low-1;
							else m_iCurrFramePosition = low;
						}
					}
				}

				if ( m_iCurrFramePosition == m_pAnim->m_iNumPositions-1 )
				{
					// reached last keyframe and animation hasn't looped yet, must be some dead time for this bone
					newPos = m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_position;
				}
				else
				{
					float timeDiff = m_pAnim->m_pPositions[ m_iCurrFramePosition+1 ].m_fTime - m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime;
					if ( timeDiff <= 0 )
					{
						// key frames have the same time, should never happen but account for it anyway
						newPos = m_pAnim->m_pPositions[ m_iCurrFramePosition+1 ].m_position;
					}
					else
					{
						// interpolate between these key frames
						float t = (currtime - m_pAnim->m_pPositions[ m_iCurrFramePosition ].m_fTime) / timeDiff;
						m_pAnim->m_pPositions[ m_iCurrFramePosition ].Interpolate( &(m_pAnim->m_pPositions[m_iCurrFramePosition+1]), t, newPos );
					}
				}
			}
		}

		// rotation
		if ( m_pAnim->m_iNumRotations > 0 && m_pAnim->m_pRotations[ 0 ].m_fTime <= currtime )
		{
			if ( m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime == currtime || m_pAnim->m_iNumRotations == 1 )
			{
				// animation frame exactly matches time we are looking for, no interpolation required
				newRot = m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_qRotation;
			}
			else
			{
				if ( m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime < currtime )
				{
					if ( m_pAnim->m_pRotations[ m_pAnim->m_iNumRotations-1 ].m_fTime < currtime ) m_iCurrFrameRotation = m_pAnim->m_iNumRotations-1;

					// forwards
					if ( m_iCurrFrameRotation < m_pAnim->m_iNumRotations-1 )
					{
						// time is ahead of our pointer, advance until we find the right frame
						float keydiff = m_pAnim->m_pRotations[ m_iCurrFrameRotation+1 ].m_fTime - m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime;
						float currdiff = currtime - m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime;

						if ( keydiff == 0 || currdiff < keydiff*10 )
						{
							// frame should be quite close, use linear search
							while ( m_iCurrFrameRotation < m_pAnim->m_iNumRotations-1 
								 && m_pAnim->m_pRotations[ m_iCurrFrameRotation+1 ].m_fTime < currtime ) m_iCurrFrameRotation++;
						}
						else
						{
							// frame might be quite far, use binary search
							int high = m_pAnim->m_iNumRotations-1;
							int low = m_iCurrFrameRotation;
							int result = 0;
							while( high >= low )
							{
								int mid = (high+low)/2;
								if( m_pAnim->m_pRotations[ mid ].m_fTime > currtime ) high = mid-1;
								else if ( m_pAnim->m_pRotations[ mid ].m_fTime < currtime ) low = mid+1;
								else { low = mid; high = mid-1; break; }
							}

							if( m_pAnim->m_pRotations[ low ].m_fTime > currtime ) m_iCurrFrameRotation = low-1;
							else m_iCurrFrameRotation = low;
						}
					}
				}
				else
				{
					// backwards
					if ( m_iCurrFrameRotation > 0 )
					{
						// time is behind our pointer, go backwards until we find the right frame
						float keydiff = m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime - m_pAnim->m_pRotations[ m_iCurrFrameRotation-1 ].m_fTime;
						float currdiff = m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime - currtime;

						if ( keydiff == 0 || currdiff < keydiff*10 )
						{
							// frame should be quite close, use linear search
							while ( m_iCurrFrameRotation > 0
								 && m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime > currtime ) m_iCurrFrameRotation--;
						}
						else
						{
							// frame might be quite far, use binary search
							int high = m_iCurrFrameRotation;
							int low = 0;
							int result = 0;
							while( high >= low )
							{
								int mid = (high+low)/2;
								if( m_pAnim->m_pRotations[ mid ].m_fTime > currtime ) high = mid-1;
								else if ( m_pAnim->m_pRotations[ mid ].m_fTime < currtime ) low = mid+1;
								else { low = mid; high = mid-1; break; }
							}

							if( m_pAnim->m_pRotations[ low ].m_fTime > currtime ) m_iCurrFrameRotation = low-1;
							else m_iCurrFrameRotation = low;
						}
					}
				}

				if ( m_iCurrFrameRotation == m_pAnim->m_iNumRotations-1 )
				{
					// reached last keyframe and animation hasn't looped yet, must be some dead time for this bone
					newRot = m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_qRotation;
				}
				else
				{
					float timeDiff = m_pAnim->m_pRotations[ m_iCurrFrameRotation+1 ].m_fTime - m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime;
					if ( timeDiff <= 0 )
					{
						// key frames have the same time, should never happen but account for it anyway
						newRot = m_pAnim->m_pRotations[ m_iCurrFrameRotation+1 ].m_qRotation;
					}
					else
					{
						// interpolate between these key frames
						float t = (currtime - m_pAnim->m_pRotations[ m_iCurrFrameRotation ].m_fTime) / timeDiff;
						m_pAnim->m_pRotations[ m_iCurrFrameRotation ].Interpolate( &(m_pAnim->m_pRotations[m_iCurrFrameRotation+1]), t, newRot );
					}
				}
			}
		}

		// scales
		if ( m_pAnim->m_iNumScales > 0 && m_pAnim->m_pScales[ 0 ].m_fTime <= currtime )
		{
			if ( m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime == currtime || m_pAnim->m_iNumScales == 1 )
			{
				// animation frame exactly matches time we are looking for, no interpolation required
				newScale = m_pAnim->m_pScales[ m_iCurrFrameScale ].m_scale;
			}
			else
			{
				if ( m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime < currtime )
				{
					if ( m_pAnim->m_pScales[ m_pAnim->m_iNumScales-1 ].m_fTime < currtime ) m_iCurrFrameScale = m_pAnim->m_iNumScales-1;

					// forwards
					if ( m_iCurrFrameScale < m_pAnim->m_iNumScales-1 )
					{
						// time is ahead of our pointer, advance until we find the right frame
						float keydiff = m_pAnim->m_pScales[ m_iCurrFrameScale+1 ].m_fTime - m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime;
						float currdiff = currtime - m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime;

						if ( keydiff == 0 || currdiff < keydiff*10 )
						{
							// frame should be quite close, use linear search
							while ( m_iCurrFrameScale < m_pAnim->m_iNumScales-1 
								 && m_pAnim->m_pScales[ m_iCurrFrameScale+1 ].m_fTime < currtime ) m_iCurrFrameScale++;
						}
						else
						{
							// frame might be quite far, use binary search
							int high = m_pAnim->m_iNumScales-1;
							int low = m_iCurrFrameScale;
							int result = 0;
							while( high >= low )
							{
								int mid = (high+low)/2;
								if( m_pAnim->m_pScales[ mid ].m_fTime > currtime ) high = mid-1;
								else if ( m_pAnim->m_pScales[ mid ].m_fTime < currtime ) low = mid+1;
								else { low = mid; high = mid-1; break; }
							}

							if( m_pAnim->m_pScales[ low ].m_fTime > currtime ) m_iCurrFrameScale = low-1;
							else m_iCurrFrameScale = low;
						}
					}
				}
				else
				{
					// backwards
					if ( m_iCurrFrameScale > 0 )
					{
						// time is behind our pointer, go backwards until we find the right frame
						float keydiff = m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime - m_pAnim->m_pScales[ m_iCurrFrameScale-1 ].m_fTime;
						float currdiff = m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime - currtime;

						if ( keydiff == 0 || currdiff < keydiff*10 )
						{
							// frame should be quite close, use linear search
							while ( m_iCurrFrameScale > 0
								 && m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime > currtime ) m_iCurrFrameScale--;
						}
						else
						{
							// frame might be quite far, use binary search
							int high = m_iCurrFrameScale;
							int low = 0;
							int result = 0;
							while( high >= low )
							{
								int mid = (high+low)/2;
								if( m_pAnim->m_pScales[ mid ].m_fTime > currtime ) high = mid-1;
								else if ( m_pAnim->m_pScales[ mid ].m_fTime < currtime ) low = mid+1;
								else { low = mid; high = mid-1; break; }
							}

							if( m_pAnim->m_pScales[ low ].m_fTime > currtime ) m_iCurrFrameScale = low-1;
							else m_iCurrFrameScale = low;
						}
					}
				}

				if ( m_iCurrFrameScale == m_pAnim->m_iNumScales-1 )
				{
					// reached last keyframe and animation hasn't looped yet, must be some dead time for this bone
					newScale = m_pAnim->m_pScales[ m_iCurrFrameScale ].m_scale;
				}
				else
				{
					float timeDiff = m_pAnim->m_pScales[ m_iCurrFrameScale+1 ].m_fTime - m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime;
					if ( timeDiff <= 0 )
					{
						// key frames have the same time, should never happen but account for it anyway
						newScale = m_pAnim->m_pScales[ m_iCurrFrameScale+1 ].m_scale;
					}
					else
					{
						// interpolate between these key frames
						float t = (currtime - m_pAnim->m_pScales[ m_iCurrFrameScale ].m_fTime) / timeDiff;
						m_pAnim->m_pScales[ m_iCurrFrameScale ].Interpolate( &(m_pAnim->m_pScales[m_iCurrFrameScale+1]), t, newScale );
					}
				}
			}
		}

		SetNodePosition( newPos.x, newPos.y, newPos.z );
		SetNodeRotation( newRot.w, newRot.x, newRot.y, newRot.z );
		SetNodeScale( newScale.x, newScale.y, newScale.z );
	}
}

// Bone animation

Anim3DBone::Anim3DBone()
{
	m_iNumRotations = 0;
	m_pRotations = 0;

	m_iNumPositions = 0;
	m_pPositions = 0;

	m_iNumScales = 0;
	m_pScales = 0;
}

void Anim3DBone::Copy( const Anim3DBone *pOther )
{
	if ( m_pRotations ) delete [] m_pRotations;
	if ( m_pPositions ) delete [] m_pPositions;
	if ( m_pScales ) delete [] m_pScales;

	m_sBoneName.SetStr( pOther->m_sBoneName.GetStr() );

	m_iNumRotations = pOther->m_iNumRotations;
	m_pRotations = 0;
	if ( m_iNumRotations > 0 )
	{
		m_pRotations = new Anim3DKeyFrameRotation[ m_iNumRotations ];
		for ( UINT i = 0; i < m_iNumRotations; i++ )
		{
			m_pRotations[ i ].m_fTime = pOther->m_pRotations[ i ].m_fTime;
			m_pRotations[ i ].m_qRotation = pOther->m_pRotations[ i ].m_qRotation;
		}
	}

	m_iNumPositions = pOther->m_iNumPositions;
	m_pPositions = 0;
	if ( m_iNumPositions > 0 )
	{
		m_pPositions = new Anim3DKeyFramePosition[ m_iNumPositions ];
		for ( UINT i = 0; i < m_iNumPositions; i++ )
		{
			m_pPositions[ i ].m_fTime = pOther->m_pPositions[ i ].m_fTime;
			m_pPositions[ i ].m_position = pOther->m_pPositions[ i ].m_position;
		}
	}

	m_iNumScales = pOther->m_iNumScales;
	m_pScales = 0;
	if ( m_iNumScales > 0 )
	{
		m_pScales = new Anim3DKeyFrameScale[ m_iNumScales ];
		for ( UINT i = 0; i < m_iNumScales; i++ )
		{
			m_pScales[ i ].m_fTime = pOther->m_pScales[ i ].m_fTime;
			m_pScales[ i ].m_scale = pOther->m_pScales[ i ].m_scale;
		}
	}
}

Anim3DBone::~Anim3DBone()
{
	if ( m_pRotations ) delete [] m_pRotations;
	if ( m_pPositions ) delete [] m_pPositions;
	if ( m_pScales ) delete [] m_pScales;
}

// Animation

Animation3D::Animation3D()
{
	m_iID = 0;
	m_iFlags = 0;
	m_iReferences = 1;

	m_fTime = 0;
			
	m_iNumBones = 0;
	m_pBoneAnims = 0;

	m_pBoneSearchList = 0;
}

Animation3D::Animation3D( const Animation3D *pOther )
{
	m_iID = 0;
	m_iFlags = pOther->m_iFlags;
	m_iReferences = 1;

	m_sName.SetStr( pOther->m_sName );
	m_fTime = pOther->m_fTime;
			
	m_iNumBones = pOther->m_iNumBones;
	m_pBoneAnims = 0;
	m_pBoneSearchList = 0;
	if ( m_iNumBones > 0 )
	{
		m_pBoneAnims = new Anim3DBone[ m_iNumBones ];
		m_pBoneSearchList = new sSearchItem[ m_iNumBones ];
		for ( UINT i = 0; i < m_iNumBones; i++ )
		{
			m_pBoneAnims[ i ].Copy( &(pOther->m_pBoneAnims[i]) );
			m_pBoneSearchList[ i ].m_iIndex = pOther->m_pBoneSearchList[ i ].m_iIndex;
		}

		// don't combine with previous loop, need bones set up first
		for ( UINT i = 0; i < m_iNumBones; i++ )
		{
			m_pBoneSearchList[ i ].m_sName = m_pBoneAnims[ m_pBoneSearchList[i].m_iIndex ].m_sBoneName.GetStr();
		}
	}	
}

Animation3D::~Animation3D()
{
	if ( m_pBoneAnims ) delete [] m_pBoneAnims;
	if ( m_pBoneSearchList ) delete [] m_pBoneSearchList;
}

void Animation3D::RemoveRef()
{
	m_iReferences--;
	if ( m_iReferences < 0 ) 
	{
		agk::Error( "Internal Error - Animation has been released too many times" );
	}

	if ( m_iReferences <= 0 ) delete this;
}

Anim3DBone* Animation3D::GetAnimForBone( const char* name )
{
	/*
	// linear search
	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		if ( m_pBoneAnims[ i ]->m_sName.CompareTo( name ) == 0 ) return m_pBoneAnims+i;
	}
	*/

	// binary search
	int high = m_iNumBones-1;
	int low = 0;
	int result = 0;
	while( high >= low )
	{
		int mid = (high+low)/2;
		result = strcmp( m_pBoneSearchList[ mid ].m_sName, name );
		if( result > 0 ) high = mid-1;
		else if ( result < 0 ) low = mid+1;
		else return m_pBoneAnims + m_pBoneSearchList[ mid ].m_iIndex;
	}

	return 0;
}

void Animation3D::LoadFromScene( aiAnimation *pAnim )
{
	if ( m_pBoneAnims ) delete [] m_pBoneAnims;
	m_pBoneAnims = 0;

	m_sName.SetStr( pAnim->mName.C_Str() );
	m_fTime = (float) pAnim->mDuration;
	m_iNumBones = pAnim->mNumChannels;
	if ( m_iNumBones == 0 ) return;

	m_pBoneAnims = new Anim3DBone[ m_iNumBones ];

	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBoneAnims[ i ].m_sBoneName.SetStr( pAnim->mChannels[ i ]->mNodeName.C_Str() );

		m_pBoneAnims[ i ].m_iNumPositions = pAnim->mChannels[ i ]->mNumPositionKeys;
		if ( m_pBoneAnims[ i ].m_iNumPositions > 0 )
		{
			m_pBoneAnims[ i ].m_pPositions = new Anim3DKeyFramePosition[ m_pBoneAnims[ i ].m_iNumPositions ];
			for ( UINT j = 0; j < m_pBoneAnims[ i ].m_iNumPositions; j++ )
			{
				m_pBoneAnims[ i ].m_pPositions[ j ].m_fTime = (float) pAnim->mChannels[ i ]->mPositionKeys[ j ].mTime;
				m_pBoneAnims[ i ].m_pPositions[ j ].m_position.x = pAnim->mChannels[ i ]->mPositionKeys[ j ].mValue.x;
				m_pBoneAnims[ i ].m_pPositions[ j ].m_position.y = pAnim->mChannels[ i ]->mPositionKeys[ j ].mValue.y;
				m_pBoneAnims[ i ].m_pPositions[ j ].m_position.z = pAnim->mChannels[ i ]->mPositionKeys[ j ].mValue.z;
			}
		}

		m_pBoneAnims[ i ].m_iNumRotations = pAnim->mChannels[ i ]->mNumRotationKeys;
		if ( m_pBoneAnims[ i ].m_iNumRotations > 0 )
		{
			m_pBoneAnims[ i ].m_pRotations = new Anim3DKeyFrameRotation[ m_pBoneAnims[ i ].m_iNumRotations ];
			for ( UINT j = 0; j < m_pBoneAnims[ i ].m_iNumRotations; j++ )
			{
				m_pBoneAnims[ i ].m_pRotations[ j ].m_fTime = (float) pAnim->mChannels[ i ]->mRotationKeys[ j ].mTime;
				m_pBoneAnims[ i ].m_pRotations[ j ].m_qRotation.w = pAnim->mChannels[ i ]->mRotationKeys[ j ].mValue.w;
				m_pBoneAnims[ i ].m_pRotations[ j ].m_qRotation.x = pAnim->mChannels[ i ]->mRotationKeys[ j ].mValue.x;
				m_pBoneAnims[ i ].m_pRotations[ j ].m_qRotation.y = pAnim->mChannels[ i ]->mRotationKeys[ j ].mValue.y;
				m_pBoneAnims[ i ].m_pRotations[ j ].m_qRotation.z = pAnim->mChannels[ i ]->mRotationKeys[ j ].mValue.z;
			}
		}

		m_pBoneAnims[ i ].m_iNumScales = pAnim->mChannels[ i ]->mNumScalingKeys;
		if ( m_pBoneAnims[ i ].m_iNumScales > 0 )
		{
			m_pBoneAnims[ i ].m_pScales = new Anim3DKeyFrameScale[ m_pBoneAnims[ i ].m_iNumScales ];
			for ( UINT j = 0; j < m_pBoneAnims[ i ].m_iNumScales; j++ )
			{
				static int warned = 0;
				AGKVector scale;
				scale.x = pAnim->mChannels[ i ]->mScalingKeys[ j ].mValue.x;
				scale.y = pAnim->mChannels[ i ]->mScalingKeys[ j ].mValue.y;
				scale.z = pAnim->mChannels[ i ]->mScalingKeys[ j ].mValue.z;
				if ( warned == 0 && (agk::Abs(scale.x-1) > 0.01f || agk::Abs(scale.y-1) > 0.01f || agk::Abs(scale.z-1) > 0.01f) )
				{
					warned = 1;
					agk::Warning( "Object animation contains scale values, scaling of bones is not supported and will be ignored" );
				}
				m_pBoneAnims[ i ].m_pScales[ j ].m_fTime = (float) pAnim->mChannels[ i ]->mScalingKeys[ j ].mTime;
				m_pBoneAnims[ i ].m_pScales[ j ].m_scale.Set( 1,1,1 );
				//m_pBoneAnims[ i ].m_pScales[ j ].m_scale.x = pAnim->mChannels[ i ]->mScalingKeys[ j ].mValue.x;
				//m_pBoneAnims[ i ].m_pScales[ j ].m_scale.y = pAnim->mChannels[ i ]->mScalingKeys[ j ].mValue.y;
				//m_pBoneAnims[ i ].m_pScales[ j ].m_scale.z = pAnim->mChannels[ i ]->mScalingKeys[ j ].mValue.z;
			}
		}
	}

	// create sorted list of names an indices to make searching by name faster
	if ( m_pBoneSearchList ) delete [] m_pBoneSearchList;
	m_pBoneSearchList = new sSearchItem[ m_iNumBones ];
	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBoneSearchList[ i ].m_sName = m_pBoneAnims[i].m_sBoneName.GetStr();
		m_pBoneSearchList[ i ].m_iIndex = i;
	}

	qsort( m_pBoneSearchList, m_iNumBones, sizeof(sSearchItem), &CompareSearchItem );
}

void Animation3D::ScaleFrames( float x, float y, float z )
{
	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		for ( UINT j = 0; j < m_pBoneAnims[ i ].m_iNumPositions; j++ )
		{
			m_pBoneAnims[ i ].m_pPositions[ j ].m_position.Scale( x,y,z );
		}
	}
}

// Skeleton

Skeleton3D::Skeleton3D()
{
	m_iNumBones = 0;
	m_pBones = 0;
	m_pBoneSearchList = 0;

	m_iID = 0;
	m_bFlags = 0;
	m_fCurrTime = 0;
	m_fTweenTime = 0;
	m_fTotalTweenTime = 0;
	m_fSpeed = 1.0f;
	m_pCurrAnim = 0;
	m_iLoopCount = 0;
	m_iLoopTotal = 0;
	m_fLoopStartTime = 0;
	m_fLoopEndTime = 0;
}

Skeleton3D::Skeleton3D( const Skeleton3D *pOther )
{
	m_iNumBones = pOther->m_iNumBones;
	if ( m_iNumBones > 0 )
	{
		m_pBones = new Bone3D*[ m_iNumBones ];
		m_pBoneSearchList = new sSearchItem[ m_iNumBones ];
	}
	else
	{
		m_pBones = 0;
		m_pBoneSearchList = 0;
	}

	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBones[ i ] = new Bone3D( pOther->m_pBones[i] );
		if ( pOther->m_pBones[i]->m_pParent )
		{
			m_pBones[ i ]->m_pParent = m_pBones[ pOther->m_pBones[i]->m_pParent->m_iIndex ];
			m_pBones[ i ]->m_pParent->AddChild( m_pBones[i] );
		}
	}

	// don't merge this loop with the previous one, we need all the bones set up first
	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBoneSearchList[ i ].m_iIndex = pOther->m_pBoneSearchList[ i ].m_iIndex;
		m_pBoneSearchList[ i ].m_sName = m_pBones[ m_pBoneSearchList[ i ].m_iIndex ]->m_sName.GetStr();
	}

	// don't copy playing animations
	m_iID = 0;
	m_bFlags = pOther->m_bFlags;
	m_bFlags &= ~(AGK_SKELETON3D_PLAYING | AGK_SKELETON3D_LOOPING); // don't play automatically
	m_fCurrTime = 0;
	m_fTweenTime = 0;
	m_fTotalTweenTime = 0;
	m_fSpeed = pOther->m_fSpeed;
	m_pCurrAnim = 0;
	m_iLoopCount = 0;
	m_iLoopTotal = 0;
	m_fLoopStartTime = 0;
	m_fLoopEndTime = 0;
}

Skeleton3D::~Skeleton3D()
{
	if ( m_iNumBones > 0 && m_pBones )
	{
		for( UINT i = 0; i < m_iNumBones; i++ ) delete m_pBones[i];
		delete [] m_pBones;
	}

	if ( m_pBoneSearchList ) delete [] m_pBoneSearchList;
}

Bone3D* Skeleton3D::GetBone( const char* name )
{
	// binary search
	int high = m_iNumBones-1;
	int low = 0;
	int result = 0;
	while( high >= low )
	{
		int mid = (high+low)/2;
		result = strcmp( m_pBoneSearchList[ mid ].m_sName, name );
		if( result > 0 ) high = mid-1;
		else if ( result < 0 ) low = mid+1;
		else return m_pBones[ m_pBoneSearchList[ mid ].m_iIndex ];
	}

	return 0;
}

Bone3D* Skeleton3D::GetBone( UINT index )
{
	if ( index >= m_iNumBones ) return 0;
	return m_pBones[ index ];
}

int Skeleton3D::GetBoneIndex( const char* name )
{
	/*
	// linear search
	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		if ( m_pBones[ i ]->m_sName.CompareTo( name ) == 0 ) return i;
	}
	*/

	// binary search
	int high = m_iNumBones-1;
	int low = 0;
	int result = 0;
	while( high >= low )
	{
		int mid = (high+low)/2;
		result = strcmp( m_pBoneSearchList[ mid ].m_sName, name );
		if( result > 0 ) high = mid-1;
		else if ( result < 0 ) low = mid+1;
		else return m_pBoneSearchList[ mid ].m_iIndex;
	}

	return -1;
}

void Skeleton3D::SetRoot( cNode *pNode )
{
	if ( !pNode ) return;

	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		if ( !m_pBones[ i ]->m_pParent )
		{
			pNode->AddChild( m_pBones[ i ] );
		}
	}
}

void Skeleton3D::SetBones( std::vector<Bone3D*> &boneList )
{
	if ( m_iNumBones > 0 && m_pBones )
	{
		for( UINT i = 0; i < m_iNumBones; i++ ) delete m_pBones[i];
		delete [] m_pBones;
	}

	m_iNumBones = boneList.size();
	if ( m_iNumBones > AGK_OBJECT_MAX_BONES ) 
	{
		m_iNumBones = AGK_OBJECT_MAX_BONES;
		uString info;
		info.Format( "AGK has a limit of %d bones per object, extra bones will be ignored", AGK_OBJECT_MAX_BONES );
		agk::Warning( info );
	}
	m_pBones = new Bone3D*[ m_iNumBones ];
	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBones[ i ] = boneList[ i ];
		m_pBones[ i ]->m_iIndex = i;
	}

	// create sorted list of names an indices to make searching by name faster
	if ( m_pBoneSearchList ) delete [] m_pBoneSearchList;
	m_pBoneSearchList = new sSearchItem[ m_iNumBones ];
	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBoneSearchList[ i ].m_sName = m_pBones[i]->m_sName.GetStr();
		m_pBoneSearchList[ i ].m_iIndex = i;
	}

	qsort( m_pBoneSearchList, m_iNumBones, sizeof(sSearchItem), &CompareSearchItem );
}

void Skeleton3D::ScaleBones( float x, float y, float z )
{
	AGKVector vScale( x,y,z );

	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBones[ i ]->SetNodePosition( m_pBones[ i ]->pos().x*vScale.x, m_pBones[ i ]->pos().y*vScale.y, m_pBones[ i ]->pos().z*vScale.z );
		m_pBones[ i ]->m_offsetPosition.Scale( x,y,z );
		m_pBones[ i ]->m_origPosition.Scale( x,y,z );
	}
}

void Skeleton3D::FixBonePivots( const AGKVector &pos, const AGKQuaternion &rot )
{
	AGKQuaternion q = rot;
	q.Invert();

	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		AGKVector p = q * -pos;
		m_pBones[ i ]->m_offsetPosition = m_pBones[ i ]->m_offsetRotation * p + m_pBones[ i ]->m_offsetPosition;
		m_pBones[ i ]->m_offsetRotation = m_pBones[ i ]->m_offsetRotation * q;

		m_pBones[ i ]->m_origPosition = rot * m_pBones[ i ]->m_origPosition;
		m_pBones[ i ]->m_origPosition += pos;
		m_pBones[ i ]->m_origRotation = rot * m_pBones[ i ]->m_origRotation;
	}
}

void Skeleton3D::ResetBoundingBoxes()
{
	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBones[i]->m_BoundingBox.resetBox();
	}
}

void Skeleton3D::DiscoverBounds( cMesh *pMesh )
{
	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		pMesh->GetBoundingBoxForBone( i, m_pBones[i], &(m_pBones[i]->m_BoundingBox) );
	}
}

void Skeleton3D::PlayAnimation( Animation3D *pAnim, float starttime, float endtime, int loop, float tweentime )
{
	if ( !pAnim ) return;
	if ( tweentime < 0 ) tweentime = 0;
	if ( loop < 0 ) loop = 0;
	
	if ( starttime < 0 ) starttime = 0;
	if ( starttime > pAnim->GetDuration() ) starttime = pAnim->GetDuration();

	if ( endtime < 0 ) endtime = pAnim->GetDuration();
	if ( endtime < starttime ) endtime = starttime;
	if ( endtime > pAnim->GetDuration() ) endtime = pAnim->GetDuration();

	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBones[ i ]->m_tweenPosition = m_pBones[ i ]->pos();
		m_pBones[ i ]->m_tweenRotation = m_pBones[ i ]->rot();
		m_pBones[ i ]->m_tweenScale = m_pBones[ i ]->scale();
		m_pBones[ i ]->m_pAnim = pAnim->GetAnimForBone( m_pBones[ i ]->m_sName );
	}

	m_fTweenTime = tweentime;
	m_fTotalTweenTime = tweentime;
	m_fCurrTime = starttime;
	m_fLoopStartTime = starttime;
	m_fLoopEndTime = endtime;

	m_pCurrAnim = pAnim;

	if ( m_fLoopEndTime - m_fLoopStartTime < 0.00001f ) 
	{
		Update(0);
	}
	else
	{
		m_bFlags |= AGK_SKELETON3D_PLAYING;
		if ( loop > 0 ) 
		{
			if ( loop == 1 ) m_bFlags |= AGK_SKELETON3D_LOOPING;
			else
			{
				m_bFlags &= ~AGK_SKELETON3D_LOOPING;
				m_iLoopTotal = loop;
				m_iLoopCount = 0;
			}
		}
		else 
		{
			m_bFlags &= ~AGK_SKELETON3D_LOOPING;
			m_iLoopTotal = 0;
			m_iLoopCount = 0;
		}
	}
}

void Skeleton3D::SetAnimationFrame( Animation3D *pAnim, float time, float tweentime )
{
	if ( !pAnim ) return;
	if ( tweentime < 0 ) tweentime = 0;
	if ( time < 0 ) time = 0;

	for ( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBones[ i ]->m_tweenPosition = m_pBones[ i ]->pos();
		m_pBones[ i ]->m_tweenRotation = m_pBones[ i ]->rot();
		m_pBones[ i ]->m_tweenScale = m_pBones[ i ]->scale();
		m_pBones[ i ]->m_pAnim = pAnim->GetAnimForBone( m_pBones[ i ]->m_sName );
	}

	m_fTweenTime = tweentime;
	m_fTotalTweenTime = tweentime;
	m_fCurrTime = time;

	m_pCurrAnim = pAnim;

	if ( (m_bFlags & AGK_SKELETON3D_PLAYING) == 0 )
	{
		Update(0);
	}
}

void Skeleton3D::SetAnimationSpeed( float speed )
{
	m_fSpeed = speed;
}

void Skeleton3D::StopAnimation()
{
	m_bFlags &= ~AGK_SKELETON3D_PLAYING;
	m_bFlags &= ~AGK_SKELETON3D_LOOPING;
	m_iLoopTotal = 0;
	m_iLoopCount = 0;
	m_fTweenTime = 0;
	m_fTotalTweenTime = 0;
	m_pCurrAnim = 0;
	m_fCurrTime = 0;
}

void Skeleton3D::ResetAnimation()
{
	StopAnimation();
	for( UINT i = 0; i < m_iNumBones; i++ )
	{
		m_pBones[ i ]->ResetToOrig();
	}
}

int Skeleton3D::GetIsAnimating()
{
	return (m_bFlags & AGK_SKELETON3D_PLAYING) ? 1 : 0;
}

int Skeleton3D::GetIsTweening()
{
	return (m_fTweenTime > 0) ? 1 : 0;
}

float Skeleton3D::GetAnimationTime()
{
	return m_fCurrTime;
}

void Skeleton3D::Update( float time )
{
	if ( m_fTweenTime > 0 && m_fTotalTweenTime > 0 )
	{
		// blend to the first frame of animation over time
		m_fTweenTime -= time;
		if ( m_fTweenTime < 0 ) m_fTweenTime = 0;

		float t = 1 - (m_fTweenTime / m_fTotalTweenTime);
		
		for( UINT i = 0; i < m_iNumBones; i++ )
		{
			m_pBones[ i ]->Tween( m_fCurrTime, t );
		}
	}
	else
	{
		// interpolate the animation
		if ( m_pCurrAnim )
		{
			if ( m_bFlags & AGK_SKELETON3D_PLAYING )
			{
				m_fCurrTime += time*m_fSpeed;
				if ( m_fCurrTime > m_fLoopEndTime )
				{
					m_iLoopCount++;
					if ( (m_bFlags & AGK_SKELETON3D_LOOPING) || (m_iLoopTotal > 0 && m_iLoopCount < m_iLoopTotal) )
					{
						if ( m_fLoopEndTime-m_fLoopStartTime < 0.000001f ) m_fCurrTime = m_fLoopEndTime;
						else while ( m_fCurrTime > m_fLoopEndTime ) m_fCurrTime -= (m_fLoopEndTime-m_fLoopStartTime);
					}
					else
					{
						m_fCurrTime = m_fLoopEndTime;
						m_bFlags &= ~AGK_SKELETON3D_PLAYING;
					}
				}
				else if ( m_fCurrTime < m_fLoopStartTime )
				{
					m_iLoopCount++;
					if ( (m_bFlags & AGK_SKELETON3D_LOOPING) || (m_iLoopTotal > 0 && m_iLoopCount < m_iLoopTotal) )
					{
						if ( m_fLoopEndTime-m_fLoopStartTime < 0.000001f ) m_fCurrTime = m_fLoopStartTime;
						else while ( m_fCurrTime < m_fLoopStartTime ) m_fCurrTime += (m_fLoopEndTime-m_fLoopStartTime);
					}
					else 
					{
						m_fCurrTime = m_fLoopStartTime;
						m_bFlags &= ~AGK_SKELETON3D_PLAYING;
					}
				}
			}

			for( UINT i = 0; i < m_iNumBones; i++ )
			{
				m_pBones[ i ]->Interpolate( m_fCurrTime );
			}

			if ( (m_bFlags & AGK_SKELETON3D_PLAYING) == 0 ) m_pCurrAnim = 0;
		}		
	}
}