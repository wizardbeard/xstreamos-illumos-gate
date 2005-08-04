/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_SYS_PX_ERR_IMPL_H
#define	_SYS_PX_ERR_IMPL_H

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Bit Error handling tables:
 * bit		Bit Number
 * counter	Counter for number of errors countered for this bit
 * err_handler	Error Handler Function
 * erpt_handler	Ereport Handler Function
 * class_name	Class Name used for sending ereports for this bit.
 */
typedef struct px_err_bit_desc {
	uint_t		bit;
	uint_t		counter;
	int		(*err_handler)();
	int		(*erpt_handler)();
	char		*class_name;
} px_err_bit_desc_t;

/*
 * Reg Error handling tables:
 *
 * enabled		enable this register error handler
 *
 * *intr_mask_p		bitmask for enabled interrupts
 * *log_mask_p		bitmask for logged  interrupts
 * *count_mask_p	bitmask for counted interrupts
 *
 * *err_bit_tbl		error bit table
 * err_bit_keys		number of entries in the error bit table.
 *
 * last_reg		last captured register
 * log_addr		interrupt log    register offset
 * enable_addr		interrupt enable register offset
 * status_addr		interrupt status register offset
 * clear_addr		interrupt clear  register offset
 *
 * *msg			error messages table
 */
typedef struct px_err_reg_desc {
	boolean_t		enabled;
	uint64_t		*intr_mask_p;
	uint64_t		*log_mask_p;
	uint64_t		*count_mask_p;
	px_err_bit_desc_t	*err_bit_tbl;
	uint_t			err_bit_keys;
	uint64_t		last_reg;
	uint32_t		log_addr;
	uint32_t		enable_addr;
	uint32_t		status_addr;
	uint32_t		clear_addr;
	char			*msg;
} px_err_reg_desc_t;

/*
 * Macro to create the error handling forward declaration
 *
 * The error handlers examines error, determine the nature of the error
 * and return error status in terms of PX_FATAL_HW | PX_FATAL_GOS | ...
 * terminology.
 */
#define	PX_ERR_BIT_HANDLE_DEC(n)	int px_err_ ## n ## _handle\
	(dev_info_t *rpdip, caddr_t csr_base, ddi_fm_error_t *derr, \
	px_err_reg_desc_t *err_reg_descr, px_err_bit_desc_t *err_bit_descr)
#define	PX_ERR_BIT_HANDLE(n)		px_err_ ## n ## _handle

/*
 * Predefined error handling functions.
 */
int px_err_fatal_hw_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_fatal_gos_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_fatal_stuck_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_fatal_sw_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_non_fatal_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_ok_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_unknown_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);

/*
 * Macro to create the ereport forward declaration
 */
#define	PX_ERPT_SEND_DEC(n)	int px_err_ ## n ## _send_ereport\
	(dev_info_t *rpdip, caddr_t csr_base, uint64_t ss_reg, \
	ddi_fm_error_t *derr, char *class_name)
#define	PX_ERPT_SEND(n)		px_err_ ## n ## _send_ereport


/*
 * JBC error handling and ereport forward declarations
 */

#define	PX_ERR_JBC_CLASS(n)	PCIEX_FIRE "." FIRE_JBC_ ## n

/*
 * Fire JBC error Handling Forward Declarations
 * the must-panic type errors such as PX_FATAL_GOS or
 * post-reset-diagnosed type error such as PX_FATAL_HW
 * are not furthur diagnosed here because there is no
 * justification to find out more as immediate error
 * handling. FMA DE will do the post analysis.
 */
int px_err_jbc_merge_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_jbc_jbusint_in_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_jbc_dmcint_odcd_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_jbc_csr_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);

/* Fire JBC error ereport Forward Declarations */
PX_ERPT_SEND_DEC(jbc_fatal);
PX_ERPT_SEND_DEC(jbc_merge);
PX_ERPT_SEND_DEC(jbc_in);
PX_ERPT_SEND_DEC(jbc_out);
PX_ERPT_SEND_DEC(jbc_odcd);
PX_ERPT_SEND_DEC(jbc_idc);
PX_ERPT_SEND_DEC(jbc_csr);


/*
 * DMC error handling and ereport forward declarations
 */

#define	PX_ERR_DMC_CLASS(n)	PCIEX_FIRE "." FIRE_DMC_ ## n

/* Fire Bit Error Handling Forward Declarations */
int px_err_imu_rbne_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_imu_pme_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_imu_eq_ovfl_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_mmu_rbne_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_mmu_tfa_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_mmu_tte_cae_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);
int px_err_mmu_tblwlk_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);

/* Fire Ereport Handling Forward Declarations */
PX_ERPT_SEND_DEC(imu_rds);
PX_ERPT_SEND_DEC(imu_scs);
PX_ERPT_SEND_DEC(imu);
PX_ERPT_SEND_DEC(mmu_tfar_tfsr);
PX_ERPT_SEND_DEC(mmu);

/*
 * PEC error handling and ereport forward declarations
 */

#define	PX_ERR_PEC_CLASS(n)	PCIEX_FIRE "." FIRE_PEC_ ## n

int px_err_tlu_lup_handle(dev_info_t *rpdip, caddr_t csr_base,
	ddi_fm_error_t *derr, px_err_reg_desc_t *err_reg_descr,
	px_err_bit_desc_t *err_bit_descr);

/* Fire Ereport Handling Forward Declarations */
PX_ERPT_SEND_DEC(pec_ilu);
PX_ERPT_SEND_DEC(pciex_rx_ue);
PX_ERPT_SEND_DEC(pciex_tx_ue);
PX_ERPT_SEND_DEC(pciex_rx_tx_ue);
PX_ERPT_SEND_DEC(pciex_ue);
PX_ERPT_SEND_DEC(pciex_ce);
PX_ERPT_SEND_DEC(pciex_rx_oe);
PX_ERPT_SEND_DEC(pciex_rx_tx_oe);
PX_ERPT_SEND_DEC(pciex_oe);
PX_ERPT_SEND_DEC(pciex_lup);
PX_ERPT_SEND_DEC(pciex_ldn);

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_PX_ERR_IMPL_H */
