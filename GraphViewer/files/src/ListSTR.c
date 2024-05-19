#include "ListSTR.h"

/// @brief Cr�e une liste vide.
/// @return La liste cr��e.
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

/// @brief D�truit une liste de cha�nes de caract�res.
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

/// @brief Renvoie la taille d'une liste de cha�nes de caract�res.
/// @param list la liste.
/// @return La taille de la liste.
int ListStr_size(ListStr* list) {
    return list->nodeCount;
}

/// @brief Indique si une liste de cha�nes de caract�res est vide.
/// @param list la liste.
/// @return true si la liste est vide, false sinon.
bool ListStr_isEmpty(ListStr* list) {
    return list->nodeCount == 0;
}

/// @brief Affiche une liste de cha�nes de caract�res.
/// @param list la liste.
void ListStr_print(ListStr* list) {
    ListStrNode* node = list->sentinel.next;
    while (node != &list->sentinel) {
        printf("%s ", node->value);
        node = node->next;
    }
    printf("\n");
}

/// @brief Renvoie la premi�re cha�ne de caract�res d'une liste.
/// @param list la liste.
/// @return La premi�re cha�ne de caract�res de la liste.
char* ListStr_getFirst(ListStr* list) {
    return list->sentinel.next->value;
}

/// @brief Renvoie la derni�re cha�ne de caract�res d'une liste.
/// @param list la liste.
/// @return La derni�re cha�ne de caract�res de la liste.
char* ListStr_getLast(ListStr* list) {
    return list->sentinel.prev->value;
}

/// @brief Insert une cha�ne de caract�res au d�but d'une liste.
/// @param list la liste.
/// @param value la cha�ne de caract�res � ins�rer.
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

/// @brief Insert une cha�ne de caract�res � la fin d'une liste.
/// @param list la liste.
/// @param value la cha�ne de caract�res � ins�rer.
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

/// @brief Insert un noeud dans une liste de cha�nes de caract�res apr�s un noeud de r�f�rence.
/// @param list la liste.
/// @param ref le noeud de r�f�rence.
/// @param node le noeud � ins�rer.
void ListStr_insertNodeAfter(ListStr* list, ListStrNode* ref, ListStrNode* node) {
    node->next = ref->next;
    node->prev = ref;
    ref->next->prev = node;
    ref->next = node;
    list->nodeCount++;
}

/// @brief Supprime et renvoie la premi�re cha�ne de caract�res d'une liste.
/// @param list la liste.
/// @return La premi�re cha�ne de caract�res de la liste.
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

/// @brief Supprime et renvoie la derni�re cha�ne de caract�res d'une liste.
/// @param list la liste.
/// @return La derni�re cha�ne de caract�res de la liste.
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

/// @brief Retire un noeud d'une liste de cha�nes de caract�res.
/// @param list la liste.
/// @param node le noeud � retirer.
void ListStr_popNode(ListStr* list, ListStrNode* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
    list->nodeCount--;
}

/// @brief Teste la pr�sence d'un �l�ment dans une liste.
/// @param list la liste.
/// @param element l'�l�ment recherch�.
/// @return true si l'�l�ment est pr�sent dans la liste,
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
/// @param list la liste � copier.
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

/// @brief Concat�ne deux listes.
/// Les noeuds de la deuxi�me liste sont ajout�s en fin de la premi�re liste.
/// La seconde liste devient vide apr�s l'appel � cette fonction.
/// Cette m�thode s'ex�cute en temps constant.
/// @param list1 la premi�re liste.
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

/// @brief Cr�e un it�rateur sur une liste de cha�nes de caract�res.
/// @param list la liste � parcourir.
/// @return Un it�rateur sur la liste de cha�nes de caract�res.
ListStrIter* ListStrIter_create(ListStr* list) {
    ListStrIter* iter = (ListStrIter*)calloc(1, sizeof(ListStrIter));
    AssertNew(iter);

    iter->sentinel = &list->sentinel;
    iter->current = list->sentinel.next;
    return iter;
}

/// @brief D�truit un it�rateur sur une liste de cha�nes de caract�res.
/// @param iter l'it�rateur.
void ListStrIter_destroy(ListStrIter* iter) {
    free(iter);
}

/// @brief Renvoie la cha�ne de caract�res courante de l'it�rateur.
/// L'utilisateur doit v�rifier que l'it�rateur est valide avant d'appeler cette m�thode.
/// @param iter l'it�rateur.
/// @return La cha�ne de caract�res courante de l'it�rateur.
char* ListStrIter_get(ListStrIter* iter) {
    if (!iter || iter->current == iter->sentinel) {
        return NULL;
    }
    return iter->current->value;
}

/// @brief Fait avancer l'it�rateur vers l'�l�ment suivant.
/// L'utilisateur doit v�rifier que l'it�rateur est valide avant d'appeler cette m�thode.
/// @param iter l'it�rateur.
void ListStrIter_next(ListStrIter* iter) {
    if (iter && iter->current != iter->sentinel) {
        iter->current = iter->current->next;
    }
}

/// @brief Indique si un it�rateur pointe sur un �l�ment valide.
/// @param iter l'it�rateur.
/// @return true si l'it�rateur pointe sur un �l�ment valide, false sinon.
bool ListStrIter_isValid(ListStrIter* iter) {
    assert(iter);
    return (iter->current != NULL && iter->current != iter->sentinel);
}
