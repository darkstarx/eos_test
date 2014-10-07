package com.eosproject.eos;

import com.eosproject.graphics.EOSGLSurfaceView;

import android.os.Bundle;
import android.provider.Settings;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.app.Activity;
import android.content.pm.ActivityInfo;


public class EOSActivity extends Activity {
	
	private static EOSActivity m_instance = null;
	
	protected RelativeLayout m_layout = null;
	protected EOSGLSurfaceView m_glView = null;
	
	public static EOSActivity getInstance() {
		return m_instance;
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		m_instance = this;
		final Window window = getWindow();
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		window.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		m_layout = new RelativeLayout(this);
		setContentView(m_layout);
		
		m_glView = new EOSGLSurfaceView(this);
		m_glView.setKeepScreenOn(true);
		m_layout.addView(m_glView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
		
		nativeOnCreate();
	}
	
	@Override
	protected void onStart()
	{
		super.onStart();
		nativeOnStart();
		
		if (Settings.System.getInt(getContentResolver(), Settings.System.ACCELEROMETER_ROTATION, 0) == 1) {
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
		} else {
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}
	}
	
	@Override
	protected void onStop()
	{
		nativeOnStop();
		super.onStop();
	}
	
	@Override
	protected void onPause()
	{
		m_glView.onPause();
		nativeOnPause();
		super.onPause();
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
		nativeOnResume();
		m_glView.onResume();
	}
	
	@Override
	protected void onDestroy()
	{
		nativeOnDestroy();
		
		m_glView.setVisibility(View.INVISIBLE);
		m_glView = null;
		m_layout = null;
		
		m_instance = null;
		super.onDestroy();
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		switch (keyCode) {
			case KeyEvent.KEYCODE_BACK:
				EOSActivity.this.finish();
				return true;
		}
		return super.onKeyDown(keyCode, event);
	}
	
	static {
		System.loadLibrary("eos_client");
	}
	
	private native void nativeOnCreate();
	private native void nativeOnDestroy();
	private native void nativeOnStart();
	private native void nativeOnResume();
	private native void nativeOnPause();
	private native void nativeOnStop();
}
