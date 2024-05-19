#ifndef LISTSTR_H
#define LISTSTR_H

    #include "Settings.h"

    /// @brief Structure représentant un noeud d'une liste de chaînes de caractères.
    typedef struct ListStrNode {
        /// @brief Pointeur vers le noeud précédent.
        struct ListStrNode* prev;

        /// @brief Pointeur vers le noeud suivant.
        struct ListStrNode* next;

        /// @brief Valeur du noeud (une chaîne de caractères).
        char* value;
    } ListStrNode;

    /// @brief Structure représentant une liste doublement chaînée de chaînes de caractères.
    typedef struct ListStr {
        /// @brief Sentinelle.
        ListStrNode sentinel;

        /// @brief Le nombre de noeuds de la liste.
        int nodeCount;
    } ListStr;

    /// @brief Itérateur sur une liste de chaînes de caractères.
    typedef struct ListStrIter {
        ListStrNode* sentinel;

        ListStrNode* current;
    } ListStrIter;

    /// @brief Crée une liste vide de chaînes de caractères.
    /// @return La liste créée.
    ListStr* ListStr_create();

    /// @brief Détruit une liste de chaînes de caractères.
    /// @param list la liste.
    void ListStr_destroy(ListStr* list);

    /// @brief Renvoie la taille d'une liste de chaînes de caractères.
    /// @param list la liste.
    /// @return La taille de la liste.
    int ListStr_size(ListStr* list);

    /// @brief Indique si une liste de chaînes de caractères est vide.
    /// @param list la liste.
    /// @return true si la liste est vide, false sinon.
    bool ListStr_isEmpty(ListStr* list);

    /// @brief Affiche une liste de chaînes de caractères.
    /// @param list la liste.
    void ListStr_print(ListStr* list);

    /// @brief Renvoie la première chaîne de caractères d'une liste.
    /// L'utilisateur doit au préalable vérifier que la liste n'est pas vide.
    /// @param list la liste.
    /// @return La première chaîne de caractères de la liste.
    char* ListStr_getFirst(ListStr* list);

    /// @brief Renvoie la dernière chaîne de caractères d'une liste.
    /// L'utilisateur doit au préalable vérifier que la liste n'est pas vide.
    /// @param list la liste.
    /// @return La dernière chaîne de caractères de la liste.
    char* ListStr_getLast(ListStr* list);

    /// @brief Insert une chaîne de caractères au début d'une liste.
    /// @param list la liste.
    /// @param value la chaîne de caractères à insérer.
    void ListStr_insertFirst(ListStr* list, const char* value);

    /// @brief Insert une chaîne de caractères à la fin d'une liste.
    /// @param list la liste.
    /// @param value la chaîne de caractères à insérer.
    void ListStr_insertLast(ListStr* list, const char* value);

    /// @brief Insert un noeud dans une liste de chaînes de caractères après un noeud de référence.
    /// @param list la liste.
    /// @param ref le noeud de référence.
    /// @param node le noeud à insérer.
    void ListStr_insertNodeAfter(ListStr* list, ListStrNode* ref, ListStrNode* node);

    /// @brief Supprime et renvoie la première chaîne de caractères d'une liste.
    /// L'utilisateur doit au préalable vérifier que la liste n'est pas vide.
    /// @param list la liste.
    /// @return La première chaîne de caractères de la liste.
    char* ListStr_popFirst(ListStr* list);

    /// @brief Supprime et renvoie la dernière chaîne de caractères d'une liste.
    /// L'utilisateur doit au préalable vérifier que la liste n'est pas vide.
    /// @param list la liste.
    /// @return La dernière chaîne de caractères de la liste.
    char* ListStr_popLast(ListStr* list);

    /// @brief Retire un noeud d'une liste de chaînes de caractères.
    /// Le noeud n'est pas détruit, il est juste extrait de la liste.
    /// @param list la liste.
    /// @param node le noeud à retirer.
    void ListStr_popNode(ListStr* list, ListStrNode* node);

    /// @brief Copie une liste de chaînes de caractères avec tous ses noeuds.
    /// @param list la liste à copier.
    /// @return Une copie profonde de la liste.
    ListStr* ListStr_copy(ListStr* list);

    /// @brief Concatène deux listes de chaînes de caractères.
    /// Les noeuds de la deuxième liste sont ajoutés en fin de la première liste.
    /// La seconde liste devient vide après l'appel à cette fonction.
    /// Cette méthode s'exécute en temps constant.
    /// @param list1 la première liste.
    /// @param list2 la seconde liste.
    void ListStr_concatenate(ListStr* list1, ListStr* list2);

    /// @brief Crée un itérateur sur une liste de chaînes de caractères.
    /// Si la liste n'est pas vide, l'itérateur pointe vers le premier élément.
    /// @param list la liste à parcourir.
    /// @return Un itérateur sur la liste de chaînes de caractères.
    ListStrIter* ListStrIter_create(ListStr* list);

    /// @brief Détruit un itérateur sur une liste de chaînes de caractères.
    /// @param iter l'itérateur.
    void ListStrIter_destroy(ListStrIter* iter);

    /// @brief Renvoie l'élément courant de l'itérateur.
    /// L'utilisateur doit vérifier que l'itérateur est valide avant d'appeler cette méthode.
    /// @param iter l'itérateur.
    /// @return L'élément courant de l'itérateur.
    char* ListStrIter_get(ListStrIter* iter);

    /// @brief Fait avancer l'itérateur sur vers l'élément suivant.
    /// L'utilisateur doit vérifier que l'itérateur est valide avant d'appeler cette méthode.
    /// @param iter l'itérateur.
    void ListStrIter_next(ListStrIter* iter);

    /// @brief Indique si un itérateur pointe sur un élément valide.
    /// @param iter l'itérateur.
    /// @return true si l'itérateur pointe sur un élément valide, false sinon.
    bool ListStrIter_isValid(ListStrIter* iter);

#endif
