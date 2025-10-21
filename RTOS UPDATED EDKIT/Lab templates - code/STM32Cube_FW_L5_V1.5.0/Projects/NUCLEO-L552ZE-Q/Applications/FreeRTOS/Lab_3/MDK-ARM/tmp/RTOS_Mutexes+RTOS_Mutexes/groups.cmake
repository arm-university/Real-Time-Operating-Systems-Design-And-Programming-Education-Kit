# groups.cmake

# group Application/MDK-ARM
add_library(Group_Application_MDK-ARM OBJECT
  "${SOLUTION_ROOT}/startup_stm32l552xx.s"
)
target_include_directories(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Application_MDK-ARM_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Application_MDK-ARM_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Application_MDK-ARM PUBLIC
  Group_Application_MDK-ARM_ABSTRACTIONS
)
set(COMPILE_DEFINITIONS
  __MICROLIB
  STM32L552xx
  _RTE_
)
cbuild_set_defines(AS_ARM COMPILE_DEFINITIONS)
set_source_files_properties("${SOLUTION_ROOT}/startup_stm32l552xx.s" PROPERTIES
  COMPILE_FLAGS "${COMPILE_DEFINITIONS}"
)
set_source_files_properties("${SOLUTION_ROOT}/startup_stm32l552xx.s" PROPERTIES
  COMPILE_OPTIONS "-masm=armasm"
)

# group Application/User
add_library(Group_Application_User OBJECT
  "${SOLUTION_ROOT}/../Src/main.c"
  "${SOLUTION_ROOT}/../Src/stm32l5xx_it.c"
  "${SOLUTION_ROOT}/../Src/stm32l5xx_hal_msp.c"
  "${SOLUTION_ROOT}/../Src/stm32l5xx_hal_timebase_tim.c"
  "${SOLUTION_ROOT}/../Src/app_freertos.c"
)
target_include_directories(Group_Application_User PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Application_User PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Application_User_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Application_User_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Application_User PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Application_User PUBLIC
  Group_Application_User_ABSTRACTIONS
)

# group Drivers/BSP/STM32L5xx_Nucleo
add_library(Group_Drivers_BSP_STM32L5xx_Nucleo OBJECT
  "${SOLUTION_ROOT}/../../../../../../Drivers/BSP/STM32L5xx_Nucleo/stm32l5xx_nucleo.c"
)
target_include_directories(Group_Drivers_BSP_STM32L5xx_Nucleo PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_BSP_STM32L5xx_Nucleo PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Drivers_BSP_STM32L5xx_Nucleo_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Drivers_BSP_STM32L5xx_Nucleo_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Drivers_BSP_STM32L5xx_Nucleo PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_BSP_STM32L5xx_Nucleo PUBLIC
  Group_Drivers_BSP_STM32L5xx_Nucleo_ABSTRACTIONS
)

# group Drivers/STM32L5xx_HAL_Driver
add_library(Group_Drivers_STM32L5xx_HAL_Driver OBJECT
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_exti.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_tim.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_tim_ex.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_i2c.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_i2c_ex.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_rcc.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_rcc_ex.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_flash.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_flash_ex.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_flash_ramfunc.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_gpio.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_dma.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_dma_ex.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_pwr.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_pwr_ex.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_cortex.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_gtzc.c"
  "${SOLUTION_ROOT}/../../../../../../Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_icache.c"
)
target_include_directories(Group_Drivers_STM32L5xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_STM32L5xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Drivers_STM32L5xx_HAL_Driver_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Drivers_STM32L5xx_HAL_Driver_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Drivers_STM32L5xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_STM32L5xx_HAL_Driver PUBLIC
  Group_Drivers_STM32L5xx_HAL_Driver_ABSTRACTIONS
)

# group Drivers/CMSIS
add_library(Group_Drivers_CMSIS OBJECT
  "${SOLUTION_ROOT}/../Src/system_stm32l5xx.c"
)
target_include_directories(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Drivers_CMSIS_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Drivers_CMSIS_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_CMSIS PUBLIC
  Group_Drivers_CMSIS_ABSTRACTIONS
)

# group Middlewares/FreeRTOS
add_library(Group_Middlewares_FreeRTOS OBJECT
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/croutine.c"
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c"
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/list.c"
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/queue.c"
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c"
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/tasks.c"
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/timers.c"
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c"
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c"
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure/port.c"
  "${SOLUTION_ROOT}/../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c"
)
target_include_directories(Group_Middlewares_FreeRTOS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Middlewares_FreeRTOS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Middlewares_FreeRTOS_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Middlewares_FreeRTOS_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Middlewares_FreeRTOS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Middlewares_FreeRTOS PUBLIC
  Group_Middlewares_FreeRTOS_ABSTRACTIONS
)
