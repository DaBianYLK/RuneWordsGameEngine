package com.dbylk.rwge;

import com.dbylk.rwge.audio.Music;
import com.dbylk.rwge.audio.Sound;
import com.dbylk.rwge.files.RwgeFile;

/** Interface to access audio resources. */

public interface Audio {
	public Sound getSound(RwgeFile file);
	public Music getMusic(RwgeFile file);
}
