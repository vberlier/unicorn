#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <unistd.h>
#include "narwhal.h"


#define TEMP_DIR_TEMPLATE "/tmp/test_tmpdirXXXXXX"


// File tree walk callback that removes every file and directory.

static int remove_item(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    (void)sb;
    (void)typeflag;
    (void)ftwbuf;

    return remove(fpath);
}


TEST_FIXTURE(tmpdir, struct { char path[sizeof(TEMP_DIR_TEMPLATE)]; char original_path[512]; })
{
    // Get the current working directory
    getcwd(tmpdir->original_path, sizeof (tmpdir->original_path));

    // Define the template path for the temporary directory
    strncpy(tmpdir->path, TEMP_DIR_TEMPLATE, sizeof (tmpdir->path));

    // Create the temporary directory and change the working directory
    mkdtemp(tmpdir->path);
    chdir(tmpdir->path);

    CLEANUP_FIXTURE(tmpdir)
    {
        // Change the working directory back to the original directory
        chdir(tmpdir->original_path);

        // Remove everything inside of the temporary directory
        nftw(tmpdir->path, remove_item, 64, FTW_DEPTH | FTW_PHYS);
    }
}
