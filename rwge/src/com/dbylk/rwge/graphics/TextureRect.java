package com.dbylk.rwge.graphics;

/**
 * Defines a rectangular area of a texture. The coordinate system used has its
 * origin in the upper left corner with the x-axis pointing to the right and the
 * y axis pointing downwards. (Actually, OpenGL's texture coordinate has a
 * opposite y-axis direction.)
 */

public class TextureRect {
	protected Texture texture;
	
	/** The pixels number on x-axis. */;
	protected int rectWidth;
	/** The pixels number on y-axis. */
	protected int rectHeight;
	
	protected float topLeftU;
	protected float topLeftV;
	protected float bottomRightU = 1f;
	protected float bottomRightV = 1f;
	
	public TextureRect(Texture texture) {
		set(texture);
	}
	
	public TextureRect(Texture texture, int x, int y, int width, int height) {
		this.texture = texture;
		
		setRect(x, y, width, height);
	}
	
	public TextureRect(Texture texture, float topLeftU, float topLeftV, float bottomRightU, float bottomRightV) {
		this.texture = texture;
		
		setRect(topLeftU, topLeftV, bottomRightU, bottomRightV);
	}
	
	public TextureRect(TextureRect textureRect) {
		set(textureRect);
	}
	
	public void set(Texture texture) {
		this.texture = texture;
		rectWidth = texture.getWidth();
		rectHeight = texture.getHeight();
		
		topLeftU = 0f;
		topLeftV = 0f;
		bottomRightU = 1f;
		bottomRightV = 1f;
	}
	
	public void set(TextureRect textureRect) {
		this.texture = textureRect.texture;
		this.rectWidth = textureRect.rectWidth;
		this.rectHeight = textureRect.rectHeight;
		this.topLeftU = textureRect.topLeftU;
		this.topLeftV = textureRect.topLeftV;
		this.bottomRightU = textureRect.bottomRightU;
		this.bottomRightV = textureRect.bottomRightV;
	}
	
	public void setRect(int x, int y, int width, int height) {
		this.rectWidth = Math.abs(width);
		this.rectHeight = Math.abs(height);
		
		float textureWidthReciprocal = 1f / texture.getWidth();
		float textureHeightReciprocal = 1f / texture.getHeight();
		
		topLeftU = x * textureWidthReciprocal;
		topLeftV = y * textureHeightReciprocal;
		bottomRightU = (x + width) * textureWidthReciprocal;
		bottomRightV = (y + height) * textureHeightReciprocal;
	}
	
	public void setRect(float topLeftU, float topLeftV, float bottomRightU, float bottomRightV) {
		int textureWidth = texture.getWidth();
		int textureHeight = texture.getHeight();
		
		rectWidth = Math.round(Math.abs(bottomRightU - topLeftU) * textureWidth);
		rectHeight = Math.round(Math.abs(bottomRightV - topLeftV) * textureHeight);
		
		this.topLeftU = topLeftU;
		this.topLeftV = topLeftV;
		this.bottomRightU = bottomRightU;
		this.bottomRightV = bottomRightV;
	}
	
	public void bind() {
		texture.bind();
	}
	
	public void unbind() {
		texture.unbind();
	}
}
