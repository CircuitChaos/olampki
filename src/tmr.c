#include <avr/interrupt.h>
#include <inttypes.h>
#include "io.h"
#include "tmr.h"

static const uint8_t PWM_CHG_SPEED	= 3;
static const uint8_t PWM_NUM_LEVELS	= 64;
static const uint16_t BTN_SHORT		= 117;
static const uint16_t BTN_LONG		= 2344;

enum mode_t
{
	MODE_OFF,
	MODE_PWM,
	MODE_HOLD,
};

struct ctx_t
{
	enum mode_t mode;

	uint8_t pwm_cnt;
	uint8_t pwm_val;
	uint8_t pwm_chg_cnt;
	uint8_t pwm_chg_dir;

	uint16_t btn_cnt;
};

static volatile struct ctx_t ctx;

static inline void update_pwm(void)
{
	if (ctx.mode != MODE_PWM)
		return;

	if (++ctx.pwm_chg_cnt != PWM_CHG_SPEED)
		return;

	ctx.pwm_chg_cnt = 0;
	if (ctx.pwm_chg_dir == 1)
	{
		if (ctx.pwm_val == 0)
			ctx.pwm_chg_dir = 0;
		else
			--ctx.pwm_val;
	}
	else
	{
		if (ctx.pwm_val == PWM_NUM_LEVELS)
			ctx.pwm_chg_dir = 1;
		else
			++ctx.pwm_val;
	}
}

static inline void handle_pwm(void)
{
	if (ctx.mode == MODE_OFF)
		return;

	if (ctx.pwm_cnt >= ctx.pwm_val)
		io_clr_led();
	else
		io_set_led();

	if (++ctx.pwm_cnt < PWM_NUM_LEVELS)
		return;

	ctx.pwm_cnt = 0;
	update_pwm();
}

static inline void handle_btn(void)
{
	uint16_t cnt;

	if (io_get_btn())
	{
		if (ctx.btn_cnt < (BTN_LONG + 1))
			++ctx.btn_cnt;

		return;
	}

	cnt = ctx.btn_cnt;
	ctx.btn_cnt = 0;

	if (cnt >= BTN_SHORT && cnt < BTN_LONG)
	{
		if (ctx.mode == MODE_OFF || ctx.mode == MODE_HOLD)
			ctx.mode = MODE_PWM;
		else
			ctx.mode = MODE_HOLD;
	}
	else if (cnt >= BTN_LONG)
	{
		if (ctx.mode == MODE_OFF)
			ctx.mode = MODE_PWM;
		else
		{
			ctx.mode = MODE_OFF;
			io_clr_led();
			ctx.pwm_cnt = 0;
			ctx.pwm_val = 0;
			ctx.pwm_chg_cnt = 0;
			ctx.pwm_chg_dir = 0;
		}
	}
}

// freq: 9600000 / 8 / 256 = 4687.5 Hz

ISR(TIM0_OVF_vect)
{
	handle_pwm();
	handle_btn();
}

void tmr_init(void)
{
	TCCR0A	= 0;
	TCCR0B	= _BV(CS01);	// div by 8
	TIMSK0	= _BV(TOIE0);
}
