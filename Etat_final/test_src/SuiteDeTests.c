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
/*tests unitaires pour fonctions de word*/
/* ------------------------------------------------------------------ */
/*      Test unitaire pour nextWord                                   */
/* ------------------------------------------------------------------ */
void test_nextWord(CuTest *tc){
    pid_t pid = fork();
    CuAssert(tc, "fork() a echoue", pid >= 0);
    if(pid==0){
        FILE *f = fopen("./tests_unitaires/inputs/test_texte","r");
        CuAssertPtrNotNull(tc, f);
        const char *expected[] = {"prog", "sr", "algo", "moca"};
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

int compare_displayDico(char *input, char *output, char *expected_output)
{
    FILE *f = fopen(input, "r");
    unsigned int line = 0;
    unsigned int colonne = 0;
    char *word = NULL;
    dico *dictionary = NULL;
    while ((word = next_word(f, &line, &colonne)) != NULL) {
      addToDico(&dictionary, word, &line, &colonne);
      free(word);
    }
    displayDicoInternal(dictionary, input, expected_output);
    free(dictionary);
    return compare_file(expected_output, output);
}

void test_compareDico_un_mot(CuTest *tc)
{
    CuAssertIntEquals(tc, 0, compare_displayDico("./tests_unitaires/inputs/mot_simple.txt",
        "./tests_unitaires/outputs/mot_simple.txt",  "./tests_unitaires/outputs_expected/mot_simple.txt"));
}

void test_compareDico_null(CuTest *tc)
{
    CuAssertIntEquals(tc, 0, compare_displayDico("./tests_unitaires/inputs/test_vide",
        "./tests_unitaires/outputs/displayDico_null.txt",  "./tests_unitaires/outputs_expected/displayDico_null.txt"));
}

void test_compareDico_texte(CuTest *tc)
{
    CuAssertIntEquals(tc, 0, compare_displayDico("./tests_unitaires/inputs/test_texte",
        "./tests_unitaires/outputs/displayDico_texte.txt",  "./tests_unitaires/outputs_expected/displayDico_texte.txt"));
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
/*      Test unitaire pour displayWord                                */
/* ------------------------------------------------------------------ */
void test_displayWord(CuTest *tc){
    mot_data_t *w1 = make_word("titi");
    mot_data_t *w2 = make_word("toto");
    incWord(w1,1,14);
    incWord(w1,2,9);
    incWord(w1,3,42);
    incWord(w2,1,7);
    incWord(w2,1,16);
    incWord(w2,2,1);
    char *input = "./tests_unitaires/outputs/displayWord.txt";
    char *expected= "./tests_unitaires/outputs_expected/displayWord.txt";
    FILE *f = fopen(input,"w");
    CuAssertPtrNotNull(tc,f);
    displayWord(w1,f);
    displayWord(w2,f);
    fclose(f);
    free(w1);
    free(w2);
    int diff = compare_file(input, expected);
    CuAssertIntEquals(tc, 0, diff);
}
void test_displayWord_null(CuTest *tc){
    mot_data_t *w1 = NULL;
    char *input = "./tests_unitaires/outputs/displayWord_null.txt";
    char *expected= "./tests_unitaires/outputs_expected/displayWord_null.txt";
    FILE *f = fopen(input,"w");
    CuAssertPtrNotNull(tc,f);
    displayWord(w1,f);
    fclose(f);
    free(w1);
    int diff = compare_file(input, expected);
    CuAssertIntEquals(tc, 0, diff);
}
void test_displayWord_listeVide(CuTest *tc){
    mot_data_t *w1 = make_word("titi");
    char *input = "./tests_unitaires/outputs/displayWord_listeVide.txt";
    char *expected= "./tests_unitaires/outputs_expected/displayWord_listeVide.txt";
    FILE *f = fopen(input,"w");
    CuAssertPtrNotNull(tc,f);
    displayWord(w1,f);
    fclose(f);
    free(w1);
    int diff = compare_file(input, expected);
    CuAssertIntEquals(tc, 0, diff);
}
/*tests unitaires pour fonctions de dico*/
mot_t *cree_mot_t(mot_data_t *data){
    mot_t *w = (mot_t *)malloc(sizeof(mot_t));
    w->data = *data;
    return w;
}
void test_insertDico(CuTest *tc){
    dico *d = NULL;
    CuAssertPtrEquals(tc,NULL,d);
    mot_data_t *w1 = make_word("barbe");
    incWord(w1,1,14);
     mot_data_t *w12 = make_word("barbe");
    incWord(w12,2,9);
    mot_data_t *w2 = make_word("citron");
    incWord(w2,1,7);
    mot_data_t *w3 = make_word("abeille");
    incWord(w3,3,14);

    mot_t *m1 = cree_mot_t(w1);
    mot_t *m12 = cree_mot_t(w12);
    mot_t *m2 = cree_mot_t(w2);
    mot_t *m3 = cree_mot_t(w3);

    insertDico(&d,m1);
    CuAssertPtrNotNull(tc,d);
    CuAssertPtrEquals(tc,m1,d->mot);
    insertDico(&d,m12);
    CuAssertIntEquals(tc,2,d->mot->data.queue_liste->line);
    CuAssertIntEquals(tc,9,d->mot->data.queue_liste->colonne);
    insertDico(&d,m2);
    CuAssertPtrEquals(tc,m2,d->fd->mot);
    CuAssertPtrNotNull(tc,d->fd->mot->data.tete_liste);
    insertDico(&d,m3);
    CuAssertPtrEquals(tc,m3,d->fg->mot);
    CuAssertPtrNotNull(tc,d->fg->mot->data.tete_liste);
    free(w1);
    free(w2);
    free(w3);
    free(m1);
    free(m2);
    free(m3);
    free(d);
}
void test_addToDico(CuTest *tc){
    dico *d = NULL;
    CuAssertPtrEquals(tc,NULL,d);
    unsigned int l=1,c=4;
    addToDico(&d,"barbe",&l,&c);
    l=2,c=9;
    addToDico(&d,"barbe",&l,&c);
    CuAssertStrEquals(tc,"barbe",d->mot->data.lemot);
    CuAssertIntEquals(tc,2,d->mot->data.queue_liste->line);
    CuAssertIntEquals(tc,9,d->mot->data.queue_liste->colonne);
    l=1,c=7;
    addToDico(&d,"citron",&l,&c);
    CuAssertStrEquals(tc,"citron",d->fd->mot->data.lemot);
    CuAssertPtrNotNull(tc,d->fd->mot->data.tete_liste);
    l=3,c=14;
    addToDico(&d,"abeille",&l,&c);
    CuAssertStrEquals(tc,"abeille",d->fg->mot->data.lemot);
    CuAssertPtrNotNull(tc,d->fg->mot->data.tete_liste);
    free(d);
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
    SUITE_ADD_TEST(suite,  test_displayWord);
    SUITE_ADD_TEST(suite,  test_displayWord_null);
    SUITE_ADD_TEST(suite,  test_displayWord_listeVide);

    SUITE_ADD_TEST(suite,  test_insertDico);
    SUITE_ADD_TEST(suite,  test_addToDico);

    // Ajouter test unitaire pour displayDico
    SUITE_ADD_TEST(suite, test_compareDico_un_mot);
    SUITE_ADD_TEST(suite, test_compareDico_null);
    SUITE_ADD_TEST(suite, test_compareDico_texte);

    // Ajouter les tests système
    SUITE_ADD_TEST(suite, test_systeme_plusieurs_espaces);
    SUITE_ADD_TEST(suite, test_systeme_ponctuation);
    SUITE_ADD_TEST(suite, test_systeme_vide);
    SUITE_ADD_TEST(suite, test_systeme_ordre);
    SUITE_ADD_TEST(suite, test_systeme_EOF);
    SUITE_ADD_TEST(suite, test_systeme_normal);
    return suite;
}
