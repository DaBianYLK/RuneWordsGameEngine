package com.dbylk.rwge.math;

/** Interface to different dimension of vectors. */

public interface Vector<T extends Vector<T>> {
	/** Calculate and return the norm of the vector. */
	public float mod();
	/** Calculate the square of the norm while calculating this is faster than calculating the norm. */
	public float mod2();
	
	public T set(T v);
	public T add(T v);
	public T sub(T v);
	public float dot(T v);
	// T cross(T v);
	public T scale(float s);
}
