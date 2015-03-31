package rwge.graphics.scene;

import rwge.math.Angle;
import rwge.math.Radian;
import rwge.math.Transform;
import rwge.math.Transformable;
import rwge.math.Vector2;

import java.util.ArrayList;
import java.util.List;

public class SceneLayer implements Comparable<SceneLayer>, Transformable {
	private int priority;
	private SceneLayer father;
	private Transform transform = new Transform();

	protected List<SceneLayer> childs = new ArrayList<SceneLayer>();

	private SceneLayer() {

	}

	private SceneLayer(int priority, SceneLayer fatherLayer) {
		this.priority = priority;
		this.father = fatherLayer;
	}

	/**
	 * The child layers will transform just as the father layer doing. While the
	 * priority is not specified, the newer layer will be put on the top.
	 */
	public SceneLayer createChild() {
		SceneLayer layer = new SceneLayer(
				childs.get(childs.size() - 1).priority + 1, this);
		childs.add(layer);

		return layer;
	}

	/**
	 * The child layers will transform just as the father layer doing. The layer
	 * with higher priority will cover the layer with lower priority.
	 */
	public SceneLayer createChild(int priority) {
		SceneLayer layer = new SceneLayer(priority, this);

		boolean hasbeenAdded = false;
		for (int i = 0; i < childs.size(); ++i) {
			if (priority < childs.get(i).priority) {
				childs.add(i, layer);
				hasbeenAdded = true;
				break;
			}
		}

		if (!hasbeenAdded) {
			childs.add(layer);
		}

		return layer;
	}

	/** If the layer's father is null, this method will return false. */
	public boolean setPriority(int priority) {
		this.priority = priority;

		// When the priority was changed, resort the father's child layers.
		if (father != null) {
			father.childs.sort(new SceneLayerComparator());
		} else {
			return false;
		}

		return true;
	}

	public void render() {

	}

	@Override
	public int compareTo(SceneLayer layer) {
		if (this.priority < layer.priority) {
			return -1;
		} else if (this.priority > layer.priority) {
			return 1;
		}

		return 0;
	}

	@Override
	public void scale(float s) {
		transform.scale(s);
	}

	@Override
	public void scale(float x, float y) {
		transform.scale(x, y);
	}

	@Override
	public void scale(Vector2 vector) {
		transform.scale(vector);
	}

	@Override
	public void translate(float x, float y) {
		transform.translate(x, y);
	}

	@Override
	public void translate(Vector2 vector) {
		transform.translate(vector);
	}

	@Override
	public void rotate(float radian) {
		transform.rotate(radian);
	}

	@Override
	public void rotate(Radian radian) {
		transform.rotate(radian);
	}

	@Override
	public void rotate(Angle angle) {
		transform.rotate(angle);
	}
	
	public Transform getTransform() {
		return transform;
	}
}
