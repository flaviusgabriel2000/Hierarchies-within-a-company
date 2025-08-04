#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"

#define MAX_NO_EMPLOYEES 100

void get_node(Tree tree, char *manager_name, Tree *node){
    if (tree == NULL){
        return;
    }
    if (!strcmp(tree->name, manager_name)){
        *node = tree;
        return;
    }
    if (tree->team != NULL){
        for (int i = 0; i < tree->direct_employees_no; i++){
            get_node(tree->team[i], manager_name, node);
        }
    }
}

int comparare(const void* p1, const void* p2){
    TreeNode **name1 = (TreeNode **)p1;
    TreeNode **name2 = (TreeNode **)p2;
    return strcmp((*name1)->name, (*name2)->name);
}

/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, 
           atunci employee_name e primul om din ierarhie.
 */
Tree hire(Tree tree, char *employee_name, char *manager_name){
    if (tree == NULL && manager_name == NULL){
        tree = (Tree)malloc(sizeof(TreeNode));
        tree->name = employee_name;
        tree->direct_employees_no = 0;
        tree->manager = NULL;
        tree->team = NULL;

        return tree;
    }
    Tree t = NULL;
    get_node(tree, manager_name, &t);     


    t->direct_employees_no++;
    if (t->team == NULL){
        t->team =(TreeNode **)calloc(MAX_NO_EMPLOYEES, sizeof(TreeNode *));
    }

    t->team[t->direct_employees_no - 1] = (TreeNode *)calloc(1, sizeof(TreeNode));
    t->team[t->direct_employees_no - 1]->name = employee_name;
    t->team[t->direct_employees_no - 1]->manager = t;

    qsort(t->team, t->direct_employees_no, sizeof(TreeNode *), comparare);

    return tree;
}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire(Tree tree, char *employee_name) {
    Tree t = NULL;
    int index;
    get_node(tree, employee_name, &t);

    //daca nu exista angajatul sau daca este primul nod din ierarhie
    if (t == NULL || t->manager == NULL){
        return tree;
    }

    for (int i = 0; i < t->manager->direct_employees_no; i++){
        if (t->manager->team[i] == t){
            index = i;
        }
    }
    int nr_angajati = t->direct_employees_no;
    int nr_angajati_manager = t->manager->direct_employees_no;
    t->manager->direct_employees_no += nr_angajati - 1;

    /*daca nodul de sters are un singur angajat,
    facem legatura de la managerul nodului de sters la
    angajatul nodului de sters*/
    if (nr_angajati == 1){
        t->manager->team[index] = t->team[0];
        t->team[0]->manager = t->manager;
        qsort(t->manager->team, t->manager->direct_employees_no, sizeof(TreeNode *), comparare);
        free(t->team);
        free(t);
        return tree;
    }

    /*daca nodul de sters nu are niciun angajat,
    atunci fiecare nod din dreapta sa va fi mutat la stanga cu o unitate*/
    if (nr_angajati == 0){
        for (int i = index; i < t->manager->direct_employees_no; i++){
            t->manager->team[i] = t->manager->team[i + 1];
        }
        qsort(t->manager->team, t->manager->direct_employees_no, sizeof(TreeNode *), comparare);
        free(t);
        return tree;
    }
    
    /* daca nodul de sters are 'n' angajati, atunci nodurile din
    dreapta sa vor fi mutate cu n - 1 unitati la dreapta*/
    int i = t->manager->direct_employees_no - 1, j = 0;
    while (j < nr_angajati_manager - 1 - index){
        t->manager->team[i] = t->manager->team[i - (nr_angajati - 1)];
        i--;
        j++;
    }

    i = 0;

    //managerul va prelua noii angajati
    while(i < nr_angajati){
        t->manager->team[index] = t->team[i];
        t->team[i]->manager = t->manager;
        index++;
        i++;
    }
    qsort(t->manager->team, t->manager->direct_employees_no, sizeof(TreeNode *), comparare);

    free(t->team);
    free(t);
    
    return tree;
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */
Tree promote(Tree tree, char *employee_name) {
    Tree t = NULL;
    int index;
    get_node(tree, employee_name, &t);

    if (t ==  NULL || t->manager == NULL || t->manager->manager == NULL){
        return tree;
    }

    for (int i = 0; i < t->manager->direct_employees_no; i++){
        if (t->manager->team[i] == t){
            index = i;
        }
    }

    int nr_angajati = t->direct_employees_no;
    int nr_angajati_manager = t->manager->direct_employees_no;
    t->manager->direct_employees_no += nr_angajati - 1;
    
    /*daca nodul de promovat nu are niciun angajat,
    atunci fiecare nod din dreapta sa va fi mutat la stanga cu o unitate*/
    if (nr_angajati == 0){
        for (int i = index; i < t->manager->direct_employees_no; i++){
            t->manager->team[i] = t->manager->team[i + 1];
        }
        if (t->manager->direct_employees_no == 0){
            free(t->manager->team);
            t->manager->team = NULL;
        }
        qsort(t->manager->team, t->manager->direct_employees_no, sizeof(TreeNode *), comparare);

        //promovam angajatul
        t->manager = t->manager->manager;
        t->manager->direct_employees_no += 1;
        t->manager->team[t->manager->direct_employees_no - 1] = t;
        qsort(t->manager->team, t->manager->direct_employees_no, sizeof(TreeNode *), comparare);

        return tree;
    }

    /*daca nodul de promovat are un singur angajat,
    facem legatura de la managerul nodului de promovat la
    angajatul nodului de promovat*/
    if (nr_angajati == 1){
        t->manager->team[index] = t->team[0];
        t->team[0]->manager = t->manager;
        qsort(t->manager->team, t->manager->direct_employees_no, sizeof(TreeNode *), comparare);
        
        //promovam angajatul
        free(t->team);
        t->team = NULL;
        t->direct_employees_no = 0;
        t->manager = t->manager->manager;
        t->manager->direct_employees_no += 1;
        t->manager->team[t->manager->direct_employees_no - 1] = t;
        qsort(t->manager->team, t->manager->direct_employees_no, sizeof(TreeNode *), comparare);

        return tree;
    }

    /* daca nodul de promovat are 'n' angajati, atunci nodurile din
    dreapta sa vor fi mutate cu n - 1 unitati la dreapta*/
    int i = t->manager->direct_employees_no - 1, j = 0;
    while (j < nr_angajati_manager - 1 - index){
        t->manager->team[i] = t->manager->team[i - (nr_angajati - 1)];
        i--;
        j++;
    }

    i = 0;
    //managerul nodului de promovat va prelua noii angajati
    while(i < nr_angajati){
        t->manager->team[index] = t->team[i];
        t->team[i]->manager = t->manager;
        index++;
        i++;
    }
    qsort(t->manager->team, t->manager->direct_employees_no, sizeof(TreeNode *), comparare);

    //promovam angajatul
    free(t->team);
    t->team = NULL;
    t->direct_employees_no = 0;
    t->manager = t->manager->manager;
    t->manager->direct_employees_no += 1;
    t->manager->team[t->manager->direct_employees_no - 1] = t;
    qsort(t->manager->team, t->manager->direct_employees_no, sizeof(TreeNode *), comparare);
    
    return tree;
}

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name) {
    Tree employee = NULL, new_manager = NULL;
    int index;

    get_node(tree, employee_name, &employee);
    get_node(tree, new_manager_name, &new_manager);
    if (new_manager == NULL || employee == NULL || employee->manager == NULL || employee->manager == new_manager){
        return tree;
    }
    
    for (int i = 0; i < employee->manager->direct_employees_no; i++){
        if (employee->manager->team[i] == employee){
            index = i;
        }
    }


    int nr_angajati = employee->direct_employees_no;
    int nr_angajati_manager = employee->manager->direct_employees_no;
    employee->manager->direct_employees_no += nr_angajati - 1;

    /*daca nodul de mutat nu are niciun angajat,
    atunci fiecare nod din dreapta sa va fi mutat la stanga cu o unitate*/
    if (nr_angajati == 0){
        for (int i = index; i < employee->manager->direct_employees_no; i++){
            employee->manager->team[i] = employee->manager->team[i + 1];
        }
        if (employee->manager->direct_employees_no == 0){
            free(employee->manager->team);        
            employee->manager->team = NULL; 
        }
        qsort(employee->manager->team, employee->manager->direct_employees_no, sizeof(TreeNode *), comparare);
        
        
        //mutam angajatul
        employee->manager = new_manager;
        new_manager->direct_employees_no += 1;
        if (new_manager->team == NULL){
            new_manager->team =(TreeNode **)calloc(MAX_NO_EMPLOYEES, sizeof(TreeNode *));
        }
        new_manager->team[new_manager->direct_employees_no - 1] = employee;
        qsort(new_manager->team, new_manager->direct_employees_no, sizeof(TreeNode *), comparare);

        return tree;
    }

    /*daca nodul de mutat are un singur angajat,
    facem legatura de la managerul nodului de mutat la
    angajatul nodului de mutat*/
    if (nr_angajati == 1){
        employee->manager->team[index] = employee->team[0];
        employee->team[0]->manager = employee->manager;
        qsort(employee->manager->team, employee->manager->direct_employees_no, sizeof(TreeNode *), comparare);
        
        //mutam angajatul
        free(employee->team);
        employee->team = NULL;
        employee->direct_employees_no = 0;
        employee->manager = new_manager;
        new_manager->direct_employees_no += 1;
        if (new_manager->team == NULL){
            new_manager->team =(TreeNode **)calloc(MAX_NO_EMPLOYEES, sizeof(TreeNode *));
        }
        new_manager->team[new_manager->direct_employees_no - 1] = employee;
        qsort(new_manager->team, new_manager->direct_employees_no, sizeof(TreeNode *), comparare);

        return tree;
    }

    /* daca nodul de mutat are 'n' angajati, atunci nodurile din
    dreapta sa vor fi mutate cu n - 1 unitati la dreapta*/
    int i = employee->manager->direct_employees_no - 1, j = 0;
    while (j < nr_angajati_manager - 1 - index){
        employee->manager->team[i] = employee->manager->team[i - (nr_angajati - 1)];
        i--;
        j++;
    }

    i = 0;
    //managerul nodului de mutat va prelua noii angajati
    while(i < nr_angajati){
        employee->manager->team[index] = employee->team[i];
        employee->team[i]->manager = employee->manager;
        index++;
        i++;
    }
    qsort(employee->manager->team, employee->manager->direct_employees_no, sizeof(TreeNode *), comparare);

    //mutam angajatul
    free(employee->team);
    employee->team = NULL;
    employee->direct_employees_no = 0;
    employee->manager = new_manager;
    new_manager->direct_employees_no += 1;
    if (new_manager->team == NULL){
            new_manager->team =(TreeNode **)calloc(MAX_NO_EMPLOYEES, sizeof(TreeNode *));
    }
    new_manager->team[new_manager->direct_employees_no - 1] = employee;
    qsort(new_manager->team, new_manager->direct_employees_no, sizeof(TreeNode *), comparare);
    
    return tree;

}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name) {
    Tree employee = NULL, new_manager = NULL;
    int index;

    get_node(tree, employee_name, &employee);
    get_node(tree, new_manager_name, &new_manager);
    if (new_manager == NULL || employee == NULL || employee->manager == NULL || employee->manager == new_manager){
        return tree;
    }

    for (int i = 0; i < employee->manager->direct_employees_no; i++){
        if (employee->manager->team[i] == employee){
            index = i;
        }
    }

    /*daca managerul angajatului din varful echipei care trebuie mutate
    coincide cu managerul angajatului sub care trebuie plasat*/
    if (employee->manager == new_manager->manager){
        if (employee->manager->team[index + 1] != NULL){
            employee->manager = new_manager;
            new_manager->direct_employees_no += 1;
            if (new_manager->team == NULL){
                new_manager->team =(TreeNode **)calloc(MAX_NO_EMPLOYEES, sizeof(TreeNode *));
            }
            new_manager->team[new_manager->direct_employees_no - 1] = employee;
            qsort(new_manager->team, new_manager->direct_employees_no, sizeof(TreeNode *), comparare);

            for (int i = index; i < employee->manager->manager->direct_employees_no; i++){
                employee->manager->manager->team[i] = employee->manager->manager->team[i + 1];
            }
            employee->manager->manager->direct_employees_no -= 1;
            return tree;
        }
        else{
            employee->manager->direct_employees_no -= 1;
            employee->manager->team[index] = NULL;

            employee->manager = new_manager;
            new_manager->direct_employees_no += 1;
            if (new_manager->team == NULL){
                new_manager->team =(TreeNode **)calloc(MAX_NO_EMPLOYEES, sizeof(TreeNode *));
            }
            new_manager->team[new_manager->direct_employees_no - 1] = employee;
            qsort(new_manager->team, new_manager->direct_employees_no, sizeof(TreeNode *), comparare);

            return tree;
        }

    }
    /*daca managerul echipei care trebuie mutate are in dreapta sa colegi,
    atunci colegii vor fi mutati la stanga cu o unitate*/
    if (employee->manager->team[index + 1] != NULL){
        for (int i = index; i < employee->manager->direct_employees_no; i++){
            employee->manager->team[i] = employee->manager->team[i + 1];
        }
        employee->manager->direct_employees_no -= 1;
        if (employee->manager->direct_employees_no == 0){
            free(employee->manager->team);        
            employee->manager->team = NULL; 
        }
        qsort(employee->manager->team, employee->manager->direct_employees_no, sizeof(TreeNode *), comparare);

        //mutam echipa
        employee->manager = new_manager;
        new_manager->direct_employees_no += 1;
        if (new_manager->team == NULL){
            new_manager->team =(TreeNode **)calloc(MAX_NO_EMPLOYEES, sizeof(TreeNode *));
        }
        new_manager->team[new_manager->direct_employees_no - 1] = employee;
        qsort(new_manager->team, new_manager->direct_employees_no, sizeof(TreeNode *), comparare);

        return tree;

    }
    else{
        employee->manager->direct_employees_no -= 1;
    
        //mutam echipa
        employee->manager = new_manager;
        new_manager->direct_employees_no += 1;
        if (new_manager->team == NULL){
            new_manager->team =(TreeNode **)calloc(MAX_NO_EMPLOYEES, sizeof(TreeNode *));
        }
        new_manager->team[new_manager->direct_employees_no - 1] = employee;
        qsort(new_manager->team, new_manager->direct_employees_no, sizeof(TreeNode *), comparare);

        return tree;
    }
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire_team(Tree tree, char *employee_name) {
    Tree employee = NULL;
    int index;
    get_node(tree, employee_name, &employee);

    if (employee == NULL || employee->manager == NULL){
        return tree;
    }

    for (int i = 0; i < employee->manager->direct_employees_no; i++){
        if (employee->manager->team[i] == employee){
            index = i;
        }
    }

    /*daca managerul echipei care trebuie sterse are in dreapta sa colegi,
    atunci colegii vor fi mutati la stanga cu o unitate*/
    if (employee->manager->team[index + 1] != NULL){
        for (int i = index; i < employee->manager->direct_employees_no; i++){
            employee->manager->team[i] = employee->manager->team[i + 1];
        }
        employee->manager->direct_employees_no -= 1;
        if (employee->manager->direct_employees_no == 0){
            free(employee->manager->team);        
            employee->manager->team = NULL; 
        }
        qsort(employee->manager->team, employee->manager->direct_employees_no, sizeof(TreeNode *), comparare);
    }
    else{
        employee->manager->direct_employees_no -= 1;
    }


    destroy_tree(employee);
    return tree;
}

void parcurgere(Tree tree, char *vector[], int *k){
    if (tree == NULL){
        return;
    }
    
    vector[*k] = tree->name;
    *k = *k + 1;
    if (tree->team != NULL){ 
        for (int i = 0; i < tree->direct_employees_no; i++){
            parcurgere(tree->team[i], vector, k);
        }
    }
}

int comp_char(const void* p1, const void* p2){
    char **str1 = (char **)p1;
    char **str2 = (char **)p2;
    return strcmp(*str1, *str2);
}

/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */
void get_employees_by_manager(FILE *f, Tree tree, char *employee_name) {
    Tree employee = NULL;
    get_node(tree, employee_name, &employee);

    if(employee == NULL){
       fprintf (f, "\n");
    }

    char *vector[MAX_NO_EMPLOYEES] = {0};
    int i = 0, k = 0;

    /*functie care retine intr-un vector toti angajatii sub
    conducerea unui angajat */
    parcurgere(employee, vector, &k);
    qsort(vector, k, sizeof(char *), comp_char);

    for (i = 0; i < k; i++){
       fprintf (f, "%s ", vector[i]);
       if (i == k - 1){
           fprintf (f, "\n");
       }
    }
}

void get_level_node(Tree tree, int level, int *index, Tree *node){
    if (tree == NULL){
        return;
    }
    if (*index == level){
        *node = tree;
        return;
    }
    *index = *index + 1;
    if (tree->team != NULL){
        for (int i = 0; i < tree->direct_employees_no; i++){
            get_level_node(tree->team[i], level, index, node);
        }
    }
    *index -= 1;
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */
void get_employees_by_level(FILE *f, Tree tree, int level) {
    int i = 0, k = 0, j = 0, count, index;
    char *vector[MAX_NO_EMPLOYEES];
    Tree level_node = NULL;

    if (level < 0){
        fprintf(f, "\n");
        return;
    }
    if(level == 0){
        fprintf(f, "%s \n", tree->name);
        return;
    }
    if (level == 1){
        for (i = 0; i < tree->direct_employees_no; i++){
            vector[i] = tree->team[i]->name;
        }
        count = i;
        qsort(vector, count, sizeof(char *), comp_char);

        for (i = 0; i < count; i++){
            fprintf(f, "%s ", vector[i]);
        }
        fprintf(f, "\n");
        return;
    }

    index = -1;

    //functie care obtine un nod de pe nivelul dat ca parametru
    get_level_node(tree, level - 1, &index, &level_node);

    if (level_node != NULL){
        level_node = level_node->manager->manager;
        j = 0, k = 0;
        for (i = 0; i < level_node->direct_employees_no; i++){
            if (level_node->team[i] != NULL){
                for (j = 0; j < level_node->team[i]->direct_employees_no; j++){
                    vector[k++] = level_node->team[i]->team[j]->name;
                }
            }
        }

        count = k;
        qsort(vector, count, sizeof(char *), comp_char);

        for (i = 0; i < count; i++){
            fprintf(f, "%s ", vector[i]);
        }
        fprintf(f, "\n");
    }
    else{
        fprintf(f, "\n");
    }
}

void employees_counter(Tree tree, Tree team[], int *k){
    if (tree == NULL){
        return;
    }
    team[*k] = tree;
    *k = *k + 1;
    if (tree->team != NULL){
        for (int i = 0; i < tree->direct_employees_no; i++){
            employees_counter(tree->team[i], team, k);
        }
    }
}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void get_best_manager(FILE *f, Tree tree) {
    Tree team[MAX_NO_EMPLOYEES], best_managers[MAX_NO_EMPLOYEES];
    int k = 0, max = 0, j = 0;

    employees_counter(tree, team, &k);
    for (int i = 0; i < k; i++){
        if (team[i]->direct_employees_no > max){
            //maximul de angajati pe care ii are un manager
            max = team[i]->direct_employees_no;
        }
    }

    //retinem managerii care au maximul de angajati
    for (int i = 0; i < k; i++){
        if (team[i]->direct_employees_no == max){
            best_managers[j++] = team[i];
        }
    }
    qsort(best_managers, j, sizeof(TreeNode *), comparare);

    for (int i = 0; i < j; i++){
        fprintf(f, "%s ", best_managers[i]->name);
    }
    fprintf(f, "\n");
}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
Tree reorganize(Tree tree, char *employee_name) {
    return tree;
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void preorder_traversal_aux(FILE *f, Tree tree){
    if (tree == NULL){
        return;
    }
    if (tree->manager == NULL){
        fprintf(f, "%s ", tree->name);
    }
    if (tree->team != NULL && tree->manager != NULL){
        fprintf (f, "%s-%s ", tree->name, tree->manager->name);
        
    }
    if (tree->team != NULL){
        for (int i = 0; i < tree->direct_employees_no; i++){
            preorder_traversal_aux(f, tree->team[i]);
        }
    }
    if (tree->team == NULL && tree->manager != NULL){
        fprintf (f, "%s-%s ", tree->name, tree->manager->name);
    }
}

void preorder_traversal(FILE *f, Tree tree){
    preorder_traversal_aux(f, tree);
    fprintf(f, "\n");
}

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */
void destroy_tree(Tree tree) {
    if (tree == NULL){
		return;
	}
    Tree aux = tree;
    if (aux->team != NULL){
        for (int i = 0; i < aux->direct_employees_no; i++){
            destroy_tree(aux->team[i]);
        }
    }

    free(aux->team);
    aux->team = NULL;
    free(aux);
    aux = NULL;
}

