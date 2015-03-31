/*
 * rwge_graphics_TextureData.cpp
 *
 *  Created on: 2015Äê3ÔÂ30ÈÕ
 *      Author: DaBian
 */

#include "rwge_graphics_TextureData.h"

JNIEXPORT jobject JNICALL Java_rwge_graphics_TextureData_intArraytoByteBuffer(JNIEnv *env, jobject obj, jintArray array, jint length) {
	jobject pixelBuffer = env->NewDirectByteBuffer((void*)array, length);

	return pixelBuffer;
}
