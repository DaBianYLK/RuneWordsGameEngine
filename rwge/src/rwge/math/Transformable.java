package rwge.math;

/** Interface to object that can scale, translate and rotate in a 2D surface. */

public interface Transformable {
	public void scale(float s);
	public void scale(float x, float y);
	public void scale(Vector2 vector);
	
	public void translate(float x, float y);
	public void translate(Vector2 vector);
	
	public void rotate(float radian);
	public void rotate(Radian radian);
	public void rotate(Angle angle);
}
