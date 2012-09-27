
#ifndef _INDICOLOG_H_
#define _INDICOLOG_H_

#include <android/log.h>

#define LOGI(source, ...) ((void)__android_log_print(ANDROID_LOG_INFO, source, __VA_ARGS__))
#define LOGE(source, ...) ((void)__android_log_print(ANDROID_LOG_ERROR, source, __VA_ARGS__))

#endif
