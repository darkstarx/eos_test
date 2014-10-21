package com.eosproject.graphics;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.res.Configuration;

import com.eosproject.eos.EOSActivity;


public class EOSGLRenderer implements EOSGLSurfaceView.Renderer {

	@Override
	public void onDrawFrame(GL10 gl) {
		nativeOnDrawFrame();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		boolean is_tablet = false;
		final EOSActivity context = EOSActivity.getInstance();
		if (context!=null) {
			is_tablet = (context.getResources().getConfiguration().screenLayout & Configuration.SCREENLAYOUT_SIZE_MASK) >= Configuration.SCREENLAYOUT_SIZE_LARGE;
		}
		nativeOnSurfaceChanged(width, height, is_tablet);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	}

	// Нативные методы - все выполняются из OpenGL потока!
	private static native void nativeOnSurfaceChanged(int width, int height, boolean is_tablet);
	private static native void nativeOnDrawFrame();
	public static native boolean nativeIsValid();
}
