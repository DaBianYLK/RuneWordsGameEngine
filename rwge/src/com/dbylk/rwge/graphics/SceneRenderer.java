package com.dbylk.rwge.graphics;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.Stack;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLES20;
import android.util.Log;

import com.dbylk.rwge.RenderListener;
import com.dbylk.rwge.Files.PathType;
import com.dbylk.rwge.android.AndroidFile;
import com.dbylk.rwge.math.Matrix3;
import com.dbylk.rwge.math.Transform2D;
import com.dbylk.rwge.math.Vector2;

public class SceneRenderer implements RenderListener {
	static final int SpriteVertexNum = 4;
	static final int SpriteIndexNum = 6;
	
	Context context;
	
	private Scene scene;
	private Stack<Transform2D> transformStack = new Stack<Transform2D>();
	
	private OrthographicCamera camera = new OrthographicCamera();
	
	private ShaderProgram shader;
	private VertexBuffer vertexBuffer;
	private IndexBuffer indexBuffer;
	
	/** Used to save four vertices attributes of sprite. */
	private float[] vertices;
	
	/**
	 * @param scene The scene this render is going to render.
	 * @param size The max number of sprite this renderer can render.
	 */
	public SceneRenderer(Context context) {
		this.context = context;
		
		shader = createSceneShaderProgram();
		
		vertexBuffer = new VertexBuffer(SpriteVertexNum, shader);
		indexBuffer = new IndexBuffer(SpriteIndexNum);
		
		// Set the indices of vertices of rectangular.
		short[] indices = {
			(short)0, (short)1, (short)2,
			(short)2, (short)3, (short)1
		};
//		int indicesLength = maxSpriteNum * 6;
//		short[] indices = new short[6];
//		indices[0] = (short)0;
//		indices[1] = (short)1;
//		indices[2] = (short)2;
//		indices[3] = (short)2;
//		indices[4] = (short)3;
//		indices[5] = (short)1;
		indexBuffer.setIndices(indices);
		
		vertices = new float[shader.getAttributeBufferSize() / ShaderProgram.ElementSize * SpriteVertexNum];

		transformStack.push(new Transform2D());
	}
	
	public void setScene(Scene scene) {
		this.scene = scene;
	}
	
	public Scene getScene() {
		return scene;
	}
	
	public void setCamera(OrthographicCamera camera) {
		this.camera = camera;
	}
	
	public OrthographicCamera getCamera() {
		return camera;
	}
	
	@Override
	public void render() {
		if (scene != null && camera != null) {
			shader.begin();
			enableBlend();
			
			updateCamera();

			traversalSceneLayer(scene.rootLayer);
			
			disableBlend();
			shader.end();
		}
		else {
			Log.w("SceneRenderer", "Failed to render since the scene or the camera does not exist.");
		}
	}
	
	private void updateCamera() {
		Transform2D projection = new Transform2D(camera.getTransform());
		
		SceneLayer layer = camera.father;
		while(layer != null) {
			projection = Transform2D.combine(layer.getTransform(), projection);
			
			layer = layer.father;
		}
		
		// The transform matrix of projection needs to be inversed before calculation.
	    float[] projectionMatrix = projection.getMatrix().inverse().values;
	    int projectionMatrixLocation = shader.getUniform(0).location;
	    GLES20.glUniformMatrix3fv(projectionMatrixLocation, 1, false, projectionMatrix, 0);
	}
	
	private void traversalSceneLayer(SceneLayer root) {
		if (root == null) {
			return;
		}
		
		Transform2D transform = new Transform2D(transformStack.peek());
		transform.combine(root.getTransform());
		
		transformStack.push(transform);
		
		if (root instanceof Sprite) {
			drawSprite((Sprite)root);
		}
		
		synchronized(root.children) {
			for (SceneLayer childLayer : root.children) {
				traversalSceneLayer(childLayer);
			}
		}
		
		transformStack.pop();
	}
	
	private void drawSprite(Sprite sprite) {
		// Calculate sprite coordinates.
		Transform2D transform = transformStack.peek();
		
		Vector2 position1 = transform.transform(sprite.cornerX, 			   sprite.cornerY);					// bottom left
		Vector2 position2 = transform.transform(sprite.cornerX + sprite.width, sprite.cornerY);					// bottom right
		Vector2 position3 = transform.transform(sprite.cornerX, 			   sprite.cornerY + sprite.height);	// top left
		Vector2 position4 = transform.transform(sprite.cornerX + sprite.width, sprite.cornerY + sprite.height);	// top right
		
		float textureTopLeftU = sprite.textureRect.topLeftU;
		float textureTopLeftV = sprite.textureRect.topLeftV;
		float textureBottomRightU = sprite.textureRect.bottomRightU;
		float textureBottomRightV = sprite.textureRect.bottomRightV;
		
		int index = 0;
		vertices[index++] = position1.x;
		vertices[index++] = position1.y;
		vertices[index++] = sprite.color[0];
		vertices[index++] = sprite.color[1];
		vertices[index++] = sprite.color[2];
		vertices[index++] = sprite.color[3];
		vertices[index++] = textureTopLeftU;
		vertices[index++] = textureBottomRightV;
		
		vertices[index++] = position2.x;
		vertices[index++] = position2.y;
		vertices[index++] = sprite.color[0];
		vertices[index++] = sprite.color[1];
		vertices[index++] = sprite.color[2];
		vertices[index++] = sprite.color[3];
		vertices[index++] = textureBottomRightU;
		vertices[index++] = textureBottomRightV;
		
		vertices[index++] = position3.x;
		vertices[index++] = position3.y;
		vertices[index++] = sprite.color[0];
		vertices[index++] = sprite.color[1];
		vertices[index++] = sprite.color[2];
		vertices[index++] = sprite.color[3];
		vertices[index++] = textureTopLeftU;
		vertices[index++] = textureTopLeftV;
		
		vertices[index++] = position4.x;
		vertices[index++] = position4.y;
		vertices[index++] = sprite.color[0];
		vertices[index++] = sprite.color[1];
		vertices[index++] = sprite.color[2];
		vertices[index++] = sprite.color[3];
		vertices[index++] = textureBottomRightU;
		vertices[index++] = textureTopLeftV;
		
		// Bind texture.
		sprite.bind();
		
		// Bind vertex buffer.
		vertexBuffer.bind();
		vertexBuffer.setVertices(vertices);
		
		indexBuffer.bind();

		shader.enableAttributes();
		
		GLES20.glDrawElements(GLES20.GL_TRIANGLES, SpriteIndexNum, GLES20.GL_UNSIGNED_SHORT, 0);
		
		shader.disableAttributes();
		
		indexBuffer.unbind();
		
		vertexBuffer.unbind();
		
		sprite.unbind();
	}
	
	public void enableBlend() {
		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);
	}
	
	public void disableBlend() {
		GLES20.glDisable(GLES20.GL_BLEND);
	}

	@Override
	public void pause() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void resume() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void dispose() {
		// TODO Auto-generated method stub
		
	}
	
	/** Default name for position attributes. */
	public static final String POSITION_NAME = "a_position";
	/** Default name for normal attributes. **/
	public static final String NORMAL_NAME = "a_normal";
	/** Default name for color attributes. **/
	public static final String COLOR_NAME = "a_color";
	/** Default name for texcoords attributes. **/
	public static final String TEXCOORD_NAME = "a_texCoord";
	/** Default name for tangent attribute. **/
	public static final String TANGENT_NAME = "a_tangent";
	/** Default name for binormal attribute. **/
	public static final String BINORMAL_NAME = "a_binormal";
	/** Default name for projection matrix uniform. **/
	public static final String PROJECTION_MATRIX_NAME = "u_projTrans";
	/** Default name for texture sampler uniform. **/
	public static final String SAMPLER_NAME = "u_texture";
	
	/** Create and return a shader program with default vertex shader and fragment shader. */
	private ShaderProgram createSceneShaderProgram() {
		if (context == null) {
			Log.e("SceneRenderer", "Shader can not be created since context does not exisetd.");
			return null;
		}
		
		AssetManager assetManager = context.getAssets();
		
		AndroidFile vertexShaderFile = new AndroidFile(assetManager, "glsl/default.vss", PathType.Internal);
		AndroidFile fragmentShaderFile = new AndroidFile(assetManager, "glsl/default.fss", PathType.Internal);
			
		ShaderProgram shader = new ShaderProgram(vertexShaderFile.readString(), fragmentShaderFile.readString());
		
		shader.addAttribute(ShaderAttribute.Usage.Position, GLES20.GL_FLOAT, 2, POSITION_NAME);
		shader.addAttribute(ShaderAttribute.Usage.Color, GLES20.GL_FLOAT, 4, COLOR_NAME);
		shader.addAttribute(ShaderAttribute.Usage.TextureCoord, GLES20.GL_FLOAT, 2, TEXCOORD_NAME);
		
		shader.addUniform(ShaderUniform.Usage.TransformMatrix, PROJECTION_MATRIX_NAME);
		shader.addUniform(ShaderUniform.Usage.Sampler, SAMPLER_NAME);
		
		return shader;
	}
}
