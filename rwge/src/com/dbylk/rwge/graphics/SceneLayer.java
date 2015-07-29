package com.dbylk.rwge.graphics;

import com.dbylk.rwge.math.Angle;
import com.dbylk.rwge.math.Radian;
import com.dbylk.rwge.math.Transform2D;
import com.dbylk.rwge.math.Transformable;
import com.dbylk.rwge.math.Vector2;

import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.LinkedList;

/**
 * Scene layer is used to organize renderable objects. 
 * It is a point located at position (x, y). 
 * It can translate, rotate and scale.
 */

public class SceneLayer implements Comparable<SceneLayer>, Transformable {
	protected int priority;
	protected SceneLayer father;
	// This is the relatively transform of this layer.
	// transform.reset().translate(x-centerX, y-centerY).rotate(rotation).scale(scaleX, scaleY).translate(centerX, centerY)
	protected Transform2D transform = new Transform2D();
	// Whether the transform has updated with translation, rotation and scale.
	boolean transformUpdated = true;
	
	protected float x;
	protected float y;
	/** The rotation is based on radian. */
	protected float rotation;
	protected float scaleX = 1f; 
	protected float scaleY = 1f;

	LinkedList<SceneLayer> children = new LinkedList<SceneLayer>();

	SceneLayer() {
	}

	private SceneLayer(int priority, SceneLayer fatherLayer) {
		this.priority = priority;
		this.father = fatherLayer;
	}
	
	/** This method won't influence the priority, father and children of this scene layer. */
	public void set(SceneLayer sceneLayer) {
		this.transform = sceneLayer.transform;
		this.transformUpdated = sceneLayer.transformUpdated;
		this.x = sceneLayer.x;
		this.y = sceneLayer.y;
		this.rotation = sceneLayer.rotation;
		this.scaleX = sceneLayer.scaleX; 
		this.scaleY = sceneLayer.scaleY;
	}
	
	public void resetTransform() {
		this.transform.reset();
		this.transformUpdated = true;
		this.x = 0f;
		this.y = 0f;
		this.rotation = 0f;
		this.scaleX = 1f; 
		this.scaleY = 1f;
	}

	/**
	 * The child layers will transform just as the father layer doing. While the
	 * priority is not specified, the newer layer will be put on the top.
	 */
	public SceneLayer createChild() {
		int priority = 0;
		if (!children.isEmpty()) {
			priority = children.getLast().priority + 1;
		}
		
		SceneLayer layer = new SceneLayer(priority, this);
		children.add(layer);

		return layer;
	}

	/**
	 * The child layers will transform just as the father layer doing. The layer
	 * with higher priority will cover the layer with lower priority.
	 */
	public SceneLayer createChild(int priority) {
		SceneLayer layer = new SceneLayer(priority, this);
		
		insertChild(layer);
		
		return layer;
	}
	
	private void insertChild(SceneLayer layer) {
		Iterator<SceneLayer> it = children.iterator();
		int position = 0;
		int priority = layer.priority;
		while (it.hasNext()) {
			SceneLayer child = it.next();
			
			if (priority < child.priority) {
				break;
			}
			
			position++;
		}
		
		children.add(position, layer);
		
		if (layer.father != null && father != this) {
			layer.father.removeChild(layer);
		}
		layer.father = this;
	}
	
	private void removeChild(SceneLayer layer) {
		Iterator<SceneLayer> it = children.iterator();
		while (it.hasNext()) {
			SceneLayer child = it.next();
			
			if (child == layer) {
				it.remove();
				layer.father = null;
				break;
			}
		}
	}
	
	public void attachSprite(Sprite sprite) {
		if (children.size() == 0) {
			sprite.priority = 0;
		}
		else {
			sprite.priority = children.getLast().priority + 1;
		}
		
		if (sprite.father != null) {
			sprite.father.removeChild(sprite);
		}
		sprite.father = this;
		
		children.add(sprite);
	}
	
	public void attachSprite(Sprite sprite, int priority) {
		sprite.priority = priority;
		
		insertChild(sprite);
	}
	
	public void attachCamera(OrthographicCamera camera) {
		camera.priority = -1;
		
		insertChild(camera);
	}

	/** If the layer's father is null, this method will return false. */
	public boolean setPriority(int priority) {
		// When the priority was changed, resort the father's child layers.
		if (father != null) {
			// The following two lines' average complexity is lg(n)
			//this.priority = priority;
			//Collections.sort(father.childs, new SceneLayerComparator());		
			
			// The following method's average complexity is n.
//			Iterator<SceneLayer> it = father.childs.iterator();
//			while (it.hasNext()) {
//				SceneLayer child = it.next();
//				
//				if (child == this) {
//					it.remove();
//					break;
//				}
//			}
			
			father.removeChild(this);
			
			this.priority = priority;
			father.insertChild(this);
			
			return true;
		}
		
		return false;
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
	
	public void setPosition(float x, float y) {
		transformUpdated = false;
		
		this.x = x;
		this.y = y;
	}
	
	public void setRotation(float rotation) {
		transformUpdated = false;
		
		this.rotation = rotation;
	}
	
	public void setScale(float s) {
		transformUpdated = false;
		
		scaleX = s;
		scaleY = s;
	}
	
	public void setScale(float x, float y) {
		transformUpdated = false;
		
		scaleX = x;
		scaleY = y;
	}
	
	public Transform2D getTransform() {
		if (!transformUpdated) {
			transform.reset().translate(x, y).rotate(rotation).scale(scaleX, scaleY);
			transformUpdated = true;
		}
		
		return transform;
	}

	@Override
	public SceneLayer scale(float s) {
		transformUpdated = false;
		
		scaleX *= s;
		scaleY *= s;

		return this;
	}

	@Override
	public SceneLayer scale(float x, float y) {
		transformUpdated = false;
		
		scaleX *= x;
		scaleY *= y;
		
		return this;
	}

	@Override
	public SceneLayer scale(Vector2 vector) {
		transformUpdated = false;
		
		scaleX *= vector.x;
		scaleY *= vector.y;
		
		return this;
	}

	@Override
	public SceneLayer translate(float x, float y) {
		transformUpdated = false;
		
		this.x += x;
		this.y += y;
		
		return this;
	}
	
	@Override
	public SceneLayer translate(Vector2 vector) {
		transformUpdated = false;
		
		this.x += vector.x;
		this.y += vector.y;
		
		return this;
	}
	
	@Override
	public SceneLayer rotate(float radian) {
		transformUpdated = false;
		
		rotation += radian;
		
		return this;
	}
	
	@Override
	public SceneLayer rotate(Radian radian) {
		transformUpdated = false;
		
		rotation += radian.value;
		
		return this;
	}

	/** This method is not suggested to be used, while radian is used during trigonometric calculation. */
	@Override
	public SceneLayer rotate(Angle angle) {
		transformUpdated = false;
		
		rotation += Radian.getRadianValue(angle);
		
		return this;
	}
	
	private class SceneLayerComparator implements Comparator<SceneLayer> {
		@Override
		public int compare(SceneLayer layerA, SceneLayer layerB) {
			return layerA.compareTo(layerB);
		}
	}
}
