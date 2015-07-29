package com.dbylk.rwge.android;

import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.List;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;

import com.dbylk.rwge.Graphics;
import com.dbylk.rwge.RenderListener;
import com.dbylk.rwge.graphics.Color;
import com.dbylk.rwge.graphics.SceneRenderer;

/**
 * Implement the Graphics interface for Android. The Renderer Interface is
 * defined in GLSufaceView, it contains the methods which are called when the
 * GLSurface is created, the GLSurface is changed or the frame is rendered.
 */

public class AndroidGraphics implements Graphics, Renderer {
	AndroidApplication app;
	protected final AndroidAppConfig config;

	final View view;
	int width;
	int height;

	protected long lastFrameTime = System.nanoTime();
	/** Delta time between two frames, the unit is second. */
	protected float deltaTime = 0f;
	protected long frameId = -1;

	/**
	 * When (current time - fpsStartTime) equals to or bigger than one second,
	 * the frames is the last second's fps.
	 */
	protected long fpsStartTime = System.nanoTime();
	protected int frames;
	protected int fps;

	protected RendererState state = new RendererState();

	protected Color backgroundColor;
	protected BufferFormat bufferFormat;
	
	protected SceneRenderer sceneRenderer;
	ArrayList<RenderListener> renderListeners = new ArrayList<RenderListener>();
	

	public AndroidGraphics(AndroidApplication app, AndroidAppConfig config) {
		this.app = app;
		this.config = config;
		view = createGLSurfaceView();
		view.setFocusable(true);
		view.setFocusableInTouchMode(true);
		
		this.backgroundColor = config.backgroundColor;
	}

	protected View createGLSurfaceView() {
		if (!glEs20Available()) {
			Log.e("Graphics", "Can't run without OpenGL ES 2.0.");
			
			return null;
		}
		
		//Log.i("Test", "Create gl surface view.");
		GLSurfaceView view = new GLSurfaceView(app);
		view.setEGLContextClientVersion(2);
		view.setEGLConfigChooser(config.redSize, config.greenSize,config.blueSize, 
				config.alphaSize, config.depthSize,config.stencilSize);
		view.setRenderer(this);

		return view;
	}
	
	/** Check if OpenGL ES 2.0 is available. */
	protected boolean glEs20Available() {
		EGL10 egl = (EGL10)EGLContext.getEGL();
		EGLDisplay display = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);

		int[] version = new int[2];
		egl.eglInitialize(display, version);

		int EGL_OPENGL_ES2_BIT = 4;
		int[] configAttribs = {EGL10.EGL_RED_SIZE, 4, EGL10.EGL_GREEN_SIZE, 4, EGL10.EGL_BLUE_SIZE, 4, 
				EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL10.EGL_NONE};

		EGLConfig[] configs = new EGLConfig[10];
		int[] num_config = new int[1];
		egl.eglChooseConfig(display, configAttribs, configs, 10, num_config);
		egl.eglTerminate(display);
		
		return num_config[0] > 0;
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig eglConfig) {
		/** Set buffer format. */
		setBufferFormat(eglConfig);

		DisplayMetrics displayMetrics = new DisplayMetrics();
		app.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
		width = displayMetrics.widthPixels;
		height = displayMetrics.heightPixels;
		lastFrameTime = System.nanoTime();

		GLES20.glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		GLES20.glViewport(0, 0, width, height);
		
		// 以下代码参考libgdx，因为模块未完成，在此仅当作一个标记
		// Mesh.invalidateAllMeshes(app);
		// Texture.invalidateAllTextures(app);
		// Cubemap.invalidateAllCubemaps(app);
		// ShaderProgram.invalidateAllShaderPrograms(app);
		// FrameBuffer.invalidateAllFrameBuffers(app);
		
		if (this.config.useSceneRenderer) {
			sceneRenderer = new SceneRenderer(config.maxSpriteNum);
			addRenderListener(sceneRenderer);
		}
	}
	
	public void addRenderListener(RenderListener renderListener) {
		renderListeners.add(renderListener);
	}

	private void setBufferFormat(EGLConfig eglConfig) {
		EGL10 egl = (EGL10) EGLContext.getEGL();
		EGLDisplay display = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
		int redSize = getEGLAttrib(egl, display, eglConfig, EGL10.EGL_RED_SIZE, 0);
		int greenSize = getEGLAttrib(egl, display, eglConfig, EGL10.EGL_GREEN_SIZE, 0);
		int blueSize = getEGLAttrib(egl, display, eglConfig, EGL10.EGL_BLUE_SIZE, 0);
		int alphaSize = getEGLAttrib(egl, display, eglConfig, EGL10.EGL_ALPHA_SIZE, 0);
		int depthSize = getEGLAttrib(egl, display, eglConfig, EGL10.EGL_DEPTH_SIZE, 0);
		int stencilSize = getEGLAttrib(egl, display, eglConfig, EGL10.EGL_STENCIL_SIZE, 0);
		int sampleRate = getEGLAttrib(egl, display, eglConfig, EGL10.EGL_SAMPLES, 0);
		bufferFormat = new BufferFormat(redSize, greenSize, blueSize, alphaSize, depthSize, stencilSize, sampleRate);
	}

	private int getEGLAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attrib, int defaultValue) {
		int[] value = new int[1];

		if (egl.eglGetConfigAttrib(display, config, attrib, value)) {
			return value[0];
		}

		return defaultValue;
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		this.width = width;
		this.height = height;

		GLES20.glViewport(0, 0, width, height);
		//Log.i("Surface", "Width = " + width + ", Height = " + height);

		// When the first time changing the surface, call the application's method "create"
		if (!state.started) {
			app.appListener.create();

			synchronized (state) {
				state.started = true;
				state.running = true;
			}
		}

		// When the surface is changed, the application's method "resize" should be called.
		app.appListener.resize(width, height);
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		//Log.i("Render", fps + "");
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
		
		long time = System.nanoTime();
		deltaTime = (time - lastFrameTime) / 1000000000f;
		lastFrameTime = time;
		frameId++;

		synchronized (state) {
			if (state.resume) {
				// After a pause, deltaTime doesn't have any meaning.
				deltaTime = 0;

				app.appListener.resume();
				synchronized (renderListeners) {
					for (RenderListener renderListener : renderListeners) {
						renderListener.resume();
					}
				}
			}

			if (state.running) {
				app.appListener.render(deltaTime);
				synchronized (renderListeners) {
					for (RenderListener renderListener : renderListeners) {
						renderListener.render(deltaTime);
					}
				}

				frameId++;
			}

			if (state.pause) {
				app.appListener.pause();
				synchronized (renderListeners) {
					for (RenderListener renderListener : renderListeners) {
						renderListener.pause();
					}
				}
			}

			if (state.ended) {
				app.appListener.release();
				synchronized (renderListeners) {
					for (RenderListener renderListener : renderListeners) {
						renderListener.dispose();
					}
				}
			}

			// Temporary state should be correct after related methods having been executed.
			if (state.resume) {
				state.resume = false;
			}

			if (state.pause) {
				state.pause = false;

				// Send message to method "pause" to notify that pause action has been taken.
				state.notifyAll();
			}

			if (state.ended) {
				state.ended = false;

				// Send message to method "destroy" to notify that destroy action has been taken.
				state.notifyAll();
			}
		}
		
		// Calculate FPS.
		if (time - fpsStartTime > 1000000000) {
			fps = frames;
			frames = 0;
			fpsStartTime = time;
		}
		frames++;
	}
	
	/**
	 * This method only changes the status of Graphics. The real resume actions
	 * are taken in the "onDrawFrame" method.
	 */
	public void resume() {
		synchronized (state) {
			state.running = true;
			state.resume = true;
		}
	}

	/**
	 * This method only changes the status of Graphics. The real pause actions
	 * are taken in the "onDrawFrame" method.
	 */
	public void pause() {
		synchronized (state) {
			if (!state.running) {
				return;
			}

			state.running = false;
			state.pause = true;
			
			try {
				// Wait until the pause actions had been taken in "onDrawFrame" method.
				state.wait();
			} catch (InterruptedException ex) {
				System.err.println("Waitiong for pause synchronization failed!");
			}
		}
	}

	/**
	 * This method only changes the status of Graphics. The real destroy actions
	 * are taken in the "onDrawFrame" method.
	 */
	public void destroy() {
		synchronized (state) {
			state.running = false;
			state.ended = true;

			try {
				// Wait until the destroy actions had been taken in "onDrawFrame" method.
				state.wait();
			} catch (InterruptedException ex) {
				System.err.println("Waitiong for destroy synchronization failed!");
			}
		}
	}
	
	public void setBackgroundColor(Color color) {
		backgroundColor.set(color);
	}

	public View getView() {
		return view;
	}

	@Override
	public DisplayMode getDisplayMode() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean setDisplayMode(DisplayMode displayMode) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public BufferFormat getBufferFormat() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int getWidth() {
		return width;
	}

	@Override
	public int getHeight() {
		return height;
	}

	@Override
	public int getRefreshRate() {
		return fps;
	}

	@Override
	public int getBitPerPixel() {
		// TODO Auto-generated method stub
		return 0;
	}
	
	@Override
	public SceneRenderer getSceneRenderer() {
		return sceneRenderer;
	}

	public class RendererState {
		volatile boolean started = false;
		volatile boolean running = false;
		volatile boolean pause = false;
		volatile boolean resume = false;
		volatile boolean ended = false;

		public RendererState() {
			// Do nothing.
		}
	}
}
