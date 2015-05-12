package com.dbylk.rwge;

/** Interface provides developer of a set of modules, such as graphics, listener, audio and so on. */

public interface Application {
	public Graphics getGraphics();
	public Input getInput();
	public Audio getAudio();
	public Files getFiles();
	public Net getNet();
}
