package com.dbylk.rwge.utils;

import java.util.HashMap;

import com.dbylk.rwge.android.utils.Debug;
import com.dbylk.rwge.files.RwgeFile;

import android.util.Log;

/** 
 * Parse simple text files to get tokens' value.
 * The rule of text file is :
 * 		tokenName = tokenValue
 * TokenName is not allowed to contain space while tokenValue is allowed to contain spaces.
 * A matched tokenName and tokenValue must be in the same line.
 *  */

public class Parser {
	HashMap<String, String> tokenMap;
	
	public Parser() {
		 
	}
	
	public Parser(RwgeFile file) {
		parseFile(file);
	}
	
	public void parseFile(RwgeFile file) {
		tokenMap = new HashMap<String, String>();
		
		String text = file.readString();
		
		String token;
		String value;
		
		int startIndex = 0;
		
		int i = 0;
		char c = text.charAt(i++);
		int length = text.length();
		while (i < length) {
			token = null;
			value = null;
			
			// Ignore spaces, tabs and line breaks.
			while ((c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0') && i < length) {
				c = text.charAt(i++);
			}
			
			// Get the token name.
			startIndex = i - 1;
			while (c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '\0' && i < length) {
				c = text.charAt(i++);
			}
			if (i < length) {
				token = text.substring(startIndex, i - 1);
			}
			else {
				token = text.substring(startIndex, i);
			}
			
			// Ignore spaces and tabs.
			while ((c == ' ' || c == '\t') && i < length) {
				c = text.charAt(i++);
			}
						
			// Get the character "=". If there is no "=", send warnings and return.
			if (c == '=') {
				c = text.charAt(i++);
			}
			else {
				Log.w("Parser", "The input file's grammar is illegal: \"=\" is missing. Operation is interrupted.");
				
				return;
			}
			
			// Ignore spaces and tabs.
			while ((c == ' ' || c == '\t') && i < length) {
				c = text.charAt(i++);
			}
			
			// Get the token value.
			startIndex = i - 1;
			while (c != '\n' && c != '\r' && c != '\0' && i < length) {
				c = text.charAt(i++);
			}
			if (i < length) {
				value = text.substring(startIndex, i - 1);
			}
			else {
				value = text.substring(startIndex, i);
			}
			
			
			// Check the integrity of token.
			if (token == null || value == null || token.isEmpty() || value.isEmpty()) {
				Log.w("Parser", "The input file's grammar is illegal: incomplete token. Operation is interrupted.");
				
				return;
			}
			
			// Put the token into the hash map, check the token whether it does exist.
			if (tokenMap.put(token, value) != null) {
				Log.w("Parser", "The token \"" + token + "\" does already exist and it has been updated with value \"" + value + "\".");
			}
			
			Debug.log("Parser", "Token (" + token + ", " + value + ") has been added.");
		}
	}
	
	/** @Return the value of this token as string. Return null if there is no such token. */
	public String getString(String token) {
		if (tokenMap != null) {
			return tokenMap.get(token);
		}
		
		return null;
	}
	
	/** @Return the value of this token as float. Return 0.0f if there is no such token. */
	public float getFloat(String token) {
		if (tokenMap != null) {
			return Float.parseFloat(tokenMap.get(token));
		}
		
		return 0f;
	}
	
	/** @Return the value of this token as integer. Return 0 if there is no such token. */
	public int getInt(String token) {
		if (tokenMap != null) {
			return Integer.parseInt(tokenMap.get(token));
		}
		
		return 0;
	}
	
	/** @Return the value of this token as float array. Return null if there is no such token. */
	public float[] getFloatArray(String token, String separator) {
		if (tokenMap != null) {
			String[] floatStringArray = tokenMap.get(token).split(separator);
			int length = floatStringArray.length;
			
			if (length == 0) {
				return null;
			}
			
			float[] floatArray = new float[length];
			for (int i = 0; i < length; i++) {
				floatArray[i] = Float.parseFloat(floatStringArray[i]);
			}
			
			return floatArray;
		}
		
		return null;
	}
	
	/** @Return the value of this token as integer array. Return null if there is no such token. */
	public int[] getIntArray(String token, String separator) {
		if (tokenMap != null) {
			String[] intStringArray = tokenMap.get(token).split(separator);
			int length = intStringArray.length;
			
			if (length == 0) {
				return null;
			}
			
			int[] intArray = new int[length];
			for (int i = 0; i < length; i++) {
				intArray[i] = Integer.parseInt(intStringArray[i]);
			}
			
			return intArray;
		}
		
		return null;
	}
}
