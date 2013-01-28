#include <dirent.h>
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "up_dir.h"
#include "dirlib.h"
#include "copy_file.h"
static int dir_filter(const struct dirent *dir){
    int ret = 0, self, parent;
    self    = strcmp(dir->d_name, ".\0");
    parent  = strcmp(dir->d_name, "..\0"); 
#ifdef _DIRENT_HAVE_D_TYPE
    if ((dir->d_type == DT_REG || dir->d_type == DT_DIR)
            && (self != 0) && (parent != 0))
        ret = 1;
#else
    if ((self != 0) && (parent != 0))
        ret = 1;
#endif
    return ret;
}

void update_dir(const char * src_path, const char * dst_path, int rec_bucket, bool verbose){
    struct dirent **src_list = 0;
    struct dirent **dst_list = 0;
    int count_scr, count_dst, delta, i, j;
    char *path_src_file=0, *path_dst_file=0;
    count_scr = scandir(src_path, &src_list, dir_filter, alphasort);
    if (count_scr<0){
        printf("Try to open %s, but:\n",src_path);
        printf("ERROR: %s\n", strerror(errno));
        exit(2);
    }
    count_dst = scandir(dst_path, &dst_list, dir_filter, alphasort);
    if (count_dst <0){
        printf("Try to open %s, but:\n",dst_path);
        printf("ERROR: %s\n", strerror(errno));
        exit(2);
    }
    struct stat stat_src, stat_dst;

    for (i=0,j=0; i<count_scr; i++){
        path_src_file = create_path(src_path, src_list[i]->d_name);
        lstat(path_src_file, &stat_src);
#ifndef _DIRENT_HAVE_D_TYPE
	    if (!(S_ISREG(stat_src.st_mode)||S_ISDIR(stat_src.st_mode)))
	        break;
#endif
        if (j+1 > count_dst)
            delta = -1;
        else
            delta = strcmp(src_list[i]->d_name, dst_list[j]->d_name);

        if (delta < 0){                         // src not in dst. copy
            path_dst_file = create_path(dst_path, src_list[i]->d_name);

            if (S_ISREG(stat_src.st_mode)){ // copy file
                copy_file(path_src_file, path_dst_file); 
                if (verbose)
                    printf("Copy\t%s\t->\t%s\n",path_src_file,path_dst_file);
            }
            else{                           // create and copy subfolder
                if (0 == mkdir(path_dst_file, stat_src.st_mode)){
                    if (rec_bucket > 1)
                        update_dir(path_src_file, path_dst_file, rec_bucket - 1, verbose);
                    else {
                        if (verbose)
                            printf("Reached recursion limit\n");
                    }
                }
            }
            free(src_list[i]);
        }
        else if (delta > 0){
            j++;                                // dst not in src. skip
            i--;
            path_dst_file = 0;
        }
        else if (delta == 0){                   // src existiert in dst
            path_dst_file = create_path(dst_path, dst_list[j]->d_name);

            if (S_ISDIR(stat_src.st_mode)){ // subfolder 
                if (rec_bucket > 1)
                    update_dir(path_src_file, path_dst_file, rec_bucket - 1, verbose);
                else {
                    if (verbose)
                        printf("Reached recursion limit\n");
                }
            }
            else {                              // copy newer file
                lstat(path_dst_file, &stat_dst);
                if (stat_dst.st_mtim.tv_sec < stat_src.st_mtim.tv_sec){
                    copy_file(path_src_file, path_dst_file); 
                    if (verbose)
                        printf("Update\t%s\t->\t%s\n",path_src_file,path_dst_file);
                }                
            }

            free(src_list[i]);
            free(dst_list[j]);
       
            j++;
        }
        if (path_src_file)
            free(path_src_file);
        if (path_dst_file)
            free(path_dst_file);
    }
    if (dst_list)
        free(dst_list);
    if (src_list)
        free(src_list);

}
