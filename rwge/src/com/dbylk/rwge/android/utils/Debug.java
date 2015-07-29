package com.dbylk.rwge.android.utils;

import android.util.Log;

/** Used to control the log cat's debug information globally. */
/** The default setting of hiding log. */

public class Debug {
	static boolean showDebugLog = false;
	
	public static void showLog() {
		showDebugLog = true;
	}
	
	public static void hideLog() {
		showDebugLog = false;
	}
	
	public static void log(String tag, String msg) {
		if (showDebugLog) {
			Log.d(tag, msg);
		}
	}
}
