// 2025/11/12 10:37:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "File.h"


namespace File
{
    static wxMemoryBuffer bytes;
}


void File::Create(const wxString &path)
{
    wxFile file;

    if (file.Open(path))
    {
        wxFileOffset file_size = file.Length();

        bytes.Clear();

        bytes.SetBufSize(file_size);

        file.Read(bytes.GetData(), file_size);

        bytes.SetDataLen(file_size);
    }
}


uint File::GetVersion()
{
    unsigned char *p = (unsigned char *)bytes.GetData();

    uint result = 0;

    std::memcpy(&result, (const void *)&(p[0x2000]), 4);

    return result;
}


wxString File::GetDateBuild()
{
    char *p = (char *)bytes.GetData();

    p += 0x2004;

    wxString result;

    while(*p)
    {
        result.Append(*p);
        p++;
    }

    return result;
}
