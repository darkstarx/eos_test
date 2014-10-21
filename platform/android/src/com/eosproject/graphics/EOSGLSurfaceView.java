package com.eosproject.graphics;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import com.eosproject.eos.EOSActivity;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class EOSGLSurfaceView extends GLSurfaceView {
	
	private static final int GL_ES_VERSION = 2;
	private static long TICK_INTERVAL = 1000 / 60;
	
	private EOSGLRenderer m_renderer;
	private boolean m_resumed = false;
	private boolean m_waiting_for_resume_deferred_call = false;
	
	private final Runnable m_gl_thread_tick = new Runnable() {
		@Override
		public void run() {
			if (!EOSGLRenderer.nativeIsValid()) {
				requestRender();
			}
		}
	};
	
	private final Runnable m_main_thread_tick = new Runnable() {
		@Override
		public void run() {
			if (m_resumed) {
				nativeOnTick();
				postDelayed(m_main_thread_tick, TICK_INTERVAL);
				queueEvent(m_gl_thread_tick);
			}
		}
	};
	
	
	public EOSGLSurfaceView(Context context) {
		super(context);
		setEGLContextClientVersion(GL_ES_VERSION);
		setPreserveEGLContextOnPause(true);
		setEGLConfigChooser(8, 8, 8, 8, 0, 0);
		setEGLContextFactory(new ContextFactory());
		
		m_renderer = new EOSGLRenderer();
		setRenderer(m_renderer);
		setRenderMode(RENDERMODE_WHEN_DIRTY);
		
		setFocusable(true);
		setFocusableInTouchMode(true);
		requestFocus();
	}
	
	
	@Override
	public void onResume() {
		m_waiting_for_resume_deferred_call = true;
		postDelayed(new Runnable() {
			@Override
			public void run() {
				if (!m_resumed && m_waiting_for_resume_deferred_call) {
					m_resumed = true;
					EOSGLSurfaceView.super.onResume();
					post(m_main_thread_tick);
				}
				m_waiting_for_resume_deferred_call = false;
			}
		}, TICK_INTERVAL);
	}
	
	
	@Override
	public void onPause() {
		if (m_resumed) {
			super.onPause();
			m_resumed = false;
		}
		m_waiting_for_resume_deferred_call = false;
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
	public boolean onKeyDown(int keyCode_, KeyEvent event)
	{
		final EOSActivity activity = EOSActivity.getInstance();
		if (activity == null) {
			return super.onKeyDown(keyCode_, event);
		}
		return nativeOnKeyDown(keyCode_);
	}
	
	
	@Override
	public boolean onKeyUp(int keyCode_, KeyEvent event)
	{
		final EOSActivity activity = EOSActivity.getInstance();
		if (activity == null) {
			return super.onKeyUp(keyCode_, event);
		}
		return nativeOnKeyUp(keyCode_);
	}
	
	
	private static class ContextFactory implements GLSurfaceView.EGLContextFactory 
	{
		@Override
		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
			final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
			final int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, GL_ES_VERSION, EGL10.EGL_NONE };
			final EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
			nativeCreateRenderer();
			return context;
		}
		
		@Override
		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
			nativeDestroyRenderer();
			egl.eglDestroyContext(display, context);
		}
	}
	
	
	private static native void nativeCreateRenderer();
	private static native void nativeDestroyRenderer();
	private static native void nativeOnTick();
	private static native void nativeOnTouchEvent(final int touch_action, final float x, final float y);
	private static native boolean nativeOnKeyDown(final int keycode);
	private static native boolean nativeOnKeyUp(final int keycode);
	
}
