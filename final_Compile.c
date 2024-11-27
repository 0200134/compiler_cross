#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_FILE_COUNT 10
#define MAX_FILENAME_LENGTH 256
#define MAX_COMMAND_LENGTH 2048

void error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

void compile_and_link_files(char *filenames[], int file_count, const char *output_executable, const char *output_assembly, const char *output_binary, const char *output_hex) {
    char command[MAX_COMMAND_LENGTH];

    // Compile source files
    snprintf(command, sizeof(command), "arm-none-eabi-gcc -c");
    for (int i = 0; i < file_count; i++) {
        strncat(command, " ", sizeof(command) - strlen(command) - 1);
        strncat(command, filenames[i], sizeof(command) - strlen(command) - 1);
    }
    strncat(command, " -specs=nosys.specs", sizeof(command) - strlen(command) - 1);
    printf("Compiling with command: %s\n", command);
    int result = system(command);
    if (result != 0) {
        error("Compilation failed. Please check your code.");
    }

    // Link object files to create executable
    snprintf(command, sizeof(command), "arm-none-eabi-gcc -o %s", output_executable);
    for (int i = 0; i < file_count; i++) {
        char object_file[MAX_FILENAME_LENGTH];
        snprintf(object_file, sizeof(object_file), "%.*s.o", (int)(strrchr(filenames[i], '.') - filenames[i]), filenames[i]);
        strncat(command, " ", sizeof(command) - strlen(command) - 1);
        strncat(command, object_file, sizeof(command) - strlen(command) - 1);
    }
    strncat(command, " -specs=nosys.specs", sizeof(command) - strlen(command) - 1);
    printf("Linking with command: %s\n", command);
    result = system(command);
    if (result != 0) {
        error("Linking failed. Please check your code.");
    }

    // Generate ARM assembly code
    snprintf(command, sizeof(command), "arm-none-eabi-gcc -S -o %s", output_assembly);
    for (int i = 0; i < file_count; i++) {
        char object_file[MAX_FILENAME_LENGTH];
        snprintf(object_file, sizeof(object_file), "%.*s.o", (int)(strrchr(filenames[i], '.') - filenames[i]), filenames[i]);
        strncat(command, " ", sizeof(command) - strlen(command) - 1);
        strncat(command, object_file, sizeof(command) - strlen(command) - 1);
    }
    printf("Generating ARM assembly with command: %s\n", command);
    result = system(command);
    if (result != 0) {
        error("Failed to generate ARM assembly code.");
    }

    // Generate binary file
    snprintf(command, sizeof(command), "arm-none-eabi-objcopy -O binary %s %s", output_executable, output_binary);
    printf("Generating binary file with command: %s\n", command);
    result = system(command);
    if (result != 0) {
        error("Failed to generate binary file.");
    }

    // Generate hex file
    snprintf(command, sizeof(command), "arm-none-eabi-objcopy -O ihex %s %s", output_executable, output_hex);
    printf("Generating hex file with command: %s\n", command);
    result = system(command);
    if (result != 0) {
        error("Failed to generate hex file.");
    }

    printf("Compilation and linking successful! Executable: %s\n", output_executable);
    printf("ARM assembly code generated: %s\n", output_assembly);
    printf("Binary file generated: %s\n", output_binary);
    printf("Hex file generated: %s\n", output_hex);
}

int main() {
    char *filenames[MAX_FILE_COUNT];
    int file_count = 0;
    char filename[MAX_FILENAME_LENGTH];

    printf("Enter your C source and header file names (type 'END' to finish):\n");

    while (file_count < MAX_FILE_COUNT) {
        if (scanf("%s", filename) != 1) {
            error("Error reading input");
        }

        if (strcmp(filename, "END") == 0) {
            break;
        }

        filenames[file_count] = strdup(filename);
        file_count++;
    }

    if (file_count == 0) {
        error("No files provided for compilation.");
    }

    const char *output_executable = "output_program";
    const char *output_assembly = "output_program.s";
    const char *output_binary = "output_program.bin";
    const char *output_hex = "output_program.hex";

    compile_and_link_files(filenames, file_count, output_executable, output_assembly, output_binary, output_hex);

    // Free allocated memory
    for (int i = 0; i < file_count; i++) {
        free(filenames[i]);
    }

    return 0;
}
