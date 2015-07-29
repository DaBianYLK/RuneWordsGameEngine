package com.dbylk.rwge.android;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;

import com.dbylk.rwge.Files.PathType;
import com.dbylk.rwge.Rwge;
import com.dbylk.rwge.files.RwgeFile;

public class AndroidFile extends RwgeFile {
	protected final AssetManager assetManager;

	public AndroidFile(AssetManager assetManager, String path, PathType type) {
		super(path.replace('\\', '/'), type);

		this.assetManager = assetManager;
	}
	
	public File getFile () {
		if (pathType == PathType.Local) {
			return new File(Rwge.files.getLocalPath(), file.getPath());
		}
		
		return super.getFile();
	}

	public long getLength() {
		if (pathType == PathType.Internal) {
			AssetFileDescriptor fileDescriptor = null;
			try {
				fileDescriptor = assetManager.openFd(file.getPath());
				return fileDescriptor.getLength();
			} catch (IOException ignored) {
			} finally {
				if (fileDescriptor != null) {
					try {
						fileDescriptor.close();
					} 
					catch (IOException e) {
					}
				}
			}
		}

		return super.getLength();
	}

	public InputStream read() {
		if (pathType == PathType.Internal) {
			try {
				return assetManager.open(file.getPath());
			} catch (IOException ex) {
				throw new RuntimeException("Error reading file: " + file + " (" + pathType + ")", ex);
			}
		}
		
		return super.read();
	}
}
