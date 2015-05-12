package com.dbylk.rwge;

import com.dbylk.rwge.graphics.SceneRenderer;

/** Interface to access graphics devices. */

public interface Graphics {
	public DisplayMode getDisplayMode();
	public boolean setDisplayMode(DisplayMode displayMode);
	
	public BufferFormat getBufferFormat();
	
	public int getWidth();
	public int getHeight();
	public int getRefreshRate();
	public int getBitPerPixel();
	
	public SceneRenderer getSceneRenderer();
	
	public class DisplayMode {
		public final int width;
		public final int height;
		public final int refreshRate;
		public final int bitsPerPixel;

		protected DisplayMode (int width, int height, int refreshRate, int bitsPerPixel) {
			this.width = width;
			this.height = height;
			this.refreshRate = refreshRate;
			this.bitsPerPixel = bitsPerPixel;
		}

		public String toString () {
			return width + "x" + height + ", bpp: " + bitsPerPixel + ", hz: " + refreshRate;
		}
	}
	
	public static class BufferFormat {
		/** number of bits per color channel */
		public final int redSize;
		public final int greenSize;
		public final int blueSize;
		public final int alphaSize;
		
		/** number of bits for depth and stencil buffer */
		public final int depthSize;
		public final int stencilSize;
		
		/** number of samples for multi-sample anti-aliasing (MSAA) */
		public final int sampleRate;
		/** whether coverage sampling anti-aliasing is used. in that case you have to clear the coverage buffer as well! */
		//public final boolean coverageSampling;

		public BufferFormat (int r, int g, int b, int a, int depth, int stencil, int samples) {
			this.redSize = r;
			this.greenSize = g;
			this.blueSize = b;
			this.alphaSize = a;
			this.depthSize = depth;
			this.stencilSize = stencil;
			this.sampleRate = samples;
		}

		public String toString () {
			return "r: " + redSize + ", g: " + greenSize + ", b: " + blueSize + ", a: " + alphaSize + ", depth: " + depthSize + ", stencil: " + stencilSize
				+ ", num samples: " + sampleRate;
		}
	}
}
