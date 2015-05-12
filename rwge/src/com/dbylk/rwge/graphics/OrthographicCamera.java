package com.dbylk.rwge.graphics;

import com.dbylk.rwge.math.Angle;
import com.dbylk.rwge.math.Radian;
import com.dbylk.rwge.math.Transform2D;
import com.dbylk.rwge.math.Vector2;

/** 
 * Class encapsulate the method used to observe the scene. 
 * An orthographic camera can capture a rectangular viewport from OpenGL ES.
 * The position of this camera locates in the center of the rectangular.
 * It can translate, rotate and scale.
 * */

public class OrthographicCamera extends SceneLayer {
	float viewportHalfWidth = 1f;
	float viewportHalfHeight = 1f;
	
	public OrthographicCamera() {

	}
	
	public OrthographicCamera(float viewportWidth, float viewportHeight) {
		setOrtho(viewportWidth, viewportHeight);
	}
	
	public OrthographicCamera(float left, float right, float bottom, float top) {
		setOrtho(left, right, bottom, top);
	}
	
	public OrthographicCamera(OrthographicCamera camera) {
		set(camera);
	}
	
	/** This method won't change the father of this camera. */
	public void set(OrthographicCamera camera) {
		super.set(camera);
		
		this.viewportHalfWidth = camera.viewportHalfWidth;
		this.viewportHalfHeight = camera.viewportHalfHeight;
	}
	
	public void setOrtho(float viewportWidth, float viewportHeight) {
		transformUpdated = false;
		
		this.viewportHalfWidth = viewportWidth / 2;
		this.viewportHalfHeight = viewportHeight / 2;
	}
	
	public void setOrtho(float left, float right, float bottom, float top) {
		resetTransform();
		transformUpdated = false;
		
		viewportHalfWidth = (right - left) / 2;
		viewportHalfHeight = (top - bottom) / 2;
		
		setPosition(left + viewportHalfWidth, bottom + viewportHalfHeight);
	}
	
	/** The viewport will influence transform matrix of camera. */
	public Transform2D getTransform() {
		if (!transformUpdated) {
			transform.reset().translate(x, y).rotate(rotation).scale(viewportHalfWidth * scaleX, viewportHalfHeight * scaleY);
			transformUpdated = true;
		}
		
		return transform;
	}
	
	@Override
	public OrthographicCamera scale(float s) {
		super.scale(s);
		
		return this;
	}

	@Override
	public OrthographicCamera scale(float x, float y) {
		super.scale(x, y);
		
		return this;
	}

	@Override
	public OrthographicCamera scale(Vector2 vector) {
		super.scale(vector);
		
		return this;
	}

	@Override
	public OrthographicCamera translate(float x, float y) {
		super.translate(x, y);
		
		return this;
	}
	
	@Override
	public OrthographicCamera translate(Vector2 vector) {
		super.translate(vector);
		
		return this;
	}
	
	@Override
	public OrthographicCamera rotate(float radian) {
		super.rotate(radian);
		
		return this;
	}
	
	@Override
	public OrthographicCamera rotate(Radian radian) {
		super.rotate(radian);
		
		return this;
	}

	/** This method is not suggested to be used, while radian is used during trigonometric calculation. */
	@Override
	public OrthographicCamera rotate(Angle angle) {
		super.rotate(angle);
		
		return this;
	}
}
