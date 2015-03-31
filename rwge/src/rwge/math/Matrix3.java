package rwge.math;

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

	public Matrix3 multiply(Matrix3 matrix) {
		float[] temp = new float[9];

		temp[M00] = values[M00] * matrix.values[M00] + values[M01]
				* matrix.values[M10] + values[M02] * matrix.values[M20];
		temp[M01] = values[M00] * matrix.values[M01] + values[M01]
				* matrix.values[M11] + values[M02] * matrix.values[M21];
		temp[M02] = values[M00] * matrix.values[M02] + values[M01]
				* matrix.values[M12] + values[M02] * matrix.values[M22];

		temp[M10] = values[M10] * matrix.values[M00] + values[M11]
				* matrix.values[M10] + values[M12] * matrix.values[M20];
		temp[M11] = values[M10] * matrix.values[M01] + values[M11]
				* matrix.values[M11] + values[M12] * matrix.values[M21];
		temp[M12] = values[M10] * matrix.values[M02] + values[M11]
				* matrix.values[M12] + values[M12] * matrix.values[M22];

		temp[M20] = values[M20] * matrix.values[M00] + values[M21]
				* matrix.values[M10] + values[M22] * matrix.values[M20];
		temp[M21] = values[M20] * matrix.values[M01] + values[M21]
				* matrix.values[M11] + values[M22] * matrix.values[M21];
		temp[M22] = values[M20] * matrix.values[M02] + values[M21]
				* matrix.values[M12] + values[M22] * matrix.values[M22];

		System.arraycopy(temp, 0, this.values, 0, 9);

		return this;
	}

	public Vector3 multiply(Vector3 vector) {
		float x = values[M00] * vector.x + values[M01] * vector.y + values[M02]
				* vector.z;
		float y = values[M10] * vector.x + values[M11] * vector.y + values[M12]
				* vector.z;
		float z = values[M20] * vector.x + values[M21] * vector.y + values[M22]
				* vector.z;

		vector.x = x;
		vector.y = y;
		vector.z = z;

		return vector;
	}
}
