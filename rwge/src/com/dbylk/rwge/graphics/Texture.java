package com.dbylk.rwge.graphics;

import java.nio.IntBuffer;

import com.dbylk.rwge.memory.Disposable;

import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.Log;

/** This class is used to manage the OpenGL texture. */

public class Texture implements Disposable {
	/** The target of this texture, used when binding the texture.
	 *  Usually it will be set to GL_TEXTURE_2D */
	public static final int glTarget = GLES20.GL_TEXTURE_2D;
	
	protected final int textureHandle;
	/** Minimized Filter. */
	protected TextureFilter minFilter = TextureFilter.Nearest;
	/** Maximized Filter. */
	protected TextureFilter magFilter = TextureFilter.Nearest;
	protected TextureWrap uWrap = TextureWrap.ClampToEdge;
	protected TextureWrap vWrap = TextureWrap.ClampToEdge;
	
	protected TextureData data;

	public Texture() {
		int[] tempHandle = new int[1];
		GLES20.glGenTextures(1, tempHandle, 0);
		textureHandle = tempHandle[0];
	}
	
	public Texture(TextureData data) {
		this();
		
		load(data);
	}
	
	public Texture(Bitmap bitmap) {
		this(new TextureData(bitmap));
	}
	
	public Texture(String pathName) {
		this(new TextureData(pathName));
	}
	
	public void load(TextureData data) {
		this.data = data;
		
		if (data != null) {
			bind();
			
			GLES20.glPixelStorei(GLES20.GL_UNPACK_ALIGNMENT, 4);
			GLUtils.texImage2D(glTarget, 0, GLES20.GL_RGBA, data.getBitmap(), GLES20.GL_UNSIGNED_BYTE, 0);
			
			setFilterWithoutBind(TextureFilter.Nearest, TextureFilter.Nearest, true);
			setWrapWithoutBind(TextureWrap.Repeat, TextureWrap.Repeat, true);
		} 
		else {
			Log.e("Render", "Texture " + textureHandle + " loaded failed.");
		}
	}
	
	public void bind() {
		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(glTarget, textureHandle);
	}
	
	public void unbind() {
		GLES20.glBindTexture(glTarget, 0);
	}
	
	/** Choose one texture unit of multi-textures to bind. */
	public void bind(int texUnit) {
		GLES20.glActiveTexture(GLES20.GL_TEXTURE0 + texUnit);
		GLES20.glBindTexture(glTarget, textureHandle);
	}
	
	public int getWidth() {
		return data.getWidth();
	}
	
	public int getHeight() {
		return data.getHeight();
	}
	
	public int getTextureHandle() {
		return textureHandle;
	}
	
	/** Before set the filters, this method will bind this texture. */
	public void setFilter(TextureFilter minFilter, TextureFilter magFilter) {
		bind();
		
		setFilterWithoutBind(minFilter, magFilter);
	}
	
	/** This method is supposed be called when you are sure that this texture has been bound. */
	public void setFilterWithoutBind(TextureFilter minFilter, TextureFilter magFilter) {
		setFilterWithoutBind(minFilter, magFilter, false);
	}
	
	/** This method is supposed be called when you are sure that this texture has been bound. 
	 * @param force True to always set the values, even if they are the same as the current values. */
	public void setFilterWithoutBind(TextureFilter minFilter, TextureFilter magFilter, boolean force) {
		if (minFilter != null && (force || this.minFilter.glEnum != minFilter.glEnum)) {
			GLES20.glTexParameterf(glTarget, GLES20.GL_TEXTURE_MIN_FILTER, minFilter.glEnum);
			this.minFilter = minFilter;
		}
		if (magFilter != null && (force || this.magFilter.glEnum != magFilter.glEnum)) {
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
		bind();
		
		setWrapWithoutBind(uWrap, vWrap);
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
	
	/** Destroys the OpenGL Texture specified by the glHandle. */
	@Override
	public void dispose() {
		if (textureHandle != 0) {
			int[] temp = new int[1];
			temp[0] = textureHandle;
			GLES20.glDeleteBuffers(1, IntBuffer.wrap(temp));
		}
		
		if (data != null) {
			data.dispose();
		}
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
