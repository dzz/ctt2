
/* define functions which python application must
 * provide which may be called by the host */

CLIENT_FUNCTION( init,                 "init"                 )
CLIENT_FUNCTION( tick,                 "tick"                 )
CLIENT_FUNCTION( dispatch_mouseup,     "dispatch_mouseup"     )
CLIENT_FUNCTION( dispatch_mousedown,   "dispatch_mousedown"   )
CLIENT_FUNCTION( dispatch_mousemotion, "dispatch_mousemotion" )
CLIENT_FUNCTION( dispatch_key,         "dispatch_key"         )
CLIENT_FUNCTION( finalize,             "finalize"             )

