#pragma once

/************************************************************************

参考：https://blog.csdn.net/u012319493/article/details/102931968

struct argp

	const struct argp_option *options 指定要解析的选项，为 0 时没有选项
	argp_parser_t parser 解析选项的函数，为 0 时不解析选项
	const char *args_doc 参数说明，为 0 时没有参数说明
	const char *doc 程序说明，为 0 时没有程序说明
	const struct argp_child *children
	char *(*help_filter)(int key, const char *text, void *input)
	const char *argp_domain
	以上，前四个字段最常用

struct argp_option

	const char *name 选项的长名称，“–name”形式
	int key 选项的短名称的 ASCII 码
	const char *arg 如果非 0，为选项的参数名
	int flags 如果非 0，为选项的标志。OPTION_ALIAS 表示当前选项是前一个选项的别名
	const char *doc 选项说明
	int group

error_t parser (int key, char *arg, struct argp_state *state)

	key 选项的短名称
	arg 选项的参数
	state 包含要解析的信息

struct argp_state

	const struct argp *const root_argp
	int argc
	char **argv
	int next
	unsigned flags
	unsigned arg_num
	当解析函数的参数 key 为 ARGP_KEY_ARG 时，表示当前参数的个数，从 0 开始。每当 ARGP_KEY_ARG 调用返回时，会递增；
	其它情况下，为 ARGP_KEY_ARG 已经处理的参数个数。
	int quoted
	void *input
	void **child_inputs
	void *hook
	char *name
	FILE *err_stream
	FILE *out_stream
	void *pstate

*********************************************************************************/

#ifdef __linux__

#include <stdlib.h>
#include <argp.h>

const char *argp_program_version =
"argp-ex3 1.0";
const char *argp_program_bug_address =
"<bug-gnu-utils@gnu.org>";

/* 程序说明 */
static char doc[] =
"Argp example #3 -- a program with options and arguments using argp";

/* 参数说明 */
static char args_doc[] = "ARG1 ARG2";

/* 选项说明 */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  {"quiet",    'q', 0,      0,  "Don't produce any output" },
  {"silent",   's', 0,      OPTION_ALIAS },
  {"output",   'o', "FILE", 0, "Output to FILE instead of standard output" },
  { 0 }
};

/* parse_opt 的参数 */
struct arguments
{
	char *args[2];                /* arg1 & arg2 */
	int silent, verbose;
	char *output_file;
};

/* 选项解释 */
static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
	/* 从 argp_parse 获取参数，该参数为 arguments 结构体 */
	struct arguments *arguments = (struct arguments*)(state->input);

	switch (key)
	{
	case 'q': case 's':
		arguments->silent = 1;
		break;
	case 'v':
		arguments->verbose = 1;
		break;
	case 'o':
		arguments->output_file = arg;
		break;

	case ARGP_KEY_ARG:
		if (state->arg_num >= 2)
			/* Too many arguments. */
			argp_usage(state);

		arguments->args[state->arg_num] = arg;

		break;

	case ARGP_KEY_END:
		if (state->arg_num < 2)
			/* Not enough arguments. */
			argp_usage(state);
		break;

	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/* argp 解析器。argp_parse 的参数 */
static struct argp argp = { options, parse_opt, args_doc, doc };

int test_argp_parse(int argc, char **argv)
{
	char default_file[] = "-";

	struct arguments arguments;

	/* 默认值 */
	arguments.silent = 0;
	arguments.verbose = 0;
	arguments.output_file = default_file;

	/* 解析 arguments；parse_opt 解析后的每个选项会反映在 arguments 中 */
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	printf("ARG1 = %s\nARG2 = %s\nOUTPUT_FILE = %s\n"
		"VERBOSE = %s\nSILENT = %s\n",
		arguments.args[0], arguments.args[1],
		arguments.output_file,
		arguments.verbose ? "yes" : "no",
		arguments.silent ? "yes" : "no");

	return 0;
}

/******************************************************
测试方式

事例一：
./test aa bb -o 1.txt
	ARG1 = aa
	ARG2 = bb
	OUTPUT_FILE = 1.txt
	VERBOSE = no
	SILENT = no

事例二：
./test --help
	Usage: args [OPTION...] ARG1 ARG2
	Argp example #3 -- a program with options and arguments using argp

	  -o, --output=FILE          Output to FILE instead of standard output
	  -q, -s, --quiet, --silent  Don't produce any output
	  -v, --verbose              Produce verbose output
	  -?, --help                 Give this help list
		  --usage                Give a short usage message
	  -V, --version              Print program version

	Mandatory or optional arguments to long options are also mandatory or optional
	for any corresponding short options.

	Report bugs to <bug-gnu-utils@gnu.org>.

事例三：
./args3 aa bb -v
	ARG1 = aa
	ARG2 = bb
	OUTPUT_FILE = -
	VERBOSE = yes
	SILENT = no

*********************************************************/

#endif