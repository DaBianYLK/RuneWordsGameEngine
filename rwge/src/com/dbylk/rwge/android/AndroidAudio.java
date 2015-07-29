package com.dbylk.rwge.android;

import java.io.IOException;
import java.util.ArrayList;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;

import com.dbylk.rwge.Audio;
import com.dbylk.rwge.Files.PathType;
import com.dbylk.rwge.android.utils.Debug;
import com.dbylk.rwge.audio.Music;
import com.dbylk.rwge.audio.Sound;
import com.dbylk.rwge.files.RwgeFile;

public class AndroidAudio implements Audio {
	private final SoundPool soundPool;
	private final AudioManager audioManager;
	private final ArrayList<AndroidMusic> musics = new ArrayList<AndroidMusic>();
	
	public AndroidAudio (AndroidApplication app, AndroidAppConfig config) {
		if (config.useAudio) {
			soundPool = new SoundPool(config.maxAudioStreams, AudioManager.STREAM_MUSIC, 100);
			audioManager = (AudioManager)app.getSystemService(Context.AUDIO_SERVICE);
			app.setVolumeControlStream(AudioManager.STREAM_MUSIC);
		} else {
			soundPool = null;
			audioManager = null;
		}
	}
	
	@Override
	public Sound getSound(RwgeFile file) {
		if (soundPool == null) {
			Debug.log("AndroidAudio", "Audio is not enabled.");
			return null;
		}
		AndroidFile androidFile = (AndroidFile)file;
		if (androidFile.getPathType() == PathType.Internal) {
			try {
				AssetFileDescriptor descriptor = androidFile.assetManager.openFd(androidFile.getPath());
				AndroidSound sound = new AndroidSound(soundPool, audioManager, soundPool.load(descriptor, 1));
				descriptor.close();
				return sound;
			} catch (IOException ex) {
				throw new RuntimeException("Error loading audio file: " + file
					+ "\nNote: Internal audio files must be placed in the assets directory.", ex);
			}
		} else {
			try {
				return new AndroidSound(soundPool, audioManager, soundPool.load(androidFile.getFile().getPath(), 1));
			} catch (Exception ex) {
				throw new RuntimeException("Error loading audio file: " + file, ex);
			}
		}
	}

	@Override
	public Music getMusic(RwgeFile file) {
		if (soundPool == null) {
			Debug.log("AndroidAudio", "Audio is not enabled.");
			return null;
		}
		AndroidFile androidFile = (AndroidFile)file;

		MediaPlayer mediaPlayer = new MediaPlayer();

		if (androidFile.getPathType() == PathType.Internal) {
			try {
				AssetFileDescriptor descriptor = androidFile.assetManager.openFd(androidFile.getPath());
				mediaPlayer.setDataSource(descriptor.getFileDescriptor(), descriptor.getStartOffset(), descriptor.getLength());
				descriptor.close();
				mediaPlayer.prepare();
				AndroidMusic music = new AndroidMusic(this, mediaPlayer);
				synchronized (musics) {
					musics.add(music);
				}
				return music;
			} catch (Exception ex) {
				throw new RuntimeException("Error loading audio file: " + file
					+ "\nNote: Internal audio files must be placed in the assets directory.", ex);
			}
		} else {
			try {
				mediaPlayer.setDataSource(androidFile.getFile().getPath());
				mediaPlayer.prepare();
				AndroidMusic music = new AndroidMusic(this, mediaPlayer);
				synchronized (musics) {
					musics.add(music);
				}
				return music;
			} catch (Exception ex) {
				throw new RuntimeException("Error loading audio file: " + file, ex);
			}
		}
	}

}
