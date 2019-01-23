package com.yong.smartsignal;

import android.util.*;

import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.iid.FirebaseInstanceIdService;

public class FirebaseInstanceIDService extends FirebaseInstanceIdService
{
	@Override
	public void onTokenRefresh(){
		String refreshedToken = FirebaseInstanceId.getInstance().getToken();
		Log.d("Token Refresh", refreshedToken);
		
		sendRegistrationToServer(refreshedToken);
	}
	
	private void sendRegistrationToServer(String token){
		
	}
}
