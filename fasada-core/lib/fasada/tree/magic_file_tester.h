#ifndef MAGIC_FILE_TESTER_H
#define MAGIC_FILE_TESTER_H
// see -> http://man7.org/linux/man-pages/man3/libmagic.3.html
#include <magic.h>
#include <string>
#include <stdexcept>

class magic_file_tester
//Restricted wraper for libmagic
{
    magic_t magic_cookie;
public:
    magic_file_tester();

    magic_file_tester(unsigned magicFlags=MAGIC_MIME):magic_cookie(NULL)
    {
        open(magicFlags);
    }

    void open(unsigned magicFlags)
    {
        /* MAGIC_MIME tells magic to return a mime of the file,
           but you can specify different things	*/

        magic_cookie = magic_open(magicFlags);

        if (magic_cookie == NULL)
        {
                throw std::runtime_error("Unable to initialize magic library");
        }

        if (magic_load(magic_cookie, NULL) != 0)
        {
                std::string mesg=magic_error(magic_cookie);
                magic_close(magic_cookie);
                throw std::runtime_error(mesg);
        }

    }

    ~magic_file_tester()
    {
        if (magic_cookie != NULL)
            magic_close(magic_cookie);
    }

    const char* getFileInfo(const std::string& p)
    {
        return magic_file(magic_cookie, p.c_str() );
    }

};

#endif // MAGIC_FILE_TESTER_H
