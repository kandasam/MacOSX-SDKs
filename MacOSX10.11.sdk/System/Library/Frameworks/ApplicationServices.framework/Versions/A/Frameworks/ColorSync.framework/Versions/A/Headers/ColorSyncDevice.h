/*
 * ColorSync - ColorSyncDevice.h
 * Copyright (c)  2008 Apple Inc.
 * All rights reserved.
 */

#ifndef __COLORSYNCDEVICE__
#define __COLORSYNCDEVICE__

#ifdef __cplusplus
extern "C" {
#endif

#include <ColorSync/ColorSyncProfile.h>

CSEXTERN CFStringRef kColorSyncDeviceID;                /* CFUUIDRef */
CSEXTERN CFStringRef kColorSyncDeviceClass;             /* one of the below : */
    CSEXTERN CFStringRef kColorSyncCameraDeviceClass;   /* cmra */
    CSEXTERN CFStringRef kColorSyncDisplayDeviceClass;  /* mntr */
    CSEXTERN CFStringRef kColorSyncPrinterDeviceClass;  /* prtr */
    CSEXTERN CFStringRef kColorSyncScannerDeviceClass;  /* scnr */

CSEXTERN CFStringRef kColorSyncDeviceProfileURL;

CSEXTERN CFStringRef kColorSyncDeviceDescription;       /* CFString with a name in current locale */
CSEXTERN CFStringRef kColorSyncDeviceDescriptions;      /* CFDictionary with localized names */

CSEXTERN CFStringRef kColorSyncFactoryProfiles;         /* CFDictionary containing factory profile info */
CSEXTERN CFStringRef kColorSyncCustomProfiles;          /* CFDictionary containing custom profile info */

CSEXTERN CFStringRef kColorSyncDeviceModeDescription;   /* CFString, e.g. Glossy, Best Quality */
CSEXTERN CFStringRef kColorSyncDeviceModeDescriptions;  /* CFDictionary with localized mode names */
CSEXTERN CFStringRef kColorSyncDeviceDefaultProfileID;  /* see below */
CSEXTERN CFStringRef kColorSyncDeviceHostScope;         /* kCFPreferences{Current,Any}Host */
CSEXTERN CFStringRef kColorSyncDeviceUserScope;         /* kCFPreferences{Current,Any}User */
CSEXTERN CFStringRef kColorSyncProfileHostScope;        /* kCFPreferences{Current,Any}Host */
CSEXTERN CFStringRef kColorSyncProfileUserScope;        /* kCFPreferences{Current,Any}User */

CSEXTERN CFStringRef kColorSyncDeviceProfileIsFactory;  /* Present in ColorSyncDeviceProfileInfo dictionary.*/
                                                        /* See ColorSyncDeviceProfileIterateCallback below. */
CSEXTERN CFStringRef kColorSyncDeviceProfileIsDefault;  /* ditto */
CSEXTERN CFStringRef kColorSyncDeviceProfileIsCurrent;  /* ditto */
CSEXTERN CFStringRef kColorSyncDeviceProfileID;         /* ditto */

CSEXTERN CFStringRef kColorSyncDeviceRegisteredNotification;        /* com.apple.ColorSync.DeviceRegisteredNotification */
CSEXTERN CFStringRef kColorSyncDeviceUnregisteredNotification;      /* com.apple.ColorSync.DeviceUnregisteredNotification */
CSEXTERN CFStringRef kColorSyncDeviceProfilesNotification;         /* com.apple.ColorSync.DeviceProfilesNotification */
CSEXTERN CFStringRef kColorSyncDisplayDeviceProfilesNotification;   /* com.apple.ColorSync.DisplayProfileNotification */

CSEXTERN bool ColorSyncRegisterDevice (CFStringRef deviceClass, CFUUIDRef deviceID, CFDictionaryRef deviceInfo);
   /*
    *   deviceInfo  -a dictionary containing information needed to register a device.
    *   ----------------------------------------------------------------------------
    *               
    *               Required keys:
    *               ==============
    *                   kColorSyncDeviceDescriptions: CFDictionary with localized names of the device.
    *                                                 Localization keys must be five character strings
    *                                                 containing language code and region code in the
    *                                                 lc_RG format and it must contain (at least) the "en_US" locale.
    *                   kColorSyncFactoryProfiles   : CFDictionary with factory profile info CFDictionaries
    *                                                 The keys are the profile IDs and the values
    *                                                 are the profile info dictionaries.
    *               Optional keys:
    *               ==============
    *                   kColorSyncDeviceHostScope   : host scope of the device;
    *                                                 one of kCFPreferences{Current,Any}Host;
    *                                                 if unspecified kCFPreferencesCurrentHost is
    *                                                 assumed.
    *                   kColorSyncDeviceUserScope   : user scope of the device;
    *                                                 one of kCFPreferences{Current,Any}User;
    *                                                 if unspecified kCFPreferencesCurrentUser is
    *                                                 assumed.
    *
    *           factory profiles dictionary - value for the key kColorSyncFactoryProfiles in deviceInfo
    *           -------------------------------------------------------------------------------------
    *               Required keys and values:
    *               ========================
    *                   Each profile is identified by a ProfileID (of CFStringRef type) which used as the key.
    *                   Value associated with the key is a  profile info dictionary
    *                   that describes an individual device profile.
    *
    *                   kColorSyncDeviceDefaultProfileID: the associated value must be one of the ProfileID
    *                                                     present in the dictionary. Presence of this 
    *                                                     key is not required if there is only one factory profile.
    *
    *                   profile info CFDictionary
    *                   --------------------------------
    *                   Required keys:
    *                   ==============
    *                       kColorSyncDeviceProfileURL      :CFURLRef of the profile to be registered
    *                       kColorSyncDeviceModeDescriptions:CFDictionary with localized device mode
    *                                                        names for the profile. Localization keys 
    *                                                        must be five character strings containing
    *                                                        language code and region code in the lc_RG 
    *                                                        format and it must contain (at least) the
    *                                                        "en_US" locale.
    *                                                        E.g. "en_US" "Glossy Paper with best quality"
    *
    * Example of deviceInfo dictionary:
    *
    *   <<
    *       kColorSyncDeviceDescriptions   <<
    *                                           en_US  My Little Printer
    *                                           de_DE  Mein Kleiner Drucker
    *                                           fr_FR  Mon petit immprimeur
    *                                           ...
    *                                       >>
    *       kColorSyncFactoryProfiles       <<
    *                                           CFSTR("Profile 1")  <<
    *                                                                   kColorSyncDeviceProfileURL    {CFURLRef}
    *
    *                                                                   kColorSyncDeviceModeDescriptions    <<
    *                                                                                                           en_US Glossy Paper
    *                                                                                                           de_DE Glanzpapier
    *                                                                                                           fr_FR Papier glace
    *                                                                                                           ...
    *                                                                                                       >>
    *                                           ...
    *                                           
    *                                           kColorSyncDeviceDefaultProfileID  CFSTR("Profile 1")
    *                                       >>
    *       kColorSyncDeviceUserScope   kCFPreferencesAnyUser
    *
    *       kColorSyncDeviceHostScope   kCFPreferencesCurrentHost
    *   <<
    *
    * Notes:    1. Scope for factory profiles is exactly the same as the device scope.
    *           2. Pass kCFNull in lieu of the profile URL or no URl key/value pair at all if 
    *              factory profile is not available. This will enable setting custom profile.
    *           3. For the reasons of compatibility with legacy API, it is recommended that the
    *              profile keys are created as CFStrings from uint32_t numbers as follows:
    *              CFStringRef key = CFStringCreateWithFormat(NULL, NULL, CFSTR("%u"), (uint32_t) i);
    *
    *   returns true on success and false in case of failure
    */

CSEXTERN bool ColorSyncUnregisterDevice (CFStringRef deviceClass, CFUUIDRef deviceID);
   /*
    *   Unregister a device of given deviceClass and deviceID.
    *
    *   returns true on success and false in case of failure
    */

CSEXTERN bool ColorSyncDeviceSetCustomProfiles (CFStringRef deviceClass, CFUUIDRef deviceID, CFDictionaryRef profileInfo);
   /*
    *                   profileInfo is a CFDictionary containing the information about 
    *                   custom profiles to be set in lieu of factory profiles.
    *                   Required keys:
    *                   ==============
    *                       ProfileIDs which must be the subset of the ProfileIDs that device was registered with
    *                       or kColorSyncDeviceDefaultProfileID for setting custom default profile.
    *
    *                   Required values:
    *                   ==============
    *                       CFURLRef of the profile to be set as a custom profile.
    *
    *                   Optional keys:
    *                   ==============
    *                       kColorSyncProfileHostScope  : host scope of the profile;
    *                                                     one of kCFPreferences{Current,Any}Host;
    *                                                     if unspecified kCFPreferencesCurrentHost
    *                                                     is assumed.
    *                       kColorSyncProfileUserScope  : user scope of the profile;
    *                                                     one of kCFPreferences{Current,Any}User;
    *                                                     if unspecified kCFPreferencesCurrentUser
    *                                                     is assumed.
    *
    *
    * Notes:    1. Profile scope for custom profiles cannot exceed scope of the factory profiles.
    *           2. There is only one host scope and user scope per dictionary (i.e. per call)
    *           3. Pass kCFNull in lieu of the profile URL to unset the custom profile and
    *              reset the current profile to the factory profile.
    *
    *   returns true on success and false in case of failure
    */

CSEXTERN CFDictionaryRef ColorSyncDeviceCopyDeviceInfo (CFStringRef deviceClass, CFUUIDRef devID);
   /*
    *   Returns a dictionary with the following keys and values resolved for the current host and current user.
    *
    *   <<
    *       kColorSyncDeviceClass                   {camera, display, printer, scanner}
    *       kColorSyncDeviceID                      {CFUUIDRef registered with ColorSync}
    *       kColorSyncDeviceDescription             {localized device description}
    *       kColorSyncFactoryProfiles  (dictionary) <<
    *                                                   {ProfileID}    (dictionary) <<
    *                                                                                   kColorSyncDeviceProfileURL      {CFURLRef or kCFNull}
    *                                                                                   kColorSyncDeviceModeDescription {localized mode description}
    *                                                                               >>
    *                                                    ...
    *                                                   kColorSyncDeviceDefaultProfileID {ProfileID}
    *                                               >>
    *       kColorSyncCustomProfiles  (dictionary) <<
    *                                                   {ProfileID}    {CFURLRef or kCFNull}
    *                                                   ...
    *                                              <<
    *       kColorSyncDeviceUserScope              {kCFPreferencesAnyUser or kCFPreferencesCurrentUser}
    *       kColorSyncDeviceHostScope              {kCFPreferencesAnyHost or kCFPreferencesCurrentHost}
    *   >>
    */
    
typedef bool (*ColorSyncDeviceProfileIterateCallback) (CFDictionaryRef colorSyncDeviceProfileInfo,
                                                       void* userInfo);
   /*
    *   colorSyncDeviceProfileInfo contains the following keys: 
    *   <<
    *       kColorSyncDeviceClass                   {camera, display, printer, scanner}
    *       kColorSyncDeviceID                      {CFUUIDRef registered with ColorSync}
    *       kColorSyncDeviceDescription             {localized device description}
    *       kColorSyncDeviceModeDescription         {localized device mode description}
    *       kColorSyncDeviceProfileID               {ProfileID registered with ColorSync}
    *       kColorSyncDeviceProfileURL              {CFURLRef registered with ColorSync}
    *       kColorSyncDeviceProfileIsFactory        {kCFBooleanTrue or kCFBooleanFalse}
    *       kColorSyncDeviceProfileIsDefault        {kCFBooleanTrue or kCFBooleanFalse}
    *       kColorSyncDeviceProfileIsCurrent        {kCFBooleanTrue or kCFBooleanFalse}
    *   >>
    */
                                                       
CSEXTERN void ColorSyncIterateDeviceProfiles(ColorSyncDeviceProfileIterateCallback callBack,
                                            void* userInfo);

    CSEXTERN CFUUIDRef CGDisplayCreateUUIDFromDisplayID (uint32_t displayID);
    /*
     * A utility function converting displayID to CFUUIDRef
     */
    
    CSEXTERN uint32_t CGDisplayGetDisplayIDFromUUID (CFUUIDRef uuid);
    /*
     * A utility function converting first 32 bits of CFUUIDRef to displayID
     */
    

#ifdef __cplusplus
}
#endif

#endif /* __COLORSYNCDEVICE__ */
