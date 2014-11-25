package com.eosproject.eos;
// https://code.google.com/p/android-native-egl-example/

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Handler;
import android.widget.RelativeLayout;
import android.view.Surface;
import android.view.SurfaceView;
import android.view.SurfaceHolder;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;


public class EOSActivity extends Activity implements SurfaceHolder.Callback
{
	
	private static EOSActivity m_instance = null;
	private static long TICK_INTERVAL = 1000 / 60;
	
	private Handler m_handler = new Handler();
	protected RelativeLayout m_layout = null;
	
	public static EOSActivity getInstance() {
		return m_instance;
	}
	
	public static AssetManager getAssetManager() {
		return m_instance.getAssets();
	}
	
	private final Runnable m_tick = new Runnable() {
		@Override
		public void run() {
			try {
				nativeOnTick();
				m_handler.postDelayed(m_tick, TICK_INTERVAL);
			}
			catch (Exception e) {
				e.printStackTrace();
			}
		}
	};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		final Window window = getWindow();
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		window.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		SurfaceView surfaceView = new SurfaceView(this);
		surfaceView.setKeepScreenOn(true);
		surfaceView.getHolder().addCallback(this);
		
		m_layout = new RelativeLayout(this);
		m_layout.addView(surfaceView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
		setContentView(m_layout);
		
		m_instance = this;
		
		nativeOnCreate();
	}
	
	@Override
	protected void onStart()
	{
		super.onStart();
		nativeOnStart();
		m_handler.post(m_tick);
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
		nativeOnResume();
	}
	
	@Override
	protected void onPause()
	{
		nativeOnPause();
		super.onPause();
	}
	
	@Override
	protected void onStop()
	{
		m_handler.removeCallbacks(m_tick);
		nativeOnStop();
		super.onStop();
	}
	
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		nativeSetSurface(holder.getSurface());
	}
	
	public void surfaceCreated(SurfaceHolder holder) {
	}
	
	public void surfaceDestroyed(SurfaceHolder holder) {
		nativeSetSurface(null);
	}
	
	@Override
	protected void onDestroy()
	{
		nativeOnDestroy();
		m_layout = null;
		m_instance = null;
		super.onDestroy();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		if (event.getActionIndex() == 0) {
			final int action = event.getAction();
			final float x = event.getX(0);
			final float y = event.getY(0);
			nativeOnTouchEvent(action, x, y);
		}
		return true;
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		switch (keyCode) {
			case KeyEvent.KEYCODE_BACK:
				finish();
				return true;
			default:
				return nativeOnKeyDown(keyCode);
		}
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event)
	{
		return nativeOnKeyUp(keyCode);
	}
	
	static {
		System.loadLibrary("eos_client");
	}
	
	
	private native void nativeOnCreate();
	private native void nativeOnStart();
	private native void nativeOnResume();
	private native void nativeOnTick();
	private native void nativeOnPause();
	private native void nativeOnStop();
	private native void nativeOnDestroy();
	
	private static native void nativeSetSurface(Surface surface);
	private static native void nativeOnTouchEvent(final int touch_action, final float x, final float y);
	private static native boolean nativeOnKeyDown(final int keycode);
	private static native boolean nativeOnKeyUp(final int keycode);
}
