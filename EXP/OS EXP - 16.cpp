
#include <stdio.h>
#include <string.h>

struct Employee {
    int id;
    char name[50];
    float salary;
};

int main() {
    struct Employee e;
    FILE *fp;
    char path[260];

    printf("Enter the full file path to save employee.dat:\n");
    printf("Example: D:\\OS Lab\\employee.dat\n");
    printf("File path: ");

    if (fgets(path, sizeof(path), stdin) == NULL) {
        printf("Invalid input!\n");
        return 1;
    }

    path[strcspn(path, "\n")] = '\0';

    if (strlen(path) == 0) {
        printf("File path cannot be empty!\n");
        return 1;
    }

    printf("\nEnter Employee ID: ");
    if (scanf("%d", &e.id) != 1) {
        printf("Invalid employee ID!\n");
        return 1;
    }

    printf("Enter Employee Name: ");
    scanf("%49s", e.name);

    printf("Enter Employee Salary: ");
    if (scanf("%f", &e.salary) != 1) {
        printf("Invalid salary!\n");
        return 1;
    }

    fp = fopen(path, "w");

    if (fp == NULL) {
        printf("\nCannot create file at the given location.\n");
        printf("Check that the folder exists and the path is valid.\n");
        return 1;
    }

    fprintf(fp, "%d %s %.2f\n",
            e.id, e.name, e.salary);
    fclose(fp);

    printf("\nEmployee details saved successfully!\n");
    printf("Saved location: %s\n", path);

    fp = fopen(path, "r");

    if (fp == NULL) {
        printf("Unable to read the file!\n");
        return 1;
    }

    printf("\n--- Employee Details ---\n");

    if (fscanf(fp, "%d %49s %f",
               &e.id, e.name, &e.salary) == 3) {
        printf("Employee ID: %d\n", e.id);
        printf("Employee Name: %s\n", e.name);
        printf("Employee Salary: %.2f\n", e.salary);
    } else {
        printf("Unable to read employee details.\n");
    }

    fclose(fp);

    return 0;
}
