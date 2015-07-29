package com.dbylk.rwge.graphics;

import java.util.Stack;

import android.content.Context;
import android.opengl.GLES20;
import android.util.Log;

import com.dbylk.rwge.RenderListener;
import com.dbylk.rwge.Files.PathType;
import com.dbylk.rwge.files.RwgeFile;
import com.dbylk.rwge.math.Transform2D;
import com.dbylk.rwge.math.Vector2;

public class SceneRenderer implements RenderListener {
	static final int SpriteVertexNum = 4;
	static final int SpriteIndexNum = 6;
	
	private int maxSpriteNum;
	private int drawnSpriteNum = 0;
	
	private Scene scene;
	private Stack<Transform2D> transformStack = new Stack<Transform2D>();
	
	private OrthographicCamera camera = new OrthographicCamera();
	
	private ShaderProgram shader;
	private VertexBuffer vertexBuffer;
	private IndexBuffer indexBuffer;
	
	/** Used to save four vertices attributes of sprite temporarily. */
	private float[] vertices;
	
	/**
	 * @param scene The scene this render is going to render.
	 * @param size The max number of sprite this renderer can render.
	 */
	public SceneRenderer(int maxSpriteNum) {
		this.maxSpriteNum = maxSpriteNum;
		
		shader = createSceneShaderProgram();
		
		int vertexNum = SpriteVertexNum * maxSpriteNum;
		int indexNum = SpriteIndexNum * maxSpriteNum;
		vertexBuffer = new VertexBuffer(vertexNum, shader);
		indexBuffer = new IndexBuffer(indexNum);
		
		// Set the indices of vertices of rectangular.
//		short[] indices = {
//			(short)0, (short)1, (short)2,
//			(short)2, (short)3, (short)1
//		};
		short[] indices = new short[indexNum];
		for (int i = 0, j = 0; i < indexNum; j += 4) {
			indices[i++] = (short)(j + 0);
			indices[i++] = (short)(j + 1);
			indices[i++] = (short)(j + 2);
			indices[i++] = (short)(j + 2);
			indices[i++] = (short)(j + 3);
			indices[i++] = (short)(j + 1);
		}
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
	public void render(float deltaTime) {
		if (scene != null && camera != null) {
			vertexBuffer.clear();
			drawnSpriteNum = 0;
			
			shader.begin();
			enableBlend();
			
			vertexBuffer.bind();
			indexBuffer.bind();
			shader.enableAttributes();
			
			updateCamera();
			traverseSceneLayer(scene.rootLayer);
			
			shader.disableAttributes();
			indexBuffer.unbind();
			vertexBuffer.unbind();
			
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
	
	private void traverseSceneLayer(SceneLayer root) {
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
				traverseSceneLayer(childLayer);
			}
		}
		
		transformStack.pop();
	}
	
	int x = 0;
	
	private void drawSprite(Sprite sprite) {
		if (drawnSpriteNum >= maxSpriteNum) {
			return;
		}
		
		// Calculate sprite coordinates.
		Transform2D transform = transformStack.peek();
		
		float cornerX = sprite.cornerX;
		float cornerY = sprite.cornerY;
		float width = sprite.width;
		float height = sprite.height;
		float red = sprite.color.r;
		float green = sprite.color.g;
		float blue = sprite.color.b;
		float alpha = sprite.color.a;
		float textureTopLeftU = sprite.textureRect.topLeftU;
		float textureTopLeftV = sprite.textureRect.topLeftV;
		float textureBottomRightU = sprite.textureRect.bottomRightU;
		float textureBottomRightV = sprite.textureRect.bottomRightV;
		
		Vector2 position1 = transform.transform(cornerX, 		 cornerY);			// bottom left
		Vector2 position2 = transform.transform(cornerX + width, cornerY);			// bottom right
		Vector2 position3 = transform.transform(cornerX, 		 cornerY + height);	// top left
		Vector2 position4 = transform.transform(cornerX + width, cornerY + height);	// top right
		
		int index = 0;
		vertices[index++] = position1.x;
		vertices[index++] = position1.y;
		vertices[index++] = red;
		vertices[index++] = green;
		vertices[index++] = blue;
		vertices[index++] = alpha;
		vertices[index++] = textureTopLeftU;
		vertices[index++] = textureBottomRightV;
		
		vertices[index++] = position2.x;
		vertices[index++] = position2.y;
		vertices[index++] = red;
		vertices[index++] = green;
		vertices[index++] = blue;
		vertices[index++] = alpha;
		vertices[index++] = textureBottomRightU;
		vertices[index++] = textureBottomRightV;
		
		vertices[index++] = position3.x;
		vertices[index++] = position3.y;
		vertices[index++] = red;
		vertices[index++] = green;
		vertices[index++] = blue;
		vertices[index++] = alpha;
		vertices[index++] = textureTopLeftU;
		vertices[index++] = textureTopLeftV;
		
		vertices[index++] = position4.x;
		vertices[index++] = position4.y;
		vertices[index++] = red;
		vertices[index++] = green;
		vertices[index++] = blue;
		vertices[index++] = alpha;
		vertices[index++] = textureBottomRightU;
		vertices[index++] = textureTopLeftV;
		
		vertexBuffer.setVertices(vertices);
		
		// Bind texture.
		sprite.bind();
		
		GLES20.glDrawElements(GLES20.GL_TRIANGLES, SpriteIndexNum, GLES20.GL_UNSIGNED_SHORT, drawnSpriteNum * SpriteIndexNum * 2);
		drawnSpriteNum++;

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
		RwgeFile vertexShaderFile = new RwgeFile("com/dbylk/rwge/graphics/shader/default.vertex.glsl", PathType.Classpath);
		RwgeFile fragmentShaderFile = new RwgeFile("com/dbylk/rwge/graphics/shader/default.fragment.glsl", PathType.Classpath);
		
		ShaderProgram shader = new ShaderProgram(vertexShaderFile.readString(), fragmentShaderFile.readString());
		
		shader.addAttribute(ShaderAttribute.Usage.Position, GLES20.GL_FLOAT, 2, POSITION_NAME);
		shader.addAttribute(ShaderAttribute.Usage.Color, GLES20.GL_FLOAT, 4, COLOR_NAME);
		shader.addAttribute(ShaderAttribute.Usage.TextureCoord, GLES20.GL_FLOAT, 2, TEXCOORD_NAME);
		
		shader.addUniform(ShaderUniform.Usage.TransformMatrix, PROJECTION_MATRIX_NAME);
		shader.addUniform(ShaderUniform.Usage.Sampler, SAMPLER_NAME);
		
		return shader;
	}
}
