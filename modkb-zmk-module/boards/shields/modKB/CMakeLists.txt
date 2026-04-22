if("${SHIELD}" STREQUAL "ModKB_KP" AND CONFIG_ZMK_SPLIT_PERIPHERAL_HID_INDICATORS) 
    # Expose ZMK app public headers to kp_numlock_indicator
    zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
    zephyr_library_sources(kp_numlock_indicator.c)
endif()