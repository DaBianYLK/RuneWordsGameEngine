package com.dbylk.rwge.android;

import com.dbylk.rwge.graphics.Color;

public class AndroidAppConfig {
	/** Number of bits for red channel. */
	public int redSize = 5;
	/** Number of bits for green channel. */
	public int greenSize = 6;
	/** Number of bits for yellow channel. */
	public int blueSize = 5;
	/** Number of bits for alpha channel. */
	public int alphaSize = 0;
	
	/** Number of bits for depth buffer. */
	public int depthSize = 16;
	/** Number of bits for stencil buffer. */
	public int stencilSize = 0;
	
	/** Number of samples for CSAA / MSAA. */
	public int numSamples = 0;
	
	public boolean keepScreenOn = true;
	public boolean useAccelerometer = true;
	public boolean useCompass = true;
	
	public boolean useSceneRenderer = true;
	public int maxSpriteNum = 32;
	public Color backgroundColor = Color.White;
	
	public boolean useAudio = true;
	public int maxAudioStreams = 16;
	
	public boolean showDebugLog = true;
	
	/** Create a default android application configuration. */
	public AndroidAppConfig() {
		// Do nothing.
	}
}
