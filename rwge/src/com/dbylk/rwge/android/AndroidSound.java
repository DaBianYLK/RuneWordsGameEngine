package com.dbylk.rwge.android;

import java.util.LinkedList;

import android.media.AudioManager;
import android.media.SoundPool;

import com.dbylk.rwge.audio.Sound;

public class AndroidSound implements Sound {
	static final int MaxStreamNum = 8;
	
	final SoundPool soundPool;
	final AudioManager manager;
	final int soundId;
	final LinkedList<Integer> streamIds = new LinkedList<Integer>();
	
	AndroidSound(SoundPool pool, AudioManager manager, int soundId) {
		this.soundPool = pool;
		this.manager = manager;
		this.soundId = soundId;
	}
	
	@Override
	public long play() {
		return play(1);
	}

	@Override
	public long loop() {
		return loop(1);
	}

	@Override
	public long play(float volume) {
		if (streamIds.size() == 8) {
			streamIds.pop();
		}
		int streamId = soundPool.play(soundId, volume, volume, 1, 0, 1);
		// standardise error code with other backends
		if (streamId == 0) {
			return -1;
		}
		streamIds.add(0, streamId);
		
		return streamId;
	}

	@Override
	public long loop(float volume) {
		if (streamIds.size() == 8) {
			streamIds.pop();
		}
		int streamId = soundPool.play(soundId, volume, volume, 1, -1, 1);
		// standardise error code with other backends
		if (streamId == 0) {
			return -1;
		}
		streamIds.add(0, streamId);
		
		return streamId;
	}

	@Override
	public void pause() {
		for (int streamId : streamIds) {
			soundPool.pause(streamId);
		}
	}

	@Override
	public void resume() {
		for (int streamId : streamIds) {
			soundPool.resume(streamId);
		}
	}

	@Override
	public void stop() {
		for (int streamId : streamIds) {
			soundPool.stop(streamId);
		}
	}

}
