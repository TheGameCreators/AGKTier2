package com.thegamecreators.agk_player;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import androidx.core.app.NotificationCompat;

import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;
import com.thegamecreators.agk_player2.R;
import android.util.Log;

import java.util.Map;

public class NotificationService extends FirebaseMessagingService {

    static NotificationChannel channel = null;

    @Override
    public void onNewToken( String token )
    {
        AGKHelper.GCM_PNRegID = token;
        Log.e( "Push Token", ": " + AGKHelper.GCM_PNRegID );
    }

    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {

        super.onMessageReceived(remoteMessage);

        if ( AGKHelper.isVisible == 1 ) return;

        Context context = getApplicationContext();

        if (channel == null && android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O)
        {
            NotificationManager mNotificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
            channel = new NotificationChannel("push_notify","Notifications", NotificationManager.IMPORTANCE_DEFAULT );
            channel.setDescription("Push Notification");
            mNotificationManager.createNotificationChannel(channel);
        }

        Intent intent2 = new Intent(context, AGKActivity.class);

        Map<String, String> params = remoteMessage.getData();
        String deeplink = params.get("deeplink");
        if ( deeplink != null && !deeplink.equals("") )
        {
            intent2.setData( Uri.parse(deeplink) );
        }

        PendingIntent pIntent = PendingIntent.getActivity(context, 0, intent2, 0);

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

        NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

        Notification notif = new NotificationCompat.Builder(context)
                .setContentTitle(remoteMessage.getData().get("title"))
                .setContentText(remoteMessage.getData().get("body"))
                .setSmallIcon(R.drawable.icon_white)
                .setContentIntent(pIntent)
                .setAutoCancel(true)
                .setLargeIcon(largeIcon)
                .setDefaults(Notification.DEFAULT_SOUND | Notification.DEFAULT_LIGHTS)
                .setChannelId("push_notify")
                .build();


        notificationManager.notify(1, notif);
  }
}
