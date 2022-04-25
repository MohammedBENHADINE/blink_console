#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_console.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"

extern int calibration_factor;
extern int threshold;


static void register_show(void);
static int show(int argc, char **argv);
static void register_set_calib(void);
static int set_calib(int argc, char **argv);


void startConsoleCLI(void)
{
	esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    
    repl_config.prompt = "ESPconsole";
    repl_config.max_cmdline_length = 1024;


    register_show();
    register_set_calib();

    //start console task
   ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl)); // create the task console_repl
   ESP_ERROR_CHECK(esp_console_start_repl(repl)); // start the REPL (Read-Evaluate-Print-Loop)

}

//set_calib command
static struct {
	struct arg_int *calib;
	struct arg_end *end;
}set_calib_arg;

static void register_set_calib(void){

	set_calib_arg.calib = arg_int0("c", "factor" , "<int>", "CALIB");
	set_calib_arg.end = arg_end(1);

	const esp_console_cmd_t cmd = {
		.command="set_calib",
		.help="Change calibration factor",
		.hint=NULL,
		.func=&set_calib,
		.argtable = &set_calib_arg
	};

 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));



}

static int set_calib(int argc, char **argv)
{

  int nerrors = arg_parse(argc, argv, (void **) &set_calib_arg);
  if (nerrors != 0) {
       arg_print_errors(stderr, set_calib_arg.end, argv[0]);
       return 1;
    }

    calibration_factor = set_calib_arg.calib->ival[0];

  return 0;
}









// show command
static void register_show(void)
{
	const esp_console_cmd_t cmd ={
		.command = "show",
		.help = "Time to show settings",
		.hint = NULL,
		.func = &show
	};
	ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

// console task will give 2 args to the handler
static int show(int argc, char **argv)
{
	printf(" ##### Running configuration ##### \n");
    printf("calibration_factor : %d \n", calibration_factor);
    printf("threshold is %d \n", threshold);

   return 0;
}



