#!/bin/awk
#
# Copyright (c) 2012 Brian C. Milco <bcmilco AT gmail DOT com>
#
# This script is a part of the Note Pages project
#
#    This program is free software; you can redistribute it and/or
#    modify it under the terms of the GNU General Public License
#    as published by the Free Software Foundation; version 2
#    of the License.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
#
#This is an awk script designed to parse the output of QtTestLib
#test runs. It will condense the output to the following format:
#TestName: XX passed, XX failed, XX skipped
#The output will be displayed with color for easy reading.
BEGIN {
    padTest = 50;
    print "+-------------------------------+--------------------------+";
    print "|                               | \033[1;32mPassed\033[0m  \033[1;31mFailed\033[0m  \033[1;33mSkipped\033[0m  |";
    print "+-------------------------------+--------------------------+";
}
{
    passFill = "%7s";
    failFill = "%8s";
    skipFill = "%9s";

    if ($1 == "*********") { 
        printf "|\033[1;29m%30s\033[0m |", $5;
    } else {
        if($2 > 0)
            pass = "\033[1;32m" passFill "\033[0m";
        else
            pass = passFill;

        if($4 > 0)
            fail = "\033[1;31m" failFill "\033[0m";
        else
            fail = failFill;

        if($6 > 0) 
            skip = "\033[1;33m" skipFill "\033[0m  |\n";
        else
            skip= skipFill "  |\n";

        printf pass, $2; 
        printf fail, $4;
        printf skip, $6;
        passCnt += $2;
        failCnt += $4;
        skipCnt += $6;
    }    
}
END {
    if(passCnt > 0)
        pass = "\033[1;32m" passFill "\033[0m";
    else
        pass = passFill;

    if(failCnt > 0)
        fail = "\033[1;31m" failFill "\033[0m";
    else
        fail = failFill;

    if(skipCnt > 0)
        skip = "\033[1;33m" skipFill "\033[0m  |\n";
    else
        skip = skipFill "  |\n";
    totalsStr = "TOTALS";

    print "+-------------------------------+--------------------------+";
    printf "|\033[1;29m%30s\033[0m |", totalsStr;
    printf pass, passCnt;
    printf fail, failCnt;
    printf skip, skipCnt;
    print "+-------------------------------+--------------------------+";
}
