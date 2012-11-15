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
#This awk script is designed to parse the output of QtTestLib
#test runs. It will condense the output and show only the important
#lines. It will color-ize failed, and skipped tests as well as any
#misc output.
{
OFMT="%6s";
    if ($1 == "FAIL!") { 
        printf "\033[1;31mFAIL\033[0m", $1;
        tail=$0; for (i=1;i<2;i++) sub($i,"",tail); sub(/ */,"",tail)
        print tail;
    } else if ($1 == "SKIP") {
        printf "\033[1;33m%s\033[0m", $1;
        tail=$0; for (i=1;i<2;i++) sub($i,"",tail); sub(/ */,"",tail)
        print tail;
    } else if ($1 != "PASS" && $1 != "*********" && $1 != "Config:" && $1 != "Totals:") {
        printf "     \033[1;29m%s\033[0m", $1;
        tail=$0; for (i=1;i<2;i++) sub($i,"",tail); sub(/ */,"",tail)
        print tail;
    }
}
