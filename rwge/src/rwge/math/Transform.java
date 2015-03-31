package rwge.math;

/**
 * The transformation of a 2D coordinate is performed by the formula:
 * TransformMatrix3 * Vector3.
 * 
 * This class is used to hold the transform matrix used in the above formula.
 */

public class Transform implements Transformable {
	public Matrix3 transMatrix;
	
	public Transform() {
		transMatrix = new Matrix3();
	}
	
	public Transform(Matrix3 matrix) {
		transMatrix = new Matrix3(matrix);
	}
	
	public Transform(Transform transform) {
		transMatrix = new Matrix3(transform.transMatrix);
	}
	
	public Transform set(Matrix3 matrix) {
		transMatrix.set(matrix);
		
		return this;
	}
	
	public Transform set(Transform transform) {
		return this.set(transform.transMatrix);
	}
	
	@Override
	public void scale(float s) {
		transMatrix.multiply(Matrix3.scale(s));
	}

	@Override
	public void scale(float x, float y) {
		transMatrix.multiply(Matrix3.scale(x, y));

	}

	@Override
	public void scale(Vector2 vector) {
		transMatrix.multiply(Matrix3.scale(vector));
	}

	@Override
	public void translate(float x, float y) {
		transMatrix.multiply(Matrix3.translate(x, y));
	}

	@Override
	public void translate(Vector2 vector) {
		transMatrix.multiply(Matrix3.translate(vector));
	}

	@Override
	public void rotate(float radian) {
		transMatrix.multiply(Matrix3.rotate(radian));
	}

	@Override
	public void rotate(Radian radian) {
		transMatrix.multiply(Matrix3.rotate(radian));
	}

	@Override
	public void rotate(Angle angle) {
		transMatrix.multiply(Matrix3.rotate(angle));
	}
}
