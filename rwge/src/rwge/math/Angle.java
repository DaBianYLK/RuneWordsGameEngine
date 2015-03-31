package rwge.math;

/** Euler Angle is ranged from 0 to 360. */

public class Angle {
	public float value;
	
	public Angle(float angle) {
		this.value = angle;
	}
	
	public Angle(Radian radian) {
		this.value = radian.value * 180f / (float) Math.PI;
	}
	
	/**
	 * The Standard Euler angle is represented by a float range from 0 to 360. If the value is not
	 * in this range, it will be transformed.
	 */
	public void correct() {
		if (value >= 360f) {
			int temp = (int) value;
			value -= temp;
			temp %= 360;
			value += temp;
		} else if (value < 0f) {
			int temp = (int) value;
			value -= temp;
			temp %= 360;
			value = value + temp + 360f;
		}
	}

	public String toString() {
		return "Euler Angle: " + value;
	}
}