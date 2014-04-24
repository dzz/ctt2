#undef _DEBUG
#include <stdio.h>
#include <conio.h>
#include <Python.h>
#include <SDL_image.h>

#include "drawing/drawingSurfaces.h"
#include "system/ctt2_host.h"
#include "system/ctt2.h"
#include "hwgfx/shader.h"
#include "hwgfx/label.h"
#include "hwgfx/rect.h"
#include "hwgfx/blend_control.h"
#include "hwgfx/texture.h"
#include "hwgfx/primitive.h"

//build a structure to hold required host callables inside of client
#define CLIENT_FUNCTION(x,y) PyObject* x;
typedef struct {
    PyObject* __module;
    #include "api-includes/client-handler-inventory.h"
} CLIENT_HANDLERS;
#undef CLIENT_FUNCTION
static CLIENT_HANDLERS client_if;


void embed_modules();                       // fwd declaration for use in init

#include "api-includes\api_py_util.h"       // error handling, function calls


//load the python client code:
#define FAIL_RETURN { api_fail_hard(); return 1; }
#define CLIENT_FUNCTION(x,y) client_if.##x = PyObject_GetAttrString\
(client_if.__module,y); if(client_if.##x==0) FAIL_RETURN
int api_init() {
    embed_modules();
    Py_Initialize();
    client_if.__module = PyImport_ImportModule("client.ctt2.main");
    if(client_if.__module == 0) FAIL_RETURN
    #include "api-includes/client-handler-inventory.h"
    return _pycall_noargs(client_if.init);
}
#undef CLIENT_FUNCTION

//terminate
#define CLIENT_FUNCTION(x,y) Py_CLEAR(client_if.##x);
int api_drop() {
    int ret; 
    ret = _pycall_noargs(client_if.finalize);
    #include "api-includes/client-handler-inventory.h"
    Py_CLEAR(client_if.__module);
    return ret;
}

#include "api-includes/client-handlers.h"  //C function bindings for client_if

/*
 * implementations of modules
 */

#define MODULE_FUNC static PyObject* 
#define DEF_ARGS (PyObject *self, PyObject *args )
#define INPUT_ARGS PyArg_ParseTuple

#include "api-includes/modules/host.h"
#include "api-includes/modules/hwgfx.h"
#include "api-includes/modules/localgfx.h"
void embed_modules() {

    PyImport_AppendInittab("host",      &PyInit_host);
    PyImport_AppendInittab("hwgfx",     &PyInit_hwgfx);
    PyImport_AppendInittab("localgfx",  &PyInit_localgfx);
    api_checkfailure();
}
#undef CLIENT_FUNCTION
