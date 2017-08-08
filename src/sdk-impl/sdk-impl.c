/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include "sdk-impl_internal.h"

void IOT_OpenLog(const char *ident)
{
    const char         *mod = ident;

    if (NULL == mod) {
        mod = "---";
    }
    return LITE_openlog(mod);
}

void IOT_CloseLog(void)
{
    return LITE_closelog();
}

void IOT_SetLogLevel(IOT_LogLevel level)
{
    LOGLEVEL            lvl = level;

    if (lvl < LOG_EMERG_LEVEL || lvl > LOG_DEBUG_LEVEL) {
        log_err("Invalid input level: %d out of [%d, %d]", level,
                LOG_EMERG_LEVEL,
                LOG_DEBUG_LEVEL);
        return;
    }

    return LITE_set_loglevel(lvl);
}

void IOT_DumpMemoryStats(IOT_LogLevel level)
{
    LOGLEVEL            lvl = level;

    if (lvl < LOG_EMERG_LEVEL || lvl > LOG_DEBUG_LEVEL) {
        lvl = LOG_DEBUG_LEVEL;
        log_warning("Invalid input level, using default: %d => %d", level, lvl);
    }

    return LITE_dump_malloc_free_stats(lvl);
}

int IOT_SetupConnInfo(const char *product_key,
            const char *device_name,
            const char *device_secret,
            void **info_ptr)
{
    int                 rc = -1;

    iotx_device_info_init();
    iotx_device_info_set(product_key, device_name, device_secret);

    rc = iotx_guider_authenticate();
    if (rc == 0) {
        *info_ptr = (void *)iotx_conn_info_get();
        return 0;
    } else {
        *info_ptr = NULL;
        return -1;
    }

    return 0;
}
