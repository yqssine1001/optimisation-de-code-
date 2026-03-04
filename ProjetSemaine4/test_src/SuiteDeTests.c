#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include "CuTest.h"
#include "compare_file.h"
#include "word.h"
#include "dico.h"


/* ------------------------------------------------------------------ */
/* Tests unitaires pour compareWord                                   */
/* ------------------------------------------------------------------ */

// Permet de créer un mot_data_t à partir d'une chaîne de caractères
static mot_data_t *make_word(const char *mot) {
    mot_data_t *w = (mot_data_t *) malloc(sizeof(mot_data_t));
    strncpy(w->lemot, mot, maxSizeWord - 1);
    w->lemot[maxSizeWord - 1] = '\0';
    w->tete_liste = NULL;
    w->queue_liste = NULL;
    return w;
}

int compare_dico(dico *d1, dico *d2)
{
    if(!d1 && !d2)
        return 1;
    if(!d1 || !d2)
        return 0;
    if(d1->mot->lehash != d2->mot->lehash)
        return 0;
    return compare_dico(d1->fd, d2->fd) && compare_dico(d1->fg, d2->fg);
}

/* compareWord(NULL, w2) doit retourner 1 */
void test_compareWord_w1_null(CuTest *tc) {
    mot_data_t *w2 = make_word("bonjour");
    CuAssertIntEquals(tc, 1, compareWord(NULL, w2));
    free(w2);
}

/* compareWord(w1, NULL) doit retourner -1 */
void test_compareWord_w2_null(CuTest *tc) {
    mot_data_t *w1 = make_word("bonjour");
    CuAssertIntEquals(tc, -1, compareWord(w1, NULL));
    free(w1);
}

/* compareWord(NULL, NULL) : w1 est NULL => retourne 1 en premier */
void test_compareWord_both_null(CuTest *tc) {
    CuAssertIntEquals(tc, 1, compareWord(NULL, NULL));
}

/* Deux mots identiques => retourne 0 */
void test_compareWord_egal(CuTest *tc) {
    mot_data_t *w1 = make_word("chat");
    mot_data_t *w2 = make_word("chat");
    CuAssertIntEquals(tc, 0, compareWord(w1, w2));
    free(w1);
    free(w2);
}

/* w1 < w2 alphabetiquement ("abc" < "abd") => retourne -1 */
void test_compareWord_w1_inferieur(CuTest *tc) {
    mot_data_t *w1 = make_word("abc");
    mot_data_t *w2 = make_word("abd");
    CuAssertTrue(tc, compareWord(w1, w2) < 0);
    free(w1);
    free(w2);
}

/* w1 > w2 alphabetiquement ("abd" > "abc") => retourne 1 */
void test_compareWord_w1_superieur(CuTest *tc) {
    mot_data_t *w1 = make_word("abd");
    mot_data_t *w2 = make_word("abc");
    CuAssertTrue(tc, compareWord(w1, w2) > 0);
    free(w1);
    free(w2);
}

/* w1 est un prefixe de w2 ("ab" < "abc") => retourne -1 */
void test_compareWord_w1_prefixe_de_w2(CuTest *tc) {
    mot_data_t *w1 = make_word("ab");
    mot_data_t *w2 = make_word("abc");
    CuAssertTrue(tc, compareWord(w1, w2) < 0);
    free(w1);
    free(w2);
}

/* w2 est un prefixe de w1 ("abc" > "ab") => retourne 1 */
void test_compareWord_w2_prefixe_de_w1(CuTest *tc) {
    mot_data_t *w1 = make_word("abc");
    mot_data_t *w2 = make_word("ab");
    CuAssertTrue(tc, compareWord(w1, w2) > 0);
    free(w1);
    free(w2);
}

/* Casse 'A' < 'a' => retourne -1 */
void test_compareWord_casse(CuTest *tc) {
    mot_data_t *w1 = make_word("ABC");
    mot_data_t *w2 = make_word("abc");
    CuAssertTrue(tc, compareWord(w1, w2) < 0);
    free(w1);
    free(w2);
}

/* Mots d'une seule lettre differente "a" et "b" => retourne -1 */
void test_compareWord_un_caractere(CuTest *tc) {
    mot_data_t *w1 = make_word("a");
    mot_data_t *w2 = make_word("b");
    CuAssertTrue(tc, compareWord(w1, w2) < 0);
    free(w1);
    free(w2);
}

/* ------------------------------------------------------------------ */
/* Tests systèmes                                                     */
/* ------------------------------------------------------------------ */

/* Fonction générique : lance monDico dans un processus fils isolé sur 'filename' (L'isolation par fork() car évite que les fuites mémoire / corruptions d'un appel précédent de monDico affectent les tests suivants. )*/
static void run_systeme_test(CuTest *tc, const char *filename) {
    char input[256], expected[256];
    snprintf(input, sizeof(input), "tests_systeme/inputs/%s", filename);
    snprintf(expected, sizeof(expected), "tests_systeme/expected_outputs/%s", filename);

    pid_t pid = fork();
    CuAssert(tc, "fork() a echoue", pid >= 0);

    if (pid == 0) {
        char *argv[] = { "main", input, NULL };
        exit(monDico(2, argv));
    }

    int status;
    waitpid(pid, &status, 0);

    // Vérifie que monDico s'est terminé normalement avec code 0
    CuAssert(tc, "monDico : signal inattendu", WIFEXITED(status));
    CuAssertIntEquals(tc, 0, WEXITSTATUS(status));

    // Compare dictionnaires.txt avec la sortie attendue
    int diff = compare_file(DICORES, expected);
    CuAssertIntEquals(tc, 0, diff);
}


void test_systeme_plusieurs_espaces(CuTest *tc) {
    run_systeme_test(tc, "test_avec_plusieurs_espaces.txt");
}

void test_systeme_ponctuation(CuTest *tc) {
    run_systeme_test(tc, "test_ponctuation.txt");
}
void test_systeme_vide(CuTest *tc) {
    run_systeme_test(tc, "test_vide.txt");
}
void test_systeme_ordre(CuTest *tc) {
    run_systeme_test(tc, "test_ordre.txt");
}
void test_systeme_EOF(CuTest *tc) {
    run_systeme_test(tc, "test_EOF.txt");
}
void test_systeme_normal(CuTest *tc) {
    run_systeme_test(tc, "test_3mots_3lignes.txt");
}

int test_serealisation(char * path)
{
    FILE *f = fopen(path, "r");
    if(!f) 
    {
        perror(path);
        return -1;
    }
    int i;
    unsigned int* line = (unsigned int*) malloc(sizeof(int));
    unsigned int* colonne = (unsigned int*) malloc(sizeof(int));
    char* word = (char*) malloc(sizeof(char)*maxSizeWord);
    dico* dictionary = NULL;
    dico* copiedico = NULL; 
    mot_data_t **serialized_dico = (mot_data_t **)malloc(MaxSizeArray*sizeof(mot_data_t *));
    while((word = next_word(f,line,colonne))!=NULL) {
        addToDico(&dictionary,word,line,colonne);
    }
    for(i=0; i<MaxSizeArray; i++)
    serialized_dico[i] = NULL;
    serializeDico(dictionary,serialized_dico);
    for(i=0; i<MaxSizeArray; i++)
    if (serialized_dico[i] != NULL) {
      deserializeDico(&copiedico, serialized_dico[i]);
    }
    return compare_dico(dictionary, copiedico);
}

void test_ser_plusieurs_espaces(CuTest *tc) {
    CuAssertTrue(tc, test_serealisation("tests_systeme/inputs/test_avec_plusieurs_espaces.txt"));
}

void test_ser_ponctuation(CuTest *tc) {
    CuAssertTrue(tc, test_serealisation("tests_systeme/inputs/est_ponctuation.txt"));
}
void test_ser_vide(CuTest *tc) {
    CuAssertTrue(tc, test_serealisation("tests_systeme/inputs/test_vide.txt"));
}
void test_ser_ordre(CuTest *tc) {
    CuAssertTrue(tc, test_serealisation("tests_systeme/inputs/test_ordre.txt"));
}
void test_ser_EOF(CuTest *tc) {
    CuAssertTrue(tc, test_serealisation("tests_systeme/inputs/test_EOF.txt"));
}
void test_ser_normal(CuTest *tc) {
    CuAssertTrue(tc, test_serealisation("tests_systeme/inputs/test_3mots_3lignes.txt"));
}
/* ------------------------------------------------------------------ */
/* Suite de tests                                                     */
/* ------------------------------------------------------------------ */
CuSuite *MaTestSuite(void) {
    CuSuite *suite = CuSuiteNew();
    // Ajouter les tests unitaires pour compareWord
    SUITE_ADD_TEST(suite, test_compareWord_w1_null);
    SUITE_ADD_TEST(suite, test_compareWord_w2_null);
    SUITE_ADD_TEST(suite, test_compareWord_both_null);
    SUITE_ADD_TEST(suite, test_compareWord_egal);
    SUITE_ADD_TEST(suite, test_compareWord_w1_inferieur);
    SUITE_ADD_TEST(suite, test_compareWord_w1_superieur);
    SUITE_ADD_TEST(suite, test_compareWord_w1_prefixe_de_w2);
    SUITE_ADD_TEST(suite, test_compareWord_w2_prefixe_de_w1);
    SUITE_ADD_TEST(suite, test_compareWord_casse);
    SUITE_ADD_TEST(suite, test_compareWord_un_caractere);
    // Ajouter les tests système
    SUITE_ADD_TEST(suite, test_systeme_plusieurs_espaces);
    SUITE_ADD_TEST(suite, test_systeme_ponctuation);
    SUITE_ADD_TEST(suite, test_systeme_vide);
    SUITE_ADD_TEST(suite, test_systeme_ordre);
    SUITE_ADD_TEST(suite, test_systeme_EOF);
    SUITE_ADD_TEST(suite, test_systeme_normal);
    // Ajouter les test serealisation
    SUITE_ADD_TEST(suite, test_ser_plusieurs_espaces);
    SUITE_ADD_TEST(suite, test_ser_ponctuation);
    SUITE_ADD_TEST(suite, test_ser_vide);
    SUITE_ADD_TEST(suite, test_ser_ordre);
    SUITE_ADD_TEST(suite, test_ser_EOF);
    SUITE_ADD_TEST(suite, test_ser_normal);
    return suite;
}



