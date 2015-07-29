package com.dbylk.rwge.utils;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.dbylk.rwge.Rwge;
import com.dbylk.rwge.android.utils.Debug;
import com.dbylk.rwge.files.RwgeFile;
import com.dbylk.rwge.graphics.Animation;
import com.dbylk.rwge.graphics.AnimationFrame;
import com.dbylk.rwge.graphics.Sprite;
import com.dbylk.rwge.graphics.Texture;
import com.dbylk.rwge.graphics.TextureRect;
import com.dbylk.rwge.math.Vector4;

/** 
 * Load a sprite from a sprite file ("*.sprite", format is text file.). 
 * The texture file needs to put in the sd card.
 * 
 * The sprite file format requirement see the introduction of {@link Parser}.
 * */

public class SpriteLoader {
	public static final String SpirteWidthToken = "SpriteWidth";
	public static final String SpirteHeightToken = "SpirteHeight";
	
	public static final String FrameWidthToken = "FrameWidth";
	public static final String FrameHeightToken = "FrameHeight";
	
	public static final String CornerXToken = "CornerX";
	public static final String CornerYToken = "CornerY";
	
	public static final String TexturePathToken = "TexturePath";
	
	public static final String AnimationNumToken = "AnimationNum";
	
	/** 
	 * To get attribute of a animation, you should use "AnimationToken +  SerialNumber + AttributeToken".
	 * For example, AnimationToken = "Animation", SerialNumber = 1, AttributeToken is AnimationNameToken = "Name",
	 * the whole token is "Animation1Name".
	 *  */
	public static final String AnimationToken = "Animation";
	public static final String AnimationNameToken = "Name";
	public static final String AnimationStartFrameToken = "StartFrame"; 
	public static final String AnimationFrameNumToken = "FrameNum";
	/** Between each time use ";" character to separate. */
	public static final String AnimationDurationsOfFrameToken = "DurationsOfFrame";
	public static final String AnimationTimeSeparator = ";";
	
	private SpriteLoader() { }
	
	/** Sprite file needs to put in external path storage. */
	public static Sprite load(RwgeFile file) {
		Parser parser = new Parser(file);
		
		float spriteWidth  = parser.getFloat(SpirteWidthToken);
		float spriteHeight = parser.getFloat(SpirteHeightToken);
		float frameWidth   = parser.getFloat(FrameWidthToken);
		float frameHeight  = parser.getFloat(FrameHeightToken);
		float cornerX      = parser.getFloat(CornerXToken);
		float cornerY      = parser.getFloat(CornerYToken);
		String texturePath = parser.getString(TexturePathToken);
		
		Debug.log("SpriteLoader", "sprite path 	= " + file.getPath() 	+ "\n" + 
				  				  "sprite width 	= " + spriteWidth 	+ "\n" + 
								  "sprite height 	= " + spriteHeight 	+ "\n" +
								  "frame width 		= " + frameWidth 	+ "\n" +
								  "frame height 	= " + frameHeight 	+ "\n" +
								  "corner X 		= " + cornerX 		+ "\n" +
								  "corner Y 		= " + cornerY 		+ "\n" +
								  "texture path 	= " + texturePath); 
		
		//RwgeFile textureFile = new RwgeFile(texturePath, PathType.External);
		Bitmap bitmap = BitmapFactory.decodeFile(Rwge.files.getExternalPath() + texturePath);
		Texture texture = new Texture(bitmap);
		TextureRect rect = new TextureRect(texture, 0f, 0f, frameWidth, frameHeight);
		Sprite sprite = new Sprite(rect, 0f, 0f, cornerX, cornerY, spriteWidth, spriteHeight);
		
		int animationNum = parser.getInt(AnimationNumToken);
		if (animationNum > 0) {
			int rowFrameNum = (int)(1f / frameWidth);
			int columnFrameNum = (int)(1f / frameHeight);
			int frameNum = rowFrameNum * columnFrameNum;
			
			// x = topLeftU, y = topLeftV, z = bottomRightU, w = bottomRightV
			Vector4[] frameCoords = new Vector4[frameNum];
			for (int v = 0; v < columnFrameNum; v++) {
				for (int u = 0; u < rowFrameNum; u++) {
					int frameIndex = u + v * rowFrameNum;
					frameCoords[frameIndex] = new Vector4();
					frameCoords[frameIndex].x = u * frameWidth;
					frameCoords[frameIndex].y = v * frameHeight;
					frameCoords[frameIndex].z = (u + 1) * frameWidth;
					frameCoords[frameIndex].w = (v + 1) * frameHeight;
				}
			}
			
			for (int i = 0; i < animationNum; i++) {
				String prefix = AnimationToken + i;
				String animationName = parser.getString(prefix + AnimationNameToken);
				int animationStartFrame = parser.getInt(prefix + AnimationStartFrameToken);
				int animationFrameNum = parser.getInt(prefix + AnimationFrameNumToken);
				float[] durations = parser.getFloatArray(prefix + AnimationDurationsOfFrameToken, AnimationTimeSeparator);
				
				AnimationFrame[] frames = new AnimationFrame[animationFrameNum];
				int frameIndex;
				float topLeftU;
				float topLeftV;
				float bottomRightU;
				float bottomRightV;
				for (int j = 0; j < animationFrameNum; j++) {
					frameIndex = animationStartFrame + j;
					topLeftU = frameCoords[frameIndex].x;
					topLeftV = frameCoords[frameIndex].y;
					bottomRightU = frameCoords[frameIndex].z;
					bottomRightV = frameCoords[frameIndex].w;
					
					frames[j] = new AnimationFrame(texture, topLeftU, topLeftV, bottomRightU, bottomRightV, durations[j]);
					Debug.log("SpriteLoader", animationName + " has loaded.");
				}
				
				sprite.addAnimation(animationName, new Animation(frames));
			}
		}
		
		return sprite;
	}
}
