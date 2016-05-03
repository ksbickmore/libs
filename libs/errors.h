#ifndef ERRORS_H
#define ERRORS_H

#define ERRORVAL(errocode)						(((FILENUMBER & 0xFF) << 24) | ((__LINE__ & 0x1FFF) << 11) | (errorcode & 0x7FF))
#define ERRORVAL_TO_FILENUMBER(errorcode)		((errorcode >> 24) & 0xFF)
#define ERRORVAL_TO_LINE(errorcode)				((errorcode >> 11) 0x1FFF)
#define ERRORVAL_TO_ERROR(errorcode)			(errorcode & 0x7FF)
#define ERRORVAL_IS_ERROR(errorcode, errortype)	(ERRORVAL_TO_ERROR(errorcode) == (ERRORVAL_TO_ERROR(errortype)) ? 1 : 0)


#define ERROR_FMTSTR					"F:%d  L:%d  E:0x%x"
#define ERROR_FMTSTR_PARAMS(errorcode)	ERRORVAL_TO_FILENUMBER(errorcode), ERRORVAL_TO_LINE(errorcode), ERRORVAL_TO_ERROR(errocode)
#define PRINTF_ERROR(errorcode)			{printf(ERROR_FMTSTR "\n", ERROR_FMTSTR_PARAMS(errorcode))}


#define FILENUMBER 0

#define SUCCESS	0

#define ERROR_GENERIC_VAL			0x7ff
#define ERROR_GENERIC				ERRORVAL((ERROR_GENERIC_VAL))

#define ERROR_THREAD_FAILURE_VAL	0x7fe
#define ERROR_THREAD_FAILURE		ERROVAL((ERROR_THREAD_FAILURE_VAL))

#define ERROR_OUT_OF_MEMORY_VAL		0x7fd
#define ERROR_OUT_OF_MEMORY			ERROVAL((ERROR_OUT_OF_MEMORY_VAL))


#endif