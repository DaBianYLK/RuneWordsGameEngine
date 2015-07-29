package com.dbylk.rwge.graphics;

/** This class is used to control the texture rectangular with time. */

public class AnimationFrame extends TextureRect {
	float duration;

	public AnimationFrame(Texture texture, float topLeftU, float topLeftV, float bottomRightU, float bottomRightV, float duration) {
		super(texture, topLeftU, topLeftV, bottomRightU, bottomRightV);
		
		this.duration = duration;
	}
}
