package com.dbylk.rwge.graphics;

import android.opengl.GLES20;

/** This class is used to describe an uniform defined in shader. */

public class ShaderUniform {
	/** The usage of this uniform. An uniform can be a transform matrix or a sampler. */
	public final Usage usage;
	/** The name of this attribute in shader source. */
	public String name;
	/** The location of this attribute in OpenGL. */
	public final int location;
	
	/**
	 * @param usage The usage of this uniform. A uniform can be a transform matrix or a sampler.
	 * @param name The name of this uniform in shader source. 
	 * */
	ShaderUniform(Usage usage, String name, ShaderProgram shader) {
		this.usage = usage;
		this.name = name;
		
		this.location = GLES20.glGetUniformLocation(shader.getProgramHandle(), name);
	}
	
	public enum Usage {
		TransformMatrix,
		Sampler
	}
}
