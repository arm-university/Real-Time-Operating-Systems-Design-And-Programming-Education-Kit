# roots.cmake
set(CMSIS_PACK_ROOT "C:/Users/tainek01/AppData/Local/arm/packs" CACHE PATH "CMSIS pack root")
cmake_path(ABSOLUTE_PATH CMSIS_PACK_ROOT NORMALIZE OUTPUT_VARIABLE CMSIS_PACK_ROOT)
set(CMSIS_COMPILER_ROOT "C:/Users/tainek01/.vcpkg/artifacts/2139c4c6/tools.open.cmsis.pack.cmsis.toolbox/2.7.0/etc" CACHE PATH "CMSIS compiler root")
cmake_path(ABSOLUTE_PATH CMSIS_COMPILER_ROOT NORMALIZE OUTPUT_VARIABLE CMSIS_COMPILER_ROOT)
set(SOLUTION_ROOT "C:/Users/tainek01/OneDrive - Arm/Desktop/en.stm32cubel5_v1-5-0/STM32Cube_FW_L5_V1.5.0/Projects/NUCLEO-L552ZE-Q/Applications/FreeRTOS/FreeRTOS_Mutexes/MDK-ARM" CACHE PATH "CMSIS solution root")
cmake_path(ABSOLUTE_PATH SOLUTION_ROOT NORMALIZE OUTPUT_VARIABLE SOLUTION_ROOT)
