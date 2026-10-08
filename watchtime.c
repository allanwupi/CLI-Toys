#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int add_time(double *h0, double *m0, double *s0, char *tstr, double speed)
{
    double dt[3] = {0.0, 0.0, 0.0};
    char buffer[9];
    strncpy(buffer, tstr, 9);

    size_t count = 0;
    size_t length = strlen(tstr);
    for (size_t i = 0; i < length; i++) {
        if (tstr[i] == ':') count++;
    }
    while (count < 2) {
        memmove(buffer + 3, buffer, length + 1);  // Includes '\0'
        memcpy(buffer, "00:", 3);
        length+=3;
        count++;
    }
    char *token = strtok(buffer, ":");
    for (int i = 2; i > -1; i--) {
        dt[i] = atof(token);
        token = strtok(NULL, ":");
    }
    double stot = *h0 * 3600.0 + *m0 * 60.0 + *s0
                         + (dt[2] * 3600.0 + dt[1] * 60.0 + dt[0]) / speed;
    if (stot >= 60.0 * 3600.0) {
        printf("exceeded maximum duration: total seconds=%.1f\n", stot);
        return EXIT_FAILURE;
    }
    *h0 = floor(stot / 3600.0);
    stot = fmod(stot, 3600.0);
    *m0 = floor(stot / 60.0);
    *s0 = fmod(stot, 60.0);
    return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
    double speed = 1.0;
    double h = 0.0, m = 0.0, s = 0.0;
    for (int i = 1; i < argc; i++) {
        size_t length = strlen(argv[i]);
        if (length == 0) {
            return EXIT_FAILURE;
        }
        int flag;
        if (argv[i][length-1] == 'x') {
            argv[i][length-1] = '\0';
            speed = atof(argv[i]);
            continue;
        }
        flag = add_time(&h, &m, &s, argv[i], speed);
        if (flag == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }
    if (h > 0) {
        printf("%d:%02d:%02d\n", (int)h, (int)m, (int)s);
        return EXIT_SUCCESS;
    }
    printf("%02d:%02d\n", (int)m, (int)s);
    return EXIT_SUCCESS;
}
