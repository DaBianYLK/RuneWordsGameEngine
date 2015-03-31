package rwge.graphics;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import rwge.memory.Disposable;
import android.opengl.GLES20;
import android.opengl.GLUtils;

/** This class is used to manage the OpenGL texture. */

public class GLTexture implements Disposable {
	/** The target of this texture, used when binding the texture.
	 *  Usually it will be set to GL_TEXTURE_2D */
	public final int glTarget;
	protected int glHandle;
	/** Minimized Filter. */
	protected TextureFilter minFilter = TextureFilter.Nearest;
	/** Maximized Filter. */
	protected TextureFilter magFilter = TextureFilter.Nearest;
	protected TextureWrap uWrap = TextureWrap.ClampToEdge;
	protected TextureWrap vWrap = TextureWrap.ClampToEdge;

	public GLTexture(int glTarget) {
		this.glTarget = glTarget;
	}
	
	public GLTexture(int glTarget, int glHandle) {
		this.glTarget = glTarget;
		this.glHandle = glHandle;
	}
	
	public void bind() {
		GLES20.glBindTexture(glTarget, glHandle);
	}
	
	/** Choose one texture unit of multi-textures to bind. */
	public void bind(int texUnit) {
		GLES20.glActiveTexture(GLES20.GL_TEXTURE0 + texUnit);
		GLES20.glBindTexture(glTarget, glHandle);
	}
	
	public int getTextureHandle() {
		return glHandle;
	}
	
	/** Before set the filters, this method will bind this texture. */
	public void setFilter(TextureFilter minFilter, TextureFilter magFilter) {
		this.minFilter = minFilter;
		this.magFilter = magFilter;
		
		bind();
		
		GLES20.glTexParameterf(glTarget, GLES20.GL_TEXTURE_MIN_FILTER, minFilter.glEnum);
		GLES20.glTexParameterf(glTarget, GLES20.GL_TEXTURE_MAG_FILTER, magFilter.glEnum);
	}
	
	/** This method is supposed be called when you are sure that this texture has been bound. */
	public void setFilterWithoutBind(TextureFilter minFilter, TextureFilter magFilter) {
		setFilterWithoutBind(minFilter, magFilter, false);
	}
	
	/** This method is supposed be called when you are sure that this texture has been bound. 
	 * @param force True to always set the values, even if they are the same as the current values. */
	public void setFilterWithoutBind(TextureFilter minFilter, TextureFilter magFilter, boolean force) {
		if (minFilter != null && (force || this.minFilter != minFilter)) {
			GLES20.glTexParameterf(glTarget, GLES20.GL_TEXTURE_MIN_FILTER, minFilter.glEnum);
			this.minFilter = minFilter;
		}
		if (magFilter != null && (force || this.magFilter != magFilter)) {
			GLES20.glTexParameterf(glTarget, GLES20.GL_TEXTURE_MAG_FILTER, magFilter.glEnum);
			this.magFilter = magFilter;
		}
	}
	
	public TextureFilter getMinFilter() {
		return minFilter;
	}
	
	public TextureFilter getMagFilter() {
		return magFilter;
	}
	
	/** Sets the {@link TextureWrap} for this texture on the u and v axis. This will bind this texture. */
	public void setWrap(TextureWrap uWrap, TextureWrap vWrap) {
		this.uWrap = uWrap;
		this.vWrap = vWrap;
		
		bind();
		
		GLES20.glTexParameterf(glTarget, GLES20.GL_TEXTURE_WRAP_S, uWrap.getGLEnum());
		GLES20.glTexParameterf(glTarget, GLES20.GL_TEXTURE_WRAP_T, vWrap.getGLEnum());
	}
	
	/** This method is supposed be called when you are sure that this texture has been bound.  */
	public void setWrapWithoutBind(TextureWrap uWrap, TextureWrap vWrap) {
		setWrapWithoutBind(uWrap, vWrap, false);
	}

	/** This method is supposed be called when you are sure that this texture has been bound.
	 * @param force True to always set the values, even if they are the same as the current values. */
	public void setWrapWithoutBind(TextureWrap uWrap, TextureWrap vWrap, boolean force) {
		if (uWrap != null && (force || this.uWrap != uWrap)) {
			GLES20.glTexParameterf(glTarget, GLES20.GL_TEXTURE_WRAP_S, uWrap.glEnum);
			this.uWrap = uWrap;
		}
		if (vWrap != null && (force || this.vWrap != vWrap)) {
			GLES20.glTexParameterf(glTarget, GLES20.GL_TEXTURE_WRAP_T, vWrap.glEnum);
			this.vWrap = vWrap;
		}
	}
	
	/** @return The {@link Texture.TextureWrap} used for horizontal (U) texture coordinates. */
	public TextureWrap getUWrap() {
		return uWrap;
	}

	/** @return The {@link Texture.TextureWrap} used for vertical (V) texture coordinates. */
	public TextureWrap getVWrap() {
		return vWrap;
	}
	
	/** Destroys the OpenGL Texture as specified by the glHandle. */
	protected void delete() {
		if (glHandle != 0) {
			int[] temp = new int[1];
			temp[0] = glHandle;
			GLES20.glDeleteBuffers(1, IntBuffer.wrap(temp));
			glHandle = 0;
		}
	}
	
	@Override
	public void dispose() {
		delete();
	}

	/** A Filter is used to define the mode of texture filtration. */
	public enum TextureFilter {
		Nearest(GLES20.GL_NEAREST), 
		Linear(GLES20.GL_LINEAR), 
		MipMap(GLES20.GL_LINEAR_MIPMAP_LINEAR), 
		MipMapNearestNearest(GLES20.GL_NEAREST_MIPMAP_NEAREST), 
		MipMapLinearNearest(GLES20.GL_LINEAR_MIPMAP_NEAREST), 
		MipMapNearestLinear(GLES20.GL_NEAREST_MIPMAP_LINEAR), 
		MipMapLinearLinear(GLES20.GL_LINEAR_MIPMAP_LINEAR);

		public final int glEnum;

		TextureFilter(int glEnum) {
			this.glEnum = glEnum;
		}

		public boolean isMipMap() {
			return glEnum != GLES20.GL_NEAREST && glEnum != GLES20.GL_LINEAR;
		}

		public int getGLEnum() {
			return glEnum;
		}
	}

	/**
	 * Wrap is used to define how the texture wrap a mesh when the texture
	 * coordinate is out of [0f, 1f].
	 */
	public enum TextureWrap {
		MirroredRepeat(GLES20.GL_MIRRORED_REPEAT), 
		ClampToEdge(GLES20.GL_CLAMP_TO_EDGE), 
		Repeat(GLES20.GL_REPEAT);

		final int glEnum;

		TextureWrap(int glEnum) {
			this.glEnum = glEnum;
		}

		public int getGLEnum() {
			return glEnum;
		}
	}
}
