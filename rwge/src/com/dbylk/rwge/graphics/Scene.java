package com.dbylk.rwge.graphics;

public class Scene {
	SceneLayer rootLayer;
	
	public Scene() {
		rootLayer = new SceneLayer();
	}
	
	public SceneLayer getRootLayer() {
		return rootLayer;
	}
}
