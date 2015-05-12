package com.dbylk.rwge.graphics;

import com.dbylk.rwge.memory.Disposable;

import java.nio.IntBuffer;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

/** The texture data is based on Android Class "Bitmap". 
 * The data format is RGBA_8888. */

public class TextureData implements Disposable {
	private Bitmap bitmap;
	
	public TextureData(Bitmap bitmap) {
		this.bitmap = bitmap;
	}
	
	public TextureData(String pathName) {
		this.bitmap = BitmapFactory.decodeFile(pathName);
	}
	
	public IntBuffer getPixelBuffer() {
		return IntBuffer.wrap(getPixels());
	}
	
	public Bitmap getBitmap() {
		return bitmap;
	}
	
	public int getWidth() {
		return bitmap.getWidth();
	}
	
	public int getHeight() {
		return bitmap.getHeight();
	}
	
	public int getPixel(int x, int y) {
		return bitmap.getPixel(x, y);
	}
	
	public int[] getPixels() {
		int width = bitmap.getWidth();
		int height = bitmap.getHeight();

		int[] pixels = new int[width * height];
		bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
		
		return pixels;
	}
	
	@Override
	public void dispose() {
//		if (bitmap != null && !bitmap.isRecycled()) {
//			bitmap.recycle();
//		}
		
		bitmap = null;
	}
}
