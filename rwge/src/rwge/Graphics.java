package rwge;

/** Interface to access graphics devices. */

public interface Graphics {
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
		public final int r, g, b, a;
		/** number of bits for depth and stencil buffer */
		public final int depth, stencil;
		/** number of samples for multi-sample anti-aliasing (MSAA) */
		public final int samples;
		/** whether coverage sampling anti-aliasing is used. in that case you have to clear the coverage buffer as well! */
		public final boolean coverageSampling;

		public BufferFormat (int r, int g, int b, int a, int depth, int stencil, int samples, boolean coverageSampling) {
			this.r = r;
			this.g = g;
			this.b = b;
			this.a = a;
			this.depth = depth;
			this.stencil = stencil;
			this.samples = samples;
			this.coverageSampling = coverageSampling;
		}

		public String toString () {
			return "r: " + r + ", g: " + g + ", b: " + b + ", a: " + a + ", depth: " + depth + ", stencil: " + stencil
				+ ", num samples: " + samples + ", coverage sampling: " + coverageSampling;
		}
	}
	
	
}
