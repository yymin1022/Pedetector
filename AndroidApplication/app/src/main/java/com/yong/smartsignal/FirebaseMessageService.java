package com.yong.smartsignal;

import android.annotation.*;
import android.app.*;
import android.content.*;
import android.media.*;
import android.os.*;
import android.speech.tts.*;
import android.util.*;
import android.view.*;
import android.widget.*;

import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

import java.util.*;

import androidx.core.app.NotificationCompat;

public class FirebaseMessageService extends FirebaseMessagingService
{
	TextToSpeech tts;
	
	@Override
	public  void onMessageReceived(RemoteMessage remoteMessage){
		Log.d("MessageReceived from", remoteMessage.getFrom());
		
		if(remoteMessage.getData().size() > 0){
			Log.d("MessageReceived data", remoteMessage.getData().toString());
		}
		
		if(remoteMessage.getNotification() != null){
			Log.d("MessageReceived Body", remoteMessage.getNotification().getBody());
		}
		sendNotification(remoteMessage.getNotification().getBody());
	}
	
	private void sendNotification(String messageBody){
		//Statusbar Notification
		Intent intent = new Intent(this, MainActivity.class);
		intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_ONE_SHOT);
		NotificationCompat.Builder notificationBuilder = new NotificationCompat.Builder(this);
		notificationBuilder.setSmallIcon(R.drawable.ic_launcher);
		notificationBuilder.setContentTitle("Pedetector Notification");
		notificationBuilder.setContentText(messageBody);
		notificationBuilder.setAutoCancel(true);
		notificationBuilder.setContentIntent(pendingIntent);
		NotificationManager notificationManager = (NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);
		notificationManager.notify(0, notificationBuilder.build());
		
		//Toasts Notification
		Toast toast = new Toast(getApplicationContext());
		toast.setDuration(Toast.LENGTH_LONG);
		toast.setGravity(Gravity.CENTER, 0, 0);
		LayoutInflater toastLayout = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View toastView = toastLayout.inflate(R.layout.toast_layout, null);
		toast.setView(toastView);
		toast.show();
		
		//TTS Notification
		AudioManager am = (AudioManager) getApplicationContext().getSystemService(Context.AUDIO_SERVICE);
		if(am.isWiredHeadsetOn()){
			if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.LOLLIPOP){
				ttsUp21(messageBody);
			}else{
				ttsUnder20(messageBody);
			}
		}
	}
	
	@SuppressWarnings("deprecation")
	private void ttsUnder20(String text) {
		HashMap<String, String> map = new HashMap<>();
		map.put(TextToSpeech.Engine.KEY_PARAM_UTTERANCE_ID, "MessageId");
		tts.speak(text, TextToSpeech.QUEUE_FLUSH, map);
	}

	@TargetApi(Build.VERSION_CODES.LOLLIPOP)
	private void ttsUp21(String text) {
		String utteranceId=this.hashCode() + "";
		tts.speak(text, TextToSpeech.QUEUE_FLUSH, null, utteranceId);
	}
}
