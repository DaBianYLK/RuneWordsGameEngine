package rwge.math;

/** Interface to different dimension of vectors. */

public interface Vector<T extends Vector<T>> {
	/** Calculate and return the norm of the vector. */
	float mod();
	/** Calculate the square of the norm while calculating this is faster than calculating the norm. */
	float mod2();
	
	T set(T v);
	T add(T v);
	T sub(T v);
	float dot(T v);
	// T crs(T v);
	T scl(float scalar);
}
