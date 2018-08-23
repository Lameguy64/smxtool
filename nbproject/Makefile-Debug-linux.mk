#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug-linux
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/b4460d1a/common.o \
	${OBJECTDIR}/_ext/4e2fa2fa/profileforms.o \
	${OBJECTDIR}/_ext/b4460d1a/profiles.o \
	${OBJECTDIR}/_ext/b4460d1a/smx.o \
	${OBJECTDIR}/_ext/b4460d1a/timloader.o \
	${OBJECTDIR}/ColorPreview.o \
	${OBJECTDIR}/CommsClass.o \
	${OBJECTDIR}/ConfigClass.o \
	${OBJECTDIR}/ContextClass.o \
	${OBJECTDIR}/ModelClass.o \
	${OBJECTDIR}/SerialClass.o \
	${OBJECTDIR}/ShaderClass.o \
	${OBJECTDIR}/TextureClass.o \
	${OBJECTDIR}/about.o \
	${OBJECTDIR}/commsform.o \
	${OBJECTDIR}/configform.o \
	${OBJECTDIR}/glwidget.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/mainform.o \
	${OBJECTDIR}/texturepreview.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lfltk_images -lfltk_gl -lfltk -ltinyxml2 -lGL -lGLU -lGLEW

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/smxtool

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/smxtool: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/smxtool ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/b4460d1a/common.o: ../common/common.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/b4460d1a
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/b4460d1a/common.o ../common/common.cpp

${OBJECTDIR}/_ext/4e2fa2fa/profileforms.o: ../common/forms/profileforms.cxx 
	${MKDIR} -p ${OBJECTDIR}/_ext/4e2fa2fa
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4e2fa2fa/profileforms.o ../common/forms/profileforms.cxx

${OBJECTDIR}/_ext/b4460d1a/profiles.o: ../common/profiles.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/b4460d1a
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/b4460d1a/profiles.o ../common/profiles.cpp

${OBJECTDIR}/_ext/b4460d1a/smx.o: ../common/smx.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/b4460d1a
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/b4460d1a/smx.o ../common/smx.cpp

${OBJECTDIR}/_ext/b4460d1a/timloader.o: ../common/timloader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/b4460d1a
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/b4460d1a/timloader.o ../common/timloader.cpp

${OBJECTDIR}/ColorPreview.o: ColorPreview.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ColorPreview.o ColorPreview.cpp

${OBJECTDIR}/CommsClass.o: CommsClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CommsClass.o CommsClass.cpp

${OBJECTDIR}/ConfigClass.o: ConfigClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ConfigClass.o ConfigClass.cpp

${OBJECTDIR}/ContextClass.o: ContextClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ContextClass.o ContextClass.cpp

${OBJECTDIR}/ModelClass.o: ModelClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ModelClass.o ModelClass.cpp

${OBJECTDIR}/SerialClass.o: SerialClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SerialClass.o SerialClass.cpp

${OBJECTDIR}/ShaderClass.o: ShaderClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ShaderClass.o ShaderClass.cpp

${OBJECTDIR}/TextureClass.o: TextureClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TextureClass.o TextureClass.cpp

${OBJECTDIR}/about.o: about.cxx 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/about.o about.cxx

${OBJECTDIR}/commsform.o: commsform.cxx 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/commsform.o commsform.cxx

${OBJECTDIR}/configform.o: configform.cxx 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/configform.o configform.cxx

${OBJECTDIR}/glwidget.o: glwidget.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/glwidget.o glwidget.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/mainform.o: mainform.cxx 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mainform.o mainform.cxx

${OBJECTDIR}/texturepreview.o: texturepreview.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -I../common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/texturepreview.o texturepreview.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/smxtool

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
