# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

set(UNICORN_ENGINE_SYSTEM_DIR
    "${UNICORN_ENGINE_ROOT}/system"
)

set(UNICORN_ENGINE_SYSTEM_LIB
    system
)

## WindowManager
include(WindowManager)

set(UNICORN_ENGINE_SYSTEM_INCLUDE_DIRS
    ${WINDOW_MANAGER_INCLUDE_DIR}
)
