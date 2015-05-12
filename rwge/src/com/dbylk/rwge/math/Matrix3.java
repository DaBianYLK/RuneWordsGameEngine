package com.dbylk.rwge.math;

import java.io.Serializable;

/**
 * A Matrix with 4*4 elements. The matrix elements are saved in an array, the
 * layout of the array is as the following graph: 0 3 6 | 1 4 7 | 2 5 8
 * 
 * The coordinates are as the follows: M00 M01 M02 | M10 M11 M12 | M20 M21 M22
 * 
 * Transform formula is Matrix * oldVector = newVector.
 * */

public class Matrix3 implements Serializable {
	private static final long serialVersionUID = 778127892148150941L;

	public static final int M00 = 0;
	public static final int M01 = 3;
	public static final int M02 = 6;
	public static final int M10 = 1;
	public static final int M11 = 4;
	public static final int M12 = 7;
	public static final int M20 = 2;
	public static final int M21 = 5;
	public static final int M22 = 8;

	public static final float[] identityValues = new float[] { 1, 0, 0, 0, 1,
			0, 0, 0, 1 };

	public float[] values = new float[9];

	/** Default construction, an identity matrix will be created. */
	public Matrix3() {
		values[M00] = 1f;
		values[M11] = 1f;
		values[M22] = 1f;
	}

	/** Construction. */
	public Matrix3(Matrix3 matrix) {
		this.set(matrix.values);
	}

	/** Construction. */
	public Matrix3(float[] values) {
		this.set(values);
	}

	public Matrix3 set(Matrix3 matrix) {
		this.set(matrix.values);

		return this;
	}

	public Matrix3 set(float[] values) {
		System.arraycopy(values, 0, this.values, 0, 9);

		return this;
	}

	public float[] getValues() {
		return values;
	}

	/** Set the matrix to be an identity matrix. */
	public Matrix3 identity() {
		return this.set(identityValues);
	}

	/** Set the matrix to be a scale matrix. */
	public static Matrix3 scale(float x, float y) {
		Matrix3 matrix = new Matrix3();
		
		matrix.values[M00] = x;
		matrix.values[M11] = y;

		return matrix;
	}
	
	/** Set the matrix to be a scale matrix. */
	public static Matrix3 scale(Vector2 vector) {
		return Matrix3.scale(vector.x, vector.y);
	}

	/** Set the matrix to be a scale matrix. */
	public static Matrix3 scale(float s) {
		return Matrix3.scale(s, s);
	}

	/** Set the matrix to be a translate matrix. */
	public static Matrix3 translate(float x, float y) {
		Matrix3 matrix = new Matrix3();
		
		matrix.values[M02] = x;
		matrix.values[M12] = y;

		return matrix;
	}
	
	/** Set the matrix to be a translate matrix. */
	public static Matrix3 translate(Vector2 vector) {
		return Matrix3.translate(vector.x, vector.y);
	}

	/** Set the matrix to be a rotate matrix. */
	public static Matrix3 rotate(float radian) {
		Matrix3 matrix = new Matrix3();

		float sin = (float) Math.sin(radian);
		float cos = (float) Math.cos(radian);

		matrix.values[M00] = cos;
		matrix.values[M10] = sin;
		matrix.values[M01] = -sin;
		matrix.values[M11] = cos;

		return matrix;
	}
	
	/** Set the matrix to be a rotate matrix. */
	public static Matrix3 rotate(Angle angle) {
		return Matrix3.rotate(angle.value * (float) Math.PI / 180f);
	}

	/** Set the matrix to be a rotate matrix. */
	public static Matrix3 rotate(Radian radian) {
		return Matrix3.rotate(radian.value);
	}

	/** The result is saved in this matrix. */
	public Matrix3 multiply(Matrix3 matrix) {
		float[] temp = new float[9];

		temp[M00] = values[M00] * matrix.values[M00] + values[M01] * matrix.values[M10] + values[M02] * matrix.values[M20];
		temp[M01] = values[M00] * matrix.values[M01] + values[M01] * matrix.values[M11] + values[M02] * matrix.values[M21];
		temp[M02] = values[M00] * matrix.values[M02] + values[M01] * matrix.values[M12] + values[M02] * matrix.values[M22];

		temp[M10] = values[M10] * matrix.values[M00] + values[M11] * matrix.values[M10] + values[M12] * matrix.values[M20];
		temp[M11] = values[M10] * matrix.values[M01] + values[M11] * matrix.values[M11] + values[M12] * matrix.values[M21];
		temp[M12] = values[M10] * matrix.values[M02] + values[M11] * matrix.values[M12] + values[M12] * matrix.values[M22];

		temp[M20] = values[M20] * matrix.values[M00] + values[M21] * matrix.values[M10] + values[M22] * matrix.values[M20];
		temp[M21] = values[M20] * matrix.values[M01] + values[M21] * matrix.values[M11] + values[M22] * matrix.values[M21];
		temp[M22] = values[M20] * matrix.values[M02] + values[M21] * matrix.values[M12] + values[M22] * matrix.values[M22];

		System.arraycopy(temp, 0, this.values, 0, 9);

		return this;
	}

	/** The result is saved in parameter vector. */
	public Vector3 multiply(Vector3 vector) {
		float x = values[M00] * vector.x + values[M01] * vector.y + values[M02] * vector.z;
		float y = values[M10] * vector.x + values[M11] * vector.y + values[M12] * vector.z;
		float z = values[M20] * vector.x + values[M21] * vector.y + values[M22] * vector.z;

		vector.x = x;
		vector.y = y;
		vector.z = z;

		return vector;
	}
	
	/** Calculate the result of A*B without change them. */
	public static Matrix3 multiply(Matrix3 matrixA, Matrix3 matrixB) {
		float[] temp = new float[9];
		
		temp[M00] = matrixA.values[M00] * matrixB.values[M00] + matrixA.values[M01] * matrixB.values[M10] + matrixA.values[M02] * matrixB.values[M20];
		temp[M01] = matrixA.values[M00] * matrixB.values[M01] + matrixA.values[M01] * matrixB.values[M11] + matrixA.values[M02] * matrixB.values[M21];
		temp[M02] = matrixA.values[M00] * matrixB.values[M02] + matrixA.values[M01] * matrixB.values[M12] + matrixA.values[M02] * matrixB.values[M22];

		temp[M10] = matrixA.values[M10] * matrixB.values[M00] + matrixA.values[M11] * matrixB.values[M10] + matrixA.values[M12] * matrixB.values[M20];
		temp[M11] = matrixA.values[M10] * matrixB.values[M01] + matrixA.values[M11] * matrixB.values[M11] + matrixA.values[M12] * matrixB.values[M21];
		temp[M12] = matrixA.values[M10] * matrixB.values[M02] + matrixA.values[M11] * matrixB.values[M12] + matrixA.values[M12] * matrixB.values[M22];

		temp[M20] = matrixA.values[M20] * matrixB.values[M00] + matrixA.values[M21] * matrixB.values[M10] + matrixA.values[M22] * matrixB.values[M20];
		temp[M21] = matrixA.values[M20] * matrixB.values[M01] + matrixA.values[M21] * matrixB.values[M11] + matrixA.values[M22] * matrixB.values[M21];
		temp[M22] = matrixA.values[M20] * matrixB.values[M02] + matrixA.values[M21] * matrixB.values[M12] + matrixA.values[M22] * matrixB.values[M22];

		return new Matrix3(temp);
	}
	
	/** @return The determinant of this matrix */
	public float getDeterminant() {
		return values[M00] * values[M11] * values[M22] + values[M01] * values[M12] * values[M20] 
			 + values[M02] * values[M10] * values[M21] - values[M00] * values[M12] * values[M21] 
			 - values[M01] * values[M10] * values[M22] - values[M02] * values[M11] * values[M20];
	}

	/** Calculate the inverse matrix. Result is saved in this matrix.
	 * @return This matrix for the purpose of chaining operations.
	 * @throws RuntimeException if the matrix is singular (not invertible). */
	public Matrix3 inverse() {
		float determinant = getDeterminant();
		
		if (determinant == 0) {
			throw new RuntimeException("Can't invert a singular matrix");
		}

		float inverseDeterminant = 1.0f / determinant;

		float[] temp = new float[9];
		
		temp[M00] = values[M11] * values[M22] - values[M21] * values[M12];
		temp[M10] = values[M20] * values[M12] - values[M10] * values[M22];
		temp[M20] = values[M10] * values[M21] - values[M20] * values[M11];
		temp[M01] = values[M21] * values[M02] - values[M01] * values[M22];
		temp[M11] = values[M00] * values[M22] - values[M20] * values[M02];
		temp[M21] = values[M20] * values[M01] - values[M00] * values[M21];
		temp[M02] = values[M01] * values[M12] - values[M11] * values[M02];
		temp[M12] = values[M10] * values[M02] - values[M00] * values[M12];
		temp[M22] = values[M00] * values[M11] - values[M10] * values[M01];

		values[M00] = inverseDeterminant * temp[M00];
		values[M10] = inverseDeterminant * temp[M10];
		values[M20] = inverseDeterminant * temp[M20];
		values[M01] = inverseDeterminant * temp[M01];
		values[M11] = inverseDeterminant * temp[M11];
		values[M21] = inverseDeterminant * temp[M21];
		values[M02] = inverseDeterminant * temp[M02];
		values[M12] = inverseDeterminant * temp[M12];
		values[M22] = inverseDeterminant * temp[M22];

		return this;
	}
}
