#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[20];
    float grade;
} Student;

void readStudent(const char *filename, Student students[], int count) { // чтение файла
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    fread(students, sizeof(Student), count, file);
    fclose(file);
}

void writeStudents(const char *filename, Student students[], int count) { // запись файла
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    fwrite(students, sizeof(Student), count, file);
    fclose(file);
}

void rewrite(const char *filename, int(*is_correct)(float)) { //перезапись с сортировкой по указанной функции
    FILE *src = fopen(filename, "rb");
    FILE *tmp = fopen("rem.txt", "wb");

    if (src == NULL || tmp == NULL) {
        printf("Error opening file\n");
        return;
    }
    Student student;

    while (fread(&student, sizeof(Student), 1, src)) {
        if (is_correct(student.grade)) {
            fwrite(&student, sizeof(Student), 1, tmp);
        }
    }
    fclose(src);
    fclose(tmp);
    remove(filename);
    rename("rem.txt", filename);
}
int is_good_student(float i) {
    return i>=4.0;
}

int main(void) {

    Student student[] = {
        {1, "Mark", 3.5},
        {2, "Anna", 5.0},
        {3, "Sofia", 4.8},
        {4, "Connor", 4.0},
        {5, "Lera", 4.7},
    };

    writeStudents("students.txt", student, 5);

    rewrite("students.txt",
                  is_good_student);

    Student *result = malloc(4 * sizeof(Student));

    if (result == NULL) {
        printf("Memory allocation error\n");
        return 1;
    }

    readStudent("students.txt",
                 result,
                 4);

    for (int i = 0; i < 4; i++) {

        printf("Student %d: %.1f %s\n",
               result[i].id,
               result[i].grade,
               result[i].name);
    }

    free(result);
    return 0;
}