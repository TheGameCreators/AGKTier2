package com.thegamecreators.agk_player;

import com.mycompany.mytemplate.R;
import java.util.Arrays;

import com.facebook.Session;
import com.facebook.SessionLoginBehavior;
import com.facebook.SessionState;

import android.app.Activity;
import android.content.Intent; 
import android.net.Uri;
import android.os.Bundle;

// Getting file from image intent return data
import android.provider.MediaStore;
import android.util.Log;
import android.database.Cursor;

// Dynamic Java Code
public class MyFacebookActivity extends Activity
{
	public static Session.StatusCallback statusCallback = new SessionStatusCallback();
	public static Activity act;

	// Called when the activity is first created.
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		act = this;
		// call Image Picker right away
		super.onCreate(savedInstanceState);
		setContentView(R.layout.facebookmain);
		
		Session session = Session.getActiveSession();
        if (session == null || !session.isOpened()) {
            if (savedInstanceState != null) {
            	session = Session.restoreSession(this, null, statusCallback, savedInstanceState);
            }
            if (session == null) {
            	session = new Session.Builder(this).setApplicationId(AGKHelper.FacebookAppID).build();
            }
            Session.setActiveSession(session);

			session.openForRead(new Session.OpenRequest(this).setPermissions(Arrays.asList("public_profile", "user_friends")).setCallback(statusCallback));
		}
        else 
        {
        	AGKHelper.facebookLoginState = 0;
        	finish();
        }
	}	
	
	//@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data)
	{ 
		// we can now intercept the return value in our dynamically created Java code
	    super.onActivityResult(requestCode, resultCode, data); 
	    Session.getActiveSession().onActivityResult(this, requestCode, resultCode, data);
	}
	
	@Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        Session session = Session.getActiveSession();
        Session.saveSession(session, outState); 
    }
	
	private static class SessionStatusCallback implements Session.StatusCallback {
        public void call(Session session, SessionState state, Exception exception) {
        	if ( session.isOpened() || session.isClosed() ) 
        	{
        		AGKHelper.facebookLoginState = 0;
				act.finish();
        	}
        }
    }
}

	