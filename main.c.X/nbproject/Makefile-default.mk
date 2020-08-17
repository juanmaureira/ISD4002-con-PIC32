#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile

# Environment
# Adding MPLAB X bin directory to path
PATH:=/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/:$(PATH)
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/main.c.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/main.c.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o ${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o ${OBJECTDIR}/_ext/1472/User.o ${OBJECTDIR}/_ext/1472/2Mdelay.o ${OBJECTDIR}/_ext/1472/Helpers.o ${OBJECTDIR}/_ext/1472/ISD4002.o ${OBJECTDIR}/_ext/1472/Telit-GM862.o ${OBJECTDIR}/_ext/1472/Servicio.o ${OBJECTDIR}/_ext/1472/Adc.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o.d ${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o.d ${OBJECTDIR}/_ext/1472/User.o.d ${OBJECTDIR}/_ext/1472/2Mdelay.o.d ${OBJECTDIR}/_ext/1472/Helpers.o.d ${OBJECTDIR}/_ext/1472/ISD4002.o.d ${OBJECTDIR}/_ext/1472/Telit-GM862.o.d ${OBJECTDIR}/_ext/1472/Servicio.o.d ${OBJECTDIR}/_ext/1472/Adc.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o ${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o ${OBJECTDIR}/_ext/1472/User.o ${OBJECTDIR}/_ext/1472/2Mdelay.o ${OBJECTDIR}/_ext/1472/Helpers.o ${OBJECTDIR}/_ext/1472/ISD4002.o ${OBJECTDIR}/_ext/1472/Telit-GM862.o ${OBJECTDIR}/_ext/1472/Servicio.o ${OBJECTDIR}/_ext/1472/Adc.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

# Path to java used to run MPLAB X when this makefile was created
MP_JAVA_PATH="/System/Library/Java/JavaVirtualMachines/1.6.0.jdk/Contents/Home/bin/"
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC="/Applications/microchip/mplabc32/v2.02/bin/pic32-gcc"
# MP_BC is not defined
MP_AS="/Applications/microchip/mplabc32/v2.02/bin/pic32-as"
MP_LD="/Applications/microchip/mplabc32/v2.02/bin/pic32-ld"
MP_AR="/Applications/microchip/mplabc32/v2.02/bin/pic32-ar"
DEP_GEN=${MP_JAVA_PATH}java -jar "/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/extractobjectdependencies.jar" 
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps
MP_CC_DIR="/Applications/microchip/mplabc32/v2.02/bin"
# MP_BC_DIR is not defined
MP_AS_DIR="/Applications/microchip/mplabc32/v2.02/bin"
MP_LD_DIR="/Applications/microchip/mplabc32/v2.02/bin"
MP_AR_DIR="/Applications/microchip/mplabc32/v2.02/bin"
# MP_BC_DIR is not defined

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/main.c.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o: ../Yelcho_PIC32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o.d" -o ${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o ../Yelcho_PIC32.c  
	
${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o: ../dee_emulation_pic32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o.d" -o ${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o ../dee_emulation_pic32.c  
	
${OBJECTDIR}/_ext/1472/User.o: ../User.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/User.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/User.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/User.o.d" -o ${OBJECTDIR}/_ext/1472/User.o ../User.c  
	
${OBJECTDIR}/_ext/1472/2Mdelay.o: ../2Mdelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/2Mdelay.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/2Mdelay.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/2Mdelay.o.d" -o ${OBJECTDIR}/_ext/1472/2Mdelay.o ../2Mdelay.c  
	
${OBJECTDIR}/_ext/1472/Helpers.o: ../Helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Helpers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Helpers.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Helpers.o.d" -o ${OBJECTDIR}/_ext/1472/Helpers.o ../Helpers.c  
	
${OBJECTDIR}/_ext/1472/ISD4002.o: ../ISD4002.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/ISD4002.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ISD4002.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/ISD4002.o.d" -o ${OBJECTDIR}/_ext/1472/ISD4002.o ../ISD4002.c  
	
${OBJECTDIR}/_ext/1472/Telit-GM862.o: ../Telit-GM862.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Telit-GM862.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Telit-GM862.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Telit-GM862.o.d" -o ${OBJECTDIR}/_ext/1472/Telit-GM862.o ../Telit-GM862.c  
	
${OBJECTDIR}/_ext/1472/Servicio.o: ../Servicio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Servicio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Servicio.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Servicio.o.d" -o ${OBJECTDIR}/_ext/1472/Servicio.o ../Servicio.c  
	
${OBJECTDIR}/_ext/1472/Adc.o: ../Adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Adc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Adc.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Adc.o.d" -o ${OBJECTDIR}/_ext/1472/Adc.o ../Adc.c  
	
else
${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o: ../Yelcho_PIC32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o.d" -o ${OBJECTDIR}/_ext/1472/Yelcho_PIC32.o ../Yelcho_PIC32.c  
	
${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o: ../dee_emulation_pic32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o.d" -o ${OBJECTDIR}/_ext/1472/dee_emulation_pic32.o ../dee_emulation_pic32.c  
	
${OBJECTDIR}/_ext/1472/User.o: ../User.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/User.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/User.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/User.o.d" -o ${OBJECTDIR}/_ext/1472/User.o ../User.c  
	
${OBJECTDIR}/_ext/1472/2Mdelay.o: ../2Mdelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/2Mdelay.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/2Mdelay.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/2Mdelay.o.d" -o ${OBJECTDIR}/_ext/1472/2Mdelay.o ../2Mdelay.c  
	
${OBJECTDIR}/_ext/1472/Helpers.o: ../Helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Helpers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Helpers.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Helpers.o.d" -o ${OBJECTDIR}/_ext/1472/Helpers.o ../Helpers.c  
	
${OBJECTDIR}/_ext/1472/ISD4002.o: ../ISD4002.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/ISD4002.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ISD4002.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/ISD4002.o.d" -o ${OBJECTDIR}/_ext/1472/ISD4002.o ../ISD4002.c  
	
${OBJECTDIR}/_ext/1472/Telit-GM862.o: ../Telit-GM862.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Telit-GM862.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Telit-GM862.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Telit-GM862.o.d" -o ${OBJECTDIR}/_ext/1472/Telit-GM862.o ../Telit-GM862.c  
	
${OBJECTDIR}/_ext/1472/Servicio.o: ../Servicio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Servicio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Servicio.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Servicio.o.d" -o ${OBJECTDIR}/_ext/1472/Servicio.o ../Servicio.c  
	
${OBJECTDIR}/_ext/1472/Adc.o: ../Adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Adc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Adc.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I".." -MMD -MF "${OBJECTDIR}/_ext/1472/Adc.o.d" -o ${OBJECTDIR}/_ext/1472/Adc.o ../Adc.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/main.c.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/main.c.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-L"../C:/Program Files/Microchip/MPLAB C32 Suite/lib",-L"../C:/Program Files/Microchip/MPLAB C32 Suite/pic32mx/lib",-Map="${DISTDIR}/main.c.X.${IMAGE_TYPE}.map" 
else
dist/${CND_CONF}/${IMAGE_TYPE}/main.c.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/main.c.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-L"../C:/Program Files/Microchip/MPLAB C32 Suite/lib",-L"../C:/Program Files/Microchip/MPLAB C32 Suite/pic32mx/lib",-Map="${DISTDIR}/main.c.X.${IMAGE_TYPE}.map"
	${MP_CC_DIR}/pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/main.c.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  
endif


# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
