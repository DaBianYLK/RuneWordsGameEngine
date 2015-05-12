package com.dbylk.rwge.android.utils;

import android.util.Log;

/** Used to control the log cat's debug information. */
/** The default setting of showing log is off. */

public class Debug {
	static boolean showDebugLog = false;
	
	public static void showLog() {
		showDebugLog = true;
	}
	
	public static void hideLog() {
		showDebugLog = false;
	}
}
