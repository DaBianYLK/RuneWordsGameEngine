package com.dbylk.rwge.graphics;

import android.util.Log;

import com.dbylk.rwge.math.Angle;
import com.dbylk.rwge.math.Radian;
import com.dbylk.rwge.math.Transform2D;
import com.dbylk.rwge.math.Vector2;

/**
 * The basic render unit in RWGE. 
 * It is rectangular and its position (x, y) is the center of this sprite, 
 * around which rotations and scaling are performed.
 * 
 * There is a corner point located in the bottom left corner of that rectangle. 
 * The corner's coordinate is based on its position (For example, there is a sprite with
 * position (x, y) and corner (a, b), then the cooner's real coordinate is (x+a, y+b).
 */

public class Sprite extends SceneLayer {
	protected TextureRect textureRect;
	// a / r / g / b
	protected float[] color = new float[4];
	
	protected float width;
	protected float height;
	protected float cornerX;
	protected float cornerY;
	
	/** It is not allowed to create an empty sprite. */
	private Sprite() { }
	
	public Sprite(Sprite sprite) {
		set(sprite);
	}
	
	public Sprite(TextureRect textureRect, float x, float y, float width, float height) {
		this.textureRect = textureRect;
		
		int len = this.color.length;
		for (int i = 0; i < len; i++) {
			this.color[i] = 1f;
		}
		
		setBounds(x, y, width, height);
	}
	
	public Sprite(Texture texture, float x, float y, float width, float height) {
		this(new TextureRect(texture), x, y, width, height);
	}
	
	public void bind() {
		textureRect.bind();
	}
	
	public void unbind() {
		textureRect.unbind();
	}
	
	public void set(Sprite sprite) {
		super.set(sprite);
		
		this.textureRect = sprite.textureRect;
		
		int len = this.color.length;
		for (int i = 0; i < len; i++) {
			this.color[i] = sprite.color[i];
		}
		
		this.width = sprite.width;
		this.height = sprite.height;
		this.cornerX = sprite.cornerX;
		this.cornerY = sprite.cornerY;
	}
	
	public void setColor(float[] color) {
		int len = this.color.length;
		
		if (color.length != len) {
			Log.w("Sprite", "The color component number is not matched.");
		}
		
		for (int i = 0; i < len; i++) {
			this.color[i] = color[i];
		}
	}
	
	public void setSize(float width, float height) {
		this.width = width;
		this.height = height;
	}
	
	public void setBounds(float x, float y, float width, float height) {
		setPosition(x, y);
		
		this.width = width;
		this.height = height;
	}
	
	@Override
	public Sprite scale(float s) {
		super.scale(s);
		
		return this;
	}

	@Override
	public Sprite scale(float x, float y) {
		super.scale(x, y);
		
		return this;
	}

	@Override
	public Sprite scale(Vector2 vector) {
		super.scale(vector);
		
		return this;
	}

	@Override
	public Sprite translate(float x, float y) {
		super.translate(x, y);
		
		return this;
	}
	
	@Override
	public Sprite translate(Vector2 vector) {
		super.translate(vector);
		
		return this;
	}
	
	@Override
	public Sprite rotate(float radian) {
		super.rotate(radian);
		
		return this;
	}
	
	@Override
	public Sprite rotate(Radian radian) {
		super.rotate(radian);
		
		return this;
	}

	/** This method is not suggested to be used, while radian is used during trigonometric calculation. */
	@Override
	public Sprite rotate(Angle angle) {
		super.rotate(angle);
		
		return this;
	}
}
