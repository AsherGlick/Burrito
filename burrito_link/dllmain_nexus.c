#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
// Forward declare the run_link() function defined in burrito_link.c
void run_link();

#ifndef true
#define true TRUE
#endif

#ifndef false
#define false FALSE
#endif

#ifndef nullptr
#define nullptr 0
#endif

////////////////////////////////////////////////////////////////////////////////
// BurritoLinkThread
//
// A helper function that calls the burrito link run_link() function. It exists
// so that it can be passed into the Windows API CreateThread and can be run
// within a the new thread instead of the DLL's main thread.
////////////////////////////////////////////////////////////////////////////////
void WINAPI BurritoLinkThread() {
    run_link();
    return;
}


////////////////////////////////////////////////////////////////////////////////
// start_burrito_link_thread
//
// Creates a new burrito link thread if there is no existing burrito link
// thread running.
////////////////////////////////////////////////////////////////////////////////
HANDLE burrito_link_thread_handle = nullptr;
void start_burrito_link_thread() {
    if (burrito_link_thread_handle != nullptr) {
        return;
    }

    burrito_link_thread_handle = CreateThread(
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)BurritoLinkThread,
        NULL,
        0,
        NULL
    );

    if (burrito_link_thread_handle == nullptr) {
        // Failed to create the thread.
        printf("Failed to create burrito link thread");
    }

}


////////////////////////////////////////////////////////////////////////////////
// stop_burrito_link_thread
//
// Kills a burrito link thread if there is a burrito link thread running. This
// is necessary to avoid an error message on exit.
////////////////////////////////////////////////////////////////////////////////
void stop_burrito_link_thread() {
    if (burrito_link_thread_handle != nullptr) {
        TerminateThread(burrito_link_thread_handle, 0);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Raidcore Nexus
//
// GetAddonDef()
// 
//
//
// These function is present to allow nexus to recognize this dll as a addon.
// These function is the only function that is required all others are optional.
////////////////////////////////////////////////////////////////////////////////
struct AddonDefinition {
	/* required */
	signed int      Signature;      /* Raidcore Addon ID, set to random unqiue negative integer if not on Raidcore */
	signed int      APIVersion;     /* Determines which AddonAPI struct revision the Loader will pass, use the NEXUS_API_VERSION define from Nexus.h */
	const char*     Name;           /* Name of the addon as shown in the library */
	struct AddonVersion {
		signed short      Major;
		signed short      Minor;
		signed short      Build;
		signed short      Revision;
	} Version;
	const char*     Author;         /* Author of the addon */
	const char*     Description;    /* Short description */
	void*           Load;           /* Pointer to Load Function of the addon */
	void*           Unload;         /* Pointer to Unload Function of the addon. Not required if EAddonFlags::DisableHotloading is set. */
	signed int      Flags;          /* Information about the addon */

	/* update fallback */
	signed int      Provider;       /* What platform is the the addon hosted on */
	const char*     UpdateLink;     /* Link to the update resource */

} AddonDef;

extern __declspec(dllexport) struct AddonDefinition* GetAddonDef()
{
	AddonDef.Signature = -217;
	AddonDef.APIVersion = 6; // taken from Nexus.h
	AddonDef.Name = "Burrito Link";
	AddonDef.Version.Major = 1;
	AddonDef.Version.Minor = 4;
	AddonDef.Version.Build = 0;
	AddonDef.Version.Revision = 3;
	AddonDef.Author = "AsherGlick";
	AddonDef.Description = "Automatically provides the link for Burrito.";
	AddonDef.Load = start_burrito_link_thread;
	AddonDef.Unload = stop_burrito_link_thread;
	AddonDef.Flags = 0;

	AddonDef.Provider = 2;
	AddonDef.UpdateLink = "https://github.com/Masgalor/Burrito";

	return &AddonDef;
}
