package rwge.math;

import java.io.Serializable;

/** A Matrix with 4*4 elements. 
 * The matrix elements are saved in an array, the layout of the array is as the following graph:
 * 		0	4	8	12
 * 		1	5	9	13
 * 		2	6	10	14
 * 		3	7	11	15
 * And the coordinates are as the follows:
 * 		M00	M01	M02	M03
 * 		M10	M11	M12	M13
 * 		M20	M21	M22	M23
 * 		M30	M31	M32	M33
 * */

public class Matrix4 implements Serializable {
	private static final long serialVersionUID = 7570678718655565027L;
	
	public static final int M00 = 0;
	public static final int M01 = 4;
	public static final int M02 = 8;
	public static final int M03 = 12;
	public static final int M10 = 1;
	public static final int M11 = 5;
	public static final int M12 = 9;
	public static final int M13 = 13;
	public static final int M20 = 2;
	public static final int M21 = 6;
	public static final int M22 = 10;
	public static final int M23 = 14;
	public static final int M30 = 3;
	public static final int M31 = 7;
	public static final int M32 = 11;
	public static final int M33 = 15;

	public final float values[] = new float[16];

	/** Constructs an identity matrix */
	public Matrix4() {
		values[M00] = 1f;
		values[M11] = 1f;
		values[M22] = 1f;
		values[M33] = 1f;
	}

	public Matrix4(Matrix4 matrix) {
		this.set(matrix);
	}

	/** Constructs a matrix from the given float array. The array must have at least 16 elements; the first 16 will be copied. 
	 * The number in the array are organized just as the graph on the top of this class. */
	public Matrix4(float[] values) {
		this.set(values);
	}

	public Matrix4 set(Matrix4 matrix) {
		return this.set(matrix.values);
	}

	public Matrix4 set(float[] values) {
		System.arraycopy(values, 0, this.values, 0, this.values.length);
		
		return this;
	}

	/** @return the backing float array */
	public float[] getValues() {
		return values;
	}
	
	public native void multiply(float[] matrixA, float[] matrixB);
}
