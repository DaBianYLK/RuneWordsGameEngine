package com.dbylk.rwge.android;

import java.io.IOException;

import android.media.MediaPlayer;

import com.dbylk.rwge.audio.Music;

public class AndroidMusic implements Music {
	private final AndroidAudio audio;
	private MediaPlayer player;
	private boolean isPrepared = true;
	private float volume = 1f;
	
	AndroidMusic(AndroidAudio audio, MediaPlayer player) {
		this.audio = audio;
		this.player = player;
	}
	
	@Override
	public void play() {
		if (player == null || player.isPlaying()) {
			return;
		}

		try {
			if (!isPrepared) {
				player.prepare();
				isPrepared = true;
			}
			player.start();
		} catch (IllegalStateException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void pause() {
		if (player == null) {
			return;
		}
		
		if (player.isPlaying()) {			
			player.pause();
		}
	}

	@Override
	public void stop() {
		if (player == null) {
			return;
		}
		
		if (isPrepared) {
			player.seekTo(0);
		}
		player.stop();
		isPrepared = false;
	}

	@Override
	public void setVolume(float volume) {
		if (player == null) {
			return;
		}
		player.setVolume(volume, volume);
		this.volume = volume;
	}

	@Override
	public float getVolume() {
		return volume;
	}

	@Override
	public void setLooping(boolean loop) {
		if (player == null) {
			return;
		}
		
		player.setLooping(loop);
	}

	@Override
	public boolean isLooping() {
		if (player == null) {
			return false;
		}
		
		return player.isLooping();
	}

	@Override
	public boolean isPlaying() {
		if (player == null) {
			return false;
		}
		
		return player.isPlaying();
	}

	/** Set the position by seconds. */
	@Override
	public void setPosition(float position) {
		if (player == null) {
			return;
		}
		
		try {
			if (!isPrepared) {
				player.prepare();
				isPrepared = true;
			}
			player.seekTo((int)(position * 1000));
		} catch (IllegalStateException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/** @return the current position in seconds. */
	@Override
	public float getPosition() {
		if (player == null) {
			return 0.0f;
		}
		
		return player.getCurrentPosition() / 1000f;
	}

	/** @return the duration of the music in seconds. */
	@Override
	public float getDuration() {
		if (player == null) {
			return 0.0f;
		}
		
		return player.getDuration() / 1000f;
	}

}
