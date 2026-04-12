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
/*      Test unitaire pour nextWord                                   */
/* ------------------------------------------------------------------ */
void test_nextWord(CuTest *tc){
    pid_t pid = fork();
    CuAssert(tc, "fork() a echoue", pid >= 0);
    if(pid==0){
        FILE *f = fopen("./tests_unitaires/test_texte","r");
        CuAssertPtrNotNull(tc, f);
        const char *expected[] = {"prog", "sr", "algo", "lambda"};
        unsigned int ligne = 0;
        unsigned int colonne = 0;

        for(int i=0;i<4;i++){
            char *mot = next_word(f,&ligne,&colonne);
            if (!mot || strcmp(mot, expected[i]) != 0) {
                exit(i + 1);
            }
            free(mot);
        }
        if (next_word(f, &ligne, &colonne) != NULL){
            fclose(f);
            exit(99);
        }
        fclose(f);
        exit(0);
        
    }
    else{
        int status;
        waitpid(pid, &status, 0);
        CuAssert(tc, "Le processus fils a crashé", WIFEXITED(status));
        CuAssertIntEquals(tc, 0, WEXITSTATUS(status));
    }
}
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
/*      Test unitaire pour incWord                                    */
/* ------------------------------------------------------------------ */
void test_incWord(CuTest *tc){
    mot_data_t *w = make_word("toto");
    CuAssertPtrEquals(tc,NULL,w->tete_liste);
    CuAssertPtrEquals(tc,NULL,w->queue_liste);

    incWord(w,1,14);
    CuAssertPtrNotNull(tc,w->tete_liste);
    CuAssertPtrEquals(tc,w->queue_liste,w->tete_liste); //verifie si queue et tete pointent vers le meme emplacement
    CuAssertIntEquals(tc,1,w->tete_liste->line);
    CuAssertIntEquals(tc,14,w->tete_liste->colonne);

    incWord(w,2,9);
    CuAssertPtrNotNull(tc,w->queue_liste);
    CuAssertIntEquals(tc,2,w->queue_liste->line);
    CuAssertIntEquals(tc,9,w->queue_liste->colonne);
    CuAssertPtrEquals(tc,NULL,w->queue_liste->next);
    free(w);
}
/* ------------------------------------------------------------------ */
/* Tests systèmes                                                     */
/* ------------------------------------------------------------------ */

/* Fonction générique : lance monDico dans un processus fils isolé sur 'filename' (L'isolation par fork() car évite que les fuites mémoire / corruptions d'un appel précédent de monDico affectent les tests suivants. )*/
static void run_systeme_test(CuTest *tc, const char *filename) {
    char input[256], expected[256];
    snprintf(input, sizeof(input), "./tests_systeme/inputs/%s", filename);
    snprintf(expected, sizeof(expected), "./tests_systeme/expected_outputs/%s", filename);

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

/* ------------------------------------------------------------------ */
/* Suite de tests                                                     */
/* ------------------------------------------------------------------ */
CuSuite *MaTestSuite(void) {
    CuSuite *suite = CuSuiteNew();
    //Ajouter test unitaire pour next_word
    SUITE_ADD_TEST(suite,test_nextWord);
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
    //Ajouter test unitaire pour incWord
    SUITE_ADD_TEST(suite,  test_incWord);
    // Ajouter les tests système
    SUITE_ADD_TEST(suite, test_systeme_plusieurs_espaces);
    SUITE_ADD_TEST(suite, test_systeme_ponctuation);
    SUITE_ADD_TEST(suite, test_systeme_vide);
    SUITE_ADD_TEST(suite, test_systeme_ordre);
    SUITE_ADD_TEST(suite, test_systeme_EOF);
    SUITE_ADD_TEST(suite, test_systeme_normal);
    return suite;
}



