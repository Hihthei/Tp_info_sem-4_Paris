#include "ListSTR.h"

/// @brief Crée une liste vide.
/// @return La liste créée.
ListStr* ListStr_create() {
    ListStr* list = (ListStr*)malloc(sizeof(ListStr));
    if (!list) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    list->sentinel.prev = &list->sentinel;
    list->sentinel.next = &list->sentinel;
    list->sentinel.value = NULL;
    list->nodeCount = 0;
    return list;
}

/// @brief Détruit une liste de chaînes de caractères.
/// @param list la liste.
void ListStr_destroy(ListStr* list) {
    ListStrNode* node = list->sentinel.next;
    while (node != &list->sentinel) {
        ListStrNode* nextNode = node->next;
        free(node->value);
        free(node);
        node = nextNode;
    }
    free(list);
}

/// @brief Renvoie la taille d'une liste de chaînes de caractères.
/// @param list la liste.
/// @return La taille de la liste.
int ListStr_size(ListStr* list) {
    return list->nodeCount;
}

/// @brief Indique si une liste de chaînes de caractères est vide.
/// @param list la liste.
/// @return true si la liste est vide, false sinon.
bool ListStr_isEmpty(ListStr* list) {
    return list->nodeCount == 0;
}

/// @brief Affiche une liste de chaînes de caractères.
/// @param list la liste.
void ListStr_print(ListStr* list) {
    ListStrNode* node = list->sentinel.next;
    while (node != &list->sentinel) {
        printf("%s ", node->value);
        node = node->next;
    }
    printf("\n");
}

/// @brief Renvoie la première chaîne de caractères d'une liste.
/// @param list la liste.
/// @return La première chaîne de caractères de la liste.
char* ListStr_getFirst(ListStr* list) {
    return list->sentinel.next->value;
}

/// @brief Renvoie la dernière chaîne de caractères d'une liste.
/// @param list la liste.
/// @return La dernière chaîne de caractères de la liste.
char* ListStr_getLast(ListStr* list) {
    return list->sentinel.prev->value;
}

/// @brief Insert une chaîne de caractères au début d'une liste.
/// @param list la liste.
/// @param value la chaîne de caractères à insérer.
void ListStr_insertFirst(ListStr* list, const char* value) {
    ListStrNode* node = (ListStrNode*)malloc(sizeof(ListStrNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    node->value = _strdup(value);
    if (!node->value) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    node->prev = &list->sentinel;
    node->next = list->sentinel.next;
    list->sentinel.next->prev = node;
    list->sentinel.next = node;
    list->nodeCount++;
}

/// @brief Insert une chaîne de caractères à la fin d'une liste.
/// @param list la liste.
/// @param value la chaîne de caractères à insérer.
void ListStr_insertLast(ListStr* list, const char* value) {
    ListStrNode* node = (ListStrNode*)malloc(sizeof(ListStrNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    node->value = _strdup(value);
    if (!node->value) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    node->next = &list->sentinel;
    node->prev = list->sentinel.prev;
    list->sentinel.prev->next = node;
    list->sentinel.prev = node;
    list->nodeCount++;
}

/// @brief Insert un noeud dans une liste de chaînes de caractères après un noeud de référence.
/// @param list la liste.
/// @param ref le noeud de référence.
/// @param node le noeud à insérer.
void ListStr_insertNodeAfter(ListStr* list, ListStrNode* ref, ListStrNode* node) {
    node->next = ref->next;
    node->prev = ref;
    ref->next->prev = node;
    ref->next = node;
    list->nodeCount++;
}

/// @brief Supprime et renvoie la première chaîne de caractères d'une liste.
/// @param list la liste.
/// @return La première chaîne de caractères de la liste.
char* ListStr_popFirst(ListStr* list) {
    if (ListStr_isEmpty(list)) {
        return NULL;
    }
    ListStrNode* node = list->sentinel.next;
    node->next->prev = &list->sentinel;
    list->sentinel.next = node->next;
    char* value = node->value;
    free(node);
    list->nodeCount--;
    return value;
}

/// @brief Supprime et renvoie la dernière chaîne de caractères d'une liste.
/// @param list la liste.
/// @return La dernière chaîne de caractères de la liste.
char* ListStr_popLast(ListStr* list) {
    if (ListStr_isEmpty(list)) {
        return NULL;
    }
    ListStrNode* node = list->sentinel.prev;
    node->prev->next = &list->sentinel;
    list->sentinel.prev = node->prev;
    char* value = node->value;
    free(node);
    list->nodeCount--;
    return value;
}

/// @brief Retire un noeud d'une liste de chaînes de caractères.
/// @param list la liste.
/// @param node le noeud à retirer.
void ListStr_popNode(ListStr* list, ListStrNode* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
    list->nodeCount--;
}

/// @brief Teste la présence d'un élément dans une liste.
/// @param list la liste.
/// @param element l'élément recherché.
/// @return true si l'élément est présent dans la liste,
/// false sinon.
bool ListStr_isIn(ListStr* list, const char* element) {
    ListStrNode* node = list->sentinel.next;
    while (node != &list->sentinel) {
        if (strcmp(node->value, element) == 0) {
            return true;
        }
        node = node->next;
    }
    return false;
}

/// @brief Copie une liste avec tous ses noeuds.
/// @param list la liste à copier.
/// @return Une copie profonde de la liste.
ListStr* ListStr_copy(ListStr* list) {
    ListStr* copy = ListStr_create();
    ListStrNode* node = list->sentinel.next;
    while (node != &list->sentinel) {
        ListStr_insertLast(copy, node->value);
        node = node->next;
    }
    return copy;
}

/// @brief Concatène deux listes.
/// Les noeuds de la deuxième liste sont ajoutés en fin de la première liste.
/// La seconde liste devient vide après l'appel à cette fonction.
/// Cette méthode s'exécute en temps constant.
/// @param list1 la première liste.
/// @param list2 la seconde liste.
void ListStr_concatenate(ListStr* list1, ListStr* list2) {
    if (ListStr_isEmpty(list2)) {
        return;
    }
    list1->sentinel.prev->next = list2->sentinel.next;
    list2->sentinel.next->prev = list1->sentinel.prev;
    list2->sentinel.prev->next = &list1->sentinel;
    list1->sentinel.prev = list2->sentinel.prev;
    list1->nodeCount += list2->nodeCount;
    list2->sentinel.next = &list2->sentinel;
    list2->sentinel.prev = &list2->sentinel;
    list2->nodeCount = 0;
}

/// @brief Crée un itérateur sur une liste de chaînes de caractères.
/// @param list la liste à parcourir.
/// @return Un itérateur sur la liste de chaînes de caractères.
ListStrIter* ListStrIter_create(ListStr* list) {
    ListStrIter* iter = (ListStrIter*)calloc(1, sizeof(ListStrIter));
    AssertNew(iter);

    iter->sentinel = &list->sentinel;
    iter->current = list->sentinel.next;
    return iter;
}

/// @brief Détruit un itérateur sur une liste de chaînes de caractères.
/// @param iter l'itérateur.
void ListStrIter_destroy(ListStrIter* iter) {
    free(iter);
}

/// @brief Renvoie la chaîne de caractères courante de l'itérateur.
/// L'utilisateur doit vérifier que l'itérateur est valide avant d'appeler cette méthode.
/// @param iter l'itérateur.
/// @return La chaîne de caractères courante de l'itérateur.
char* ListStrIter_get(ListStrIter* iter) {
    if (!iter || iter->current == iter->sentinel) {
        return NULL;
    }
    return iter->current->value;
}

/// @brief Fait avancer l'itérateur vers l'élément suivant.
/// L'utilisateur doit vérifier que l'itérateur est valide avant d'appeler cette méthode.
/// @param iter l'itérateur.
void ListStrIter_next(ListStrIter* iter) {
    if (iter && iter->current != iter->sentinel) {
        iter->current = iter->current->next;
    }
}

/// @brief Indique si un itérateur pointe sur un élément valide.
/// @param iter l'itérateur.
/// @return true si l'itérateur pointe sur un élément valide, false sinon.
bool ListStrIter_isValid(ListStrIter* iter) {
    assert(iter);
    return (iter->current != NULL && iter->current != iter->sentinel);
}
