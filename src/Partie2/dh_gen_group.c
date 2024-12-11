#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../utils/utils.h"
#include "dh_gen_group.h"
#include "dh_prime.h"


void dh_gen_group(char *file_name) {
        FILE *output_file = create_file(file_name, true, TMP);
        
        srand(time(NULL));
        generate_shared_key(MIN_PRIME, MAX_PRIME, output_file);

        if (fclose(output_file) != 0) {
                perror("Erreur : fclose\n");
                exit(3);
        }
}
