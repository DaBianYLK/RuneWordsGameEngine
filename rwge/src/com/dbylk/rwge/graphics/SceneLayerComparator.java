package com.dbylk.rwge.graphics;

import java.util.Comparator;

public class SceneLayerComparator implements Comparator<SceneLayer> {
	@Override
	public int compare(SceneLayer layerA, SceneLayer layerB) {
		return layerA.compareTo(layerB);
	}
}
