package com.yong.smartsignal;

import android.app.*;
import android.bluetooth.*;
import android.content.*;
import android.os.*;

import com.google.firebase.FirebaseApp;
import com.google.firebase.messaging.FirebaseMessaging;

public class BTService extends Service
{
	int distance = 0;
	
    private BluetoothAdapter bluetoothAdapter;
    private BluetoothAdapter.LeScanCallback BLECallback = new BluetoothAdapter.LeScanCallback() {

        @Override
        public void onLeScan(BluetoothDevice device, final int rssi, byte[] scanRecoed) {
			if(device!=null && device.getName()!=null){
				distance = 10^((-59-rssi)/(100));
				if(device.getName().startsWith("Pedetector") && distance <= 3){
					FirebaseMessaging.getInstance().subscribeToTopic(device.getName());
				}else if(device.getName().startsWith("Pedetector") && distance > 3){
                    FirebaseMessaging.getInstance().unsubscribeFromTopic(device.getName());
                }
			}
        }
    };

    @Override
    public void onCreate() {
        super.onCreate();
        BluetoothManager manager = (BluetoothManager) getSystemService(BLUETOOTH_SERVICE);
        bluetoothAdapter = manager.getAdapter();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        startBLEScan();
		startForeground(1, new Notification());

        FirebaseApp.initializeApp(getApplicationContext());
		FirebaseMessaging.getInstance().subscribeToTopic("test");
        return startId;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onDestroy() {
        stopBLEScan();
		stopForeground(true);
		
		FirebaseMessaging.getInstance().unsubscribeFromTopic("test");
        super.onDestroy();
    }

    private void startBLEScan() {
        bluetoothAdapter.startLeScan(BLECallback);
    }

    private void stopBLEScan() {
        bluetoothAdapter.stopLeScan(BLECallback);
    }
}
