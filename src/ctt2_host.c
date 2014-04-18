#undef _DEBUG
#include <stdio.h>
#include <conio.h>
#include <Python.h>

#include "system/ctt2_host.h"
#include "system/ctt2.h"
#include "hwgfx/label.h"
#include "hwgfx/quads.h"
#include "hwgfx/blend_control.h"

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
#define FAIL_RETURN { api_fail_hard(); return; }
#define CLIENT_FUNCTION(x,y) client_if.##x = PyObject_GetAttrString\
(client_if.__module,y); if(client_if.##x==0) FAIL_RETURN
int api_init() {
    PyObject *module_name;
    embed_modules();
    module_name         = PyString_FromString("kittens.main");
    client_if.__module  = PyImport_Import(module_name);
    Py_DECREF(module_name);
    if(client_if.__module == 0) FAIL_RETURN
    #include "api-includes/client-handler-inventory.h"
    return _pycall_noargs(client_if.init);
}
#undef CLIENT_FUNCTION

//terminate
#define CLIENT_FUNCTION(x,y) Py_CLEAR(client_if.##x);
void api_drop() {
    _pycall_noargs(client_if.finalize);
    #include "api-includes/client-handler-inventory.h"
    Py_CLEAR(client_if.__module);
}

/*****************************************************************************/
#include "api-includes/client-handlers.h"  //C function bindings for client_if

/*
 * implementations of modules
 */

#define MODULE_FUNC static PyObject* 
#define DEF_ARGS (PyObject *self, PyObject *args )
#define INPUT_ARGS PyArg_ParseTuple

#include "api-includes/modules/hwgfx.h"
#include "api-includes/modules/host.h"

void embed_modules() {
    Py_InitModule("hwgfx",  hwgfx_methods); 
    Py_InitModule("host",   host_methods); 
    api_checkfailure();
}
#undef CLIENT_FUNCTION