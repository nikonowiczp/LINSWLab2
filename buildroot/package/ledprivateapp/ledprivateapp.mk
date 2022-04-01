##########################
### ledprivateapp
##########################

LEDPRIVATEAPP_VERSION = 1.0
LEDPRIVATEAPP_SITE = $(TOPDIR)/../cpp_app/src
LEDPRIVATEAPP_SITE_METHOD = local
LEDPRIVATEAPP_DEPENDENCIES = libgpiod
LEDPRIVATEAPP_LICENSE = Proprietary

$(eval $(cmake-package))
