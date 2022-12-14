Microsoft Windows XP Support from wxWidgets {#plat_msw_winxp}
-------------------------------------------

Windows XP introduces the themes (called "visual styles" in the Microsoft
documentation) in Windows world. As wxWidgets uses the standard Windows
controls for most of its classes, it can take advantage of it without
(almost) any effort from your part. The only thing you need to do if you
want your program to honour the visual style setting of Windows XP is to
add the manifest file to your program (this is not at all specific to
wxWidgets programs but is required for all Windows applications).

wxWidgets now includes manifest resources in wx.rc, so it should be enough to
include "wx/msw/wx.rc" in your application's resource file and you get
XP look automatically. Notice that MSVC embeds manifest in the
executables it produces and wxWidgets doesn't use its own manifest when using
this compiler. And if you don't want to use wxWidgets manifest with another
compiler you may define wxUSE_NO_MANIFEST as 1 prior to including wx/msw/wx.rc.


Finally, if all else fails you may always use a manifest manually. For this you
need to create your own manifest file and put it in a file called
"yourapp.exe.manifest" in the same directory where "yourapp.exe" resides.
Alternatively, you can include the manifest in your applications resource
section. Please see the MSDN documentation at

http://msdn.microsoft.com/en-us/library/windows/desktop/bb773175

for more details.

Here is the example manifest which you can put into controls.exe.manifest
file to test theme support using the controls sample:

    <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
    <assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
    <assemblyIdentity
        version="0.64.1.0"
        processorArchitecture="x86"
        name="Controls"
        type="win32"
    />
    <description>Controls: wxWidgets sample application</description>
    <dependency>
        <dependentAssembly>
            <assemblyIdentity
                type="win32"
                name="Microsoft.Windows.Common-Controls"
                version="6.0.0.0"
                processorArchitecture="*"
                publicKeyToken="6595b64144ccf1df"
                language="*"
            />
        </dependentAssembly>
    </dependency>
    </assembly>
