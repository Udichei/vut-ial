/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {

    (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

    if(tree != NULL)
    {
        if(tree->key == key)
        {
            *value = tree->value;
            return true;
        } else if(key < tree->key)
        {
            *value = bst_search(tree->left, key, value);
            return true;
        } else
        {
            *value = bst_search(tree->right, key, value);
            return true;
        }

    } else
    {
    }


  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

    if(*tree != NULL)
    {
        if(key < (*tree)->key)
        {
            bst_insert(&((*tree)->left), key, value);
        } else if(key > (*tree)->key)
        {
            bst_insert(&((*tree)->right), key, value);
        } else
        {
            (*tree)->value = value;
        }

    } else
    {
        bst_node_t *new = (bst_node_t *) malloc(sizeof (bst_node_t));

        if(new)
        {
            new->key = key;
            new->value = value;
            new->right = NULL;
            new->left = NULL;

            *tree = new;
        }


    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

    if((*tree) != NULL)
    {
        if((*tree)->right == NULL)
        {
            (*target).key = (*tree)->key;
            (*target).value = (*tree)->value;

            free(*tree);

            if((*tree)->left == NULL)
            {
                (*tree) = NULL;
            } else
            {
                (*tree) = (*tree)->left;
            }

        }else
        {
            bst_replace_by_rightmost(target, &((*tree)->right));
        }
    }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    if((*tree) != NULL)
    {
        if(key < (*tree)->key)                       // klic je v levem podstromu
        {
            bst_delete(&((*tree)->left), key);
        } else if(key > (*tree)->key)                // klic je v pravem podstromu
        {
            bst_delete(&((*tree)->right), key);
        } else
        {
            if(((*tree)->right == NULL) && (*tree)->left == NULL)              // nema zadneho syna
            {
                free(*tree);
                *tree = NULL;
            } else if(((*tree)->right != NULL) && (*tree)->left != NULL)       // ma oba podstromy
            {
                bst_replace_by_rightmost(*tree, &((*tree)->left));
            } else
            {
                bst_node_t *syn;

                if((*tree)->left == NULL)                                      // nema leveho syna
                {
                    syn = (*tree)->right;
                } else                                                         // nema praveho syna
                {
                    syn = (*tree)->left;
                }

                free(*tree);

                (*tree) = syn;

            }
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {

    if((*tree) != NULL)
    {
        if(((*tree)->right == NULL) && (*tree)->left == NULL)             // nema zadneho syna
        {
            free(*tree);
            *tree = NULL;
        }else if(((*tree)->right != NULL) && (*tree)->left != NULL)       // ma oba podstromy
        {
            bst_dispose(&((*tree))->left);
            bst_dispose(&((*tree))->right);
            bst_dispose(tree);
        } else                                                            // jen jeden podstrom
        {
            if((*tree)->left == NULL)                                      // nema leveho syna
            {
                bst_dispose(&((*tree))->right);
            } else                                                         // nema praveho syna
            {
                bst_dispose(&((*tree))->left);
            }
        }
    }


}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {

    if(tree != NULL)
    {
        bst_print_node(tree);

        bst_preorder(tree->left);
        bst_preorder(tree->right);
    }

}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {

    if(tree != NULL)
    {
        bst_inorder(tree->left);
        bst_print_node(tree);
        bst_inorder(tree->right);
    }

}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {

    if(tree != NULL)
    {
        bst_postorder(tree->left);
        bst_postorder(tree->right);
        bst_print_node(tree);
    }
}
