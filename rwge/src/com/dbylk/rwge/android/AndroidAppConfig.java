package com.dbylk.rwge.android;

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
	
	public boolean useSceneRenderer = true;
	
	/** Create a default android application configuration. */
	public AndroidAppConfig() {
		// Do nothing.
	}
}
