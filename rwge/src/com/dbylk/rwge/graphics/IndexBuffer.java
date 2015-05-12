package com.dbylk.rwge.graphics;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.ShortBuffer;

import android.opengl.GLES20;

/** 
 * IndexBuffer is a short integer buffer. 
 */

public class IndexBuffer {
	/** A short integer is 2 bytes. */
	static final int IndexSize = 2;
	
	int bufferHandle;
	ShortBuffer indices;
	
	private boolean updated = true;
	private boolean bound = false;
	
	public IndexBuffer(int indexNum) {
		int[] tempHandle = new int[1];
		// This OpenGL method's params are: 
		// 		1. number of buffer needed; 
		//		2. integer array used to get buffer handles; 
		//		3. offset of this integer array
		GLES20.glGenBuffers(1, tempHandle, 0);	
		bufferHandle = tempHandle[0];
		
		ByteBuffer bytes = ByteBuffer.allocateDirect(indexNum * IndexSize);
		bytes.order(ByteOrder.nativeOrder());
		indices = bytes.asShortBuffer();
		indices.flip();
	}

	public void setIndices(short[] indices) {
		updated = false;
		this.indices.position(0);
		this.indices.limit(this.indices.capacity());
		this.indices.put(indices);
		this.indices.flip();
		update();
	}
	
	public void update() {
		// We need to update the buffer only when the buffer is bound.
		if (bound) {
			// glBufferSubData效率更高
			GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, indices.limit() * IndexSize, indices, GLES20.GL_STATIC_DRAW);
			updated = true;
		}
	}
	
	public void bind() {
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, bufferHandle);

		bound = true;
		
		if (!updated) {
			update();
		}
	}
	
	public void unbind() {
		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
		bound = false;
	}
}
