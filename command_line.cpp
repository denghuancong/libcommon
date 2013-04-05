/*
 * =====================================================================================
 *
 *       Filename:  command_line.cpp
 *
 *    Description:  command line parse for linux
 *
 *        Version:  1.0
 *        Created:  04/05/2013 03:59:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dhc3@hotmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#include "command_line.h"
#ifdef __DEBUG__
#include <iostream>
#endif
namespace dhc_common {
   
    const CMD_OPT_CONFIG CommandLine::c_cmd_opt_config[] = {
        {'h',"help",OPTION_ARGUMENT,false,true,"get info about the command"}, 
        {'d',"dump",NO_ARGUMENT,true,true,"dump the argument"},
        {'c',"config",REQUIRED_ARGUMENT,true,true,"get the config file"}
    };

    /*
     *  [public] 
     *  constructor
     */
    CommandLine::CommandLine(int argv, char** argc) {
        m_mapCmd.clear();
        int iRet = parse(argv,argc);
        if(iRet) {
            exit(iRet);
        }
    }


    /* 
     * [public]
     * dump out the option from file descitpor
     */
    void CommandLine::dump(int iFD) {
        if( m_mapCmd.size() == 0 ) {
            dprintf(iFD,"%s\n","no command option.");
        } else {
            for( std::map<std::string, std::string>::const_iterator cit = m_mapCmd.begin(); cit != m_mapCmd.end(); cit++) {
                dprintf(iFD,"%s:%s\n",cit->first.c_str(), cit->second.c_str());
            }
        }
    }

    /*
     * [public]
     * print the help info
     */
    void CommandLine::usage(int iFD) {  
       int iCount = sizeof(CommandLine::c_cmd_opt_config)/sizeof(CMD_OPT_CONFIG);
       dprintf(iFD,"%s","Usage:\n");
       for( int i=0; i<iCount; i++) {
            dprintf(
                iFD,"-%c, --%s, %s",
                CommandLine::c_cmd_opt_config[i].short_opt,
                CommandLine::c_cmd_opt_config[i].long_opt,
                CommandLine::c_cmd_opt_config[i].description
                ); 
       }
    }

    /*
     * [public]
     * parse the command line 
     */
    int CommandLine::parse(int argc, char** argv) {   
        
        if( argc == 1 ){
            return -1;
        } else {
            struct option* ptrOptConfig =  new struct option[sizeof(CommandLine::c_cmd_opt_config)/sizeof(CMD_OPT_CONFIG)+1];
            char pCharOpt[64] = {0};
            int longopt_index = 0;
            for(int i = 0,j = 0; i<sizeof(CommandLine::c_cmd_opt_config)/sizeof(CMD_OPT_CONFIG); i++ ) {
                if(CommandLine::c_cmd_opt_config[i].has_short_opt){
                    switch(CommandLine::c_cmd_opt_config[i].type){
                        case NO_ARGUMENT:pCharOpt[j++]=CommandLine::c_cmd_opt_config[i].short_opt;break;
                        case REQUIRED_ARGUMENT:{
                                    pCharOpt[j++]=CommandLine::c_cmd_opt_config[i].short_opt; 
                                    pCharOpt[j++]=':';
                                    break;
                        } 
                        default:break;
                    }
                }
                if(CommandLine::c_cmd_opt_config[i].has_long_opt){
                    ptrOptConfig[longopt_index++] = {
                        CommandLine::c_cmd_opt_config[i].long_opt,
                        CommandLine::c_cmd_opt_config[i].type,
                        0,
                        CommandLine::c_cmd_opt_config[i].has_short_opt?CommandLine::c_cmd_opt_config[i].short_opt:0
                    };
                }
            }
            ptrOptConfig[longopt_index] = {0,0,0,0};//end
            int iRet = _parse(argc, argv, ptrOptConfig, pCharOpt);
            delete[] ptrOptConfig;
            return iRet;
        }
    }

    /*
     * [protected]
     *  get the cmd option into map 
     */
    int CommandLine::_parse(int argc, char** argv, struct option* pOpt, char* pCharOpt) {
        int iRet, index;
        iRet = getopt_long(argc, argv, pCharOpt, pOpt, &index);
        bool bfind = false;
        switch(iRet) {
            case -1:break;
            case 0:m_mapCmd.insert(std::make_pair(pOpt[index].name, optarg?optarg:""));break;
            case '?':exit(-1);
            default:{
                        for( int i=0;i<sizeof(CommandLine::c_cmd_opt_config)/sizeof(CMD_OPT_CONFIG);i++) {
                             if(CommandLine::c_cmd_opt_config[i].has_short_opt && CommandLine::c_cmd_opt_config[i].short_opt==iRet){
                                 m_mapCmd.insert(std::make_pair(CommandLine::c_cmd_opt_config[i].long_opt, optarg?optarg:""));
                                 bfind = true;
                                 break;
                             }
                        }
                        if(!bfind){
                            dprintf(STDERR_FILENO,"unknown param :\"%c\"\n",iRet);
                            exit(1);
                        }
                    }
        }
        if(iRet==-1){
            return 0;
        } else {
            return _parse(argc,argv,pOpt,pCharOpt);
        }
    }

    /*
     * [public]
     * whether has set the target option
     */
    bool CommandLine::hasOption(std::string name) {
        std::map<std::string, std::string>::const_iterator i = m_mapCmd.find(name);
        if( i == m_mapCmd.end() ) {
            return false;
        } else {
            return true;
        }
    }

    /*
     * [public]
     * get the int value of the taget option
     */
    int CommandLine::IntOption(std::string name) {
        std::string strValue = StringOption(name); 
        if(strValue.size() == 0 ) {
            return -1;//ERROR
        } else {
            return strtol(strValue.c_str(),NULL,10); 
        }
    }

    /*
     * [public]
     * get the string value of the target option
     */
    std::string CommandLine::StringOption(std::string name) {
        std::map<std::string, std::string>::const_iterator i = m_mapCmd.find(name);
        if( i == m_mapCmd.end() ) {
            return "";
        } else {
            return i->second;
        }
    }

    /*
     * [public]
     * get all the param's number
     */
    int CommandLine::Size() {
        return m_mapCmd.size();
    }
}
