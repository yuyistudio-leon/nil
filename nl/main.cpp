/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/



/**
 * @file main.cpp
 * @author liyong11(com@baidu.com)
 * @date 2015/08/28 16:08:05
 * @brief
 *
 **/
#include "vm.h"
#include "parser.h"

char* g_buf;
GlobalConfig g_conf;

void print_help(){
    printf("\n\
nl - A New Language!\n\
    Version %s\n\
\n\
    Usage: nl [-acgprs] [filepath-list]\n\
\n\
    -a show arranged base ip\n\
    -c print call stacks\n\
    -g print generated codes\n\
    -p print parse progress\n\
    -r print running codes\n\
    -s pause after each code\n\
    -h show this help message\n\
\n\
    e.g. \n\
    nl -g main.nl                        // show the generated code of main.nl\n\
    nl main.nl                           // run main.nl\n\
    nl file1.nl file2.nl file3.nl ...    // run multiple files\n\
\n\
", VERSION_STR);
}
int main(int argc, char* argv[]){
	vector<char*> files;
	for (int i = 1; i < argc; ++i){
		if (argv[i][0] == '-'){
			bool switches[256];
			memset(switches, 0, sizeof(switches));
			for (char* p = argv[i]; *p; ++p){
				switches[(int)*p] = true;
			}
			if (switches['h']){
				print_help();
				exit(0);
			}
			g_conf.print_arranged_base_ip = switches['a'];
			g_conf.print_call_stack = switches['c'];
			g_conf.print_generated_code = switches['g'];
			g_conf.print_parse_op = switches['p'];
			g_conf.print_running_code = switches['r'];
			g_conf.pause_after_instruction = switches['s'];
		}else{
			files.push_back(argv[i]);
		}
	}
	if (files.size() == 0){
		print_help();
		exit(0);
	}
	g_buf = new char[GBUF_SIZE];
	for (int i = 0; i < (int)files.size(); ++i){
		Parser parser;
		if (parser.Parse(files[i])){
			VM* pvm = VM::GetVM();
			pvm->RunCode(parser.GetAllCodes());
			VM::ReleaseVM();
		}
	}
    return 0;
}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
