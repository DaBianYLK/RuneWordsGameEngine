package com.dbylk.rwge.graphics;

import android.util.Log;

/** 
 * Describe the color used in OpenGL ES. 
 * Contains four channel (r, g, b, a),
 * each channel range between [0.0f, 1.0f].
 *  */

public class Color {
	public static final Color Black = new Color(0f, 0f, 0f, 1f);
	public static final Color White = new Color(1f, 1f, 1f, 1f);
	public static final Color Red = new Color(1f, 0f, 0f, 1f);
	public static final Color Green = new Color(0f, 1f, 0f, 1f);
	public static final Color Blue = new Color(0f, 0f, 1f, 1f);
	public static final Color Gray = new Color(0.5f, 0.5f, 0.5f, 1f);
	public static final Color Yellow = new Color(1f, 1f, 0f, 1f);
	public static final Color Purple = new Color(1f, 0f, 1f, 1f);
	public static final Color Cyan = new Color(0f, 1f, 1f, 1f);
	
	public float r;
	public float g;
	public float b;
	public float a;
	
	public Color() {
		
	}
	
	public Color(float r, float g, float b, float a) {
		set(r, g, b, a);
	}
	
	public Color(Color color) {
		set(color);
	}
	
	public Color(float[] color) {
		set(color);
	}
	
	public Color set(float r, float g, float b, float a) {
		this.r = r;
		this.g = g;
		this.b = b;
		this.a = a;
		
		return clamp();
	}
	
	public Color set(float[] color) {
		if (color.length < 4) {
			Log.w("Sprite", "The color component number is not matched.");
			return null;
		}
		
		this.r = color[0];
		this.g = color[1];
		this.b = color[2];
		this.a = color[3];
		
		return clamp();
	}
	
	public Color set(Color color) {
		this.r = color.r;
		this.g = color.g;
		this.b = color.b;
		this.a = color.a;
		
		return this;
	}
	
	public Color clamp() {
		if (r < 0f) {
			r = 0f;
		}
		else if (r > 1f) {
			r = 1f;
		}
		
		if (g < 0f) {
			g = 0f;
		}
		else if (g > 1f) {
			g = 1f;
		}
		
		if (b < 0f) {
			b = 0f;
		}
		else if (b > 1f) {
			b = 1f;
		}
		
		if (a < 0f) {
			a = 0f;
		}
		else if (a > 1f) {
			a = 1f;
		}
		
		return this;
	}
}
