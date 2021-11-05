#include <android/log.h>
#include <core/assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <jni.h>

static JNIEnv * restrict global_env_ptr = NULL;
static jclass global_class = NULL;
static const char * module = NULL;

int main(const int argc, char * const * const argv);

JNIEXPORT jint JNICALL startModule(JNIEnv * const restrict env_ptr, const jobject obj, const jarray jarg_arr) {
	const JNIEnv env = *env_ptr;

	const int argc = env->GetArrayLength(env_ptr, jarg_arr);
	const char ** const restrict argv = malloc(argc * sizeof(argv[0]));
	jstring * const restrict jargs = malloc(argc * sizeof(jargs[0]));

	for (int i = 0; i < argc; ++i) {
		jargs[i] = env->GetObjectArrayElement(env_ptr, jarg_arr, i);
		argv[i] = env->GetStringUTFChars(env_ptr, jargs[i], NULL);
	}

	global_class = env->GetObjectClass(env_ptr, obj);
	global_env_ptr = env_ptr;
	module = argv[0];

	const int code = main(argc, (char * const *)argv);

	for (int i = 0; i < argc; ++i) {
		env->ReleaseStringUTFChars(env_ptr, jargs[i], argv[i]);
	}

	free(jargs);
	free(argv);

	return code;
}

_Noreturn void exit(const int code) {
	const JNIEnv env = *global_env_ptr;
	
	const jmethodID exit_id = env->GetMethodID(global_env_ptr, global_class, "finishAffinity", "(Ljava/lang/Void;)V");
	env->CallStaticVoidMethod(global_env_ptr, global_class, exit_id, code);
	
	while (true); // Will never start
}

int fprintf(FILE * const file, const char * const format, ...) {
	int result;

	va_list args;
	va_start(args, format);

	if (file == stdout) {
		result = __android_log_vprint(ANDROID_LOG_INFO, module, format, args);
	} else if (file == stderr) {
		result = __android_log_vprint(ANDROID_LOG_ERROR, module, format, args);
	} else {
		result = vfprintf(file, format, args);
	}

	va_end(args);
	return result;
}

int puts(const char * const str) {
	return __android_log_write(ANDROID_LOG_INFO, module, str);
}

int printf(const char * const format, ...) {
	va_list args;
	va_start(args, format);

	const int result = __android_log_vprint(ANDROID_LOG_INFO, module, format, args);

	va_end(args);
	return result;
}
