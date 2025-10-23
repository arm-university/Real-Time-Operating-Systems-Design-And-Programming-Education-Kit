t
  @verbatim
  ******************************************************************************
  *
  *         Portions COPYRIGHT 2016-2019 STMicroelectronics, All Rights Reserved
  *         Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  *
  * @file    st_readme.txt 
  * @author  MCD Application Team
  * @brief   This file lists the main modification done by STMicroelectronics on
  *          trustedfirmware for integration with STM32Cube solution.
  ******************************************************************************
  *
  * original licensing conditions
  * as issued by SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  @endverbatim

### 22-June-2020 ###
==========================
    Fix perso area offset in iat-verifier tool
    Add shebang in checkiat.sh script

### 16-June-2020 ###
==========================
    Remove duplicated msp setting as done in jumper

### 04-June-2020 ###
==========================
    Fix compile warnings
    BL2: Robustness improvement: resistance to basic hw attack
    BL2: Rename of bl2_main() to main()
    BL2: Put boot_jump_to_next_image() function is specific NoHDP section
    BL2: imgtool: add command do_flash : to update hardening.sh, regression.sh, TFM_Update.sh
    BL2: Add support of second flash device (ospi external flash) in mcu boot
    BL2: Remove __CC_ARM switch introduced for ".BL2_NoHdp_Code" section
    BL2: update directory  from TF-Mv1.0
    BL2: Adaptation for SBSFU_Boot
    BL2: add MCUBOOT_PRIMARY_ONLY
    BL2: modif for MCUBOOT_PRIMARY_ONLY and MCUBOOT_EXT_LOADER
    BL2: bl2_main : mbedtls min size to stay in SRAM2
    BL2: add MCUBOOT_IMAGE_NUMBER=1
    BL2: remove mcuboot_config.h
    BL2: Reduce BL2_MBEDTLS_MEM_BUF_LEN, to get BL2 RAM fitting in SRAM2
    qcbor: Fix long long conversion to double compiler warning in MDK-ARM
    cose: Fix crypto_struct.h path for t_cose

### 10-February-2020 ###
==========================
    Fix compile warnings
    BL2: Fix boot install when no valid image in primary slot
    fix to have nonsecure build independant of project installation
    iat_verifier: byte array displayed in hexadecimal and add build ass command
    add script to compute cbor and boot.binary without provision
    add script to help initial attestation check
    Fix TFM_SBSFU_Boot binary path in checkiat.sh script

### 02-December-2019 ###
==========================
    use  TF-Mv1.0-RC2
	BL2: fix key for secure /non secure authentication
	BL2 / TFM core : modification for IAR build
	TFM core : add interface to set non secure mpu

### 03-September-2019 ###
==========================
    TFM Crypto: Modify mbed-Crypto build config to use hardware for Random Generation
	BL2 Shared Data: Area is move on top of SRAM in dedicated place(no overlay) 
	TFM core: Add function to get boot value from BL2 Shared Data
    BL2:Add a weak function call in bl2_main to set BL2 Shared Data specific to STM32 
	BL2 Shared Data: Create TLV minor definition for TLV MAJOR Core to support BL2 Shared Data
	specific to STM32
	TFM EAT : fix to pass non regression test

### 08-July-2019 ###
========================
    use  TF-Mv1.0-RC1
	Modify mbed-crypto include to fix build issue
	Add windows executeable scripts in bl2/ext/mcuboot/scripts
	Remove gpio platform service not relevant for STM32 platform
	Add bl2_main function to allow STM32 specific main
	Add weak function to jump in install application for STM32 customization
	remove directory  platform/ext/target
