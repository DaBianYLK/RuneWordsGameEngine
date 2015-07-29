package com.dbylk.rwge.math;

import java.io.Serializable;

/** A 4D vector. */

public class Vector4 implements Serializable, Vector<Vector4>{
	private static final long serialVersionUID = 7265591770156449753L;
	
	public float x;
	public float y;
	public float z;
	public float w;
	
	public static final Vector4 X = new Vector4(1, 0, 0, 0);
	public static final Vector4 Y = new Vector4(0, 1, 0, 0);
	public static final Vector4 Z = new Vector4(0, 0, 1, 0);
	public static final Vector4 W = new Vector4(0, 0, 0, 1);
	public static final Vector4 Zero = new Vector4(0, 0, 0, 0);
	
	
	public Vector4() {
		this.set(Zero);
	}
	
	public Vector4(final Vector4 vector) {
		this.set(vector);
	}
	
	public Vector4(float x, float y, float z, float w) {
		this.set(x, y, z, w);
	}
	
	public Vector4(final float[] coordinates) {
		this.set(coordinates);
	}
	
	public Vector4 set(Vector4 v) {
		this.x = v.x;
		this.y = v.y;
		this.z = v.z;
		
		return this;
	}

	public Vector4 set(float x, float y, float z, float w) {
		this.x = x;
		this.y = y;
		this.z = z;
		this.w = w;
		
		return this;
	}
	
	public Vector4 set(final float[] coordinates) {
		this.x = coordinates[0];
		this.y = coordinates[1];
		this.z = coordinates[2];
		this.w = coordinates[3];
		
		return this;
	}

	@Override
	public float mod() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public float mod2() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public Vector4 add(Vector4 v) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Vector4 sub(Vector4 v) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public float dot(Vector4 v) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public Vector4 scale(float s) {
		// TODO Auto-generated method stub
		return null;
	}
}