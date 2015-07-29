package com.dbylk.rwge.android;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Vibrator;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.View.OnTouchListener;

import com.dbylk.rwge.Input;
import com.dbylk.rwge.RenderListener;
import com.dbylk.rwge.android.utils.Debug;
import com.dbylk.rwge.input.AccelerometerListener;
import com.dbylk.rwge.input.CompassListener;
import com.dbylk.rwge.input.TouchListener;


public class AndroidInput implements Input, OnKeyListener, OnTouchListener, RenderListener, SensorEventListener {
	AndroidApplication application;
	AndroidAppConfig config;
	SensorManager sensorManager;
	
	private final float[] accelerometerValues = new float[3];
	private boolean accelerometerAvailable = false;
	private ArrayList<AccelerometerListener> accelerometerListeners = new ArrayList<AccelerometerListener>();
	
	private final float[] compassValues = new float[3];
	private boolean compassAvailable = false;
	private ArrayList<CompassListener> compassListeners = new ArrayList<CompassListener>();
	
	private Vibrator vibrator;
	
	private final Orientation nativeOrientation;
	
	boolean supportMultiTouch;
	private final int MultiTouchNum = 20;
	private final int[] pointerId = new int[MultiTouchNum];
	private final int[] touchX = new int[MultiTouchNum];
	private final int[] touchY = new int[MultiTouchNum];
	private final int[] deltaX = new int[MultiTouchNum];
	private final int[] deltaY = new int[MultiTouchNum];
	private final boolean[] isTouched = new boolean[MultiTouchNum];
	private int pointerCount;
	private ArrayList<TouchListener> touchListeners = new ArrayList<TouchListener>();
	
	public AndroidInput(AndroidApplication application, AndroidAppConfig config) {
		this.application = application;
		
		View view = application.graphics.getView();
		view.setOnKeyListener(this);
		view.setOnTouchListener(this);
		view.setFocusable(true);
		view.setFocusableInTouchMode(true);
		view.requestFocus();
		
		this.config = config;
		
		vibrator = (Vibrator)application.getSystemService(Context.VIBRATOR_SERVICE);
		
		float rotation = getScreenRotation();
		int width = application.getGraphics().getWidth();
		int height = application.getGraphics().getHeight();
		if (((rotation ==  0f || rotation == 180f) && width >= height) || 
			((rotation == 90f || rotation == 270f) && width <= height)) {
			nativeOrientation = Orientation.Landscape;
		}
		else {
			nativeOrientation = Orientation.Portrait;
		}
		
		supportMultiTouch = application.getPackageManager().hasSystemFeature("android.hardware.touchscreen.multitouch");
		resetPointers();
		
		registerSensorListeners();
	}
	
	private void resetPointers() {
		Arrays.fill(pointerId, -1);
		Arrays.fill(touchX, -1);
		Arrays.fill(touchY, -1);
		Arrays.fill(deltaX, -1);
		Arrays.fill(deltaY, -1);
		Arrays.fill(isTouched, false);
		pointerCount = 0;
	}
	
	/** @return the index of pointer by its id. Return -1 if there is no such pointer id. */
	private int getPointerIndex(int pointerID) {
		for (int i = 0; i < MultiTouchNum; i++) {
			if (this.pointerId[i] == pointerID) {
				return i;
			}
		}
		
		Debug.log("AndroidInput", "Pointer id " + pointerID + "is not found.");
		
		return -1;
	}
	
	/** @return the first free index of pointer. Return -1 if there is no free index to use. */
	private int getFreePointerIndex() {
		for (int i = 0; i < MultiTouchNum; i++) {
			if (this.pointerId[i] == -1) {
				return i;
			}
		}
		
		Debug.log("AndroidInput", "Find free pointer index failed.");
		
		return -1;
	}
	
	@Override
	public float getAccelerometerX() {
		return accelerometerValues[0];
	}

	@Override
	public float getAccelerometerY() {
		return accelerometerValues[1];
	}

	@Override
	public float getAccelerometerZ() {
		return accelerometerValues[2];
	}
	
	@Override
	public float getCompassX() {
		return compassValues[0];
	}

	@Override
	public float getCompassY() {
		return compassValues[1];
	}

	@Override
	public float getCompassZ() {
		return compassValues[2];
	}

	@Override
	public int getX() {
		synchronized (this) {
			return touchX[0];
		}
	}

	@Override
	public int getY() {
		synchronized (this) {
			return touchY[0];
		}
	}

	@Override
	public int getDeltaX() {
		synchronized (this) {
			return deltaX[0];
		}
	}

	@Override
	public int getDeltaY() {
		synchronized (this) {
			return deltaY[0];
		}
	}

	@Override
	public int getX(int pointer) {
		synchronized (this) {
			return touchX[pointer];
		}
	}

	@Override
	public int getY(int pointer) {
		synchronized (this) {
			return touchY[pointer];
		}
	}

	@Override
	public int getDeltaX(int pointer) {
		synchronized (this) {
			return deltaX[pointer];
		}
	}

	@Override
	public int getDeltaY(int pointer) {
		synchronized (this) {
			return deltaY[pointer];
		}
	}
	
	@Override
	public int getPointerCount() {
		synchronized (this) {
			return pointerCount;
		}
	}

	@Override
	public boolean isTouched(int pointer) {
		synchronized (this) {
			return isTouched[pointer];
		}
	}
	
	@Override
	public boolean supportMultiTouch() {
		return supportMultiTouch;
	}

	@Override
	public float getScreenRotation() {
		int orientation = application.getWindowManager().getDefaultDisplay().getRotation();

		switch (orientation) {
		case Surface.ROTATION_0:
			return 0f;
		case Surface.ROTATION_90:
			return 90f;
		case Surface.ROTATION_180:
			return 180f;
		case Surface.ROTATION_270:
			return 270f;
		default:
			return 0f;
		}
	}
	
	@Override
	public Orientation getNativeOrientation() {
		return nativeOrientation;
	}
	
	@Override
	public boolean isDeviceAvailable(Device device) {
		if (device == Device.Accelerometer) {
			return accelerometerAvailable;
		}
		if (device == Device.Compass) {
			return compassAvailable;
		}
		
		return false;
	}

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		final int action = event.getAction() & MotionEvent.ACTION_MASK;
		int pointerIndexInEvent = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
		int id = event.getPointerId(pointerIndexInEvent);
		
		int pointerIndex;

		synchronized (this) {
			switch (action) {
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
				pointerIndex = getFreePointerIndex();
				if (pointerIndex == -1) {
					break;
				}
				pointerId[pointerIndex] = id;
				touchX[pointerIndex] = (int)event.getX(pointerIndexInEvent);
				touchY[pointerIndex] = (int)event.getY(pointerIndexInEvent);
				deltaX[pointerIndex] = 0;
				deltaY[pointerIndex] = 0;
				isTouched[pointerIndex] = true;
				break;

			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_OUTSIDE:
			case MotionEvent.ACTION_CANCEL:
				pointerIndex = getPointerIndex(id);
				if (pointerIndex == -1) {
					break;
				}
				touchX[pointerIndex] = (int)event.getX(pointerIndexInEvent);
				touchY[pointerIndex] = (int)event.getY(pointerIndexInEvent);
				deltaX[pointerIndex] = 0;
				deltaY[pointerIndex] = 0;
				isTouched[pointerIndex] = false;
				break;

			case MotionEvent.ACTION_MOVE:
				int x;
				int y;
				int pointerCount = event.getPointerCount();
				for (int i = 0; i < pointerCount; i++) {
					id = event.getPointerId(i);
					pointerIndex = getPointerIndex(id);
					if (pointerIndex == -1) {
						continue;
					}

					x = (int)event.getX(pointerIndexInEvent);
					y = (int)event.getY(pointerIndexInEvent);
					deltaX[pointerIndex] = x - touchX[pointerIndex];
					deltaY[pointerIndex] = y - touchY[pointerIndex];
					touchX[pointerIndex] = x;
					touchY[pointerIndex] = y;
				}
				break;
			}
		}
		
		synchronized (touchListeners) {
			for (TouchListener touchListener : touchListeners) {
				touchListener.onTouched(event);
			}
		}

		return true;
	}
	
	public void addTouchListener(TouchListener listener) {
		touchListeners.add(listener);
	}

	@Override
	public boolean onKey(View v, int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		return false;
	}
	
	private void registerSensorListeners() {
		if (config.useAccelerometer) {
			sensorManager = (SensorManager)application.getSystemService(Context.SENSOR_SERVICE);
			if (sensorManager.getSensorList(Sensor.TYPE_ACCELEROMETER).size() == 0) {
				accelerometerAvailable = false;
			} else {
				Sensor accelerometer = sensorManager.getSensorList(Sensor.TYPE_ACCELEROMETER).get(0);
				accelerometerAvailable = sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_GAME);
			}
		} 
		else {
			accelerometerAvailable = false;
		}

		if (config.useCompass) {
			if (sensorManager == null) {
				sensorManager = (SensorManager)application.getSystemService(Context.SENSOR_SERVICE);
			}
			Sensor sensor = sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
			if (sensor != null) {
				compassAvailable = accelerometerAvailable;
				if (compassAvailable) {
					compassAvailable = sensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_GAME);
				}
			} 
			else {
				compassAvailable = false;
			}
		} 
		else {
			compassAvailable = false;
		}
		
		Debug.log("AndroidInput", "Sensor listener has been registered.");
	}

	private void unregisterSensorListeners() {
		if (sensorManager != null) {
			sensorManager.unregisterListener(this);
			sensorManager = null;
			
			accelerometerAvailable = false;
			compassAvailable = false;
		}
		
		Debug.log("AndroidInput", "Sensor listener has been unregistered.");
	}
	
//	private class SensorListener implements SensorEventListener {
//		final float[] accelerometerValues;
//		final float[] compassValues;
//		final Orientation nativeOrientation;
//
//		SensorListener (Orientation nativeOrientation, float[] accelerometerValues, float[] compassValues) {
//			this.nativeOrientation = nativeOrientation;
//			this.accelerometerValues = accelerometerValues;
//			this.compassValues = compassValues;
//		}
//
//		@Override
//		public void onAccuracyChanged (Sensor arg0, int arg1) {
//
//		}
//
//		@Override
//		public void onSensorChanged (SensorEvent event) {
//			if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
//				if (nativeOrientation == Orientation.Portrait) {
//					System.arraycopy(event.values, 0, accelerometerValues, 0, accelerometerValues.length);
//				} else {
//					accelerometerValues[0] = event.values[1];
//					accelerometerValues[1] = -event.values[0];
//					accelerometerValues[2] = event.values[2];
//				}
//			}
//			if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
//				System.arraycopy(event.values, 0, compassValues, 0, compassValues.length);
//			}
//		}
//	}

	@Override
	public void onSensorChanged(SensorEvent event) {
		if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
			if (nativeOrientation == Orientation.Portrait) {
				System.arraycopy(event.values, 0, accelerometerValues, 0, accelerometerValues.length);
			} else {
				accelerometerValues[0] = event.values[1];
				accelerometerValues[1] = -event.values[0];
				accelerometerValues[2] = event.values[2];
			}
			
			synchronized (accelerometerListeners) {
				for (AccelerometerListener accelerometerListener : accelerometerListeners) {
					accelerometerListener.onAccelerometerChanged(accelerometerValues[0], accelerometerValues[1], accelerometerValues[2]);
				}
			}
		}
		if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
			System.arraycopy(event.values, 0, compassValues, 0, compassValues.length);
			
			synchronized (compassListeners) {
				for (CompassListener compassListener : compassListeners) {
					compassListener.onCompassChanged(compassValues[0], compassValues[1], compassValues[2]);
				}
			}
		}
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
	}
	
	public void addAccelerometerListener(AccelerometerListener listener) {
		accelerometerListeners.add(listener);
	}
	
	public void addCompassListener(CompassListener listener) {
		compassListeners.add(listener);
	}

	@Override
	public void render(float deltaTime) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void pause() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void resume() {
		registerSensorListeners();
	}

	@Override
	public void dispose() {
		unregisterSensorListeners();
	}
}
