package com.thegamecreators.agk_player;

import android.app.NativeActivity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.thegamecreators.agk_player2.R;

public class NotificationAlarmReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        Log.i("Notification Alarm","Received");
        if ( AGKHelper.isVisible == 1 ) return;

        String message = intent.getStringExtra("message");
        String title = intent.getStringExtra("title");
        int id = intent.getIntExtra("id", 0);
        String deeplink = intent.getStringExtra("deeplink");

        NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

        //Intent intent2 = new Intent(this, NotificationReceiver.class);
        //PendingIntent pIntent = PendingIntent.getActivity(this, 0, intent2, 0);

        Intent intent2 = new Intent(context, AGKActivity.class);
        if ( deeplink != null && !deeplink.equals("") )
        {
            intent2.setData( Uri.parse(deeplink) );
        }
        PendingIntent pIntent = PendingIntent.getActivity(context, 0, intent2, 0);

        CharSequence from = title;

        Bitmap largeIcon = null;
        try
        {
            largeIcon = BitmapFactory.decodeResource(context.getResources(), R.drawable.icon);
        }
        catch (Exception ex)
        {
            Log.i("AGK", "Failed to create large icon from app icon");
            ex.printStackTrace();
        }

        Notification notif = new NotificationCompat.Builder(context)
                .setContentTitle(from)
                .setContentText(message)
                .setSmallIcon(R.drawable.icon_white)
                .setContentIntent(pIntent)
                .setAutoCancel(true)
                .setLargeIcon(largeIcon)
                .setDefaults(Notification.DEFAULT_SOUND | Notification.DEFAULT_LIGHTS)
                .setChannelId("notify")
                .build();

        notificationManager.notify(id, notif);
    }
}