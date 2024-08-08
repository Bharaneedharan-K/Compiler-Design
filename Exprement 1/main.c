#include <stdio.h>
#include <ctype.h>
#include <string.h>

void main() {
    FILE *fi, *fo, *fop, *fk;
    int flag = 0, i = 1;
    char c, a[15], ch[15], file[20];

    printf("\nEnter the File Name: ");
    scanf("%s", file);

    if ((fi = fopen(file, "r")) == NULL) {
        perror("Error opening source file");
        return;
    }
    if ((fo = fopen("inter.c", "w")) == NULL) {
        perror("Error opening intermediate file");
        fclose(fi);
        return;
    }
    if ((fop = fopen("oper.c", "r")) == NULL) {
        perror("Error opening operators file");
        fclose(fi);
        fclose(fo);
        return;
    }
    if ((fk = fopen("key.c", "r")) == NULL) {
        perror("Error opening keywords file");
        fclose(fi);
        fclose(fo);
        fclose(fop);
        return;
    }

    while ((c = getc(fi)) != EOF) {
        if (isalpha(c) || isdigit(c) || c == '[' || c == ']' || c == '.') {
            fputc(c, fo);
        } else {
            if (c == '\n') {
                fprintf(fo, "\t$\t");
            } else {
                fprintf(fo, "\t%c\t", c);
            }
        }
    }

    fclose(fi);
    fclose(fo);

    if ((fi = fopen("inter.c", "r")) == NULL) {
        perror("Error opening intermediate file");
        fclose(fop);
        fclose(fk);
        return;
    }

    printf("\nLexical Analysis:\n");

    while (fscanf(fi, "%s", a) != EOF) {
        printf("\nLine: %d\n", i++);

        if (strcmp(a, "$") == 0) {
            continue;
        }

        rewind(fop);
        while (fscanf(fop, "%s", ch) != EOF) {
            if (strcmp(ch, a) == 0) {
                printf("\t\t%s\t:\tOperator\n", a);
                flag = 1;
                break;
            }
        }

        if (flag == 0) {
            rewind(fk);
            while (fscanf(fk, "%s", ch) != EOF) {
                if (strcmp(ch, a) == 0) {
                    printf("\t\t%s\t:\tKeyword\n", a);
                    flag = 1;
                    break;
                }
            }
        }

        if (flag == 0) {
            if (isdigit(a[0])) {
                printf("\t\t%s\t:\tConstant\n", a);
            } else {
                printf("\t\t%s\t:\tIdentifier\n", a);
            }
        }

        flag = 0;
    }

    fclose(fi);
    fclose(fop);
    fclose(fk);
}
