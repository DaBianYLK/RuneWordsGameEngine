package com.dbylk.rwge.graphics;

import java.util.ArrayList;

import com.dbylk.rwge.graphics.ShaderAttribute.Usage;

import android.opengl.GLES20;
import android.util.Log;

public class ShaderProgram {
	/** A float is 4 bytes. */
	static final int ElementSize = 4;
	
	/** Shader program handle. This is assigned by OpenGL ES. */
	private int programHandle;
	/** Vertex shader handle. This is assigned by OpenGL ES. */
	private int vertexShaderHandle;
	/** Fragment shader handle. This is assigned by OpenGL ES. */
	private int fragmentShaderHandle;
	/** Vertex shader source. **/
	private String vertexShaderSource;
	/** Fragment shader source. **/
	private String fragmentShaderSource;
	
	private ArrayList<ShaderAttribute> attributes = new ArrayList<ShaderAttribute>();
	private ArrayList<ShaderUniform> uniforms = new ArrayList<ShaderUniform>();
	private int attributeBufferSize;
	
	boolean isLinked = false;
	
	public ShaderProgram(String vertexShaderSource, String fragmentShaderSource) {
		linkProgram(vertexShaderSource, fragmentShaderSource);
		
		if (!isLinked) {
			return;
		}
		
		this.vertexShaderSource = vertexShaderSource;
		this.fragmentShaderSource = fragmentShaderSource;
	}
	
	private void linkProgram(String vertexShaderSource, String fragmentShaderSource) {
		vertexShaderHandle = createShader(GLES20.GL_VERTEX_SHADER, vertexShaderSource);
		fragmentShaderHandle = createShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderSource);
		
		if (vertexShaderHandle == -1 || fragmentShaderHandle == -1) {
			return;
		}
		
		programHandle = GLES20.glCreateProgram();
		if (programHandle == 0) {
			programHandle = -1;
			Log.e("ShaderProgram", "Unable to get a new program handle from OpenGL ES.");
			return;
		}
		
		GLES20.glAttachShader(programHandle, vertexShaderHandle);
		GLES20.glAttachShader(programHandle, fragmentShaderHandle);
		GLES20.glLinkProgram(programHandle);
		
		int[] linkStatus = new int[1];
		GLES20.glGetProgramiv(programHandle, GLES20.GL_LINK_STATUS, linkStatus, 0);
		if (linkStatus[0] == 0) {
			Log.e("ShaderProgram", GLES20.glGetProgramInfoLog(programHandle));
			return;
		}
		
		isLinked = true;
	}
	
	/** Create a shader by source and return its handle. Return -1 if create failed. */
	private int createShader(int shaderType, String source) {
		if (source == null) {
			throw new IllegalArgumentException("Shader source can not be null.");
		}
		
		int shader = GLES20.glCreateShader(shaderType);
		if (shader == 0) {
			Log.e("ShaderProgram", "Unable to get a new shader handle from OpenGL ES.");
			return -1;
		}
		
		GLES20.glShaderSource(shader, source);
		GLES20.glCompileShader(shader);
		
		int[] compileStatus = new int[1];
		GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compileStatus, 0);
		if (compileStatus[0] == 0) {
			Log.e("ShaderProgram", GLES20.glGetShaderInfoLog(shader));
			
			return -1;
		}
		
		return shader;
	}
	
	/**@param usage The usage of this attribute. An attribute can be a position, a color, a texture coordinate or a transform matrix.
	 * @param type The basic data type of the attribute, it can be GL_FLOAT or GL_UNSIGNED_BYTE.
	 * @param size The number of basic data type this attribute owns.
	 * @param name The name of this attribute in shader source. */
	public void addAttribute(ShaderAttribute.Usage usage, int type, int size, String name) {
		ShaderAttribute attribute = new ShaderAttribute(usage, type, size, name, this);
		
		// Calculate the offset of this attribute in a vertex buffer, the offset's unit is byte.
		attribute.offset = attributeBufferSize;
		attributeBufferSize += attribute.count * ElementSize;
		
		attributes.add(attribute);
	}
	
	public ShaderAttribute getAttribute(int index) {
		return attributes.get(index);
	}
	
	public int getAttributeNum() {
		return attributes.size();
	}
	
	/** Return the buffer size that this shader's attributes needed, the unit is byte.  */
	public int getAttributeBufferSize() {
		return attributeBufferSize;
	}
	
	public ArrayList<ShaderAttribute> getAttributes() {
		return attributes;
	}
	
	public void enableAttributes() {
		for (int i = 0; i < attributes.size(); i++) {
			ShaderAttribute attribute = attributes.get(i);
			GLES20.glEnableVertexAttribArray(attribute.location);
			GLES20.glVertexAttribPointer(attribute.location, attribute.count, attribute.type, false, attributeBufferSize, attribute.offset);
			//Log.i("Render", "stride = " + attributeBufferSize + ", offset = " + attribute.offset);
		}
	}
	
	public void enableAttribute(int i) {
		ShaderAttribute attribute = attributes.get(i);
		GLES20.glEnableVertexAttribArray(attribute.location);
		GLES20.glVertexAttribPointer(attribute.location, attribute.count, attribute.type, false, attributeBufferSize, attribute.offset);
	}
	
	public void disableAttributes() {
		for (int i = 0; i < attributes.size(); i++) {
			GLES20.glDisableVertexAttribArray(attributes.get(i).location);
		}
	}
	
	public void addUniform(ShaderUniform.Usage usage, String name) {
		ShaderUniform uniform = new ShaderUniform(usage, name, this);
		
		uniforms.add(uniform);
	}
	
	public ShaderUniform getUniform(int index) {
		return uniforms.get(index);
	}
	
	public String getVertexShaderSource() {
		return vertexShaderSource;
	}
	
	public String getFragmentShaderSource() {
		return fragmentShaderSource;
	}
	
	public int getProgramHandle() {
		return programHandle;
	}
	
	/** Activate this shader program. */
	public void begin() {
		GLES20.glUseProgram(programHandle);
	}
	
	/** Deactivate this shader program. Note that this method won't delete the program. */
	public void end() {
		GLES20.glUseProgram(0);
	}
}
