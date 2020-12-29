#pragma once

/************************************************************************

�ο���https://blog.csdn.net/u012319493/article/details/102931968

struct argp

	const struct argp_option *options ָ��Ҫ������ѡ�Ϊ 0 ʱû��ѡ��
	argp_parser_t parser ����ѡ��ĺ�����Ϊ 0 ʱ������ѡ��
	const char *args_doc ����˵����Ϊ 0 ʱû�в���˵��
	const char *doc ����˵����Ϊ 0 ʱû�г���˵��
	const struct argp_child *children
	char *(*help_filter)(int key, const char *text, void *input)
	const char *argp_domain
	���ϣ�ǰ�ĸ��ֶ����

struct argp_option

	const char *name ѡ��ĳ����ƣ����Cname����ʽ
	int key ѡ��Ķ����Ƶ� ASCII ��
	const char *arg ����� 0��Ϊѡ��Ĳ�����
	int flags ����� 0��Ϊѡ��ı�־��OPTION_ALIAS ��ʾ��ǰѡ����ǰһ��ѡ��ı���
	const char *doc ѡ��˵��
	int group

error_t parser (int key, char *arg, struct argp_state *state)

	key ѡ��Ķ�����
	arg ѡ��Ĳ���
	state ����Ҫ��������Ϣ

struct argp_state

	const struct argp *const root_argp
	int argc
	char **argv
	int next
	unsigned flags
	unsigned arg_num
	�����������Ĳ��� key Ϊ ARGP_KEY_ARG ʱ����ʾ��ǰ�����ĸ������� 0 ��ʼ��ÿ�� ARGP_KEY_ARG ���÷���ʱ���������
	��������£�Ϊ ARGP_KEY_ARG �Ѿ�����Ĳ���������
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

/* ����˵�� */
static char doc[] =
"Argp example #3 -- a program with options and arguments using argp";

/* ����˵�� */
static char args_doc[] = "ARG1 ARG2";

/* ѡ��˵�� */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  {"quiet",    'q', 0,      0,  "Don't produce any output" },
  {"silent",   's', 0,      OPTION_ALIAS },
  {"output",   'o', "FILE", 0, "Output to FILE instead of standard output" },
  { 0 }
};

/* parse_opt �Ĳ��� */
struct arguments
{
	char *args[2];                /* arg1 & arg2 */
	int silent, verbose;
	char *output_file;
};

/* ѡ����� */
static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
	/* �� argp_parse ��ȡ�������ò���Ϊ arguments �ṹ�� */
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

/* argp ��������argp_parse �Ĳ��� */
static struct argp argp = { options, parse_opt, args_doc, doc };

int test_argp_parse(int argc, char **argv)
{
	char default_file[] = "-";

	struct arguments arguments;

	/* Ĭ��ֵ */
	arguments.silent = 0;
	arguments.verbose = 0;
	arguments.output_file = default_file;

	/* ���� arguments��parse_opt �������ÿ��ѡ��ᷴӳ�� arguments �� */
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
���Է�ʽ

����һ��
./test aa bb -o 1.txt
	ARG1 = aa
	ARG2 = bb
	OUTPUT_FILE = 1.txt
	VERBOSE = no
	SILENT = no

��������
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

��������
./args3 aa bb -v
	ARG1 = aa
	ARG2 = bb
	OUTPUT_FILE = -
	VERBOSE = yes
	SILENT = no

*********************************************************/

#endif