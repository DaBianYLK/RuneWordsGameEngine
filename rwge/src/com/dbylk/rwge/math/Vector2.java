package com.dbylk.rwge.math;

import java.io.Serializable;

/** A 2D vector. */

public class Vector2 implements Serializable, Vector<Vector2> {
	private static final long serialVersionUID = -2666239485448854614L;
	
	public static final Vector2 X = new Vector2(1, 0);
	public static final Vector2 Y = new Vector2(0, 1);
	public static final Vector2 Zero = new Vector2(0, 0);
	
	public float x;
	public float y;
	
	public Vector2() {
		this.set(Zero);
	}
	
	public Vector2(final Vector2 vector) {
		this.set(vector);
	}
	
	public Vector2(float x, float y) {
		this.set(x, y);
	}
	
	public Vector2(final float[] coordinates) {
		this.set(coordinates);
	}
	
	/** Calculate and return the norm of the vector. */
	public float mod() {
		return (float)Math.sqrt(x * x + y * y);
	}
	
	/** Calculate and return the square of the norm while calculating this is faster than calculating the norm. */
	public float mod2() {
		return x * x + y * y;
	}
	
	public Vector2 set(Vector2 v) {
		this.x = v.x;
		this.y = v.y;
		
		return this;
	}

	public Vector2 set(float x, float y) {
		this.x = x;
		this.y = y;
		
		return this;
	}
	
	public Vector2 set(final float[] coordinates) {
		this.x = coordinates[0];
		this.y = coordinates[1];
		
		return this;
	}
	
	public Vector2 add(Vector2 v) {
		return this.set(this.x + v.x, this.y + v.y);
	}
	
	public Vector2 sub(Vector2 v) {
		return this.set(this.x - v.x, this.y - v.y);
	}
	
	public float dot(Vector2 v) {
		return this.x * v.x + this.y * v.y;
	}
	
	public float cross(Vector2 v) {
		return this.x * v.y - v.x * this.y;
	}
	
	public Vector2 scale(float s) {
		this.x *= s;
		this.y *= s;
		
		return this;
	}
}
