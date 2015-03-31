package rwge.memory;

/**
 * Interface for disposable resources which needs to be freed manually.
 */
public interface Disposable {
	/** Releases all resources of this object. */
	public void dispose();
}