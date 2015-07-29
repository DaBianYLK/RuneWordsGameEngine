package com.dbylk.rwge.graphics;

import java.util.HashMap;

import android.util.Log;

import com.dbylk.rwge.math.Angle;
import com.dbylk.rwge.math.Radian;
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
	protected Color color = Color.White;
	
	protected HashMap<String, Animation> animations = new HashMap<String, Animation>();
	
	protected float width;
	protected float height;
	protected float cornerX;
	protected float cornerY;
	
	/** It is not allowed to create an empty sprite. */
	@SuppressWarnings("unused")
	private Sprite() { }
	
	public Sprite(Sprite sprite) {
		set(sprite);
	}
	
	public Sprite(TextureRect textureRect, float x, float y, float cornerX, float cornerY, float width, float height) {
		this.textureRect = textureRect;
		
		setPosition(x, y);
		setBounds(cornerX, cornerY, width, height);
	}
	
	public Sprite(Texture texture, float x, float y, float cornerX, float cornerY, float width, float height) {
		this(new TextureRect(texture), x, y, cornerX, cornerY, width, height);
	}
	
	public void bind() {
		textureRect.texture.bind();
	}
	
	public void unbind() {
		textureRect.texture.unbind();
	}
	
	public void set(Sprite sprite) {
		super.set(sprite);
		
		this.textureRect = sprite.textureRect;
		
		color.set(sprite.color);
		
		this.width = sprite.width;
		this.height = sprite.height;
		this.cornerX = sprite.cornerX;
		this.cornerY = sprite.cornerY;
	}
	
	public void setColor(Color color) {
		this.color.set(color);
	}
	
	public void setSize(float width, float height) {
		this.width = width;
		this.height = height;
	}
	
	/** A corner's coordinate is its bottom left corner's coordinate, and it is based on its position. */
	public void setBounds(float cornerX, float cornerY, float width, float height) {
		this.cornerX = cornerX;
		this.cornerY = cornerY;
		this.width = width;
		this.height = height;
	}
	
	public void horizontalFlip() {
		this.cornerX = -this.cornerX;
		this.width = -this.width;
	}
	
	public void verticalFlip() {
		this.cornerY = -this.cornerY;
		this.height = -this.height;
	}
	
	public void addAnimation(String animationName, Animation animation) {
		animation.sprite = this;
		animations.put(animationName, animation);
	}
	
	public Animation getAnimation(String animationName) {
		return animations.get(animationName);
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
