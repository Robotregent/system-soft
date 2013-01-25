#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "copy_file.h"

static bool copy_time_stamp(const char * target_path, struct stat stat_buf){
    struct timespec times[2];
    times[0] = stat_buf.st_atim;
    times[1] = stat_buf.st_mtim;
    if(utimensat(AT_FDCWD, target_path, times, 0) < 0){
        return false;
    }
    return true;
}

static bool copy_file_content(const char * src, const char * dst){
    FILE *src_file = fopen(src,"r");
    FILE *dst_file = fopen(dst,"w");
    bool ret = false;
    char c;
    if (src_file && dst_file){
        do {
            c = fgetc(src_file);
            if (!feof(src_file))
                fputc(c,dst_file);
        } while (c != EOF);
        fclose(src_file);
        fclose(dst_file);
        ret = true;
    }
    return ret;
}

static bool copy_permissions(const char * dst, struct stat stat_buf){
    bool ret = false;
    mode_t permissions = stat_buf.st_mode;
    if (chmod(dst, permissions) == 0)
        ret = true;
    return ret;
}

bool copy_file(const char * src, const char * dst){
    bool ret = false, content = false, 
         timestamp = false, permissions = false;
    if (src && dst){
        struct stat stat_buf;
        lstat(src, &stat_buf);
        content = copy_file_content(src, dst); 
        if (content)
            timestamp = copy_time_stamp(dst, stat_buf);
        if (content && timestamp)
            permissions = copy_permissions(dst, stat_buf);
        ret = content && timestamp && permissions;
    }
    return ret;
}
