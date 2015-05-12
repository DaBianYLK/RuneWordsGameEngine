package com.dbylk.rwge.math;

/** Interface to object that can scale, translate and rotate in a 2D surface. */

public interface Transformable {
	public Object scale(float s);
	public Object scale(float x, float y);
	public Object scale(Vector2 vector);
	
	public Object translate(float x, float y);
	public Object translate(Vector2 vector);
	
	public Object rotate(float radian);
	public Object rotate(Radian radian);
	public Object rotate(Angle angle);
}
