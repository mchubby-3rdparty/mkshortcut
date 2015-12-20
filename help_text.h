/******************************************************************************
Copyright 2010 Daniel Miranda. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

const wchar_t g_helpText[] =
L"Creates a shortcut with advanced properties for use by Window 7's new Taskbar.\n"
L"\n"
L"mkshortcut -output <filename> -target <filename> [-parameters <parameters>]\n"
L"	[-workingdir <directory>] [-appid <string>] [-relaunchcmd <command>]\n"
L"	[-relaunchname <name>]\n"
L"\n"
L"-output <filename>            Where to save the shortcut.\n"
L"-target <filename>            Target of the newly created shortcut, not\n"
L"                              necessarily an executable: files will be opened\n"
L"                              by the program associated with their extensions.\n"
L"-parameters <parameters>      Parameters to be passed to the shortcut's target.\n"
L"                              Wrap all of them in a single pair of quotes if\n"
L"                              passing multiple switches.\n"
L"-workingdir <pathname>        Working directory used by the target program.\n"
L"-appid <string>               String used by Windows 7's Taskbar to identify\n"
L"                              applications.\n"
L"-relaunchcommand <command>    Command to be used when relaunching this shortcut\n"
L"                              using Windows 7's Taskbar, whether through Middle\n"
L"                              Click/Shift+Click, or after being pinned.\n"
L"-relaunchname <string>        User-friendly name displayed for the relaunch \n"
L"                              command.\n"
L"                              You may use a translatable resource by preceding\n"
L"                              its name with an \'@\' character.\n"
L"\n"
L"The following is only meaningful if you are using Windows 7 (or newer).\n"
L"\n"
L"\n"
L"The ApplicationUserModelID (AppID in short), controls grouping of different\n"
L"windows in the taskbar. Windows sharing a common AppID (inherited from a\n"
L"process-wide value by default) are grouped together.\n"
L"\n"
L"An AppID must have at most 128 characters, and has the following form:\n"
L"CompanyName.ProductName.SubProduct.VersionInformation\n"
L"The last two items may be ommited.\n"
L"\n"
L"AppIDs can be set for the whole process through both WinAPI's\n"
L"SetCurrentProcessExplicitUserModelAppID and shortcut's properties.\n"
L"The latter is, of course, the method used in this piece of software.\n"
L"\n"
L"You can also edit the AppID of a single window. In this case, you are also\n"
L"able change the command used when pinning/relaunching its Taskbar entry.\n"
L"\n"
L"You need to set both the command and a friendly name: ommiting either causes\n"
L"the other to be ignored as well. Unfortunately, this particular feature seems\n"
L"not to work from shortcuts: both properties are ignored. If you know of a\n"
L"workaround, please contact me at danielkza2 at gmail dot com.";