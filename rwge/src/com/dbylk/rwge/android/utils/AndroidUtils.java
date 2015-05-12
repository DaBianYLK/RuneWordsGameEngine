package com.dbylk.rwge.android.utils;

import android.app.Activity;
import android.content.res.Resources;

public final class AndroidUtils {
	private static Activity activity;
	
	private AndroidUtils() {
		
	}
	
	public static void initialize(Activity a) {
		activity = a;
	}
	
	public static Resources getResources(){
		return activity.getResources();
	}
}
