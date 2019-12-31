package com.thegamecreators.agk_player;

import android.Manifest;
import android.app.Activity;
import android.app.NativeActivity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.hardware.display.DisplayManager;
import android.media.MediaRecorder;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore;
import androidx.core.content.ContextCompat;
import android.util.DisplayMetrics;
import android.util.Log;
import android.widget.Toast;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInStatusCodes;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.tasks.Task;
import com.google.android.youtube.player.YouTubeInitializationResult;
import com.google.android.youtube.player.YouTubeStandalonePlayer;

import java.io.FileOutputStream;
import java.io.IOException;

public class AGKActivity extends NativeActivity
{
    @Override
    public void onCreate( Bundle state )
    {
        super.onCreate( state );

        Intent intent = getIntent();
        if ( intent != null )
        {
            Uri data = intent.getData();
            if ( data != null ) AGKHelper.g_sLastURI = data.toString();
        }
    }

    @Override
    public void onNewIntent( Intent intent )
    {
        if ( intent == null ) return;
        if ( intent.getData() == null ) return;
        AGKHelper.g_sLastURI = intent.getData().toString();
    }

    @Override
    public void onActivityResult( int requestCode, int resultCode, Intent data )
    {
        switch( requestCode ) {
            case 9000:
            case 9002: { // InAppBilling
                if (!AGKHelper.mHelper.handleActivityResult(requestCode, resultCode, data)) {
                    Log.e("IAP", "Failed to handle activity result " + resultCode);
                } else {
                    Log.i("IAP", "Result code handled by IABUtil: " + resultCode);
                }
                break;
            }
            case 10002: { // Screen recording
                if (resultCode != Activity.RESULT_OK) {
                    Log.i("MediaProjection", "User cancelled");
                    return;
                }
                if (Build.VERSION.SDK_INT >= 21) {
                    Log.i("MediaProjection", "Starting screen capture");

                    int width = AGKHelper.g_pAct.getWindow().getDecorView().getWidth();
                    int height = AGKHelper.g_pAct.getWindow().getDecorView().getHeight();
                    if (width > height) {
                        if (width > 1280) width = 1280;
                        if (height > 720) height = 720;
                    } else {
                        if (width > 720) width = 720;
                        if (height > 1280) height = 1280;
                    }
                    int audioSource = 0;
                    if (AGKHelper.g_iScreenRecordMic == 1) {
                        if (ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED) {
                            audioSource = MediaRecorder.AudioSource.MIC;
                        } else {
                            Log.w("Screen Recording", "The app does not have the RECORD_AUDIO permission, video will have no audio");
                        }
                    }

                    AGKHelper.mMediaRecorder = new MediaRecorder();
                    if (audioSource > 0) AGKHelper.mMediaRecorder.setAudioSource(audioSource);
                    AGKHelper.mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.SURFACE);
                    AGKHelper.mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);
                    AGKHelper.mMediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);
                    if (audioSource > 0) {
                        AGKHelper.mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AAC);
                        AGKHelper.mMediaRecorder.setAudioEncodingBitRate(96000);
                        AGKHelper.mMediaRecorder.setAudioSamplingRate(44100);
                    }
                    AGKHelper.mMediaRecorder.setVideoEncodingBitRate(2048 * 1000);
                    AGKHelper.mMediaRecorder.setVideoFrameRate(30);
                    AGKHelper.mMediaRecorder.setVideoSize(width, height);
                    AGKHelper.mMediaRecorder.setOutputFile(AGKHelper.g_sScreenRecordFile);
                    try {
                        AGKHelper.mMediaRecorder.prepare();
                    } catch (Exception e) {
                        e.printStackTrace();
                        AGKHelper.mMediaRecorder.release();
                        AGKHelper.mMediaRecorder = null;
                        return;
                    }

                    Log.i("Screen Recording", "Recording to: " + AGKHelper.g_sScreenRecordFile);

                    DisplayMetrics metrics = AGKHelper.g_pAct.getResources().getDisplayMetrics();
                    AGKHelper.mMediaProjection = AGKHelper.mMediaProjectionManager.getMediaProjection(resultCode, data);
                    Log.i("MediaProjection", "Setting up a VirtualDisplay: " + width + "x" + height + " (" + metrics.densityDpi + ")");
                    AGKHelper.mVirtualDisplay = AGKHelper.mMediaProjection.createVirtualDisplay("ScreenCapture",
                            width, height, metrics.densityDpi,
                            DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
                            AGKHelper.mMediaRecorder.getSurface(), null, null);
                    AGKHelper.mMediaRecorder.start();
                }
                break;
            }
            case 10003: // Google Drive sign in
            {
                Task<GoogleSignInAccount> task = GoogleSignIn.getSignedInAccountFromIntent(data);
                try {
                    AGKHelper.g_GoogleAccount = task.getResult(ApiException.class);
                    AGKHelper.FinishCloudDataSetup(task);
                } catch (ApiException e) {
                    Log.e("Google Sign In", "Failed to sign in, error code: " + e.getStatusCode());
                    if (e.getStatusCode() == GoogleSignInStatusCodes.SIGN_IN_CANCELLED)
                        AGKHelper.g_iCloudDataStatus = -1;
                    else if (e.getStatusCode() != GoogleSignInStatusCodes.SIGN_IN_CURRENTLY_IN_PROGRESS)
                        AGKHelper.g_iCloudDataStatus = -2;
                }
                break;
            }
            case 10004: // Google Games sign in
            {
                Task<GoogleSignInAccount> task = GoogleSignIn.getSignedInAccountFromIntent(data);
                try {
                    AGKHelper.g_GamesAccount = task.getResult(ApiException.class);
                    AGKHelper.GameCenterCompleteLogin( this );
                } catch (ApiException e) {
                    Log.e("Games Sign In", "Failed to sign in, error code: " + e.getStatusCode());
                    if (e.getStatusCode() == GoogleSignInStatusCodes.SIGN_IN_CANCELLED)
                        AGKHelper.m_GameCenterLoggedIn = -1;
                    else if (e.getStatusCode() != GoogleSignInStatusCodes.SIGN_IN_CURRENTLY_IN_PROGRESS)
                        AGKHelper.m_GameCenterLoggedIn = -1;
                    AGKHelper.g_GamesAccount = null;
                    AGKHelper.g_GamesSignIn = null;
                }
                break;
            }
            case 9003: // youtube start video playback
            {
                if ( resultCode != RESULT_OK ) {
                    YouTubeInitializationResult errorReason = YouTubeStandalonePlayer.getReturnedInitializationResult(data);
                    if (errorReason.isUserRecoverableError()) {
                        errorReason.getErrorDialog(this, 0).show();
                    } else {
                        String errorMessage = errorReason.toString();
                        Toast.makeText(this, errorMessage, Toast.LENGTH_LONG).show();
                    }
                }
                break;
            }
            case 9004: // capture camera image
            {
                if ( resultCode == RESULT_OK )
                {
                    if (data != null && data.getExtras() != null) {
                        Bitmap imageBitmap = (Bitmap) data.getExtras().get("data");
                        try {
                            FileOutputStream out = new FileOutputStream(AGKHelper.sCameraSavePath);
                            imageBitmap.compress(Bitmap.CompressFormat.JPEG, 95, out);
                            Log.w("Camera Image", "Saved image to: " + AGKHelper.sCameraSavePath);
                            AGKHelper.iCapturingImage = 2;
                            return;
                        }
                        catch( IOException e )
                        {
                            Log.e("Camera Image", "Failed to save image: "+e.toString() );
                        }
                    }
                    AGKHelper.iCapturingImage = 0;
                }
                else
                {
                    Log.e("Camera Image", "User cancelled capture image" );
                    AGKHelper.iCapturingImage = 0;
                }
                break;
            }
            case 9005: // choose image
            {
                if ( resultCode == RESULT_OK )
                {
                    if (data != null) {
                        Uri uri = data.getData();
                        try {
                            Bitmap imageBitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), uri);
                            FileOutputStream out = new FileOutputStream(AGKHelper.sChosenImagePath);
                            imageBitmap.compress(Bitmap.CompressFormat.JPEG, 95, out);
                            Log.w("Choose Image", "Saved image to: " + AGKHelper.sChosenImagePath);
                            AGKHelper.iChoosingImage = 2;
                            return;
                        }
                        catch( IOException e )
                        {
                            Log.e("Choose Image", "Failed to save image: "+e.toString() );
                        }
                    }
                    AGKHelper.iChoosingImage = 0;
                }
                else
                {
                    Log.e("Choose Image", "User cancelled choose image" );
                    AGKHelper.iChoosingImage = 0;
                }
                break;
            }
        }
    }

    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults)
    {
        int index = requestCode-5;
        if ( index < 0 ) return;
        if ( index >= AGKHelper.g_sPermissions.length ) return;

        AGKHelper.g_iPermissionStatus[index] = 2;
    }
}