package com.dbylk.rwge.math;

/**
 * The transformation of a 2D coordinate is performed by the formula:
 * TransformMatrix3 * Vector3.
 * 
 * This class is used to hold the transform matrix used in the above formula.
 */

public class Transform2D implements Transformable {
	public Matrix3 transMatrix;
	
	public Transform2D() {
		transMatrix = new Matrix3();
	}
	
	public Transform2D(Matrix3 matrix) {
		transMatrix = new Matrix3(matrix);
	}
	
	public Transform2D(Transform2D transform) {
		transMatrix = new Matrix3(transform.transMatrix);
	}
	
	public Transform2D set(Matrix3 matrix) {
		transMatrix.set(matrix);
		
		return this;
	}
	
	public Transform2D set(Transform2D transform) {
		return this.set(transform.transMatrix);
	}
	
	public Transform2D reset() {
		transMatrix.identity();
		
		return this;
	}
	
	public Matrix3 getMatrix() {
		return transMatrix;
	}
	
	@Override
	public Transform2D scale(float s) {
		transMatrix.multiply(Matrix3.scale(s));
		
		return this;
	}

	@Override
	public Transform2D scale(float x, float y) {
		transMatrix.multiply(Matrix3.scale(x, y));
		
		return this;

	}

	@Override
	public Transform2D scale(Vector2 vector) {
		transMatrix.multiply(Matrix3.scale(vector));
		
		return this;
	}

	@Override
	public Transform2D translate(float x, float y) {
		transMatrix.multiply(Matrix3.translate(x, y));
		
		return this;
	}

	@Override
	public Transform2D translate(Vector2 vector) {
		transMatrix.multiply(Matrix3.translate(vector));
		
		return this;
	}

	@Override
	public Transform2D rotate(float radian) {
		transMatrix.multiply(Matrix3.rotate(radian));
		
		return this;
	}

	@Override
	public Transform2D rotate(Radian radian) {
		transMatrix.multiply(Matrix3.rotate(radian));
		
		return this;
	}

	@Override
	public Transform2D rotate(Angle angle) {
		transMatrix.multiply(Matrix3.rotate(angle));
		
		return this;
	}
	
	/** Combine another transform to this transform. The result is saved in this transform. */
	/** Notice that the result will be different when the order of transform combination is different. */
	public Transform2D combine(Transform2D transform) {
		transMatrix.multiply(transform.transMatrix);
		
		return this;
	}
	
	/** Calculate the result of this transform matrix multiply a 3D vector. */
	public Vector3 transform(final Vector3 vector) {
		return transMatrix.multiply(vector);
	}
	
	/** Calculate the result of this transform matrix multiply a 3D vector (vector2, 1). */
	public Vector2 transform(final Vector2 vector) {
		Vector3 temp = transMatrix.multiply(new Vector3(vector));
		
		return new Vector2(temp.x, temp.y);
	}
	
	/** Calculate the result of this transform matrix multiply a 3D vector (x, y, 1). */
	public Vector2 transform(float x, float y) {
		Vector3 temp = transMatrix.multiply(new Vector3(x, y));
		
		return new Vector2(temp.x, temp.y);
	}
	
	/** Combine two transform without change them. */
	/** Notice that the result will be different when the order of transform combination is different. */
	public static Transform2D combine(Transform2D transformA, Transform2D transformB) {
		return new Transform2D(Matrix3.multiply(transformA.transMatrix, transformB.transMatrix));
	}
}
