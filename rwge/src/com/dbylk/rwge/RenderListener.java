package com.dbylk.rwge;

/**
 * This interface can be added to an application to receive calls when the
 * application is paused, resumed or rendered.
 */

public interface RenderListener {
	/** Called when the application frame is rendered. */
	public void render();
	
	/** Called when the application is paused. */
	public void pause();
	
	/** Called when the application get focus. */
	public void resume();
	
	/** Called when the application is closed. */
	public void dispose();
}
