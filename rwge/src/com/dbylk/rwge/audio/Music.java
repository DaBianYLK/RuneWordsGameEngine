package com.dbylk.rwge.audio;

/** A Music is a streamed audio file. */

public interface Music {
	public void play();
	public void pause();
	public void stop();
	
	public void setVolume(float volume);
	public float getVolume();
	
	public void setLooping(boolean loop);
	public boolean isLooping();
	
	public boolean isPlaying();
	
	public void setPosition(float position);
	public float getPosition();
	public float getDuration();
}
