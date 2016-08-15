#!/bin/bash
#===============================================================================
#
#          FILE:  test1.sh
# 
#         USAGE:  ./test1.sh 
# 
#   DESCRIPTION:  
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR:   (), 
#       COMPANY:  
#       VERSION:  1.0
#       CREATED:  03/28/07 19:30:40 TST
#      REVISION:  ---
#===============================================================================

usage()
{
    echo "================= Usage ================="
    echo "make 16   : 16M F/W + 64M DRAM"
    echo "make 16r  : 16M F/W + 16M DRAM"
    echo "make 16d  : 16M Debug Mode F/W + 64M DRAM"
    echo "make 32   : 32M Debug Mode F/W + 64M DRAM"
    echo "make 32   : 64M Debug Mode F/W + 64M DRAM"
    echo 
    echo "make help : This help screen"
    echo "========================================="
}

## show initial settings
## must be confirmed by user
get_initial_settings()
{
    echo ""
    echo "============ CTK Platform Setting ============="
    echo "Platform          : $platform"
    echo "IC System         : $ic_system"
    echo "OSD Language      : $osd_lan"
    echo "Support DMP       : $sup_dmp"
    echo "Support STB       : $sup_stb"
    echo "Dram Solution     : $dram_size"  MB
    echo "Flash Solution    : $flash_size" MB
    echo "==============================================="
    echo ""
}

CT909R()
{
    ## [1] copy 909S folder
    echo "Copying 909S folder"
    cp -f ./909S/* .

    ## [2] copy 950_Files folder
    if [ "$sup_dmp" = "1" ]; then
    echo "Copying 950_Files folder"
        cp -f ./950_Files/* .
    fi

    ## [3] modify internal.h
    if [ "$dram_size" = "16" ]; then
        cat internal.h | sed                                                                \
            -e "s/^\(#define *DRAM_CONFIGURATION_TYPE  *\).*/\1DRAM_SIZE_${dram_size}/"     \
            -e 's/^\(#define *CPU_SPEED *\).*/\1CPU_146M/' > internal.h.tmp
    else
        cat internal.h | sed                                                                \
            -e "s/^\(#define *DRAM_CONFIGURATION_TYPE  *\).*/\1DRAM_SIZE_${dram_size}/"     \
            -e 's/^\(#define *CPU_SPEED *\).*/\1CPU_133M/' > internal.h.tmp
    fi
    mv -vf internal.h.tmp internal.h

    ## [4] copy DVD909.ld
    if [ "$dram_size" = "16" ]; then
        if [ "$debug_en" = "_DBG" ]; then
            cp -vf DVD909_16MDBG.ld DVD909.ld
        else
            cp -vf DVD909_16M.ld DVD909.ld
        fi
    else
        cp -vf DVD909_${dram_size}M.ld DVD909.ld
    fi
    
    ## [4.1] modify DVD909.ld
    if [ "$sup_dmp" = "1" ]
    then
        cat DVD909.ld | sed                                 \
            -e '/ *srv_dram.a.*/d'                          \
            > DVD909.ld.tmp
        mv -vf DVD909.ld.tmp DVD909.ld
    fi
    
    ## [5] copy romcfg.txt
    cp -vf romcfg${dmpcfg}${stbcfg}_${flash_size}flash_${dram_size}dram${debug_en}.txt romcfg.txt

    ## [6] modify address.txt
    if [ "$dram_size" = "16" ]; then
        if [ "$real_16m" = "TRUE" ]; then
            cat address.txt | sed -e 's/^mclk_config/#&/'   \
                -e 's/^#\(mclk_config.*00000092.*\)/\1/'    \
                -e 's/^prom_config/#&/'                     \
                -e 's/#\(prom_config.*20621031.*\)/\1/'     \
                -e 's/^dram_config/#&/'                     \
                -e 's/^#\(dram_config.*0004001b.*\)/\1/'    \
                > address.txt.tmp
        else
            cat address.txt | sed -e 's/^mclk_config/#&/'   \
                -e 's/^#\(mclk_config.*00000092.*\)/\1/'    \
                -e 's/^prom_config/#&/'                     \
                -e 's/#\(prom_config.*20621031.*\)/\1/'     \
                -e 's/^dram_config/#&/'                     \
                -e 's/^#\(dram_config.*0004001e.*\)/\1/'    \
                > address.txt.tmp
        fi
    else
        cat address.txt | sed -e 's/^mclk_config/#&/'       \
            -e 's/^#\(mclk_config.*00000085.*\)/\1/'        \
            -e 's/^prom_config/#&/'                         \
            -e 's/#\(prom_config.*00023131.*\)/\1/'         \
            -e 's/^dram_config/#&/'                         \
            -e 's/^#\(dram_config.*0004001e.*)\)/\1/'       \
        > address.txt.tmp
    fi
    mv -vf address.txt.tmp address.txt

    ## [7] copy OSDString
    echo "Removing OSDString/*.c"
    rm -f ./OSDString/*.c
    if [ "$sup_stb" = "1" ]
    then
        echo "Copy ./STB/OSDString/OSD${osd_lan}_${dram_size}M folder"
        cp -f ./STB/OSDString/OSD${osd_lan}_${dram_size}M/* ./OSDString/
    else
        echo "Copy OSD${osd_lan}_${dram_size}M folder"
        cp -f ./OSDString/OSD${osd_lan}_${dram_size}M/* ./OSDString/
    fi

    ## [8] modify Makefile
    cat Makefile | sed                                      \
        -e "s/^\(SUPPORT_950[ \t]*= *\)./\1${sup_dmp}/"     \
        -e "s/^\(SUPPORT_STB[ \t]*= *\)./\1${sup_stb}/"     \
        -e "s/^#\([ \t]*cd usb && \$(MAKE) INSTALL_DIR=\$(INSTALL_DIR)\)/\1/"  \
        > Makefile.tmp
    mv Makefile.tmp Makefile

    ## [9] modify Winav.h
    if [ "$sup_dmp" = "1" ]
    then
        cat Winav.h | sed                                   \
            -e "s/\/\/.*\(#define CT950_STYLE\)/\1/"        \
            > Winav.h.tmp
    else
        cat Winav.h | sed                           \
            -e "s/^\(#define CT950_STYLE\)/\/\/\1/" \
        > Winav.h.tmp
    fi
    mv -vf Winav.h.tmp Winav.h

    ## [10] modify customer.h
    if [ "$sup_dmp" = "1" ]
    then
        cat customer.h | sed                                   \
            -e "s/\/\/.*\(#define CT951_PLATFORM\)/\1/"        \
            > customer.h.tmp
    else
        cat customer.h | sed                           \
            -e "s/^\(#define CT951_PLATFORM\)/\/\/\1/" \
        > customer.h.tmp
    fi
    mv -vf customer.h.tmp customer.h

    ## [11] copy STB relative file
    if [ "$sup_stb" = "1" ]
    then
        cp ./STB/Overlap_Files/* .
    fi
}

CT909P()
{
    ## [1] copy relative folder
    echo "Copying 909P folder"
    cp -f ./909P/* .

    ## [2] copy 950_Files folder
    if [ "$sup_dmp" = "1" ]; then
        echo "Copying 950_Files folder"
        cp -f ./950_Files/* .
    fi

    ## [3] modify internal.h
    cat internal.h | sed                                                                \
        -e "s/^\(#define *DRAM_CONFIGURATION_TYPE  *\).*/\1DRAM_SIZE_${dram_size}/"     \
        -e 's/^\(#define *CPU_SPEED *\).*/\1CPU_133M/' > internal.h.tmp
    mv -v internal.h.tmp internal.h

    ## [4] copy DVD909.ld
    if [ "$dram_size" = "16" ]; then
        if [ "$debug_en" = "_DBG" ]; then
            cp -vf DVD909_16MDBG.ld DVD909.ld
        else
            cp -vf DVD909_16M.ld DVD909.ld
        fi
    else
        cp -vf DVD909_${dram_size}M.ld DVD909.ld
    fi
    
    ## [4.1] modify DVD909.ld
    if [ "$sup_dmp" = "1" ]
    then
        cat DVD909.ld | sed                                 \
            -e '/ *srv_dram.a.*/d'                          \
            > DVD909.ld.tmp
        mv -vf DVD909.ld.tmp DVD909.ld
    fi

    ## [5] copy romcfg.txt
    cp -vf romcfg${dmpcfg}${stbcfg}_${flash_size}flash_${dram_size}dram${debug_en}.txt romcfg.txt

    ## [6] modify address.txt
#        -e 's/#\(prom_config.*20443131.*\)/\1/'         \
    if [ "$dram_size" = "16" ]; then
        if [ "$real_16m" = "TRUE" ]; then
            cat address.txt | sed -e 's/^mclk_config/#&/'       \
                -e 's/^#\(mclk_config.*00000085.*\)/\1/'        \
                -e 's/^prom_config/#&/'                         \
                -e 's/^dram_config/#&/'                         \
                -e 's/^#\(dram_config.*0108011b.*)\)/\1/'       \
                -e 's/#\(prom_config.*20541010.*909P\)/\1/'     \
            > address.txt.tmp
        else
            cat address.txt | sed -e 's/^mclk_config/#&/'       \
                -e 's/^#\(mclk_config.*00000085.*\)/\1/'        \
                -e 's/^prom_config/#&/'                         \
                -e 's/^dram_config/#&/'                         \
                -e 's/^#\(dram_config.*0108011e.*)\)/\1/'       \
                -e 's/#\(prom_config.*20541010.*909P\)/\1/'     \
            > address.txt.tmp
        fi
    else
        cat address.txt | sed -e 's/^mclk_config/#&/'       \
            -e 's/^#\(mclk_config.*00000085.*\)/\1/'        \
            -e 's/^prom_config/#&/'                         \
            -e 's/^dram_config/#&/'                         \
            -e 's/^#\(dram_config.*0108011e.*)\)/\1/'       \
            -e 's/#\(prom_config.*20443131.*\)/\1/'         \
        > address.txt.tmp
    fi
    mv address.txt.tmp address.txt

    ## [7] copy OSDString
    echo "Removing OSDString/*.c"
    rm -f ./OSDString/*.c
    if [ "$sup_stb" = "1" ]
    then
        echo "Copy ./STB/OSDString/OSD${osd_lan}_${dram_size}M folder"
        cp -f ./STB/OSDString/OSD${osd_lan}_${dram_size}M/* ./OSDString/
    else
        echo "Copy OSD${osd_lan}_${dram_size}M folder"
        cp -f ./OSDString/OSD${osd_lan}_${dram_size}M/* ./OSDString/
    fi

    ## [8] modify Makefile
    cat Makefile | sed                                      \
        -e "s/^\(SUPPORT_950[ \t]*= *\)./\1${sup_dmp}/"     \
        -e "s/^\(SUPPORT_STB[ \t]*= *\)./\1${sup_stb}/"     \
        -e "s/^[ \t]*cd usb && \$(MAKE) INSTALL_DIR=\$(INSTALL_DIR)/#&/"  \
        > Makefile.tmp
    mv Makefile.tmp Makefile

    ## [9] modify Winav.h
    if [ "$sup_dmp" = "1" ]
    then
        cat Winav.h | sed                                   \
            -e "s/\/\/.*\(#define CT950_STYLE\)/\1/"        \
            > Winav.h.tmp
    else
        cat Winav.h | sed                           \
            -e "s/^\(#define CT950_STYLE\)/\/\/\1/" \
        > Winav.h.tmp
    fi
    mv -vf Winav.h.tmp Winav.h

    ## [10] copy STB relative file
    if [ "$sup_stb" = "1" ]
    then
        cp ./STB/Overlap_Files/* .
    fi
}

CT909G()
{
    ## [1] copy relative folder
    echo "Copying 909G folder"
    cp -f ./909G/* .

    ## [2] copy 950_Files folder
    if [ "$sup_dmp" = "1" ]; then
        echo "Copying 950_Files folder"
        cp -f ./950_Files/* .
    fi

    ## [3] modify internal.h
    cat internal.h | sed                                                                \
        -e "s/^\(#define *DRAM_CONFIGURATION_TYPE  *\).*/\1DRAM_SIZE_${dram_size}/"     \
        -e 's/^\(#define *CPU_SPEED *\).*/\1CPU_133M/' > internal.h.tmp
    mv -v internal.h.tmp internal.h

    ## [4] copy DVD909.ld
    if [ "$dram_size" = "16" ]; then
        if [ "$debug_en" = "_DBG" ]; then
            cp -vf DVD909_16MDBG.ld DVD909.ld
        else
            cp -vf DVD909_16M.ld DVD909.ld
        fi
    else
        cp -vf DVD909_${dram_size}M.ld DVD909.ld
    fi
    
    ## [5] copy romcfg.txt
    cp -vf romcfg${dmpcfg}${stbcfg}_${flash_size}flash_${dram_size}dram${debug_en}.txt romcfg.txt

    ## [6] modify address.txt
#    -e 's/#\(prom_config.*20043131.*\)/\1/'         \
    if [ "$dram_size" = "16" ]; then
        if [ "$real_16m" = "TRUE" ]; then
            cat address.txt | sed -e 's/^mclk_config/#&/'       \
                -e 's/^#\(mclk_config.*00000085.*\)/\1/'        \
                -e 's/^prom_config/#&/'                         \
                -e 's/^dram_config/#&/'                         \
                -e 's/^#\(dram_config.*0009001b.*)\)/\1/'       \
                -e 's/#\(prom_config.*20541010.*909G\)/\1/'     \
            > address.txt.tmp
        else
            cat address.txt | sed -e 's/^mclk_config/#&/'       \
                -e 's/^#\(mclk_config.*00000085.*\)/\1/'        \
                -e 's/^prom_config/#&/'                         \
                -e 's/^dram_config/#&/'                         \
                -e 's/^#\(dram_config.*00090018.*)\)/\1/'       \
                -e 's/#\(prom_config.*20541010.*909G\)/\1/'     \
            > address.txt.tmp
        fi
    else
        cat address.txt | sed -e 's/^mclk_config/#&/'       \
            -e 's/^#\(mclk_config.*00000085.*\)/\1/'        \
            -e 's/^prom_config/#&/'                         \
            -e 's/^dram_config/#&/'                         \
            -e 's/^#\(dram_config.*00090018.*)\)/\1/'       \
            -e 's/#\(prom_config.*20043131.*\)/\1/'     \
        > address.txt.tmp
    fi
    mv address.txt.tmp address.txt

    ## [7] copy OSDString
    echo "Removing OSDString/*.c"
    rm -f ./OSDString/*.c
    echo "Copy OSD${osd_lan}_${dram_size}M folder"
    cp -f ./OSDString/OSD${osd_lan}_${dram_size}M/* ./OSDString/

    ## [8] modify Makefile
    cat Makefile | sed                                      \
        -e "s/^\(SUPPORT_950[ \t]*= *\)./\1${sup_dmp}/"     \
        -e "s/^\(SUPPORT_STB[ \t]*= *\)./\1${sup_stb}/"     \
        -e "s/^[ \t]*cd usb && \$(MAKE) INSTALL_DIR=\$(INSTALL_DIR)/#&/"  \
        > Makefile.tmp
    mv Makefile.tmp Makefile
}

Get_IC_Version()
{
## referece Winav.h
    ic_type_array=(IC_VERSION_RESERVED \
                   IC_VERSION_909R \
                   IC_VERSION_219 \
                   IC_VERSION_909P_A_W \
                   IC_VERSION_909P_B_X \
                   IC_VERSION_909G \
                   IC_VERSION_950A \
                   IC_VERSION_952A \
                   IC_VERSION_955A \
                   IC_VERSION_956A \
                   IC_VERSION_956A_DEV \
                   IC_VERSION_951 \
                   IC_VERSION_908G)

#    ic_num_array=(0x0001 \
#                  0x0010 \
#                  0x0011 \
#                  0x0020 \
#                  0x0021 \
#                  0x0030 \
#                  0x0040 \
#                  0x0041 \
#                  0x0042 \
#                  0x0043 \
#                  0x0044 \
#                  0x0045 \
#                  0x00f0)

    ic_num_array=(1 \
                  16 \
                  17 \
                  32 \
                  33 \
                  48 \
                  64 \
                  65 \
                  66 \
                  67 \
                  68 \
                  69 \
                  180)
    platform=$(cat internal.h | sed -ne 's/^ *#define *DECODER_SYSTEM *\([^ ]*\)/\1/p')
    str1="ic_type=\`cat Winav.h | sed -ne '/^#if (DECODER_SYSTEM == $platform)/,/#endif.*$platform/s/^#define *IC_VERSION_ID *\([^ ]*\)/\1/p'\`"
    eval $str1
    echo "IC_TYPE:" $ic_type
    str2="upgname=\`cat Winav.h | sed -ne '/^#if (DECODER_SYSTEM == $platform)/,/#endif.*$platform/s/^#define *CS_UPGAP_NAME *\"\([^ ]*\)\"/\1/p'\`"
    eval $str2
    echo "UPG_NAME:" $upgname

    for ((i=0;i<13;i++))
    do
        #echo $i : ${ic_type_array[i]}
        #echo $i : ${ic_num_array[i]}
        if [ "$ic_type" = "${ic_type_array[i]}" ]
        then
            #echo $i
            ic_version=${ic_num_array[i]}
            #echo $ic_version
        fi
    done

    cat Makefile | sed                                      \
        -e "s/^\(IC_VERSION[ \t]*= *\).*/\1${ic_version}/"  \
        -e "s/^\(UPG_NAME[ \t]*= *\).*/\1${upgname}/"       \
        > Makefile.tmp
    mv Makefile.tmp Makefile
}

CTK_platform()
{
#Get_IC_Version

    case "$ic_system" in
        #DVD909[SR]_EVAL )
        CT909R_IC_SYSTEM )
            CT909R
            ;;
        #DVD909P_[AB]_EVAL | DMP95[256]A_EVAL | DVD956A_DEV )
        CT909P_IC_SYSTEM )
            CT909P
            ;;
        #DVD909G_EVAL )
        CT909G_IC_SYSTEM )
            CT909G
            ;;
    esac
}

while getopts ":d:hwc" opt; do
    case $opt in
    d )
        case $OPTARG in
        16 | 32 | 64 )
            dramtype="DRAM_SIZE_$OPTARG"
            dram_size=$OPTARG ;;
        16r )
            dramtype="DRAM_SIZE_16"
            dram_size=16 
            real_16m=TRUE ;;
        16d )
            dramtype="DRAM_SIZE_16"
            dram_size=16
            debug_en=_DBG ;;
        esac ;;
    c )
        Get_IC_Version
        exit 0 ;;
    w )
        GET_PLATFORM_INFO
        exit 1 ;;
    v )
        exit 1 ;;
    h | \? )
        usage
        exit 1 ;;
    esac
done
shift $(( $OPTIND - 1 ))

## Get initial setting information
platform=$(cat internal.h | sed -ne 's/^ *#define *DECODER_SYSTEM *\([^ ]*\)/\1/p')
ic_system=$(cat platform.h | sed -ne 's/^ *#define *\(CT909[RPG]_IC_SYSTEM\)/\1/p')

sup_stb=$(cat internal.h | sed -ne '/^ *#define *SUPPORT_STB/p')
if [ -n "$sup_stb" ]; then
    sup_stb=1
    stbcfg=_DVB
else
    sup_stb=0
fi

sup_dmp=$(cat internal.h | sed -ne '/^ *#define *SUPPORT_DMP/p')
if [ -n "$sup_dmp" ]; then
    sup_dmp=1
    dmpcfg=_DMP
else
    sup_dmp=0
    dmpcfg=_DVD
fi

flash_size=$(cat internal.h | sed -ne 's/^ *#define *FLASH_SIZE  *FLASH_SIZE_\([0-9]*\)/\1/p')
osd_lan=$(cat internal.h | sed -ne 's/^#define *SELL_DESTINATION *\([^ ]*\)/\1/p')

get_initial_settings

## Get user input, 'y' to continue, 'n' to abort
echo -n "Do you want to continue? (y/n): "
read input

until [ "$input" = "y" -o "$input" = "Y" -o "$input" = "N" -o "$input" = "n" ]
do
    echo -n "Do you want to continue? (y/n): "
    read input
done

#if [ "$input" = "y" -o "$input" = "Y" ]
if echo "$input" | grep -q '^[Yy]$'
then
    ## continue
    CTK_platform
    echo ""
else
    ## abort
    exit 1
fi
