package com.dbylk.rwge.android;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.util.Log;
import android.view.Gravity;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;

import com.dbylk.rwge.*;
import com.dbylk.rwge.android.utils.AndroidUtils;
import com.dbylk.rwge.android.utils.Debug;

public class AndroidApplication extends Activity implements Application {
	protected AndroidGraphics graphics;
	protected AndroidInput input;
	protected AndroidAudio audio;
	protected Files files;
	protected Net net;
	ApplicationListener appListener;
	
	protected boolean keepScreenOn;
	//protected boolean #showStatuesBar;
	
	/** Use default android application configuration to set up an android application. */
	public void initialize(ApplicationListener listener) {
		AndroidAppConfig config = new AndroidAppConfig();
		
		initialize(listener, config);
	}
	
	/** Set up an android application with custom configuration. */
	public void initialize(ApplicationListener listener, AndroidAppConfig config) {
		// Initialize Android Utilities.
		if (config.showDebugLog) {
			Debug.showLog();
		}
		
		appListener = listener;
		
		graphics = new AndroidGraphics(this, config);
		// Show the GLSurfaceView on the screen.
		try {
			requestWindowFeature(Window.FEATURE_NO_TITLE);
		} catch (Exception ex) {
			System.out.println("Failed to request window with no title.");
		}
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);

		setContentView(graphics.getView(), createLayoutParams());
		
		setScreenAlwaysOn(config.keepScreenOn);
		//setStatusBarVisibility();
		
		input = new AndroidInput(this, config);
		graphics.addRenderListener(input);
		audio = new AndroidAudio(this, config);
		
		// Set the Rwge properties.
		Rwge.application = this;
		Rwge.graphics = this.graphics;
		Rwge.files = new AndroidFiles(this.getAssets());
		Rwge.input = this.input;
		Rwge.audio = this.audio;
	}

	private FrameLayout.LayoutParams createLayoutParams () {
		FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(android.view.ViewGroup.LayoutParams.MATCH_PARENT,
			android.view.ViewGroup.LayoutParams.MATCH_PARENT);
		layoutParams.gravity = Gravity.CENTER;
		return layoutParams;
	}
	
	private void setScreenAlwaysOn(boolean keepScreenOn) {
		if (keepScreenOn) {
			getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		}
		
		this.keepScreenOn = keepScreenOn;
	}
	
//	private void setStatusBarVisibility(boolean visible) {
//		if (!visible) {
//			graphics.getView().setSystemUiVisibility(View.GONE);
//		}
//		
//		this.showStatuesBar = visible;
//	}
	
	public Context getContext() {
		return this;
	}
	
	@Override
	public Graphics getGraphics() {
		return graphics;
	}

	@Override
	public Input getInput() {
		return input;
	}

	@Override
	public Audio getAudio() {
		return audio;
	}

	@Override
	public Files getFiles() {
		return files;
	}

	@Override
	public Net getNet() {
		return net;
	}

}
