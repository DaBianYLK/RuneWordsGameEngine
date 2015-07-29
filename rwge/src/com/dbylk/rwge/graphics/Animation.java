package com.dbylk.rwge.graphics;

import java.util.LinkedList;

public class Animation {
	Sprite sprite;
	boolean isEnabled = true;
	
	AnimationFrame[] frames;
	private float frameDuration;
	private int currentFrameIndex;
	
	private float playSpeed = 1.0f;
	private PlayMode playMode = PlayMode.Normal;
	private boolean isFinished = false;
	
	LinkedList<CompletionListener> completionListeners = new LinkedList<CompletionListener>();
	
	public Animation(AnimationFrame[] frames) {
		this.frames = frames;
	}
	
	public Animation(AnimationFrame[] frames, Sprite sprite) {
		this.frames = frames;
		this.sprite = sprite;
	}
	
	public void setSprite(Sprite sprite) {
		this.sprite = sprite;
	}
	
	public void setPlayMode(PlayMode playMode) {
		this.playMode = playMode;
		
		isFinished = false;
	}
	
	public PlayMode getPlayMode() {
		return playMode;
	}
	
	public void enable() {
		this.isEnabled = true;
	}
	
	public void disable() {
		this.isEnabled = false;
	}
	
	public boolean isEnabled() {
		return isEnabled;
	}
	
	public void play(float pastTime) {
		frameDuration += pastTime * playSpeed;
		
		if (pastTime > 0) {
			while (frameDuration > frames[currentFrameIndex].duration) {
				frameDuration -= frames[currentFrameIndex].duration;
				playNextFrame();
			}
		}
		else {
		// if (pastTime < 0)
			while (frameDuration < 0f) {
				frameDuration += frames[currentFrameIndex].duration;
				playLastFrame();
			}
		}
	}
	
	public void playNextFrame() {
		if (currentFrameIndex == frames.length - 1) {
			if (playMode == PlayMode.Loop) {
				currentFrameIndex = 0;
			}
			else {
				isFinished = true;
				for (CompletionListener listener : completionListeners) {
					listener.onAnimationCompleted();
				}
			}
		}
		else {
			currentFrameIndex++;
		}
		
		updateSprite();
	}
	
	public void playLastFrame() {
		if (currentFrameIndex == 0) {
			if (playMode == PlayMode.Loop) {
				currentFrameIndex = frames.length - 1;
			}
			else {
				isFinished = true;
				for (CompletionListener listener : completionListeners) {
					listener.onAnimationCompleted();
				}
			}
		}
		else {
			currentFrameIndex--;
		}
		
		updateSprite();
	}
	
	/** This method will reset the current duration of frame. */
	public void setFrame(int frameIndex) {
		if (frameIndex < 0) {
			currentFrameIndex = 0;
		}
		else if (frameIndex >= frames.length) {
			currentFrameIndex = frames.length - 1;
		}
		else {
			currentFrameIndex = frameIndex;
		}
		
		frameDuration = 0f;
		isFinished = false;
		
		updateSprite();
	}
	
	private void updateSprite() {
		if (isEnabled) {
			sprite.textureRect = frames[currentFrameIndex];
		}
	}
	
	public AnimationFrame getCurrentFrame() {
		return frames[currentFrameIndex];
	}
	
	/** 
	 * @return whether this animation has finished playing.
	 * An animation in loop mode will never finishes playing.
	 *  */
	public boolean isFinished() {
		return isFinished;
	}
	
	public void addCompletionListener(CompletionListener listener) {
		completionListeners.add(listener);
	}
	
	public void removeCompletionListener(CompletionListener listener) {
		completionListeners.remove(listener);
	}
	
	public enum PlayMode {
		Normal,
		Loop
	}
	
	public interface CompletionListener {
		public void onAnimationCompleted();
	}
}
