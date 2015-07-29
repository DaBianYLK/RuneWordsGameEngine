package com.dbylk.rwge.audio;

/** A Sound is a short audio. A lot of Sounds can be played together at the same time. */

public interface Sound {
	/** Play the sound once. */
	public long play();
	/** Play the sound over and over again. */
	public long loop();
	
	/** Play the sound once at a volume ranged from [0, 1]. */
	public long play(float volume);
	/** Play the sound over and over again at a volume ranged from [0, 1]. */
	public long loop(float volume);
	
	public void pause();
	public void resume();
	public void stop();
}
