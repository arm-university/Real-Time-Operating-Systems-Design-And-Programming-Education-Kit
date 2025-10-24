/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "platform_sp.h"

#include "platform/include/tfm_platform_system.h"
#include "secure_fw/include/tfm_spm_services_api.h"

enum tfm_platform_err_t platform_sp_init(void)
{
    /* Nothing to be done */

    return TFM_PLATFORM_ERR_SUCCESS;
}

enum tfm_platform_err_t platform_sp_system_reset(void)
{
    /* Check if SPM allows the system reset */

    if (tfm_spm_request_reset_vote() != 0) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    /* FIXME: The system reset functionality is only supported in isolation
     *        level 1.
     */

    tfm_platform_hal_system_reset();

    return TFM_PLATFORM_ERR_SUCCESS;
}

enum tfm_platform_err_t
platform_sp_ioctl(psa_invec  *in_vec,  uint32_t num_invec,
                  psa_outvec *out_vec, uint32_t num_outvec)
{
    void *input, *output;
    tfm_platform_ioctl_req_t request;

    if ((num_invec < 1) || (num_invec > 2) ||
        (num_outvec > 1) ||
        (in_vec[0].base == NULL) ||
        (in_vec[0].len != sizeof(tfm_platform_ioctl_req_t))) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    input = (num_invec == 1) ? NULL : &in_vec[1];
    output = out_vec;
    request = *((tfm_platform_ioctl_req_t *)in_vec[0].base);

    return tfm_platform_hal_ioctl(request, input, output);
}

