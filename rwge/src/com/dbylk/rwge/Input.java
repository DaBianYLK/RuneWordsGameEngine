package com.dbylk.rwge;

import com.dbylk.rwge.input.AccelerometerListener;
import com.dbylk.rwge.input.CompassListener;
import com.dbylk.rwge.input.TouchListener;

/** Interface to access input devices. */

public interface Input {
	/** @return the value of the accelerometer on its x-axis, range between [-10, 10]. */
	public float getAccelerometerX();
	/** @return the value of the accelerometer on its y-axis, range between [-10, 10]. */
	public float getAccelerometerY();
	/** @return the value of the accelerometer on its z-axis, range between [-10, 10]. */
	public float getAccelerometerZ();

	public float getCompassX();
	public float getCompassY();
	public float getCompassZ();
	
	/** @return the last touch location's x-coordinate, the unit is pixel of screen. */
	public int getX();
	/** @return the last touch location's y-coordinate, the unit is pixel of screen. */
	public int getY();
	/** @return the different between the last touch's begin and end locations on the x-axis. */
	public int getDeltaX();
	/** @return the different between the last touch's begin and end location on the y-axis. */
	public int getDeltaY();
	/** @return the last touch location's x-coordinate in a multi-touch specified by pointer index, the unit is pixel of screen. */
	public int getX(int pointer);
	/** @return the last touch location's y-coordinate in a multi-touch specified by pointer index, the unit is pixel of screen. */
	public int getY(int pointer);
	/** @return the different between the last touch's begin and end location on the x-axis in a multi-touch specified by pointer index. */
	public int getDeltaX(int pointer);
	/** @return the different between the last touch's begin and end location on the y-axis in a multi-touch specified by pointer index. */
	public int getDeltaY(int pointer);
	/** @return the number of pointer in last touch. */
	public int getPointerCount();
	/** @return whether pointer is currently touched. */
	public boolean isTouched(int pointer);
	
	public boolean supportMultiTouch();
	
	public float getScreenRotation();
	
	/** @return the native orientation of the device. */
	public Orientation getNativeOrientation();

	public boolean isDeviceAvailable(Device device);
	
	public void addTouchListener(TouchListener listener);
	public void addAccelerometerListener(AccelerometerListener listener);
	public void addCompassListener(CompassListener listener);

	
	public enum Orientation {
		Landscape, 
		Portrait
	}
	
	public enum Device {
		Accelerometer,
		Compass
	}
}
