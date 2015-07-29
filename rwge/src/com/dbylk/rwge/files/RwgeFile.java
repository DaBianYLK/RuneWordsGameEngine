package com.dbylk.rwge.files;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import android.util.Log;

import com.dbylk.rwge.utils.StreamUtil;
import com.dbylk.rwge.Files.PathType;
import com.dbylk.rwge.Rwge;

/** Provides method to access files. */

public class RwgeFile {
	protected File file;
	protected PathType pathType;

	public RwgeFile(String path) {
		file = new File(path);
		pathType = PathType.Absolute;
	}

	public RwgeFile(File file) {
		this.file = file;
		pathType = PathType.Absolute;
	}

	public RwgeFile(String path, PathType type) {
		file = new File(path);
		
		if (!file.exists() && type == PathType.External) {
			path = Rwge.files.getExternalPath() + path;
			
			file = new File(path);
		}
		
		pathType = type;
	}

	public String getPath() {
		return file.getPath().replace('\\', '/');
	}

	public String getName() {
		return file.getName();
	}

	public PathType getPathType() {
		return pathType;
	}

	public File getFile() {
		return file;
	}

	public String readString() {
		StringBuilder output = new StringBuilder(estimateLength());
		InputStreamReader reader = null;
		try {
			reader = new InputStreamReader(read());
			char[] buffer = new char[256];
			while (true) {
				int length = reader.read(buffer);
				if (length == -1) {
					break;
				}
				output.append(buffer, 0, length);
			}
		} 
		catch (IOException ex) {
			throw new RuntimeException("Error reading file: " + this, ex);
		} 
		finally {
			StreamUtil.closeQuietly(reader);
		}
		
		return output.toString();
	}

	public byte[] readBytes() {
		InputStream input = read();
		try {
			return StreamUtil.copyStreamToByteArray(input, estimateLength());
		} 
		catch (IOException ex) {
			throw new RuntimeException("Error reading file: " + this, ex);
		} 
		finally {
			StreamUtil.closeQuietly(input);
		}
	}

	public long getLength() {
		if (pathType == PathType.Internal && !file.exists()) {
			InputStream input = read();
			try {
				return input.available();
			} 
			catch (Exception ignored) {
			} 
			finally {
				StreamUtil.closeQuietly(input);
			}
			
			return 0;
		}

		return file.length();
	}

	/** Return a nonzero number when the file is empty. */
	protected int estimateLength() {
		int length = (int) getLength();

		return length == 0 ? 512 : length;
	}

	public InputStream read() {
		// If path type is internal or local, and File class can not open it,
		// then try to find it by "getResourceAsStream" method.
		if (pathType == PathType.Classpath || (pathType == PathType.Internal && !file.exists()) || (pathType == PathType.Local && !file.exists())) {
			InputStream input = RwgeFile.class.getResourceAsStream("/" + file.getPath().replace('\\', '/'));
			if (input == null) {
				throw new RuntimeException("File not found: " + file + " (" + pathType + ")");
			}
			return input;
		}

		try {
			return new FileInputStream(file);
		} 
		catch (Exception ex) {
			if (file.isDirectory()) {
				throw new RuntimeException("Cannot open a stream to a directory: " + file + " (" + pathType + ")", ex);
			}
			throw new RuntimeException("Error reading file: " + file + " (" + pathType + ")", ex);
		}
	}
}
