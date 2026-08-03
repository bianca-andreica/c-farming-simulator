# Proiect C cu Raylib și MinGW

Acesta este un proiect C pentru dezvoltarea unui joc 2D, folosind **Raylib** pentru grafică și **MinGW** ca compilator pe platforma Windows. Urmează pașii de mai jos pentru a configura mediul de dezvoltare și a rula proiectul.

## Prerechizite

Pentru a compila și rula acest proiect, vei avea nevoie de următoarele software-uri instalate pe sistemul tău:

- **MinGW** (pentru compilarea codului C)
- **Raylib** (biblioteca pentru grafică)
- **CMake** (pentru configurarea proiectului)

### 1. Instalarea MinGW

MinGW este un compilator pentru limbajul C care rulează pe Windows. Urmează acești pași pentru a-l instala:

1. Mergi pe [pagina oficială MinGW](https://sourceforge.net/projects/mingw/).
2. Descarcă și instalează MinGW folosind **mingw-get**.
3. După instalare, adaugă calea `bin` a MinGW în variabila de mediu `PATH`:
   - De obicei, calea este `C:\MinGW\bin`.
4. Verifică instalarea MinGW din linia de comandă (CMD) rulând:
   ```bash
   gcc --version
