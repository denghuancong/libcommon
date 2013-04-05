/*
 * =====================================================================================
 *
 *       Filename:  command_line.h
 *
 *    Description:  command line parse for linux under c++
 *
 *        Version:  1.0
 *        Created:  04/05/2013 04:05:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dhc3@hotmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#ifndef __DHC__COMMAND__LINE__H__
#define __DHC__COMMAND__LINE__H__

#include <map>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

namespace dhc_common {

#define NO_ARGUMENT 0
#define REQUIRED_ARGUMENT 1
#define OPTION_ARGUMENT 2

    typedef struct {
        unsigned short short_opt;
        const char* long_opt;
        int type;
        bool has_short_opt;
        bool has_long_opt;
        const char* description;
    }CMD_OPT_CONFIG, * PCMD_OPT_CONFIG;

    
    class CommandLine {
        public:
            CommandLine(int argv, char** argc);
        public:
            int IntOption(std::string name);
            std::string StringOption(std::string name);
            bool hasOption(std::string name);
            int Size();
        public:
            void dump(int iFD = STDOUT_FILENO);
            void usage(int iFD = STDOUT_FILENO);
            int parse(int argv, char** argc);
        protected:
            int _parse(int argv,char** argc, struct option* pOpt, char* pCharOpt );
        private:
            std::map<std::string, std::string> m_mapCmd;
            static const CMD_OPT_CONFIG c_cmd_opt_config[];
    };
}
#endif
