package rwge.graphics.scene;

import java.util.Comparator;

public class SceneLayerComparator implements Comparator<SceneLayer> {
	@Override
	public int compare(SceneLayer layerA, SceneLayer layerB) {
		return layerA.compareTo(layerB);
	}
}
