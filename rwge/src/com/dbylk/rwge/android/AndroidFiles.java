package com.dbylk.rwge.android;

import android.content.res.AssetManager;
import android.os.Environment;

import com.dbylk.rwge.Files;
import com.dbylk.rwge.files.RwgeFile;

public class AndroidFiles implements Files {
	protected AssetManager assetManager;
	protected final String sdCardPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/";
	
	public AndroidFiles(AssetManager assetManager) {
		this.assetManager = assetManager;
	}
	
	@Override
	public RwgeFile getFile(String path, PathType type) {
		return new RwgeFile(path, type);
	}

	@Override
	public RwgeFile getInternalFile(String path) {
		return new AndroidFile(assetManager, path, PathType.Internal);
	}

	@Override
	public RwgeFile getExternalFile(String path) {
		return new RwgeFile(path, PathType.External);
	}

	@Override
	public RwgeFile getLocalFile(String path) {
		return new RwgeFile(path, PathType.Local);
	}

	@Override
	public RwgeFile getAbsoluteFile(String path) {
		return new RwgeFile(path, PathType.Absolute);
	}

	/** Android's external path is sd card's path. */
	@Override
	public String getExternalPath() {
		return sdCardPath;
	}

	/** Android's local path is sd card's path. */
	@Override
	public String getLocalPath() {
		return sdCardPath;
	}

	public AndroidFile getAssetFile(String path) {
		return new AndroidFile(assetManager, path, PathType.Internal);
	}
}
