package com.dbylk.rwge.math;

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
 * 
 * Transform formula is Matrix * oldVector = newVector.
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

	/** @return the float array */
	public float[] getValues() {
		return values;
	}
	
	/** Scale the matrix in three dimensions. */
	public Matrix4 scale(Vector3 vector) {
		values[M00] *= vector.x;
		values[M11] *= vector.y;
		values[M22] *= vector.z;
		
		return this;
	}
	
	/** Scale the matrix in three dimensions. */
	public Matrix4 scale(float x, float y, float z) {
		values[M00] *= x;
		values[M11] *= y;
		values[M22] *= z;
		
		return this;
	}
	
	/** Scale the matrix in three dimensions. */
	public Matrix4 scale(float s) {
		values[M00] *= s;
		values[M11] *= s;
		values[M22] *= s;
		
		return this;
	}
	
	public Matrix4 translate(Vector3 vector) {
		values[M03] += vector.x;
		values[M13] += vector.y;
		values[M23] += vector.z;
		
		return this;
	}
	
	public Matrix4 translate(float x, float y, float z) {
		values[M03] += x;
		values[M13] += y;
		values[M23] += z;
		
		return this;
	}
	
	/** The result is saved in matrixA. */
	public native void multiply(float[] matrixA, float[] matrixB);
	
	/** 
	 * Transform formula is Matrix * oldVector = newVector. 
	 * The result is saved in vector. 
	 * */
	public native void multiplyVector(float[] matrix, float[] vector);
	
	/** @return The determinant of this matrix */
	public float getDeterminant() {
		return values[M30] * values[M21] * values[M12] * values[M03] - values[M20] * values[M31] * values[M12] * values[M03] 
			 - values[M30] * values[M11] * values[M22] * values[M03] + values[M10] * values[M31] * values[M22] * values[M03] 
			 + values[M20] * values[M11] * values[M32] * values[M03] - values[M10] * values[M21] * values[M32] * values[M03]
			 - values[M30] * values[M21] * values[M02] * values[M13] + values[M20] * values[M31] * values[M02] * values[M13]
			 + values[M30] * values[M01] * values[M22] * values[M13] - values[M00] * values[M31] * values[M22] * values[M13]
			 - values[M20] * values[M01] * values[M32] * values[M13] + values[M00] * values[M21] * values[M32] * values[M13] 
			 + values[M30] * values[M11] * values[M02] * values[M23] - values[M10] * values[M31] * values[M02] * values[M23]
			 - values[M30] * values[M01] * values[M12] * values[M23] + values[M00] * values[M31] * values[M12] * values[M23] 
			 + values[M10] * values[M01] * values[M32] * values[M23] - values[M00] * values[M11] * values[M32] * values[M23] 
			 - values[M20] * values[M11] * values[M02] * values[M33] + values[M10] * values[M21] * values[M02] * values[M33] 
			 + values[M20] * values[M01] * values[M12] * values[M33] - values[M00] * values[M21] * values[M12] * values[M33]
			 - values[M10] * values[M01] * values[M22] * values[M33] + values[M00] * values[M11] * values[M22] * values[M33];
	}
	
	/** 
	 * Inverts the matrix. Stores the result in this matrix.
	 * 
	 * @return This matrix for the purpose of chaining methods together.
	 * @throws RuntimeException if the matrix is singular (not invertible)
	 *  */
	public Matrix4 inverse() {
		float determinant = getDeterminant();
		
		if (determinant == 0f) {
			throw new RuntimeException("non-invertible matrix");
		}
		
		float inverseDeterminant = 1.0f / determinant;
		
		float[] temp = new float[16];
		temp[M00] = values[M12] * values[M23] * values[M31] - values[M13] * values[M22] * values[M31]
				  + values[M13] * values[M21] * values[M32] - values[M11] * values[M23] * values[M32] 
				  - values[M12] * values[M21] * values[M33] + values[M11] * values[M22] * values[M33];
		
		temp[M01] = values[M03] * values[M22] * values[M31] - values[M02] * values[M23] * values[M31] 
				  - values[M03] * values[M21] * values[M32] + values[M01] * values[M23] * values[M32]
				  + values[M02] * values[M21] * values[M33] - values[M01] * values[M22] * values[M33];
		
		temp[M02] = values[M02] * values[M13] * values[M31] - values[M03] * values[M12] * values[M31]
				  + values[M03] * values[M11] * values[M32] - values[M01] * values[M13] * values[M32] 
				  - values[M02] * values[M11] * values[M33] + values[M01] * values[M12] * values[M33];
		
		temp[M03] = values[M03] * values[M12] * values[M21] - values[M02] * values[M13] * values[M21] 
				  - values[M03] * values[M11] * values[M22] + values[M01] * values[M13] * values[M22] 
				  + values[M02] * values[M11] * values[M23] - values[M01] * values[M12] * values[M23];
		
		temp[M10] = values[M13] * values[M22] * values[M30] - values[M12] * values[M23] * values[M30] 
				  - values[M13] * values[M20] * values[M32] + values[M10] * values[M23] * values[M32]
				  + values[M12] * values[M20] * values[M33] - values[M10] * values[M22] * values[M33];
		
		temp[M11] = values[M02] * values[M23] * values[M30] - values[M03] * values[M22] * values[M30] 
				  + values[M03] * values[M20] * values[M32] - values[M00] * values[M23] * values[M32] 
				  - values[M02] * values[M20] * values[M33] + values[M00] * values[M22] * values[M33];
		
		temp[M12] = values[M03] * values[M12] * values[M30] - values[M02] * values[M13] * values[M30] 
				  - values[M03] * values[M10] * values[M32] + values[M00] * values[M13] * values[M32] 
				  + values[M02] * values[M10] * values[M33] - values[M00] * values[M12] * values[M33];
		
		temp[M13] = values[M02] * values[M13] * values[M20] - values[M03] * values[M12] * values[M20] 
				  + values[M03] * values[M10] * values[M22] - values[M00] * values[M13] * values[M22] 
				  - values[M02] * values[M10] * values[M23] + values[M00] * values[M12] * values[M23];
		
		temp[M20] = values[M11] * values[M23] * values[M30] - values[M13] * values[M21] * values[M30] 
				  + values[M13] * values[M20] * values[M31] - values[M10] * values[M23] * values[M31] 
				  - values[M11] * values[M20] * values[M33] + values[M10] * values[M21] * values[M33];
		
		temp[M21] = values[M03] * values[M21] * values[M30] - values[M01] * values[M23] * values[M30] 
				  - values[M03] * values[M20] * values[M31] + values[M00] * values[M23] * values[M31] 
				  + values[M01] * values[M20] * values[M33] - values[M00] * values[M21] * values[M33];
		
		temp[M22] = values[M01] * values[M13] * values[M30] - values[M03] * values[M11] * values[M30] 
				  + values[M03] * values[M10] * values[M31] - values[M00] * values[M13] * values[M31] 
				  - values[M01] * values[M10] * values[M33] + values[M00] * values[M11] * values[M33];
		
		temp[M23] = values[M03] * values[M11] * values[M20] - values[M01] * values[M13] * values[M20] 
				  - values[M03] * values[M10] * values[M21] + values[M00] * values[M13] * values[M21] 
				  + values[M01] * values[M10] * values[M23] - values[M00] * values[M11] * values[M23];
		
		temp[M30] = values[M12] * values[M21] * values[M30] - values[M11] * values[M22] * values[M30] 
				  - values[M12] * values[M20] * values[M31] + values[M10] * values[M22] * values[M31] 
				  + values[M11] * values[M20] * values[M32] - values[M10] * values[M21] * values[M32];
		
		temp[M31] = values[M01] * values[M22] * values[M30] - values[M02] * values[M21] * values[M30] 
				  + values[M02] * values[M20] * values[M31] - values[M00] * values[M22] * values[M31] 
				  - values[M01] * values[M20] * values[M32] + values[M00] * values[M21] * values[M32];
		
		temp[M32] = values[M02] * values[M11] * values[M30] - values[M01] * values[M12] * values[M30]
				  - values[M02] * values[M10] * values[M31] + values[M00] * values[M12] * values[M31] 
				  + values[M01] * values[M10] * values[M32] - values[M00] * values[M11] * values[M32];
		
		temp[M33] = values[M01] * values[M12] * values[M20] - values[M02] * values[M11] * values[M20] 
				  + values[M02] * values[M10] * values[M21] - values[M00] * values[M12] * values[M21] 
				  - values[M01] * values[M10] * values[M22] + values[M00] * values[M11] * values[M22];
		
		values[M00] = temp[M00] * inverseDeterminant;
		values[M01] = temp[M01] * inverseDeterminant;
		values[M02] = temp[M02] * inverseDeterminant;
		values[M03] = temp[M03] * inverseDeterminant;
		values[M10] = temp[M10] * inverseDeterminant;
		values[M11] = temp[M11] * inverseDeterminant;
		values[M12] = temp[M12] * inverseDeterminant;
		values[M13] = temp[M13] * inverseDeterminant;
		values[M20] = temp[M20] * inverseDeterminant;
		values[M21] = temp[M21] * inverseDeterminant;
		values[M22] = temp[M22] * inverseDeterminant;
		values[M23] = temp[M23] * inverseDeterminant;
		values[M30] = temp[M30] * inverseDeterminant;
		values[M31] = temp[M31] * inverseDeterminant;
		values[M32] = temp[M32] * inverseDeterminant;
		values[M33] = temp[M33] * inverseDeterminant;
		return this;
	}
}
