
case AGKI_WORLDTOSCREENX_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::WorldToScreenX( param0 );
    break;
}
case AGKI_WORLDTOSCREENY_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::WorldToScreenY( param0 );
    break;
}
case AGKI_SCREENTOWORLDX_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ScreenToWorldX( param0 );
    break;
}
case AGKI_SCREENTOWORLDY_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ScreenToWorldY( param0 );
    break;
}
case AGKI_SETRESOLUTIONMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetResolutionMode( param0 );
    break;
}
case AGKI_SETSYNCRATE_0_F_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetSyncRate( param0, param1 );
    break;
}
case AGKI_SETORIENTATIONALLOWED_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetOrientationAllowed( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTRANSITIONMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTransitionMode( param0 );
    break;
}
case AGKI_SETDISPLAYASPECT_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetDisplayAspect( param0 );
    break;
}
case AGKI_SETRENDERTOIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetRenderToImage( param0, param1 );
    break;
}
case AGKI_SETRENDERTOSCREEN_0_0:
{
    agk::SetRenderToScreen( );
    break;
}
case AGKI_GETCL_S_0:
{
    char *szReturnStr = agk::GetCL( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETSCISSOR_0_F_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetScissor( param0, param1, param2, param3 );
    break;
}
case AGKI_SETINTENDEDDEVICESIZE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetIntendedDeviceSize( param0, param1 );
    break;
}
case AGKI_USENEWDEFAULTFONTS_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::UseNewDefaultFonts( param0 );
    break;
}
case AGKI_GETDEVICEWIDTH_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetDeviceWidth( );
    break;
}
case AGKI_GETDEVICEHEIGHT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetDeviceHeight( );
    break;
}
case AGKI_SETVIRTUALRESOLUTION_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualResolution( param0, param1 );
    break;
}
case AGKI_UPDATEDEVICESIZE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::UpdateDeviceSize( param0, param1 );
    break;
}
case AGKI_GETVIRTUALWIDTH_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetVirtualWidth( );
    break;
}
case AGKI_GETVIRTUALHEIGHT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetVirtualHeight( );
    break;
}
case AGKI_GETSCREENBOUNDSLEFT_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetScreenBoundsLeft( );
    break;
}
case AGKI_GETSCREENBOUNDSRIGHT_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetScreenBoundsRight( );
    break;
}
case AGKI_GETSCREENBOUNDSTOP_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetScreenBoundsTop( );
    break;
}
case AGKI_GETSCREENBOUNDSBOTTOM_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetScreenBoundsBottom( );
    break;
}
case AGKI_GETDISPLAYASPECT_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetDisplayAspect( );
    break;
}
case AGKI_GETORIENTATION_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetOrientation( );
    break;
}
case AGKI_GETPAUSED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetPaused( );
    break;
}
case AGKI_GETRESUMED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetResumed( );
    break;
}
case AGKI_SETDEFAULTMAGFILTER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetDefaultMagFilter( param0 );
    break;
}
case AGKI_SETDEFAULTMINFILTER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetDefaultMinFilter( param0 );
    break;
}
case AGKI_SETDEFAULTWRAPU_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetDefaultWrapU( param0 );
    break;
}
case AGKI_SETDEFAULTWRAPV_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetDefaultWrapV( param0 );
    break;
}
case AGKI_SETVIEWOFFSET_0_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetViewOffset( param0, param1 );
    break;
}
case AGKI_SETVIEWZOOM_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetViewZoom( param0 );
    break;
}
case AGKI_SETVIEWZOOMMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetViewZoomMode( param0 );
    break;
}
case AGKI_GETVIEWOFFSETX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetViewOffsetX( );
    break;
}
case AGKI_GETVIEWOFFSETY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetViewOffsetY( );
    break;
}
case AGKI_GETVIEWZOOM_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetViewZoom( );
    break;
}
case AGKI_SYNC_0_0:
{
    agk::Sync( );
    break;
}
case AGKI_BREAK_0_0:
{
    agk::Break( );
    break;
}
case AGKI_STEPPHYSICS_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::StepPhysics( param0 );
    break;
}
case AGKI_UPDATE_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::Update( param0 );
    break;
}
case AGKI_UPDATE2D_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::Update2D( param0 );
    break;
}
case AGKI_UPDATE3D_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::Update3D( param0 );
    break;
}
case AGKI_RENDER2DBACK_0_0:
{
    agk::Render2DBack( );
    break;
}
case AGKI_RENDER_0_0:
{
    agk::Render( );
    break;
}
case AGKI_RENDER2DFRONT_0_0:
{
    agk::Render2DFront( );
    break;
}
case AGKI_RENDER3D_0_0:
{
    agk::Render3D( );
    break;
}
case AGKI_SWAP_0_0:
{
    agk::Swap( );
    break;
}
case AGKI_CLEARSCREEN_0_0:
{
    agk::ClearScreen( );
    break;
}
case AGKI_CLEARDEPTHBUFFER_0_0:
{
    agk::ClearDepthBuffer( );
    break;
}
case AGKI_SETGENERATEMIPMAPS_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetGenerateMipmaps( param0 );
    break;
}
case AGKI_TIMER_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::Timer( );
    break;
}
case AGKI_RESETTIMER_0_0:
{
    agk::ResetTimer( );
    break;
}
case AGKI_GETRUNTIME_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRunTime( );
    break;
}
case AGKI_GETSECONDS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetSeconds( );
    break;
}
case AGKI_GETFRAMETIME_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetFrameTime( );
    break;
}
case AGKI_GETMILLISECONDS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetMilliseconds( );
    break;
}
case AGKI_GETDEVICEID_S_0:
{
    char *szReturnStr = agk::GetDeviceID( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETNUMPROCESSORS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetNumProcessors( );
    break;
}
case AGKI_SHA1_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::Sha1( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SHA256_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::Sha256( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SHA512_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::Sha512( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_HTTPENCODE_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::HTTPEncode( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_HTTPDECODE_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::HTTPDecode( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETRANDOMSEED_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetRandomSeed( param0 );
    break;
}
case AGKI_RANDOM_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::Random( );
    break;
}
case AGKI_RANDOM_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Random( param0, param1 );
    break;
}
case AGKI_RANDOMSIGN_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::RandomSign( param0 );
    break;
}
case AGKI_SETRANDOMSEED2_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetRandomSeed2( param0 );
    break;
}
case AGKI_RANDOM2_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::Random2( );
    break;
}
case AGKI_RANDOM2_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Random2( param0, param1 );
    break;
}
case AGKI_ATANFULL_F_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ATanFull( param0, param1 );
    break;
}
case AGKI_ATANFULLRAD_F_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ATanFullRad( param0, param1 );
    break;
}
case AGKI_SETSORTTEXTURES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSortTextures( param0 );
    break;
}
case AGKI_SETSORTDEPTH_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSortDepth( param0 );
    break;
}
case AGKI_SETSORTTRANSPARENTDEPTH_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSortTransparentDepth( param0 );
    break;
}
case AGKI_SETSORTCREATED_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSortCreated( param0 );
    break;
}
case AGKI_SCREENFPS_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::ScreenFPS( );
    break;
}
case AGKI_GETPOLYGONSDRAWN_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetPolygonsDrawn( );
    break;
}
case AGKI_GETVERTICESPROCESSED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetVerticesProcessed( );
    break;
}
case AGKI_ENABLECLEARDEPTH_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::EnableClearDepth( param0 );
    break;
}
case AGKI_ENABLECLEARCOLOR_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::EnableClearColor( param0 );
    break;
}
case AGKI_SETBORDERCOLOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetBorderColor( param0, param1, param2 );
    break;
}
case AGKI_SETCLEARCOLOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetClearColor( param0, param1, param2 );
    break;
}
case AGKI_LOADIMAGE_0_L_S_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadImage( param0, param1, param2 );
    break;
}
case AGKI_LOADIMAGE_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadImage( param0, param1 );
    break;
}
case AGKI_LOADIMAGE_L_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadImage( param0, param1 );
    break;
}
case AGKI_LOADIMAGE_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadImage( param0 );
    break;
}
case AGKI_LOADSUBIMAGE_0_L_L_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadSubImage( param0, param1, param2 );
    break;
}
case AGKI_LOADSUBIMAGE_L_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::LoadSubImage( param0, param1 );
    break;
}
case AGKI_LOADIMAGERESIZED_0_L_S_F_F_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadImageResized( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_LOADIMAGERESIZED_L_S_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadImageResized( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATEIMAGECOLOR_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateImageColor( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CREATEIMAGECOLOR_L_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateImageColor( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATERENDERIMAGE_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateRenderImage( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CREATERENDERIMAGE_L_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateRenderImage( param0, param1, param2, param3 );
    break;
}
case AGKI_GETIMAGEEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetImageExists( param0 );
    break;
}
case AGKI_DELETEIMAGE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteImage( param0 );
    break;
}
case AGKI_DELETEALLIMAGES_0_0:
{
    agk::DeleteAllImages( );
    break;
}
case AGKI_GETIMAGEWIDTH_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetImageWidth( param0 );
    break;
}
case AGKI_GETIMAGEHEIGHT_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetImageHeight( param0 );
    break;
}
case AGKI_SETIMAGEMINFILTER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetImageMinFilter( param0, param1 );
    break;
}
case AGKI_SETIMAGEMAGFILTER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetImageMagFilter( param0, param1 );
    break;
}
case AGKI_SETIMAGEWRAPU_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetImageWrapU( param0, param1 );
    break;
}
case AGKI_SETIMAGEWRAPV_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetImageWrapV( param0, param1 );
    break;
}
case AGKI_SETIMAGEMASK_0_L_L_L_L_L_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetImageMask( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETIMAGETRANSPARENTCOLOR_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetImageTransparentColor( param0, param1, param2, param3 );
    break;
}
case AGKI_SAVEIMAGE_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SaveImage( param0, param1 );
    break;
}
case AGKI_GETIMAGE_L_F_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::GetImage( param0, param1, param2, param3 );
    break;
}
case AGKI_GETIMAGE_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::GetImage( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_GETIMAGEFILENAME_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetImageFilename( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_COPYIMAGE_L_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CopyImage( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_COPYIMAGE_0_L_L_L_L_L_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CopyImage( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_RESIZEIMAGE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResizeImage( param0, param1, param2 );
    break;
}
case AGKI_SHOWCHOOSEIMAGESCREEN_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::ShowChooseImageScreen( );
    break;
}
case AGKI_ISCHOOSINGIMAGE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::IsChoosingImage( );
    break;
}
case AGKI_GETCHOSENIMAGE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetChosenImage( );
    break;
}
case AGKI_SHOWIMAGECAPTURESCREEN_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::ShowImageCaptureScreen( );
    break;
}
case AGKI_ISCAPTURINGIMAGE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::IsCapturingImage( );
    break;
}
case AGKI_GETCAPTUREDIMAGE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetCapturedImage( );
    break;
}
case AGKI_DECODEQRCODE_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::DecodeQRCode( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_ENCODEQRCODE_L_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::EncodeQRCode( param0, param1 );
    break;
}
case AGKI_PRINTIMAGE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PrintImage( param0, param1 );
    break;
}
case AGKI_SETIMAGESAVEPIXELS_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetImageSavePixels( param0 );
    break;
}
case AGKI_GETSPRITEPTR_P_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::GetSpritePtr( param0 );
    break;
}
case AGKI_GETSPRITEIMAGEID_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteImageID( param0 );
    break;
}
case AGKI_CREATESPRITE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateSprite( param0, param1 );
    break;
}
case AGKI_CREATESPRITE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateSprite( param0 );
    break;
}
case AGKI_LOADSPRITE_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadSprite( param0 );
    break;
}
case AGKI_LOADSPRITE_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadSprite( param0, param1 );
    break;
}
case AGKI_CLONESPRITE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CloneSprite( param0, param1 );
    break;
}
case AGKI_CLONESPRITE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CloneSprite( param0 );
    break;
}
case AGKI_CREATEDUMMYSPRITE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateDummySprite( param0 );
    break;
}
case AGKI_CREATEDUMMYSPRITE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::CreateDummySprite( );
    break;
}
case AGKI_GETSPRITEEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteExists( param0 );
    break;
}
case AGKI_DELETESPRITE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteSprite( param0 );
    break;
}
case AGKI_DELETEALLSPRITES_0_0:
{
    agk::DeleteAllSprites( );
    break;
}
case AGKI_SETSPRITEIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteImage( param0, param1 );
    break;
}
case AGKI_SETSPRITEIMAGE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteImage( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEADDITIONALIMAGE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteAdditionalImage( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEUV_0_L_F_F_F_F_F_F_F_F:
{
    float param8 = m_pStack[ --m_iStackPtr ].f;
    float param7 = m_pStack[ --m_iStackPtr ].f;
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteUV( param0, param1, param2, param3, param4, param5, param6, param7, param8 );
    break;
}
case AGKI_RESETSPRITEUV_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResetSpriteUV( param0 );
    break;
}
case AGKI_SETSPRITEPOSITION_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePosition( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEPOSITIONBYOFFSET_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePositionByOffset( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEX_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteX( param0, param1 );
    break;
}
case AGKI_SETSPRITEY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteY( param0, param1 );
    break;
}
case AGKI_SETSPRITEANGLE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteAngle( param0, param1 );
    break;
}
case AGKI_SETSPRITEANGLERAD_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteAngleRad( param0, param1 );
    break;
}
case AGKI_SETSPRITEDEPTH_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteDepth( param0, param1 );
    break;
}
case AGKI_SETSPRITEFLIP_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteFlip( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITESNAP_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteSnap( param0, param1 );
    break;
}
case AGKI_SETSPRITEVISIBLE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteVisible( param0, param1 );
    break;
}
case AGKI_SETSPRITEACTIVE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteActive( param0, param1 );
    break;
}
case AGKI_GETSPRITEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteX( param0 );
    break;
}
case AGKI_GETSPRITEY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteY( param0 );
    break;
}
case AGKI_GETSPRITEXBYOFFSET_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteXByOffset( param0 );
    break;
}
case AGKI_GETSPRITEYBYOFFSET_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteYByOffset( param0 );
    break;
}
case AGKI_GETSPRITEOFFSETX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteOffsetX( param0 );
    break;
}
case AGKI_GETSPRITEOFFSETY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteOffsetY( param0 );
    break;
}
case AGKI_GETSPRITEWIDTH_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteWidth( param0 );
    break;
}
case AGKI_GETSPRITEANGLE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteAngle( param0 );
    break;
}
case AGKI_GETSPRITEANGLERAD_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteAngleRad( param0 );
    break;
}
case AGKI_GETSPRITEHEIGHT_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteHeight( param0 );
    break;
}
case AGKI_GETSPRITESCALEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteScaleX( param0 );
    break;
}
case AGKI_GETSPRITESCALEY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteScaleY( param0 );
    break;
}
case AGKI_GETSPRITEHITTEST_L_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteHitTest( param0, param1, param2 );
    break;
}
case AGKI_GETSPRITEHIT_L_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteHit( param0, param1 );
    break;
}
case AGKI_GETSPRITEHITGROUP_L_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteHitGroup( param0, param1, param2 );
    break;
}
case AGKI_GETSPRITEHITCATEGORY_L_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteHitCategory( param0, param1, param2 );
    break;
}
case AGKI_GETSPRITEDEPTH_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteDepth( param0 );
    break;
}
case AGKI_GETSPRITECOLORRED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteColorRed( param0 );
    break;
}
case AGKI_GETSPRITECOLORGREEN_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteColorGreen( param0 );
    break;
}
case AGKI_GETSPRITECOLORBLUE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteColorBlue( param0 );
    break;
}
case AGKI_GETSPRITECOLORALPHA_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteColorAlpha( param0 );
    break;
}
case AGKI_GETSPRITEPLAYING_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpritePlaying( param0 );
    break;
}
case AGKI_GETSPRITEVISIBLE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteVisible( param0 );
    break;
}
case AGKI_GETSPRITEACTIVE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteActive( param0 );
    break;
}
case AGKI_GETSPRITEGROUP_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteGroup( param0 );
    break;
}
case AGKI_GETSPRITEXFROMPIXEL_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteXFromPixel( param0, param1 );
    break;
}
case AGKI_GETSPRITEYFROMPIXEL_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteYFromPixel( param0, param1 );
    break;
}
case AGKI_GETSPRITEPIXELFROMX_L_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpritePixelFromX( param0, param1 );
    break;
}
case AGKI_GETSPRITEPIXELFROMY_L_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpritePixelFromY( param0, param1 );
    break;
}
case AGKI_GETWORLDXFROMSPRITE_F_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetWorldXFromSprite( param0, param1, param2 );
    break;
}
case AGKI_GETWORLDYFROMSPRITE_F_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetWorldYFromSprite( param0, param1, param2 );
    break;
}
case AGKI_GETSPRITEXFROMWORLD_F_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteXFromWorld( param0, param1, param2 );
    break;
}
case AGKI_GETSPRITEYFROMWORLD_F_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteYFromWorld( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITESIZE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteSize( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEANIMATION_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteAnimation( param0, param1, param2, param3 );
    break;
}
case AGKI_ADDSPRITEANIMATIONFRAME_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddSpriteAnimationFrame( param0, param1 );
    break;
}
case AGKI_CLEARSPRITEANIMATIONFRAMES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ClearSpriteAnimationFrames( param0 );
    break;
}
case AGKI_SETSPRITESPEED_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteSpeed( param0, param1 );
    break;
}
case AGKI_PLAYSPRITE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlaySprite( param0 );
    break;
}
case AGKI_PLAYSPRITE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlaySprite( param0, param1 );
    break;
}
case AGKI_PLAYSPRITE_0_L_F_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlaySprite( param0, param1, param2 );
    break;
}
case AGKI_PLAYSPRITE_0_L_F_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlaySprite( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_STOPSPRITE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopSprite( param0 );
    break;
}
case AGKI_RESUMESPRITE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResumeSprite( param0 );
    break;
}
case AGKI_SETSPRITEFRAME_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteFrame( param0, param1 );
    break;
}
case AGKI_GETSPRITECURRENTFRAME_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteCurrentFrame( param0 );
    break;
}
case AGKI_GETSPRITEFRAMECOUNT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteFrameCount( param0 );
    break;
}
case AGKI_SETSPRITETRANSPARENCY_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteTransparency( param0, param1 );
    break;
}
case AGKI_SETSPRITEOFFSET_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteOffset( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITECOLOR_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteColor( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETSPRITECOLORRED_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteColorRed( param0, param1 );
    break;
}
case AGKI_SETSPRITECOLORGREEN_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteColorGreen( param0, param1 );
    break;
}
case AGKI_SETSPRITECOLORBLUE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteColorBlue( param0, param1 );
    break;
}
case AGKI_SETSPRITECOLORALPHA_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteColorAlpha( param0, param1 );
    break;
}
case AGKI_SETSPRITEUVBORDER_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteUVBorder( param0, param1 );
    break;
}
case AGKI_SETSPRITEUVOFFSET_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteUVOffset( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEUVSCALE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteUVScale( param0, param1, param2 );
    break;
}
case AGKI_FIXSPRITETOSCREEN_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::FixSpriteToScreen( param0, param1 );
    break;
}
case AGKI_SETSPRITESCALE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteScale( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITESCALEBYOFFSET_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteScaleByOffset( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITESCISSOR_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteScissor( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETSPRITESHADER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteShader( param0, param1 );
    break;
}
case AGKI_DRAWSPRITE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DrawSprite( param0 );
    break;
}
case AGKI_SETPHYSICSSCALE_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetPhysicsScale( param0 );
    break;
}
case AGKI_SETPHYSICSGRAVITY_0_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetPhysicsGravity( param0, param1 );
    break;
}
case AGKI_SETPHYSICSDEBUGON_0_0:
{
    agk::SetPhysicsDebugOn( );
    break;
}
case AGKI_SETPHYSICSDEBUGOFF_0_0:
{
    agk::SetPhysicsDebugOff( );
    break;
}
case AGKI_SETPHYSICSTHREADING_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsThreading( param0 );
    break;
}
case AGKI_SETPHYSICSCCD_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsCCD( param0 );
    break;
}
case AGKI_SETPHYSICSSLEEPING_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsSleeping( param0 );
    break;
}
case AGKI_GETPHYSICSSOLVETIME_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetPhysicsSolveTime( );
    break;
}
case AGKI_GETPHYSICSISLANDCOUNT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetPhysicsIslandCount( );
    break;
}
case AGKI_SETPHYSICSMAXPOLYGONPOINTS_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsMaxPolygonPoints( param0 );
    break;
}
case AGKI_SETPHYSICSWALLTOP_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsWallTop( param0 );
    break;
}
case AGKI_SETPHYSICSWALLLEFT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsWallLeft( param0 );
    break;
}
case AGKI_SETPHYSICSWALLRIGHT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsWallRight( param0 );
    break;
}
case AGKI_SETPHYSICSWALLBOTTOM_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsWallBottom( param0 );
    break;
}
case AGKI_CREATEPHYSICSFORCE_L_F_F_F_F_F_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreatePhysicsForce( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_DELETEPHYSICSFORCE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeletePhysicsForce( param0 );
    break;
}
case AGKI_SETPHYSICSFORCEPOSITION_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsForcePosition( param0, param1, param2 );
    break;
}
case AGKI_SETPHYSICSFORCEPOWER_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsForcePower( param0, param1 );
    break;
}
case AGKI_SETPHYSICSFORCERANGE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPhysicsForceRange( param0, param1 );
    break;
}
case AGKI_SETSPRITESHAPE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteShape( param0, param1 );
    break;
}
case AGKI_SETSPRITESHAPEBOX_0_L_F_F_F_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteShapeBox( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETSPRITESHAPECIRCLE_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteShapeCircle( param0, param1, param2, param3 );
    break;
}
case AGKI_SETSPRITESHAPEPOLYGON_0_L_L_L_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteShapePolygon( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_ADDSPRITESHAPEBOX_0_L_F_F_F_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddSpriteShapeBox( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_ADDSPRITESHAPECIRCLE_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddSpriteShapeCircle( param0, param1, param2, param3 );
    break;
}
case AGKI_ADDSPRITESHAPEPOLYGON_0_L_L_L_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddSpriteShapePolygon( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CLEARSPRITESHAPES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ClearSpriteShapes( param0 );
    break;
}
case AGKI_SETSPRITEPHYSICSCOM_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsCOM( param0, param1, param2 );
    break;
}
case AGKI_CALCULATESPRITEPHYSICSCOM_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CalculateSpritePhysicsCOM( param0 );
    break;
}
case AGKI_SETSPRITEPHYSICSON_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsOn( param0, param1 );
    break;
}
case AGKI_SETSPRITEPHYSICSOFF_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsOff( param0 );
    break;
}
case AGKI_SETSPRITEPHYSICSDELETE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsDelete( param0 );
    break;
}
case AGKI_SETSPRITEPHYSICSFRICTION_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsFriction( param0, param1 );
    break;
}
case AGKI_SETSPRITEPHYSICSRESTITUTION_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsRestitution( param0, param1 );
    break;
}
case AGKI_SETSPRITEPHYSICSCANROTATE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsCanRotate( param0, param1 );
    break;
}
case AGKI_SETSPRITEPHYSICSVELOCITY_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsVelocity( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEPHYSICSANGULARVELOCITY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsAngularVelocity( param0, param1 );
    break;
}
case AGKI_SETSPRITEPHYSICSDAMPING_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsDamping( param0, param1 );
    break;
}
case AGKI_SETSPRITEPHYSICSANGULARDAMPING_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsAngularDamping( param0, param1 );
    break;
}
case AGKI_SETSPRITEPHYSICSISBULLET_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsIsBullet( param0, param1 );
    break;
}
case AGKI_SETSPRITEPHYSICSMASS_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsMass( param0, param1 );
    break;
}
case AGKI_SETSPRITEPHYSICSISSENSOR_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsIsSensor( param0, param1 );
    break;
}
case AGKI_SETSPRITEGROUP_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteGroup( param0, param1 );
    break;
}
case AGKI_SETSPRITECATEGORYBITS_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteCategoryBits( param0, param1 );
    break;
}
case AGKI_SETSPRITECATEGORYBIT_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteCategoryBit( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITECOLLIDEBITS_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteCollideBits( param0, param1 );
    break;
}
case AGKI_SETSPRITECOLLIDEBIT_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteCollideBit( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEPHYSICSFORCE_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsForce( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETSPRITEPHYSICSTORQUE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsTorque( param0, param1 );
    break;
}
case AGKI_SETSPRITEPHYSICSIMPULSE_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsImpulse( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETSPRITEPHYSICSANGULARIMPULSE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsAngularImpulse( param0, param1 );
    break;
}
case AGKI_GETSPRITEPHYSICSVELOCITYX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpritePhysicsVelocityX( param0 );
    break;
}
case AGKI_GETSPRITEPHYSICSVELOCITYY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpritePhysicsVelocityY( param0 );
    break;
}
case AGKI_GETSPRITEPHYSICSANGULARVELOCITY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpritePhysicsAngularVelocity( param0 );
    break;
}
case AGKI_GETSPRITEPHYSICSMASS_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpritePhysicsMass( param0 );
    break;
}
case AGKI_DELETEJOINT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteJoint( param0 );
    break;
}
case AGKI_GETJOINTEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetJointExists( param0 );
    break;
}
case AGKI_CREATEDISTANCEJOINT_0_L_L_L_F_F_F_F_L:
{
    int param7 = m_pStack[ --m_iStackPtr ].i;
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateDistanceJoint( param0, param1, param2, param3, param4, param5, param6, param7 );
    break;
}
case AGKI_CREATEREVOLUTEJOINT_0_L_L_L_F_F_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateRevoluteJoint( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_CREATEPRISMATICJOINT_0_L_L_L_F_F_F_F_L:
{
    int param7 = m_pStack[ --m_iStackPtr ].i;
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreatePrismaticJoint( param0, param1, param2, param3, param4, param5, param6, param7 );
    break;
}
case AGKI_CREATEPULLEYJOINT_0_L_L_L_F_F_F_F_F_F_F_F_F_L:
{
    int param12 = m_pStack[ --m_iStackPtr ].i;
    float param11 = m_pStack[ --m_iStackPtr ].f;
    float param10 = m_pStack[ --m_iStackPtr ].f;
    float param9 = m_pStack[ --m_iStackPtr ].f;
    float param8 = m_pStack[ --m_iStackPtr ].f;
    float param7 = m_pStack[ --m_iStackPtr ].f;
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreatePulleyJoint( param0, param1, param2, param3, param4, param5, param6, param7, param8, param9, param10, param11, param12 );
    break;
}
case AGKI_CREATEMOUSEJOINT_0_L_L_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateMouseJoint( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CREATELINEJOINT_0_L_L_L_F_F_F_F_L:
{
    int param7 = m_pStack[ --m_iStackPtr ].i;
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateLineJoint( param0, param1, param2, param3, param4, param5, param6, param7 );
    break;
}
case AGKI_CREATEWELDJOINT_0_L_L_L_F_F_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateWeldJoint( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_CREATEGEARJOINT_0_L_L_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateGearJoint( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATEDISTANCEJOINT_L_L_L_F_F_F_F_L:
{
    int param6 = m_pStack[ --m_iStackPtr ].i;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateDistanceJoint( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_CREATEREVOLUTEJOINT_L_L_L_F_F_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateRevoluteJoint( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CREATEPRISMATICJOINT_L_L_L_F_F_F_F_L:
{
    int param6 = m_pStack[ --m_iStackPtr ].i;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreatePrismaticJoint( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_CREATEPULLEYJOINT2_0_L_L_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreatePulleyJoint2( param0, param1, param2, param3 );
    break;
}
case AGKI_FINISHPULLEYJOINT_L_F_F_F_F_F_F_F_F:
{
    float param7 = m_pStack[ --m_iStackPtr ].f;
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::FinishPulleyJoint( param0, param1, param2, param3, param4, param5, param6, param7 );
    break;
}
case AGKI_CREATEMOUSEJOINT_L_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateMouseJoint( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATELINEJOINT_L_L_L_F_F_F_F_L:
{
    int param6 = m_pStack[ --m_iStackPtr ].i;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateLineJoint( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_CREATEWELDJOINT_L_L_L_F_F_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateWeldJoint( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CREATEGEARJOINT_L_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateGearJoint( param0, param1, param2 );
    break;
}
case AGKI_SETJOINTLIMITON_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetJointLimitOn( param0, param1, param2 );
    break;
}
case AGKI_SETJOINTLIMITOFF_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetJointLimitOff( param0 );
    break;
}
case AGKI_SETJOINTMOTORON_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetJointMotorOn( param0, param1, param2 );
    break;
}
case AGKI_SETJOINTMOTOROFF_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetJointMotorOff( param0 );
    break;
}
case AGKI_SETJOINTMOUSETARGET_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetJointMouseTarget( param0, param1, param2 );
    break;
}
case AGKI_GETJOINTREACTIONFORCEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetJointReactionForceX( param0 );
    break;
}
case AGKI_GETJOINTREACTIONFORCEY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetJointReactionForceY( param0 );
    break;
}
case AGKI_GETJOINTREACTIONTORQUE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetJointReactionTorque( param0 );
    break;
}
case AGKI_PHYSICSRAYCAST_L_F_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::PhysicsRayCast( param0, param1, param2, param3 );
    break;
}
case AGKI_PHYSICSRAYCASTGROUP_L_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::PhysicsRayCastGroup( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_PHYSICSRAYCASTCATEGORY_L_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::PhysicsRayCastCategory( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SPRITERAYCASTSINGLE_L_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SpriteRayCastSingle( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SPRITERAYCAST_L_F_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::SpriteRayCast( param0, param1, param2, param3 );
    break;
}
case AGKI_SPRITERAYCASTGROUP_L_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SpriteRayCastGroup( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SPRITERAYCASTCATEGORY_L_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SpriteRayCastCategory( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_GETRAYCASTSPRITEID_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRayCastSpriteID( );
    break;
}
case AGKI_GETRAYCASTX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRayCastX( );
    break;
}
case AGKI_GETRAYCASTY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRayCastY( );
    break;
}
case AGKI_GETRAYCASTNORMALX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRayCastNormalX( );
    break;
}
case AGKI_GETRAYCASTNORMALY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRayCastNormalY( );
    break;
}
case AGKI_GETRAYCASTFRACTION_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRayCastFraction( );
    break;
}
case AGKI_GETSPRITEINBOX_L_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteInBox( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_GETSPRITEINCIRCLE_L_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteInCircle( param0, param1, param2, param3 );
    break;
}
case AGKI_GETSPRITECOLLISION_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteCollision( param0, param1 );
    break;
}
case AGKI_GETSPRITEDISTANCE_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteDistance( param0, param1 );
    break;
}
case AGKI_GETSPRITEDISTANCEPOINT1X_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteDistancePoint1X( );
    break;
}
case AGKI_GETSPRITEDISTANCEPOINT1Y_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteDistancePoint1Y( );
    break;
}
case AGKI_GETSPRITEDISTANCEPOINT2X_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteDistancePoint2X( );
    break;
}
case AGKI_GETSPRITEDISTANCEPOINT2Y_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteDistancePoint2Y( );
    break;
}
case AGKI_GETFIRSTCONTACT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetFirstContact( );
    break;
}
case AGKI_GETNEXTCONTACT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetNextContact( );
    break;
}
case AGKI_GETCONTACTWORLDX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetContactWorldX( );
    break;
}
case AGKI_GETCONTACTWORLDY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetContactWorldY( );
    break;
}
case AGKI_GETCONTACTSPRITEID1_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetContactSpriteID1( );
    break;
}
case AGKI_GETCONTACTSPRITEID2_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetContactSpriteID2( );
    break;
}
case AGKI_GETSPRITEFIRSTCONTACT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteFirstContact( param0 );
    break;
}
case AGKI_GETSPRITENEXTCONTACT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteNextContact( );
    break;
}
case AGKI_GETSPRITECONTACTWORLDX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteContactWorldX( );
    break;
}
case AGKI_GETSPRITECONTACTWORLDY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteContactWorldY( );
    break;
}
case AGKI_GETSPRITECONTACTSPRITEID2_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteContactSpriteID2( );
    break;
}
case AGKI_GETPHYSICSCOLLISION_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetPhysicsCollision( param0, param1 );
    break;
}
case AGKI_GETPHYSICSCOLLISIONX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetPhysicsCollisionX( );
    break;
}
case AGKI_GETPHYSICSCOLLISIONY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetPhysicsCollisionY( );
    break;
}
case AGKI_GETPHYSICSCOLLISIONWORLDX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetPhysicsCollisionWorldX( );
    break;
}
case AGKI_GETPHYSICSCOLLISIONWORLDY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetPhysicsCollisionWorldY( );
    break;
}
case AGKI_CREATEPARTICLES_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateParticles( param0, param1, param2 );
    break;
}
case AGKI_CREATEPARTICLES_L_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateParticles( param0, param1 );
    break;
}
case AGKI_GETPARTICLESEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetParticlesExists( param0 );
    break;
}
case AGKI_DELETEPARTICLES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteParticles( param0 );
    break;
}
case AGKI_SETPARTICLESPOSITION_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesPosition( param0, param1, param2 );
    break;
}
case AGKI_SETPARTICLESDEPTH_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesDepth( param0, param1 );
    break;
}
case AGKI_SETPARTICLESFREQUENCY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesFrequency( param0, param1 );
    break;
}
case AGKI_SETPARTICLESSTARTZONE_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesStartZone( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETPARTICLESDIRECTION_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesDirection( param0, param1, param2 );
    break;
}
case AGKI_SETPARTICLESVELOCITYRANGE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesVelocityRange( param0, param1, param2 );
    break;
}
case AGKI_SETPARTICLESANGLE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesAngle( param0, param1 );
    break;
}
case AGKI_SETPARTICLESANGLERAD_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesAngleRad( param0, param1 );
    break;
}
case AGKI_SETPARTICLESROTATIONRANGE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesRotationRange( param0, param1, param2 );
    break;
}
case AGKI_SETPARTICLESROTATIONRANGERAD_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesRotationRangeRad( param0, param1, param2 );
    break;
}
case AGKI_SETPARTICLESFACEDIRECTION_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesFaceDirection( param0, param1 );
    break;
}
case AGKI_SETPARTICLESSIZE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesSize( param0, param1 );
    break;
}
case AGKI_SETPARTICLESLIFE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesLife( param0, param1 );
    break;
}
case AGKI_SETPARTICLESMAX_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesMax( param0, param1 );
    break;
}
case AGKI_RESETPARTICLECOUNT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResetParticleCount( param0 );
    break;
}
case AGKI_SETPARTICLESIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesImage( param0, param1 );
    break;
}
case AGKI_SETPARTICLESVISIBLE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesVisible( param0, param1 );
    break;
}
case AGKI_SETPARTICLESACTIVE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesActive( param0, param1 );
    break;
}
case AGKI_SETPARTICLESTRANSPARENCY_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesTransparency( param0, param1 );
    break;
}
case AGKI_SETPARTICLESCOLORINTERPOLATION_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetParticlesColorInterpolation( param0, param1 );
    break;
}
case AGKI_GETPARTICLESX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetParticlesX( param0 );
    break;
}
case AGKI_GETPARTICLESY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetParticlesY( param0 );
    break;
}
case AGKI_GETPARTICLESDEPTH_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetParticlesDepth( param0 );
    break;
}
case AGKI_GETPARTICLESVISIBLE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetParticlesVisible( param0 );
    break;
}
case AGKI_GETPARTICLESACTIVE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetParticlesActive( param0 );
    break;
}
case AGKI_GETPARTICLESFREQUENCY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetParticlesFrequency( param0 );
    break;
}
case AGKI_GETPARTICLESDIRECTIONX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetParticlesDirectionX( param0 );
    break;
}
case AGKI_GETPARTICLESDIRECTIONY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetParticlesDirectionY( param0 );
    break;
}
case AGKI_GETPARTICLESANGLE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetParticlesAngle( param0 );
    break;
}
case AGKI_GETPARTICLESANGLERAD_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetParticlesAngleRad( param0 );
    break;
}
case AGKI_GETPARTICLESSIZE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetParticlesSize( param0 );
    break;
}
case AGKI_GETPARTICLESLIFE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetParticlesLife( param0 );
    break;
}
case AGKI_GETPARTICLESMAXREACHED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetParticlesMaxReached( param0 );
    break;
}
case AGKI_ADDPARTICLESFORCE_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddParticlesForce( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CLEARPARTICLESFORCES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ClearParticlesForces( param0 );
    break;
}
case AGKI_ADDPARTICLESCOLORKEYFRAME_0_L_F_L_L_L_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddParticlesColorKeyFrame( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_CLEARPARTICLESCOLORS_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ClearParticlesColors( param0 );
    break;
}
case AGKI_ADDPARTICLESSCALEKEYFRAME_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddParticlesScaleKeyFrame( param0, param1, param2 );
    break;
}
case AGKI_CLEARPARTICLESSCALES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ClearParticlesScales( param0 );
    break;
}
case AGKI_FIXPARTICLESTOSCREEN_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::FixParticlesToScreen( param0, param1 );
    break;
}
case AGKI_UPDATEPARTICLES_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::UpdateParticles( param0, param1 );
    break;
}
case AGKI_OFFSETPARTICLES_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::OffsetParticles( param0, param1, param2 );
    break;
}
case AGKI_CREATETEXT_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateText( param0, param1 );
    break;
}
case AGKI_CREATETEXT_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CreateText( param0 );
    break;
}
case AGKI_GETTEXTEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextExists( param0 );
    break;
}
case AGKI_DELETETEXT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteText( param0 );
    break;
}
case AGKI_DELETEALLTEXT_0_0:
{
    agk::DeleteAllText( );
    break;
}
case AGKI_SETTEXTSTRING_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextString( param0, param1 );
    break;
}
case AGKI_SETTEXTPOSITION_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextPosition( param0, param1, param2 );
    break;
}
case AGKI_SETTEXTX_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextX( param0, param1 );
    break;
}
case AGKI_SETTEXTY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextY( param0, param1 );
    break;
}
case AGKI_SETTEXTANGLE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextAngle( param0, param1 );
    break;
}
case AGKI_SETTEXTANGLERAD_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextAngleRad( param0, param1 );
    break;
}
case AGKI_SETTEXTALIGNMENT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextAlignment( param0, param1 );
    break;
}
case AGKI_SETTEXTSIZE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextSize( param0, param1 );
    break;
}
case AGKI_SETTEXTSPACING_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextSpacing( param0, param1 );
    break;
}
case AGKI_SETTEXTLINESPACING_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextLineSpacing( param0, param1 );
    break;
}
case AGKI_SETTEXTDEPTH_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextDepth( param0, param1 );
    break;
}
case AGKI_SETTEXTVISIBLE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextVisible( param0, param1 );
    break;
}
case AGKI_SETTEXTCOLOR_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextColor( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETTEXTCOLORRED_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextColorRed( param0, param1 );
    break;
}
case AGKI_SETTEXTCOLORGREEN_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextColorGreen( param0, param1 );
    break;
}
case AGKI_SETTEXTCOLORBLUE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextColorBlue( param0, param1 );
    break;
}
case AGKI_SETTEXTCOLORALPHA_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextColorAlpha( param0, param1 );
    break;
}
case AGKI_GETTEXTCOLORRED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextColorRed( param0 );
    break;
}
case AGKI_GETTEXTCOLORGREEN_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextColorGreen( param0 );
    break;
}
case AGKI_GETTEXTCOLORBLUE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextColorBlue( param0 );
    break;
}
case AGKI_GETTEXTCOLORALPHA_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextColorAlpha( param0 );
    break;
}
case AGKI_SETTEXTCHARPOSITION_0_L_L_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharPosition( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTEXTCHARX_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharX( param0, param1, param2 );
    break;
}
case AGKI_SETTEXTCHARY_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharY( param0, param1, param2 );
    break;
}
case AGKI_SETTEXTCHARANGLE_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharAngle( param0, param1, param2 );
    break;
}
case AGKI_SETTEXTCHARANGLERAD_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharAngleRad( param0, param1, param2 );
    break;
}
case AGKI_SETTEXTCHARCOLOR_0_L_L_L_L_L_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharColor( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETTEXTCHARCOLORRED_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharColorRed( param0, param1, param2 );
    break;
}
case AGKI_SETTEXTCHARCOLORGREEN_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharColorGreen( param0, param1, param2 );
    break;
}
case AGKI_SETTEXTCHARCOLORBLUE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharColorBlue( param0, param1, param2 );
    break;
}
case AGKI_SETTEXTCHARCOLORALPHA_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharColorAlpha( param0, param1, param2 );
    break;
}
case AGKI_SETTEXTCHARBOLD_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextCharBold( param0, param1, param2 );
    break;
}
case AGKI_GETTEXTCHARX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextCharX( param0, param1 );
    break;
}
case AGKI_GETTEXTCHARY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextCharY( param0, param1 );
    break;
}
case AGKI_GETTEXTCHARANGLE_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextCharAngle( param0, param1 );
    break;
}
case AGKI_GETTEXTCHARANGLERAD_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextCharAngleRad( param0, param1 );
    break;
}
case AGKI_GETTEXTCHARCOLORRED_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextCharColorRed( param0, param1 );
    break;
}
case AGKI_GETTEXTCHARCOLORGREEN_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextCharColorGreen( param0, param1 );
    break;
}
case AGKI_GETTEXTCHARCOLORBLUE_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextCharColorBlue( param0, param1 );
    break;
}
case AGKI_GETTEXTCHARCOLORALPHA_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextCharColorAlpha( param0, param1 );
    break;
}
case AGKI_GETTEXTX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextX( param0 );
    break;
}
case AGKI_GETTEXTY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextY( param0 );
    break;
}
case AGKI_GETTEXTLENGTH_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextLength( param0 );
    break;
}
case AGKI_GETTEXTTOTALWIDTH_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextTotalWidth( param0 );
    break;
}
case AGKI_GETTEXTTOTALHEIGHT_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextTotalHeight( param0 );
    break;
}
case AGKI_GETTEXTHITTEST_L_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextHitTest( param0, param1, param2 );
    break;
}
case AGKI_SETTEXTFONTIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextFontImage( param0, param1 );
    break;
}
case AGKI_SETTEXTEXTENDEDFONTIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextExtendedFontImage( param0, param1 );
    break;
}
case AGKI_SETTEXTFONT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextFont( param0, param1 );
    break;
}
case AGKI_SETTEXTDEFAULTFONTIMAGE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextDefaultFontImage( param0 );
    break;
}
case AGKI_SETTEXTDEFAULTEXTENDEDFONTIMAGE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextDefaultExtendedFontImage( param0 );
    break;
}
case AGKI_SETTEXTDEFAULTMINFILTER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextDefaultMinFilter( param0 );
    break;
}
case AGKI_SETTEXTDEFAULTMAGFILTER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextDefaultMagFilter( param0 );
    break;
}
case AGKI_FIXTEXTTOSCREEN_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::FixTextToScreen( param0, param1 );
    break;
}
case AGKI_SETTEXTMAXWIDTH_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextMaxWidth( param0, param1 );
    break;
}
case AGKI_SETTEXTSCISSOR_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextScissor( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETTEXTTRANSPARENCY_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextTransparency( param0, param1 );
    break;
}
case AGKI_SETTEXTBOLD_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextBold( param0, param1 );
    break;
}
case AGKI_GETTEXTVISIBLE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextVisible( param0 );
    break;
}
case AGKI_GETTEXTDEPTH_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextDepth( param0 );
    break;
}
case AGKI_GETTEXTSIZE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextSize( param0 );
    break;
}
case AGKI_GETTEXTSPACING_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextSpacing( param0 );
    break;
}
case AGKI_GETTEXTLINESPACING_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTextLineSpacing( param0 );
    break;
}
case AGKI_GETTEXTSTRING_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetTextString( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_LOADFONT_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadFont( param0 );
    break;
}
case AGKI_LOADFONT_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadFont( param0, param1 );
    break;
}
case AGKI_GETFONTEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetFontExists( param0 );
    break;
}
case AGKI_GETSYSTEMFONTEXISTS_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::GetSystemFontExists( param0 );
    break;
}
case AGKI_DELETEFONT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteFont( param0 );
    break;
}
case AGKI_PRINT_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::Print( param0 );
    break;
}
case AGKI_PRINT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Print( param0 );
    break;
}
case AGKI_PRINT_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::Print( param0 );
    break;
}
case AGKI_PRINTC_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::PrintC( param0 );
    break;
}
case AGKI_PRINTC_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PrintC( param0 );
    break;
}
case AGKI_PRINTC_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::PrintC( param0 );
    break;
}
case AGKI_SETPRINTSIZE_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetPrintSize( param0 );
    break;
}
case AGKI_SETPRINTSPACING_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetPrintSpacing( param0 );
    break;
}
case AGKI_SETPRINTCOLOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPrintColor( param0, param1, param2 );
    break;
}
case AGKI_SETPRINTCOLOR_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPrintColor( param0, param1, param2, param3 );
    break;
}
case AGKI_SETPRINTFONT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPrintFont( param0 );
    break;
}
case AGKI_CREATESKELETON2D_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateSkeleton2D( param0 );
    break;
}
case AGKI_CREATESKELETON2D_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::CreateSkeleton2D( );
    break;
}
case AGKI_GETSKELETON2DEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSkeleton2DExists( param0 );
    break;
}
case AGKI_DELETESKELETON2D_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteSkeleton2D( param0 );
    break;
}
case AGKI_LOADSKELETON2DFROMSPINEFILE_0_L_S_F_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadSkeleton2DFromSpineFile( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_LOADSKELETON2DFROMSPINEFILE_L_S_F_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadSkeleton2DFromSpineFile( param0, param1, param2, param3 );
    break;
}
case AGKI_LOADSKELETON2DFROMSPRITERFILE_0_L_S_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadSkeleton2DFromSpriterFile( param0, param1, param2, param3 );
    break;
}
case AGKI_LOADSKELETON2DFROMSPRITERFILE_L_S_F_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadSkeleton2DFromSpriterFile( param0, param1, param2 );
    break;
}
case AGKI_SETSKELETON2DPOSITION_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DPosition( param0, param1, param2 );
    break;
}
case AGKI_SETSKELETON2DROTATION_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DRotation( param0, param1 );
    break;
}
case AGKI_SETSKELETON2DFLIP_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DFlip( param0, param1, param2 );
    break;
}
case AGKI_SETSKELETON2DDEPTH_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DDepth( param0, param1 );
    break;
}
case AGKI_FIXSKELETON2DTOSCREEN_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::FixSkeleton2DToScreen( param0, param1 );
    break;
}
case AGKI_SETSKELETON2DVISIBLE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DVisible( param0, param1 );
    break;
}
case AGKI_GETSKELETON2DX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DX( param0 );
    break;
}
case AGKI_GETSKELETON2DY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DY( param0 );
    break;
}
case AGKI_GETSKELETON2DANGLE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DAngle( param0 );
    break;
}
case AGKI_GETSKELETON2DBONE_L_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSkeleton2DBone( param0, param1 );
    break;
}
case AGKI_GETSKELETON2DBONEPARENT_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSkeleton2DBoneParent( param0, param1 );
    break;
}
case AGKI_GETSKELETON2DBONEX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DBoneX( param0, param1 );
    break;
}
case AGKI_GETSKELETON2DBONEY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DBoneY( param0, param1 );
    break;
}
case AGKI_GETSKELETON2DBONEANGLE_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DBoneAngle( param0, param1 );
    break;
}
case AGKI_GETSKELETON2DBONECURRX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DBoneCurrX( param0, param1 );
    break;
}
case AGKI_GETSKELETON2DBONECURRY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DBoneCurrY( param0, param1 );
    break;
}
case AGKI_GETSKELETON2DBONECURRANGLE_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DBoneCurrAngle( param0, param1 );
    break;
}
case AGKI_SETSKELETON2DBONEPOSITION_0_L_L_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DBonePosition( param0, param1, param2, param3 );
    break;
}
case AGKI_SETSKELETON2DBONEANGLE_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DBoneAngle( param0, param1, param2 );
    break;
}
case AGKI_SETSKELETON2DBONESCALE_0_L_L_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DBoneScale( param0, param1, param2, param3 );
    break;
}
case AGKI_SETSKELETON2DBONEMODE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DBoneMode( param0, param1, param2 );
    break;
}
case AGKI_PLAYSKELETON2DANIMATION_0_L_S_F_L_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlaySkeleton2DAnimation( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETSKELETON2DANIMATIONFRAME_0_L_S_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DAnimationFrame( param0, param1, param2, param3 );
    break;
}
case AGKI_SETSKELETON2DANIMATIONSPEED_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkeleton2DAnimationSpeed( param0, param1 );
    break;
}
case AGKI_GETSKELETON2DCURRENTTIME_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DCurrentTime( param0 );
    break;
}
case AGKI_STOPSKELETON2DANIMATION_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopSkeleton2DAnimation( param0 );
    break;
}
case AGKI_GETSKELETON2DISANIMATING_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSkeleton2DIsAnimating( param0 );
    break;
}
case AGKI_GETSKELETON2DISTWEENING_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSkeleton2DIsTweening( param0 );
    break;
}
case AGKI_GETSKELETON2DANIMATIONTIME_F_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSkeleton2DAnimationTime( param0, param1 );
    break;
}
case AGKI_TWEENLINEAR_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::TweenLinear( );
    break;
}
case AGKI_TWEENSMOOTH1_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::TweenSmooth1( );
    break;
}
case AGKI_TWEENSMOOTH2_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::TweenSmooth2( );
    break;
}
case AGKI_TWEENEASEIN1_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::TweenEaseIn1( );
    break;
}
case AGKI_TWEENEASEIN2_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::TweenEaseIn2( );
    break;
}
case AGKI_TWEENEASEOUT1_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::TweenEaseOut1( );
    break;
}
case AGKI_TWEENEASEOUT2_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::TweenEaseOut2( );
    break;
}
case AGKI_TWEENBOUNCE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::TweenBounce( );
    break;
}
case AGKI_TWEENOVERSHOOT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::TweenOvershoot( );
    break;
}
case AGKI_DELETETWEEN_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteTween( param0 );
    break;
}
case AGKI_GETTWEENEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenExists( param0 );
    break;
}
case AGKI_SETTWEENDURATION_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenDuration( param0, param1 );
    break;
}
case AGKI_CREATETWEENCUSTOM_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateTweenCustom( param0, param1 );
    break;
}
case AGKI_CREATETWEENCUSTOM_L_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateTweenCustom( param0 );
    break;
}
case AGKI_GETTWEENCUSTOMEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenCustomExists( param0 );
    break;
}
case AGKI_SETTWEENCUSTOMFLOAT1_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCustomFloat1( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCUSTOMFLOAT2_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCustomFloat2( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCUSTOMFLOAT3_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCustomFloat3( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCUSTOMFLOAT4_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCustomFloat4( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCUSTOMINTEGER1_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCustomInteger1( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCUSTOMINTEGER2_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCustomInteger2( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCUSTOMINTEGER3_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCustomInteger3( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCUSTOMINTEGER4_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCustomInteger4( param0, param1, param2, param3 );
    break;
}
case AGKI_GETTWEENCUSTOMFLOAT1_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTweenCustomFloat1( param0 );
    break;
}
case AGKI_GETTWEENCUSTOMFLOAT2_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTweenCustomFloat2( param0 );
    break;
}
case AGKI_GETTWEENCUSTOMFLOAT3_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTweenCustomFloat3( param0 );
    break;
}
case AGKI_GETTWEENCUSTOMFLOAT4_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTweenCustomFloat4( param0 );
    break;
}
case AGKI_GETTWEENCUSTOMINTEGER1_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenCustomInteger1( param0 );
    break;
}
case AGKI_GETTWEENCUSTOMINTEGER2_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenCustomInteger2( param0 );
    break;
}
case AGKI_GETTWEENCUSTOMINTEGER3_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenCustomInteger3( param0 );
    break;
}
case AGKI_GETTWEENCUSTOMINTEGER4_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenCustomInteger4( param0 );
    break;
}
case AGKI_PLAYTWEENCUSTOM_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayTweenCustom( param0, param1 );
    break;
}
case AGKI_STOPTWEENCUSTOM_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopTweenCustom( param0 );
    break;
}
case AGKI_GETTWEENCUSTOMPLAYING_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenCustomPlaying( param0 );
    break;
}
case AGKI_CREATETWEENSPRITE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateTweenSprite( param0, param1 );
    break;
}
case AGKI_CREATETWEENSPRITE_L_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateTweenSprite( param0 );
    break;
}
case AGKI_GETTWEENSPRITEEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenSpriteExists( param0 );
    break;
}
case AGKI_SETTWEENSPRITEX_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteX( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENSPRITEY_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteY( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENSPRITEXBYOFFSET_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteXByOffset( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENSPRITEYBYOFFSET_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteYByOffset( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENSPRITEANGLE_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteAngle( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENSPRITESIZEX_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteSizeX( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENSPRITESIZEY_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteSizeY( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENSPRITERED_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteRed( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENSPRITEGREEN_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteGreen( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENSPRITEBLUE_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteBlue( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENSPRITEALPHA_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenSpriteAlpha( param0, param1, param2, param3 );
    break;
}
case AGKI_PLAYTWEENSPRITE_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayTweenSprite( param0, param1, param2 );
    break;
}
case AGKI_STOPTWEENSPRITE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopTweenSprite( param0, param1 );
    break;
}
case AGKI_GETTWEENSPRITEPLAYING_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenSpritePlaying( param0, param1 );
    break;
}
case AGKI_CREATETWEENTEXT_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateTweenText( param0, param1 );
    break;
}
case AGKI_CREATETWEENTEXT_L_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateTweenText( param0 );
    break;
}
case AGKI_GETTWEENTEXTEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenTextExists( param0 );
    break;
}
case AGKI_SETTWEENTEXTX_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenTextX( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENTEXTY_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenTextY( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENTEXTANGLE_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenTextAngle( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENTEXTSIZE_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenTextSize( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENTEXTSPACING_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenTextSpacing( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENTEXTLINESPACING_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenTextLineSpacing( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENTEXTRED_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenTextRed( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENTEXTGREEN_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenTextGreen( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENTEXTBLUE_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenTextBlue( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENTEXTALPHA_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenTextAlpha( param0, param1, param2, param3 );
    break;
}
case AGKI_PLAYTWEENTEXT_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayTweenText( param0, param1, param2 );
    break;
}
case AGKI_STOPTWEENTEXT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopTweenText( param0, param1 );
    break;
}
case AGKI_GETTWEENTEXTPLAYING_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenTextPlaying( param0, param1 );
    break;
}
case AGKI_CREATETWEENCHAR_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateTweenChar( param0, param1 );
    break;
}
case AGKI_CREATETWEENCHAR_L_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateTweenChar( param0 );
    break;
}
case AGKI_GETTWEENCHAREXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenCharExists( param0 );
    break;
}
case AGKI_SETTWEENCHARX_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCharX( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCHARY_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCharY( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCHARANGLE_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCharAngle( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCHARRED_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCharRed( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCHARGREEN_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCharGreen( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCHARBLUE_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCharBlue( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCHARALPHA_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCharAlpha( param0, param1, param2, param3 );
    break;
}
case AGKI_PLAYTWEENCHAR_0_L_L_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayTweenChar( param0, param1, param2, param3 );
    break;
}
case AGKI_STOPTWEENCHAR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopTweenChar( param0, param1, param2 );
    break;
}
case AGKI_GETTWEENCHARPLAYING_L_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenCharPlaying( param0, param1, param2 );
    break;
}
case AGKI_CREATETWEENOBJECT_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateTweenObject( param0, param1 );
    break;
}
case AGKI_CREATETWEENOBJECT_L_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateTweenObject( param0 );
    break;
}
case AGKI_GETTWEENOBJECTEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenObjectExists( param0 );
    break;
}
case AGKI_SETTWEENOBJECTX_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectX( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTY_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectY( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTZ_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectZ( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTANGLEX_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectAngleX( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTANGLEY_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectAngleY( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTANGLEZ_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectAngleZ( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTSCALEX_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectScaleX( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTSCALEY_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectScaleY( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTSCALEZ_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectScaleZ( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTRED_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectRed( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTGREEN_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectGreen( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTBLUE_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectBlue( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENOBJECTALPHA_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenObjectAlpha( param0, param1, param2, param3 );
    break;
}
case AGKI_PLAYTWEENOBJECT_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayTweenObject( param0, param1, param2 );
    break;
}
case AGKI_STOPTWEENOBJECT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopTweenObject( param0, param1 );
    break;
}
case AGKI_GETTWEENOBJECTPLAYING_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenObjectPlaying( param0, param1 );
    break;
}
case AGKI_CREATETWEENCAMERA_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateTweenCamera( param0, param1 );
    break;
}
case AGKI_CREATETWEENCAMERA_L_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateTweenCamera( param0 );
    break;
}
case AGKI_GETTWEENCAMERAEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenCameraExists( param0 );
    break;
}
case AGKI_SETTWEENCAMERAX_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCameraX( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCAMERAY_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCameraY( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCAMERAZ_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCameraZ( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCAMERAANGLEX_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCameraAngleX( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCAMERAANGLEY_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCameraAngleY( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCAMERAANGLEZ_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCameraAngleZ( param0, param1, param2, param3 );
    break;
}
case AGKI_SETTWEENCAMERAFOV_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenCameraFOV( param0, param1, param2, param3 );
    break;
}
case AGKI_PLAYTWEENCAMERA_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayTweenCamera( param0, param1, param2 );
    break;
}
case AGKI_STOPTWEENCAMERA_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopTweenCamera( param0, param1 );
    break;
}
case AGKI_GETTWEENCAMERAPLAYING_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenCameraPlaying( param0, param1 );
    break;
}
case AGKI_CREATETWEENCHAIN_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateTweenChain( param0 );
    break;
}
case AGKI_CREATETWEENCHAIN_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::CreateTweenChain( );
    break;
}
case AGKI_DELETETWEENCHAIN_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteTweenChain( param0 );
    break;
}
case AGKI_CLEARTWEENCHAIN_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ClearTweenChain( param0 );
    break;
}
case AGKI_ADDTWEENCHAINCUSTOM_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddTweenChainCustom( param0, param1, param2 );
    break;
}
case AGKI_ADDTWEENCHAINSPRITE_0_L_L_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddTweenChainSprite( param0, param1, param2, param3 );
    break;
}
case AGKI_ADDTWEENCHAINTEXT_0_L_L_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddTweenChainText( param0, param1, param2, param3 );
    break;
}
case AGKI_ADDTWEENCHAINCHAR_0_L_L_L_L_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddTweenChainChar( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_ADDTWEENCHAINOBJECT_0_L_L_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddTweenChainObject( param0, param1, param2, param3 );
    break;
}
case AGKI_ADDTWEENCHAINCAMERA_0_L_L_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddTweenChainCamera( param0, param1, param2, param3 );
    break;
}
case AGKI_PLAYTWEENCHAIN_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayTweenChain( param0 );
    break;
}
case AGKI_PAUSETWEENCHAIN_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PauseTweenChain( param0 );
    break;
}
case AGKI_STOPTWEENCHAIN_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopTweenChain( param0 );
    break;
}
case AGKI_GETTWEENCHAINPLAYING_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTweenChainPlaying( param0 );
    break;
}
case AGKI_SETTWEENCHAINTIME_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTweenChainTime( param0, param1 );
    break;
}
case AGKI_GETTWEENCHAINENDTIME_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetTweenChainEndTime( param0 );
    break;
}
case AGKI_UPDATEALLTWEENS_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::UpdateAllTweens( param0 );
    break;
}
case AGKI_UPDATETWEENCUSTOM_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::UpdateTweenCustom( param0, param1 );
    break;
}
case AGKI_UPDATETWEENSPRITE_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::UpdateTweenSprite( param0, param1, param2 );
    break;
}
case AGKI_UPDATETWEENTEXT_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::UpdateTweenText( param0, param1, param2 );
    break;
}
case AGKI_UPDATETWEENCHAR_0_L_L_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::UpdateTweenChar( param0, param1, param2, param3 );
    break;
}
case AGKI_UPDATETWEENOBJECT_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::UpdateTweenObject( param0, param1, param2 );
    break;
}
case AGKI_UPDATETWEENCAMERA_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::UpdateTweenCamera( param0, param1, param2 );
    break;
}
case AGKI_UPDATETWEENCHAIN_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::UpdateTweenChain( param0, param1 );
    break;
}
case AGKI_GETRAWTOUCHCOUNT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawTouchCount( param0 );
    break;
}
case AGKI_GETRAWFIRSTTOUCHEVENT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawFirstTouchEvent( param0 );
    break;
}
case AGKI_GETRAWNEXTTOUCHEVENT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawNextTouchEvent( );
    break;
}
case AGKI_GETRAWTOUCHTYPE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawTouchType( param0 );
    break;
}
case AGKI_GETRAWTOUCHSTARTX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawTouchStartX( param0 );
    break;
}
case AGKI_GETRAWTOUCHSTARTY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawTouchStartY( param0 );
    break;
}
case AGKI_GETRAWTOUCHCURRENTX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawTouchCurrentX( param0 );
    break;
}
case AGKI_GETRAWTOUCHCURRENTY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawTouchCurrentY( param0 );
    break;
}
case AGKI_GETRAWTOUCHLASTX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawTouchLastX( param0 );
    break;
}
case AGKI_GETRAWTOUCHLASTY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawTouchLastY( param0 );
    break;
}
case AGKI_GETRAWTOUCHRELEASED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawTouchReleased( param0 );
    break;
}
case AGKI_SETRAWTOUCHVALUE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetRawTouchValue( param0, param1 );
    break;
}
case AGKI_GETRAWTOUCHVALUE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawTouchValue( param0 );
    break;
}
case AGKI_GETRAWTOUCHTIME_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawTouchTime( param0 );
    break;
}
case AGKI_LOADSOUND_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadSound( param0, param1 );
    break;
}
case AGKI_LOADSOUND_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadSound( param0 );
    break;
}
case AGKI_LOADSOUNDOGG_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadSoundOGG( param0, param1 );
    break;
}
case AGKI_LOADSOUNDOGG_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadSoundOGG( param0 );
    break;
}
case AGKI_SAVESOUND_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SaveSound( param0, param1 );
    break;
}
case AGKI_PLAYSOUND_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::PlaySound( param0 );
    break;
}
case AGKI_PLAYSOUND_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::PlaySound( param0, param1 );
    break;
}
case AGKI_PLAYSOUND_L_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::PlaySound( param0, param1, param2 );
    break;
}
case AGKI_PLAYSOUND_L_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::PlaySound( param0, param1, param2, param3 );
    break;
}
case AGKI_STOPSOUND_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopSound( param0 );
    break;
}
case AGKI_DELETESOUND_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteSound( param0 );
    break;
}
case AGKI_GETSOUNDEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSoundExists( param0 );
    break;
}
case AGKI_GETSOUNDINSTANCES_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSoundInstances( param0 );
    break;
}
case AGKI_GETSOUNDSPLAYING_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSoundsPlaying( param0 );
    break;
}
case AGKI_SETSOUNDSYSTEMVOLUME_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSoundSystemVolume( param0 );
    break;
}
case AGKI_GETSOUNDMAXRATE_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetSoundMaxRate( );
    break;
}
case AGKI_GETSOUNDMINRATE_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetSoundMinRate( );
    break;
}
case AGKI_GETSOUNDINSTANCEPLAYING_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSoundInstancePlaying( param0 );
    break;
}
case AGKI_GETSOUNDINSTANCEVOLUME_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSoundInstanceVolume( param0 );
    break;
}
case AGKI_GETSOUNDINSTANCERATE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSoundInstanceRate( param0 );
    break;
}
case AGKI_SETSOUNDINSTANCEVOLUME_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSoundInstanceVolume( param0, param1 );
    break;
}
case AGKI_SETSOUNDINSTANCERATE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSoundInstanceRate( param0, param1 );
    break;
}
case AGKI_SETSOUNDINSTANCEBALANCE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSoundInstanceBalance( param0, param1 );
    break;
}
case AGKI_GETSOUNDINSTANCELOOPCOUNT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSoundInstanceLoopCount( param0 );
    break;
}
case AGKI_STOPSOUNDINSTANCE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopSoundInstance( param0 );
    break;
}
case AGKI_LOADMUSIC_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadMusic( param0 );
    break;
}
case AGKI_LOADMUSIC_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadMusic( param0, param1 );
    break;
}
case AGKI_SETMUSICFILEVOLUME_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMusicFileVolume( param0, param1 );
    break;
}
case AGKI_PLAYMUSIC_0_0:
{
    agk::PlayMusic( );
    break;
}
case AGKI_PLAYMUSIC_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayMusic( param0 );
    break;
}
case AGKI_PLAYMUSIC_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayMusic( param0, param1 );
    break;
}
case AGKI_PLAYMUSIC_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayMusic( param0, param1, param2, param3 );
    break;
}
case AGKI_PAUSEMUSIC_0_0:
{
    agk::PauseMusic( );
    break;
}
case AGKI_RESUMEMUSIC_0_0:
{
    agk::ResumeMusic( );
    break;
}
case AGKI_STOPMUSIC_0_0:
{
    agk::StopMusic( );
    break;
}
case AGKI_DELETEMUSIC_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteMusic( param0 );
    break;
}
case AGKI_GETMUSICPLAYING_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetMusicPlaying( );
    break;
}
case AGKI_SETMUSICSYSTEMVOLUME_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMusicSystemVolume( param0 );
    break;
}
case AGKI_GETMUSICEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMusicExists( param0 );
    break;
}
case AGKI_GETMUSICDURATION_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMusicDuration( param0 );
    break;
}
case AGKI_GETMUSICPOSITION_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetMusicPosition( );
    break;
}
case AGKI_SEEKMUSIC_0_F_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SeekMusic( param0, param1 );
    break;
}
case AGKI_LOADMUSICOGG_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadMusicOGG( param0 );
    break;
}
case AGKI_LOADMUSICOGG_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadMusicOGG( param0, param1 );
    break;
}
case AGKI_GETMUSICEXISTSOGG_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMusicExistsOGG( param0 );
    break;
}
case AGKI_SETMUSICVOLUMEOGG_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMusicVolumeOGG( param0, param1 );
    break;
}
case AGKI_PLAYMUSICOGG_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayMusicOGG( param0 );
    break;
}
case AGKI_PLAYMUSICOGG_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayMusicOGG( param0, param1 );
    break;
}
case AGKI_PAUSEMUSICOGG_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PauseMusicOGG( param0 );
    break;
}
case AGKI_RESUMEMUSICOGG_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResumeMusicOGG( param0 );
    break;
}
case AGKI_STOPMUSICOGG_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopMusicOGG( param0 );
    break;
}
case AGKI_DELETEMUSICOGG_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteMusicOGG( param0 );
    break;
}
case AGKI_GETMUSICPLAYINGOGG_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMusicPlayingOGG( param0 );
    break;
}
case AGKI_GETMUSICLOOPCOUNTOGG_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMusicLoopCountOGG( param0 );
    break;
}
case AGKI_GETMUSICDURATIONOGG_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMusicDurationOGG( param0 );
    break;
}
case AGKI_GETMUSICPOSITIONOGG_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMusicPositionOGG( param0 );
    break;
}
case AGKI_SEEKMUSICOGG_0_L_F_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SeekMusicOGG( param0, param1, param2 );
    break;
}
case AGKI_SETMUSICSYSTEMVOLUMEOGG_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMusicSystemVolumeOGG( param0 );
    break;
}
case AGKI_SETMUSICLOOPTIMESOGG_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMusicLoopTimesOGG( param0, param1, param2 );
    break;
}
case AGKI_SETMUSICLOOPCOUNTOGG_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMusicLoopCountOGG( param0, param1 );
    break;
}
case AGKI_RECORDSOUND_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::RecordSound( param0 );
    break;
}
case AGKI_STOPSOUNDRECORDING_0_0:
{
    agk::StopSoundRecording( );
    break;
}
case AGKI_ISSOUNDRECORDING_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::IsSoundRecording( );
    break;
}
case AGKI_DELETEFILE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::DeleteFile( param0 );
    break;
}
case AGKI_GETFILEEXISTS_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::GetFileExists( param0 );
    break;
}
case AGKI_CHOOSERAWFILE_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::ChooseRawFile( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_CHOOSERAWFILE_S_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::ChooseRawFile( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SIMPLIFYPATH_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::SimplifyPath( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_JOINPATHS_S_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::JoinPaths( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_ISABSOLUTEPATH_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::IsAbsolutePath( param0 );
    break;
}
case AGKI_COUNTWINDOWSDRIVES_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::CountWindowsDrives( );
    break;
}
case AGKI_GETWINDOWSDRIVE_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetWindowsDrive( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_OPENRAWFOLDER_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::OpenRawFolder( param0 );
    break;
}
case AGKI_CLOSERAWFOLDER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CloseRawFolder( param0 );
    break;
}
case AGKI_GETRAWFOLDERNUMFILES_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawFolderNumFiles( param0 );
    break;
}
case AGKI_GETRAWFOLDERNUMFOLDERS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawFolderNumFolders( param0 );
    break;
}
case AGKI_GETRAWFOLDERFILENAME_S_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetRawFolderFileName( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETRAWFOLDERFOLDERNAME_S_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetRawFolderFolderName( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_OPENTOWRITE_0_L_S_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::OpenToWrite( param0, param1, param2 );
    break;
}
case AGKI_OPENTOWRITE_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::OpenToWrite( param0, param1 );
    break;
}
case AGKI_OPENTOWRITE_L_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::OpenToWrite( param0, param1 );
    break;
}
case AGKI_OPENTOWRITE_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::OpenToWrite( param0 );
    break;
}
case AGKI_OPENTOREAD_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::OpenToRead( param0 );
    break;
}
case AGKI_OPENTOREAD_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::OpenToRead( param0, param1 );
    break;
}
case AGKI_FILEISOPEN_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::FileIsOpen( param0 );
    break;
}
case AGKI_CLOSEFILE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CloseFile( param0 );
    break;
}
case AGKI_FILEEOF_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::FileEOF( param0 );
    break;
}
case AGKI_GETFILESIZE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetFileSize( param0 );
    break;
}
case AGKI_GETFILEPOS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetFilePos( param0 );
    break;
}
case AGKI_SETFILEPOS_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetFilePos( param0, param1 );
    break;
}
case AGKI_WRITEBYTE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::WriteByte( param0, param1 );
    break;
}
case AGKI_WRITEINTEGER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::WriteInteger( param0, param1 );
    break;
}
case AGKI_WRITEFLOAT_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::WriteFloat( param0, param1 );
    break;
}
case AGKI_WRITESTRING_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::WriteString( param0, param1 );
    break;
}
case AGKI_WRITESTRING2_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::WriteString2( param0, param1 );
    break;
}
case AGKI_WRITELINE_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::WriteLine( param0, param1 );
    break;
}
case AGKI_READBYTE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ReadByte( param0 );
    break;
}
case AGKI_READINTEGER_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ReadInteger( param0 );
    break;
}
case AGKI_READFLOAT_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ReadFloat( param0 );
    break;
}
case AGKI_READSTRING_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::ReadString( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_READSTRING2_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::ReadString2( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_READLINE_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::ReadLine( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_STR_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::Str( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_STR_S_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    char *szReturnStr = agk::Str( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_STR_S_F_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    char *szReturnStr = agk::Str( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_BIN_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::Bin( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_HEX_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::Hex( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_STRINGTOBASE64_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::StringToBase64( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_HEXTOBASE64_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::HexToBase64( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_VAL_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::Val( param0 );
    break;
}
case AGKI_VAL_L_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::Val( param0, param1 );
    break;
}
case AGKI_VALFLOAT_F_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].f = agk::ValFloat( param0 );
    break;
}
case AGKI_LEFT_S_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::Left( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_RIGHT_S_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::Right( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_MID_S_S_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::Mid( param0, param1, param2 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_ASC_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::Asc( param0 );
    break;
}
case AGKI_LEN_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::Len( param0 );
    break;
}
case AGKI_BYTELEN_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::ByteLen( param0 );
    break;
}
case AGKI_CHR_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::Chr( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_LOWER_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::Lower( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_UPPER_S_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::Upper( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SPACES_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::Spaces( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_FINDSTRINGCOUNT_L_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::FindStringCount( param0, param1 );
    break;
}
case AGKI_FINDSTRINGCOUNT_L_S_S_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::FindStringCount( param0, param1, param2, param3 );
    break;
}
case AGKI_FINDSTRING_L_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::FindString( param0, param1 );
    break;
}
case AGKI_FINDSTRING_L_S_S_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::FindString( param0, param1, param2, param3 );
    break;
}
case AGKI_FINDSTRINGREVERSE_L_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::FindStringReverse( param0, param1 );
    break;
}
case AGKI_FINDSTRINGREVERSE_L_S_S_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::FindStringReverse( param0, param1, param2, param3 );
    break;
}
case AGKI_COMPARESTRING_L_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CompareString( param0, param1 );
    break;
}
case AGKI_COMPARESTRING_L_S_S_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CompareString( param0, param1, param2, param3 );
    break;
}
case AGKI_REPLACESTRING_S_S_S_S_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::ReplaceString( param0, param1, param2, param3 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_STRIPSTRING_S_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::StripString( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_TRIMSTRING_S_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::TrimString( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_TRUNCATESTRING_S_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::TruncateString( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_COUNTSTRINGTOKENS_L_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CountStringTokens( param0, param1 );
    break;
}
case AGKI_GETSTRINGTOKEN_S_S_S_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::GetStringToken( param0, param1, param2 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_COUNTSTRINGTOKENS2_L_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CountStringTokens2( param0, param1 );
    break;
}
case AGKI_GETSTRINGTOKEN2_S_S_S_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::GetStringToken2( param0, param1, param2 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETCURRENTDIR_S_0:
{
    char *szReturnStr = agk::GetCurrentDir( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETFOLDER_S_0:
{
    char *szReturnStr = agk::GetFolder( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETFOLDER_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::SetFolder( param0 );
    break;
}
case AGKI_GETFIRSTFOLDER_S_0:
{
    char *szReturnStr = agk::GetFirstFolder( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETFIRSTFOLDER_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetFirstFolder( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETNEXTFOLDER_S_0:
{
    char *szReturnStr = agk::GetNextFolder( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETFOLDERCOUNT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetFolderCount( );
    break;
}
case AGKI_GETFOLDERCOUNT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetFolderCount( param0 );
    break;
}
case AGKI_GETFIRSTFILE_S_0:
{
    char *szReturnStr = agk::GetFirstFile( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETFIRSTFILE_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetFirstFile( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETNEXTFILE_S_0:
{
    char *szReturnStr = agk::GetNextFile( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETFILECOUNT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetFileCount( );
    break;
}
case AGKI_GETFILECOUNT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetFileCount( param0 );
    break;
}
case AGKI_CREATEBROADCASTLISTENER_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateBroadcastListener( param0 );
    break;
}
case AGKI_CREATEBROADCASTLISTENER_L_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CreateBroadcastListener( param0, param1 );
    break;
}
case AGKI_GETBROADCASTMESSAGE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetBroadcastMessage( param0 );
    break;
}
case AGKI_DELETEBROADCASTLISTENER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteBroadcastListener( param0 );
    break;
}
case AGKI_CONNECTSOCKET_L_S_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::ConnectSocket( param0, param1, param2 );
    break;
}
case AGKI_CONNECTSOCKET_L_L_S_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ConnectSocket( param0, param1, param2, param3 );
    break;
}
case AGKI_GETSOCKETCONNECTED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSocketConnected( param0 );
    break;
}
case AGKI_GETSOCKETEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSocketExists( param0 );
    break;
}
case AGKI_DELETESOCKET_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteSocket( param0 );
    break;
}
case AGKI_GETSOCKETREMOTEIP_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetSocketRemoteIP( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_FLUSHSOCKET_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::FlushSocket( param0 );
    break;
}
case AGKI_GETSOCKETBYTESAVAILABLE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSocketBytesAvailable( param0 );
    break;
}
case AGKI_GETSOCKETBYTE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSocketByte( param0 );
    break;
}
case AGKI_GETSOCKETINTEGER_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSocketInteger( param0 );
    break;
}
case AGKI_GETSOCKETFLOAT_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSocketFloat( param0 );
    break;
}
case AGKI_GETSOCKETSTRING_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetSocketString( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SENDSOCKETBYTE_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SendSocketByte( param0, param1 );
    break;
}
case AGKI_SENDSOCKETINTEGER_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SendSocketInteger( param0, param1 );
    break;
}
case AGKI_SENDSOCKETFLOAT_L_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SendSocketFloat( param0, param1 );
    break;
}
case AGKI_SENDSOCKETSTRING_L_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SendSocketString( param0, param1 );
    break;
}
case AGKI_CREATESOCKETLISTENER_L_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CreateSocketListener( param0, param1 );
    break;
}
case AGKI_CREATESOCKETLISTENER_L_L_S_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateSocketListener( param0, param1, param2 );
    break;
}
case AGKI_GETSOCKETLISTENERCONNECTION_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSocketListenerConnection( param0 );
    break;
}
case AGKI_DELETESOCKETLISTENER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteSocketListener( param0 );
    break;
}
case AGKI_HOSTNETWORK_L_S_S_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::HostNetwork( param0, param1, param2 );
    break;
}
case AGKI_HOSTNETWORK_L_S_S_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::HostNetwork( param0, param1, param2, param3 );
    break;
}
case AGKI_SETNETWORKNOMORECLIENTS_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetNetworkNoMoreClients( param0 );
    break;
}
case AGKI_JOINNETWORK_L_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::JoinNetwork( param0, param1 );
    break;
}
case AGKI_JOINNETWORK_L_S_L_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::JoinNetwork( param0, param1, param2 );
    break;
}
case AGKI_ISNETWORKACTIVE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::IsNetworkActive( param0 );
    break;
}
case AGKI_GETDEVICEIP_S_0:
{
    char *szReturnStr = agk::GetDeviceIP( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETDEVICEIPV6_S_0:
{
    char *szReturnStr = agk::GetDeviceIPv6( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_CLOSENETWORK_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CloseNetwork( param0 );
    break;
}
case AGKI_SETNETWORKLATENCY_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetNetworkLatency( param0, param1 );
    break;
}
case AGKI_GETNETWORKMYCLIENTID_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkMyClientID( param0 );
    break;
}
case AGKI_GETNETWORKNUMCLIENTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkNumClients( param0 );
    break;
}
case AGKI_GETNETWORKFIRSTCLIENT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkFirstClient( param0 );
    break;
}
case AGKI_GETNETWORKNEXTCLIENT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkNextClient( param0 );
    break;
}
case AGKI_GETNETWORKCLIENTDISCONNECTED_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkClientDisconnected( param0, param1 );
    break;
}
case AGKI_DELETENETWORKCLIENT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteNetworkClient( param0, param1 );
    break;
}
case AGKI_GETNETWORKCLIENTNAME_S_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetNetworkClientName( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETNETWORKCLIENTPING_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetNetworkClientPing( param0, param1 );
    break;
}
case AGKI_GETNETWORKSERVERID_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkServerID( param0 );
    break;
}
case AGKI_SETNETWORKLOCALINTEGER_0_L_S_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetNetworkLocalInteger( param0, param1, param2 );
    break;
}
case AGKI_SETNETWORKLOCALINTEGER_0_L_S_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetNetworkLocalInteger( param0, param1, param2, param3 );
    break;
}
case AGKI_SETNETWORKLOCALFLOAT_0_L_S_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetNetworkLocalFloat( param0, param1, param2 );
    break;
}
case AGKI_SETNETWORKLOCALFLOAT_0_L_S_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetNetworkLocalFloat( param0, param1, param2, param3 );
    break;
}
case AGKI_GETNETWORKCLIENTINTEGER_L_L_L_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkClientInteger( param0, param1, param2 );
    break;
}
case AGKI_GETNETWORKCLIENTFLOAT_F_L_L_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetNetworkClientFloat( param0, param1, param2 );
    break;
}
case AGKI_CREATENETWORKMESSAGE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::CreateNetworkMessage( );
    break;
}
case AGKI_ADDNETWORKMESSAGEINTEGER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddNetworkMessageInteger( param0, param1 );
    break;
}
case AGKI_ADDNETWORKMESSAGEFLOAT_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddNetworkMessageFloat( param0, param1 );
    break;
}
case AGKI_ADDNETWORKMESSAGESTRING_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddNetworkMessageString( param0, param1 );
    break;
}
case AGKI_GETNETWORKMESSAGEFROMIP_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetNetworkMessageFromIP( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETNETWORKMESSAGEFROMCLIENT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkMessageFromClient( param0 );
    break;
}
case AGKI_GETNETWORKMESSAGEINTEGER_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkMessageInteger( param0 );
    break;
}
case AGKI_GETNETWORKMESSAGEFLOAT_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetNetworkMessageFloat( param0 );
    break;
}
case AGKI_GETNETWORKMESSAGESTRING_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetNetworkMessageString( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_DELETENETWORKMESSAGE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteNetworkMessage( param0 );
    break;
}
case AGKI_SENDNETWORKMESSAGE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SendNetworkMessage( param0, param1, param2 );
    break;
}
case AGKI_GETNETWORKMESSAGE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkMessage( param0 );
    break;
}
case AGKI_SETNETWORKCLIENTUSERDATA_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetNetworkClientUserData( param0, param1, param2, param3 );
    break;
}
case AGKI_GETNETWORKCLIENTUSERDATA_L_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkClientUserData( param0, param1, param2 );
    break;
}
case AGKI_CREATEHTTPCONNECTION_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::CreateHTTPConnection( );
    break;
}
case AGKI_DELETEHTTPCONNECTION_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteHTTPConnection( param0 );
    break;
}
case AGKI_SETHTTPHOST_L_L_S_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SetHTTPHost( param0, param1, param2 );
    break;
}
case AGKI_SETHTTPHOST_L_L_S_L_S_S:
{
    const char* param4 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param3 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SetHTTPHost( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CLOSEHTTPCONNECTION_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CloseHTTPConnection( param0 );
    break;
}
case AGKI_SETHTTPTIMEOUT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetHTTPTimeout( param0, param1 );
    break;
}
case AGKI_SETHTTPVERIFYCERTIFICATE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetHTTPVerifyCertificate( param0, param1 );
    break;
}
case AGKI_SENDHTTPREQUEST_S_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::SendHTTPRequest( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SENDHTTPREQUEST_S_L_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::SendHTTPRequest( param0, param1, param2 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SENDHTTPREQUESTASYNC_L_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SendHTTPRequestASync( param0, param1 );
    break;
}
case AGKI_SENDHTTPREQUESTASYNC_L_L_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SendHTTPRequestASync( param0, param1, param2 );
    break;
}
case AGKI_SENDHTTPFILE_L_L_S_S_S:
{
    const char* param3 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SendHTTPFile( param0, param1, param2, param3 );
    break;
}
case AGKI_GETHTTPRESPONSE_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetHTTPResponse( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETHTTPRESPONSEREADY_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetHTTPResponseReady( param0 );
    break;
}
case AGKI_GETHTTPFILE_L_L_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetHTTPFile( param0, param1, param2 );
    break;
}
case AGKI_GETHTTPFILE_L_L_S_S_S:
{
    const char* param3 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetHTTPFile( param0, param1, param2, param3 );
    break;
}
case AGKI_GETHTTPFILECOMPLETE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetHTTPFileComplete( param0 );
    break;
}
case AGKI_GETHTTPFILEPROGRESS_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetHTTPFileProgress( param0 );
    break;
}
case AGKI_SETERRORMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetErrorMode( param0 );
    break;
}
case AGKI_GETLASTERROR_S_0:
{
    char *szReturnStr = agk::GetLastError( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETERROROCCURRED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetErrorOccurred( );
    break;
}
case AGKI_MESSAGE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::Message( param0 );
    break;
}
case AGKI_GETJOYSTICKEXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetJoystickExists( );
    break;
}
case AGKI_GETRAWMOUSEX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawMouseX( );
    break;
}
case AGKI_GETRAWMOUSEY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawMouseY( );
    break;
}
case AGKI_GETRAWMOUSEWHEEL_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawMouseWheel( );
    break;
}
case AGKI_GETRAWMOUSEWHEELDELTA_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawMouseWheelDelta( );
    break;
}
case AGKI_GETRAWMOUSELEFTPRESSED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseLeftPressed( );
    break;
}
case AGKI_GETRAWMOUSELEFTSTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseLeftState( );
    break;
}
case AGKI_GETRAWMOUSELEFTRELEASED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseLeftReleased( );
    break;
}
case AGKI_GETRAWMOUSERIGHTPRESSED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseRightPressed( );
    break;
}
case AGKI_GETRAWMOUSERIGHTSTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseRightState( );
    break;
}
case AGKI_GETRAWMOUSERIGHTRELEASED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseRightReleased( );
    break;
}
case AGKI_GETRAWMOUSEMIDDLEPRESSED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseMiddlePressed( );
    break;
}
case AGKI_GETRAWMOUSEMIDDLESTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseMiddleState( );
    break;
}
case AGKI_GETRAWMOUSEMIDDLERELEASED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseMiddleReleased( );
    break;
}
case AGKI_GETACCELEROMETEREXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetAccelerometerExists( );
    break;
}
case AGKI_GETGYROSENSOREXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetGyroSensorExists( );
    break;
}
case AGKI_GETPROXIMITYSENSOREXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetProximitySensorExists( );
    break;
}
case AGKI_GETLIGHTSENSOREXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetLightSensorExists( );
    break;
}
case AGKI_GETMAGNETICSENSOREXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetMagneticSensorExists( );
    break;
}
case AGKI_GETROTATIONVECTORSENSOREXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRotationVectorSensorExists( );
    break;
}
case AGKI_GETRAWACCELX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawAccelX( );
    break;
}
case AGKI_GETRAWACCELY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawAccelY( );
    break;
}
case AGKI_GETRAWACCELZ_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawAccelZ( );
    break;
}
case AGKI_GETRAWGYROVELOCITYX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawGyroVelocityX( );
    break;
}
case AGKI_GETRAWGYROVELOCITYY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawGyroVelocityY( );
    break;
}
case AGKI_GETRAWGYROVELOCITYZ_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawGyroVelocityZ( );
    break;
}
case AGKI_GETRAWPROXIMITYDISTANCE_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawProximityDistance( );
    break;
}
case AGKI_GETRAWLIGHTLEVEL_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawLightLevel( );
    break;
}
case AGKI_GETRAWMAGNETICX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawMagneticX( );
    break;
}
case AGKI_GETRAWMAGNETICY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawMagneticY( );
    break;
}
case AGKI_GETRAWMAGNETICZ_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawMagneticZ( );
    break;
}
case AGKI_GETRAWROTATIONVECTORX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawRotationVectorX( );
    break;
}
case AGKI_GETRAWROTATIONVECTORY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawRotationVectorY( );
    break;
}
case AGKI_GETRAWROTATIONVECTORZ_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawRotationVectorZ( );
    break;
}
case AGKI_GETRAWROTATIONVECTORW_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawRotationVectorW( );
    break;
}
case AGKI_GETRAWROTATIONVECTORX2_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawRotationVectorX2( );
    break;
}
case AGKI_GETRAWROTATIONVECTORY2_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawRotationVectorY2( );
    break;
}
case AGKI_GETRAWROTATIONVECTORZ2_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawRotationVectorZ2( );
    break;
}
case AGKI_GETRAWROTATIONVECTORW2_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawRotationVectorW2( );
    break;
}
case AGKI_COMPLETERAWJOYSTICKDETECTION_0_0:
{
    agk::CompleteRawJoystickDetection( );
    break;
}
case AGKI_GETRAWJOYSTICKEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawJoystickExists( param0 );
    break;
}
case AGKI_GETRAWJOYSTICKCONNECTED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawJoystickConnected( param0 );
    break;
}
case AGKI_GETRAWJOYSTICKX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawJoystickX( param0 );
    break;
}
case AGKI_GETRAWJOYSTICKY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawJoystickY( param0 );
    break;
}
case AGKI_GETRAWJOYSTICKZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawJoystickZ( param0 );
    break;
}
case AGKI_GETRAWJOYSTICKRX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawJoystickRX( param0 );
    break;
}
case AGKI_GETRAWJOYSTICKRY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawJoystickRY( param0 );
    break;
}
case AGKI_GETRAWJOYSTICKRZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawJoystickRZ( param0 );
    break;
}
case AGKI_GETRAWJOYSTICKBUTTONPRESSED_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawJoystickButtonPressed( param0, param1 );
    break;
}
case AGKI_GETRAWJOYSTICKBUTTONSTATE_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawJoystickButtonState( param0, param1 );
    break;
}
case AGKI_GETRAWJOYSTICKBUTTONRELEASED_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawJoystickButtonReleased( param0, param1 );
    break;
}
case AGKI_SETRAWJOYSTICKDEADZONE_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetRawJoystickDeadZone( param0 );
    break;
}
case AGKI_ADDVIRTUALJOYSTICK_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddVirtualJoystick( param0, param1, param2, param3 );
    break;
}
case AGKI_DELETEVIRTUALJOYSTICK_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteVirtualJoystick( param0 );
    break;
}
case AGKI_GETVIRTUALJOYSTICKEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetVirtualJoystickExists( param0 );
    break;
}
case AGKI_GETVIRTUALJOYSTICKX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetVirtualJoystickX( param0 );
    break;
}
case AGKI_GETVIRTUALJOYSTICKY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetVirtualJoystickY( param0 );
    break;
}
case AGKI_SETVIRTUALJOYSTICKALPHA_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualJoystickAlpha( param0, param1, param2 );
    break;
}
case AGKI_SETVIRTUALJOYSTICKPOSITION_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualJoystickPosition( param0, param1, param2 );
    break;
}
case AGKI_SETVIRTUALJOYSTICKSIZE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualJoystickSize( param0, param1 );
    break;
}
case AGKI_SETVIRTUALJOYSTICKACTIVE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualJoystickActive( param0, param1 );
    break;
}
case AGKI_SETVIRTUALJOYSTICKVISIBLE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualJoystickVisible( param0, param1 );
    break;
}
case AGKI_SETVIRTUALJOYSTICKIMAGEINNER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualJoystickImageInner( param0, param1 );
    break;
}
case AGKI_SETVIRTUALJOYSTICKIMAGEOUTER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualJoystickImageOuter( param0, param1 );
    break;
}
case AGKI_SETVIRTUALJOYSTICKDEADZONE_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetVirtualJoystickDeadZone( param0 );
    break;
}
case AGKI_ADDVIRTUALBUTTON_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddVirtualButton( param0, param1, param2, param3 );
    break;
}
case AGKI_DELETEVIRTUALBUTTON_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteVirtualButton( param0 );
    break;
}
case AGKI_GETVIRTUALBUTTONEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetVirtualButtonExists( param0 );
    break;
}
case AGKI_GETVIRTUALBUTTONPRESSED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetVirtualButtonPressed( param0 );
    break;
}
case AGKI_GETVIRTUALBUTTONRELEASED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetVirtualButtonReleased( param0 );
    break;
}
case AGKI_GETVIRTUALBUTTONSTATE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetVirtualButtonState( param0 );
    break;
}
case AGKI_SETVIRTUALBUTTONCOLOR_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualButtonColor( param0, param1, param2, param3 );
    break;
}
case AGKI_SETVIRTUALBUTTONALPHA_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualButtonAlpha( param0, param1 );
    break;
}
case AGKI_SETVIRTUALBUTTONPOSITION_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualButtonPosition( param0, param1, param2 );
    break;
}
case AGKI_SETVIRTUALBUTTONSIZE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualButtonSize( param0, param1 );
    break;
}
case AGKI_SETVIRTUALBUTTONSIZE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualButtonSize( param0, param1, param2 );
    break;
}
case AGKI_SETVIRTUALBUTTONACTIVE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualButtonActive( param0, param1 );
    break;
}
case AGKI_SETVIRTUALBUTTONVISIBLE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualButtonVisible( param0, param1 );
    break;
}
case AGKI_SETVIRTUALBUTTONIMAGEUP_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualButtonImageUp( param0, param1 );
    break;
}
case AGKI_SETVIRTUALBUTTONIMAGEDOWN_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualButtonImageDown( param0, param1 );
    break;
}
case AGKI_SETVIRTUALBUTTONTEXT_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVirtualButtonText( param0, param1 );
    break;
}
case AGKI_GETRAWKEYPRESSED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawKeyPressed( param0 );
    break;
}
case AGKI_GETRAWKEYSTATE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawKeyState( param0 );
    break;
}
case AGKI_GETRAWKEYRELEASED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawKeyReleased( param0 );
    break;
}
case AGKI_GETRAWLASTKEY_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawLastKey( );
    break;
}
case AGKI_GETDIRECTIONX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetDirectionX( );
    break;
}
case AGKI_GETDIRECTIONY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetDirectionY( );
    break;
}
case AGKI_GETDIRECTIONANGLE_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetDirectionAngle( );
    break;
}
case AGKI_GETDIRECTIONSPEED_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetDirectionSpeed( );
    break;
}
case AGKI_GETPOINTERPRESSED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetPointerPressed( );
    break;
}
case AGKI_GETPOINTERRELEASED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetPointerReleased( );
    break;
}
case AGKI_GETPOINTERSTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetPointerState( );
    break;
}
case AGKI_GETPOINTERX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetPointerX( );
    break;
}
case AGKI_GETPOINTERY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetPointerY( );
    break;
}
case AGKI_GETJOYSTICKX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetJoystickX( );
    break;
}
case AGKI_GETJOYSTICKY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetJoystickY( );
    break;
}
case AGKI_SETJOYSTICKDEADZONE_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetJoystickDeadZone( param0 );
    break;
}
case AGKI_SETJOYSTICKSCREENPOSITION_0_F_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetJoystickScreenPosition( param0, param1, param2 );
    break;
}
case AGKI_GETBUTTONPRESSED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetButtonPressed( param0 );
    break;
}
case AGKI_GETBUTTONSTATE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetButtonState( param0 );
    break;
}
case AGKI_GETBUTTONRELEASED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetButtonReleased( param0 );
    break;
}
case AGKI_SETBUTTONSCREENPOSITION_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetButtonScreenPosition( param0, param1, param2, param3 );
    break;
}
case AGKI_STARTTEXTINPUT_0_0:
{
    agk::StartTextInput( );
    break;
}
case AGKI_STARTTEXTINPUT_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::StartTextInput( param0 );
    break;
}
case AGKI_STOPTEXTINPUT_0_0:
{
    agk::StopTextInput( );
    break;
}
case AGKI_GETTEXTINPUTSTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextInputState( );
    break;
}
case AGKI_GETTEXTINPUTCOMPLETED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextInputCompleted( );
    break;
}
case AGKI_GETTEXTINPUTCANCELLED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextInputCancelled( );
    break;
}
case AGKI_GETTEXTINPUT_S_0:
{
    char *szReturnStr = agk::GetTextInput( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETLASTCHAR_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetLastChar( );
    break;
}
case AGKI_SETCURSORBLINKTIME_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetCursorBlinkTime( param0 );
    break;
}
case AGKI_SETTEXTINPUTMAXCHARS_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetTextInputMaxChars( param0 );
    break;
}
case AGKI_CREATEEDITBOX_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateEditBox( param0 );
    break;
}
case AGKI_CREATEEDITBOX_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::CreateEditBox( );
    break;
}
case AGKI_GETEDITBOXEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetEditBoxExists( param0 );
    break;
}
case AGKI_DELETEEDITBOX_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteEditBox( param0 );
    break;
}
case AGKI_GETEDITBOXHASFOCUS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetEditBoxHasFocus( param0 );
    break;
}
case AGKI_GETCURRENTEDITBOX_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetCurrentEditBox( );
    break;
}
case AGKI_SETEDITBOXPOSITION_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxPosition( param0, param1, param2 );
    break;
}
case AGKI_SETEDITBOXSIZE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxSize( param0, param1, param2 );
    break;
}
case AGKI_SETEDITBOXDEPTH_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxDepth( param0, param1 );
    break;
}
case AGKI_SETEDITBOXBORDERSIZE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxBorderSize( param0, param1 );
    break;
}
case AGKI_SETEDITBOXBORDERCOLOR_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxBorderColor( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETEDITBOXBACKGROUNDCOLOR_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxBackgroundColor( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETEDITBOXTEXT_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxText( param0, param1 );
    break;
}
case AGKI_SETEDITBOXTEXTCOLOR_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxTextColor( param0, param1, param2, param3 );
    break;
}
case AGKI_SETEDITBOXCURSORCOLOR_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxCursorColor( param0, param1, param2, param3 );
    break;
}
case AGKI_SETEDITBOXFONTIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxFontImage( param0, param1 );
    break;
}
case AGKI_SETEDITBOXEXTENDEDFONTIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxExtendedFontImage( param0, param1 );
    break;
}
case AGKI_SETEDITBOXFONT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxFont( param0, param1 );
    break;
}
case AGKI_SETEDITBOXTEXTSIZE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxTextSize( param0, param1 );
    break;
}
case AGKI_SETEDITBOXCURSORPOSITION_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxCursorPosition( param0, param1 );
    break;
}
case AGKI_SETEDITBOXFOCUS_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxFocus( param0, param1 );
    break;
}
case AGKI_SETEDITBOXACTIVE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxActive( param0, param1 );
    break;
}
case AGKI_SETEDITBOXVISIBLE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxVisible( param0, param1 );
    break;
}
case AGKI_SETEDITBOXBORDERIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxBorderImage( param0, param1 );
    break;
}
case AGKI_SETEDITBOXBACKGROUNDIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxBackgroundImage( param0, param1 );
    break;
}
case AGKI_SETEDITBOXCURSORBLINKTIME_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxCursorBlinkTime( param0, param1 );
    break;
}
case AGKI_SETEDITBOXCURSORWIDTH_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxCursorWidth( param0, param1 );
    break;
}
case AGKI_SETEDITBOXMAXCHARS_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxMaxChars( param0, param1 );
    break;
}
case AGKI_SETEDITBOXMAXLINES_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxMaxLines( param0, param1 );
    break;
}
case AGKI_SETEDITBOXMULTILINE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxMultiLine( param0, param1 );
    break;
}
case AGKI_SETEDITBOXSCISSOR_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxScissor( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETEDITBOXPASSWORDMODE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxPasswordMode( param0, param1 );
    break;
}
case AGKI_SETEDITBOXUSEALTERNATEINPUT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxUseAlternateInput( param0, param1 );
    break;
}
case AGKI_SETEDITBOXWRAPMODE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxWrapMode( param0, param1 );
    break;
}
case AGKI_FIXEDITBOXTOSCREEN_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::FixEditBoxToScreen( param0, param1 );
    break;
}
case AGKI_GETEDITBOXTEXT_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetEditBoxText( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETEDITBOXX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetEditBoxX( param0 );
    break;
}
case AGKI_GETEDITBOXY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetEditBoxY( param0 );
    break;
}
case AGKI_GETEDITBOXWIDTH_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetEditBoxWidth( param0 );
    break;
}
case AGKI_GETEDITBOXHEIGHT_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetEditBoxHeight( param0 );
    break;
}
case AGKI_GETEDITBOXCHANGED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetEditBoxChanged( param0 );
    break;
}
case AGKI_GETEDITBOXACTIVE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetEditBoxActive( param0 );
    break;
}
case AGKI_GETEDITBOXVISIBLE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetEditBoxVisible( param0 );
    break;
}
case AGKI_GETEDITBOXLINES_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetEditBoxLines( param0 );
    break;
}
case AGKI_GETEDITBOXCURSORPOSITION_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetEditBoxCursorPosition( param0 );
    break;
}
case AGKI_GETMANAGEDSPRITEDRAWNCOUNT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetManagedSpriteDrawnCount( );
    break;
}
case AGKI_GETMANAGEDSPRITECOUNT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetManagedSpriteCount( );
    break;
}
case AGKI_GETMANAGEDSPRITESORTEDCOUNT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetManagedSpriteSortedCount( );
    break;
}
case AGKI_GETPIXELSDRAWN_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetPixelsDrawn( );
    break;
}
case AGKI_GETMANAGEDSPRITEDRAWCALLS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetManagedSpriteDrawCalls( );
    break;
}
case AGKI_GETPARTICLEDRAWNPOINTCOUNT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetParticleDrawnPointCount( );
    break;
}
case AGKI_GETPARTICLEDRAWNQUADCOUNT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetParticleDrawnQuadCount( );
    break;
}
case AGKI_GETUPDATETIME_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetUpdateTime( );
    break;
}
case AGKI_GETPHYSICSTIME_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetPhysicsTime( );
    break;
}
case AGKI_GETDRAWINGSETUPTIME_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetDrawingSetupTime( );
    break;
}
case AGKI_GETDRAWINGTIME_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetDrawingTime( );
    break;
}
case AGKI_GETLOADEDIMAGES_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetLoadedImages( );
    break;
}
case AGKI_GETUNASSIGNEDIMAGES_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetUnassignedImages( );
    break;
}
case AGKI_GETUNASSIGNEDIMAGEFILENAME_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetUnassignedImageFileName( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETIMAGEMEMORYUSAGE_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetImageMemoryUsage( );
    break;
}
case AGKI_GETLEAPYEAR_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetLeapYear( param0 );
    break;
}
case AGKI_GETUNIXFROMDATE_L_L_L_L_L_L_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetUnixFromDate( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_GETYEARFROMUNIX_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetYearFromUnix( param0 );
    break;
}
case AGKI_GETMONTHFROMUNIX_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMonthFromUnix( param0 );
    break;
}
case AGKI_GETDAYSFROMUNIX_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetDaysFromUnix( param0 );
    break;
}
case AGKI_GETHOURSFROMUNIX_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetHoursFromUnix( param0 );
    break;
}
case AGKI_GETMINUTESFROMUNIX_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMinutesFromUnix( param0 );
    break;
}
case AGKI_GETSECONDSFROMUNIX_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSecondsFromUnix( param0 );
    break;
}
case AGKI_SETINNERACTIVEDETAILS_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetInneractiveDetails( param0 );
    break;
}
case AGKI_SETADMOBDETAILS_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetAdMobDetails( param0 );
    break;
}
case AGKI_SETADMOBREWARDADDETAILS_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetAdMobRewardAdDetails( param0 );
    break;
}
case AGKI_SETCHARTBOOSTDETAILS_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetChartboostDetails( param0, param1 );
    break;
}
case AGKI_SETAMAZONADDETAILS_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetAmazonAdDetails( param0 );
    break;
}
case AGKI_SETAMAZONADTESTING_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetAmazonAdTesting( param0 );
    break;
}
case AGKI_SETADMOBTESTING_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetAdMobTesting( param0 );
    break;
}
case AGKI_SHOWFULLSCREENADVERTADMOB_0_0:
{
    agk::ShowFullscreenAdvertAdMob( );
    break;
}
case AGKI_SHOWFULLSCREENADVERTCHARTBOOST_0_0:
{
    agk::ShowFullscreenAdvertChartboost( );
    break;
}
case AGKI_SHOWFULLSCREENADVERTAMAZON_0_0:
{
    agk::ShowFullscreenAdvertAmazon( );
    break;
}
case AGKI_GETFULLSCREENADVERTLOADEDADMOB_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetFullscreenAdvertLoadedAdMob( );
    break;
}
case AGKI_GETFULLSCREENADVERTLOADEDCHARTBOOST_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetFullscreenAdvertLoadedChartboost( );
    break;
}
case AGKI_GETFULLSCREENADVERTLOADEDAMAZON_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetFullscreenAdvertLoadedAmazon( );
    break;
}
case AGKI_SHOWREWARDADADMOB_0_0:
{
    agk::ShowRewardAdAdMob( );
    break;
}
case AGKI_GETREWARDADLOADEDADMOB_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRewardAdLoadedAdMob( );
    break;
}
case AGKI_GETREWARDADREWARDEDADMOB_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRewardAdRewardedAdMob( );
    break;
}
case AGKI_RESETREWARDADMOB_0_0:
{
    agk::ResetRewardAdMob( );
    break;
}
case AGKI_SHOWREWARDADCHARTBOOST_0_0:
{
    agk::ShowRewardAdChartboost( );
    break;
}
case AGKI_CACHEREWARDADCHARTBOOST_0_0:
{
    agk::CacheRewardAdChartboost( );
    break;
}
case AGKI_GETREWARDADLOADEDCHARTBOOST_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRewardAdLoadedChartboost( );
    break;
}
case AGKI_GETREWARDADREWARDEDCHARTBOOST_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRewardAdRewardedChartboost( );
    break;
}
case AGKI_RESETREWARDCHARTBOOST_0_0:
{
    agk::ResetRewardChartboost( );
    break;
}
case AGKI_CREATEFULLSCREENADVERT_0_0:
{
    agk::CreateFullscreenAdvert( );
    break;
}
case AGKI_CREATEADVERT_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateAdvert( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATEADVERTEX_0_L_L_L_L_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateAdvertEx( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETADVERTPOSITION_0_F_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetAdvertPosition( param0, param1, param2 );
    break;
}
case AGKI_SETADVERTLOCATION_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetAdvertLocation( param0, param1, param2 );
    break;
}
case AGKI_SETADVERTLOCATIONEX_0_L_L_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetAdvertLocationEx( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETADVERTVISIBLE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetAdvertVisible( param0 );
    break;
}
case AGKI_REQUESTADVERTREFRESH_0_0:
{
    agk::RequestAdvertRefresh( );
    break;
}
case AGKI_DELETEADVERT_0_0:
{
    agk::DeleteAdvert( );
    break;
}
case AGKI_CREATEZIP_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateZip( param0, param1 );
    break;
}
case AGKI_CREATEZIP_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CreateZip( param0 );
    break;
}
case AGKI_ADDZIPENTRY_0_L_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddZipEntry( param0, param1, param2 );
    break;
}
case AGKI_CLOSEZIP_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CloseZip( param0 );
    break;
}
case AGKI_EXTRACTZIP_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::ExtractZip( param0, param1 );
    break;
}
case AGKI_LOG_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::Log( param0 );
    break;
}
case AGKI_RATEAPP_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::RateApp( param0 );
    break;
}
case AGKI_RATEAPP_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::RateApp( param0, param1 );
    break;
}
case AGKI_RATEAPP_0_S_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::RateApp( param0, param1, param2 );
    break;
}
case AGKI_REQUESTAPPREVIEW_0_0:
{
    agk::RequestAppReview( );
    break;
}
case AGKI_INAPPPURCHASESETKEYS_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::InAppPurchaseSetKeys( param0, param1 );
    break;
}
case AGKI_INAPPPURCHASESETTITLE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::InAppPurchaseSetTitle( param0 );
    break;
}
case AGKI_INAPPPURCHASEADDPRODUCTID_0_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::InAppPurchaseAddProductID( param0, param1 );
    break;
}
case AGKI_INAPPPURCHASESETUP_0_0:
{
    agk::InAppPurchaseSetup( );
    break;
}
case AGKI_GETINAPPPURCHASEAVAILABLE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetInAppPurchaseAvailable( param0 );
    break;
}
case AGKI_INAPPPURCHASEACTIVATE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::InAppPurchaseActivate( param0 );
    break;
}
case AGKI_GETINAPPPURCHASELOCALPRICE_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetInAppPurchaseLocalPrice( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETINAPPPURCHASEDESCRIPTION_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetInAppPurchaseDescription( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETINAPPPURCHASESTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetInAppPurchaseState( );
    break;
}
case AGKI_INAPPPURCHASERESTORE_0_0:
{
    agk::InAppPurchaseRestore( );
    break;
}
case AGKI_GETINAPPPURCHASESIGNATURE_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetInAppPurchaseSignature( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_TWITTERSETUP_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::TwitterSetup( param0, param1 );
    break;
}
case AGKI_TWITTERMESSAGE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::TwitterMessage( param0 );
    break;
}
case AGKI_FACEBOOKSETUP_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::FacebookSetup( param0 );
    break;
}
case AGKI_GETFACEBOOKLOGGEDIN_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetFacebookLoggedIn( );
    break;
}
case AGKI_FACEBOOKGETUSERID_S_0:
{
    char *szReturnStr = agk::FacebookGetUserID( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_FACEBOOKGETUSERNAME_S_0:
{
    char *szReturnStr = agk::FacebookGetUserName( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_FACEBOOKGETACCESSTOKEN_S_0:
{
    char *szReturnStr = agk::FacebookGetAccessToken( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_FACEBOOKLOGIN_0_0:
{
    agk::FacebookLogin( );
    break;
}
case AGKI_FACEBOOKLOGOUT_0_0:
{
    agk::FacebookLogout( );
    break;
}
case AGKI_FACEBOOKPOSTONMYWALL_0_S_S_S_S_S:
{
    const char* param4 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param3 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::FacebookPostOnMyWall( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_FACEBOOKPOSTONFRIENDSWALL_0_S_S_S_S_S_S:
{
    const char* param5 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param4 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param3 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::FacebookPostOnFriendsWall( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_FACEBOOKINVITEFRIEND_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::FacebookInviteFriend( param0, param1 );
    break;
}
case AGKI_FACEBOOKSHOWLIKEBUTTON_0_S_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::FacebookShowLikeButton( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_FACEBOOKDESTROYLIKEBUTTON_0_0:
{
    agk::FacebookDestroyLikeButton( );
    break;
}
case AGKI_FACEBOOKGETFRIENDS_0_0:
{
    agk::FacebookGetFriends( );
    break;
}
case AGKI_FACEBOOKGETFRIENDSSTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::FacebookGetFriendsState( );
    break;
}
case AGKI_FACEBOOKGETFRIENDSCOUNT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::FacebookGetFriendsCount( );
    break;
}
case AGKI_FACEBOOKGETFRIENDSNAME_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::FacebookGetFriendsName( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_FACEBOOKGETFRIENDSID_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::FacebookGetFriendsID( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_FACEBOOKDOWNLOADFRIENDSPHOTO_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::FacebookDownloadFriendsPhoto( param0 );
    break;
}
case AGKI_GETFACEBOOKDOWNLOADSTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetFacebookDownloadState( );
    break;
}
case AGKI_GETFACEBOOKDOWNLOADFILE_S_0:
{
    char *szReturnStr = agk::GetFacebookDownloadFile( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_NOTIFICATIONCREATE_0_S_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::NotificationCreate( param0, param1, param2 );
    break;
}
case AGKI_GETNOTIFICATION_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetNotification( );
    break;
}
case AGKI_GETNOTIFICATIONDATA_S_0:
{
    char *szReturnStr = agk::GetNotificationData( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_NOTIFICATIONRESET_0_0:
{
    agk::NotificationReset( );
    break;
}
case AGKI_SETLOCALNOTIFICATION_0_L_L_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetLocalNotification( param0, param1, param2 );
    break;
}
case AGKI_CANCELLOCALNOTIFICATION_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CancelLocalNotification( param0 );
    break;
}
case AGKI_GETLOCALNOTIFICATIONEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetLocalNotificationExists( param0 );
    break;
}
case AGKI_GETLOCALNOTIFICATIONTIME_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetLocalNotificationTime( param0 );
    break;
}
case AGKI_GETLOCALNOTIFICATIONMESSAGE_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetLocalNotificationMessage( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_MAKECOLOR_L_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::MakeColor( param0, param1, param2 );
    break;
}
case AGKI_GETCOLORRED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetColorRed( param0 );
    break;
}
case AGKI_GETCOLORGREEN_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetColorGreen( param0 );
    break;
}
case AGKI_GETCOLORBLUE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetColorBlue( param0 );
    break;
}
case AGKI_DRAWLINE_0_F_F_F_F_L_L_L:
{
    int param6 = m_pStack[ --m_iStackPtr ].i;
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::DrawLine( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_DRAWLINE_0_F_F_F_F_L_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::DrawLine( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_DRAWBOX_0_F_F_F_F_L_L_L_L_L:
{
    int param8 = m_pStack[ --m_iStackPtr ].i;
    int param7 = m_pStack[ --m_iStackPtr ].i;
    int param6 = m_pStack[ --m_iStackPtr ].i;
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::DrawBox( param0, param1, param2, param3, param4, param5, param6, param7, param8 );
    break;
}
case AGKI_DRAWELLIPSE_0_F_F_F_F_L_L_L:
{
    int param6 = m_pStack[ --m_iStackPtr ].i;
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::DrawEllipse( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_CREATEMEMBLOCK_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateMemblock( param0 );
    break;
}
case AGKI_CREATEMEMBLOCK_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateMemblock( param0, param1 );
    break;
}
case AGKI_GETMEMBLOCKEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMemblockExists( param0 );
    break;
}
case AGKI_DELETEMEMBLOCK_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteMemblock( param0 );
    break;
}
case AGKI_COPYMEMBLOCK_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CopyMemblock( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_GETMEMBLOCKSIZE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMemblockSize( param0 );
    break;
}
case AGKI_GETMEMBLOCKBYTE_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMemblockByte( param0, param1 );
    break;
}
case AGKI_GETMEMBLOCKBYTESIGNED_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMemblockByteSigned( param0, param1 );
    break;
}
case AGKI_GETMEMBLOCKSHORT_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMemblockShort( param0, param1 );
    break;
}
case AGKI_GETMEMBLOCKINT_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMemblockInt( param0, param1 );
    break;
}
case AGKI_GETMEMBLOCKFLOAT_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMemblockFloat( param0, param1 );
    break;
}
case AGKI_GETMEMBLOCKSTRING_S_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetMemblockString( param0, param1, param2 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETMEMBLOCKBYTE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMemblockByte( param0, param1, param2 );
    break;
}
case AGKI_SETMEMBLOCKBYTESIGNED_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMemblockByteSigned( param0, param1, param2 );
    break;
}
case AGKI_SETMEMBLOCKSHORT_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMemblockShort( param0, param1, param2 );
    break;
}
case AGKI_SETMEMBLOCKINT_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMemblockInt( param0, param1, param2 );
    break;
}
case AGKI_SETMEMBLOCKFLOAT_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMemblockFloat( param0, param1, param2 );
    break;
}
case AGKI_SETMEMBLOCKSTRING_0_L_L_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMemblockString( param0, param1, param2 );
    break;
}
case AGKI_CREATEMEMBLOCKFROMIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateMemblockFromImage( param0, param1 );
    break;
}
case AGKI_CREATEMEMBLOCKFROMIMAGE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateMemblockFromImage( param0 );
    break;
}
case AGKI_CREATEIMAGEFROMMEMBLOCK_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateImageFromMemblock( param0, param1 );
    break;
}
case AGKI_CREATEIMAGEFROMMEMBLOCK_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateImageFromMemblock( param0 );
    break;
}
case AGKI_CREATEMEMBLOCKFROMSOUND_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateMemblockFromSound( param0, param1 );
    break;
}
case AGKI_CREATEMEMBLOCKFROMSOUND_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateMemblockFromSound( param0 );
    break;
}
case AGKI_CREATESOUNDFROMMEMBLOCK_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateSoundFromMemblock( param0, param1 );
    break;
}
case AGKI_CREATESOUNDFROMMEMBLOCK_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateSoundFromMemblock( param0 );
    break;
}
case AGKI_CREATEMEMBLOCKFROMFILE_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateMemblockFromFile( param0, param1 );
    break;
}
case AGKI_CREATEMEMBLOCKFROMFILE_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CreateMemblockFromFile( param0 );
    break;
}
case AGKI_CREATEFILEFROMMEMBLOCK_0_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::CreateFileFromMemblock( param0, param1 );
    break;
}
case AGKI_CREATEMEMBLOCKFROMOBJECTMESH_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateMemblockFromObjectMesh( param0, param1, param2 );
    break;
}
case AGKI_CREATEMEMBLOCKFROMOBJECTMESH_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateMemblockFromObjectMesh( param0, param1 );
    break;
}
case AGKI_CREATEOBJECTFROMMESHMEMBLOCK_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectFromMeshMemblock( param0, param1 );
    break;
}
case AGKI_CREATEOBJECTFROMMESHMEMBLOCK_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectFromMeshMemblock( param0 );
    break;
}
case AGKI_SETOBJECTMESHFROMMEMBLOCK_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshFromMemblock( param0, param1, param2 );
    break;
}
case AGKI_ADDOBJECTMESHFROMMEMBLOCK_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddObjectMeshFromMemblock( param0, param1 );
    break;
}
case AGKI_SETMESHMEMBLOCKVERTEXPOSITION_0_L_L_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMeshMemblockVertexPosition( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETMESHMEMBLOCKVERTEXNORMAL_0_L_L_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMeshMemblockVertexNormal( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETMESHMEMBLOCKVERTEXUV_0_L_L_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMeshMemblockVertexUV( param0, param1, param2, param3 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMeshMemblockVertexX( param0, param1 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMeshMemblockVertexY( param0, param1 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXZ_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMeshMemblockVertexZ( param0, param1 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXNORMALX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMeshMemblockVertexNormalX( param0, param1 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXNORMALY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMeshMemblockVertexNormalY( param0, param1 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXNORMALZ_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMeshMemblockVertexNormalZ( param0, param1 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXU_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMeshMemblockVertexU( param0, param1 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXV_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetMeshMemblockVertexV( param0, param1 );
    break;
}
case AGKI_SETGLOBAL3DDEPTH_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetGlobal3DDepth( param0 );
    break;
}
case AGKI_CREATEOBJECTBOX_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectBox( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATEOBJECTBOX_L_F_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectBox( param0, param1, param2 );
    break;
}
case AGKI_CREATEOBJECTCAPSULE_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectCapsule( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATEOBJECTCAPSULE_L_F_F_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectCapsule( param0, param1, param2 );
    break;
}
case AGKI_CREATEOBJECTSPHERE_0_L_F_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectSphere( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATEOBJECTSPHERE_L_F_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectSphere( param0, param1, param2 );
    break;
}
case AGKI_CREATEOBJECTCONE_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectCone( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATEOBJECTCONE_L_F_F_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectCone( param0, param1, param2 );
    break;
}
case AGKI_CREATEOBJECTCYLINDER_0_L_F_F_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectCylinder( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATEOBJECTCYLINDER_L_F_F_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectCylinder( param0, param1, param2 );
    break;
}
case AGKI_CREATEOBJECTPLANE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectPlane( param0, param1, param2 );
    break;
}
case AGKI_CREATEOBJECTPLANE_L_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectPlane( param0, param1 );
    break;
}
case AGKI_CREATEOBJECTQUAD_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectQuad( param0 );
    break;
}
case AGKI_CREATEOBJECTQUAD_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectQuad( );
    break;
}
case AGKI_CREATEOBJECTFROMHEIGHTMAP_L_S_F_F_F_L_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectFromHeightMap( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_CREATEOBJECTFROMHEIGHTMAP_0_L_S_F_F_F_L_L:
{
    int param6 = m_pStack[ --m_iStackPtr ].i;
    int param5 = m_pStack[ --m_iStackPtr ].i;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectFromHeightMap( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_CREATEOBJECTFROMOBJECTMESH_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectFromObjectMesh( param0, param1 );
    break;
}
case AGKI_CREATEOBJECTFROMOBJECTMESH_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectFromObjectMesh( param0, param1, param2 );
    break;
}
case AGKI_LOADOBJECT_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadObject( param0 );
    break;
}
case AGKI_LOADOBJECT_L_S_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadObject( param0, param1 );
    break;
}
case AGKI_LOADOBJECT_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadObject( param0, param1 );
    break;
}
case AGKI_LOADOBJECT_0_L_S_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadObject( param0, param1, param2 );
    break;
}
case AGKI_LOADOBJECTWITHCHILDREN_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadObjectWithChildren( param0 );
    break;
}
case AGKI_LOADOBJECTWITHCHILDREN_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadObjectWithChildren( param0, param1 );
    break;
}
case AGKI_SAVEOBJECT_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SaveObject( param0, param1 );
    break;
}
case AGKI_CLONEOBJECT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CloneObject( param0 );
    break;
}
case AGKI_CLONEOBJECT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CloneObject( param0, param1 );
    break;
}
case AGKI_INSTANCEOBJECT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::InstanceObject( param0 );
    break;
}
case AGKI_INSTANCEOBJECT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::InstanceObject( param0, param1 );
    break;
}
case AGKI_GETOBJECTEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectExists( param0 );
    break;
}
case AGKI_DELETEOBJECT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteObject( param0 );
    break;
}
case AGKI_DELETEOBJECTWITHCHILDREN_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteObjectWithChildren( param0 );
    break;
}
case AGKI_DELETEALLOBJECTS_0_0:
{
    agk::DeleteAllObjects( );
    break;
}
case AGKI_GETOBJECTNUMCHILDREN_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectNumChildren( param0 );
    break;
}
case AGKI_GETOBJECTCHILDID_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectChildID( param0, param1 );
    break;
}
case AGKI_GETOBJECTNUMBONES_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectNumBones( param0 );
    break;
}
case AGKI_GETOBJECTBONEBYNAME_L_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectBoneByName( param0, param1 );
    break;
}
case AGKI_GETOBJECTNUMMESHES_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectNumMeshes( param0 );
    break;
}
case AGKI_GETOBJECTMESHNAME_S_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetObjectMeshName( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETOBJECTMESHIMAGE_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshImage( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTMESHLIGHTMAP_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshLightMap( param0, param1, param2 );
    break;
}
case AGKI_SETOBJECTMESHSHADER_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshShader( param0, param1, param2 );
    break;
}
case AGKI_GETOBJECTMESHVSSOURCE_S_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetObjectMeshVSSource( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETOBJECTMESHPSSOURCE_S_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetObjectMeshPSSource( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETOBJECTMESHUVOFFSET_0_L_L_L_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshUVOffset( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETOBJECTMESHUVSCALE_0_L_L_L_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshUVScale( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_GETOBJECTMESHSIZEMINX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectMeshSizeMinX( param0, param1 );
    break;
}
case AGKI_GETOBJECTMESHSIZEMAXX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectMeshSizeMaxX( param0, param1 );
    break;
}
case AGKI_GETOBJECTMESHSIZEMINY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectMeshSizeMinY( param0, param1 );
    break;
}
case AGKI_GETOBJECTMESHSIZEMAXY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectMeshSizeMaxY( param0, param1 );
    break;
}
case AGKI_GETOBJECTMESHSIZEMINZ_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectMeshSizeMinZ( param0, param1 );
    break;
}
case AGKI_GETOBJECTMESHSIZEMAXZ_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectMeshSizeMaxZ( param0, param1 );
    break;
}
case AGKI_FIXOBJECTTOOBJECT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::FixObjectToObject( param0, param1 );
    break;
}
case AGKI_FIXOBJECTTOBONE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::FixObjectToBone( param0, param1, param2 );
    break;
}
case AGKI_GETOBJECTNUMANIMATIONS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectNumAnimations( param0 );
    break;
}
case AGKI_GETOBJECTANIMATIONNAME_S_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetObjectAnimationName( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_PLAYOBJECTANIMATION_0_L_S_F_F_L_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayObjectAnimation( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETOBJECTANIMATIONFRAME_0_L_S_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectAnimationFrame( param0, param1, param2, param3 );
    break;
}
case AGKI_STOPOBJECTANIMATION_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::StopObjectAnimation( param0 );
    break;
}
case AGKI_RESETOBJECTANIMATION_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResetObjectAnimation( param0 );
    break;
}
case AGKI_SETOBJECTANIMATIONSPEED_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectAnimationSpeed( param0, param1 );
    break;
}
case AGKI_GETOBJECTISANIMATING_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectIsAnimating( param0 );
    break;
}
case AGKI_GETOBJECTISTWEENING_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectIsTweening( param0 );
    break;
}
case AGKI_GETOBJECTANIMATIONTIME_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectAnimationTime( param0 );
    break;
}
case AGKI_GETOBJECTANIMATIONDURATION_F_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectAnimationDuration( param0, param1 );
    break;
}
case AGKI_SETOBJECTBONEPOSITION_0_L_L_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectBonePosition( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETOBJECTBONEROTATION_0_L_L_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectBoneRotation( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETOBJECTBONEROTATIONQUAT_0_L_L_F_F_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectBoneRotationQuat( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETOBJECTBONELOOKAT_0_L_L_F_F_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectBoneLookAt( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETOBJECTBONECANANIMATE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectBoneCanAnimate( param0, param1, param2 );
    break;
}
case AGKI_ROTATEOBJECTBONELOCALX_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateObjectBoneLocalX( param0, param1, param2 );
    break;
}
case AGKI_ROTATEOBJECTBONELOCALY_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateObjectBoneLocalY( param0, param1, param2 );
    break;
}
case AGKI_ROTATEOBJECTBONELOCALZ_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateObjectBoneLocalZ( param0, param1, param2 );
    break;
}
case AGKI_GETOBJECTBONENAME_S_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetObjectBoneName( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETOBJECTBONEX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneX( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneY( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEZ_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneZ( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEANGLEX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneAngleX( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEANGLEY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneAngleY( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEANGLEZ_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneAngleZ( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEQUATW_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneQuatW( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEQUATX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneQuatX( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEQUATY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneQuatY( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEQUATZ_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneQuatZ( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEWORLDX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneWorldX( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEWORLDY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneWorldY( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEWORLDZ_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneWorldZ( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEWORLDANGLEX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneWorldAngleX( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEWORLDANGLEY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneWorldAngleY( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEWORLDANGLEZ_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneWorldAngleZ( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEWORLDQUATW_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneWorldQuatW( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEWORLDQUATX_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneWorldQuatX( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEWORLDQUATY_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneWorldQuatY( param0, param1 );
    break;
}
case AGKI_GETOBJECTBONEWORLDQUATZ_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectBoneWorldQuatZ( param0, param1 );
    break;
}
case AGKI_SETOBJECTPOSITION_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectPosition( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTROTATION_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectRotation( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTROTATIONQUAT_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectRotationQuat( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETOBJECTSCALE_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectScale( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTSCALEPERMANENT_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectScalePermanent( param0, param1, param2, param3 );
    break;
}
case AGKI_MOVEOBJECTLOCALX_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::MoveObjectLocalX( param0, param1 );
    break;
}
case AGKI_MOVEOBJECTLOCALY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::MoveObjectLocalY( param0, param1 );
    break;
}
case AGKI_MOVEOBJECTLOCALZ_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::MoveObjectLocalZ( param0, param1 );
    break;
}
case AGKI_ROTATEOBJECTLOCALX_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateObjectLocalX( param0, param1 );
    break;
}
case AGKI_ROTATEOBJECTLOCALY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateObjectLocalY( param0, param1 );
    break;
}
case AGKI_ROTATEOBJECTLOCALZ_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateObjectLocalZ( param0, param1 );
    break;
}
case AGKI_ROTATEOBJECTGLOBALX_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateObjectGlobalX( param0, param1 );
    break;
}
case AGKI_ROTATEOBJECTGLOBALY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateObjectGlobalY( param0, param1 );
    break;
}
case AGKI_ROTATEOBJECTGLOBALZ_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateObjectGlobalZ( param0, param1 );
    break;
}
case AGKI_GETOBJECTX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectX( param0 );
    break;
}
case AGKI_GETOBJECTY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectY( param0 );
    break;
}
case AGKI_GETOBJECTZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectZ( param0 );
    break;
}
case AGKI_GETOBJECTANGLEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectAngleX( param0 );
    break;
}
case AGKI_GETOBJECTANGLEY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectAngleY( param0 );
    break;
}
case AGKI_GETOBJECTANGLEZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectAngleZ( param0 );
    break;
}
case AGKI_GETOBJECTQUATW_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectQuatW( param0 );
    break;
}
case AGKI_GETOBJECTQUATX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectQuatX( param0 );
    break;
}
case AGKI_GETOBJECTQUATY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectQuatY( param0 );
    break;
}
case AGKI_GETOBJECTQUATZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectQuatZ( param0 );
    break;
}
case AGKI_GETOBJECTWORLDX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectWorldX( param0 );
    break;
}
case AGKI_GETOBJECTWORLDY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectWorldY( param0 );
    break;
}
case AGKI_GETOBJECTWORLDZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectWorldZ( param0 );
    break;
}
case AGKI_GETOBJECTWORLDANGLEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectWorldAngleX( param0 );
    break;
}
case AGKI_GETOBJECTWORLDANGLEY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectWorldAngleY( param0 );
    break;
}
case AGKI_GETOBJECTWORLDANGLEZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectWorldAngleZ( param0 );
    break;
}
case AGKI_GETOBJECTWORLDQUATW_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectWorldQuatW( param0 );
    break;
}
case AGKI_GETOBJECTWORLDQUATX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectWorldQuatX( param0 );
    break;
}
case AGKI_GETOBJECTWORLDQUATY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectWorldQuatY( param0 );
    break;
}
case AGKI_GETOBJECTWORLDQUATZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectWorldQuatZ( param0 );
    break;
}
case AGKI_GETOBJECTSIZEMINX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectSizeMinX( param0 );
    break;
}
case AGKI_GETOBJECTSIZEMAXX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectSizeMaxX( param0 );
    break;
}
case AGKI_GETOBJECTSIZEMINY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectSizeMinY( param0 );
    break;
}
case AGKI_GETOBJECTSIZEMAXY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectSizeMaxY( param0 );
    break;
}
case AGKI_GETOBJECTSIZEMINZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectSizeMinZ( param0 );
    break;
}
case AGKI_GETOBJECTSIZEMAXZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectSizeMaxZ( param0 );
    break;
}
case AGKI_SETOBJECTLOOKAT_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectLookAt( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_FIXOBJECTPIVOT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::FixObjectPivot( param0 );
    break;
}
case AGKI_GETOBJECTHEIGHTMAPHEIGHT_F_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectHeightMapHeight( param0, param1, param2 );
    break;
}
case AGKI_SETOBJECTIMAGE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectImage( param0, param1, param2 );
    break;
}
case AGKI_SETOBJECTLIGHTMAP_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectLightMap( param0, param1 );
    break;
}
case AGKI_SETOBJECTSHADER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShader( param0, param1 );
    break;
}
case AGKI_SETOBJECTCOLOR_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectColor( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETOBJECTCOLOREMISSIVE_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectColorEmissive( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTLIGHTMODE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectLightMode( param0, param1 );
    break;
}
case AGKI_SETOBJECTSCREENCULLING_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectScreenCulling( param0, param1 );
    break;
}
case AGKI_SETOBJECTUVOFFSET_0_L_L_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectUVOffset( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTUVSCALE_0_L_L_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectUVScale( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTFOGMODE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectFogMode( param0, param1 );
    break;
}
case AGKI_SETOBJECTDEPTHREADMODE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectDepthReadMode( param0, param1 );
    break;
}
case AGKI_SETOBJECTDEPTHWRITE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectDepthWrite( param0, param1 );
    break;
}
case AGKI_SETOBJECTDEPTHBIAS_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectDepthBias( param0, param1 );
    break;
}
case AGKI_SETOBJECTDEPTHRANGE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectDepthRange( param0, param1, param2 );
    break;
}
case AGKI_SETOBJECTTRANSPARENCY_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectTransparency( param0, param1 );
    break;
}
case AGKI_SETOBJECTALPHAMASK_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectAlphaMask( param0, param1 );
    break;
}
case AGKI_SETOBJECTCULLMODE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectCullMode( param0, param1 );
    break;
}
case AGKI_SETOBJECTVISIBLE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectVisible( param0, param1 );
    break;
}
case AGKI_GETOBJECTDEPTHREADMODE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectDepthReadMode( param0 );
    break;
}
case AGKI_GETOBJECTDEPTHWRITE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectDepthWrite( param0 );
    break;
}
case AGKI_GETOBJECTDEPTHBIAS_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectDepthBias( param0 );
    break;
}
case AGKI_GETOBJECTTRANSPARENCY_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectTransparency( param0 );
    break;
}
case AGKI_GETOBJECTCULLMODE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectCullMode( param0 );
    break;
}
case AGKI_GETOBJECTVISIBLE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectVisible( param0 );
    break;
}
case AGKI_GETOBJECTINSCREEN_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectInScreen( param0 );
    break;
}
case AGKI_GETOBJECTNAME_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetObjectName( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETOBJECTSHADERCONSTANTBYNAME_0_L_S_F_F_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShaderConstantByName( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETOBJECTSHADERCONSTANTARRAYBYNAME_0_L_S_L_F_F_F_F:
{
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShaderConstantArrayByName( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_SETOBJECTSHADERCONSTANTDEFAULT_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShaderConstantDefault( param0, param1 );
    break;
}
case AGKI_DRAWOBJECT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DrawObject( param0 );
    break;
}
case AGKI_GETSCREENXFROM3D_F_F_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::GetScreenXFrom3D( param0, param1, param2 );
    break;
}
case AGKI_GETSCREENYFROM3D_F_F_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::GetScreenYFrom3D( param0, param1, param2 );
    break;
}
case AGKI_GET3DVECTORXFROMSCREEN_F_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DVectorXFromScreen( param0, param1 );
    break;
}
case AGKI_GET3DVECTORYFROMSCREEN_F_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DVectorYFromScreen( param0, param1 );
    break;
}
case AGKI_GET3DVECTORZFROMSCREEN_F_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DVectorZFromScreen( param0, param1 );
    break;
}
case AGKI_SETOBJECTCOLLISIONMODE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectCollisionMode( param0, param1 );
    break;
}
case AGKI_OBJECTRAYCAST_L_L_F_F_F_F_F_F:
{
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ObjectRayCast( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_OBJECTSPHERECAST_L_L_F_F_F_F_F_F_F:
{
    float param7 = m_pStack[ --m_iStackPtr ].f;
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ObjectSphereCast( param0, param1, param2, param3, param4, param5, param6, param7 );
    break;
}
case AGKI_OBJECTSPHERESLIDE_L_L_F_F_F_F_F_F_F:
{
    float param7 = m_pStack[ --m_iStackPtr ].f;
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ObjectSphereSlide( param0, param1, param2, param3, param4, param5, param6, param7 );
    break;
}
case AGKI_GETOBJECTRAYCASTNUMHITS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectRayCastNumHits( );
    break;
}
case AGKI_GETOBJECTRAYCASTHITID_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectRayCastHitID( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastX( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastY( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastZ( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTSLIDEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastSlideX( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTSLIDEY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastSlideY( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTSLIDEZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastSlideZ( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTNORMALX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastNormalX( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTNORMALY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastNormalY( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTNORMALZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastNormalZ( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTBOUNCEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastBounceX( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTBOUNCEY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastBounceY( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTBOUNCEZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastBounceZ( param0 );
    break;
}
case AGKI_GETOBJECTRAYCASTDISTANCE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObjectRayCastDistance( param0 );
    break;
}
case AGKI_SETFOGMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetFogMode( param0 );
    break;
}
case AGKI_SETFOGCOLOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetFogColor( param0, param1, param2 );
    break;
}
case AGKI_SETFOGSUNCOLOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetFogSunColor( param0, param1, param2 );
    break;
}
case AGKI_SETFOGRANGE_0_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetFogRange( param0, param1 );
    break;
}
case AGKI_GETFOGMODE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetFogMode( );
    break;
}
case AGKI_SETSKYBOXVISIBLE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkyBoxVisible( param0 );
    break;
}
case AGKI_SETSKYBOXSKYCOLOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkyBoxSkyColor( param0, param1, param2 );
    break;
}
case AGKI_SETSKYBOXHORIZONCOLOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkyBoxHorizonColor( param0, param1, param2 );
    break;
}
case AGKI_SETSKYBOXSUNCOLOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkyBoxSunColor( param0, param1, param2 );
    break;
}
case AGKI_SETSKYBOXHORIZONSIZE_0_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetSkyBoxHorizonSize( param0, param1 );
    break;
}
case AGKI_SETSKYBOXSUNVISIBLE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSkyBoxSunVisible( param0 );
    break;
}
case AGKI_SETSKYBOXSUNSIZE_0_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetSkyBoxSunSize( param0, param1 );
    break;
}
case AGKI_LOADSHADER_L_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadShader( param0, param1 );
    break;
}
case AGKI_LOADSHADER_0_L_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadShader( param0, param1, param2 );
    break;
}
case AGKI_LOADFULLSCREENSHADER_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadFullScreenShader( param0 );
    break;
}
case AGKI_LOADFULLSCREENSHADER_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadFullScreenShader( param0, param1 );
    break;
}
case AGKI_LOADSPRITESHADER_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadSpriteShader( param0 );
    break;
}
case AGKI_LOADSPRITESHADER_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadSpriteShader( param0, param1 );
    break;
}
case AGKI_SETSHADERCONSTANTBYNAME_0_L_S_F_F_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetShaderConstantByName( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETSHADERCONSTANTARRAYBYNAME_0_L_S_L_F_F_F_F:
{
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetShaderConstantArrayByName( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_SETCAMERAPOSITION_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetCameraPosition( param0, param1, param2, param3 );
    break;
}
case AGKI_SETCAMERAROTATIONQUAT_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetCameraRotationQuat( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETCAMERAROTATION_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetCameraRotation( param0, param1, param2, param3 );
    break;
}
case AGKI_MOVECAMERALOCALX_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::MoveCameraLocalX( param0, param1 );
    break;
}
case AGKI_MOVECAMERALOCALY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::MoveCameraLocalY( param0, param1 );
    break;
}
case AGKI_MOVECAMERALOCALZ_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::MoveCameraLocalZ( param0, param1 );
    break;
}
case AGKI_ROTATECAMERALOCALX_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateCameraLocalX( param0, param1 );
    break;
}
case AGKI_ROTATECAMERALOCALY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateCameraLocalY( param0, param1 );
    break;
}
case AGKI_ROTATECAMERALOCALZ_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateCameraLocalZ( param0, param1 );
    break;
}
case AGKI_ROTATECAMERAGLOBALX_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateCameraGlobalX( param0, param1 );
    break;
}
case AGKI_ROTATECAMERAGLOBALY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateCameraGlobalY( param0, param1 );
    break;
}
case AGKI_ROTATECAMERAGLOBALZ_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RotateCameraGlobalZ( param0, param1 );
    break;
}
case AGKI_GETCAMERAX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraX( param0 );
    break;
}
case AGKI_GETCAMERAY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraY( param0 );
    break;
}
case AGKI_GETCAMERAZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraZ( param0 );
    break;
}
case AGKI_GETCAMERAANGLEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraAngleX( param0 );
    break;
}
case AGKI_GETCAMERAANGLEY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraAngleY( param0 );
    break;
}
case AGKI_GETCAMERAANGLEZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraAngleZ( param0 );
    break;
}
case AGKI_GETCAMERAQUATW_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraQuatW( param0 );
    break;
}
case AGKI_GETCAMERAQUATX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraQuatX( param0 );
    break;
}
case AGKI_GETCAMERAQUATY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraQuatY( param0 );
    break;
}
case AGKI_GETCAMERAQUATZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraQuatZ( param0 );
    break;
}
case AGKI_SETCAMERALOOKAT_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetCameraLookAt( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETCAMERARANGE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetCameraRange( param0, param1, param2 );
    break;
}
case AGKI_SETCAMERAASPECT_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetCameraAspect( param0, param1 );
    break;
}
case AGKI_SETCAMERAFOV_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetCameraFOV( param0, param1 );
    break;
}
case AGKI_SETCAMERAORTHOWIDTH_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetCameraOrthoWidth( param0, param1 );
    break;
}
case AGKI_SETCAMERABOUNDS_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetCameraBounds( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETCAMERAOFFCENTER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetCameraOffCenter( param0, param1 );
    break;
}
case AGKI_GETCAMERAFOV_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetCameraFOV( param0 );
    break;
}
case AGKI_CREATEPOINTLIGHT_0_L_F_F_F_F_L_L_L:
{
    int param7 = m_pStack[ --m_iStackPtr ].i;
    int param6 = m_pStack[ --m_iStackPtr ].i;
    int param5 = m_pStack[ --m_iStackPtr ].i;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreatePointLight( param0, param1, param2, param3, param4, param5, param6, param7 );
    break;
}
case AGKI_GETPOINTLIGHTEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetPointLightExists( param0 );
    break;
}
case AGKI_DELETEPOINTLIGHT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeletePointLight( param0 );
    break;
}
case AGKI_CLEARPOINTLIGHTS_0_0:
{
    agk::ClearPointLights( );
    break;
}
case AGKI_SETPOINTLIGHTPOSITION_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPointLightPosition( param0, param1, param2, param3 );
    break;
}
case AGKI_SETPOINTLIGHTCOLOR_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPointLightColor( param0, param1, param2, param3 );
    break;
}
case AGKI_SETPOINTLIGHTRADIUS_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPointLightRadius( param0, param1 );
    break;
}
case AGKI_SETPOINTLIGHTMODE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPointLightMode( param0, param1 );
    break;
}
case AGKI_SETSUNDIRECTION_0_F_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetSunDirection( param0, param1, param2 );
    break;
}
case AGKI_SETSUNCOLOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSunColor( param0, param1, param2 );
    break;
}
case AGKI_SETSUNACTIVE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSunActive( param0 );
    break;
}
case AGKI_SETAMBIENTCOLOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetAmbientColor( param0, param1, param2 );
    break;
}
case AGKI_CREATE3DPHYSICSWORLD_0_0:
{
    agk::Create3DPhysicsWorld( );
    break;
}
case AGKI_CREATE3DPHYSICSWORLD_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::Create3DPhysicsWorld( param0 );
    break;
}
case AGKI_SET3DPHYSICSGRAVITY_0_F_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::Set3DPhysicsGravity( param0, param1, param2 );
    break;
}
case AGKI_SET3DPHYSICSGRAVITY_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsGravity( param0 );
    break;
}
case AGKI_STEP3DPHYSICSWORLD_0_0:
{
    agk::Step3DPhysicsWorld( );
    break;
}
case AGKI_RESET3DPHYSICSWORLD_0_0:
{
    agk::Reset3DPhysicsWorld( );
    break;
}
case AGKI_DELETE3DPHYSICSWORLD_0_0:
{
    agk::Delete3DPhysicsWorld( );
    break;
}
case AGKI_DEBUG3DPHYSICSWORLD_0_0:
{
    agk::Debug3DPhysicsWorld( );
    break;
}
case AGKI_GET3DPHYSICSTOTALOBJECTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsTotalObjects( );
    break;
}
case AGKI_GET3DPHYSICSACTIVEOBJECTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsActiveObjects( );
    break;
}
case AGKI_GET3DPHYSICSTOTALJOINTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsTotalJoints( );
    break;
}
case AGKI_SETOBJECTSHAPEBOX_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeBox( param0 );
    break;
}
case AGKI_SETOBJECTSHAPEBOX_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeBox( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTSHAPEBOX_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeBox( param0, param1 );
    break;
}
case AGKI_SETOBJECTSHAPESPHERE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeSphere( param0 );
    break;
}
case AGKI_SETOBJECTSHAPESPHERE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeSphere( param0, param1 );
    break;
}
case AGKI_SETOBJECTSHAPECYLINDER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeCylinder( param0, param1 );
    break;
}
case AGKI_SETOBJECTSHAPECYLINDER_0_L_L_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeCylinder( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTSHAPECONE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeCone( param0, param1 );
    break;
}
case AGKI_SETOBJECTSHAPECONE_0_L_L_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeCone( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTSHAPECAPSULE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeCapsule( param0, param1 );
    break;
}
case AGKI_SETOBJECTSHAPECAPSULE_0_L_L_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeCapsule( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETOBJECTSHAPECAPSULE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeCapsule( param0, param1, param2 );
    break;
}
case AGKI_SETOBJECTSHAPECONVEXHULL_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeConvexHull( param0 );
    break;
}
case AGKI_SETOBJECTSHAPESTATICPOLYGON_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeStaticPolygon( param0 );
    break;
}
case AGKI_SETOBJECTSHAPECOMPOUND_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShapeCompound( param0 );
    break;
}
case AGKI_CREATE3DPHYSICSDYNAMICBODY_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Create3DPhysicsDynamicBody( param0 );
    break;
}
case AGKI_CREATE3DPHYSICSSTATICBODY_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Create3DPhysicsStaticBody( param0 );
    break;
}
case AGKI_CREATE3DPHYSICSKINEMATICBODY_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Create3DPhysicsKinematicBody( param0 );
    break;
}
case AGKI_DELETE3DPHYSICSBODY_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Delete3DPhysicsBody( param0 );
    break;
}
case AGKI_CREATE3DPHYSICSSTATICPLANE_L_F_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::Create3DPhysicsStaticPlane( param0, param1, param2, param3 );
    break;
}
case AGKI_SET3DPHYSICSSTATICPLANEPOSITION_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsStaticPlanePosition( param0, param1, param2, param3 );
    break;
}
case AGKI_SET3DPHYSICSSTATICPLANEROTATION_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsStaticPlaneRotation( param0, param1, param2, param3 );
    break;
}
case AGKI_DELETE3DPHYSICSSTATICPLANE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Delete3DPhysicsStaticPlane( param0 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSGROUPANDMASK_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsGroupAndMask( param0, param1, param2 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSGROUP_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObject3DPhysicsGroup( param0 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSMASK_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObject3DPhysicsMask( param0 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSMASS_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsMass( param0, param1 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSMASS_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsMass( param0 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSCANSLEEP_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsCanSleep( param0, param1 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSFRICTION_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsFriction( param0, param1 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSFRICTION_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsFriction( param0 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSROLLINGFRICTION_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsRollingFriction( param0, param1 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSROLLINGFRICTION_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsRollingFriction( param0 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSANISOTROPICFRICTION_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsAnisotropicFriction( param0, param1 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSMAXLINEARVELOCITY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsMaxLinearVelocity( param0, param1 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSLINEARVELOCITY_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsLinearVelocity( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSLINEARVELOCITY_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsLinearVelocity( param0, param1, param2 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSLINEARVELOCITYX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsLinearVelocityX( param0 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSLINEARVELOCITYY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsLinearVelocityY( param0 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSLINEARVELOCITYZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsLinearVelocityZ( param0 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSANGULARVELOCITY_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsAngularVelocity( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSANGULARVELOCITY_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsAngularVelocity( param0, param1, param2 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSANGULARVELOCITYX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsAngularVelocityX( param0 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSANGULARVELOCITYY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsAngularVelocityY( param0 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSANGULARVELOCITYZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsAngularVelocityZ( param0 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSDAMPING_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsDamping( param0, param1, param2 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSLINEARDAMP_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsLinearDamp( param0 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSANGULARDAMP_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsAngularDamp( param0 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSSLEEPINGTHRESHOLD_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsSleepingThreshold( param0, param1, param2 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSANGULARSLEEPINGTHRESHOLD_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsAngularSleepingThreshold( param0 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSLINEARSLEEPINGTHRESHOLD_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsLinearSleepingThreshold( param0 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSDEACTIVATIONTIME_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsDeactivationTime( param0, param1 );
    break;
}
case AGKI_SETOBJECT3DPHYSICSRESTITUTION_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObject3DPhysicsRestitution( param0, param1 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSRESTITUTION_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsRestitution( param0 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSFIRSTCONTACT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObject3DPhysicsFirstContact( param0 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSCONTACTX_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsContactX( );
    break;
}
case AGKI_GETOBJECT3DPHYSICSCONTACTY_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsContactY( );
    break;
}
case AGKI_GETOBJECT3DPHYSICSCONTACTZ_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsContactZ( );
    break;
}
case AGKI_GETOBJECT3DPHYSICSCONTACTVECTOR_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObject3DPhysicsContactVector( param0 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSCONTACTOBJECTB_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetObject3DPhysicsContactObjectB( );
    break;
}
case AGKI_GETOBJECT3DPHYSICSNEXTCONTACT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetObject3DPhysicsNextContact( );
    break;
}
case AGKI_GETOBJECTS3DPHYSICSCONTACTPOSITIONVECTOR_L_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjects3DPhysicsContactPositionVector( param0, param1, param2 );
    break;
}
case AGKI_CREATE3DPHYSICSPICKJOINT_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Create3DPhysicsPickJoint( param0, param1 );
    break;
}
case AGKI_UPDATE3DPHYSICSPICKJOINT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Update3DPhysicsPickJoint( param0, param1 );
    break;
}
case AGKI_DELETE3DPHYSICSPICKJOINT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Delete3DPhysicsPickJoint( param0 );
    break;
}
case AGKI_CREATE3DPHYSICSHINGEJOINT_L_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Create3DPhysicsHingeJoint( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CREATE3DPHYSICSCONETWISTJOINT_L_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Create3DPhysicsConeTwistJoint( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CREATE3DPHYSICSSLIDERJOINT_L_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Create3DPhysicsSliderJoint( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATE3DPHYSICSFIXEDJOINT_L_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Create3DPhysicsFixedJoint( param0, param1, param2 );
    break;
}
case AGKI_CREATE3DPHYSICS6DOFJOINT_L_L_L_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Create3DPhysics6DOFJoint( param0, param1, param2, param3 );
    break;
}
case AGKI_SET3DPHYSICSJOINTSLIDERANGULARLIMITS_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsJointSliderAngularLimits( param0, param1, param2 );
    break;
}
case AGKI_SET3DPHYSICSJOINTSLIDERLINEARLIMITS_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsJointSliderLinearLimits( param0, param1, param2 );
    break;
}
case AGKI_SET3DPHYSICSJOINTCONETWISTLIMITS_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsJointConeTwistLimits( param0, param1, param2, param3 );
    break;
}
case AGKI_SET3DPHYSICSJOINTHINGELIMITS_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsJointHingeLimits( param0, param1, param2 );
    break;
}
case AGKI_SET3DPHYSICSJOINTBREAKINGTHRESHOLD_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsJointBreakingThreshold( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSJOINTENABLED_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsJointEnabled( param0, param1 );
    break;
}
case AGKI_GET3DPHYSICSJOINTENABLED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsJointEnabled( param0 );
    break;
}
case AGKI_GET3DPHYSICSJOINTPOSITIONVECTOR_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsJointPositionVector( param0 );
    break;
}
case AGKI_GET3DPHYSICSJOINTROTATIONVECTOR_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsJointRotationVector( param0 );
    break;
}
case AGKI_DELETE3DPHYSICSJOINT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Delete3DPhysicsJoint( param0 );
    break;
}
case AGKI_SET3DPHYSICSHINGEJOINTMOTORISENABLED_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsHingeJointMotorIsEnabled( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSHINGEJOINTMAXMOTORIMPULSE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsHingeJointMaxMotorImpulse( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSHINGEJOINTMOTORVELOCITY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsHingeJointMotorVelocity( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSTWISTJOINTMOTORISENABLED_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsTwistJointMotorIsEnabled( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSTWISTJOINTMAXMOTORIMPULSE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsTwistJointMaxMotorImpulse( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSTWISTJOINTMOTORROTATIONTARGET_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsTwistJointMotorRotationTarget( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSSLIDERJOINTPOWEREDLINEARMOTORISENABLED_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsSliderJointPoweredLinearMotorIsEnabled( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSSLIDERJOINTMAXLINEARMOTORFORCE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsSliderJointMaxLinearMotorForce( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSSLIDERJOINTTARGETLINEARMOTORVELOCITY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsSliderJointTargetLinearMotorVelocity( param0, param1 );
    break;
}
case AGKI_ADDOBJECTSHAPEBOX_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddObjectShapeBox( param0, param1, param2, param3 );
    break;
}
case AGKI_ADDOBJECTSHAPESPHERE_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddObjectShapeSphere( param0, param1, param2 );
    break;
}
case AGKI_ADDOBJECTSHAPECAPSULE_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddObjectShapeCapsule( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_ADDOBJECTSHAPECONE_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddObjectShapeCone( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_ADDOBJECTSHAPECYLINDER_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddObjectShapeCylinder( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SAVEOBJECTSHAPE_L_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SaveObjectShape( param0, param1 );
    break;
}
case AGKI_LOADOBJECTSHAPE_L_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::LoadObjectShape( param0, param1 );
    break;
}
case AGKI_CREATEVECTOR3_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::CreateVector3( );
    break;
}
case AGKI_CREATEVECTOR3_L_F_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateVector3( param0, param1, param2 );
    break;
}
case AGKI_SETVECTOR3_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVector3( param0, param1, param2, param3 );
    break;
}
case AGKI_DELETEVECTOR3_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteVector3( param0 );
    break;
}
case AGKI_GETVECTOR3X_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetVector3X( param0 );
    break;
}
case AGKI_GETVECTOR3Y_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetVector3Y( param0 );
    break;
}
case AGKI_GETVECTOR3Z_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetVector3Z( param0 );
    break;
}
case AGKI_GETVECTOR3DISTANCE_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetVector3Distance( param0, param1 );
    break;
}
case AGKI_GETVECTOR3LENGTH_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetVector3Length( param0 );
    break;
}
case AGKI_GETVECTOR3DOT_F_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetVector3Dot( param0, param1 );
    break;
}
case AGKI_GETVECTOR3CROSS_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::GetVector3Cross( param0, param1, param2 );
    break;
}
case AGKI_GETVECTOR3MULTIPLY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::GetVector3Multiply( param0, param1 );
    break;
}
case AGKI_GETVECTOR3ADD_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::GetVector3Add( param0, param1 );
    break;
}
case AGKI_CREATE3DPHYSICSRAY_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::Create3DPhysicsRay( );
    break;
}
case AGKI_DELETE3DPHYSICSRAY_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Delete3DPhysicsRay( param0 );
    break;
}
case AGKI_RAY3DPHYSICSEXIST_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Ray3DPhysicsExist( param0 );
    break;
}
case AGKI_RAYCAST3DPHYSICS_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RayCast3DPhysics( param0, param1, param2, param3 );
    break;
}
case AGKI_RAYCAST3DPHYSICSOBJECT_L_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::RayCast3DPhysicsObject( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_GET3DPHYSICSRAYCASTOBJECTHIT_L_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsRayCastObjectHit( param0, param1 );
    break;
}
case AGKI_GET3DPHYSICSRAYCASTCLOSESTOBJECTHIT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsRayCastClosestObjectHit( param0 );
    break;
}
case AGKI_GET3DPHYSICSRAYCASTFRACTION_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DPhysicsRayCastFraction( param0 );
    break;
}
case AGKI_GET3DPHYSICSRAYCASTCONTACTPOSITION_L_L_F_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsRayCastContactPosition( param0, param1, param2 );
    break;
}
case AGKI_GET3DPHYSICSRAYCASTCLOSESTCONTACTPOSITION_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsRayCastClosestContactPosition( param0, param1 );
    break;
}
case AGKI_GET3DPHYSICSRAYCASTNORMALVECTOR_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Get3DPhysicsRayCastNormalVector( param0, param1 );
    break;
}
case AGKI_GET3DPHYSICSRAYCASTNUMHITS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsRayCastNumHits( param0 );
    break;
}
case AGKI_SPHERECAST3DPHYSICS_0_L_L_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SphereCast3DPhysics( param0, param1, param2, param3 );
    break;
}
case AGKI_SPHERECAST3DPHYSICSOBJECT_L_L_L_L_L_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SphereCast3DPhysicsObject( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_CREATE3DPHYSICSRAGDOLL_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Create3DPhysicsRagDoll( param0, param1 );
    break;
}
case AGKI_ADD3DPHYSICSRAGDOLLBONE_L_L_L_F_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Add3DPhysicsRagDollBone( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_ASSIGNTO3DPHYSICSRAGDOLLBONEOBJECTBONE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AssignTo3DPhysicsRagDollBoneObjectBone( param0, param1 );
    break;
}
case AGKI_ADD3DPHYSICSRAGDOLLHINGEJOINT_0_L_L_L_L_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Add3DPhysicsRagDollHingeJoint( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_ADD3DPHYSICSRAGDOLLTWISTJOINT_0_L_L_L_L_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Add3DPhysicsRagDollTwistJoint( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_FINALIZE3DPHYSICSRAGDOLL_0_0:
{
    agk::Finalize3DPhysicsRagDoll( );
    break;
}
case AGKI_DELETE3DPHYSICSRAGDOLL_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Delete3DPhysicsRagdoll( param0 );
    break;
}
case AGKI_GET3DPHYSICSRAGDOLLEXIST_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsRagdollExist( param0 );
    break;
}
case AGKI_GET3DPHYSICSRAGDOLLFROMBONEOBJECT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsRagdollFromBoneObject( param0 );
    break;
}
case AGKI_IS3DPHYSICSRAGDOLLSTATIC_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Is3dPhysicsRagdollStatic( param0 );
    break;
}
case AGKI_SET3DPHYSICSRAGDOLLSTATIC_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DphysicsRagdollStatic( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSRAGDOLLBONESVISIBLE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsRagdollBonesVisible( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSRAGDOLLDAMPING_0_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::Set3DPhysicsRagdollDamping( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSRAGDOLLSLEEPINGTHRESHOLDS_0_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::Set3DPhysicsRagdollSleepingThresholds( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSRAGDOLLDEACTIVATIONTIME_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::Set3DPhysicsRagdollDeactivationTime( param0 );
    break;
}
case AGKI_SET3DPHYSICSRAGDOLLDEACTIVATION_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsRagdollDeactivation( param0, param1 );
    break;
}
case AGKI_CREATE3DPHYSICSCHARACTERCONTROLLER_0_L_L_L_L_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Create3DPhysicsCharacterController( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_DELETE3DPHYSICSCHARACTERCONTROLLER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Delete3DPhysicsCharacterController( param0 );
    break;
}
case AGKI_GET3DPHYSICSCHARACTERCONTROLLEREXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DPhysicsCharacterControllerExists( param0 );
    break;
}
case AGKI_SET3DPHYSICSCHARACTERCONTROLLERGRAVITY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsCharacterControllerGravity( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSCHARACTERCONTROLLERFALLSPEED_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsCharacterControllerFallSpeed( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSCHARACTERCONTROLLERJUMPSPEED_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsCharacterControllerJumpSpeed( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSCHARACTERCONTROLLERMAXSLOPE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsCharacterControllerMaxSlope( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSCHARACTERCONTROLLERSTEPHEIGHT_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsCharacterControllerStepHeight( param0, param1 );
    break;
}
case AGKI_GET3DPHYSICSCHARACTERCONTROLLERMAXSLOPE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DPhysicsCharacterControllerMaxSlope( param0 );
    break;
}
case AGKI_GET3DPHYSICSCHARACTERCONTROLLERGRAVITY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DPhysicsCharacterControllerGravity( param0 );
    break;
}
case AGKI_MOVE3DPHYSICSCHARACTERCONTROLLER_0_L_L_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Move3DPhysicsCharacterController( param0, param1, param2 );
    break;
}
case AGKI_ROTATE3DPHYSICSCHARACTERCONTROLLER_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Rotate3DPhysicsCharacterController( param0, param1 );
    break;
}
case AGKI_JUMP3DPHYSICSCHARACTERCONTROLLER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Jump3DPhysicsCharacterController( param0 );
    break;
}
case AGKI_CROUCH3DPHYSICSCHARACTERCONTROLLER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Crouch3DPhysicsCharacterController( param0 );
    break;
}
case AGKI_STAND3DPHYSICSCHARACTERCONTROLLER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Stand3DPhysicsCharacterController( param0 );
    break;
}
case AGKI_DEBUG3DPHYSICSCHARACTERCONTROLLER_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Debug3DPhysicsCharacterController( param0, param1 );
    break;
}
case AGKI_SET3DPHYSICSCHARACTERCONTROLLERPOSITION_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DPhysicsCharacterControllerPosition( param0, param1, param2, param3 );
    break;
}
case AGKI_SETWINDOWPOSITION_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetWindowPosition( param0, param1 );
    break;
}
case AGKI_SETWINDOWSIZE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetWindowSize( param0, param1, param2 );
    break;
}
case AGKI_SETWINDOWSIZE_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetWindowSize( param0, param1, param2, param3 );
    break;
}
case AGKI_SETWINDOWALLOWRESIZE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetWindowAllowResize( param0 );
    break;
}
case AGKI_MAXIMIZEWINDOW_0_0:
{
    agk::MaximizeWindow( );
    break;
}
case AGKI_MINIMIZEAPP_0_0:
{
    agk::MinimizeApp( );
    break;
}
case AGKI_RESTOREAPP_0_0:
{
    agk::RestoreApp( );
    break;
}
case AGKI_SETIMMERSIVEMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetImmersiveMode( param0 );
    break;
}
case AGKI_SETSCREENRESOLUTION_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetScreenResolution( param0, param1 );
    break;
}
case AGKI_GETDEVICENAME_S_0:
{
    char *szReturnStr = agk::GetDeviceName( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETDEVICEBASENAME_S_0:
{
    char *szReturnStr = agk::GetDeviceBaseName( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETDEVICETYPE_S_0:
{
    char *szReturnStr = agk::GetDeviceType( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETAPPNAME_S_0:
{
    char *szReturnStr = agk::GetAppName( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETDEVICELANGUAGE_S_0:
{
    char *szReturnStr = agk::GetDeviceLanguage( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETSLEEPMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSleepMode( param0 );
    break;
}
case AGKI_SETEXPANSIONFILEKEY_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetExpansionFileKey( param0 );
    break;
}
case AGKI_SETEXPANSIONFILEVERSION_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetExpansionFileVersion( param0 );
    break;
}
case AGKI_GETEXPANSIONFILESTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetExpansionFileState( );
    break;
}
case AGKI_DOWNLOADEXPANSIONFILE_0_0:
{
    agk::DownloadExpansionFile( );
    break;
}
case AGKI_GETEXPANSIONFILEPROGRESS_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetExpansionFileProgress( );
    break;
}
case AGKI_SETWINDOWTITLE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetWindowTitle( param0 );
    break;
}
case AGKI_SLEEP_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Sleep( param0 );
    break;
}
case AGKI_SETVSYNC_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetVSync( param0 );
    break;
}
case AGKI_GETMAXDEVICEWIDTH_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetMaxDeviceWidth( );
    break;
}
case AGKI_GETMAXDEVICEHEIGHT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetMaxDeviceHeight( );
    break;
}
case AGKI_GETDEVICEDPI_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetDeviceDPI( );
    break;
}
case AGKI_GETAPPPACKAGENAME_S_0:
{
    char *szReturnStr = agk::GetAppPackageName( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETDEVICEPLATFORM_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetDevicePlatform( );
    break;
}
case AGKI_SIN_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::Sin( param0 );
    break;
}
case AGKI_COS_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::Cos( param0 );
    break;
}
case AGKI_TAN_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::Tan( param0 );
    break;
}
case AGKI_SINRAD_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::SinRad( param0 );
    break;
}
case AGKI_COSRAD_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::CosRad( param0 );
    break;
}
case AGKI_TANRAD_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::TanRad( param0 );
    break;
}
case AGKI_ASIN_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ASin( param0 );
    break;
}
case AGKI_ACOS_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ACos( param0 );
    break;
}
case AGKI_ATAN_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ATan( param0 );
    break;
}
case AGKI_ATAN2_F_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ATan2( param0, param1 );
    break;
}
case AGKI_ASINRAD_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ASinRad( param0 );
    break;
}
case AGKI_ACOSRAD_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ACosRad( param0 );
    break;
}
case AGKI_ATANRAD_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ATanRad( param0 );
    break;
}
case AGKI_ATAN2RAD_F_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::ATan2Rad( param0, param1 );
    break;
}
case AGKI_TRUNC_L_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::Trunc( param0 );
    break;
}
case AGKI_FLOOR_L_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::Floor( param0 );
    break;
}
case AGKI_CEIL_L_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::Ceil( param0 );
    break;
}
case AGKI_ROUND_L_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::Round( param0 );
    break;
}
case AGKI_SQRT_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::Sqrt( param0 );
    break;
}
case AGKI_ABS_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::Abs( param0 );
    break;
}
case AGKI_MOD_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Mod( param0, param1 );
    break;
}
case AGKI_FMOD_F_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::FMod( param0, param1 );
    break;
}
case AGKI_POW_F_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::Pow( param0, param1 );
    break;
}
case AGKI_LOG_F_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].f = agk::Log( param0 );
    break;
}
case AGKI_GETNUMDEVICECAMERAS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetNumDeviceCameras( );
    break;
}
case AGKI_SETDEVICECAMERATOIMAGE_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::SetDeviceCameraToImage( param0, param1 );
    break;
}
case AGKI_GETDEVICECAMERATYPE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetDeviceCameraType( param0 );
    break;
}
case AGKI_VIBRATEDEVICE_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::VibrateDevice( param0 );
    break;
}
case AGKI_LOADVIDEO_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadVideo( param0 );
    break;
}
case AGKI_DELETEVIDEO_0_0:
{
    agk::DeleteVideo( );
    break;
}
case AGKI_SETVIDEODIMENSIONS_0_F_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetVideoDimensions( param0, param1, param2, param3 );
    break;
}
case AGKI_PLAYVIDEOTOIMAGE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PlayVideoToImage( param0 );
    break;
}
case AGKI_PLAYVIDEO_0_0:
{
    agk::PlayVideo( );
    break;
}
case AGKI_PAUSEVIDEO_0_0:
{
    agk::PauseVideo( );
    break;
}
case AGKI_STOPVIDEO_0_0:
{
    agk::StopVideo( );
    break;
}
case AGKI_GETVIDEOPLAYING_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetVideoPlaying( );
    break;
}
case AGKI_GETVIDEOPOSITION_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetVideoPosition( );
    break;
}
case AGKI_GETVIDEODURATION_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetVideoDuration( );
    break;
}
case AGKI_SETVIDEOVOLUME_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetVideoVolume( param0 );
    break;
}
case AGKI_GETVIDEOWIDTH_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetVideoWidth( );
    break;
}
case AGKI_GETVIDEOHEIGHT_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetVideoHeight( );
    break;
}
case AGKI_SETVIDEOPOSITION_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetVideoPosition( param0 );
    break;
}
case AGKI_SETRAWWRITEPATH_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetRawWritePath( param0 );
    break;
}
case AGKI_GETWRITEPATH_S_0:
{
    char *szReturnStr = agk::GetWritePath( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETREADPATH_S_0:
{
    char *szReturnStr = agk::GetReadPath( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETDOCUMENTSPATH_S_0:
{
    char *szReturnStr = agk::GetDocumentsPath( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETCURRENTDIR_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::SetCurrentDir( param0 );
    break;
}
case AGKI_MAKEFOLDER_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::MakeFolder( param0 );
    break;
}
case AGKI_DELETEFOLDER_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::DeleteFolder( param0 );
    break;
}
case AGKI_GETMULTITOUCHEXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetMultiTouchExists( );
    break;
}
case AGKI_GETMOUSEEXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetMouseExists( );
    break;
}
case AGKI_GETKEYBOARDEXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetKeyboardExists( );
    break;
}
case AGKI_GETCAMERAEXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetCameraExists( );
    break;
}
case AGKI_GETGPSSENSOREXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetGPSSensorExists( );
    break;
}
case AGKI_SETRAWMOUSEVISIBLE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetRawMouseVisible( param0 );
    break;
}
case AGKI_SETRAWMOUSEPOSITION_0_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetRawMousePosition( param0, param1 );
    break;
}
case AGKI_GETUNIXTIME_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetUnixTime( );
    break;
}
case AGKI_GETDAYOFWEEK_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetDayOfWeek( );
    break;
}
case AGKI_GETCURRENTDATE_S_0:
{
    char *szReturnStr = agk::GetCurrentDate( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETCURRENTTIME_S_0:
{
    char *szReturnStr = agk::GetCurrentTime( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_OPENBROWSER_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::OpenBrowser( param0 );
    break;
}
case AGKI_RUNAPP_L_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::RunApp( param0, param1 );
    break;
}
case AGKI_GETAPPRUNNING_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetAppRunning( param0 );
    break;
}
case AGKI_TERMINATEAPP_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::TerminateApp( param0 );
    break;
}
case AGKI_VIEWFILE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::ViewFile( param0 );
    break;
}
case AGKI_SHARETEXT_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::ShareText( param0 );
    break;
}
case AGKI_SHAREIMAGE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::ShareImage( param0 );
    break;
}
case AGKI_SHAREIMAGEANDTEXT_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::ShareImageAndText( param0, param1 );
    break;
}
case AGKI_FACEBOOKACTIVATEAPPTRACKING_0_0:
{
    agk::FacebookActivateAppTracking( );
    break;
}
case AGKI_GETINTERNETSTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetInternetState( );
    break;
}
case AGKI_SETPUSHNOTIFICATIONKEYS_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetPushNotificationKeys( param0, param1 );
    break;
}
case AGKI_PUSHNOTIFICATIONSETUP_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::PushNotificationSetup( );
    break;
}
case AGKI_GETPUSHNOTIFICATIONTOKEN_S_0:
{
    char *szReturnStr = agk::GetPushNotificationToken( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETNOTIFICATIONTYPE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetNotificationType( );
    break;
}
case AGKI_SETNOTIFICATIONIMAGE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetNotificationImage( param0 );
    break;
}
case AGKI_SETNOTIFICATIONTEXT_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetNotificationText( param0 );
    break;
}
case AGKI_GETNFCEXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetNFCExists( );
    break;
}
case AGKI_STARTGPSTRACKING_0_0:
{
    agk::StartGPSTracking( );
    break;
}
case AGKI_STOPGPSTRACKING_0_0:
{
    agk::StopGPSTracking( );
    break;
}
case AGKI_GETRAWGPSLATITUDE_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawGPSLatitude( );
    break;
}
case AGKI_GETRAWGPSLONGITUDE_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawGPSLongitude( );
    break;
}
case AGKI_GETRAWGPSALTITUDE_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetRawGPSAltitude( );
    break;
}
case AGKI_GETGAMECENTEREXISTS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetGameCenterExists( );
    break;
}
case AGKI_GAMECENTERSETUP_0_0:
{
    agk::GameCenterSetup( );
    break;
}
case AGKI_GAMECENTERLOGIN_0_0:
{
    agk::GameCenterLogin( );
    break;
}
case AGKI_GETGAMECENTERLOGGEDIN_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetGameCenterLoggedIn( );
    break;
}
case AGKI_GETGAMECENTERPLAYERID_S_0:
{
    char *szReturnStr = agk::GetGameCenterPlayerID( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETGAMECENTERPLAYERDISPLAYNAME_S_0:
{
    char *szReturnStr = agk::GetGameCenterPlayerDisplayName( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GAMECENTERSUBMITSCORE_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::GameCenterSubmitScore( param0, param1 );
    break;
}
case AGKI_GAMECENTERSHOWLEADERBOARD_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::GameCenterShowLeaderBoard( param0 );
    break;
}
case AGKI_GAMECENTERSUBMITACHIEVEMENT_0_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::GameCenterSubmitAchievement( param0, param1 );
    break;
}
case AGKI_GAMECENTERACHIEVEMENTSSHOW_0_0:
{
    agk::GameCenterAchievementsShow( );
    break;
}
case AGKI_GAMECENTERACHIEVEMENTSRESET_0_0:
{
    agk::GameCenterAchievementsReset( );
    break;
}
case AGKI_SETSHAREDVARIABLEAPPGROUP_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetSharedVariableAppGroup( param0 );
    break;
}
case AGKI_SAVESHAREDVARIABLE_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SaveSharedVariable( param0, param1 );
    break;
}
case AGKI_LOADSHAREDVARIABLE_S_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::LoadSharedVariable( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_DELETESHAREDVARIABLE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::DeleteSharedVariable( param0 );
    break;
}
case AGKI_FIREBASESETUP_0_0:
{
    agk::FirebaseSetup( );
    break;
}
case AGKI_FIREBASELOGEVENT_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::FirebaseLogEvent( param0 );
    break;
}
case AGKI_ISSUPPORTEDDEPTHTEXTURE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::IsSupportedDepthTexture( );
    break;
}
case AGKI_SETANTIALIASMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetAntialiasMode( param0 );
    break;
}
case AGKI_RENDERSHADOWMAP_0_0:
{
    agk::RenderShadowMap( );
    break;
}
case AGKI_SETNETWORKALLOWCLIENTS_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetNetworkAllowClients( param0 );
    break;
}
case AGKI_KICKNETWORKCLIENT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::KickNetworkClient( param0, param1 );
    break;
}
case AGKI_SETOBJECTCASTSHADOW_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectCastShadow( param0, param1 );
    break;
}
case AGKI_SETOBJECTRECEIVESHADOW_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectReceiveShadow( param0, param1 );
    break;
}
case AGKI_GETOBJECTCASTSHADOWMODE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectCastShadowMode( param0 );
    break;
}
case AGKI_GETOBJECTRECEIVESHADOWMODE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectReceiveShadowMode( param0 );
    break;
}
case AGKI_GETSHADOWMAPPINGSUPPORTED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetShadowMappingSupported( );
    break;
}
case AGKI_SETSHADOWMAPPINGMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetShadowMappingMode( param0 );
    break;
}
case AGKI_GETSHADOWMAPPINGMODE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetShadowMappingMode( );
    break;
}
case AGKI_SETSHADOWMAPSIZE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetShadowMapSize( param0, param1 );
    break;
}
case AGKI_SETSHADOWRANGE_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetShadowRange( param0 );
    break;
}
case AGKI_SETSHADOWBIAS_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetShadowBias( param0 );
    break;
}
case AGKI_SETSHADOWLIGHTSTEPSIZE_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetShadowLightStepSize( param0 );
    break;
}
case AGKI_SETSHADOWSMOOTHING_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetShadowSmoothing( param0 );
    break;
}
case AGKI_GETSHADOWPOLYGONSDRAWN_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetShadowPolygonsDrawn( );
    break;
}
case AGKI_LOADSHADERFROMSTRING_L_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::LoadShaderFromString( param0, param1 );
    break;
}
case AGKI_LOADSHADERFROMSTRING_0_L_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::LoadShaderFromString( param0, param1, param2 );
    break;
}
case AGKI_CREATE3DPARTICLES_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Create3DParticles( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATE3DPARTICLES_L_F_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::Create3DParticles( param0, param1, param2 );
    break;
}
case AGKI_GET3DPARTICLESEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DParticlesExists( param0 );
    break;
}
case AGKI_DELETE3DPARTICLES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Delete3DParticles( param0 );
    break;
}
case AGKI_SET3DPARTICLESPOSITION_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesPosition( param0, param1, param2, param3 );
    break;
}
case AGKI_SET3DPARTICLESFREQUENCY_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesFrequency( param0, param1 );
    break;
}
case AGKI_SET3DPARTICLESSTARTZONE_0_L_F_F_F_F_F_F:
{
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesStartZone( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_SET3DPARTICLESDIRECTION_0_L_F_F_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesDirection( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SET3DPARTICLESVELOCITYRANGE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesVelocityRange( param0, param1, param2 );
    break;
}
case AGKI_SET3DPARTICLESDIRECTIONRANGE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesDirectionRange( param0, param1, param2 );
    break;
}
case AGKI_SET3DPARTICLESSIZE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesSize( param0, param1 );
    break;
}
case AGKI_SET3DPARTICLESLIFE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesLife( param0, param1 );
    break;
}
case AGKI_SET3DPARTICLESMAX_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesMax( param0, param1 );
    break;
}
case AGKI_RESET3DPARTICLECOUNT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Reset3DParticleCount( param0 );
    break;
}
case AGKI_SET3DPARTICLESIMAGE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesImage( param0, param1 );
    break;
}
case AGKI_SET3DPARTICLESVISIBLE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesVisible( param0, param1 );
    break;
}
case AGKI_SET3DPARTICLESACTIVE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesActive( param0, param1 );
    break;
}
case AGKI_SET3DPARTICLESTRANSPARENCY_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesTransparency( param0, param1 );
    break;
}
case AGKI_SET3DPARTICLESCOLORINTERPOLATION_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Set3DParticlesColorInterpolation( param0, param1 );
    break;
}
case AGKI_GET3DPARTICLESX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesX( param0 );
    break;
}
case AGKI_GET3DPARTICLESY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesY( param0 );
    break;
}
case AGKI_GET3DPARTICLESZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesZ( param0 );
    break;
}
case AGKI_GET3DPARTICLESVISIBLE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DParticlesVisible( param0 );
    break;
}
case AGKI_GET3DPARTICLESACTIVE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DParticlesActive( param0 );
    break;
}
case AGKI_GET3DPARTICLESFREQUENCY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesFrequency( param0 );
    break;
}
case AGKI_GET3DPARTICLESDIRECTIONX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesDirectionX( param0 );
    break;
}
case AGKI_GET3DPARTICLESDIRECTIONY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesDirectionY( param0 );
    break;
}
case AGKI_GET3DPARTICLESDIRECTIONZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesDirectionZ( param0 );
    break;
}
case AGKI_GET3DPARTICLESDIRECTIONRANGE1_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesDirectionRange1( param0 );
    break;
}
case AGKI_GET3DPARTICLESDIRECTIONRANGE2_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesDirectionRange2( param0 );
    break;
}
case AGKI_GET3DPARTICLESSIZE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesSize( param0 );
    break;
}
case AGKI_GET3DPARTICLESLIFE_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::Get3DParticlesLife( param0 );
    break;
}
case AGKI_GET3DPARTICLESMAXREACHED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::Get3DParticlesMaxReached( param0 );
    break;
}
case AGKI_ADD3DPARTICLESFORCE_0_L_F_F_F_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Add3DParticlesForce( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_CLEAR3DPARTICLESFORCES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Clear3DParticlesForces( param0 );
    break;
}
case AGKI_ADD3DPARTICLESCOLORKEYFRAME_0_L_F_L_L_L_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Add3DParticlesColorKeyFrame( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_CLEAR3DPARTICLESCOLORS_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Clear3DParticlesColors( param0 );
    break;
}
case AGKI_ADD3DPARTICLESSCALEKEYFRAME_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Add3DParticlesScaleKeyFrame( param0, param1, param2 );
    break;
}
case AGKI_CLEAR3DPARTICLESSCALES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Clear3DParticlesScales( param0 );
    break;
}
case AGKI_UPDATE3DPARTICLES_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Update3DParticles( param0, param1 );
    break;
}
case AGKI_OFFSET3DPARTICLES_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Offset3DParticles( param0, param1, param2, param3 );
    break;
}
case AGKI_SETMESHMEMBLOCKVERTEXCOLOR_0_L_L_L_L_L_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetMeshMemblockVertexColor( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXRED_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMeshMemblockVertexRed( param0, param1 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXGREEN_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMeshMemblockVertexGreen( param0, param1 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXBLUE_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMeshMemblockVertexBlue( param0, param1 );
    break;
}
case AGKI_GETMESHMEMBLOCKVERTEXALPHA_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetMeshMemblockVertexAlpha( param0, param1 );
    break;
}
case AGKI_SETOBJECTMESHNORMALMAP_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshNormalMap( param0, param1, param2 );
    break;
}
case AGKI_SETOBJECTNORMALMAP_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectNormalMap( param0, param1 );
    break;
}
case AGKI_SETOBJECTNORMALMAPSCALE_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectNormalMapScale( param0, param1, param2 );
    break;
}
case AGKI_SETOBJECTMESHNORMALMAPSCALE_0_L_L_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshNormalMapScale( param0, param1, param2, param3 );
    break;
}
case AGKI_STARTSCREENRECORDING_0_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::StartScreenRecording( param0, param1 );
    break;
}
case AGKI_STOPSCREENRECORDING_0_0:
{
    agk::StopScreenRecording( );
    break;
}
case AGKI_ISSCREENRECORDING_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::IsScreenRecording( );
    break;
}
case AGKI_DELETESHADER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteShader( param0 );
    break;
}
case AGKI_ACTIVATESMARTWATCH_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::ActivateSmartWatch( param0 );
    break;
}
case AGKI_GETSMARTWATCHSTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetSmartWatchState( );
    break;
}
case AGKI_SENDSMARTWATCHDATA_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SendSmartWatchData( param0 );
    break;
}
case AGKI_RECEIVESMARTWATCHDATA_S_0:
{
    char *szReturnStr = agk::ReceiveSmartWatchData( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_TEXTTOSPEECHSETUP_0_0:
{
    agk::TextToSpeechSetup( );
    break;
}
case AGKI_GETTEXTTOSPEECHREADY_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextToSpeechReady( );
    break;
}
case AGKI_SPEAK_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::Speak( param0 );
    break;
}
case AGKI_SETSPEECHRATE_0_F:
{
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetSpeechRate( param0 );
    break;
}
case AGKI_SETSPEECHLANGUAGE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetSpeechLanguage( param0 );
    break;
}
case AGKI_ISSPEAKING_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::IsSpeaking( );
    break;
}
case AGKI_STOPSPEAKING_0_0:
{
    agk::StopSpeaking( );
    break;
}
case AGKI_GETSPRITETRANSPARENCY_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteTransparency( param0 );
    break;
}
case AGKI_SETSPRITESHAPE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteShape( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITESHAPEBOX_0_L_F_F_F_F_F_L:
{
    int param6 = m_pStack[ --m_iStackPtr ].i;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteShapeBox( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_SETSPRITESHAPECIRCLE_0_L_F_F_F_L:
{
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteShapeCircle( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETSPRITESHAPEPOLYGON_0_L_L_L_F_F_L:
{
    int param5 = m_pStack[ --m_iStackPtr ].i;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteShapePolygon( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETSPRITESHAPECHAIN_0_L_L_L_L_F_F_L:
{
    int param6 = m_pStack[ --m_iStackPtr ].i;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteShapeChain( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_ADDSPRITESHAPECHAIN_0_L_L_L_L_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddSpriteShapeChain( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_GETSPRITENUMSHAPES_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteNumShapes( param0 );
    break;
}
case AGKI_GETSPRITESHAPENUMVERTICES_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteShapeNumVertices( param0, param1 );
    break;
}
case AGKI_GETSPRITESHAPEVERTEXX_F_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteShapeVertexX( param0, param1, param2 );
    break;
}
case AGKI_GETSPRITESHAPEVERTEXY_F_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpriteShapeVertexY( param0, param1, param2 );
    break;
}
case AGKI_GETSPRITEPHYSICSCOMX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpritePhysicsCOMX( param0 );
    break;
}
case AGKI_GETSPRITEPHYSICSCOMY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetSpritePhysicsCOMY( param0 );
    break;
}
case AGKI_SETSPRITEPHYSICSFRICTION_0_L_F_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsFriction( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEPHYSICSRESTITUTION_0_L_F_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsRestitution( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEPHYSICSDENSITY_0_L_F_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsDensity( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEPHYSICSISSENSOR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpritePhysicsIsSensor( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITEGROUP_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteGroup( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITECATEGORYBITS_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteCategoryBits( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITECATEGORYBIT_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteCategoryBit( param0, param1, param2, param3 );
    break;
}
case AGKI_SETSPRITECOLLIDEBITS_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteCollideBits( param0, param1, param2 );
    break;
}
case AGKI_SETSPRITECOLLIDEBIT_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetSpriteCollideBit( param0, param1, param2, param3 );
    break;
}
case AGKI_CREATEROPEJOINT_0_L_L_L_F_F_F_F_F_L:
{
    int param8 = m_pStack[ --m_iStackPtr ].i;
    float param7 = m_pStack[ --m_iStackPtr ].f;
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateRopeJoint( param0, param1, param2, param3, param4, param5, param6, param7, param8 );
    break;
}
case AGKI_CREATEROPEJOINT_L_L_L_F_F_F_F_F_L:
{
    int param7 = m_pStack[ --m_iStackPtr ].i;
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateRopeJoint( param0, param1, param2, param3, param4, param5, param6, param7 );
    break;
}
case AGKI_SETJOINTMOUSEMAXFORCE_0_L_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetJointMouseMaxForce( param0, param1 );
    break;
}
case AGKI_SETJOINTDAMPING_0_L_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetJointDamping( param0, param1, param2 );
    break;
}
case AGKI_PAUSETWEENCUSTOM_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PauseTweenCustom( param0 );
    break;
}
case AGKI_RESUMETWEENCUSTOM_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResumeTweenCustom( param0 );
    break;
}
case AGKI_PAUSETWEENSPRITE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PauseTweenSprite( param0, param1 );
    break;
}
case AGKI_RESUMETWEENSPRITE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResumeTweenSprite( param0, param1 );
    break;
}
case AGKI_PAUSETWEENTEXT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PauseTweenText( param0, param1 );
    break;
}
case AGKI_RESUMETWEENTEXT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResumeTweenText( param0, param1 );
    break;
}
case AGKI_PAUSETWEENCHAR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PauseTweenChar( param0, param1, param2 );
    break;
}
case AGKI_RESUMETWEENCHAR_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResumeTweenChar( param0, param1, param2 );
    break;
}
case AGKI_PAUSETWEENOBJECT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PauseTweenObject( param0, param1 );
    break;
}
case AGKI_RESUMETWEENOBJECT_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResumeTweenObject( param0, param1 );
    break;
}
case AGKI_PAUSETWEENCAMERA_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::PauseTweenCamera( param0, param1 );
    break;
}
case AGKI_RESUMETWEENCAMERA_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ResumeTweenCamera( param0, param1 );
    break;
}
case AGKI_GETNETWORKEXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkExists( param0 );
    break;
}
case AGKI_SETOBJECTBLENDMODES_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectBlendModes( param0, param1, param2 );
    break;
}
case AGKI_GETSHADEREXISTS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetShaderExists( param0 );
    break;
}
case AGKI_GETTEXTALIGNMENT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetTextAlignment( param0 );
    break;
}
case AGKI_SETUPCLOUDDATA_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetupCloudData( param0 );
    break;
}
case AGKI_GETCLOUDDATAALLOWED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetCloudDataAllowed( );
    break;
}
case AGKI_GETCLOUDDATACHANGED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetCloudDataChanged( );
    break;
}
case AGKI_GETCLOUDDATAVARIABLE_S_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    char *szReturnStr = agk::GetCloudDataVariable( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETCLOUDDATAVARIABLE_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetCloudDataVariable( param0, param1 );
    break;
}
case AGKI_DELETECLOUDDATAVARIABLE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::DeleteCloudDataVariable( param0 );
    break;
}
case AGKI_GETDEVICENETWORKTYPE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetDeviceNetworkType( );
    break;
}
case AGKI_GETSTORAGEREMAINING_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::GetStorageRemaining( param0 );
    break;
}
case AGKI_GETSTORAGETOTAL_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::GetStorageTotal( param0 );
    break;
}
case AGKI_CHECKPERMISSION_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CheckPermission( param0 );
    break;
}
case AGKI_REQUESTPERMISSION_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::RequestPermission( param0 );
    break;
}
case AGKI_DRAWTEXT_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DrawText( param0 );
    break;
}
case AGKI_SETSHADOWCASCADEVALUES_0_F_F_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetShadowCascadeValues( param0, param1, param2 );
    break;
}
case AGKI_ARSETUP_0_0:
{
    agk::ARSetup( );
    break;
}
case AGKI_ARGETSTATUS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::ARGetStatus( );
    break;
}
case AGKI_ARDESTROY_0_0:
{
    agk::ARDestroy( );
    break;
}
case AGKI_ARCONTROLCAMERA_0_0:
{
    agk::ARControlCamera( );
    break;
}
case AGKI_ARDRAWBACKGROUND_0_0:
{
    agk::ARDrawBackground( );
    break;
}
case AGKI_ARSETPLANEDETECTIONMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ARSetPlaneDetectionMode( param0 );
    break;
}
case AGKI_ARSETLIGHTESTIMATIONMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ARSetLightEstimationMode( param0 );
    break;
}
case AGKI_ARGETLIGHTESTIMATE_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetLightEstimate( );
    break;
}
case AGKI_ARHITTEST_L_F_F:
{
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    m_pStack[ m_iStackPtr++ ].i = agk::ARHitTest( param0, param1 );
    break;
}
case AGKI_ARGETHITTESTX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetHitTestX( param0 );
    break;
}
case AGKI_ARGETHITTESTY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetHitTestY( param0 );
    break;
}
case AGKI_ARGETHITTESTZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetHitTestZ( param0 );
    break;
}
case AGKI_ARGETHITTESTNORMALX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetHitTestNormalX( param0 );
    break;
}
case AGKI_ARGETHITTESTNORMALY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetHitTestNormalY( param0 );
    break;
}
case AGKI_ARGETHITTESTNORMALZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetHitTestNormalZ( param0 );
    break;
}
case AGKI_ARGETHITTESTTYPE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ARGetHitTestType( param0 );
    break;
}
case AGKI_ARHITTESTFINISH_0_0:
{
    agk::ARHitTestFinish( );
    break;
}
case AGKI_ARGETPLANES_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ARGetPlanes( param0 );
    break;
}
case AGKI_ARGETPLANEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetPlaneX( param0 );
    break;
}
case AGKI_ARGETPLANEY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetPlaneY( param0 );
    break;
}
case AGKI_ARGETPLANEZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetPlaneZ( param0 );
    break;
}
case AGKI_ARGETPLANEANGLEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetPlaneAngleX( param0 );
    break;
}
case AGKI_ARGETPLANEANGLEY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetPlaneAngleY( param0 );
    break;
}
case AGKI_ARGETPLANEANGLEZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetPlaneAngleZ( param0 );
    break;
}
case AGKI_ARGETPLANESIZEX_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetPlaneSizeX( param0 );
    break;
}
case AGKI_ARGETPLANESIZEZ_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::ARGetPlaneSizeZ( param0 );
    break;
}
case AGKI_ARGETPLANESFINISH_0_0:
{
    agk::ARGetPlanesFinish( );
    break;
}
case AGKI_ARCREATEANCHORFROMHITTEST_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ARCreateAnchorFromHitTest( param0 );
    break;
}
case AGKI_ARCREATEANCHORFROMPLANE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ARCreateAnchorFromPlane( param0 );
    break;
}
case AGKI_ARFIXOBJECTTOANCHOR_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ARFixObjectToAnchor( param0, param1 );
    break;
}
case AGKI_ARGETANCHORSTATUS_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::ARGetAnchorStatus( param0 );
    break;
}
case AGKI_ARDELETEANCHOR_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::ARDeleteAnchor( param0 );
    break;
}
case AGKI_DELETEOBJECTTREE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteObjectTree( param0 );
    break;
}
case AGKI_FIXSPRITETOSKELETON2D_0_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::FixSpriteToSkeleton2D( param0, param1, param2, param3 );
    break;
}
case AGKI_SETRAWTOUCHMOVESENSITIVITY_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetRawTouchMoveSensitivity( param0 );
    break;
}
case AGKI_MOVE3DPHYSICSCHARACTERCONTROLLER_0_L_F_F_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Move3DPhysicsCharacterController( param0, param1, param2, param3 );
    break;
}
case AGKI_SPEAK_0_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::Speak( param0, param1 );
    break;
}
case AGKI_OVERRIDECONSENTADMOB_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::OverrideConsentAdMob( param0 );
    break;
}
case AGKI_LOADCONSENTSTATUSADMOB_0_S_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::LoadConsentStatusAdMob( param0, param1 );
    break;
}
case AGKI_GETCONSENTSTATUSADMOB_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetConsentStatusAdMob( );
    break;
}
case AGKI_REQUESTCONSENTADMOB_0_0:
{
    agk::RequestConsentAdMob( );
    break;
}
case AGKI_GETSUPPORTEDSHADERVARYINGS_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetSupportedShaderVaryings( );
    break;
}
case AGKI_GETSPEECHNUMVOICES_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpeechNumVoices( );
    break;
}
case AGKI_GETSPEECHVOICELANGUAGE_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetSpeechVoiceLanguage( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETSPEECHVOICENAME_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetSpeechVoiceName( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_OVERRIDECONSENTCHARTBOOST_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::OverrideConsentChartboost( param0 );
    break;
}
case AGKI_ADDHTTPHEADER_0_L_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::AddHTTPHeader( param0, param1, param2 );
    break;
}
case AGKI_REMOVEHTTPHEADER_0_L_S:
{
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::RemoveHTTPHeader( param0, param1 );
    break;
}
case AGKI_GETHTTPSTATUSCODE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetHTTPStatusCode( param0 );
    break;
}
case AGKI_GETSPEECHVOICEID_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetSpeechVoiceID( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETSPEECHLANGUAGEBYID_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetSpeechLanguageByID( param0 );
    break;
}
case AGKI_GETSPRITEFLIPPEDH_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteFlippedH( param0 );
    break;
}
case AGKI_GETSPRITEINSCREEN_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteInScreen( param0 );
    break;
}
case AGKI_GETSPRITEFLIPPEDV_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSpriteFlippedV( param0 );
    break;
}
case AGKI_DRAWPARTICLES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DrawParticles( param0 );
    break;
}
case AGKI_DRAW3DPARTICLES_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::Draw3DParticles( param0 );
    break;
}
case AGKI_SETCLIPBOARDTEXT_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::SetClipboardText( param0 );
    break;
}
case AGKI_GETCLIPBOARDTEXT_S_0:
{
    char *szReturnStr = agk::GetClipboardText( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETEDITBOXINPUTTYPE_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetEditBoxInputType( param0, param1 );
    break;
}
case AGKI_GETEXPANSIONFILEERROR_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetExpansionFileError( );
    break;
}
case AGKI_GETSKELETON2DDEPTH_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetSkeleton2DDepth( param0 );
    break;
}
case AGKI_GETEDITBOXDEPTH_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetEditBoxDepth( param0 );
    break;
}
case AGKI_GETCHARBUFFER_S_0:
{
    char *szReturnStr = agk::GetCharBuffer( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETCHARBUFFERLENGTH_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetCharBufferLength( );
    break;
}
case AGKI_GETRAWJOYSTICKSLIDER_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawJoystickSlider( param0, param1 );
    break;
}
case AGKI_GETRAWJOYSTICKPOV_L_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawJoystickPOV( param0, param1 );
    break;
}
case AGKI_COPYNETWORKMESSAGE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CopyNetworkMessage( param0 );
    break;
}
case AGKI_GETURLSCHEMETEXT_S_0:
{
    char *szReturnStr = agk::GetURLSchemeText( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_SETOBJECTALPHA_0_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectAlpha( param0, param1 );
    break;
}
case AGKI_GETOBJECTCOLORRED_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectColorRed( param0 );
    break;
}
case AGKI_GETOBJECTCOLORGREEN_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectColorGreen( param0 );
    break;
}
case AGKI_GETOBJECTCOLORBLUE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectColorBlue( param0 );
    break;
}
case AGKI_GETOBJECTALPHA_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectAlpha( param0 );
    break;
}
case AGKI_CLEARURLSCHEMETEXT_0_0:
{
    agk::ClearURLSchemeText( );
    break;
}
case AGKI_EXTRACTZIP_0_S_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::ExtractZip( param0, param1, param2 );
    break;
}
case AGKI_CREATEOBJECTFROMRAWHEIGHTMAP_L_S_F_F_F_L_L_L_L:
{
    int param7 = m_pStack[ --m_iStackPtr ].i;
    int param6 = m_pStack[ --m_iStackPtr ].i;
    int param5 = m_pStack[ --m_iStackPtr ].i;
    int param4 = m_pStack[ --m_iStackPtr ].i;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CreateObjectFromRawHeightMap( param0, param1, param2, param3, param4, param5, param6, param7 );
    break;
}
case AGKI_CREATEOBJECTFROMRAWHEIGHTMAP_0_L_S_F_F_F_L_L_L_L:
{
    int param8 = m_pStack[ --m_iStackPtr ].i;
    int param7 = m_pStack[ --m_iStackPtr ].i;
    int param6 = m_pStack[ --m_iStackPtr ].i;
    int param5 = m_pStack[ --m_iStackPtr ].i;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::CreateObjectFromRawHeightMap( param0, param1, param2, param3, param4, param5, param6, param7, param8 );
    break;
}
case AGKI_SETOBJECTMESHCOLLISIONMODE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshCollisionMode( param0, param1, param2 );
    break;
}
case AGKI_SETOBJECTMESHVISIBLE_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshVisible( param0, param1, param2 );
    break;
}
case AGKI_GETOBJECTTEXTURENAME_S_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetObjectTextureName( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETOBJECTNUMTEXTURES_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetObjectNumTextures( param0 );
    break;
}
case AGKI_GETRAWJOYSTICKNAME_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetRawJoystickName( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_PLAYYOUTUBEVIDEO_0_S_S_F:
{
    float param2 = m_pStack[ --m_iStackPtr ].f;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::PlayYoutubeVideo( param0, param1, param2 );
    break;
}
case AGKI_SHAREFILE_0_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::ShareFile( param0 );
    break;
}
case AGKI_GETNETWORKMESSAGEFROMPORT_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetNetworkMessageFromPort( param0 );
    break;
}
case AGKI_CREATEUDPLISTENER_L_S_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::CreateUDPListener( param0, param1 );
    break;
}
case AGKI_CREATEUDPLISTENER_L_L_S_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::CreateUDPListener( param0, param1, param2 );
    break;
}
case AGKI_SENDUDPNETWORKMESSAGE_0_L_L_S_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SendUDPNetworkMessage( param0, param1, param2, param3 );
    break;
}
case AGKI_GETUDPNETWORKMESSAGE_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetUDPNetworkMessage( param0 );
    break;
}
case AGKI_DELETEUDPLISTENER_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::DeleteUDPListener( param0 );
    break;
}
case AGKI_EXTRACTZIPASYNC_0_S_S_S:
{
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::ExtractZipASync( param0, param1, param2 );
    break;
}
case AGKI_GETZIPEXTRACTPROGRESS_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetZipExtractProgress( );
    break;
}
case AGKI_GETZIPEXTRACTCOMPLETE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetZipExtractComplete( );
    break;
}
case AGKI_GETWINDOWWIDTH_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetWindowWidth( );
    break;
}
case AGKI_GETWINDOWHEIGHT_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetWindowHeight( );
    break;
}
case AGKI_CANCELZIPEXTRACT_0_0:
{
    agk::CancelZipExtract( );
    break;
}
case AGKI_GAMECENTERLOGOUT_0_0:
{
    agk::GameCenterLogout( );
    break;
}
case AGKI_GETIMAGESIZEFROMFILE_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::GetImageSizeFromFile( param0 );
    break;
}
case AGKI_SETOBJECTMESHCASTSHADOW_0_L_L_L:
{
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectMeshCastShadow( param0, param1, param2 );
    break;
}
case AGKI_SETOBJECTSHADERCONSTANTARRAYFLOATBYNAME_0_L_S_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShaderConstantArrayFloatByName( param0, param1, param2, param3 );
    break;
}
case AGKI_SETOBJECTSHADERCONSTANTARRAYVEC2BYNAME_0_L_S_L_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShaderConstantArrayVec2ByName( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETOBJECTSHADERCONSTANTARRAYVEC3BYNAME_0_L_S_L_F_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShaderConstantArrayVec3ByName( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETOBJECTSHADERCONSTANTARRAYVEC4BYNAME_0_L_S_L_F_F_F_F:
{
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetObjectShaderConstantArrayVec4ByName( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_SETSHADERERRORMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetShaderErrorMode( param0 );
    break;
}
case AGKI_SETSHADERCONSTANTARRAYFLOATBYNAME_0_L_S_L_F:
{
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetShaderConstantArrayFloatByName( param0, param1, param2, param3 );
    break;
}
case AGKI_SETSHADERCONSTANTARRAYVEC2BYNAME_0_L_S_L_F_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetShaderConstantArrayVec2ByName( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SETSHADERCONSTANTARRAYVEC3BYNAME_0_L_S_L_F_F_F:
{
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetShaderConstantArrayVec3ByName( param0, param1, param2, param3, param4, param5 );
    break;
}
case AGKI_SETSHADERCONSTANTARRAYVEC4BYNAME_0_L_S_L_F_F_F_F:
{
    float param6 = m_pStack[ --m_iStackPtr ].f;
    float param5 = m_pStack[ --m_iStackPtr ].f;
    float param4 = m_pStack[ --m_iStackPtr ].f;
    float param3 = m_pStack[ --m_iStackPtr ].f;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetShaderConstantArrayVec4ByName( param0, param1, param2, param3, param4, param5, param6 );
    break;
}
case AGKI_GETOBJECT3DPHYSICSMAXLINEARVELOCITY_F_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].f = agk::GetObject3DPhysicsMaxLinearVelocity( param0 );
    break;
}
case AGKI_ISINVERTEDDEPTH_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::IsInvertedDepth( );
    break;
}
case AGKI_GETCLIPSPACEMODE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetClipSpaceMode( );
    break;
}
case AGKI_ISTOPLEFTORIGIN_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::IsTopLeftOrigin( );
    break;
}
case AGKI_GETDRAWING3DSETUPTIME_F_0:
{
    m_pStack[ m_iStackPtr++ ].f = agk::GetDrawing3DSetupTime( );
    break;
}
case AGKI_MAKECOLOR_L_L_L_L_L:
{
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::MakeColor( param0, param1, param2, param3 );
    break;
}
case AGKI_GETCOLORALPHA_L_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    m_pStack[ m_iStackPtr++ ].i = agk::GetColorAlpha( param0 );
    break;
}
case AGKI_SETPRESENTMODE_0_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetPresentMode( param0 );
    break;
}
case AGKI_FORCEPRESENT_0_0:
{
    agk::ForcePresent( );
    break;
}
case AGKI_GETAPPINSTALLED_L_S:
{
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    m_pStack[ m_iStackPtr++ ].i = agk::GetAppInstalled( param0 );
    break;
}
case AGKI_SETSNAPCHATSTICKERSETTINGS_0_F_F_L_L_F:
{
    float param4 = m_pStack[ --m_iStackPtr ].f;
    int param3 = m_pStack[ --m_iStackPtr ].i;
    int param2 = m_pStack[ --m_iStackPtr ].i;
    float param1 = m_pStack[ --m_iStackPtr ].f;
    float param0 = m_pStack[ --m_iStackPtr ].f;
    agk::SetSnapChatStickerSettings( param0, param1, param2, param3, param4 );
    break;
}
case AGKI_SHARESNAPCHATIMAGE_0_S_S_S_S:
{
    const char* param3 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param1 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param0 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    agk::ShareSnapChatImage( param0, param1, param2, param3 );
    break;
}
case AGKI_SETLOCALNOTIFICATION_0_L_L_S_S:
{
    const char* param3 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    const char* param2 = m_pStrStack[ --m_iStrStackPtr ].GetStr();
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    agk::SetLocalNotification( param0, param1, param2, param3 );
    break;
}
case AGKI_GETRENDERERNAME_S_0:
{
    char *szReturnStr = agk::GetRendererName( );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETNETWORKCLIENTIP_S_L_L:
{
    int param1 = m_pStack[ --m_iStackPtr ].i;
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetNetworkClientIP( param0, param1 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETNETWORKSERVERIP_S_L:
{
    int param0 = m_pStack[ --m_iStackPtr ].i;
    char *szReturnStr = agk::GetNetworkServerIP( param0 );
    m_pStrStack[ m_iStrStackPtr++ ].SetStrUTF8( szReturnStr );
    delete [] szReturnStr;
    break;
}
case AGKI_GETRAWMOUSEFOURTHPRESSED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseFourthPressed( );
    break;
}
case AGKI_GETRAWMOUSEFOURTHSTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseFourthState( );
    break;
}
case AGKI_GETRAWMOUSEFOURTHRELEASED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseFourthReleased( );
    break;
}
case AGKI_GETRAWMOUSEFIFTHPRESSED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseFifthPressed( );
    break;
}
case AGKI_GETRAWMOUSEFIFTHSTATE_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseFifthState( );
    break;
}
case AGKI_GETRAWMOUSEFIFTHRELEASED_L_0:
{
    m_pStack[ m_iStackPtr++ ].i = agk::GetRawMouseFifthReleased( );
    break;
}


