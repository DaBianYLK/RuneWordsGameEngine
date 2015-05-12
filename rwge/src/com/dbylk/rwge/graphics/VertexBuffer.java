package com.dbylk.rwge.graphics;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.opengl.GLES20;

/** 
 * This class encapsulates the OpenGL buffer. Used to hold a vertices buffer for a shader program. 
 * Vertex buffer is a float buffer.
 */

public class VertexBuffer {
	int bufferHandle;
	FloatBuffer vertices;
	
	private boolean updated = true;
	private boolean bound = false;
	
	public VertexBuffer(int vertexNum, ShaderProgram shader) {
		int[] tempHandle = new int[1];
		// This OpenGL method's params are: 
		// 		1. number of buffer needed; 
		//		2. integer array used to get buffer handles; 
		//		3. offset of this integer array
		GLES20.glGenBuffers(1, tempHandle, 0);	
		bufferHandle = tempHandle[0];
		
		ByteBuffer bytes = ByteBuffer.allocateDirect(vertexNum * shader.getAttributeBufferSize());
		bytes.order(ByteOrder.nativeOrder());
		vertices = bytes.asFloatBuffer();
		vertices.flip();
	}

	public void setVertices(float[] vertices) {
		updated = false;
		this.vertices.position(0);
		this.vertices.limit(this.vertices.capacity());
		this.vertices.put(vertices);
		this.vertices.flip();
		update();
	}
	
	public void update() {
		// We need to update the buffer only when the buffer is bound.
		if (bound) {
			// glBufferSubData效率更高
			GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, vertices.limit() * ShaderProgram.ElementSize, vertices, GLES20.GL_STATIC_DRAW);
			updated = true;
		}
	}
	
	public void bind() {
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, bufferHandle);
		
		bound = true;
		
		if (!updated) {
			update();
		}
	}
	
	public void unbind() {
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
		bound = false;
	}
}
