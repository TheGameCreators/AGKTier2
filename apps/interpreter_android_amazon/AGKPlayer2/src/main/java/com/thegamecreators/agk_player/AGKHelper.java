// Temporary until the NDK build system can deal with there being no Java source.
package com.thegamecreators.agk_player;

import com.amazon.device.iap.PurchasingService;
import com.facebook.*;
import com.facebook.android.DialogError;
import com.facebook.android.Facebook;
import com.facebook.android.FacebookError;
import com.facebook.android.Facebook.DialogListener;

import com.google.ads.mediation.admob.AdMobAdapter;
import com.google.android.gms.ads.*;

import com.amazon.device.ads.*;

import android.Manifest;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.ActivityNotFoundException;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Intent;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.Runnable;
import java.lang.String;
import java.net.Inet4Address;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.MalformedURLException;
import java.net.NetworkInterface;
import java.net.URL;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Date;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.UUID;

import android.content.SharedPreferences;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.display.VirtualDisplay;
import android.media.MediaRecorder;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.os.Build;
import android.os.Environment;
import android.os.StatFs;
import android.os.Vibrator;
import android.speech.tts.TextToSpeech;
import android.speech.tts.Voice;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.FileProvider;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.text.format.Formatter;
import android.util.DisplayMetrics;
import android.util.Log;
import android.provider.MediaStore;
import android.provider.Settings.Secure;
import android.os.Bundle;
import android.os.Looper;

import android.view.Surface;
import android.webkit.MimeTypeMap;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.view.Gravity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.media.MediaPlayer;
import android.media.MediaScannerConnection;
import android.content.DialogInterface;

import com.chartboost.sdk.*;
import com.chartboost.sdk.Model.CBError.CBImpressionError;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.reward.RewardItem;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.google.android.gms.ads.reward.RewardedVideoAdListener;
import com.thegamecreators.agk_player.iap.AGKPurchasingListener;
import com.thegamecreators.agk_player2.R;

import com.google.ads.consent.*;

import android.media.MediaMetadataRetriever;

import static android.content.ClipDescription.MIMETYPE_TEXT_PLAIN;

// Used for agk::Message()
class RunnableMessage implements Runnable
{
	public Activity act;
	public String msg;
	
	public void run() {
        //Toast.makeText(act, msg, Toast.LENGTH_LONG).show();
		AlertDialog alertDialog;
		alertDialog = new AlertDialog.Builder(act).create();
		alertDialog.setTitle("Message");
		alertDialog.setMessage(msg);
		alertDialog.setButton( DialogInterface.BUTTON_POSITIVE, "OK", new DialogInterface.OnClickListener(){public void onClick(DialogInterface dialog, int which) {}});
		alertDialog.show();
    }
}

class MyTextWatcher implements TextWatcher
{
	public static MyTextWatcher m_TextWatcher = null;
	
	@Override
	public void afterTextChanged( Editable s )
	{
		//Log.e("EditText", AGKHelper.mTextInput.getText().toString());
	}

	@Override
	public void beforeTextChanged(CharSequence arg0, int arg1, int arg2, int arg3) {}

	@Override
	public void onTextChanged(CharSequence arg0, int arg1, int arg2, int arg3) {}
}

class MyTextActionWatcher implements TextView.OnEditorActionListener
{
	public static MyTextActionWatcher m_TextActionWatcher = null;
	public static Activity act = null;

	@Override
	public boolean onEditorAction(TextView arg0, int arg1, KeyEvent arg2) {
		//String out = String.format("Action: %d, Event: %s", arg1, arg2 == null ? "" : arg2.toString());
		//Log.e("TextView",out);
		AGKHelper.mTextFinished = true;
		return true;
	}
}

class RunnableKeyboard implements Runnable
{
	public Activity act;
	public int action = 0;
	public String text = "";
	public int multiline = 0;
	public int inputType = 0; //0=normal, 1=numbers
	public int cursorpos = 0;
	
	public void run() {
		switch( action )
		{
			case 1: // start text input
			{
				AGKHelper.mTextInput = new EditText(act);
				AGKHelper.mTextInput.setSingleLine(multiline == 0);
				if ( inputType==1 ) AGKHelper.mTextInput.setInputType( InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL | InputType.TYPE_NUMBER_FLAG_SIGNED );
				else AGKHelper.mTextInput.setInputType( InputType.TYPE_CLASS_TEXT );
				if ( MyTextWatcher.m_TextWatcher == null ) MyTextWatcher.m_TextWatcher = new MyTextWatcher();
				if ( MyTextActionWatcher.m_TextActionWatcher == null ) MyTextActionWatcher.m_TextActionWatcher = new MyTextActionWatcher();
				MyTextActionWatcher.act = act;
				
				FrameLayout.LayoutParams mEditTextLayoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
				mEditTextLayoutParams.gravity = Gravity.TOP;
				mEditTextLayoutParams.setMargins(0, 0, 0, 0);
				AGKHelper.mTextInput.setLayoutParams(mEditTextLayoutParams);
				AGKHelper.mTextInput.setVisibility(View.VISIBLE);
				AGKHelper.mTextInput.addTextChangedListener(MyTextWatcher.m_TextWatcher);
				AGKHelper.mTextInput.setOnEditorActionListener(MyTextActionWatcher.m_TextActionWatcher);
				
				act.addContentView(AGKHelper.mTextInput, mEditTextLayoutParams);
					
				AGKHelper.mTextInput.bringToFront();
				if ( cursorpos >= 0 ) AGKHelper.mTextInput.setSelection(cursorpos);
				AGKHelper.mTextInput.requestFocus();
				
				AGKHelper.mTextFinished = false;
				
				InputMethodManager m = (InputMethodManager) act.getSystemService(Context.INPUT_METHOD_SERVICE);
				m.showSoftInput(AGKHelper.mTextInput, 0);
				break;
			}
			case 2: // stop text input
			{
				InputMethodManager lInputMethodManager = (InputMethodManager)act.getSystemService(Context.INPUT_METHOD_SERVICE);
				lInputMethodManager.hideSoftInputFromWindow( act.getWindow().getDecorView().getWindowToken(), 0 );

				if ( AGKHelper.mTextInput != null ) {
					((ViewGroup) (AGKHelper.mTextInput.getParent())).removeView(AGKHelper.mTextInput);
					AGKHelper.mTextInput = null;
				}
				AGKHelper.mTextHiding = false;
				break;
			}
			case 3: // position text cursor
			{
				if ( AGKHelper.mTextInput != null ) 
				{
					AGKHelper.mTextInput.setText(text);
					if ( cursorpos >= 0 ) AGKHelper.mTextInput.setSelection(cursorpos);
				}
				break;
			}
			case 4: // show keyboard for existing text input
			{
				if ( AGKHelper.mTextInput != null ) 
				{
					AGKHelper.mTextInput.setSingleLine(multiline == 0);
					if ( inputType==1 ) AGKHelper.mTextInput.setInputType( InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL | InputType.TYPE_NUMBER_FLAG_SIGNED );
					else AGKHelper.mTextInput.setInputType( InputType.TYPE_CLASS_TEXT );
					AGKHelper.mTextFinished = false;
					if ( cursorpos >= 0 ) AGKHelper.mTextInput.setSelection(cursorpos);
					AGKHelper.mTextInput.requestFocus();
					InputMethodManager lInputMethodManager = (InputMethodManager)act.getSystemService(Context.INPUT_METHOD_SERVICE);
					lInputMethodManager.showSoftInput( AGKHelper.mTextInput, 0 );
				}
				break;
			}
			case 5: // position text cursor only
			{
				if ( AGKHelper.mTextInput != null )
				{
					if ( cursorpos >= 0 )
					{
						try {
							AGKHelper.mTextInput.setSelection(cursorpos);
						}
						catch( IndexOutOfBoundsException e )
						{
							Log.w("Keyboard", "SetCursor index out of bounds: " + cursorpos);
						}
					}
				}
				break;
			}
		}
    }
}

class RunnableChartboost implements Runnable
{
	public Activity act;
	public int action = 0;
	public static int caching = 0;
	public static int cached = 0;
	public static int rewardCached = 0;
	public static int rewardCaching = 0;
	public static String AppID;
	public static String AppSig;
	public static boolean consent = false;

	private ChartboostDelegate chartBoostDelegate = new ChartboostDelegate() {
		@Override
		public boolean shouldDisplayInterstitial(String location) {
			return true;
		}

		@Override
		public boolean shouldRequestInterstitial(String location) {
			return true;
		}

		@Override
		public void didCacheInterstitial(String location) {
			Log.i("Chartboost", "Interstitial cached");
			caching = 0;
			cached = 1;
		}

		@Override
		public void didFailToLoadInterstitial(String location, CBImpressionError error) {
			caching = 0;
			cached = 0;
			Log.e("Chartboost", "Interstitial failed to load: " + error.toString());
		}

		@Override
		public void didDismissInterstitial(String location) {
			Log.i("Chartboost", "Interstitial dismissed");
			if ( cached == 0 && caching == 0 )
			{
				Chartboost.cacheInterstitial(CBLocation.LOCATION_DEFAULT);
				caching = 1;
			}
		}

		@Override
		public void didCloseInterstitial(String location) {
			Log.i("Chartboost", "Interstitial closed");
			if ( cached == 0 && caching == 0 )
			{
				Chartboost.cacheInterstitial(CBLocation.LOCATION_DEFAULT);
				caching = 1;
			}
		}

		@Override
		public void didDisplayInterstitial(String location) {
			Log.i("Chartboost", "Interstitial displayed");
		}

		@Override
		public boolean shouldRequestMoreApps(String location) {
			return true;
		}

		@Override
		public boolean shouldDisplayMoreApps(String location) {
			return true;
		}

		// reward videos

		@Override
		public void didCacheRewardedVideo(String location)
		{
			Log.i("Chartboost", "Reward video cached");
			rewardCached = 1;
			rewardCaching = 0;
		}

		@Override
		public void didClickRewardedVideo(String location)
		{
			Log.i("Chartboost", "Reward video clicked");
		}

		@Override
		public void didCloseRewardedVideo(String location)
		{
			Log.i("Chartboost", "Reward video closed");
			if ( rewardCached == 0 && rewardCaching == 0 )
			{
				Chartboost.cacheRewardedVideo(CBLocation.LOCATION_DEFAULT);
				rewardCaching = 1;
			}
		}

		@Override
		public void didCompleteRewardedVideo(String location, int reward)
		{
			Log.i("Chartboost", "Reward video rewarded");
			AGKHelper.m_iRewardAdRewardedChartboost = 1;
		}

		@Override
		public void didDismissRewardedVideo(String location)
		{
			Log.i("Chartboost", "Reward video dismissed");
			if ( rewardCached == 0 && rewardCaching == 0 )
			{
				Chartboost.cacheRewardedVideo(CBLocation.LOCATION_DEFAULT);
				rewardCaching = 1;
			}
		}

		@Override
		public void didDisplayRewardedVideo(String location)
		{
			Log.i("Chartboost", "Reward video displayed");
		}

		@Override
		public void didFailToLoadRewardedVideo(String location, CBImpressionError error)
		{
			Log.i("Chartboost", "Reward video failed to load: " + error.toString());
			rewardCached = 0;
			rewardCaching = 0;
		}

		@Override
		public boolean shouldDisplayRewardedVideo(String location) { return true; }
	};

	@Override
	public void run() {
		switch ( action )
		{
			case 1: // initialize
			{
				Log.i("Chartboost", "Initialize Chartboost SDK");

				cached = 0;
				caching = 1;
				Chartboost.startWithAppId(this.act, AppID, AppSig);
				Chartboost.restrictDataCollection(this.act, !consent);
				Chartboost.onCreate(this.act);
				Chartboost.setShouldRequestInterstitialsInFirstSession(true);
				Chartboost.setDelegate(this.chartBoostDelegate);
				Chartboost.onStart(this.act);
				Chartboost.cacheInterstitial(CBLocation.LOCATION_DEFAULT);
				break;
			}
			case 3: // show ad
			{
				Log.i("Chartboost", "Display Chartboost Ad");

				if ( cached == 0 )
				{
					if (caching == 0)
					{
						caching = 1;
						Chartboost.cacheInterstitial(CBLocation.LOCATION_DEFAULT);
					}
				}
				else
				{
					Log.i("Chartboost", "Showing Chartboost Ad");
					cached = 0;
					Chartboost.showInterstitial( CBLocation.LOCATION_DEFAULT );
				}

				break;
			}
			case 4: // cache More Apps
			{
				//Chartboost.sharedChartboost().cacheMoreApps();
				break;
			}
			case 5: // show More Apps
			{
				//Chartboost.sharedChartboost().showMoreApps();
				break;
			}
			case 6: // cache reward ad
			{
				Log.i("Chartboost", "Cache Chartboost Reward Ad");

				if ( rewardCached == 0 )
				{
					if (rewardCaching == 0)
					{
						rewardCaching = 1;
						Chartboost.cacheRewardedVideo(CBLocation.LOCATION_DEFAULT);
					}
				}
				break;
			}
			case 7: // show reward ad
			{
				Log.i("Chartboost", "Display Chartboost Reward Ad");

				if ( rewardCached == 0 )
				{
					if (rewardCaching == 0)
					{
						rewardCaching = 1;
						Chartboost.cacheRewardedVideo(CBLocation.LOCATION_DEFAULT);
					}
				}
				else
				{
					Log.i("Chartboost", "Showing Chartboost Reward Ad");
					rewardCached = 0;
					Chartboost.showRewardedVideo(CBLocation.LOCATION_DEFAULT);
				}
				break;
			}
			default:
			{
				Log.i("CBTEST", "undefinedChartboostAction");
				break;
			}
		}
	}
}

class RunnableAmazonAds implements Runnable
{
	public Activity act;
	public int action = 0;
	public static int caching = 0;
	public static int cached = 0;
	public static int testing = 0;
	public static String AppID;
	private static com.amazon.device.ads.InterstitialAd interstitialAd;

	private DefaultAdListener AmazonAdsDelegate = new DefaultAdListener() {
		@Override
		public void onAdLoaded(final Ad ad, final AdProperties adProperties) {
			Log.i("Amazon Ads", "Interstitial loaded");
			caching = 0;
			cached = 1;
		}

		@Override
		public void onAdFailedToLoad(final Ad view, final AdError error) {
			caching = 0;
			cached = 0;
			Log.e("Amazon Ads", "Failed to load interstitial - Error: " + error.getMessage());
		}

		@Override
		public void onAdDismissed(final Ad ad) {
			Log.i("Amazon Ads", "Interstitial dismissed");
			if ( cached == 0 && caching == 0 )
			{
				caching = 1;
				interstitialAd.loadAd();
			}
		}
	};

	@Override
	public void run() {
		switch ( action )
		{
			case 1: // initialize
			{
				Log.i("Amazon Ads", "Initializing Amazon Ads SDK");

				cached = 0;
				caching = 1;
				interstitialAd = new com.amazon.device.ads.InterstitialAd(act);
				AdRegistration.setAppKey(AppID);
				AdRegistration.enableTesting(testing != 0 ? true : false);
				interstitialAd.setListener(AmazonAdsDelegate);
				interstitialAd.loadAd();
				break;
			}
			case 2:
			{
				AdRegistration.enableTesting(testing != 0 ? true : false);
				break;
			}
			case 3: // show ad
			{
				Log.i("Amazon Ads", "Display Ad");

				if ( cached == 0 )
				{
					if ( caching == 0 )
					{
						caching = 1;
						interstitialAd.loadAd();
					}
				}
				else
				{
					Log.i("Amazon Ads", "Showing Ad");
					cached = 0;
					interstitialAd.showAd();
				}

				break;
			}
			default:
			{
				Log.i("Amazon Ads", "undefined action");
				break;
			}
		}
	}
}

// Used for agk::CreateAdvert()
class RunnableAd implements Runnable
{
	public Activity act;
	public int action = 0;
	public int horz = 1;
	public int vert = 2;
	public int offsetX = 0;
	public int offsetY = 0;
	public String pubID = "";
	public String rewardpubID = "";

	public static int testMode = 0;
	public static AdView ad = null;
	public static int adType = 0;
	public static com.google.android.gms.ads.InterstitialAd interstitial = null;
	public static RewardedVideoAd rewardAd = null;
	public static int cached = 0;
	public static int rewardCached = 0;

	public WindowManager.LayoutParams makeLayout()
	{
		WindowManager.LayoutParams ll_lp;

		//Just a sample layout parameters.
		ll_lp = new WindowManager.LayoutParams();
		ll_lp.format = PixelFormat.TRANSPARENT;
		ll_lp.height = -2;
		ll_lp.width = -2;
		ll_lp.gravity = 0;
		ll_lp.x = offsetX;
		ll_lp.y = offsetY;
		switch( horz )
		{
			case 0: ll_lp.gravity |= Gravity.LEFT; break;
			case 1: ll_lp.gravity |= Gravity.CENTER_HORIZONTAL; break;
			case 2: ll_lp.gravity |= Gravity.RIGHT; break;
			default: ll_lp.gravity |= Gravity.CENTER_HORIZONTAL; break;
		}
		switch( vert )
		{
			case 0: ll_lp.gravity |= Gravity.TOP; break;
			case 1: ll_lp.gravity |= Gravity.CENTER_VERTICAL; break;
			case 2: ll_lp.gravity |= Gravity.BOTTOM; break;
			default: ll_lp.gravity |= Gravity.BOTTOM; break;
		}
		ll_lp.type = WindowManager.LayoutParams.TYPE_APPLICATION_PANEL;
		ll_lp.flags = WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL;
		ll_lp.flags = ll_lp.flags | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

		return ll_lp;
	}

	public static final String md5(final String s) {
		try {
			// Create MD5 Hash
			MessageDigest digest = java.security.MessageDigest.getInstance("MD5");
			digest.update(s.getBytes());
			byte messageDigest[] = digest.digest();

			// Create Hex String
			StringBuffer hexString = new StringBuffer();
			for (int i = 0; i < messageDigest.length; i++) {
				String h = Integer.toHexString(0xFF & messageDigest[i]);
				while (h.length() < 2)
					h = "0" + h;
				hexString.append(h);
			}
			return hexString.toString();

		} catch (NoSuchAlgorithmException e) {
			Log.e("AdMob",e.toString());
		}
		return "";
	}

	public void run()
	{
		switch ( action )
		{
			case 1: // make ad
			{
				if ( ad == null )
				{
					ad = new AdView(act);
					ad.setAdUnitId(pubID);
					switch (adType)
					{
						case 0: ad.setAdSize(AdSize.BANNER); break;
						case 1: ad.setAdSize(AdSize.LARGE_BANNER); break;
						case 2: ad.setAdSize(AdSize.MEDIUM_RECTANGLE); break;
						case 3: ad.setAdSize(AdSize.FULL_BANNER); break;
						case 4: ad.setAdSize(AdSize.LEADERBOARD); break;
						case 5: ad.setAdSize(AdSize.SMART_BANNER); break;
						case 6: ad.setAdSize(AdSize.FLUID); break;
						default: ad.setAdSize(AdSize.BANNER);
					}

					WindowManager wm = (WindowManager) act.getSystemService(Context.WINDOW_SERVICE);
					WindowManager.LayoutParams layout = makeLayout();
					if ( adType == 5 ) layout.width = -1;
					wm.addView(ad, layout);

					AdRequest.Builder request = new AdRequest.Builder();

					if ( testMode == 1 )
					{
						String android_id = android.provider.Settings.Secure.getString(act.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
						String deviceId = md5(android_id).toUpperCase();
						request.addTestDevice(deviceId);
					}

					// if we don't have consent for personalized ads then tell Google
					if ( AGKHelper.m_iAdMobConsentStatus < 2 )
					{
						Bundle extras = new Bundle();
						extras.putString("npa", "1");
						request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
					}

					ad.loadAd( request.build() );
				}
				break;
			}

			case 2: // position ad
			{
				if ( ad == null ) return;

				WindowManager wm = (WindowManager) act.getSystemService(Context.WINDOW_SERVICE);
				WindowManager.LayoutParams layout = makeLayout();
				if ( adType == 5 ) layout.width = -1;
				wm.updateViewLayout(ad, layout);

				break;
			}

			case 3: // delete the ad
			{
				if ( ad == null ) return;

				WindowManager wm = (WindowManager) act.getSystemService(Context.WINDOW_SERVICE);
				wm.removeView(ad);
				ad = null;
				break;
			}

			case 4: // refresh the ad
			{
				if ( ad != null )
				{
					AdRequest.Builder request = new AdRequest.Builder();
					if ( testMode == 1 )
					{
						String android_id = android.provider.Settings.Secure.getString(act.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
						String deviceId = md5(android_id).toUpperCase();
						request.addTestDevice(deviceId);
					}
					if ( AGKHelper.m_iAdMobConsentStatus < 2 )
					{
						Bundle extras = new Bundle();
						extras.putString("npa", "1");
						request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
					}
					ad.loadAd(request.build());
				}
				break;
			}

			case 5: // hide ad
			{
				if ( ad != null ) ad.setVisibility(View.GONE);
				break;
			}

			case 6: // show ad
			{
				if ( ad != null ) ad.setVisibility(View.VISIBLE);
				break;
			}

			case 7: // pause ad
			{
				if ( ad != null ) ad.pause();
				if ( rewardAd != null ) rewardAd.pause(act);
				break;
			}

			case 8: // resume ad
			{
				if ( ad != null ) ad.resume();
				if ( rewardAd != null ) rewardAd.resume(act);
				break;
			}

			case 9: // fullscreen ad
			{
				Log.i("AdMob", "Show Interstitial");
				if ( interstitial == null )
				{
					interstitial = new com.google.android.gms.ads.InterstitialAd(act);
					interstitial.setAdUnitId(pubID);

					interstitial.setAdListener(new com.google.android.gms.ads.AdListener() {
						public void onAdLoaded() { cached = 1; Log.i("AdMob", "Interstitial Loaded"); }
						public void onAdClosed()
						{
							AdRequest.Builder request = new AdRequest.Builder();
							if ( testMode == 1 )
							{
								String android_id = android.provider.Settings.Secure.getString(act.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
								String deviceId = md5(android_id).toUpperCase();
								request.addTestDevice(deviceId);
							}
							if ( AGKHelper.m_iAdMobConsentStatus < 2 )
							{
								Bundle extras = new Bundle();
								extras.putString("npa", "1");
								request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
							}
							interstitial.loadAd(request.build());
							Log.i("AdMob", "Interstitial closed");
						}
					});
				}

				if ( interstitial.isLoaded() )
				{
					cached = 0;
					interstitial.show();
				}
				else
				{
					if ( !interstitial.isLoading() )
					{
						AdRequest.Builder request = new AdRequest.Builder();
						if ( testMode == 1 )
						{
							String android_id = android.provider.Settings.Secure.getString(act.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
							String deviceId = md5(android_id).toUpperCase();
							request.addTestDevice(deviceId);
						}
						if ( AGKHelper.m_iAdMobConsentStatus < 2 )
						{
							Bundle extras = new Bundle();
							extras.putString("npa", "1");
							request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
						}
						interstitial.loadAd(request.build());
					}
				}

				break;
			}

			case 10: // cache fullscreen ad
			{
				Log.i("AdMob", "Cache Interstitial");
				if ( interstitial == null )
				{
					interstitial = new com.google.android.gms.ads.InterstitialAd(act);
					interstitial.setAdUnitId(pubID);

					interstitial.setAdListener(new com.google.android.gms.ads.AdListener() {
						public void onAdLoaded() { cached = 1; Log.i("AdMob", "Interstitial Loaded"); }
						public void onAdClosed()
						{
							AdRequest.Builder request = new AdRequest.Builder();
							if ( testMode == 1 )
							{
								String android_id = android.provider.Settings.Secure.getString(act.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
								String deviceId = md5(android_id).toUpperCase();
								request.addTestDevice(deviceId);
							}
							if ( AGKHelper.m_iAdMobConsentStatus < 2 )
							{
								Bundle extras = new Bundle();
								extras.putString("npa", "1");
								request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
							}
							interstitial.loadAd(request.build());
							Log.i("AdMob", "Interstitial closed");
						}
					});
				}

				if ( !interstitial.isLoaded() && !interstitial.isLoading() )
				{
					AdRequest.Builder request = new AdRequest.Builder();
					if ( testMode == 1 )
					{
						String android_id = android.provider.Settings.Secure.getString(act.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
						String deviceId = md5(android_id).toUpperCase();
						request.addTestDevice(deviceId);
					}
					if ( AGKHelper.m_iAdMobConsentStatus < 2 )
					{
						Bundle extras = new Bundle();
						extras.putString("npa", "1");
						request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
					}
					interstitial.loadAd(request.build());
				}

				break;
			}

			case 11: // show reward ad
			{
				Log.i("AdMob", "Show reward ad");
				if ( rewardAd == null )
				{
					rewardAd = MobileAds.getRewardedVideoAdInstance(act);

					rewardAd.setRewardedVideoAdListener(new RewardedVideoAdListener() {
						public void onRewardedVideoAdLoaded() { rewardCached = 1; Log.i("AdMob", "Reward ad loaded"); }
						public void onRewardedVideoAdOpened() { Log.i("AdMob", "Reward ad opened"); }
						public void onRewardedVideoStarted() { Log.i("AdMob", "Reward ad started"); }
						public void onRewardedVideoAdLeftApplication() { Log.i("AdMob", "Reward ad left app"); }
						public void onRewarded(RewardItem item) { AGKHelper.m_iRewardAdRewarded = 1; Log.i("AdMob", "Reward ad rewarded"); }
						public void onRewardedVideoAdFailedToLoad(int errorCode) { Log.e( "AdMob", "Failed to load reward ad: " + Integer.toString(errorCode) ); }
						public void onRewardedVideoCompleted() { Log.i("AdMob", "Reward ad completed"); }
						public void onRewardedVideoAdClosed()
						{
							Log.i("AdMob", "Reward ad closed");

							Bundle extras = new Bundle();
							extras.putString("npa", "1");

							if ( testMode == 1 )
							{
								Log.i("AdMob", "loading test reward ad");
								AdRequest.Builder request = new AdRequest.Builder();
								if ( AGKHelper.m_iAdMobConsentStatus < 2 ) request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
								rewardAd.loadAd( "ca-app-pub-3940256099942544/5224354917", request.build() );
							}
							else
							{
								Log.i("AdMob", "loading real reward ad with: " + rewardpubID);
								AdRequest.Builder request = new AdRequest.Builder();
								if ( AGKHelper.m_iAdMobConsentStatus < 2 ) request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
								rewardAd.loadAd(rewardpubID, request.build());
							}
						}
					});
				}

				if ( rewardAd.isLoaded() )
				{
					rewardCached = 0;
					rewardAd.show();
				}
				else
				{
					Bundle extras = new Bundle();
					extras.putString("npa", "1");

					if ( testMode == 1 )
					{
						Log.i("AdMob", "loading test reward ad");
						AdRequest.Builder request = new AdRequest.Builder();
						if ( AGKHelper.m_iAdMobConsentStatus < 2 ) request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
						rewardAd.loadAd( "ca-app-pub-3940256099942544/5224354917", request.build() );
					}
					else
					{
						Log.i("AdMob", "loading real reward ad with: " + rewardpubID);
						AdRequest.Builder request = new AdRequest.Builder();
						if ( AGKHelper.m_iAdMobConsentStatus < 2 ) request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
						rewardAd.loadAd(rewardpubID, request.build());
					}
				}

				break;
			}

			case 12: // cache reward ad
			{
				Log.i("AdMob", "Cache reward ad");
				if ( rewardAd == null )
				{
					rewardAd = MobileAds.getRewardedVideoAdInstance(act);

					rewardAd.setRewardedVideoAdListener(new RewardedVideoAdListener() {
						public void onRewardedVideoAdLoaded() { rewardCached = 1; Log.i("AdMob", "Reward ad loaded"); }
						public void onRewardedVideoAdOpened() { Log.i("AdMob", "Reward ad opened"); }
						public void onRewardedVideoStarted() { Log.i("AdMob", "Reward ad started"); }
						public void onRewardedVideoAdLeftApplication() { Log.i("AdMob", "Reward ad left app"); }
						public void onRewarded(RewardItem item) { AGKHelper.m_iRewardAdRewarded = 1; Log.i("AdMob", "Reward ad rewarded"); }
						public void onRewardedVideoAdFailedToLoad(int errorCode) { Log.e( "AdMob", "Failed to load reward ad: " + Integer.toString(errorCode) ); }
						public void onRewardedVideoCompleted() { Log.i("AdMob", "Reward ad completed"); }
						public void onRewardedVideoAdClosed() {
							Log.i("AdMob", "Reward ad closed");

							Bundle extras = new Bundle();
							extras.putString("npa", "1");

							if ( testMode == 1 )
							{
								Log.i("AdMob", "loading test reward ad");
								AdRequest.Builder request = new AdRequest.Builder();
								if ( AGKHelper.m_iAdMobConsentStatus < 2 ) request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
								rewardAd.loadAd( "ca-app-pub-3940256099942544/5224354917", request.build() );
							}
							else
							{
								Log.i("AdMob", "loading real reward ad with: " + rewardpubID);
								AdRequest.Builder request = new AdRequest.Builder();
								if ( AGKHelper.m_iAdMobConsentStatus < 2 ) request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
								rewardAd.loadAd(rewardpubID, request.build());
							}
						}
					});
				}

				Bundle extras = new Bundle();
				extras.putString("npa", "1");

				if ( testMode == 1 )
				{
					Log.i("AdMob", "loading test reward ad");
					AdRequest.Builder request = new AdRequest.Builder();
					if ( AGKHelper.m_iAdMobConsentStatus < 2 ) request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
					rewardAd.loadAd( "ca-app-pub-3940256099942544/5224354917", request.build() );
				}
				else
				{
					Log.i("AdMob", "loading real reward ad with: " + rewardpubID);
					AdRequest.Builder request = new AdRequest.Builder();
					if ( AGKHelper.m_iAdMobConsentStatus < 2 ) request.addNetworkExtrasBundle(AdMobAdapter.class, extras);
					rewardAd.loadAd(rewardpubID, request.build());
				}

				break;
			}
		}
	}
}

class AGKSurfaceView extends SurfaceView implements SurfaceHolder.Callback, MediaPlayer.OnCompletionListener
{
	public volatile MediaPlayer player = null;
	public Activity act;
	public SurfaceHolder pHolder = null;
	public SurfaceTexture pTexture = null;
	public int iLastTex = 0;
	public int m_x = -10;
	public int m_y = 0;
	public int m_width = 1;
	public int m_height = 1;
	public volatile String m_filename = "";
	public int m_filetype = 0;

	public int prepared = 0;
	public int isPlaying = 0;
	public int pausePos = -1;
	public int paused = 0;
	public int completed = 0;

	public int isDisplayed = 0;
	public volatile int videoWidth = 0;
	public volatile int videoHeight = 0;
	public volatile int videoDuration = 0;
	public volatile int viewAdded = 0;
	public volatile int shouldRemoveView = 0;

	public float U1, U2, V1, V2;

	public static WindowManager.LayoutParams makeLayout(int x, int y, int width, int height)
	{
		WindowManager.LayoutParams ll_lp;

		//Just a sample layout parameters.
		ll_lp = new WindowManager.LayoutParams();
		ll_lp.format = PixelFormat.OPAQUE;
		ll_lp.height = height;
		ll_lp.width = width;
		ll_lp.gravity = Gravity.LEFT | Gravity.TOP;
		ll_lp.x = x;
		ll_lp.y = y;
		ll_lp.token = null;
		//ll_lp.gravity |= Gravity.CENTER_HORIZONTAL;
		//ll_lp.gravity |= Gravity.CENTER_VERTICAL;
		ll_lp.type = WindowManager.LayoutParams.TYPE_APPLICATION;
		ll_lp.flags = WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL;
		ll_lp.flags = ll_lp.flags | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
		ll_lp.flags = ll_lp.flags | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE;

		return ll_lp;
	}

	public AGKSurfaceView(Activity context)
	{
		super(context);

		getHolder().addCallback(this);
		getHolder().setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		act = context;
	}

	public void LoadVideo( String filename, int type )
	{
		Log.i("Video", "Load Video");

		m_filename = filename;
		m_filetype = type;
		U1 = 0;
		V1 = 0;
		U2 = 1;
		V2 = 1;

		if ( player != null )
		{
			synchronized( AGKHelper.videoLock )
			{
				player.reset();
				player.release();
				player = null;
			}

			StopVideo();
		}

		int m_duration = -1;
		int m_videoWidth = -1;
		int m_videoHeight = -1;

		MediaMetadataRetriever metaRetriever = new MediaMetadataRetriever();
		MediaPlayer tempPlayer = new MediaPlayer();

		Log.d("Video", "File: "+filename+" Type:"+type );

		try
		{
			switch( type )
			{
				case 0:
				{
					// asset folder
					AssetFileDescriptor afd = act.getAssets().openFd(filename);
					metaRetriever.setDataSource(afd.getFileDescriptor(),afd.getStartOffset(), afd.getLength());
					tempPlayer.setDataSource(afd.getFileDescriptor(),afd.getStartOffset(), afd.getLength());
					afd.close();
					break;
				}
				case 1:
				{
					// data folder
					metaRetriever.setDataSource(filename);
					tempPlayer.setDataSource(filename);
					break;
				}
				case 2:
				{
					// expansion file
					/*
					int index = filename.indexOf(':');
					if ( index < 0 )
					{
						Log.e("Load Video","Invalid file name for expansion file");
						m_filename = "";
						return;
					}
					String subfilename = filename.substring(index+1);
					ZipResourceFile expansionFile = APKExpansionSupport.getAPKExpansionZipFile(act, AGKHelper.g_iExpansionVersion, AGKHelper.g_iExpansionVersion);
					if ( expansionFile == null )
					{
						Log.e("Video","Failed to load expansion file");
						m_filename = "";
						return;
					}
					AssetFileDescriptor afd = expansionFile.getAssetFileDescriptor(subfilename);
					if ( afd == null )
					{
						Log.e("Video","Failed to find video file in expansion file");
						m_filename = "";
						return;
					}
					metaRetriever.setDataSource(afd.getFileDescriptor(),afd.getStartOffset(), afd.getLength());
					tempPlayer.setDataSource(afd.getFileDescriptor(),afd.getStartOffset(), afd.getLength());
					afd.close();
					*/
					break;
				}
				default:
				{
					Log.e("Video","Unrecognised file type");
					m_filename = "";
					return;
				}
			}

			int tempprepared = 0;

			String duration = metaRetriever.extractMetadata(MediaMetadataRetriever.METADATA_KEY_DURATION);
			if ( duration != null )
			{
				m_duration = Integer.valueOf(duration);
			}
			else
			{
				Log.w("Video","Duration is null");
				try
				{
					tempPlayer.prepare();
					tempprepared = 1;
					m_duration = tempPlayer.getDuration();
				}
				catch( Exception e )
				{
					Log.e("Video","Temp player couldn't prepare");
				}
			}

			Bitmap bmp = metaRetriever.getFrameAtTime();
			if ( bmp != null )
			{
				m_videoWidth = bmp.getWidth();
				m_videoHeight = bmp.getHeight();
			}
			else
			{
				Log.w("Video","Bitmap is null");
				try
				{
					if ( tempprepared == 0 ) tempPlayer.prepare();
					tempprepared = 1;
					m_videoWidth = tempPlayer.getVideoWidth();
					m_videoHeight = tempPlayer.getVideoHeight();
				}
				catch( Exception e )
				{
					Log.e("Video","Temp player couldn't prepare 2");
				}
			}

			Log.d("Video","Duration: "+m_duration);
			Log.d("Video","Width: "+Integer.toString(m_videoWidth)+" Height: "+Integer.toString(m_videoHeight));
			if ( m_videoWidth == 0 ) m_videoWidth = -1;
			if ( m_videoHeight == 0 ) m_videoHeight = -1;

			tempPlayer.reset();
			tempPlayer.release();
			tempPlayer = null;
		}
		catch(Exception e)
		{
			Log.e("Exception", e.toString() );
			StackTraceElement[] elements = e.getStackTrace();
			for ( int i = 0; i < elements.length; i++ )
				Log.e("Exception", elements[i].toString() );
			m_filename = "Error";
		}

		videoDuration = m_duration;
		videoWidth = m_videoWidth;
		videoHeight = m_videoHeight;
		pausePos = -1;
		completed = 0;
	}

	public void DeleteVideo()
	{
		Log.i("Video","Delete Video");

		m_filename = "";
		m_filetype = 0;
		paused = 0;

		if ( viewAdded == 1 )
		{
			shouldRemoveView = 0;
			WindowManager wm = (WindowManager) act.getSystemService(Context.WINDOW_SERVICE);
			wm.removeView(this);
			isDisplayed = 0;
		}
		else shouldRemoveView = 1;

		synchronized( AGKHelper.videoLock )
		{
			if ( pTexture != null ) pTexture = null;
			iLastTex = 0;
			if ( player != null )
			{
				player.reset();
				player.release();
				player = null;
			}
		}
	}

	public void PlayVideo()
	{
		Log.i("Video","Play Video");
		if ( m_filename.equals("") || m_filename.equals("Error") ) return;

		if ( player != null )
		{
			if ( pausePos >= 0 ) player.seekTo(pausePos);
			pausePos = -1;
			completed = 0;
			player.start();
		}

		paused = 0;

		if ( pTexture != null )
		{
			Log.e("Video", "Cannot play to screen whilst the video is playing to a texture");
			AGKHelper.ShowMessage(act,"Cannot play video to screen whilst the video is playing to a texture");
		}
		else
		{
			shouldRemoveView = 0;
			if (isDisplayed == 0) {
				WindowManager wm = (WindowManager) act.getSystemService(Context.WINDOW_SERVICE);
				WindowManager.LayoutParams layout = makeLayout(m_x, m_y, m_width, m_height);
				wm.addView(this, layout);
				isDisplayed = 1;
			}
		}
	}

	public void PlayVideoToTexture(int tex)
	{
		Log.i("Video","Play Video To Image");
		if ( m_filename.equals("") || m_filename.equals("Error") ) return;

		if ( player != null )
		{
			if ( pausePos >= 0 ) player.seekTo(pausePos);
			pausePos = -1;
			completed = 0;
			player.start();
		}

		paused = 0;

		if ( isDisplayed == 1 )
		{
			Log.e("Video","Cannot play to texture whilst the video is playing to the screen");
			AGKHelper.ShowMessage(act,"Cannot play video to texture whilst the video is playing to the screen");
		}
		else
		{
			if ( Build.VERSION.SDK_INT >= 14 )
			{
				if ( pTexture != null && tex != iLastTex )
				{
					OnContextLost();
				}
				iLastTex = tex;

				if ( pTexture == null )
				{
					pTexture = new SurfaceTexture(tex);
					setupPlayer(1);
				}
			}
			else
			{
				Log.e("Video", "Playing video to texture is not supported on this device");
				AGKHelper.ShowMessage(act,"Playing video to texture is not supported on this device" );
			}
		}
	}

	public void SetPosition( float seconds )
	{
		Log.i("Video","Set Position");
		if ( player != null )
		{
			player.seekTo((int) (seconds * 1000));
		}
		else
		{
			pausePos = (int)(seconds*1000);
		}
	}

	public void PauseVideo()
	{
		Log.i("Video","Pause Video");
		if ( m_filename.equals("") || m_filename.equals("Error") ) return;

		paused = 1;
		if ( player == null ) return;
		if ( !player.isPlaying() ) return;
		player.pause();
	}

	public void StopVideo()
	{
		Log.i("Video","Stop Video");
		paused = 0;

		if ( viewAdded == 1 )
		{
			shouldRemoveView = 0;
			WindowManager wm = (WindowManager) act.getSystemService(Context.WINDOW_SERVICE);
			wm.removeView(this);
			isDisplayed = 0;
			viewAdded = 0;
		}
		else shouldRemoveView = 1;

		synchronized( AGKHelper.videoLock )
		{
			if ( pTexture != null ) pTexture = null;
			iLastTex = 0;
			if ( player != null )
			{
				player.reset();
				player.release();
				player = null;
			}
		}
	}

	public void UpdateVideo()
	{
		synchronized( AGKHelper.videoLock )
		{
			if (pTexture == null) return;
			if (Build.VERSION.SDK_INT < 14) return;

			try
			{
				pTexture.updateTexImage();
				float matrix[] = new float[16];
				pTexture.getTransformMatrix(matrix);
				U1 = matrix[12];
				V1 = matrix[5] + matrix[13];
				U2 = matrix[0] + matrix[12];
				V2 = matrix[13];
			}
			catch( RuntimeException e )
			{
				Log.e( "Video", "Failed to update video texture: " + e.toString() );
			}
		}
	}

	public void SetDimensions( int x, int y, int width, int height )
	{
		Log.i("Video","Set Dimensions X:"+x+" Y:"+y+" Width:"+width+" Height:"+height);

		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;

		if ( viewAdded == 1 && pHolder != null )
		{
			WindowManager wm = (WindowManager) act.getSystemService(Context.WINDOW_SERVICE);
			WindowManager.LayoutParams layout = makeLayout(m_x,m_y,m_width,m_height);
			wm.updateViewLayout(this, layout);
		}
	}

	private void setupPlayer( int mode )
	{
		MediaPlayer newplayer = new MediaPlayer();
		newplayer.setOnCompletionListener(this);
		try
		{
			Log.i("Video2", "File: "+m_filename+" Type:"+m_filetype );

			switch( m_filetype )
			{
				case 0:
				{
					// asset folder
					AssetFileDescriptor afd = act.getAssets().openFd(m_filename);
					newplayer.setDataSource(afd.getFileDescriptor(),afd.getStartOffset(), afd.getLength());
					afd.close();
					break;
				}
				case 1:
				{
					// data folder
					newplayer.setDataSource(m_filename);
					break;
				}
				case 2:
				{
					// expansion file
					/*
					int index = m_filename.indexOf(':');
					if ( index < 0 )
					{
						Log.e("Load Video","Invalid file name for expansion file");
						return;
					}
					String subfilename = m_filename.substring(index+1);
					ZipResourceFile expansionFile = APKExpansionSupport.getAPKExpansionZipFile( act, AGKHelper.g_iExpansionVersion, AGKHelper.g_iExpansionVersion);
					if ( expansionFile == null )
					{
						Log.e("Video","Failed to load expansion file");
						return;
					}
					AssetFileDescriptor afd = expansionFile.getAssetFileDescriptor(subfilename);
					if ( afd == null )
					{
						Log.e("Video","Failed to find video file in expansion file");
						m_filename = "";
						return;
					}
					newplayer.setDataSource(afd.getFileDescriptor(),afd.getStartOffset(), afd.getLength());
					afd.close();
					*/
					break;
				}
				default:
				{
					Log.e("Video","Unrecognised file type");
					return;
				}
			}

			if ( mode == 0 ) newplayer.setDisplay(pHolder);
			else
			{
				if ( Build.VERSION.SDK_INT >= 14 ) newplayer.setSurface(new Surface(pTexture));
				else
				{
					Log.e("Video", "Playing video to texture is not supported on this device");
					AGKHelper.ShowMessage(act,"Playing video to texture is not supported on this device");
				}
			}

			try
			{
				newplayer.prepare();
			}
			catch( Exception e )
			{
				Log.e("Prepare Exception",e.toString());
			}
			if ( pausePos >= 0 ) newplayer.seekTo(pausePos);
			pausePos = -1;
			newplayer.start();
			if ( paused == 1 ) newplayer.pause();

			player = newplayer;

			float log1=(float)(Math.log(100-AGKHelper.g_fVideoVolume)/Math.log(100));
			player.setVolume( 1-log1, 1-log1 );
		}
		catch(Exception e)
		{
			Log.e("Exception", e.toString() );
			StackTraceElement[] elements = e.getStackTrace();
			for ( int i = 0; i < elements.length; i++ )
				Log.e("Exception", elements[i].toString() );
			m_filename = "Error";
		}
	}

	public void surfaceCreated(SurfaceHolder holder)
	{
		Log.i("Video","surface created");
		pHolder = holder;
		if ( player == null )
		{
			setupPlayer(0);
		}
	}

	public void surfaceDestroyed(SurfaceHolder holder)
	{
		Log.i("Video","surface destroyed");
		pHolder = null;

		if ( player != null )
		{
			if ( completed == 0 ) pausePos = player.getCurrentPosition();
			else pausePos = -1;
			synchronized( AGKHelper.videoLock )
			{
				player.reset();
				player.release();
				player = null;
			}
		}
	}

	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
	{
		Log.i("Video", "Surface changed");
	}

	@Override
	public void onAttachedToWindow()
	{
		viewAdded = 1;
		super.onAttachedToWindow();

		if ( shouldRemoveView == 1 )
		{
			shouldRemoveView = 0;
			WindowManager wm = (WindowManager) act.getSystemService(Context.WINDOW_SERVICE);
			wm.removeView(this);
			isDisplayed = 0;
			viewAdded = 0;
		}
	}

	@Override
	public void onDetachedFromWindow()
	{
		shouldRemoveView = 0;
		super.onDetachedFromWindow();
	}

	public void onCompletion(MediaPlayer mp)
	{
		Log.i("Video","Completed");
		AGKHelper.hasStartedVideo = 0;
		completed = 1;
		StopVideo();
	}

	public void OnContextLost()
	{
		synchronized( AGKHelper.videoLock ) {
			if (pTexture != null) {
				pTexture = null;
			}
			iLastTex = 0;

			if (player != null) {
				if (completed == 0) pausePos = player.getCurrentPosition();
				else pausePos = -1;
				player.reset();
				player.release();
				player = null;
			}
		}
	}
}

class RunnableVideo implements Runnable
{
	public Activity act;
	public static volatile AGKSurfaceView video = null;
	public int action = 0;
	
	public String filename = "";
	public int fileType = 0;
	public SurfaceHolder pHolder = null;
	public int m_x,m_y,m_width,m_height;
	public int tex = 0;
	public float pos = 0;

	public void run() 
	{
		if ( video == null ) video = new AGKSurfaceView(act);
		
		switch( action )
		{
			case 1: // Load video
			{
				video.LoadVideo(filename, fileType);
				break;
			}
			case 2: // set dimensions
			{
				video.SetDimensions(m_x, m_y, m_width, m_height);
				break;
			}
			case 3: // play
			{
				video.PlayVideo();
				break;
			}
			case 4: // stop
			{
				video.StopVideo();
				break;
			}
			case 5: // pause
			{
				video.PauseVideo();
				break;
			}
			case 6: // delete
			{
				//video.StopVideo();
				video.DeleteVideo();
				video = null;
				break;
			}
			case 7: // play to texture
			{
				video.PlayVideoToTexture(tex);
				break;
			}
			case 8: // context lost
			{
				video.OnContextLost();
				break;
			}
			case 9: // set position
			{
				video.SetPosition(pos);
				break;
			}
		}
	}
}

class AGKCamera
{
	public Camera deviceCamera = null;
	public Activity act;
	public SurfaceTexture pTexture = null;
	public int iLastTex = 0;
	public int cameraWidth = 0;
	public int cameraHeight = 0;

	public AGKCamera(Activity context)
	{
		act = context;
	}

	public void Stop()
	{
		Log.i("Camera","Stop Camera");

		if ( deviceCamera != null )
		{
			deviceCamera.stopPreview();
			deviceCamera.release();
			deviceCamera = null;
		}

		if ( pTexture != null )
		{
			pTexture = null;
		}
		iLastTex = 0;
	}

	public void Start(int tex, int cameraID)
	{
		Log.i("Camera","Start Camera");

		if ( Build.VERSION.SDK_INT >= 14 )
		{
			if ( pTexture != null && tex != iLastTex )
			{
				OnContextLost();
			}
			iLastTex = tex;

			if ( pTexture == null )
			{
				if ( cameraID >= Camera.getNumberOfCameras() ) cameraID = 0;

				pTexture = new SurfaceTexture(tex);
				deviceCamera = Camera.open(cameraID);

				Camera.Parameters params = deviceCamera.getParameters();
				Camera.Size previewSize = params.getPreferredPreviewSizeForVideo();
				if ( previewSize == null ) previewSize = params.getPreviewSize();
				cameraWidth = previewSize.width;
				cameraHeight = previewSize.height;

				if (params.getSupportedFocusModes().contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO))
				{
					params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
					deviceCamera.setParameters(params);
				}

				try {
					deviceCamera.setPreviewTexture(pTexture);
					deviceCamera.startPreview();
				}
				catch( IOException e ){
					Log.e( "Camera", "Failed to set camera texture" + e.toString() );
				}
			}
		}
		else
		{
			Log.e("Camera", "Playing device camera to image is not supported on this device");
			AGKHelper.ShowMessage(act,"Playing device camera to image is not supported on this device" );
		}
	}

	public void UpdateCamera()
	{
		if ( pTexture == null ) return;
		if ( Build.VERSION.SDK_INT < 14 ) return;

		pTexture.updateTexImage();
	}

	public void OnContextLost()
	{
		if ( deviceCamera != null )
		{
			deviceCamera.stopPreview();
			deviceCamera.release();
			deviceCamera = null;
		}

		if ( pTexture != null )
		{
			pTexture = null;
		}
		iLastTex = 0;
	}
}

class RunnableFacebook implements Runnable
{
	String szID;
	String szLink;
	String szPicture;
	String szName;
	String szCaption;
	String szDescription;
	Activity act;
	public Session session;
	
	public void run()
	{
		Facebook feed = new Facebook(AGKHelper.FacebookAppID);
		Bundle parameters = new Bundle();
	    
		if ( !szID.equals("") ) parameters.putString("to", szID);
		if ( !szLink.equals("") ) parameters.putString("link", szLink);
	    if ( !szPicture.equals("") ) parameters.putString("picture",szPicture);
	    if ( !szName.equals("") ) parameters.putString("name",szName);
	    if ( !szCaption.equals("") ) parameters.putString("caption",szCaption);
	    if ( !szDescription.equals("") ) parameters.putString("description", szDescription);
	    	    
	    feed.setSession(session);
	    
		feed.dialog(act, "feed", parameters,new DialogListener() {
	        public void onFacebookError(FacebookError arg0) { }
	        public void onError(DialogError arg0) { } 
	        public void onComplete(Bundle arg0) { }
	        public void onCancel() { }
	    });
	}
}

class AGKSpeechListener  implements TextToSpeech.OnInitListener, TextToSpeech.OnUtteranceCompletedListener
{
	@Override
	public void onInit(int status)
	{
		if ( status == TextToSpeech.SUCCESS )
		{
			if ( Build.VERSION.SDK_INT >= 21 )
			{
				try {
					if (AGKHelper.g_pTextToSpeech.getAvailableLanguages() != null) {
						AGKHelper.g_SpeechLanguages = AGKHelper.g_pTextToSpeech.getAvailableLanguages().toArray();
					}
				}
				catch( Exception e ) { Log.w("TextToSpeech", "Failed to get available languages"); }
			}
			AGKHelper.g_iSpeechReady = 1;
		}
		else AGKHelper.g_iSpeechReady = -1;
	}

	@Override
	public void onUtteranceCompleted(String utteranceId)
	{
		if ( utteranceId.equals(Integer.toString(AGKHelper.g_iSpeechIDLast)))
		{
			AGKHelper.g_iIsSpeaking = 0;
		}
	}
}

// Entry point for all AGK Helper calls
public class AGKHelper {

	public static String g_sLastURI = null;
	public static int isVisible = 0;

	// screen recording
	static MediaProjectionManager mMediaProjectionManager = null;
	static MediaRecorder mMediaRecorder = null;
	static MediaProjection mMediaProjection = null;
	static VirtualDisplay mVirtualDisplay = null;
	static int g_iScreenRecordMic = 0;
	static String g_sScreenRecordFile = "";
	public static void StartScreenRecording( Activity act, String filename, int microphone )
	{
		if ( g_pAct == null ) g_pAct = act;
		if ( Build.VERSION.SDK_INT >= 21 ) {
			if ( mMediaRecorder != null ) return;
			g_iScreenRecordMic = microphone;
			g_sScreenRecordFile = filename;
			mMediaProjectionManager = (MediaProjectionManager) act.getSystemService(Context.MEDIA_PROJECTION_SERVICE);
			act.startActivityForResult(mMediaProjectionManager.createScreenCaptureIntent(), 10002);
		}
		else
		{
			Log.w("Screen Recording", "Screen recording requires Android 5.0 or above");
		}
	}
	public static void StopScreenRecording()
	{
		if ( Build.VERSION.SDK_INT >= 21 ) {
			if (mMediaRecorder != null) {
				try
				{
					mMediaRecorder.stop();
				}
				catch( IllegalStateException e )
				{
					Log.w("ScreenRecorder", "Tried to stop media recorder in an illegal state");
				}
				mMediaRecorder.release();
				mMediaRecorder = null;

				mMediaProjection.stop();
				mVirtualDisplay.release();
			}
		}
	}
	public static int IsScreenRecording()
	{
		return mMediaRecorder == null ? 0 : 1;
	}

	public static void SetClipboardText( Activity act, String text )
	{
		Looper.prepare();

		ClipboardManager clipboard = (ClipboardManager) act.getSystemService( Context.CLIPBOARD_SERVICE );
		ClipData clip = ClipData.newPlainText( "Text", text );
		clipboard.setPrimaryClip( clip );
	}

	public static String GetClipboardText( Activity act )
	{
		Looper.prepare();

		ClipboardManager clipboard = (ClipboardManager) act.getSystemService(Context.CLIPBOARD_SERVICE);
		String pasteData;

		if (!(clipboard.hasPrimaryClip())) return "";
		//if (!(clipboard.getPrimaryClipDescription().hasMimeType(MIMETYPE_TEXT_PLAIN))) return "";

		ClipData.Item item = clipboard.getPrimaryClip().getItemAt(0);
		pasteData = item.getText().toString();
		if (pasteData != null) return pasteData;

		pasteData = item.coerceToText( act ).toString();
		if (pasteData != null) return pasteData;

		return "";
	}

	// window handling
	public static void MinimizeApp( Activity act )
	{
		act.moveTaskToBack(true);
	}
	
	public static void QuitApp( Activity act )
	{
		act.finish();
	}
	
	public static void OnStart( Activity act )
	{
		g_pAct = act;
		isVisible = 1;

		// resume ad
		RunnableAd run = new RunnableAd();
		run.action = 8;
		run.act = act;
		act.runOnUiThread( run);

		if ( g_iIAPStatus == 2 )
		{
			PurchasingService.getPurchaseUpdates(false);
		}

		if (mMediaRecorder != null)
		{
			if (Build.VERSION.SDK_INT >= 24)
			{
				try { mMediaRecorder.resume(); }
				catch( IllegalStateException e ) { Log.w("ScreenRecorder", "Tried to resume MediaRecorder from illegal state"); }
			}
		}
	}
	
	public static void OnStop( Activity act )
	{
		isVisible = 0;

		// pause ad
		RunnableAd run = new RunnableAd();
		run.action = 7;
		run.act = act;
		act.runOnUiThread( run );

		RunnableVideo video = new RunnableVideo();
		video.act = act;
		video.action = 8; // OnContextLost
		act.runOnUiThread(video);

		if( deviceCamera != null ) deviceCamera.OnContextLost();

		if (mMediaRecorder != null)
		{
			if (Build.VERSION.SDK_INT >= 24) mMediaRecorder.pause();
			else StopScreenRecording();
		}
	}

	public static String GetLastURIText()
	{
		return (g_sLastURI == null) ? "" : g_sLastURI;
	}

	public static void ClearLastURIText()
	{
		g_sLastURI = null;
	}

	public static int HasFirebase() { return 0; }

	public static void SetImmersiveMode( Activity act, int mode )
	{
		// Amazon doesn't have immersive mode
	}
	
	private static boolean servicesConnected(Activity act) {
        return false;
    }
	
	public static int GetGPSExists( Activity act )
	{
		return 0;
	}
	
	public static void StartGPSTracking( Activity act )
	{
		
	}
	
	public static void StopGPSTracking()
	{
		
	}
	
	public static float GetGPSLatitude()
	{
		return 0;
	}
	
	public static float GetGPSLongitude()
	{
		return 0;
	}
	
	public static float GetGPSAltitude()
	{
		return 0;
	}
	
	// GameCenter
	public static int GetGameCenterExists( Activity act )
	{
		return 1;
	}
	
	public static void GameCenterSetup( Activity act )
	{

	}
	
	public static void GameCenterLogin( Activity act )
	{

	}

	public static void GameCenterLogout() {}
	
	public static int GetGameCenterLoggedIn()
	{
		return 0;
	}

	public static String GetGameCenterPlayerID()
	{
		return "";
	}

	public static String GetGameCenterPlayerDisplayName()
	{
		return "";
	}
	
	public static void GameCenterSubmitAchievement( String szAchievementID, int iPercentageComplete )
	{

	}

	public static void GameCenterAchievementsShow( Activity act )
	{

	}
	
	public static void GameCenterSubmitScore( String szBoardID, int iScore )
	{

	}
	
	public static void GameCenterShowLeaderBoard( Activity act, String szBoardID )
	{

	}
	// End GameCenter

	public static String GetIP(Activity act)
	{
		//WifiManager wm = (WifiManager) act.getSystemService(Context.WIFI_SERVICE);
		//return Formatter.formatIpAddress(wm.getConnectionInfo().getIpAddress());

		try {
			List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
			for (NetworkInterface intf : interfaces) {
				if ( intf.isUp() )
				{
					List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
					for (InetAddress addr : addrs) {
						if (!addr.isLoopbackAddress() && !addr.isLinkLocalAddress() && !addr.isMulticastAddress()) {
							if (addr instanceof Inet4Address)
							{
								String sAddr = addr.getHostAddress().toUpperCase();
								return sAddr;
							}
						}
					}
				}
			}
		}
		catch (Exception ex)
		{
			Log.e( "GetIP", "Failed to get IPv4 address " + ex.toString() );
		}

		return "";
	}

	public static String GetIPv6(Activity act)
	{
		String sAddr = "";
		try {
			List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
			int count = 0;
			int currLevel = 0;
			for (NetworkInterface intf : interfaces) {
				int index = 0;
				count++;
				if (Build.VERSION.SDK_INT >= 19 ) index = intf.getIndex();
				//Log.e( "IP", "Interface: " + index + ", name: " + intf.getName() + ", count: " + count );
				if ( intf.isUp() )
				{
					List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
					for (InetAddress addr : addrs) {
						//Log.e( "IP", "  Addr: " + addr.getHostAddress() + ", name: " + addr.getHostName() );
						if (addr instanceof Inet6Address)
						{
							if (!addr.isLoopbackAddress() && !addr.isMulticastAddress())
							{
								String sIP = addr.getHostAddress().toLowerCase();
								int level = 0;
								if ( addr.isLinkLocalAddress() ) level = 1;
								else if ( addr.isSiteLocalAddress() ) level = 2;
								else if ( sIP.startsWith("fd") || sIP.startsWith("fc") ) level = 3; // unique site local
								else level = 4;

								//Log.e( "IP", "IP: " + sIP + ", Level: " + level );

								if ( level > currLevel ) {
									currLevel = level;
									sAddr = sIP;
									int percent = sAddr.indexOf('%');
									if (percent >= 0) sAddr = sAddr.substring(0, percent);
									sAddr = sAddr + "%" + intf.getName();
								}
							}
						}
					}
				}
			}
		}
		catch (Exception ex)
		{
			Log.e( "GetIP", "Failed to get IPv6 address " + ex.toString() );
		}

		return sAddr;
	}

	public static void Vibrate( Activity act, float seconds )
	{
		if (ContextCompat.checkSelfPermission(act, Manifest.permission.VIBRATE) == PackageManager.PERMISSION_GRANTED)
		{
			Vibrator vib = (Vibrator) act.getSystemService(Context.VIBRATOR_SERVICE);
			vib.vibrate((int) (seconds * 1000));
		}
		else
		{
			Log.w("Vibrate", "Tried to vibrate the device without the vibrate permission");
		}
	}
	
	public static int GetDisplayWidth( Activity act )
	{
		//DisplayMetrics displaymetrics = new DisplayMetrics();
		//act.getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);
		//int screenWidth = displaymetrics.widthPixels;
		//return screenWidth;

		// this works better when showing and hiding the navigation bar
		return act.getWindow().getDecorView().getWidth();
	}
	
	public static int GetDisplayHeight( Activity act )
	{
		//DisplayMetrics displaymetrics = new DisplayMetrics();
		//act.getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);
		//int screenHeight = displaymetrics.heightPixels;
		//return screenHeight;

		// this works better when showing and hiding the navigation bar
		return act.getWindow().getDecorView().getHeight();
	}
	
	// Edit box input
	static EditText mTextInput = null;
	static boolean mTextFinished = false;
	static boolean mTextHiding = false;
	public static int GetInputFinished( Activity act )
	{
		return mTextFinished ? 1 : 0;
	}
	
	public static int GetInputCursor( Activity act )
	{
		if ( mTextInput == null ) return 0;
		return mTextInput.getSelectionStart();
	}
	
	public static String GetInputText( Activity act )
	{
		if ( mTextInput == null ) return "";
		try
		{
			return mTextInput.getText().toString();
		}
		catch( Exception e )
		{
			return "";
		}
	}

	public static void SetInputText( Activity act, String text, int cursorpos )
	{
		//if ( mTextInput == null ) return;

		RunnableKeyboard run = new RunnableKeyboard();
		run.act = act;
		run.action = 3;
		run.text = text;
		run.cursorpos = cursorpos;
		act.runOnUiThread( run );
/*
		if ( AGKHelper.mTextInput != null )
		{
			AGKHelper.mTextInput.setText(text);
			if ( cursorpos >= 0 ) AGKHelper.mTextInput.setSelection(cursorpos);
		}*/
	}

	public static void SetInputTextCursor( Activity act, int cursorpos )
	{
		//if ( mTextInput == null ) return;

		RunnableKeyboard run = new RunnableKeyboard();
		run.act = act;
		run.action = 5;
		run.cursorpos = cursorpos;
		act.runOnUiThread( run );

		/*
		if ( AGKHelper.mTextInput != null )
		{
			if ( cursorpos >= 0 ) AGKHelper.mTextInput.setSelection(cursorpos);
		}*/
	}

	public static void ShowKeyboard( Activity act, int multiline, int inputType )
	{
		//InputMethodManager lInputMethodManager = (InputMethodManager)act.getSystemService(Context.INPUT_METHOD_SERVICE);
		//lInputMethodManager.showSoftInput( act.getWindow().getDecorView(), 0 );
		
		mTextFinished = false;
		
		if ( mTextInput != null && !mTextHiding ) 
		{
			RunnableKeyboard run = new RunnableKeyboard();
			run.act = act;
			run.action = 4;
			run.multiline = multiline;
			run.inputType = inputType;
			run.cursorpos = -1;
			act.runOnUiThread( run );
			return;
		}
		
		mTextHiding = false;
		
		RunnableKeyboard run = new RunnableKeyboard();
		run.act = act;
		run.action = 1;
		run.multiline = multiline;
		run.inputType = inputType;
		run.cursorpos = -1;
		act.runOnUiThread( run );
	}
	
	public static void HideKeyboard( Activity act )
	{
		mTextFinished = true;
		mTextHiding = true;
		
		if ( mTextInput == null ) 
		{
			InputMethodManager lInputMethodManager = (InputMethodManager)act.getSystemService(Context.INPUT_METHOD_SERVICE);
			lInputMethodManager.hideSoftInputFromWindow( act.getWindow().getDecorView().getWindowToken(), 0 );
			return;
		}
		
		RunnableKeyboard run = new RunnableKeyboard();
		run.act = act;
		run.action = 2;
		act.runOnUiThread( run );
	}
	
	public static String UpdateInputDevices()
	{
		String returnValues = "";
		int ids[] = InputDevice.getDeviceIds();
		
		// find all device IDs that are joysticks
		for ( int i = 0; i < ids.length; i++ )
		{
			InputDevice device = InputDevice.getDevice( ids[i] );
			if ( (device.getSources() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0 )
			{
				if ( returnValues.length() > 0 ) returnValues += ":";
				returnValues += Integer.toString(ids[i]);
			}
		}
		return returnValues;
	}
	
	public static void RefreshMediaPath( Activity act, String path )
	{
		MediaScannerConnection.scanFile(act, new String[]{path}, null, null);
	}
	
	public static int hasStartedVideo = 0;
	public static int videoLoaded = 0;
	public static final Object videoLock = new Object();
	public static float g_fVideoVolume = 100; 
	
	public static void LoadVideo( Activity act, String filename, int type )
	{
		Log.i("Video", "Load Video");
		RunnableVideo video = new RunnableVideo();
		video.act = act;
		video.action = 1;
		video.filename = filename;
		video.fileType = type;
		act.runOnUiThread(video);
		videoLoaded = 1;
	}
	
	public static void SetVideoDimensions( Activity act, int x, int y, int width, int height )
	{
		Log.i("Video", "Set Dimensions");
		RunnableVideo video = new RunnableVideo();
		video.act = act;
		video.action = 2;
		video.m_x = x;
		video.m_y = y;
		video.m_width = width;
		video.m_height = height;
		act.runOnUiThread(video);
	}
	
	public static void PlayVideo( Activity act )
	{
		RunnableVideo video = new RunnableVideo();
		video.act = act;
		video.action = 3;
		act.runOnUiThread(video);
		
		hasStartedVideo = 1;
	}

	public static void PlayVideoToTexture( Activity act, int tex )
	{
		RunnableVideo video = new RunnableVideo();
		video.act = act;
		video.tex = tex;
		video.action = 7;
		act.runOnUiThread(video);

		hasStartedVideo = 1;
	}
	
	public static void PauseVideo( Activity act )
	{
		RunnableVideo video = new RunnableVideo();
		video.act = act;
		video.action = 5;
		act.runOnUiThread(video);
		
		hasStartedVideo = 0;
	}
	
	public static void StopVideo( Activity act )
	{
		RunnableVideo video = new RunnableVideo();
		video.act = act;
		video.action = 4;
		act.runOnUiThread(video);
		
		hasStartedVideo = 0;
	}
	
	public static void DeleteVideo( Activity act )
	{
		RunnableVideo video = new RunnableVideo();
		video.act = act;
		video.action = 6;
		act.runOnUiThread(video);
		
		hasStartedVideo = 0;
		videoLoaded = 0;
	}

	public static void UpdateVideo()
	{
		// call this on the current thread as it is an OpenGL call
		if ( RunnableVideo.video == null ) return;
		RunnableVideo.video.UpdateVideo();
	}
	
	public static int GetVideoPlaying( Activity act )
	{
		return hasStartedVideo;
	}
	
	public static float GetVideoValue( Activity act, int value )
	{
		if ( RunnableVideo.video == null ) return videoLoaded==1 ? 0 : -1;
		if ( RunnableVideo.video.m_filename == null ) return videoLoaded==1 ? 0 : -1;
		if ( RunnableVideo.video.m_filename.equals("Error") ) return -1;
		if ( RunnableVideo.video.m_filename.equals("") ) return videoLoaded==1 ? 0 : -1;
		
		switch(value)
		{
			case 1: // video position
			{
				synchronized( videoLock ) 
				{
					if ( RunnableVideo.video == null ) return 0;
					if ( RunnableVideo.video.player == null ) return 0;
					return RunnableVideo.video.player.getCurrentPosition()/1000.0f;
				}
			}
			case 2: return RunnableVideo.video.videoDuration/1000.0f; // video duration
			case 3: return RunnableVideo.video.videoWidth; // video width
			case 4: return RunnableVideo.video.videoHeight; // video height
		}
		
		return 0;
	}

	public static float GetVideoTextureValue( Activity act, int value )
	{
		if ( RunnableVideo.video == null ) return 0;
		if ( RunnableVideo.video.m_filename.equals("Error") ) return 0;
		if ( RunnableVideo.video.m_filename.equals("") ) return 0;

		switch(value)
		{
			case 1: return RunnableVideo.video.U1;
			case 2: return RunnableVideo.video.V1;
			case 3: return RunnableVideo.video.U2;
			case 4: return RunnableVideo.video.V2;
		}

		return 0;
	}
	
	public static void SetVideoVolume( float volume )
	{
		g_fVideoVolume = volume;
		if ( g_fVideoVolume > 99 ) g_fVideoVolume = 99;
		if ( g_fVideoVolume < 0 ) g_fVideoVolume = 0;

		synchronized( videoLock )
		{
			if (RunnableVideo.video == null || RunnableVideo.video.player == null) return;

			float log1 = (float) (Math.log(100 - g_fVideoVolume) / Math.log(100));
			RunnableVideo.video.player.setVolume(1 - log1, 1 - log1);
		}
	}

	public static void SetVideoPosition( Activity act, float position )
	{
		RunnableVideo video = new RunnableVideo();
		video.act = act;
		video.pos = position;
		video.action = 9;
		act.runOnUiThread(video);
	}

	// youtube
	static void PlayYoutubeVideo( Activity act, String key, String video, int time )
	{

	}

	// camera to image
	public static AGKCamera deviceCamera = null;
	public static int deviceCameraID = -1;

	public static void SetDeviceCameraToImage( Activity act, int tex, int cameraID )
	{
		if ( deviceCamera == null ) deviceCamera = new AGKCamera(act);
		if ( tex <= 0)
		{
			deviceCamera.Stop();
			deviceCameraID = -1;
		}
		else
		{
			deviceCamera.Start(tex, cameraID);
			deviceCameraID = cameraID;
		}
	}

	public static int GetCameraWidth()
	{
		if ( deviceCamera == null ) return 0;
		else return deviceCamera.cameraWidth;
	}

	public static int GetCameraHeight()
	{
		if ( deviceCamera == null ) return 0;
		else return deviceCamera.cameraHeight;
	}

	public static void UpdateCamera()
	{
		// call this on the current thread as it is an OpenGL call
		if ( deviceCamera == null ) return;

		deviceCamera.UpdateCamera();
	}

	public static int GetNumCameras()
	{
		return Camera.getNumberOfCameras();
	}

	public static int GetCameraType( int cameraID )
	{
		if ( deviceCamera == null ) return 0;
		if ( deviceCamera.deviceCamera == null ) return 0;
		if ( cameraID < 0 || cameraID >= Camera.getNumberOfCameras() ) return 0;

		android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
		android.hardware.Camera.getCameraInfo(cameraID, info);

		if ( info.facing == Camera.CameraInfo.CAMERA_FACING_BACK ) return 1;
		else if ( info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT ) return 2;
		else return 0;
	}

	// Text to speech
	public static TextToSpeech g_pTextToSpeech = null;
	public static AGKSpeechListener g_pSpeechListener = null;
	public static int g_iSpeechReady = 0;
	public static int g_iIsSpeaking = 0;
	public static int g_iSpeechIDLast = 0;
	static Object[] g_SpeechLanguages = null;

	public static void TextToSpeechSetup( Activity act )
	{
		if ( g_pTextToSpeech != null ) return;

		g_pSpeechListener = new AGKSpeechListener();
		g_pTextToSpeech = new TextToSpeech( act, g_pSpeechListener );
		g_pTextToSpeech.setOnUtteranceCompletedListener(g_pSpeechListener);

		List<TextToSpeech.EngineInfo> engines = g_pTextToSpeech.getEngines();
		for ( TextToSpeech.EngineInfo engine: engines )
		{
			Log.i( "TextToSpeech", "Engine: " + engine.name );
		}

		Log.i( "TextToSpeech", "Default Engine: " + g_pTextToSpeech.getDefaultEngine() );
	}

	public static int GetTextToSpeechReady()
	{
		return g_iSpeechReady;
	}

	public static void Speak( Activity act, String text, int mode, int delay )
	{
		if ( g_pTextToSpeech == null ) return;
		if ( g_iSpeechReady == 0 )
		{
			Log.e("Speech","Failed to speak, speech engine not yet ready");
			return;
		}
		if ( g_iSpeechReady < 0 )
		{
			Log.e("Speech","Failed to speak, speech engine failed to initialise");
			return;
		}

		g_iIsSpeaking = 1;

		int queueMode = TextToSpeech.QUEUE_ADD;
		if ( mode > 0 ) queueMode = TextToSpeech.QUEUE_FLUSH;

		if ( delay > 0 )
		{
			g_iSpeechIDLast++;
			if ( g_iSpeechIDLast > 1000000000 ) g_iSpeechIDLast = 0;
			HashMap<String,String> hashMap = new HashMap();
			hashMap.put(TextToSpeech.Engine.KEY_PARAM_UTTERANCE_ID, Integer.toString(g_iSpeechIDLast) );
			g_pTextToSpeech.playSilence( delay, queueMode, hashMap );
		}

		g_iSpeechIDLast++;
		if ( g_iSpeechIDLast > 1000000000 ) g_iSpeechIDLast = 0;
		HashMap<String,String> hashMap = new HashMap();
		hashMap.put(TextToSpeech.Engine.KEY_PARAM_UTTERANCE_ID, Integer.toString(g_iSpeechIDLast) );

		if ( g_pTextToSpeech.speak( text, queueMode, hashMap ) < 0 )
		{
			Log.e( "TextToSpeech", "Failed to queue speech" );
		}
	}

	public static int GetSpeechNumVoices( Activity act )
	{
		if ( g_SpeechLanguages == null ) return 0;
		return g_SpeechLanguages.length;
	}

	public static String GetSpeechVoiceLanguage( Activity act, int index )
	{
		if ( g_SpeechLanguages == null ) return "";
		if ( index < 0 || index >= g_SpeechLanguages.length ) return "";

		Locale locale = (Locale)g_SpeechLanguages[ index ];
		return locale.toString();
	}

	public static String GetSpeechVoiceName( Activity act, int index )
	{
		if ( g_SpeechLanguages == null ) return "";
		if ( index < 0 || index >= g_SpeechLanguages.length ) return "";

		Locale locale = (Locale)g_SpeechLanguages[ index ];
		return locale.getDisplayName();
	}

	public static void SetSpeechLanguage( Activity act, String lang )
	{
		if ( g_pTextToSpeech == null ) return;
		String[] parts = lang.split("_");
		if ( parts.length <= 1 ) g_pTextToSpeech.setLanguage( new Locale(lang) );
		else g_pTextToSpeech.setLanguage( new Locale(parts[0],parts[1]) );
	}

	public static void SetSpeechLanguageByID( Activity act, String sID )
	{
		if ( g_SpeechLanguages == null ) return;

		int index = 0;
		try {
			index = Integer.parseInt(sID);
		}
		catch( NumberFormatException e )
		{
			Log.e( "SetSpeechLanguageByID", "Invalid language ID: " + sID );
			return;
		}
		if ( index < 0 || index >= g_SpeechLanguages.length ) return;

		Locale locale = (Locale)g_SpeechLanguages[ index ];
		g_pTextToSpeech.setLanguage( locale );
	}

	public static void SetSpeechRate( Activity act, float rate )
	{
		if ( g_pTextToSpeech == null ) return;
		g_pTextToSpeech.setSpeechRate( rate );
	}

	public static int IsSpeaking()
	{
		if ( g_pTextToSpeech == null ) return 0;
		return g_iIsSpeaking;
	}

	public static void StopSpeaking()
	{
		if ( g_pTextToSpeech == null ) return;
		g_pTextToSpeech.stop();
		g_iIsSpeaking = 0;
	}
	
	public static void ShowMessage( Activity act, String msg )
	{
		RunnableMessage run = new RunnableMessage();
		run.act = act;
		run.msg = msg;
		act.runOnUiThread(run);
	}

	// adverts
	static int m_iRewardAdRewarded = 0;
	static int m_iRewardAdRewardedChartboost = 0;

	static int m_iAdMobConsentStatus = -2; // -2=startup value triggers consent load, -1=loading, 0=unknown, 1=non-personalised, 2=personalised
	static String m_sAdMobPrivacyPolicy = "";
	static ConsentForm m_pAdMobConsentForm = null;

	public static void SetAdMobTestMode( int mode )
	{
		RunnableAd.testMode = mode;
	}

	public static void LoadAdMobConsentStatus( Activity act, String publisherID, String privacyPolicy )
	{
		if ( m_iAdMobConsentStatus == -2 )
		{
			m_iAdMobConsentStatus = -1;
			m_sAdMobPrivacyPolicy = privacyPolicy;
			final Activity pAct = act;
			ConsentInformation consentInformation = ConsentInformation.getInstance(act);
			String[] publisherIds = {publisherID};
			consentInformation.requestConsentInfoUpdate(publisherIds, new ConsentInfoUpdateListener() {
				@Override
				public void onConsentInfoUpdated(ConsentStatus consentStatus) {
					// if not EEA then we can use personalized ads
					if ( ConsentInformation.getInstance(pAct).isRequestLocationInEeaOrUnknown() == false ) m_iAdMobConsentStatus = 2;
					else
					{
						switch( consentStatus )
						{
							case PERSONALIZED: m_iAdMobConsentStatus = 2; break;
							case NON_PERSONALIZED: m_iAdMobConsentStatus = 1; break;
							default: m_iAdMobConsentStatus = 0;
						}
					}
				}

				@Override
				public void onFailedToUpdateConsentInfo(String errorDescription) {
					Log.w( "AdMob Consent", "Failed to request consent status");
					m_iAdMobConsentStatus = 0;
				}
			});
		}
	}

	public static int GetAdMobConsentStatus( Activity act )
	{
		if ( m_iAdMobConsentStatus < 0 ) return -1;
		else return m_iAdMobConsentStatus;
	}

	public static void RequestAdMobConsent( Activity act )
	{
		final Activity pAct = act;
		act.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				URL privacyUrl = null;
				try {
					privacyUrl = new URL( m_sAdMobPrivacyPolicy );
				} catch (MalformedURLException e) {
					e.printStackTrace();
					ShowMessage( pAct, "Failed to construct privacy policy URL" );
					return;
				}

				m_pAdMobConsentForm = new ConsentForm.Builder(pAct, privacyUrl)
						.withListener(new ConsentFormListener() {
							@Override
							public void onConsentFormLoaded() {
								Log.i( "AdMob Consent", "Form loaded, showing form");
								m_pAdMobConsentForm.show();
							}

							@Override
							public void onConsentFormOpened() {
								Log.i( "AdMob Consent", "Form displayed");
							}

							@Override
							public void onConsentFormClosed( ConsentStatus consentStatus, Boolean userPrefersAdFree ) {
								switch( consentStatus )
								{
									case PERSONALIZED: m_iAdMobConsentStatus = 2; break;
									case NON_PERSONALIZED: m_iAdMobConsentStatus = 1; break;
									default: m_iAdMobConsentStatus = 0;
								}
								Log.i( "AdMob Consent", "Form closed");
							}

							@Override
							public void onConsentFormError(String errorDescription) {
								Log.w( "AdMob Consent", "Failed to load consent form: "+ errorDescription);
								ShowMessage( pAct, "Ad Consent Form " + errorDescription );
								m_pAdMobConsentForm = null;
							}
						})
						.withPersonalizedAdsOption()
						.withNonPersonalizedAdsOption()
						.build();

				Log.i("Ad Consent", "Loading consent form" );
				m_pAdMobConsentForm.load();
			}
		});
	}

	public static void OverrideAdMobConsent( Activity act, int mode )
	{
		m_iAdMobConsentStatus = 1;
		if ( mode == 2 ) m_iAdMobConsentStatus = 2;
	}

	public static void OverrideChartboostConsent( Activity act, int mode )
	{
		RunnableChartboost.consent = false;
		if ( mode == 2 ) RunnableChartboost.consent = true;

		Chartboost.restrictDataCollection(act, !RunnableChartboost.consent);
	}

	public static void CreateAd(Activity act, String publisherID, int horz, int vert, int offsetX, int offsetY, int type)
	{
		RunnableAd run = new RunnableAd();
		run.pubID = publisherID;
		run.horz = horz;
		run.vert = vert;
		run.offsetX = offsetX;
		run.offsetY = offsetY;
		run.action = 1;
		run.act = act;
		run.adType = type;
		act.runOnUiThread(run);
	}

	public static void CacheFullscreenAd(Activity act, String publisherID)
	{
		RunnableAd run = new RunnableAd();
		run.pubID = publisherID;
		run.action = 10;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static void CreateFullscreenAd(Activity act, String publisherID)
	{
		RunnableAd run = new RunnableAd();
		run.pubID = publisherID;
		run.action = 9;
		run.act = act;
		act.runOnUiThread(run);
	}

	public static void CacheRewardAd(Activity act, String publisherID)
	{
		RunnableAd run = new RunnableAd();
		run.rewardpubID = publisherID;
		run.action = 12;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static void ShowRewardAd(Activity act, String publisherID)
	{
		m_iRewardAdRewarded = 0;

		RunnableAd run = new RunnableAd();
		run.rewardpubID = publisherID;
		run.action = 11;
		run.act = act;
		act.runOnUiThread(run);
	}

	public static int GetRewardAdRewarded()
	{
		return m_iRewardAdRewarded;
	}

	public static void ResetRewardAd()
	{
		m_iRewardAdRewarded = 0;
	}

	public static int GetRewardAdLoadedAdMob()
	{
		return RunnableAd.rewardCached;
	}

	public static void PositionAd(Activity act, int horz, int vert, int offsetX, int offsetY)
	{
		RunnableAd run = new RunnableAd();
		run.horz = horz;
		run.vert = vert;
		run.offsetX = offsetX;
		run.offsetY = offsetY;
		run.action = 2;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static void DeleteAd(Activity act)
	{
		RunnableAd run = new RunnableAd();
		run.action = 3;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static void RefreshAd(Activity act)
	{
		RunnableAd run = new RunnableAd();
		run.action = 4;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static void SetAdVisible(Activity act, int visible)
	{
		RunnableAd run = new RunnableAd();
		run.action = visible>0 ? 6 : 5;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static void SetChartboostDetails( Activity act, String publisherID, String publisherID2 )
	{
		RunnableChartboost.AppID = publisherID;
		RunnableChartboost.AppSig = publisherID2;

		RunnableChartboost run = new RunnableChartboost();
		run.action = 1;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static void CreateFullscreenAdChartboost(Activity act, int type)
	{
		RunnableChartboost run = new RunnableChartboost();
		run.action = 3;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static void CacheRewardAdChartboost(Activity act)
	{
		RunnableChartboost run = new RunnableChartboost();
		run.action = 6;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static void ShowRewardAdChartboost(Activity act)
	{
		m_iRewardAdRewardedChartboost = 0;

		RunnableChartboost run = new RunnableChartboost();
		run.action = 7;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static int GetRewardAdRewardedChartboost()
	{
		return m_iRewardAdRewardedChartboost;
	}

	public static void ResetRewardAdChartboost()
	{
		m_iRewardAdRewardedChartboost = 0;
	}

	public static int GetRewardAdLoadedChartboost()
	{
		return RunnableChartboost.rewardCached;
	}

	public static void SetAmazonAdDetails( Activity act, String publisherID )
	{
		RunnableAmazonAds.AppID = publisherID;

		RunnableAmazonAds run = new RunnableAmazonAds();
		run.action = 1;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static void SetAmazonAdTesting( Activity act, int testing )
	{
		RunnableAmazonAds.testing = testing;

		RunnableAmazonAds run = new RunnableAmazonAds();
		run.action = 2;
		run.act = act;
		act.runOnUiThread(run);
	}

	public static void CreateFullscreenAdAmazon(Activity act)
	{
		RunnableAmazonAds run = new RunnableAmazonAds();
		run.action = 3;
		run.act = act;
		act.runOnUiThread( run );
	}

	public static int GetFullscreenLoadedAdMob()
	{
		return RunnableAd.cached;
	}

	public static int GetFullscreenLoadedChartboost()
	{
		return RunnableChartboost.cached;
	}

	public static int GetFullscreenLoadedAmazon()
	{
		return RunnableAmazonAds.cached;
	}

	// local notifications
	static NotificationChannel mNotificationChannel = null;
	public static void SetNotification( Activity act, int id, int unixtime, String message, String deeplink )
	{
		if (mNotificationChannel == null && android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O)
		{
			NotificationManager mNotificationManager = (NotificationManager) act.getSystemService(Context.NOTIFICATION_SERVICE);
			mNotificationChannel = new NotificationChannel("notify","Notifications", NotificationManager.IMPORTANCE_DEFAULT );
			mNotificationChannel.setDescription("App Notifications");
			mNotificationManager.createNotificationChannel(mNotificationChannel);
		}

		Intent intent = new Intent(act, NotificationAlarmReceiver.class);
		intent.putExtra("title", act.getString(R.string.app_name));
		intent.putExtra("message", message);
		intent.putExtra("id",id);
		intent.putExtra("deeplink",deeplink);
		PendingIntent sender = PendingIntent.getBroadcast(act, id, intent, PendingIntent.FLAG_UPDATE_CURRENT);

		// Get the AlarmManager service
		AlarmManager am = (AlarmManager) act.getSystemService(Context.ALARM_SERVICE);
		am.set(AlarmManager.RTC_WAKEUP, unixtime * 1000L, sender);
	}

	public static void SetNotification( Activity act, int id, int unixtime, String message )
	{
		SetNotification( act, id, unixtime, message, "" );
	}

	public static void CancelNotification( Activity act, int id )
	{
		Intent intent = new Intent(act, NotificationAlarmReceiver.class);
		PendingIntent sender = PendingIntent.getBroadcast(act, id, intent, PendingIntent.FLAG_UPDATE_CURRENT);

		// Get the AlarmManager service
		AlarmManager am = (AlarmManager) act.getSystemService(Context.ALARM_SERVICE);
		am.cancel(sender);
	}
	
	public static void SetOrientation( Activity act, int orien )
	{
		act.setRequestedOrientation(orien);
	}
	
	public static int GetOrientation( Activity act )
	{
		return act.getWindowManager().getDefaultDisplay().getRotation();
	}

	public static String GetDeviceID(Activity nativeactivityptr)
	{
		// This ID will remain constant for this device until a factory reset is performed
		String uuid = Secure.getString(nativeactivityptr.getContentResolver(), Secure.ANDROID_ID);
		if ( uuid == null || uuid.equals("") )
		{
			SharedPreferences sharedPrefs = nativeactivityptr.getSharedPreferences( "PREF_UNIQUE_ID", Context.MODE_PRIVATE);
			uuid = sharedPrefs.getString( "PREF_UNIQUE_ID", null);

			if (uuid == null || uuid.equals("")) {
				uuid = UUID.randomUUID().toString();
				SharedPreferences.Editor editor = sharedPrefs.edit();
				editor.putString("PREF_UNIQUE_ID", uuid);
				editor.commit();
			}
		}
		return uuid;
	}

	public static int GetDeviceDPI( Activity act )
	{
		DisplayMetrics metrics = act.getResources().getDisplayMetrics();
		return (int) ((metrics.xdpi + metrics.ydpi) / 2.0);
	}

	public static String GetPackageName( Activity act )
	{
		return act.getPackageName();
	}

	public static int GetPlatform()
	{
		if ( Build.MANUFACTURER.equals("Amazon") ) return 1;
		else return 0;
	}
	
	// ********************
	// In App Purchase
	// ********************

	public static final int MAX_PRODUCTS = 25;
	public static int g_iPurchaseState = 1;
	public static int g_iNumProducts = 0;
	public static int[] g_iPurchaseProductStates = new int[MAX_PRODUCTS];
	public static String[] g_sPurchaseProductNames = new String[MAX_PRODUCTS];
	public static String[] g_sPurchaseProductPrice = new String[MAX_PRODUCTS];
	public static String[] g_sPurchaseProductDesc = new String[MAX_PRODUCTS];
	public static int[] g_iPurchaseProductTypes = new int[MAX_PRODUCTS];
	public static String[] g_sPurchaseProductSignature = new String[MAX_PRODUCTS];
	public static Activity g_pAct = null;
	public static final Object iapLock = new Object();
	public static int g_iIAPStatus = 0;
	
	public static void iapSetKeyData( String publicKey, String developerID )
	{
    	
	}
	
	public static void iapAddProduct( String name, int ID, int type )
	{
		if ( g_iIAPStatus != 0 )
		{
			if ( g_pAct != null ) ShowMessage(g_pAct, "Cannot add IAP product after calling InAppPurchaseSetup()" );
			return;
		}

		name = name.toLowerCase();
		Log.i("IAB AddProduct","Adding: " + name + " to ID: " + Integer.toString(ID));
		if ( ID < 0 || ID >= MAX_PRODUCTS ) return;
		g_iPurchaseProductStates[ ID ] = 0;
		g_sPurchaseProductNames[ ID ] = name;
		g_iPurchaseProductTypes[ ID ] = type;
		if ( ID+1 > g_iNumProducts ) g_iNumProducts = ID+1;
    }
	
	public static void iapSetup( Activity act )
	{
		if ( g_iIAPStatus > 0 )
		{
			switch( g_iIAPStatus )
			{
				case 1: ShowMessage( act, "Cannot set up IAP, setup is already in progress" ); break;
				case 2: ShowMessage( act, "Failed to call InAppPurchaseSetup(), setup has already been completed" ); break;
			}

			return;
		}

		g_iIAPStatus = 1;

		// purchase listener is created in OnCreate

		final Set<String> productSkus = new HashSet<String>();
		for (int i = 0; i < g_iNumProducts; i++) {
			productSkus.add(g_sPurchaseProductNames[i]);
		}
		PurchasingService.getProductData(productSkus);
    }
	
	public static void iapMakePurchase( Activity act, int ID )
	{
		if ( ID < 0 || ID >= g_iNumProducts )
		{
			AGKHelper.ShowMessage(act,"Invalid item ID");
			return;
		}

		if ( g_iPurchaseProductTypes[ ID ] == 0 && g_iPurchaseProductStates[ ID ] == 1 )
		{
			AGKHelper.ShowMessage(act,"You have already purchased that item");
			return; // non-consumable item already purchased
		}

		if ( g_iIAPStatus != 2 )
		{
			switch( g_iIAPStatus )
			{
				case -1: ShowMessage( act, "Cannot start purchase as IAP setup failed" ); break;
				case 0: ShowMessage( act, "Cannot start purchase as IAP has not been setup" ); break;
				case 1: ShowMessage(act, "Cannot start purchase until setup is finished, please try again in a minute"); break;
			}
			return;
		}

		if ( g_iPurchaseState == 0 )
		{
			ShowMessage( act, "Cannot start purchase as a purchase is already in progress" );
			return;
		}

		g_iPurchaseState = 0;
		g_iPurchaseProductStates[ ID ] = 0;
		g_sPurchaseProductSignature[ ID ] = "";
		Log.i("IAB MakePurchase", "Buying " + g_sPurchaseProductNames[ID]);

		PurchasingService.purchase(g_sPurchaseProductNames[ID] );
    }
	
	public static int iapCheckPurchaseState()
	{
		return g_iPurchaseState;
	}
	
	public static int iapCheckPurchase( int ID )
	{
		if ( ID < 0 || ID >= MAX_PRODUCTS ) return 0;
		return g_iPurchaseProductStates[ ID ];
	}

	public static String iapGetPrice( int ID )
	{
		synchronized (iapLock)
		{
			if (ID < 0 || ID >= MAX_PRODUCTS || g_sPurchaseProductPrice[ID] == null) return "";
			return g_sPurchaseProductPrice[ID];
		}
	}

	public static String iapGetDescription( int ID )
	{
		synchronized (iapLock)
		{
			if (ID < 0 || ID >= MAX_PRODUCTS || g_sPurchaseProductDesc[ID] == null) return "";
			return g_sPurchaseProductDesc[ID];
		}
	}

	public static String iapGetSignature( int ID )
	{
		synchronized (iapLock)
		{
			if ( ID < 0 || ID >= MAX_PRODUCTS || g_sPurchaseProductSignature[ID] == null ) return "";
			return g_sPurchaseProductSignature[ID];
		}
	}
	
	// ******************
	// Push Notifications
	// ******************
	
	public static void setPushNotificationKeys( String key1, String key2 )
	{
		
	}
		
	public static int registerPushNotification( Activity nativeactivityptr )
	{
		ShowMessage(nativeactivityptr,"Push Notifcations are not supported with Amazon devices");
		return 0;
	}
	
	public static String getPNRegID()
	{
		return "Error";
	}
	
	public static String GetAppName(Activity act)
	{
		final PackageManager pm = act.getApplicationContext().getPackageManager();
		
		ApplicationInfo ai;
		try 
		{
		    ai = pm.getApplicationInfo( act.getPackageName(), 0);
		} 
		catch (final NameNotFoundException e) 
		{
		    ai = null;
		}
		final String applicationName = (String) (ai != null ? pm.getApplicationLabel(ai) : "unknown");
		return applicationName;
	}

	// image chooser code
	public static int iChoosingImage = 0;
	public static String sChosenImagePath = "";

	// Function to launch Choose Image intent
	public static void StartChooseImage(Activity act, String path)
	{
		if ( iChoosingImage == 1 ) return;
		sChosenImagePath = path;

		// Ensure we can create a new activity in this static function
		Looper.prepare();

		iChoosingImage = 1;
		Intent photoPickerIntent = new Intent(Intent.ACTION_GET_CONTENT);
		photoPickerIntent.setType("image/*");
		act.startActivityForResult(photoPickerIntent, 9005);
	}

	public static int ChooseImageResult() { return iChoosingImage; }

	// camera
	public static int iCapturingImage = 0; // 0=no image, 1=capturing, 2=got image
	public static String sCameraSavePath = "";
	public static void CaptureImage(Activity nativeactivityptr, String path)
	{
		if ( iCapturingImage == 1 ) return;
		sCameraSavePath = path;

		// Ensure we can create a new activity in this static function
		Looper.prepare();

		iCapturingImage = 1;
		Intent cameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		nativeactivityptr.startActivityForResult( cameraIntent, 9004 );
	}

	public static int CaptureImageResult() { return iCapturingImage; }
	
	public static String GetLanguage()
	{
		return Locale.getDefault().getLanguage();
	}
	
	public static int isNetworkAvailable( Activity act ) 
	{
	    ConnectivityManager connectivityManager = (ConnectivityManager) act.getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
	    if ( activeNetworkInfo != null && activeNetworkInfo.isConnected() ) return 1;
	    else return 0;
	}

	public static int GetNetworkType( Activity act )
	{
		ConnectivityManager connectivityManager = (ConnectivityManager) act.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
		if ( activeNetworkInfo == null ) return -1;
		switch( activeNetworkInfo.getType() ) {
			case ConnectivityManager.TYPE_MOBILE: return 0;
			case ConnectivityManager.TYPE_WIFI: return 1;
			case ConnectivityManager.TYPE_ETHERNET: return 1;
			default: return -1;
		}
	}

	public static int GetStorageRemaining( Activity act, String path )
	{
		StatFs pathStats = new StatFs( path );
		if ( Build.VERSION.SDK_INT >= 18 ) return (int)(pathStats.getAvailableBytes()/1048576);
		else
		{
			int blockSize = pathStats.getBlockSize();
			long size = pathStats.getAvailableBlocks() * blockSize;
			return (int)(size/1048576);
		}
	}

	public static int GetStorageTotal( Activity act, String path )
	{
		StatFs pathStats = new StatFs( path );
		if ( Build.VERSION.SDK_INT >= 18 ) return (int)(pathStats.getTotalBytes()/1048576);
		else
		{
			int blockSize = pathStats.getBlockSize();
			long size = pathStats.getBlockCount() * blockSize;
			return (int)(size/1048576);
		}
	}
	
	// Facebook
	static int facebookLoginState = 1;
	static String FacebookAppID = "";

	public static void FacebookSetup( Activity act, String appID )
	{
		FacebookAppID = appID;
		Settings.setApplicationId( appID );
	}

	public static void FacebookActivateAppTracking( Activity act )
	{
		AppEventsLogger.activateApp(act);
	}

	public static void FacebookLogin(Activity act, String ID)
	{
		facebookLoginState = 1;
		Looper.prepare();
		
		Intent myIntent = new Intent(act, MyFacebookActivity.class);
		act.startActivity(myIntent);
	}
	
	public static void FacebookLogout()
	{
		facebookLoginState = 1;
		Session session = Session.getActiveSession();
        if ( session != null && !session.isClosed() ) 
        {
            session.closeAndClearTokenInformation();
        }
        Session.setActiveSession(null);
        facebookLoginState = 1;
	}
	
	public static int FacebookGetLoginState()
	{
		if( facebookLoginState == 1 ) return 0;
		if ( Session.getActiveSession() == null ) return -1;
		{
			if( Session.getActiveSession().getAccessToken().equals("") )
			{
				FacebookLogout();
				return -1;
			}
		}
		return 1; 
	}
	
	public static String FacebookGetAccessToken()
	{
		//Log.e("AGK",Session.getActiveSession().getAccessToken());
		if ( facebookLoginState == 1 ) return "";
		if ( Session.getActiveSession() == null ) return "Error";
		return Session.getActiveSession().getAccessToken();
	}
	
	public static void FacebookPost( Activity act, String szID, String szLink, String szPicture, String szName, String szCaption, String szDescription )
	{
		if ( Session.getActiveSession() == null )
		{
			AGKHelper.ShowMessage(act, "Unable to share on Facebook as you are not logged in");
		}
		else
		{
			RunnableFacebook feed = new RunnableFacebook();
			feed.szID = szID;
			feed.szLink = szLink;
			feed.szPicture = szPicture;
			feed.szName = szName;
			feed.szCaption = szCaption;
			feed.szDescription = szDescription;
			feed.act = act;
			feed.session = Session.getActiveSession();
			act.runOnUiThread(feed);
		}
	}
	
	public static String ConvertString( String s )
	{
		String s2 = java.text.Normalizer.normalize(s, java.text.Normalizer.Form.NFD).replaceAll("\\p{InCombiningDiacriticalMarks}+","");
		return s2;
	}
	
	public static void GenerateCrashReport(Activity act)
	{
		Process mLogcatProc = null;
	    try 
	    {
			mLogcatProc = Runtime.getRuntime().exec( new String[] {"logcat", "-d", "*:V" });
			
			InputStream is = mLogcatProc.getInputStream();
			
			SimpleDateFormat s = new SimpleDateFormat("yyyy-MM-dd-hh-mm-ss");
			String format = s.format(new Date());
	    
		   // File f = new File("/sdcard/crashreport "+format+".txt");
			File f = new File("/sdcard/crashreport.txt");
		    OutputStream os;
			os = new FileOutputStream(f);
		
			byte[] buffer = new byte[1024];
			int bytesRead;
        
			while ((bytesRead = is.read(buffer)) != -1)
			{
			    os.write(buffer, 0, bytesRead);
			}
		
        	//is.close();
			os.close();  
			
			Intent emailIntent = new Intent(android.content.Intent.ACTION_SEND);
			emailIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			emailIntent.putExtra(android.content.Intent.EXTRA_EMAIL, new String[]{"paul@thegamecreators.com"});
			emailIntent.putExtra(android.content.Intent.EXTRA_SUBJECT, "AGK Bug Report");
			emailIntent.putExtra(android.content.Intent.EXTRA_TEXT, "Bug report attached. Any additional information: ");

			String rawFolderPath = "file:///mnt/sdcard/crashreport.txt";

			// Here my file name is shortcuts.pdf which i have stored in /res/raw folder
			Uri emailUri = Uri.parse(rawFolderPath );
			emailIntent.putExtra(Intent.EXTRA_STREAM, emailUri);
			emailIntent.setType("text/plain");
			act.startActivity(Intent.createChooser(emailIntent, "Send bug report..."));
		} 
	    catch (IOException e) 
	    {
			e.printStackTrace();
		}
	}
	
	public static void setExpansionKey( String key )
	{
		
	}
	
	public static void SetExpansionVersion(int version)
	{
		
	}
	
	public static int GetExpansionState(Activity act)
	{
		return 0;
	}

	public static int GetExpansionError(Activity act) { return 0; }
	
	public static void DownloadExpansion(Activity act)
	{
		
	}
	
	public static float GetExpansionProgress(Activity act)
	{
		return 0;
	}

	public static int GetExpansionFileExists(Activity act, String filename) { return 0; }

	public static int ExtractExpansionFileImage(Activity act, String filename, String newPath ) { return 0; }

	static String[] g_sPermissions = { "WriteExternal", "Location", "Camera", "RecordAudio" };
	static String[] g_sPermissionsReal = { Manifest.permission.WRITE_EXTERNAL_STORAGE,
			Manifest.permission.ACCESS_FINE_LOCATION,
			Manifest.permission.CAMERA,
			Manifest.permission.RECORD_AUDIO };
	static int[] g_iPermissionStatus = {0,0,0,0}; // -1=denied, 0=not granted, but not asked, 1=in progress, 2=granted

	public static int GetAPIVersion() { return Build.VERSION.SDK_INT; }

	public static int GetRealPermission( String permission )
	{
		int index = -1;
		for( int i = 0; i < g_sPermissions.length; i++ )
		{
			if (g_sPermissions[i].equals(permission)) { index = i; break; }
		}

		return index;
	}

	// -1=rejected, 0=not asked, 1=in progress, 2=granted
	public static int CheckPermission( Activity act, String permission )
	{
		int index = GetRealPermission(permission);
		if ( index < 0 )
		{
			AGKHelper.ShowMessage( act, "Could not find permission" );
			return 0;
		}

		if ( Build.VERSION.SDK_INT < 23 )
		{
			if ( ContextCompat.checkSelfPermission(act, g_sPermissionsReal[index]) == PackageManager.PERMISSION_GRANTED) return 2;
			else return -1;
		}
		else
		{
			if ( g_iPermissionStatus[index] == 1 ) return 1;
			if (act.checkSelfPermission(g_sPermissionsReal[index]) == PackageManager.PERMISSION_GRANTED) return 2;
			else
			{
				if ( g_iPermissionStatus[index] == 2 ) return -1;
				else return 0;
			}
		}
	}

	public static void RequestPermission( Activity act, String permission )
	{
		if ( Build.VERSION.SDK_INT >= 23 )
		{
			int index = GetRealPermission(permission);
			if ( index < 0 )
			{
				AGKHelper.ShowMessage( act, "Could not find permission" );
				return;
			}

			if ( g_iPermissionStatus[index] == 1 ) return;

			g_iPermissionStatus[index] = 1;
			act.requestPermissions(new String[]{g_sPermissionsReal[index]}, 5+index);
		}
	}

	// Cloud data
	public static void SetupCloudData( final Activity act ) {}

	//public static void FinishCloudDataSetup( Task<GoogleSignInAccount> signInTask ) {}

	public static int GetCloudDataAllowed( Activity act )
	{
		return -2;
	}

	public static int GetCloudDataChanged()
	{
		return 0;
	}

	public static String GetCloudDataVariable( Activity act, String varName, String defaultValue ) { return defaultValue; }

	public static void SetCloudDataVariable( Activity act, String varName, final String varValue ) {}

	public static void DeleteCloudDataVariable( Activity act, String varName ) {}

	// Shared variables
	public static void SaveSharedVariableWithPermission( Activity act, String varName, String varValue )
	{
		String packageName = act.getPackageName();
		String folderName = packageName;

		if ( !packageName.equals("com.thegamecreators.agk_player2") )
		{
			if (packageName.startsWith("com.thegamecreators.eduguru")) {
				folderName = "com.thegamecreators.eduguru";
			} else {
				int pos = packageName.lastIndexOf(".");
				if (pos >= 0) folderName = packageName.substring(0, pos);
			}
		}

		varName = varName.replace("/","_");
		varName = varName.replace("\\","_");
		varName = varName.replace("*","_");
		varName = varName.replace("?","_");
		varName = varName.replace("\"","_");
		varName = varName.replace("|","_");
		varName = varName.replace("<","_");
		varName = varName.replace(">","_");
		varName = varName.replace(":","_");

		File extStorage = Environment.getExternalStorageDirectory();
		File sharedDataFolder = new File( extStorage, "SharedData" );
		sharedDataFolder = new File( sharedDataFolder, folderName );
		if ( !sharedDataFolder.exists() && !sharedDataFolder.mkdirs() ) {
			Log.e("Shared Data", "Failed to create shared data folder: " + sharedDataFolder.getPath());
			return;
		}

		File sharedVarsFile = new File( sharedDataFolder, varName );

		try
		{
			FileOutputStream output = new FileOutputStream(sharedVarsFile);
			output.write(varValue.length());
			output.write( varValue.getBytes() );
			output.close();
		}
		catch( FileNotFoundException e )
		{
			Log.e( "Shared Data", "Failed to create shared variable file: " + sharedVarsFile.getPath() );
		}
		catch( IOException e )
		{
			Log.e( "Shared Data", "Failed to write to shared variable file: " + sharedVarsFile.getPath() );
		}
	}

	public static void SaveSharedVariable( Activity act, String varName, String varValue )
	{
		// write local value to the shared preferences, then try and write globally
		SharedPreferences sharedPref = act.getSharedPreferences("agksharedvariables", Context.MODE_PRIVATE);
		SharedPreferences.Editor edit = sharedPref.edit();
		edit.putString( varName, varValue );
		edit.apply();

		// must request permissions on API 23 (Android 6.0) and above
		if (Build.VERSION.SDK_INT >= 23)
		{
			if (act.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
			{
				// show permission request popup
				act.requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
				return;
			}
		}

		SaveSharedVariableWithPermission(act, varName, varValue);
	}

	public static String LoadSharedVariable( Activity act, String varName, String defaultValue )
	{
		// must request permissions on API 23 (Android 6.0) and above
		if (Build.VERSION.SDK_INT >= 23)
		{
			if (act.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
			{
				// read from shared preferences instead
				SharedPreferences sharedPref = act.getSharedPreferences("agksharedvariables", Context.MODE_PRIVATE);
				return sharedPref.getString(varName, defaultValue);
			}
		}

		String packageName = act.getPackageName();
		String folderName = packageName;

		if ( !packageName.equals("com.thegamecreators.agk_player2") )
		{
			if (packageName.startsWith("com.thegamecreators.eduguru")) {
				folderName = "com.thegamecreators.eduguru";
			} else {
				int pos = packageName.lastIndexOf(".");
				if (pos >= 0) folderName = packageName.substring(0, pos);
			}
		}

		varName = varName.replace("/","_");
		varName = varName.replace("\\","_");
		varName = varName.replace("*","_");
		varName = varName.replace("?","_");
		varName = varName.replace("\"","_");
		varName = varName.replace("|","_");
		varName = varName.replace("<","_");
		varName = varName.replace(">","_");
		varName = varName.replace(":","_");

		File extStorage = Environment.getExternalStorageDirectory();
		File sharedDataFolder = new File( extStorage, "SharedData" );
		sharedDataFolder = new File( sharedDataFolder, folderName );
		if ( !sharedDataFolder.exists() )
		{
			return defaultValue;
		}

		String result = defaultValue;

		File sharedVarsFile = new File( sharedDataFolder, varName );
		try
		{
			FileInputStream input = new FileInputStream(sharedVarsFile);
			int length = input.read();
			byte[] bytes = new byte[length];
			input.read(bytes, 0, length);
			input.close();
			result = new String( bytes, "UTF-8" );
		}
		catch( FileNotFoundException e ) { return defaultValue; }
		catch( IOException e )
		{
			Log.e( "Shared Data", "Failed to read from shared variable file: " + sharedVarsFile.getPath() );
		}

		return result;
	}

	public static void DeleteSharedVariable( Activity act, String varName )
	{
		// delete any local value
		SharedPreferences sharedPref = act.getSharedPreferences( "agksharedvariables", Context.MODE_PRIVATE );
		SharedPreferences.Editor edit = sharedPref.edit();
		edit.remove(varName);
		edit.apply();

		// must request permissions on API 23 (Android 6.0) and above
		if (Build.VERSION.SDK_INT >= 23)
		{
			if (act.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
			{
				return;
			}
		}

		String packageName = act.getPackageName();
		String folderName = packageName;

		if ( !packageName.equals("com.thegamecreators.agk_player2") )
		{
			if (packageName.startsWith("com.thegamecreators.eduguru")) {
				folderName = "com.thegamecreators.eduguru";
			} else {
				int pos = packageName.lastIndexOf(".");
				if (pos >= 0) folderName = packageName.substring(0, pos);
			}
		}

		varName = varName.replace("/","_");
		varName = varName.replace("\\","_");
		varName = varName.replace("*","_");
		varName = varName.replace("?","_");
		varName = varName.replace("\"","_");
		varName = varName.replace("|","_");
		varName = varName.replace("<","_");
		varName = varName.replace(">","_");
		varName = varName.replace(":","_");

		File extStorage = Environment.getExternalStorageDirectory();
		File sharedDataFolder = new File( extStorage, "SharedData" );
		sharedDataFolder = new File( sharedDataFolder, folderName );
		File sharedVarsFile = new File( sharedDataFolder, varName );
		if ( sharedVarsFile.exists() )
		{
			sharedVarsFile.delete();
		}
	}

	public static void ViewFile( Activity  act, String sPath )
	{
		int pos = sPath.lastIndexOf('/');
		String sFileName;
		if ( pos >= 0 ) sFileName = sPath.substring(pos+1);
		else sFileName = sPath;

		// get extension
		pos = sPath.lastIndexOf('.');
		String sExt = "";
		if ( pos >= 0 ) sExt = sPath.substring(pos+1);

		File src = new File(sPath);

		String sMIME = MimeTypeMap.getSingleton().getMimeTypeFromExtension(sExt);
		Uri uri = FileProvider.getUriForFile(act, act.getApplicationContext().getPackageName() + ".provider", src);

		Intent target = new Intent( Intent.ACTION_VIEW );
		target.setDataAndType( uri, sMIME );
		target.setFlags( Intent.FLAG_ACTIVITY_NO_HISTORY | Intent.FLAG_GRANT_READ_URI_PERMISSION );

		try {
			act.startActivity(target);
		} catch (ActivityNotFoundException e) {
			ShowMessage(act,"No application found for file type \"" + sExt + "\"");
		}
	}

	public static void ShareText( Activity  act, String sText )
	{
		Intent target = new Intent( Intent.ACTION_SEND );
		target.setType( "text/plain" );
		target.putExtra( Intent.EXTRA_TITLE, "Share Text" );
		target.putExtra( Intent.EXTRA_TEXT, sText );

		try {
			act.startActivity(Intent.createChooser(target,"Share Text"));
		} catch (ActivityNotFoundException e) {
			ShowMessage(act,"No application found to share text");
		}
	}

	public static void ShareImage( Activity act, String sPath )
	{
		int pos = sPath.lastIndexOf('/');
		String sFileName;
		if ( pos >= 0 ) sFileName = sPath.substring(pos+1);
		else sFileName = sPath;

		File src = new File(sPath);

		Uri uri = FileProvider.getUriForFile(act, act.getApplicationContext().getPackageName() + ".provider", src);

		Intent target = new Intent( Intent.ACTION_SEND );
		target.setType( "image/*" );
		target.putExtra( Intent.EXTRA_TITLE, "Share Image" );
		target.putExtra( Intent.EXTRA_STREAM, uri );
		target.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);

		try {
			act.startActivity(Intent.createChooser(target,"Share Image"));
		} catch (ActivityNotFoundException e) {
			ShowMessage(act,"No application found to share images");
		}
	}

	public static void ShareImageAndText( Activity act, String sPath, String sText )
	{
		int pos = sPath.lastIndexOf('/');
		String sFileName;
		if ( pos >= 0 ) sFileName = sPath.substring(pos+1);
		else sFileName = sPath;

		File src = new File(sPath);
		Uri uri = FileProvider.getUriForFile(act, act.getApplicationContext().getPackageName() + ".provider", src);

		Intent target = new Intent( Intent.ACTION_SEND );
		target.setType( "image/*" );
		target.putExtra(Intent.EXTRA_TITLE, "Share Image And Text");
		target.putExtra( Intent.EXTRA_STREAM, uri );
		target.putExtra( Intent.EXTRA_TEXT, sText );
		target.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);

		try {
			act.startActivity(Intent.createChooser(target,"Share"));
		} catch (ActivityNotFoundException e) {
			ShowMessage(act,"No application found to share images");
		}
	}

	public static void ShareFile( Activity act, String sPath )
	{
		int pos = sPath.lastIndexOf('/');
		String sFileName;
		if ( pos >= 0 ) sFileName = sPath.substring(pos+1);
		else sFileName = sPath;

		// get extension
		pos = sPath.lastIndexOf('.');
		String sExt = "";
		if ( pos >= 0 ) sExt = sPath.substring(pos+1);

		File src = new File(sPath);

		String sMIME = MimeTypeMap.getSingleton().getMimeTypeFromExtension(sExt);
		Uri uri = FileProvider.getUriForFile(act, act.getApplicationContext().getPackageName() + ".provider", src);

		Intent target = new Intent( Intent.ACTION_SEND );
		target.setType( sMIME );
		target.putExtra( Intent.EXTRA_STREAM, uri );
		target.setFlags( Intent.FLAG_ACTIVITY_NO_HISTORY | Intent.FLAG_GRANT_READ_URI_PERMISSION );

		try {
			act.startActivity(Intent.createChooser(target,"Share File"));
		} catch (ActivityNotFoundException e) {
			ShowMessage(act,"No application found to share file type \"" + sExt + "\"");
		}
	}

	public static String GetExternalDir()
	{
		return Environment.getExternalStorageDirectory().getAbsolutePath();
	}

	public static int GetPackageInstalled( Activity act, String packageName )
	{
		try {
			if ( act.getPackageManager().getApplicationInfo(packageName, 0).enabled ) return 1;
			else return 0;
		} catch (PackageManager.NameNotFoundException e) {
			return 0;
		}
	}

	// SnapChat

	public static void SetSnapChatStickerSettings( float x, float y, int width, int height, float angle )
	{

	}

	public static void ShareSnapChat( Activity act, String image, String sticker, String caption, String url )
	{

	}
}
