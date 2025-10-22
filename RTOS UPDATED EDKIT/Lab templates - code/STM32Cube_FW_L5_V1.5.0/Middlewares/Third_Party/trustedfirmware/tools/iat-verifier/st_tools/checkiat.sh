#!/bin/bash -
# EWARM PATH
tfm_sbsfu_binary=../../../../../../Projects/STM32L562E-DK/Applications/TFM/TFM_SBSFU_Boot/EWARM/STM32L562E-DK_TFM_SBSFU_Boot/Exe/Project.bin
#Cube IDE PATh
#tfm_sbsfu_binary=../../../../../../Projects/STM32L562E-DK/Applications/TFM/TFM_SBSFU_Boot/STM32CubeIDE/Release/TFM_SBSFU_Boot.bin
# KEIL PATH
#tfm_sbsfu_binary=../../../../../../Projects/STM32L562E-DK/Applications/TFM/TFM_SBSFU_Boot/MDK-ARM/STM32L562E-DK/Exe/Project.bin
python build.py cbor ./eat.txt ./eat.cbor
check_iat -k ../../../platform/ext/common/tfm_initial_attestation_key.pem  ./eat.cbor -p
python build.py boot $tfm_sbsfu_binary ./sbsfu.bin
sha256sum ./sbsfu.bin
