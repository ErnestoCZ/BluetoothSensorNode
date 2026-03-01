#ifndef SHELL_H
#define SHELL_H

#include <zephyr/shell/shell.h>

static void test_shell(const struct shell *sh, size_t argc, char **argv){
	shell_print(sh,"worked!");
};

void initShell(void){}

SHELL_CMD_REGISTER(test_shell, NULL, "Testing the shell command!",test_shell);


#endif