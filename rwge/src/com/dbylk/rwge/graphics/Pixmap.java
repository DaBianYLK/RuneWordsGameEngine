package com.dbylk.rwge.graphics;

import com.dbylk.rwge.memory.Disposable;

import java.nio.ByteBuffer;

import android.graphics.Bitmap;

public class Pixmap implements Disposable {
	private long pixelPtr;
	private int width;
	private int height;
	private Format format;
	private ByteBuffer pixelBuffer;
	
	public Pixmap(Bitmap bitmap) {
		bitmap.copyPixelsToBuffer(pixelBuffer);
		width = bitmap.getWidth();
		height = bitmap.getHeight();
	}
	
	@Override
	public void dispose() {
		
	}

	public enum Format {
		RGBA_8888,
		RGBA_4444,
		RGB_888,
		GRAY_8
	}
}
