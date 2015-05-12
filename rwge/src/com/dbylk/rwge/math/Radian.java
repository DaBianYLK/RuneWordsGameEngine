package com.dbylk.rwge.math;

/** Radian is used to represented angle which is ranged from 0 to 2*PI. */

public class Radian {
	public static final float TwoPI = 2 * (float) Math.PI;

	public float value;

	public Radian(float radian) {
		this.value = radian;
	}
	
	public Radian(Angle angle) {
		this.value = angle.value * (float) Math.PI / 180f;
	}

	/**
	 * The Standard radian is represented by a float range from 0 to 2*PI. If
	 * the value is not in this range, it will be transformed.
	 */
	public float correct() {
		while (value >= TwoPI) {
			value -= TwoPI;
		}
		while (value < 0f) {
			value += TwoPI;
		}
		
		return value;
	}
	
	public static float getRadianValue(Angle angle) {
		return angle.value * (float) Math.PI / 180f;
	}

	public String toString() {
		return "Radian: " + value;
	}
}