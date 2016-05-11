#include <sys/ioctl.h>
#include <sys/socket.h>
#include <string.h>

struct ifmediareq {
        char    ifm_name[16];             /* if name, e.g. "en0" */
        int     ifm_current;                    /* current media options */
        int     ifm_mask;                       /* don't care mask */
        int     ifm_status;                     /* media status */
        int     ifm_active;                     /* active options */
        int     ifm_count;                      /* # entries in ifm_ulist
                                                   array */
        int     *ifm_ulist;                     /* media words */
};


void main() {
        struct ifmediareq *ifmr;
        int fd;
        strlcpy(ifmr->ifm_name, "urtwn0", sizeof(ifmr->ifm_name));
        fd = socket(AF_UNSPEC,SOCK_RAW, 0);
        for (;;)
                ioctl(fd, SIOCGIFMEDIA, &ifmr);
}
