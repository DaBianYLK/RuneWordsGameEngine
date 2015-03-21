package rwge.math;

import java.io.Serializable;
import java.util.Map;

/** A 3D vector. */

public class Vector3 implements Serializable, Vector<Vector3>{
	public float x;
	public float y;
	public float z;
	
	public final static Vector3 X = new Vector3(1, 0, 0);
	public final static Vector3 Y = new Vector3(0, 1, 0);
	public final static Vector3 Z = new Vector3(0, 0, 1);
	public final static Vector3 Zero = new Vector3(0, 0, 0);
	
	
	Vector3() {
		this.set(Zero);
	}
	
	Vector3(final Vector3 vector) {
		this.set(vector);
	}
	
	Vector3(float x, float y, float z) {
		this.set(x, y, z);
	}
	
	Vector3(final float[] coordinates) {
		this.set(coordinates);
	}
	
	/** Calculate and return the norm of the vector. */
	public float mod() {
		return (float)Math.sqrt(x * x + y * y + z * z);
	}
	
	/** Calculate and return the square of the norm while calculating this is faster than calculating the norm. */
	public float mod2() {
		return x * x + y * y + z * z;
	}
	
	public Vector3 set(Vector3 v) {
		this.x = v.x;
		this.y = v.y;
		this.z = v.z;
		
		return this;
	}

	public Vector3 set(float x, float y, float z) {
		this.x = x;
		this.y = y;
		this.z = z;
		
		return this;
	}
	
	public Vector3 set(final float[] coordinates) {
		this.x = coordinates[0];
		this.y = coordinates[1];
		this.z = coordinates[2];
		
		return this;
	}
	
	public Vector3 add(Vector3 v) {
		return this.set(this.x + v.x, this.y + v.y, this.z + v.z);
	}
	
	public Vector3 sub(Vector3 v) {
		return this.set(this.x - v.x, this.y - v.y, this.z - v.z);
	}
	
	public float dot(Vector3 v) {
		return this.x * v.x + this.y * v.y + this.z * v.z;
	}
	
	public Vector3 crs(Vector3 v) {
		
	}
	
	public Vector3 scl(float scalar) {
		this.x = this.x * scalar;
		this.y = this.y * scalar;
		this.z = this.z * scalar;
		
		return this;
	}
}
