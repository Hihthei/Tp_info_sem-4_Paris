#ifndef LISTSTR_H
#define LISTSTR_H

    #include "Settings.h"

    /// @brief Structure repr�sentant un noeud d'une liste de cha�nes de caract�res.
    typedef struct ListStrNode {
        /// @brief Pointeur vers le noeud pr�c�dent.
        struct ListStrNode* prev;

        /// @brief Pointeur vers le noeud suivant.
        struct ListStrNode* next;

        /// @brief Valeur du noeud (une cha�ne de caract�res).
        char* value;
    } ListStrNode;

    /// @brief Structure repr�sentant une liste doublement cha�n�e de cha�nes de caract�res.
    typedef struct ListStr {
        /// @brief Sentinelle.
        ListStrNode sentinel;

        /// @brief Le nombre de noeuds de la liste.
        int nodeCount;
    } ListStr;

    /// @brief It�rateur sur une liste de cha�nes de caract�res.
    typedef struct ListStrIter {
        ListStrNode* sentinel;

        ListStrNode* current;
    } ListStrIter;

    /// @brief Cr�e une liste vide de cha�nes de caract�res.
    /// @return La liste cr��e.
    ListStr* ListStr_create();

    /// @brief D�truit une liste de cha�nes de caract�res.
    /// @param list la liste.
    void ListStr_destroy(ListStr* list);

    /// @brief Renvoie la taille d'une liste de cha�nes de caract�res.
    /// @param list la liste.
    /// @return La taille de la liste.
    int ListStr_size(ListStr* list);

    /// @brief Indique si une liste de cha�nes de caract�res est vide.
    /// @param list la liste.
    /// @return true si la liste est vide, false sinon.
    bool ListStr_isEmpty(ListStr* list);

    /// @brief Affiche une liste de cha�nes de caract�res.
    /// @param list la liste.
    void ListStr_print(ListStr* list);

    /// @brief Renvoie la premi�re cha�ne de caract�res d'une liste.
    /// L'utilisateur doit au pr�alable v�rifier que la liste n'est pas vide.
    /// @param list la liste.
    /// @return La premi�re cha�ne de caract�res de la liste.
    char* ListStr_getFirst(ListStr* list);

    /// @brief Renvoie la derni�re cha�ne de caract�res d'une liste.
    /// L'utilisateur doit au pr�alable v�rifier que la liste n'est pas vide.
    /// @param list la liste.
    /// @return La derni�re cha�ne de caract�res de la liste.
    char* ListStr_getLast(ListStr* list);

    /// @brief Insert une cha�ne de caract�res au d�but d'une liste.
    /// @param list la liste.
    /// @param value la cha�ne de caract�res � ins�rer.
    void ListStr_insertFirst(ListStr* list, const char* value);

    /// @brief Insert une cha�ne de caract�res � la fin d'une liste.
    /// @param list la liste.
    /// @param value la cha�ne de caract�res � ins�rer.
    void ListStr_insertLast(ListStr* list, const char* value);

    /// @brief Insert un noeud dans une liste de cha�nes de caract�res apr�s un noeud de r�f�rence.
    /// @param list la liste.
    /// @param ref le noeud de r�f�rence.
    /// @param node le noeud � ins�rer.
    void ListStr_insertNodeAfter(ListStr* list, ListStrNode* ref, ListStrNode* node);

    /// @brief Supprime et renvoie la premi�re cha�ne de caract�res d'une liste.
    /// L'utilisateur doit au pr�alable v�rifier que la liste n'est pas vide.
    /// @param list la liste.
    /// @return La premi�re cha�ne de caract�res de la liste.
    char* ListStr_popFirst(ListStr* list);

    /// @brief Supprime et renvoie la derni�re cha�ne de caract�res d'une liste.
    /// L'utilisateur doit au pr�alable v�rifier que la liste n'est pas vide.
    /// @param list la liste.
    /// @return La derni�re cha�ne de caract�res de la liste.
    char* ListStr_popLast(ListStr* list);

    /// @brief Retire un noeud d'une liste de cha�nes de caract�res.
    /// Le noeud n'est pas d�truit, il est juste extrait de la liste.
    /// @param list la liste.
    /// @param node le noeud � retirer.
    void ListStr_popNode(ListStr* list, ListStrNode* node);

    /// @brief Copie une liste de cha�nes de caract�res avec tous ses noeuds.
    /// @param list la liste � copier.
    /// @return Une copie profonde de la liste.
    ListStr* ListStr_copy(ListStr* list);

    /// @brief Concat�ne deux listes de cha�nes de caract�res.
    /// Les noeuds de la deuxi�me liste sont ajout�s en fin de la premi�re liste.
    /// La seconde liste devient vide apr�s l'appel � cette fonction.
    /// Cette m�thode s'ex�cute en temps constant.
    /// @param list1 la premi�re liste.
    /// @param list2 la seconde liste.
    void ListStr_concatenate(ListStr* list1, ListStr* list2);

    /// @brief Cr�e un it�rateur sur une liste de cha�nes de caract�res.
    /// Si la liste n'est pas vide, l'it�rateur pointe vers le premier �l�ment.
    /// @param list la liste � parcourir.
    /// @return Un it�rateur sur la liste de cha�nes de caract�res.
    ListStrIter* ListStrIter_create(ListStr* list);

    /// @brief D�truit un it�rateur sur une liste de cha�nes de caract�res.
    /// @param iter l'it�rateur.
    void ListStrIter_destroy(ListStrIter* iter);

    /// @brief Renvoie l'�l�ment courant de l'it�rateur.
    /// L'utilisateur doit v�rifier que l'it�rateur est valide avant d'appeler cette m�thode.
    /// @param iter l'it�rateur.
    /// @return L'�l�ment courant de l'it�rateur.
    char* ListStrIter_get(ListStrIter* iter);

    /// @brief Fait avancer l'it�rateur sur vers l'�l�ment suivant.
    /// L'utilisateur doit v�rifier que l'it�rateur est valide avant d'appeler cette m�thode.
    /// @param iter l'it�rateur.
    void ListStrIter_next(ListStrIter* iter);

    /// @brief Indique si un it�rateur pointe sur un �l�ment valide.
    /// @param iter l'it�rateur.
    /// @return true si l'it�rateur pointe sur un �l�ment valide, false sinon.
    bool ListStrIter_isValid(ListStrIter* iter);

#endif
