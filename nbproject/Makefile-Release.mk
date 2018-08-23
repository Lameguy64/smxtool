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
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ColorPreview.o \
	${OBJECTDIR}/ConfigClass.o \
	${OBJECTDIR}/ContextClass.o \
	${OBJECTDIR}/ModelClass.o \
	${OBJECTDIR}/ShaderClass.o \
	${OBJECTDIR}/TextureClass.o \
	${OBJECTDIR}/about.o \
	${OBJECTDIR}/common/common.o \
	${OBJECTDIR}/common/smx.o \
	${OBJECTDIR}/common/timloader.o \
	${OBJECTDIR}/configform.o \
	${OBJECTDIR}/glwidget.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/mainform.o \
	${OBJECTDIR}/texturepreview.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-mwindows
CXXFLAGS=-mwindows

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/C/fltk-1.3.4-1/lib -L/C/tinyxml2 -L/C/glew-2.1.0/lib/Release/Win32 -lfltk_images -lfltk_gl -lfltk -lcomctl32 -lcomdlg32 -luuid -lgdi32 -lole32 -lopengl32 -lglu32 -ltinyxml2 -lglew32

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/smxtool.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/smxtool.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/smxtool ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/ColorPreview.o: ColorPreview.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ColorPreview.o ColorPreview.cpp

${OBJECTDIR}/ConfigClass.o: ConfigClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ConfigClass.o ConfigClass.cpp

${OBJECTDIR}/ContextClass.o: ContextClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ContextClass.o ContextClass.cpp

${OBJECTDIR}/ModelClass.o: ModelClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ModelClass.o ModelClass.cpp

${OBJECTDIR}/ShaderClass.o: ShaderClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ShaderClass.o ShaderClass.cpp

${OBJECTDIR}/TextureClass.o: TextureClass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TextureClass.o TextureClass.cpp

${OBJECTDIR}/about.o: about.cxx 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/about.o about.cxx

${OBJECTDIR}/common/common.o: common/common.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/common.o common/common.cpp

${OBJECTDIR}/common/smx.o: common/smx.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/smx.o common/smx.cpp

${OBJECTDIR}/common/timloader.o: common/timloader.cpp 
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/timloader.o common/timloader.cpp

${OBJECTDIR}/configform.o: configform.cxx 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/configform.o configform.cxx

${OBJECTDIR}/glwidget.o: glwidget.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/glwidget.o glwidget.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/mainform.o: mainform.cxx 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mainform.o mainform.cxx

${OBJECTDIR}/texturepreview.o: texturepreview.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/C/tinyxml2 -I/C/fltk-1.3.4-1 -I/C/glew-2.1.0/include -Icommon -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/texturepreview.o texturepreview.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/smxtool.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
