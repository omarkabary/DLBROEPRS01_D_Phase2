#include <stdio.h>
#include <string.h>

//Omar Abdelrahman
//UART Taschenrechner Prototype für Phase 2

int main(void) {
    double a, b;                 // Zwei Zahlen (mit Komma)
    char op;                     // Operator (+, -, *, /)
    char input[50];              // Eingabe als Text speichern
    FILE *logfile;               // Zeiger auf Log-Datei

    // Log-Datei öffnen (wird erstellt, falls sie nicht existiert)
    logfile = fopen("calc_log.txt", "a");
    if (!logfile) {
        printf("Fehler: Datei konnte nicht geöffnet werden.\n");
        return 1; // Beenden, falls Datei nicht geöffnet werden kann
    }

    // Endlosschleife – läuft, bis Benutzer "exit" eingibt
    while (1) {
        printf("Geben Sie eine einfache Rechnung ein (oder 'exit' zum Beenden): ");
        fgets(input, sizeof(input), stdin); // Ganze Zeile einlesen

        // Das letzte Zeichen '\n' (Enter) entfernen
        input[strcspn(input, "\n")] = 0;

        // Wenn Benutzer "exit" oder "quit" schreibt → Programm beenden
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            printf("Programm beendet.\n");
            break;
        }

        // Versuchen, Eingabe zu interpretieren (z. B. "12 + 5")
        if (sscanf(input, "%lf %c %lf", &a, &op, &b) != 3) {
            printf("Fehler: Bitte Format nutzen: <Zahl> <Operator> <Zahl>\n");
            // Fehlerfall ins Log schreiben
            fprintf(logfile, "IN : %s\nOUT: ERROR_FORMAT\n", input);
            fflush(logfile);
            continue; // Neue Eingabe anfordern
        }

        double result = 0.0; // Ergebniss der Berechnung
        int valid = 1; // Flag, ob Berechnung gültig war

        // Operator prüfen und Berechnung durchführen
        switch (op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (b == 0) {
                    printf("Fehler: Division durch Null\n");
                    // Spezfischen Fehler in Log schreibn
                    fprintf(logfile, "IN : %s\nOUT: ERROR_DIVISION_BY_NULL\n", input);
                    fflush(logfile);
                    valid = 0; // ungültig
                } else {
                    result = a / b;
                }
                break;
            default:
                printf("Unbekannter Operator: %c\n", op);
                // Unbekanter Operator in Log vermerken
                fprintf(logfile, "IN : %s\nOUT: ERROR_UNKNOWN_OPRATOR\n", input);
                fflush(logfile);
                valid = 0;
        }

        // Wenn Berechnung gültig war → Ergebnis ausgeben und speichern
        if (valid) {
            printf("Ergebnis = %.2f\n", result);

            // Ergebnis in Log-Datei schreiben
            fprintf(logfile, "IN : %s\nOUT: %.2f\n", input , result);
            fflush(logfile); // Sicherstellen, dass direkt geschrieben wird
        }
    }

    // Log-Datei schließen
    fclose(logfile);
    return 0;
}