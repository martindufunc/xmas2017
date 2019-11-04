/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>


////////////////////////////////////////////////////////////////////////////////
// Pin routing
//       0
//      1 2
//       3
//    4     5
//       6
//  7         8
// ----- 9 -----
//       I

#define TREE_LED0   PIN_PA31
#define TREE_LED1   PIN_PA16
#define TREE_LED2   PIN_PA23
#define TREE_LED3   PIN_PA30
#define TREE_LED4   PIN_PA10
#define TREE_LED5   PIN_PA22
#define TREE_LED6   PIN_PA27
#define TREE_LED7   PIN_PA11
#define TREE_LED8   PIN_PA24
#define TREE_LED9   PIN_PA17

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
void clear_tree(void);
void set_tree(void);
void tree_pattern_01(int);
void tree_pattern_02(int);
void tree_pattern_03(int);
void tree_pattern_04(int);
void tree_pattern_05(int);
void tree_pattern_06(int);
void tree_pattern_07(int);
static void config_tree(void);

// Function pointer has to match with pattern code enum order
void (* pattern[])(int) = {
	tree_pattern_01,
	tree_pattern_02,
	tree_pattern_03,
	tree_pattern_04,
	tree_pattern_05,
	tree_pattern_06,
	tree_pattern_07
};

// Pattern code
enum pattern_code {
	PAT_ALTRIBBON,
	PAT_TWINKLESTAR,
	PAT_VALTERNATE,
	PAT_RUNOFF,
	PAT_VROUND,
	PAT_RAND1,
	PAT_RAND2
};

// Define transition table format
struct transition {
	enum pattern_code curr_pat;
	int pat_duration;
	enum pattern_code next_pat;
};

// Transition table lookup: current pattern, duration, next pattern
struct transition transition_table[] = {
	{PAT_ALTRIBBON,     20,     PAT_TWINKLESTAR},
	{PAT_TWINKLESTAR,   10,     PAT_VALTERNATE},
	{PAT_VALTERNATE,    20,     PAT_RUNOFF},
	{PAT_RUNOFF,        50,     PAT_VROUND},
	{PAT_VROUND,        45,     PAT_RAND1},
	{PAT_RAND1,         80,     PAT_RAND2},
	{PAT_RAND2,         50,     PAT_ALTRIBBON}
};

int patcount = sizeof(transition_table)/sizeof(transition_table[0]);            // transition table entries count
enum pattern_code run_pat = PAT_TWINKLESTAR;                                    // Initial pattern
void (* run_pattern)(int);                                                      // Function pointer prototype




////////////////////////////////////////////////////////////////////////////////
// Main and ISR

int main(void)
{
	system_init();
	// Configure system tick for periodic interrupts
	SysTick_Config((system_gclk_gen_get_hz(GCLK_GENERATOR_0) / 4));
	config_tree();
	while (1);
}

void SysTick_Handler(void)
{
	static int run_count = 0;
	run_pattern = pattern[run_pat];
	run_pattern(run_count++);

	// Checks for transition to next state
	for (int i = 0; i < patcount; i++)
	{
		if ((transition_table[i].curr_pat == run_pat) && (transition_table[i].pat_duration <= run_count))
		{
			run_pat = transition_table[i].next_pat;
			run_count = 0;
			break;
		}
	}
}




////////////////////////////////////////////////////////////////////////////////
// All LED patterns

static void config_tree(void)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	pin_conf.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(TREE_LED0, &pin_conf);
	port_pin_set_config(TREE_LED1, &pin_conf);
	port_pin_set_config(TREE_LED2, &pin_conf);
	port_pin_set_config(TREE_LED3, &pin_conf);
	port_pin_set_config(TREE_LED4, &pin_conf);
	port_pin_set_config(TREE_LED5, &pin_conf);
	port_pin_set_config(TREE_LED6, &pin_conf);
	port_pin_set_config(TREE_LED7, &pin_conf);
	port_pin_set_config(TREE_LED8, &pin_conf);
	port_pin_set_config(TREE_LED9, &pin_conf);
	clear_tree();
}

void clear_tree(void)
{
	port_pin_set_output_level(TREE_LED0, false);
	port_pin_set_output_level(TREE_LED1, false);
	port_pin_set_output_level(TREE_LED2, false);
	port_pin_set_output_level(TREE_LED3, false);
	port_pin_set_output_level(TREE_LED4, false);
	port_pin_set_output_level(TREE_LED5, false);
	port_pin_set_output_level(TREE_LED6, false);
	port_pin_set_output_level(TREE_LED7, false);
	port_pin_set_output_level(TREE_LED8, false);
	port_pin_set_output_level(TREE_LED9, false);
}

void set_tree(void)
{
	port_pin_set_output_level(TREE_LED0, true);
	port_pin_set_output_level(TREE_LED1, true);
	port_pin_set_output_level(TREE_LED2, true);
	port_pin_set_output_level(TREE_LED3, true);
	port_pin_set_output_level(TREE_LED4, true);
	port_pin_set_output_level(TREE_LED5, true);
	port_pin_set_output_level(TREE_LED6, true);
	port_pin_set_output_level(TREE_LED7, true);
	port_pin_set_output_level(TREE_LED8, true);
	port_pin_set_output_level(TREE_LED9, true);
}

void tree_pattern_01(int tick_count) // Round ribbon alternate...
{
	if (tick_count == 0) // Initialize
	{
		clear_tree();
		port_pin_set_output_level(TREE_LED0, true);
		port_pin_set_output_level(TREE_LED1, true);
		port_pin_set_output_level(TREE_LED2, false);
		port_pin_set_output_level(TREE_LED3, false);
		port_pin_set_output_level(TREE_LED4, false);
		port_pin_set_output_level(TREE_LED5, true);
		port_pin_set_output_level(TREE_LED6, true);
		port_pin_set_output_level(TREE_LED7, true);
		port_pin_set_output_level(TREE_LED8, false);
		port_pin_set_output_level(TREE_LED9, false);
	}
	port_pin_toggle_output_level(TREE_LED0);
	port_pin_toggle_output_level(TREE_LED1);
	port_pin_toggle_output_level(TREE_LED2);
	port_pin_toggle_output_level(TREE_LED3);
	port_pin_toggle_output_level(TREE_LED4);
	port_pin_toggle_output_level(TREE_LED5);
	port_pin_toggle_output_level(TREE_LED6);
	port_pin_toggle_output_level(TREE_LED7);
	port_pin_toggle_output_level(TREE_LED8);
	port_pin_toggle_output_level(TREE_LED9);
}

void tree_pattern_02(int tick_count) // Star twinkle...
{
	if (tick_count == 0) // Initialize
	{
		clear_tree();
	}
	port_pin_toggle_output_level(TREE_LED0);
}

void tree_pattern_03(int tick_count) // 1 - 2 Vertical alternate
{
	if (tick_count == 0) // Initialize
	{
		clear_tree();
		port_pin_set_output_level(TREE_LED0, true);
		port_pin_set_output_level(TREE_LED1, false);
		port_pin_set_output_level(TREE_LED2, false);
		port_pin_set_output_level(TREE_LED3, true);
		port_pin_set_output_level(TREE_LED4, false);
		port_pin_set_output_level(TREE_LED5, false);
		port_pin_set_output_level(TREE_LED6, true);
		port_pin_set_output_level(TREE_LED7, false);
		port_pin_set_output_level(TREE_LED8, false);
		port_pin_set_output_level(TREE_LED9, true);
	}
	port_pin_toggle_output_level(TREE_LED0);
	port_pin_toggle_output_level(TREE_LED1);
	port_pin_toggle_output_level(TREE_LED2);
	port_pin_toggle_output_level(TREE_LED3);
	port_pin_toggle_output_level(TREE_LED4);
	port_pin_toggle_output_level(TREE_LED5);
	port_pin_toggle_output_level(TREE_LED6);
	port_pin_toggle_output_level(TREE_LED7);
	port_pin_toggle_output_level(TREE_LED8);
	port_pin_toggle_output_level(TREE_LED9);
}

void tree_pattern_04(int tick_count) // Running off...
{
	set_tree();
	if (tick_count % 10 == 0) port_pin_set_output_level(TREE_LED0, false);
	if (tick_count % 10 == 1) port_pin_set_output_level(TREE_LED1, false);
	if (tick_count % 10 == 2) port_pin_set_output_level(TREE_LED2, false);
	if (tick_count % 10 == 3) port_pin_set_output_level(TREE_LED3, false);
	if (tick_count % 10 == 4) port_pin_set_output_level(TREE_LED4, false);
	if (tick_count % 10 == 5) port_pin_set_output_level(TREE_LED5, false);
	if (tick_count % 10 == 6) port_pin_set_output_level(TREE_LED6, false);
	if (tick_count % 10 == 7) port_pin_set_output_level(TREE_LED7, false);
	if (tick_count % 10 == 8) port_pin_set_output_level(TREE_LED8, false);
	if (tick_count % 10 == 9) port_pin_set_output_level(TREE_LED9, false);
}

void tree_pattern_05(int tick_count) // Running round vertical...
{
	clear_tree();
	port_pin_set_output_level(TREE_LED0, true);
	if (tick_count % 3 == 0)
	{
		port_pin_set_output_level(TREE_LED2, true);
		port_pin_set_output_level(TREE_LED5, true);
		port_pin_set_output_level(TREE_LED8, true);
	}
	if (tick_count % 3 == 1)
	{
		port_pin_set_output_level(TREE_LED3, true);
		port_pin_set_output_level(TREE_LED6, true);
		port_pin_set_output_level(TREE_LED9, true);
	}
	if (tick_count % 3 == 2)
	{
		port_pin_set_output_level(TREE_LED1, true);
		port_pin_set_output_level(TREE_LED4, true);
		port_pin_set_output_level(TREE_LED7, true);
	}
}

void tree_pattern_06(int tick_count) // Random1...
{
	clear_tree();
	port_pin_set_output_level(TREE_LED0, rand() % 2);
	port_pin_set_output_level(TREE_LED1, rand() % 2);
	port_pin_set_output_level(TREE_LED2, rand() % 2);
	port_pin_set_output_level(TREE_LED3, rand() % 2);
	port_pin_set_output_level(TREE_LED4, rand() % 2);
	port_pin_set_output_level(TREE_LED5, rand() % 2);
	port_pin_set_output_level(TREE_LED6, rand() % 2);
	port_pin_set_output_level(TREE_LED7, rand() % 2);
	port_pin_set_output_level(TREE_LED8, rand() % 2);
	port_pin_set_output_level(TREE_LED9, rand() % 2);
}

void tree_pattern_07(int tick_count) // Random 2...
{
	clear_tree();
	switch (rand() % 10)
	{
		case 0: port_pin_set_output_level(TREE_LED0, true);
		break;
		case 1: port_pin_set_output_level(TREE_LED1, true);
		break;
		case 2: port_pin_set_output_level(TREE_LED2, true);
		break;
		case 3: port_pin_set_output_level(TREE_LED3, true);
		break;
		case 4: port_pin_set_output_level(TREE_LED4, true);
		break;
		case 5: port_pin_set_output_level(TREE_LED5, true);
		break;
		case 6: port_pin_set_output_level(TREE_LED6, true);
		break;
		case 7: port_pin_set_output_level(TREE_LED7, true);
		break;
		case 8: port_pin_set_output_level(TREE_LED8, true);
		break;
		case 9: port_pin_set_output_level(TREE_LED9, true);
		break;
		default: clear_tree();
	}
}

