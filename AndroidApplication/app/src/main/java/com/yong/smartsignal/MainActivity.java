package com.yong.smartsignal;

import android.app.*;
import android.content.*;
import android.os.*;
import android.view.*;

public class MainActivity extends Activity 
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
    }
	
	public void start(View v){
		startService(new Intent(this, BTService.class));
	}
	
	public void stop(View v){
		stopService(new Intent(this, BTService.class));
	}
}
