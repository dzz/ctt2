#ifndef __CTT2_RT_MODULE_CODES__
#define __CTT2_RT_MODULE_CODES__

#define CTT2_RT_MODULE_LOG            (1)
#define CTT2_RT_MODULE_DISPLAY        (2)
#define CTT2_RT_MODULE_OPENGL         (4)
#define CTT2_RT_MODULE_AUDIO          (8)
#define CTT2_RT_MODULE_WINDOW_MSGS    (16)
#define CTT2_RT_MODULE_TEXT_INPUT     (32)
#define CTT2_RT_MODULE_TIMER          (64)
#define CTT2_RT_MODULE_GAMEPAD        (128)
#define CTT2_RT_MODULE_PYTHON         (256)
#define CTT2_RT_MODULE_CORE           (512)
#define CTT2_CLIENT_APPLICATION       (1024)
#define CTT2_API_BRIDGE               (2048)

#define MODULE_LOADED 1
#define MODULE_FAILURE 0

extern char * ctt2_module_from_code( unsigned int module);

#endif
