# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.

ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

#===== USEFILE - the file containing the usage message for the application. 
USEFILE=

#===== LDFLAGS - add the flags to the linker command line.
LDFLAGS+=-Wl,-E -M

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PROJECT_ROOT)/SysProject  \
	$(PROJECT_ROOT)/DeviceLibrary/CANDevice  \
	$(PROJECT_ROOT)/DeviceLibrary/FileDevice  \
	$(PROJECT_ROOT)/DataProcess/JointData  \
	$(PROJECT_ROOT)/Algorithm/JointAlgo  \
	$(PROJECT_ROOT)/DataProcess/ADCalibrate

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PROJECT_ROOT)

include $(MKFILES_ROOT)/qmacros.mk
ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif
include $(QNX_INTERNAL)

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))

