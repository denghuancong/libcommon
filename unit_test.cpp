/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/05/2013 04:55:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "command_line.h"

int main(int argv, char** argc){
    
    dhc_common::CommandLine cmd(argv,argc);
    if(cmd.hasOption("dump")){
        cmd.dump(STDOUT_FILENO);
    }
    return 0;
}
