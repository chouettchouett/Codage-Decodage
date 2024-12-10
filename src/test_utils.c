#include "test_utils.h"

void create_mock_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur à l'ouverture du fichier fictif");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s", content);
    fclose(file);
}

void cleanup_files(const char* files[], int count) {
    for (int i = 0; i < count; i++) {
        remove(files[i]);
    }
}
