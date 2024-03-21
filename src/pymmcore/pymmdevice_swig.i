// NOTES
// ensure that:
// - %shared_ptr directives are defined for all classes managed through std::shared_ptr.
// - Each class that returns or accepts std::shared_ptr in its methods is
//   properly included and shared in the SWIG interface.
// - These calls to %shared_ptr are placed after the class definition.

%module (package="pymmdevice", directors="1", threads="1") pymmdevice_swig
%feature("director") CPluginManager;
%feature("autodoc", "3");


%include "std_function.i"
%include exception.i
%include std_string.i
%include std_vector.i
%include std_map.i
%include std_pair.i
%include <std_shared_ptr.i>
%template(StrVector) std::vector<std::string>;

// Include or declare other necessary headers or types here
%{
#define SWIG_FILE_WITH_INIT
#include "PluginManager.h"
#include "LoadedDeviceAdapter.h"
%}

%std_function(Functor, void, TEntryData, const char*);

//////////////// LoadedDeviceAdapter ///////////////////////

// %include "LoadedDeviceAdapter.h"
%shared_ptr(LoadedDeviceAdapter)


//////////////// CPluginManager ///////////////////////

%include "PluginManager.h"

// SetSearchPaths is a template method.
// SWIG cannot automatically generate bindings for C++ template methods unless
// explicit instantiations of these templates are provided.
%rename(SetSearchPaths) SetSearchPaths_pywrap;
%extend CPluginManager {
    void SetSearchPaths_pywrap(const std::vector<std::string>& paths) {
        $self->SetSearchPaths(paths.begin(), paths.end());
    }
}

