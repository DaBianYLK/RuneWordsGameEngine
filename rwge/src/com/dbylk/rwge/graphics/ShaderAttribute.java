package com.dbylk.rwge.graphics;

import android.opengl.GLES20;

/** This class is used to describe an attribute defined in shader. */

public class ShaderAttribute {
	/** The usage of this attribute. An attribute can be a position, a color or a texture coordinate. */
	public final Usage usage;
	/** The basic data type of the attribute, it can be GL_FLOAT or GL_UNSIGNED_BYTE. */
	public final int type;
	/** The number of basic data type this attribute owns. */
	public final int count;
	/** The name of this attribute in shader source. */
	public String name;
	/** The location of this attribute in OpenGL. */
	public final int location;
	/** The offset of this attribute in a vertex buffer(Unit is one byte).
	 *  This is decided by the order of this attribute added into a shader program, and it can not be changed manually. */
	public int offset;
	
	/**@param usage The usage of this attribute. An attribute can be a position, a color, a texture coordinate or a transform matrix.
	 * @param type The basic data type of the attribute, it can be GL_FLOAT or GL_UNSIGNED_BYTE.
	 * @param size The number of basic data type this attribute owns.
	 * @param name The name of this attribute in shader source. */
	ShaderAttribute(Usage usage, int type, int count, String name, ShaderProgram shader) {
		this.usage = usage;
		this.type = type;
		this.count = count;
		this.name = name;
		
		this.location = GLES20.glGetAttribLocation(shader.getProgramHandle(), name);
	}
	
	
	
	public enum Usage {
		Position,
		Color,
		TextureCoord
	}
}
