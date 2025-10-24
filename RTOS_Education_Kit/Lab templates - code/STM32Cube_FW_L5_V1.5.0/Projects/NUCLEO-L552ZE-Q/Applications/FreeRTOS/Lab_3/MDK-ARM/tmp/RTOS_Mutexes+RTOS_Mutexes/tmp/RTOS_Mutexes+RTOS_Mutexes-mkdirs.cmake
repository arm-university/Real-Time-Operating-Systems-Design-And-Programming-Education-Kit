# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/tainek01/OneDrive - Arm/Desktop/en.stm32cubel5_v1-5-0/STM32Cube_FW_L5_V1.5.0/Projects/NUCLEO-L552ZE-Q/Applications/FreeRTOS/FreeRTOS_Mutexes/MDK-ARM/tmp/RTOS_Mutexes+RTOS_Mutexes"
  "C:/Users/tainek01/OneDrive - Arm/Desktop/en.stm32cubel5_v1-5-0/STM32Cube_FW_L5_V1.5.0/Projects/NUCLEO-L552ZE-Q/Applications/FreeRTOS/FreeRTOS_Mutexes/MDK-ARM/tmp/1"
  "C:/Users/tainek01/OneDrive - Arm/Desktop/en.stm32cubel5_v1-5-0/STM32Cube_FW_L5_V1.5.0/Projects/NUCLEO-L552ZE-Q/Applications/FreeRTOS/FreeRTOS_Mutexes/MDK-ARM/tmp/RTOS_Mutexes+RTOS_Mutexes"
  "C:/Users/tainek01/OneDrive - Arm/Desktop/en.stm32cubel5_v1-5-0/STM32Cube_FW_L5_V1.5.0/Projects/NUCLEO-L552ZE-Q/Applications/FreeRTOS/FreeRTOS_Mutexes/MDK-ARM/tmp/RTOS_Mutexes+RTOS_Mutexes/tmp"
  "C:/Users/tainek01/OneDrive - Arm/Desktop/en.stm32cubel5_v1-5-0/STM32Cube_FW_L5_V1.5.0/Projects/NUCLEO-L552ZE-Q/Applications/FreeRTOS/FreeRTOS_Mutexes/MDK-ARM/tmp/RTOS_Mutexes+RTOS_Mutexes/src/RTOS_Mutexes+RTOS_Mutexes-stamp"
  "C:/Users/tainek01/OneDrive - Arm/Desktop/en.stm32cubel5_v1-5-0/STM32Cube_FW_L5_V1.5.0/Projects/NUCLEO-L552ZE-Q/Applications/FreeRTOS/FreeRTOS_Mutexes/MDK-ARM/tmp/RTOS_Mutexes+RTOS_Mutexes/src"
  "C:/Users/tainek01/OneDrive - Arm/Desktop/en.stm32cubel5_v1-5-0/STM32Cube_FW_L5_V1.5.0/Projects/NUCLEO-L552ZE-Q/Applications/FreeRTOS/FreeRTOS_Mutexes/MDK-ARM/tmp/RTOS_Mutexes+RTOS_Mutexes/src/RTOS_Mutexes+RTOS_Mutexes-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/tainek01/OneDrive - Arm/Desktop/en.stm32cubel5_v1-5-0/STM32Cube_FW_L5_V1.5.0/Projects/NUCLEO-L552ZE-Q/Applications/FreeRTOS/FreeRTOS_Mutexes/MDK-ARM/tmp/RTOS_Mutexes+RTOS_Mutexes/src/RTOS_Mutexes+RTOS_Mutexes-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/tainek01/OneDrive - Arm/Desktop/en.stm32cubel5_v1-5-0/STM32Cube_FW_L5_V1.5.0/Projects/NUCLEO-L552ZE-Q/Applications/FreeRTOS/FreeRTOS_Mutexes/MDK-ARM/tmp/RTOS_Mutexes+RTOS_Mutexes/src/RTOS_Mutexes+RTOS_Mutexes-stamp${cfgdir}") # cfgdir has leading slash
endif()
