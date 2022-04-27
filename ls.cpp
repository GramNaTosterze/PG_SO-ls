//sort dirent stat
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <list>
    bool l = false,R = false;

char* arguments(int argc, char* argv[], bool &l, bool &R) {
    int i = 1;
    for(i; i < argc; i++) {
        if(argv[i][0] != '-')
            break;
        else {
            switch(argv[i][1]) {
                case 'R': {
                    R = true;
                    break;
                }
                case 'l': {
                    l = true;
                    break;
                }
            }
        }
    }
    if(argc > i)
        return argv[i];
    else
        return ".";
}
bool check_dir(char* path) {
    if((path[0] == '.' && path[1] == '\0') || (path[1] == '.' && path[2] == '\0'))
        return false;

    struct stat s;
    if( stat(path,&s) == 0 ) {
        if( s.st_mode & S_IFDIR )
        {
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void _N(char* path, std::list<std::string>& sDir) {
    DIR *pDIR;
    struct dirent *pDirEnt;
    pDIR = opendir(path);
    if ( pDIR == NULL ) {
        fprintf( stderr, "%s %d: opendir() failed (%s)\n",
        __FILE__, __LINE__, strerror( errno ));
        exit( -1 );
    }
    pDirEnt = readdir( pDIR );
    while ( pDirEnt != NULL ) {
        sDir.push_back(std::string(pDirEnt->d_name));
        pDirEnt = readdir( pDIR );
    }
    closedir( pDIR );
    sDir.sort();
}
void _R(char* prev_path, char* path, std::list<std::list<std::string>> &sdir) {
    std::list<std::string> LS;
    sdir.push_back(LS);
    DIR *pDIR;
    struct dirent *pDirEnt;
    pDIR = opendir(path);
    if ( pDIR == NULL ) {
        fprintf( stderr, "%s %d: opendir() failed (%s)\n",
        __FILE__, __LINE__, strerror( errno ));
        exit( -1 );
    }
    pDirEnt = readdir( pDIR );
    while ( pDirEnt != NULL ) {
        if(check_dir(pDirEnt->d_name)) {
            char new_path[160];
            sprintf(new_path,"%s/%s/",path,pDirEnt->d_name);
            _R(new_path,new_path,sdir);
        }
        else
            LS.push_back(std::string(pDirEnt->d_name));
        pDirEnt = readdir( pDIR );
    }
    closedir( pDIR );
    //LS.sort();
    std::string s = std::string(prev_path)+std::string(pDirEnt->d_name)+":\n";
    LS.push_front(s);
}
void ls(char* path) {
    if(R) {
        std::list<std::list<std::string>> sDir;
        _R("",path,sDir);
        for(std::list<std::string> stringList : sDir)
            for(const std::string& str : stringList)
                printf("%s   ",str.c_str());
    }
    else {
        std::list<std::string> sDir;
        _N(path,sDir);
        for(const std::string& str : sDir)
            printf("%s   ",str.c_str());
    }
    printf("\n");
}

int main( int argc, char *argv[] ) {
    
    char* path = arguments(argc,argv,l,R);
    ls(path);

    return 0;
}