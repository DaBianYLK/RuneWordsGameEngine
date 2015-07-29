package com.dbylk.rwge;

import com.dbylk.rwge.files.RwgeFile;

/** Interface to access file system. */

public interface Files {
	public RwgeFile getFile(String path, PathType type);
	
	public RwgeFile getInternalFile(String path);
	
	public RwgeFile getExternalFile(String path);
	
	public RwgeFile getLocalFile(String path);
	
	public RwgeFile getAbsoluteFile(String path);
	
	/** Returns the external storage path directory. This is the SD card on Android. */
	public String getExternalPath ();

	/** Returns the local storage path directory. This is the private files directory on Android. */
	public String getLocalPath ();
	
	public enum PathType {
		/** Path relative to the project's source file. */
		Classpath,
		
		/** Path relative to the asset directory on Android. */
		Internal,
		
		/** Path relative to SD card path on Android. */
		External,
		
		/** Path relative to root path of application. */
		Local,
		
		/** Path relative to absolute path on operating system. */
		Absolute;
	}
}
