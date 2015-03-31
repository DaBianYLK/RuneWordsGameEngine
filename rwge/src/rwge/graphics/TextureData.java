package rwge.graphics;

import rwge.memory.Disposable;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import android.animation.IntArrayEvaluator;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

/** The texture data's format is RGBA_8888. */

public class TextureData implements Disposable {
	static {
		System.loadLibrary("rwge");
	}
	
	public static void main(String[] args) {
		int[] test = new int[100];
		for (int i = 0; i < 100; ++i) {
			test[i] = 2048;
		}
		
		ByteBuffer buffer = intArraytoByteBuffer(test, 100);
		
		System.out.println(buffer.toString());
		
		//System.out.println(System.getProperty("java.library.path"));
	}
	
	
	private int width;
	private int height;
	private ByteBuffer pixelBuffer;
	
	public TextureData(Bitmap bitmap) {
		width = bitmap.getWidth();
		height = bitmap.getHeight();

		int[] pixels = new int[width * height];
		bitmap.getPixels(pixels, 0, width, 0, 0, width, height);

	}
	
	public TextureData(String pathName) {
		//Bitmap bitmap = BitmapFactory.decodeFile(pathName);
	}
	
	public ByteBuffer getPixelBuffer() {
		return pixelBuffer;
	}
	
	public int getWidth() {
		return width;
	}
	
	public int getHeight() {
		return height;
	}
	
//	public int getPixel(int x, int y) {
//		return bitmap.getPixel(x, y);
//	}
	
	public void getPixels(int[] pixels, int width, int height) {
		
	}
	
	public static native ByteBuffer intArraytoByteBuffer(int[] array, int length);
	
	@Override
	public void dispose() {
//		if (bitmap != null && !bitmap.isRecycled()) {
//			bitmap.recycle();
//		}
	}
}
