package com.dbylk.rwge;

/** This interface is used to monitor the application's life cycle. */

public interface ApplicationListener {
	/** Called when the application is created. */
	public void create();
	
	/** Called when the application window was resized. */
	public void resize(int width, int height);
	
	/** Called when the application frame is rendered. */
	public void render(float deltaTime);
	
	/** Called when the application is paused. */
	public void pause();
	
	/** Called when the application get focus. */
	public void resume();
	
	/** Called when the application is closed. */
	public void release();
}
